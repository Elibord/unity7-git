/*
 * Copyright 2011 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 3, as
 * published by the  Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the applicable version of the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of both the GNU Lesser General Public
 * License version 3 along with this program.  If not, see
 * <http://www.gnu.org/licenses/>
 **
 * Authored by: Nick Dedekind <nick.dedekind@canonical.com>
 *
 */

#include <pthread.h>
#include <NuxCore/Logger.h>
#include "UnityCore/GLibSource.h"
#include "UnityCore/GLibWrapper.h"
#include "ThumbnailGenerator.h"
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "TextureThumbnailProvider.h"
#include "DefaultThumbnailProvider.h"

namespace unity
{

namespace
{
  nux::logging::Logger logger("unity.thumbnailgenerator");
  ThumbnailGenerator* thumbnail_instance = nullptr;
  static unsigned int last_handle = 1;

  class ThumbnailerProcess : public Thumbnailer
  {
  public:
    ThumbnailerProcess(std::string const& name, std::string const& command_line)
    : name(name)
    , command_line(command_line)
    {}

    std::string name;
    std::string command_line;

    virtual std::string GetName() const { return name; }

    virtual bool Run(int size, std::string const& input_file, std::string& output_file, std::string& error_hint);
  };
  static std::map<std::string, std::string> thumbnail_content_map;
  static std::map<std::string, Thumbnailer::Ptr> thumbnailers_;
  static bool all_thumbnailers_initialized = false;

  pthread_mutex_t thumbnailers_mutex_ = PTHREAD_MUTEX_INITIALIZER;
}

class Thumbnail
{
public:
  typedef std::shared_ptr<Thumbnail> Ptr;

  Thumbnail(std::string const& uri, unsigned int size);
  
  virtual ~Thumbnail();

  std::string Generate(std::string& error_hint);

  unsigned int GetHandle() const;

protected:
  std::string const uri_;
  unsigned int size_;
  const unsigned int handle_;
};

class ThumbnailGeneratorImpl
{
public:
  ThumbnailGeneratorImpl(ThumbnailGenerator* parent)
  : parent_(parent)
  , thumbnails_mutex_(PTHREAD_MUTEX_INITIALIZER)
  , thumbnail_thread_is_running_(false)
  {}

  ~ThumbnailGeneratorImpl()
  {
    pthread_join(thumbnail_thread_, NULL);
  }

  void Run_();

  unsigned int GetThumbnail(std::string const& uri, int size);

  bool OnThumbnailComplete();

  static void LoadThumbnailers();
  static Thumbnailer::Ptr GetThumbnailer(std::string const& content_type, std::string& error_hint);

protected:
  ThumbnailGenerator* parent_;

  glib::Source::UniquePtr idle_;
  glib::Source::UniquePtr idle_return_;

  /* Our mutex used when accessing data shared between the main thread and the
   thumbnail thread, i.e. the thumbnail_thread_is_running flag and the
   thumbnails_to_make list. */
  pthread_mutex_t thumbnails_mutex_;

  /* A flag to indicate whether a thumbnail thread is running, so we don't
   start more than one. Lock thumbnails_mutex when accessing this. */
  volatile bool thumbnail_thread_is_running_;
  pthread_t thumbnail_thread_;

  std::queue<Thumbnail::Ptr> thumbnails_;

