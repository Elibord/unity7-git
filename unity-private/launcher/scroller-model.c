/* scroller-model.c generated by valac, the Vala compiler
 * generated from scroller-model.vala, do not modify */

/*
 *      scroller-model.vala
 *      Copyright (C) 2010 Canonical Ltd
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 *
 *
 *      Authored by Gordon Allott <gord.allott@canonical.com>
 */

#include <glib.h>
#include <glib-object.h>
#include <gee.h>
#include <clutk/clutk.h>
#include <stdlib.h>
#include <string.h>
#include <gobject/gvaluecollector.h>


#define UNITY_LAUNCHER_TYPE_SCROLLER_MODEL (unity_launcher_scroller_model_get_type ())
#define UNITY_LAUNCHER_SCROLLER_MODEL(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), UNITY_LAUNCHER_TYPE_SCROLLER_MODEL, UnityLauncherScrollerModel))
#define UNITY_LAUNCHER_SCROLLER_MODEL_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), UNITY_LAUNCHER_TYPE_SCROLLER_MODEL, UnityLauncherScrollerModelClass))
#define UNITY_LAUNCHER_IS_SCROLLER_MODEL(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), UNITY_LAUNCHER_TYPE_SCROLLER_MODEL))
#define UNITY_LAUNCHER_IS_SCROLLER_MODEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), UNITY_LAUNCHER_TYPE_SCROLLER_MODEL))
#define UNITY_LAUNCHER_SCROLLER_MODEL_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), UNITY_LAUNCHER_TYPE_SCROLLER_MODEL, UnityLauncherScrollerModelClass))

typedef struct _UnityLauncherScrollerModel UnityLauncherScrollerModel;
typedef struct _UnityLauncherScrollerModelClass UnityLauncherScrollerModelClass;
typedef struct _UnityLauncherScrollerModelPrivate UnityLauncherScrollerModelPrivate;

#define UNITY_LAUNCHER_TYPE_SCROLLER_CHILD (unity_launcher_scroller_child_get_type ())
#define UNITY_LAUNCHER_SCROLLER_CHILD(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), UNITY_LAUNCHER_TYPE_SCROLLER_CHILD, UnityLauncherScrollerChild))
#define UNITY_LAUNCHER_SCROLLER_CHILD_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), UNITY_LAUNCHER_TYPE_SCROLLER_CHILD, UnityLauncherScrollerChildClass))
#define UNITY_LAUNCHER_IS_SCROLLER_CHILD(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), UNITY_LAUNCHER_TYPE_SCROLLER_CHILD))
#define UNITY_LAUNCHER_IS_SCROLLER_CHILD_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), UNITY_LAUNCHER_TYPE_SCROLLER_CHILD))
#define UNITY_LAUNCHER_SCROLLER_CHILD_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), UNITY_LAUNCHER_TYPE_SCROLLER_CHILD, UnityLauncherScrollerChildClass))

typedef struct _UnityLauncherScrollerChild UnityLauncherScrollerChild;
typedef struct _UnityLauncherScrollerChildClass UnityLauncherScrollerChildClass;
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

#define UNITY_LAUNCHER_SCROLLER_MODEL_TYPE_ITERATOR (unity_launcher_scroller_model_iterator_get_type ())
#define UNITY_LAUNCHER_SCROLLER_MODEL_ITERATOR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), UNITY_LAUNCHER_SCROLLER_MODEL_TYPE_ITERATOR, UnityLauncherScrollerModelIterator))
#define UNITY_LAUNCHER_SCROLLER_MODEL_ITERATOR_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), UNITY_LAUNCHER_SCROLLER_MODEL_TYPE_ITERATOR, UnityLauncherScrollerModelIteratorClass))
#define UNITY_LAUNCHER_SCROLLER_MODEL_IS_ITERATOR(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), UNITY_LAUNCHER_SCROLLER_MODEL_TYPE_ITERATOR))
#define UNITY_LAUNCHER_SCROLLER_MODEL_IS_ITERATOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), UNITY_LAUNCHER_SCROLLER_MODEL_TYPE_ITERATOR))
#define UNITY_LAUNCHER_SCROLLER_MODEL_ITERATOR_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), UNITY_LAUNCHER_SCROLLER_MODEL_TYPE_ITERATOR, UnityLauncherScrollerModelIteratorClass))

