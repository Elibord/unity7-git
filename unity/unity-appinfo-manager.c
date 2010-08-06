/* unity-appinfo-manager.c generated by valac, the Vala compiler
 * generated from unity-appinfo-manager.vala, do not modify */

/*
 * Copyright (C) 2010 Canonical, Ltd.
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * version 3.0 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License version 3.0 for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Authored by Mikkel Kamstrup Erlandsen <mikkel.kamstrup@canonical.com>
 *
 */
/*
 * IMPLEMENTATION NOTE:
 * We want the generatedd C API to be nice and not too Vala-ish. We must
 * anticipate that place daemons consuming libunity will be written in
 * both Vala and C.
 *
 */

#include <glib.h>
#include <glib-object.h>
#include <gee.h>
#include <stdlib.h>
#include <string.h>
#include <gio/gio.h>
#include <gio/gdesktopappinfo.h>


#define UNITY_TYPE_APP_INFO_MANAGER (unity_app_info_manager_get_type ())
#define UNITY_APP_INFO_MANAGER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), UNITY_TYPE_APP_INFO_MANAGER, UnityAppInfoManager))
#define UNITY_APP_INFO_MANAGER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), UNITY_TYPE_APP_INFO_MANAGER, UnityAppInfoManagerClass))
#define UNITY_IS_APP_INFO_MANAGER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), UNITY_TYPE_APP_INFO_MANAGER))
#define UNITY_IS_APP_INFO_MANAGER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), UNITY_TYPE_APP_INFO_MANAGER))
#define UNITY_APP_INFO_MANAGER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), UNITY_TYPE_APP_INFO_MANAGER, UnityAppInfoManagerClass))

typedef struct _UnityAppInfoManager UnityAppInfoManager;
typedef struct _UnityAppInfoManagerClass UnityAppInfoManagerClass;
typedef struct _UnityAppInfoManagerPrivate UnityAppInfoManagerPrivate;
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
#define _g_free0(var) (var = (g_free (var), NULL))
#define _g_error_free0(var) ((var == NULL) ? NULL : (var = (g_error_free (var), NULL)))
#define _g_key_file_free0(var) ((var == NULL) ? NULL : (var = (g_key_file_free (var), NULL)))
typedef struct _UnityAppInfoManagerLookupAsyncData UnityAppInfoManagerLookupAsyncData;

struct _UnityAppInfoManager {
	GObject parent_instance;
	UnityAppInfoManagerPrivate * priv;
};

struct _UnityAppInfoManagerClass {
	GObjectClass parent_class;
};

struct _UnityAppInfoManagerPrivate {
	GeeMap* appinfo_by_id;
	guchar* buffer;
	gint buffer_length1;
	gint _buffer_size_;
	gsize buffer_size;
};

struct _UnityAppInfoManagerLookupAsyncData {
	int _state_;
	GAsyncResult* _res_;
	GSimpleAsyncResult* _async_result;
	UnityAppInfoManager* self;
	char* id;
	GAppInfo* result;
	GAppInfo* appinfo;
	gsize data_size;
	void* data;
	GFileInputStream* input;
	GFile* f;
	GFileInputStream* _tmp0_;
	GFileInputStream* _tmp1_;
	char* path;
	GFileInputStream* _tmp2_;
	GFileInputStream* _tmp3_;
	guchar* _tmp4_;
	GError * e;
	GKeyFile* keyfile;
	GError * ee;
	GAppInfo* _tmp5_;
	GError * _inner_error_;
};


static UnityAppInfoManager* unity_app_info_manager_singleton;
static UnityAppInfoManager* unity_app_info_manager_singleton = NULL;
static gpointer unity_app_info_manager_parent_class = NULL;