  struct CompleteThumbnail
  {
    unsigned int handle;
    std::string thubnail_uri;
    std::string error_hint;
  };
  std::list<CompleteThumbnail> complete_thumbnails_;
};

static void*
thumbnail_thread_start (void* data)
{
  ((ThumbnailGeneratorImpl*)data)->Run_();
  return NULL;
}

unsigned int ThumbnailGeneratorImpl::GetThumbnail(std::string const& uri, int size)
{
  pthread_mutex_lock (&thumbnails_mutex_);
  /*********************************
   * MUTEX LOCKED
   *********************************/

  if (!idle_ && thumbnail_thread_is_running_ == false)
  {
    idle_.reset(new glib::Idle([&]()
    {

      thumbnail_thread_is_running_ = true;
      pthread_create (&thumbnail_thread_, NULL, thumbnail_thread_start, this);
      idle_.reset();
      return false;
    }, glib::Source::Priority::LOW));
  }

  Thumbnail::Ptr thumb(new Thumbnail(uri, size));
  thumbnails_.push(thumb);
  unsigned int handle = thumb->GetHandle();

  pthread_mutex_unlock (&thumbnails_mutex_);
  /*********************************
   * MUTEX UNLOCKED
   *********************************/
  return handle;
}


void ThumbnailGeneratorImpl::Run_()
{
  for (;;)
  {  
    /*********************************
     * MUTEX LOCKED
     *********************************/
    pthread_mutex_lock (&thumbnails_mutex_);

    if (thumbnails_.size() == 0)
    {
      thumbnail_thread_is_running_ = FALSE;
      pthread_mutex_unlock (&thumbnails_mutex_);
      pthread_exit (NULL);
    }

    pthread_mutex_unlock (&thumbnails_mutex_);  
    /*********************************
     * MUTEX UNLOCKED
     *********************************/

    Thumbnail::Ptr thumb(thumbnails_.front());
    std::string error_hint;
    std::string uri_result = thumb->Generate(error_hint);
    thumbnails_.pop();


    /*********************************
     * MUTEX LOCKED
     *********************************/
    pthread_mutex_lock (&thumbnails_mutex_);

    CompleteThumbnail complete_thumb;
    complete_thumb.handle = thumb->GetHandle();
    complete_thumb.thubnail_uri = uri_result;
    complete_thumb.error_hint = error_hint;

    complete_thumbnails_.push_back(complete_thumb);

    if (!idle_return_)
    {
      idle_return_.reset(new glib::Idle(sigc::mem_fun(this, &ThumbnailGeneratorImpl::OnThumbnailComplete), glib::Source::Priority::LOW));
    }

    pthread_mutex_unlock (&thumbnails_mutex_);
    /*********************************
     * MUTEX UNLOCKED
     *********************************/
   }
}

bool ThumbnailGeneratorImpl::OnThumbnailComplete()
{
  for (;;)
  {
    pthread_mutex_lock (&thumbnails_mutex_);

    if (complete_thumbnails_.size() == 0)
    {
      idle_return_.reset();
      pthread_mutex_unlock (&thumbnails_mutex_);
      return false;
    }
    CompleteThumbnail complete_thumbnail = complete_thumbnails_.front();
    complete_thumbnails_.pop_front();

    pthread_mutex_unlock (&thumbnails_mutex_);

    if (complete_thumbnail.error_hint == "")
      parent_->ready.emit(complete_thumbnail.handle, complete_thumbnail.thubnail_uri);
    else
      parent_->error.emit(complete_thumbnail.handle, complete_thumbnail.error_hint);
  }
  return false;
}

void ThumbnailGeneratorImpl::LoadThumbnailers()
{
  if (all_thumbnailers_initialized)
    return;

  GError* err = NULL;
  GDir* thumbnailer_dir = g_dir_open("/usr/share/thumbnailers", 0, &err);
  if (err != NULL)
    return;

  const gchar* file;
  while((file = g_dir_read_name(thumbnailer_dir)) != NULL)
  {
    std::string file_name(file);
    if (file_name == "." || file_name == "..")
      continue;

    pthread_mutex_lock (&thumbnailers_mutex_);
    // Already have this one?
    if (thumbnailers_.find(file_name) != thumbnailers_.end())
    {
      pthread_mutex_unlock (&thumbnailers_mutex_);
      continue;
    }
    pthread_mutex_unlock (&thumbnailers_mutex_);

    /*********************************
     * READ SETTINGS
     *********************************/  

    GKeyFile* key_file = g_key_file_new();

    err=NULL;
    if (!g_key_file_load_from_file (key_file, (std::string("/usr/share/thumbnailers/") + file_name).c_str(), G_KEY_FILE_NONE, &err))
    {
      g_key_file_free(key_file);
      g_error_free(err);
      continue;
    }

    err=NULL;
    glib::String command_line(g_key_file_get_string (key_file, "Thumbnailer Entry", "Exec", &err));
    if (err != NULL)
    {
      g_key_file_free(key_file);
      g_error_free(err);
      continue;
    }

    err=NULL;
    gsize mime_count = 0;
    gchar** mime_types = g_key_file_get_string_list (key_file, "Thumbnailer Entry", "MimeType", &mime_count, &err);
    if (err != NULL)
    {
      g_key_file_free(key_file);
      g_error_free(err);
      continue;
    }

    Thumbnailer::Ptr thumbnailer(new ThumbnailerProcess(file_name, command_line.Value()));
    std::list<std::string> mime_type_list;
    for (gsize i = 0; i < mime_count && mime_types[i] != NULL; i++)
    {
      mime_type_list.push_front(mime_types[i]);
    }

    ThumbnailGenerator::RegisterThumbnailer(mime_type_list, thumbnailer);

    g_strfreev(mime_types);
    g_key_file_free(key_file);
  }

  all_thumbnailers_initialized = true;
  g_dir_close(thumbnailer_dir);
}

Thumbnailer::Ptr ThumbnailGeneratorImpl::GetThumbnailer(std::string const& content_type, std::string& error_hint)
{
  LoadThumbnailers();

  gchar** content_split = g_strsplit(content_type.c_str(), "/", -1);

  std::vector<std::string> content_list;
  int i = 0;
  while(content_split[i] != NULL)
  {
    if (g_strcmp0(content_split[i], "") != 0)
      content_list.push_back(content_split[i]);
    i++;
  }

  unsigned int content_last = content_list.size();
  for (unsigned int i = 0; i < content_list.size()+1; ++i)
  {
    std::stringstream ss_content_type;
    for (unsigned int j = 0; j < content_last; ++j)
    {
      ss_content_type << content_list[j];
      if (j < content_last-1)
        ss_content_type << "/";
    }

    if (content_last == 0)
      ss_content_type << "*";      
    else if (content_last < content_list.size())
      ss_content_type << "/*";

    content_last--;

    /*********************************
     * FIND THUMBNAILER
     *********************************/  

    pthread_mutex_lock (&thumbnailers_mutex_);

    // have already got this content type?
    auto iter_content = thumbnail_content_map.find(ss_content_type.str());
    if (iter_content != thumbnail_content_map.end())
    {
      pthread_mutex_unlock (&thumbnailers_mutex_);

      // find the thumbnailer.
      auto iter_tumbnailers = thumbnailers_.find(iter_content->second);
      if (iter_tumbnailers != thumbnailers_.end())
         return iter_tumbnailers->second;   
    }
    pthread_mutex_unlock (&thumbnailers_mutex_);
  }

  error_hint = "thumbnailer not found.";
  return NULL;
}

ThumbnailGenerator::ThumbnailGenerator()
: pimpl(new ThumbnailGeneratorImpl(this))
{
  if (thumbnail_instance)
  {
    LOG_ERROR(logger) << "More than one thumbnail generator created.";
  }
  else
  {
    thumbnail_instance = this;
    
    TextureThumbnailProvider::Initialise();
    DefaultThumbnailProvider::Initialise();
  }

}

ThumbnailGenerator::~ThumbnailGenerator()
{

}

ThumbnailGenerator& ThumbnailGenerator::Instance()
{
  if (!thumbnail_instance)
  {
    LOG_ERROR(logger) << "No panel::Style created yet.";
  }

  return *thumbnail_instance;
}

unsigned int ThumbnailGenerator::GetThumbnail(std::string const& uri, int size)
{
  return pimpl->GetThumbnail(uri, size);
}

void ThumbnailGenerator::RegisterThumbnailer(std::list<std::string> mime_types, Thumbnailer::Ptr thumbnailer)
{
  pthread_mutex_lock (&thumbnailers_mutex_);

  thumbnailers_[thumbnailer->GetName()] = thumbnailer;

  for (std::string mime_type : mime_types)
  {
    thumbnail_content_map[mime_type] = thumbnailer->GetName();      
  }

  pthread_mutex_unlock (&thumbnailers_mutex_);
}

Thumbnail::Thumbnail(std::string const& uri, unsigned int size)
: uri_(uri)
, size_(size)
, handle_(last_handle++)
{
}

Thumbnail::~Thumbnail()
{
}

std::string  Thumbnail::Generate(std::string& error_hint)
{
  std::string output_file;

  glib::Object<GFile> file(::g_file_new_for_uri(uri_.c_str()));

  GError *err = NULL;
  glib::Object<GFileInfo> file_info(g_file_query_info(file, G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE, G_FILE_QUERY_INFO_NONE, NULL, &err));
  if (err != NULL)
  {
    error_hint = err->message;
    g_error_free (err);
    return "";
  }

  std::string file_type = g_file_info_get_attribute_string(file_info, G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE);

  Thumbnailer::Ptr thumbnailer = ThumbnailGeneratorImpl::GetThumbnailer(file_type, error_hint);
  if (!thumbnailer)
    return "";

  std::hash<std::string> hash_fn;
  std::stringstream ss_output; ss_output << "/home/nick/test_thumbnails/" << hash_fn(uri_) << ".png";
  output_file = ss_output.str();

  if (!thumbnailer->Run(size_, uri_, output_file, error_hint))
  {
    return "";
  }

  return output_file;
}

unsigned int Thumbnail::GetHandle() const
{
  return handle_;
}

bool ThumbnailerProcess::Run(int size, std::string const& input_file, std::string& output_file, std::string& error_hint)
{
  std::string tmp_command_line = command_line;

  size_t pos = tmp_command_line.find("%s");
  std::stringstream ss; ss << size;
  if (pos != std::string::npos) { tmp_command_line.replace(pos, 2, ss.str()); }

  pos = tmp_command_line.find("%u");
  if (pos != std::string::npos) { tmp_command_line.replace(pos, 2, input_file); }

  pos = tmp_command_line.find("%o");
  if (pos != std::string::npos) { tmp_command_line.replace(pos, 2, output_file); }

  gint exit_status = 0;
  GError* err = NULL;
  g_spawn_command_line_sync(tmp_command_line.c_str(), NULL, NULL, &exit_status, &err);
  if (err != NULL)
  {
    error_hint = err->message;
    g_error_free (err);
    return false;
  }

  return true;
}

} // namespace unity