typedef struct _UnityLauncherScrollerModelIterator UnityLauncherScrollerModelIterator;
typedef struct _UnityLauncherScrollerModelIteratorClass UnityLauncherScrollerModelIteratorClass;

#define UNITY_TESTING_TYPE_OBJECT_REGISTRY (unity_testing_object_registry_get_type ())
#define UNITY_TESTING_OBJECT_REGISTRY(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), UNITY_TESTING_TYPE_OBJECT_REGISTRY, UnityTestingObjectRegistry))
#define UNITY_TESTING_OBJECT_REGISTRY_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), UNITY_TESTING_TYPE_OBJECT_REGISTRY, UnityTestingObjectRegistryClass))
#define UNITY_TESTING_IS_OBJECT_REGISTRY(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), UNITY_TESTING_TYPE_OBJECT_REGISTRY))
#define UNITY_TESTING_IS_OBJECT_REGISTRY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), UNITY_TESTING_TYPE_OBJECT_REGISTRY))
#define UNITY_TESTING_OBJECT_REGISTRY_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), UNITY_TESTING_TYPE_OBJECT_REGISTRY, UnityTestingObjectRegistryClass))

typedef struct _UnityTestingObjectRegistry UnityTestingObjectRegistry;
typedef struct _UnityTestingObjectRegistryClass UnityTestingObjectRegistryClass;
#define _unity_testing_object_registry_unref0(var) ((var == NULL) ? NULL : (var = (unity_testing_object_registry_unref (var), NULL)))
typedef struct _UnityLauncherScrollerModelIteratorPrivate UnityLauncherScrollerModelIteratorPrivate;
typedef struct _UnityLauncherScrollerModelParamSpecIterator UnityLauncherScrollerModelParamSpecIterator;

struct _UnityLauncherScrollerModel {
	GObject parent_instance;
	UnityLauncherScrollerModelPrivate * priv;
};

struct _UnityLauncherScrollerModelClass {
	GObjectClass parent_class;
};

struct _UnityLauncherScrollerModelPrivate {
	GeeArrayList* children;
};

struct _UnityLauncherScrollerModelIterator {
	GTypeInstance parent_instance;
	volatile int ref_count;
	UnityLauncherScrollerModelIteratorPrivate * priv;
};

struct _UnityLauncherScrollerModelIteratorClass {
	GTypeClass parent_class;
	void (*finalize) (UnityLauncherScrollerModelIterator *self);
};

struct _UnityLauncherScrollerModelIteratorPrivate {
	gint iter_index;
	GeeArrayList* array;
};

struct _UnityLauncherScrollerModelParamSpecIterator {
	GParamSpec parent_instance;
};


static gpointer unity_launcher_scroller_model_iterator_parent_class = NULL;
static gpointer unity_launcher_scroller_model_parent_class = NULL;