GType unity_app_info_manager_get_type (void) G_GNUC_CONST;
#define UNITY_APP_INFO_MANAGER_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), UNITY_TYPE_APP_INFO_MANAGER, UnityAppInfoManagerPrivate))
enum  {
	UNITY_APP_INFO_MANAGER_DUMMY_PROPERTY
};
static UnityAppInfoManager* unity_app_info_manager_new (void);
static UnityAppInfoManager* unity_app_info_manager_construct (GType object_type);
UnityAppInfoManager* unity_app_info_manager_get_instance (void);
GAppInfo* unity_app_info_manager_lookup (UnityAppInfoManager* self, const char* id);
static void unity_app_info_manager_lookup_async_data_free (gpointer _data);
static void unity_app_info_manager_lookup_async_ready (GObject* source_object, GAsyncResult* _res_, gpointer _user_data_);
void unity_io_open_from_data_dirs (const char* filename, GAsyncReadyCallback _callback_, gpointer _user_data_);
GFileInputStream* unity_io_open_from_data_dirs_finish (GAsyncResult* _res_, GError** error);
void unity_io_read_stream_async (GInputStream* input, guchar* buffer, int buffer_length1, gsize buffer_lenght, gint io_priority, GCancellable* cancellable, GAsyncReadyCallback _callback_, gpointer _user_data_);
void unity_io_read_stream_finish (GAsyncResult* _res_, void** data, gsize* size, GError** error);
static guchar* _vala_array_dup1 (guchar* self, int length);
void unity_app_info_manager_lookup_async (UnityAppInfoManager* self, const char* id, GAsyncReadyCallback _callback_, gpointer _user_data_);
GAppInfo* unity_app_info_manager_lookup_finish (UnityAppInfoManager* self, GAsyncResult* _res_, GError** error);
static gboolean unity_app_info_manager_lookup_async_co (UnityAppInfoManagerLookupAsyncData* data);
void unity_app_info_manager_clear (UnityAppInfoManager* self);
static void unity_app_info_manager_finalize (GObject* obj);



static UnityAppInfoManager* unity_app_info_manager_construct (GType object_type) {
	UnityAppInfoManager * self;
	GeeMap* _tmp0_;
	guchar* _tmp1_;
	self = (UnityAppInfoManager*) g_object_new (object_type, NULL);
	self->priv->appinfo_by_id = (_tmp0_ = (GeeMap*) gee_hash_map_new (G_TYPE_STRING, (GBoxedCopyFunc) g_strdup, g_free, G_TYPE_APP_INFO, (GBoxedCopyFunc) g_object_ref, g_object_unref, g_str_hash, g_str_equal, NULL), _g_object_unref0 (self->priv->appinfo_by_id), _tmp0_);
	self->priv->buffer_size = (gsize) 1024;
	self->priv->buffer = (_tmp1_ = g_new0 (guchar, self->priv->buffer_size), self->priv->buffer = (g_free (self->priv->buffer), NULL), self->priv->buffer_length1 = self->priv->buffer_size, self->priv->_buffer_size_ = self->priv->buffer_length1, _tmp1_);
	return self;
}


static UnityAppInfoManager* unity_app_info_manager_new (void) {
	return unity_app_info_manager_construct (UNITY_TYPE_APP_INFO_MANAGER);
}


static gpointer _g_object_ref0 (gpointer self) {
	return self ? g_object_ref (self) : NULL;
}


/**
     * Get a ref to the singleton AppInfoManager
     */
UnityAppInfoManager* unity_app_info_manager_get_instance (void) {
	UnityAppInfoManager* result = NULL;
	if (unity_app_info_manager_singleton == NULL) {
		UnityAppInfoManager* _tmp0_;
		unity_app_info_manager_singleton = (_tmp0_ = unity_app_info_manager_new (), _g_object_unref0 (unity_app_info_manager_singleton), _tmp0_);
	}
	result = _g_object_ref0 (unity_app_info_manager_singleton);
	return result;
}


/**
     * Look up an AppInfo given its desktop id or absolute path. The desktop id
     * is the base filename of the .desktop file for the application including
     * the .desktop extension.
     *
     * If the AppInfo is not already cached this method will do synchronous
     * IO to look it up.
     */
GAppInfo* unity_app_info_manager_lookup (UnityAppInfoManager* self, const char* id) {
	GAppInfo* result = NULL;
	GAppInfo* appinfo;
	g_return_val_if_fail (self != NULL, NULL);
	g_return_val_if_fail (id != NULL, NULL);
	appinfo = (GAppInfo*) gee_map_get (self->priv->appinfo_by_id, id);
	if (appinfo != NULL) {
		result = appinfo;
		return result;
	}
	if (g_str_has_prefix (id, "/")) {
		GAppInfo* _tmp0_;
		appinfo = (_tmp0_ = (GAppInfo*) g_desktop_app_info_new_from_filename (id), _g_object_unref0 (appinfo), _tmp0_);
	} else {
		GAppInfo* _tmp1_;
		appinfo = (_tmp1_ = (GAppInfo*) g_desktop_app_info_new (id), _g_object_unref0 (appinfo), _tmp1_);
	}
	if (appinfo != NULL) {
		gee_map_set (self->priv->appinfo_by_id, id, appinfo);
	}
	result = appinfo;
	return result;
}


