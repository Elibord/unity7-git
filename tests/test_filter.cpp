#include <gtest/gtest.h>
#include <glib-object.h>
#include <UnityCore/GLibWrapper.h>
#include <UnityCore/Filter.h>

using namespace std;
using namespace unity::dash;
using namespace unity;

namespace
{

class TestFilter : public ::testing::Test
{
public:
  TestFilter()
    : model_(dee_sequence_model_new())
  {
    dee_model_set_schema(model_, "s", "s", "s", "s", "a{sv}", "b", "b", "b", NULL);

    AddIters();
  }

  void AddIters()
  {
    GVariantBuilder b;
    g_variant_builder_init(&b, G_VARIANT_TYPE("a{sv}"));
    GVariant *hints = g_variant_builder_end(&b);

    iter0_ = dee_model_append(model_,
                              "ratings",
                              "Ratings0",
                              "gtk-apply",
                              "ratings",
                              hints,
                              TRUE,
                              TRUE,
                              FALSE);

    iter1_ = dee_model_append(model_,
                              "ratings",
                              "Ratings1",
                              "gtk-apply",
                              "ratings",
                              hints,
                              TRUE,
                              TRUE,
                              FALSE);

    iter2_ = dee_model_append(model_,
                              "ratings",
                              "Ratings2",
                              "gtk-apply",
                              "ratings",
                              hints,
                              TRUE,
                              TRUE,
                              FALSE);
    g_variant_unref(hints);
  }

  glib::Object<DeeModel> model_;
  DeeModelIter* iter0_;
  DeeModelIter* iter1_;
  DeeModelIter* iter2_;
};

class FilterRecorder : public Filter
{
public:
  FilterRecorder(DeeModel* model, DeeModelIter* iter)
    : removed_(false)
    , updated_(false)
  {
    model_ = model;
    iter_ = iter;
    Connect();

    removed.connect(sigc::mem_fun(this, &FilterRecorder::OnRemoved));
  }

  void Update(Filter::Hints& hints)
  {
    updated_ = true;
  }

  void OnRemoved()
  {
    removed_ = true;
  }

  void Clear()
  {}

  bool removed_;
  bool updated_;
};

TEST_F(TestFilter, TestConstruction)
{
  Filter filter;
  Filter::Ptr(new Filter());
}

TEST_F(TestFilter, TestConnect)
{
  FilterRecorder recorder(model_, iter0_);

  EXPECT_EQ(recorder.id, "ratings");
  EXPECT_EQ(recorder.name, "Ratings0");
  EXPECT_EQ(recorder.icon_hint, "gtk-apply");
  EXPECT_EQ(recorder.renderer_name, "ratings");
  EXPECT_EQ(recorder.visible, true);
  EXPECT_EQ(recorder.collapsed, true);
  EXPECT_EQ(recorder.filtering, false);
}

TEST_F(TestFilter, TestChanged)
{
  FilterRecorder recorder(model_, iter1_);

  dee_model_set_value(model_, iter1_, 0, g_variant_new("s", "checkbox"));
  dee_model_set_value(model_, iter1_, 7, g_variant_new("b", TRUE));

  EXPECT_EQ(recorder.updated_, true);
  EXPECT_EQ(recorder.id, "checkbox");
  EXPECT_EQ(recorder.name, "Ratings1");
  EXPECT_EQ(recorder.icon_hint, "gtk-apply");
  EXPECT_EQ(recorder.renderer_name, "ratings");
  EXPECT_EQ(recorder.visible, true);
  EXPECT_EQ(recorder.collapsed, true);
  EXPECT_EQ(recorder.filtering, true);
}

TEST_F(TestFilter, TestRemoved)
{
  FilterRecorder recorder(model_, iter1_);

  dee_model_remove(model_, iter2_);
  EXPECT_EQ(recorder.removed_, false);

  dee_model_remove(model_, iter0_);
  EXPECT_EQ(recorder.removed_, false);

  dee_model_remove(model_, iter1_);
  EXPECT_EQ(recorder.removed_, true);
}

}