GType unity_launcher_scroller_model_get_type (void);
GType unity_launcher_scroller_child_get_type (void);
#define UNITY_LAUNCHER_SCROLLER_MODEL_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), UNITY_LAUNCHER_TYPE_SCROLLER_MODEL, UnityLauncherScrollerModelPrivate))
enum  {
	UNITY_LAUNCHER_SCROLLER_MODEL_DUMMY_PROPERTY,
	UNITY_LAUNCHER_SCROLLER_MODEL_SIZE
};
UnityLauncherScrollerModel* unity_launcher_scroller_model_new (void);
UnityLauncherScrollerModel* unity_launcher_scroller_model_construct (GType object_type);
char* unity_launcher_scroller_model_to_string (UnityLauncherScrollerModel* self);
UnityLauncherScrollerModelIterator* unity_launcher_scroller_model_iterator_new (GeeArrayList* arraylist);
UnityLauncherScrollerModelIterator* unity_launcher_scroller_model_iterator_construct (GType object_type, GeeArrayList* arraylist);
gpointer unity_launcher_scroller_model_iterator_ref (gpointer instance);
void unity_launcher_scroller_model_iterator_unref (gpointer instance);
GParamSpec* unity_launcher_scroller_model_param_spec_iterator (const gchar* name, const gchar* nick, const gchar* blurb, GType object_type, GParamFlags flags);
void unity_launcher_scroller_model_value_set_iterator (GValue* value, gpointer v_object);
void unity_launcher_scroller_model_value_take_iterator (GValue* value, gpointer v_object);
gpointer unity_launcher_scroller_model_value_get_iterator (const GValue* value);
GType unity_launcher_scroller_model_iterator_get_type (void);
UnityLauncherScrollerModelIterator* unity_launcher_scroller_model_iterator (UnityLauncherScrollerModel* self);
gboolean unity_launcher_scroller_model_contains (UnityLauncherScrollerModel* self, UnityLauncherScrollerChild* child);
void unity_launcher_scroller_model_add (UnityLauncherScrollerModel* self, UnityLauncherScrollerChild* child);
void unity_launcher_scroller_model_remove (UnityLauncherScrollerModel* self, UnityLauncherScrollerChild* child);
void unity_launcher_scroller_model_insert (UnityLauncherScrollerModel* self, UnityLauncherScrollerChild* child, gint i);
void unity_launcher_scroller_model_move (UnityLauncherScrollerModel* self, UnityLauncherScrollerChild* child, gint i);
gint unity_launcher_scroller_model_index_of (UnityLauncherScrollerModel* self, UnityLauncherScrollerChild* child);
void unity_launcher_scroller_model_sort (UnityLauncherScrollerModel* self, GCompareFunc compare);
UnityLauncherScrollerChild* unity_launcher_scroller_model_get (UnityLauncherScrollerModel* self, gint i);
void unity_launcher_scroller_model_set (UnityLauncherScrollerModel* self, gint i, UnityLauncherScrollerChild* item);
gint unity_launcher_scroller_model_get_size (UnityLauncherScrollerModel* self);
gpointer unity_testing_object_registry_ref (gpointer instance);
void unity_testing_object_registry_unref (gpointer instance);
GParamSpec* unity_testing_param_spec_object_registry (const gchar* name, const gchar* nick, const gchar* blurb, GType object_type, GParamFlags flags);
void unity_testing_value_set_object_registry (GValue* value, gpointer v_object);
void unity_testing_value_take_object_registry (GValue* value, gpointer v_object);
gpointer unity_testing_value_get_object_registry (const GValue* value);
GType unity_testing_object_registry_get_type (void);
UnityTestingObjectRegistry* unity_testing_object_registry_get_default (void);
void unity_testing_object_registry_register (UnityTestingObjectRegistry* self, const char* name, GObject* object);
static GObject * unity_launcher_scroller_model_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties);
#define UNITY_LAUNCHER_SCROLLER_MODEL_ITERATOR_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), UNITY_LAUNCHER_SCROLLER_MODEL_TYPE_ITERATOR, UnityLauncherScrollerModelIteratorPrivate))
enum  {
	UNITY_LAUNCHER_SCROLLER_MODEL_ITERATOR_DUMMY_PROPERTY
};
gboolean unity_launcher_scroller_model_iterator_next (UnityLauncherScrollerModelIterator* self);
UnityLauncherScrollerChild* unity_launcher_scroller_model_iterator_get (UnityLauncherScrollerModelIterator* self);
static void unity_launcher_scroller_model_iterator_finalize (UnityLauncherScrollerModelIterator* obj);
static void unity_launcher_scroller_model_finalize (GObject* obj);
static void unity_launcher_scroller_model_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec);



UnityLauncherScrollerModel* unity_launcher_scroller_model_construct (GType object_type) {
	UnityLauncherScrollerModel * self;
	self = g_object_newv (object_type, 0, NULL);
	return self;
}