static void unity_app_info_manager_lookup_async_data_free (gpointer _data) {
	UnityAppInfoManagerLookupAsyncData* data;
	data = _data;
	_g_free0 (data->id);
	_g_object_unref0 (data->result);
	g_object_unref (data->self);
	g_slice_free (UnityAppInfoManagerLookupAsyncData, data);
}


void unity_app_info_manager_lookup_async (UnityAppInfoManager* self, const char* id, GAsyncReadyCallback _callback_, gpointer _user_data_) {
	UnityAppInfoManagerLookupAsyncData* _data_;
	_data_ = g_slice_new0 (UnityAppInfoManagerLookupAsyncData);
	_data_->_async_result = g_simple_async_result_new (G_OBJECT (self), _callback_, _user_data_, unity_app_info_manager_lookup_async);
	g_simple_async_result_set_op_res_gpointer (_data_->_async_result, _data_, unity_app_info_manager_lookup_async_data_free);
	_data_->self = g_object_ref (self);
	_data_->id = g_strdup (id);
	unity_app_info_manager_lookup_async_co (_data_);
}


GAppInfo* unity_app_info_manager_lookup_finish (UnityAppInfoManager* self, GAsyncResult* _res_, GError** error) {
	GAppInfo* result;
	UnityAppInfoManagerLookupAsyncData* _data_;
	if (g_simple_async_result_propagate_error (G_SIMPLE_ASYNC_RESULT (_res_), error)) {
		return NULL;
	}
	_data_ = g_simple_async_result_get_op_res_gpointer (G_SIMPLE_ASYNC_RESULT (_res_));
	result = _data_->result;
	_data_->result = NULL;
	return result;
}


static void unity_app_info_manager_lookup_async_ready (GObject* source_object, GAsyncResult* _res_, gpointer _user_data_) {
	UnityAppInfoManagerLookupAsyncData* data;
	data = _user_data_;
	data->_res_ = _res_;
	unity_app_info_manager_lookup_async_co (data);
}


static guchar* _vala_array_dup1 (guchar* self, int length) {
	return g_memdup (self, length * sizeof (guchar));
}


/**
     * Look up an AppInfo given its desktop id or absolute path.
     * The desktop id is the base filename of the .desktop file for the
     * application including the .desktop extension.
     *
     * If the AppInfo is not already cached this method will do asynchronous
     * IO to look it up.
     */