UnityLauncherScrollerModel* unity_launcher_scroller_model_new (void) {
	return unity_launcher_scroller_model_construct (UNITY_LAUNCHER_TYPE_SCROLLER_MODEL);
}


char* unity_launcher_scroller_model_to_string (UnityLauncherScrollerModel* self) {
	char* result = NULL;
	g_return_val_if_fail (self != NULL, NULL);
	result = g_strdup_printf ("a ScrollerModel model with %i entries", gee_collection_get_size ((GeeCollection*) self->priv->children));
	return result;
}


UnityLauncherScrollerModelIterator* unity_launcher_scroller_model_iterator (UnityLauncherScrollerModel* self) {
	UnityLauncherScrollerModelIterator* result = NULL;
	g_return_val_if_fail (self != NULL, NULL);
	result = unity_launcher_scroller_model_iterator_new (self->priv->children);
	return result;
}


gboolean unity_launcher_scroller_model_contains (UnityLauncherScrollerModel* self, UnityLauncherScrollerChild* child) {
	gboolean result = FALSE;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (child != NULL, FALSE);
	result = gee_abstract_collection_contains ((GeeAbstractCollection*) self->priv->children, child);
	return result;
}


void unity_launcher_scroller_model_add (UnityLauncherScrollerModel* self, UnityLauncherScrollerChild* child) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (child != NULL);
	gee_abstract_collection_add ((GeeAbstractCollection*) self->priv->children, child);
	g_signal_emit_by_name (self, "child-added", child);
	g_signal_emit_by_name (self, "order-changed");
}


static gpointer _g_object_ref0 (gpointer self) {
	return self ? g_object_ref (self) : NULL;
}


void unity_launcher_scroller_model_remove (UnityLauncherScrollerModel* self, UnityLauncherScrollerChild* child) {
	UnityLauncherScrollerChild* tempchild;
	g_return_if_fail (self != NULL);
	g_return_if_fail (child != NULL);
	tempchild = _g_object_ref0 (child);
	gee_abstract_collection_remove ((GeeAbstractCollection*) self->priv->children, child);
	g_signal_emit_by_name (self, "child-removed", tempchild);
	g_signal_emit_by_name (self, "order-changed");
	_g_object_unref0 (tempchild);
}


void unity_launcher_scroller_model_insert (UnityLauncherScrollerModel* self, UnityLauncherScrollerChild* child, gint i) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (child != NULL);
	gee_abstract_list_insert ((GeeAbstractList*) self->priv->children, i, child);
	g_signal_emit_by_name (self, "child-added", child);
}


void unity_launcher_scroller_model_move (UnityLauncherScrollerModel* self, UnityLauncherScrollerChild* child, gint i) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (child != NULL);
	if (!gee_abstract_collection_contains ((GeeAbstractCollection*) self->priv->children, child)) {
		return;
	}
	if (gee_abstract_list_index_of ((GeeAbstractList*) self->priv->children, child) == i) {
		return;
	}
	gee_abstract_collection_remove ((GeeAbstractCollection*) self->priv->children, child);
	gee_abstract_list_insert ((GeeAbstractList*) self->priv->children, i, child);
	g_signal_emit_by_name (self, "order-changed");
}


gint unity_launcher_scroller_model_index_of (UnityLauncherScrollerModel* self, UnityLauncherScrollerChild* child) {
	gint result = 0;
	g_return_val_if_fail (self != NULL, 0);
	g_return_val_if_fail (child != NULL, 0);
	if (gee_abstract_collection_contains ((GeeAbstractCollection*) self->priv->children, child)) {
		result = gee_abstract_list_index_of ((GeeAbstractList*) self->priv->children, child);
		return result;
	}
	result = -1;
	return result;
}


void unity_launcher_scroller_model_sort (UnityLauncherScrollerModel* self, GCompareFunc compare) {
	g_return_if_fail (self != NULL);
	gee_list_sort ((GeeList*) self->priv->children, compare);
}


UnityLauncherScrollerChild* unity_launcher_scroller_model_get (UnityLauncherScrollerModel* self, gint i) {
	UnityLauncherScrollerChild* result = NULL;
	g_return_val_if_fail (self != NULL, NULL);
	result = (UnityLauncherScrollerChild*) gee_abstract_list_get ((GeeAbstractList*) self->priv->children, i);
	return result;
}


void unity_launcher_scroller_model_set (UnityLauncherScrollerModel* self, gint i, UnityLauncherScrollerChild* item) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (item != NULL);
	gee_abstract_list_set ((GeeAbstractList*) self->priv->children, i, item);
}


gint unity_launcher_scroller_model_get_size (UnityLauncherScrollerModel* self) {
	gint result;
	g_return_val_if_fail (self != NULL, 0);
	result = gee_collection_get_size ((GeeCollection*) self->priv->children);
	return result;
}


static GObject * unity_launcher_scroller_model_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties) {
	GObject * obj;
	GObjectClass * parent_class;
	UnityLauncherScrollerModel * self;
	parent_class = G_OBJECT_CLASS (unity_launcher_scroller_model_parent_class);
	obj = parent_class->constructor (type, n_construct_properties, construct_properties);
	self = UNITY_LAUNCHER_SCROLLER_MODEL (obj);
	{
		UnityTestingObjectRegistry* _tmp0_;
		GeeArrayList* _tmp1_;
		unity_testing_object_registry_register (_tmp0_ = unity_testing_object_registry_get_default (), "UnityScrollerModel", (GObject*) self);
		_unity_testing_object_registry_unref0 (_tmp0_);
		self->priv->children = (_tmp1_ = gee_array_list_new (UNITY_LAUNCHER_TYPE_SCROLLER_CHILD, (GBoxedCopyFunc) g_object_ref, g_object_unref, NULL), _g_object_unref0 (self->priv->children), _tmp1_);
	}
	return obj;
}


UnityLauncherScrollerModelIterator* unity_launcher_scroller_model_iterator_construct (GType object_type, GeeArrayList* arraylist) {
	UnityLauncherScrollerModelIterator* self;
	GeeArrayList* _tmp0_;
	g_return_val_if_fail (arraylist != NULL, NULL);
	self = (UnityLauncherScrollerModelIterator*) g_type_create_instance (object_type);
	self->priv->array = (_tmp0_ = _g_object_ref0 (arraylist), _g_object_unref0 (self->priv->array), _tmp0_);
	return self;
}


UnityLauncherScrollerModelIterator* unity_launcher_scroller_model_iterator_new (GeeArrayList* arraylist) {
	return unity_launcher_scroller_model_iterator_construct (UNITY_LAUNCHER_SCROLLER_MODEL_TYPE_ITERATOR, arraylist);
}


gboolean unity_launcher_scroller_model_iterator_next (UnityLauncherScrollerModelIterator* self) {
	gboolean result = FALSE;
	g_return_val_if_fail (self != NULL, FALSE);
	if (self->priv->iter_index >= gee_collection_get_size ((GeeCollection*) self->priv->array)) {
		result = FALSE;
		return result;
	}
	result = TRUE;
	return result;
}


UnityLauncherScrollerChild* unity_launcher_scroller_model_iterator_get (UnityLauncherScrollerModelIterator* self) {
	UnityLauncherScrollerChild* result = NULL;
	g_return_val_if_fail (self != NULL, NULL);
	self->priv->iter_index++;
	result = (UnityLauncherScrollerChild*) gee_abstract_list_get ((GeeAbstractList*) self->priv->array, self->priv->iter_index - 1);
	return result;
}


static void unity_launcher_scroller_model_value_iterator_init (GValue* value) {
	value->data[0].v_pointer = NULL;
}


static void unity_launcher_scroller_model_value_iterator_free_value (GValue* value) {
	if (value->data[0].v_pointer) {
		unity_launcher_scroller_model_iterator_unref (value->data[0].v_pointer);
	}
}


static void unity_launcher_scroller_model_value_iterator_copy_value (const GValue* src_value, GValue* dest_value) {
	if (src_value->data[0].v_pointer) {
		dest_value->data[0].v_pointer = unity_launcher_scroller_model_iterator_ref (src_value->data[0].v_pointer);
	} else {
		dest_value->data[0].v_pointer = NULL;
	}
}