static gboolean unity_app_info_manager_lookup_async_co (UnityAppInfoManagerLookupAsyncData* data) {
	switch (data->_state_) {
		case 0:
		goto _state_0;
		case 3:
		goto _state_3;
		case 4:
		goto _state_4;
		case 5:
		goto _state_5;
		default:
		g_assert_not_reached ();
	}
	_state_0:
	{
		data->appinfo = (GAppInfo*) gee_map_get (data->self->priv->appinfo_by_id, data->id);
		if (data->appinfo != NULL) {
			data->result = data->appinfo;
			{
				if (data->_state_ == 0) {
					g_simple_async_result_complete_in_idle (data->_async_result);
				} else {
					g_simple_async_result_complete (data->_async_result);
				}
				g_object_unref (data->_async_result);
				return FALSE;
			}
		}
		if (g_str_has_prefix (data->id, "/")) {
			data->f = g_file_new_for_path (data->id);
			data->_state_ = 3;
			g_file_read_async (data->f, G_PRIORITY_DEFAULT, NULL, unity_app_info_manager_lookup_async_ready, data);
			return FALSE;
			_state_3:
			data->_tmp0_ = g_file_read_finish (data->f, data->_res_, &data->_inner_error_);
			if (data->_inner_error_ != NULL) {
				g_simple_async_result_set_from_error (data->_async_result, data->_inner_error_);
				g_error_free (data->_inner_error_);
				_g_object_unref0 (data->f);
				_g_object_unref0 (data->input);
				_g_object_unref0 (data->appinfo);
				{
					if (data->_state_ == 0) {
						g_simple_async_result_complete_in_idle (data->_async_result);
					} else {
						g_simple_async_result_complete (data->_async_result);
					}
					g_object_unref (data->_async_result);
					return FALSE;
				}
			}
			data->input = (data->_tmp1_ = data->_tmp0_, _g_object_unref0 (data->input), data->_tmp1_);
			_g_object_unref0 (data->f);
		} else {
			data->path = g_build_filename ("applications", data->id, NULL, NULL);
			data->_state_ = 4;
			unity_io_open_from_data_dirs (data->path, unity_app_info_manager_lookup_async_ready, data);
			return FALSE;
			_state_4:
			data->_tmp2_ = unity_io_open_from_data_dirs_finish (data->_res_, &data->_inner_error_);
			if (data->_inner_error_ != NULL) {
				g_simple_async_result_set_from_error (data->_async_result, data->_inner_error_);
				g_error_free (data->_inner_error_);
				_g_free0 (data->path);
				_g_object_unref0 (data->input);
				_g_object_unref0 (data->appinfo);
				{
					if (data->_state_ == 0) {
						g_simple_async_result_complete_in_idle (data->_async_result);
					} else {
						g_simple_async_result_complete (data->_async_result);
					}
					g_object_unref (data->_async_result);
					return FALSE;
				}
			}
			data->input = (data->_tmp3_ = data->_tmp2_, _g_object_unref0 (data->input), data->_tmp3_);
			_g_free0 (data->path);
		}
		if (data->input == NULL) {
			data->result = NULL;
			_g_object_unref0 (data->input);
			_g_object_unref0 (data->appinfo);
			{
				if (data->_state_ == 0) {
					g_simple_async_result_complete_in_idle (data->_async_result);
				} else {
					g_simple_async_result_complete (data->_async_result);
				}
				g_object_unref (data->_async_result);
				return FALSE;
			}
		}
		{
			data->_state_ = 5;
			unity_io_read_stream_async ((GInputStream*) data->input, (data->_tmp4_ = data->self->priv->buffer, (data->_tmp4_ == NULL) ? ((gpointer) data->_tmp4_) : _vala_array_dup1 (data->_tmp4_, data->self->priv->buffer_length1)), data->self->priv->buffer_length1, data->self->priv->buffer_size, G_PRIORITY_DEFAULT, NULL, unity_app_info_manager_lookup_async_ready, data);
			return FALSE;
			_state_5:
			unity_io_read_stream_finish (data->_res_, &data->data, &data->data_size, &data->_inner_error_);
			if (data->_inner_error_ != NULL) {
				goto __catch9_g_error;
			}
		}
		goto __finally9;
		__catch9_g_error:
		{
			data->e = data->_inner_error_;
			data->_inner_error_ = NULL;
			{
				g_warning ("unity-appinfo-manager.vala:137: Error reading '%s': %s", data->id, data->e->message);
				data->result = NULL;
				_g_error_free0 (data->e);
				_g_object_unref0 (data->input);
				_g_object_unref0 (data->appinfo);
				{
					if (data->_state_ == 0) {
						g_simple_async_result_complete_in_idle (data->_async_result);
					} else {
						g_simple_async_result_complete (data->_async_result);
					}
					g_object_unref (data->_async_result);
					return FALSE;
				}
				_g_error_free0 (data->e);
			}
		}
		__finally9:
		if (data->_inner_error_ != NULL) {
			g_simple_async_result_set_from_error (data->_async_result, data->_inner_error_);
			g_error_free (data->_inner_error_);
			_g_object_unref0 (data->input);
			_g_object_unref0 (data->appinfo);
			{
				if (data->_state_ == 0) {
					g_simple_async_result_complete_in_idle (data->_async_result);
				} else {
					g_simple_async_result_complete (data->_async_result);
				}
				g_object_unref (data->_async_result);
				return FALSE;
			}
		}
		data->keyfile = g_key_file_new ();
		{
			g_key_file_load_from_data (data->keyfile, (const char*) data->data, data->data_size, G_KEY_FILE_NONE, &data->_inner_error_);
			if (data->_inner_error_ != NULL) {
				goto __catch10_g_error;
			}
		}
		goto __finally10;
		__catch10_g_error:
		{
			data->ee = data->_inner_error_;
			data->_inner_error_ = NULL;
			{
				g_warning ("unity-appinfo-manager.vala:148: Error parsing '%s': %s", data->id, data->ee->message);
				data->result = NULL;
				_g_error_free0 (data->ee);
				_g_key_file_free0 (data->keyfile);
				_g_object_unref0 (data->input);
				_g_object_unref0 (data->appinfo);
				{
					if (data->_state_ == 0) {
						g_simple_async_result_complete_in_idle (data->_async_result);
					} else {
						g_simple_async_result_complete (data->_async_result);
					}
					g_object_unref (data->_async_result);
					return FALSE;
				}
				_g_error_free0 (data->ee);
			}
		}
		__finally10:
		if (data->_inner_error_ != NULL) {
			g_simple_async_result_set_from_error (data->_async_result, data->_inner_error_);
			g_error_free (data->_inner_error_);
			_g_key_file_free0 (data->keyfile);
			_g_object_unref0 (data->input);
			_g_object_unref0 (data->appinfo);
			{
				if (data->_state_ == 0) {
					g_simple_async_result_complete_in_idle (data->_async_result);
				} else {
					g_simple_async_result_complete (data->_async_result);
				}
				g_object_unref (data->_async_result);
				return FALSE;
			}
		}
		data->appinfo = (data->_tmp5_ = (GAppInfo*) g_desktop_app_info_new_from_keyfile (data->keyfile), _g_object_unref0 (data->appinfo), data->_tmp5_);
		gee_map_set (data->self->priv->appinfo_by_id, data->id, data->appinfo);
		g_free (data->data);
		data->result = data->appinfo;
		_g_key_file_free0 (data->keyfile);
		_g_object_unref0 (data->input);
		{
			if (data->_state_ == 0) {
				g_simple_async_result_complete_in_idle (data->_async_result);
			} else {
				g_simple_async_result_complete (data->_async_result);
			}
			g_object_unref (data->_async_result);
			return FALSE;
		}
		_g_key_file_free0 (data->keyfile);
		_g_object_unref0 (data->input);
		_g_object_unref0 (data->appinfo);
	}
	{
		if (data->_state_ == 0) {
			g_simple_async_result_complete_in_idle (data->_async_result);
		} else {
			g_simple_async_result_complete (data->_async_result);
		}
		g_object_unref (data->_async_result);
		return FALSE;
	}
}