static gpointer unity_launcher_scroller_model_value_iterator_peek_pointer (const GValue* value) {
	return value->data[0].v_pointer;
}


static gchar* unity_launcher_scroller_model_value_iterator_collect_value (GValue* value, guint n_collect_values, GTypeCValue* collect_values, guint collect_flags) {
	if (collect_values[0].v_pointer) {
		UnityLauncherScrollerModelIterator* object;
		object = collect_values[0].v_pointer;
		if (object->parent_instance.g_class == NULL) {
			return g_strconcat ("invalid unclassed object pointer for value type `", G_VALUE_TYPE_NAME (value), "'", NULL);
		} else if (!g_value_type_compatible (G_TYPE_FROM_INSTANCE (object), G_VALUE_TYPE (value))) {
			return g_strconcat ("invalid object type `", g_type_name (G_TYPE_FROM_INSTANCE (object)), "' for value type `", G_VALUE_TYPE_NAME (value), "'", NULL);
		}
		value->data[0].v_pointer = unity_launcher_scroller_model_iterator_ref (object);
	} else {
		value->data[0].v_pointer = NULL;
	}
	return NULL;
}


static gchar* unity_launcher_scroller_model_value_iterator_lcopy_value (const GValue* value, guint n_collect_values, GTypeCValue* collect_values, guint collect_flags) {
	UnityLauncherScrollerModelIterator** object_p;
	object_p = collect_values[0].v_pointer;
	if (!object_p) {
		return g_strdup_printf ("value location for `%s' passed as NULL", G_VALUE_TYPE_NAME (value));
	}
	if (!value->data[0].v_pointer) {
		*object_p = NULL;
	} else if (collect_flags && G_VALUE_NOCOPY_CONTENTS) {
		*object_p = value->data[0].v_pointer;
	} else {
		*object_p = unity_launcher_scroller_model_iterator_ref (value->data[0].v_pointer);
	}
	return NULL;
}


GParamSpec* unity_launcher_scroller_model_param_spec_iterator (const gchar* name, const gchar* nick, const gchar* blurb, GType object_type, GParamFlags flags) {
	UnityLauncherScrollerModelParamSpecIterator* spec;
	g_return_val_if_fail (g_type_is_a (object_type, UNITY_LAUNCHER_SCROLLER_MODEL_TYPE_ITERATOR), NULL);
	spec = g_param_spec_internal (G_TYPE_PARAM_OBJECT, name, nick, blurb, flags);
	G_PARAM_SPEC (spec)->value_type = object_type;
	return G_PARAM_SPEC (spec);
}


gpointer unity_launcher_scroller_model_value_get_iterator (const GValue* value) {
	g_return_val_if_fail (G_TYPE_CHECK_VALUE_TYPE (value, UNITY_LAUNCHER_SCROLLER_MODEL_TYPE_ITERATOR), NULL);
	return value->data[0].v_pointer;
}


void unity_launcher_scroller_model_value_set_iterator (GValue* value, gpointer v_object) {
	UnityLauncherScrollerModelIterator* old;
	g_return_if_fail (G_TYPE_CHECK_VALUE_TYPE (value, UNITY_LAUNCHER_SCROLLER_MODEL_TYPE_ITERATOR));
	old = value->data[0].v_pointer;
	if (v_object) {
		g_return_if_fail (G_TYPE_CHECK_INSTANCE_TYPE (v_object, UNITY_LAUNCHER_SCROLLER_MODEL_TYPE_ITERATOR));
		g_return_if_fail (g_value_type_compatible (G_TYPE_FROM_INSTANCE (v_object), G_VALUE_TYPE (value)));
		value->data[0].v_pointer = v_object;
		unity_launcher_scroller_model_iterator_ref (value->data[0].v_pointer);
	} else {
		value->data[0].v_pointer = NULL;
	}
	if (old) {
		unity_launcher_scroller_model_iterator_unref (old);
	}
}


void unity_launcher_scroller_model_value_take_iterator (GValue* value, gpointer v_object) {
	UnityLauncherScrollerModelIterator* old;
	g_return_if_fail (G_TYPE_CHECK_VALUE_TYPE (value, UNITY_LAUNCHER_SCROLLER_MODEL_TYPE_ITERATOR));
	old = value->data[0].v_pointer;
	if (v_object) {
		g_return_if_fail (G_TYPE_CHECK_INSTANCE_TYPE (v_object, UNITY_LAUNCHER_SCROLLER_MODEL_TYPE_ITERATOR));
		g_return_if_fail (g_value_type_compatible (G_TYPE_FROM_INSTANCE (v_object), G_VALUE_TYPE (value)));
		value->data[0].v_pointer = v_object;
	} else {
		value->data[0].v_pointer = NULL;
	}
	if (old) {
		unity_launcher_scroller_model_iterator_unref (old);
	}
}


static void unity_launcher_scroller_model_iterator_class_init (UnityLauncherScrollerModelIteratorClass * klass) {
	unity_launcher_scroller_model_iterator_parent_class = g_type_class_peek_parent (klass);
	UNITY_LAUNCHER_SCROLLER_MODEL_ITERATOR_CLASS (klass)->finalize = unity_launcher_scroller_model_iterator_finalize;
	g_type_class_add_private (klass, sizeof (UnityLauncherScrollerModelIteratorPrivate));
}


static void unity_launcher_scroller_model_iterator_instance_init (UnityLauncherScrollerModelIterator * self) {
	self->priv = UNITY_LAUNCHER_SCROLLER_MODEL_ITERATOR_GET_PRIVATE (self);
	self->priv->iter_index = 0;
	self->ref_count = 1;
}


static void unity_launcher_scroller_model_iterator_finalize (UnityLauncherScrollerModelIterator* obj) {
	UnityLauncherScrollerModelIterator * self;
	self = UNITY_LAUNCHER_SCROLLER_MODEL_ITERATOR (obj);
	_g_object_unref0 (self->priv->array);
}


GType unity_launcher_scroller_model_iterator_get_type (void) {
	static volatile gsize unity_launcher_scroller_model_iterator_type_id__volatile = 0;
	if (g_once_init_enter (&unity_launcher_scroller_model_iterator_type_id__volatile)) {
		static const GTypeValueTable g_define_type_value_table = { unity_launcher_scroller_model_value_iterator_init, unity_launcher_scroller_model_value_iterator_free_value, unity_launcher_scroller_model_value_iterator_copy_value, unity_launcher_scroller_model_value_iterator_peek_pointer, "p", unity_launcher_scroller_model_value_iterator_collect_value, "p", unity_launcher_scroller_model_value_iterator_lcopy_value };
		static const GTypeInfo g_define_type_info = { sizeof (UnityLauncherScrollerModelIteratorClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) unity_launcher_scroller_model_iterator_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (UnityLauncherScrollerModelIterator), 0, (GInstanceInitFunc) unity_launcher_scroller_model_iterator_instance_init, &g_define_type_value_table };
		static const GTypeFundamentalInfo g_define_type_fundamental_info = { (G_TYPE_FLAG_CLASSED | G_TYPE_FLAG_INSTANTIATABLE | G_TYPE_FLAG_DERIVABLE | G_TYPE_FLAG_DEEP_DERIVABLE) };
		GType unity_launcher_scroller_model_iterator_type_id;
		unity_launcher_scroller_model_iterator_type_id = g_type_register_fundamental (g_type_fundamental_next (), "UnityLauncherScrollerModelIterator", &g_define_type_info, &g_define_type_fundamental_info, 0);
		g_once_init_leave (&unity_launcher_scroller_model_iterator_type_id__volatile, unity_launcher_scroller_model_iterator_type_id);
	}
	return unity_launcher_scroller_model_iterator_type_id__volatile;
}


gpointer unity_launcher_scroller_model_iterator_ref (gpointer instance) {
	UnityLauncherScrollerModelIterator* self;
	self = instance;
	g_atomic_int_inc (&self->ref_count);
	return instance;
}