void unity_app_info_manager_clear (UnityAppInfoManager* self) {
	g_return_if_fail (self != NULL);
	gee_map_clear (self->priv->appinfo_by_id);
}


static void unity_app_info_manager_class_init (UnityAppInfoManagerClass * klass) {
	unity_app_info_manager_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (UnityAppInfoManagerPrivate));
	G_OBJECT_CLASS (klass)->finalize = unity_app_info_manager_finalize;
}


static void unity_app_info_manager_instance_init (UnityAppInfoManager * self) {
	self->priv = UNITY_APP_INFO_MANAGER_GET_PRIVATE (self);
}


static void unity_app_info_manager_finalize (GObject* obj) {
	UnityAppInfoManager * self;
	self = UNITY_APP_INFO_MANAGER (obj);
	_g_object_unref0 (self->priv->appinfo_by_id);
	self->priv->buffer = (g_free (self->priv->buffer), NULL);
	G_OBJECT_CLASS (unity_app_info_manager_parent_class)->finalize (obj);
}


/**
   * A singleton class that caches GLib.AppInfo objects.
   * Singletons are evil, yes, but this on slightly less
   * so because the exposed API is immutable
   */
GType unity_app_info_manager_get_type (void) {
	static volatile gsize unity_app_info_manager_type_id__volatile = 0;
	if (g_once_init_enter (&unity_app_info_manager_type_id__volatile)) {
		static const GTypeInfo g_define_type_info = { sizeof (UnityAppInfoManagerClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) unity_app_info_manager_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (UnityAppInfoManager), 0, (GInstanceInitFunc) unity_app_info_manager_instance_init, NULL };
		GType unity_app_info_manager_type_id;
		unity_app_info_manager_type_id = g_type_register_static (G_TYPE_OBJECT, "UnityAppInfoManager", &g_define_type_info, 0);
		g_once_init_leave (&unity_app_info_manager_type_id__volatile, unity_app_info_manager_type_id);
	}
	return unity_app_info_manager_type_id__volatile;
}