void unity_launcher_scroller_model_iterator_unref (gpointer instance) {
	UnityLauncherScrollerModelIterator* self;
	self = instance;
	if (g_atomic_int_dec_and_test (&self->ref_count)) {
		UNITY_LAUNCHER_SCROLLER_MODEL_ITERATOR_GET_CLASS (self)->finalize (self);
		g_type_free_instance ((GTypeInstance *) self);
	}
}


static void unity_launcher_scroller_model_class_init (UnityLauncherScrollerModelClass * klass) {
	unity_launcher_scroller_model_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (UnityLauncherScrollerModelPrivate));
	G_OBJECT_CLASS (klass)->get_property = unity_launcher_scroller_model_get_property;
	G_OBJECT_CLASS (klass)->constructor = unity_launcher_scroller_model_constructor;
	G_OBJECT_CLASS (klass)->finalize = unity_launcher_scroller_model_finalize;
	g_object_class_install_property (G_OBJECT_CLASS (klass), UNITY_LAUNCHER_SCROLLER_MODEL_SIZE, g_param_spec_int ("size", "size", "size", G_MININT, G_MAXINT, 0, G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB | G_PARAM_READABLE));
	g_signal_new ("child_added", UNITY_LAUNCHER_TYPE_SCROLLER_MODEL, G_SIGNAL_RUN_LAST, 0, NULL, NULL, g_cclosure_marshal_VOID__OBJECT, G_TYPE_NONE, 1, UNITY_LAUNCHER_TYPE_SCROLLER_CHILD);
	g_signal_new ("child_removed", UNITY_LAUNCHER_TYPE_SCROLLER_MODEL, G_SIGNAL_RUN_LAST, 0, NULL, NULL, g_cclosure_marshal_VOID__OBJECT, G_TYPE_NONE, 1, UNITY_LAUNCHER_TYPE_SCROLLER_CHILD);
	g_signal_new ("order_changed", UNITY_LAUNCHER_TYPE_SCROLLER_MODEL, G_SIGNAL_RUN_LAST, 0, NULL, NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);
}


static void unity_launcher_scroller_model_instance_init (UnityLauncherScrollerModel * self) {
	self->priv = UNITY_LAUNCHER_SCROLLER_MODEL_GET_PRIVATE (self);
}


static void unity_launcher_scroller_model_finalize (GObject* obj) {
	UnityLauncherScrollerModel * self;
	self = UNITY_LAUNCHER_SCROLLER_MODEL (obj);
	_g_object_unref0 (self->priv->children);
	G_OBJECT_CLASS (unity_launcher_scroller_model_parent_class)->finalize (obj);
}


GType unity_launcher_scroller_model_get_type (void) {
	static volatile gsize unity_launcher_scroller_model_type_id__volatile = 0;
	if (g_once_init_enter (&unity_launcher_scroller_model_type_id__volatile)) {
		static const GTypeInfo g_define_type_info = { sizeof (UnityLauncherScrollerModelClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) unity_launcher_scroller_model_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (UnityLauncherScrollerModel), 0, (GInstanceInitFunc) unity_launcher_scroller_model_instance_init, NULL };
		GType unity_launcher_scroller_model_type_id;
		unity_launcher_scroller_model_type_id = g_type_register_static (G_TYPE_OBJECT, "UnityLauncherScrollerModel", &g_define_type_info, 0);
		g_once_init_leave (&unity_launcher_scroller_model_type_id__volatile, unity_launcher_scroller_model_type_id);
	}
	return unity_launcher_scroller_model_type_id__volatile;
}


static void unity_launcher_scroller_model_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec) {
	UnityLauncherScrollerModel * self;
	self = UNITY_LAUNCHER_SCROLLER_MODEL (object);
	switch (property_id) {
		case UNITY_LAUNCHER_SCROLLER_MODEL_SIZE:
		g_value_set_int (value, unity_launcher_scroller_model_get_size (self));
		break;
		default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}




