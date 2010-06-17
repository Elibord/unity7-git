/* panel-indicator-background.c generated by valac, the Vala compiler
 * generated from panel-indicator-background.vala, do not modify */

/*
 * Copyright (C) 2010 Canonical Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by Neil Jagdish Patel <neil.patel@canonical.com>
 *
 */

#include <glib.h>
#include <glib-object.h>
#include <clutter/clutter.h>
#include <gdk-pixbuf/gdk-pixdata.h>
#include <stdlib.h>
#include <string.h>
#include <cairo.h>
#include <float.h>
#include <math.h>
#include <gdk/gdk.h>


#define UNITY_PANEL_TYPE_INDICATOR_BACKGROUND (unity_panel_indicator_background_get_type ())
#define UNITY_PANEL_INDICATOR_BACKGROUND(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), UNITY_PANEL_TYPE_INDICATOR_BACKGROUND, UnityPanelIndicatorBackground))
#define UNITY_PANEL_INDICATOR_BACKGROUND_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), UNITY_PANEL_TYPE_INDICATOR_BACKGROUND, UnityPanelIndicatorBackgroundClass))
#define UNITY_PANEL_IS_INDICATOR_BACKGROUND(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), UNITY_PANEL_TYPE_INDICATOR_BACKGROUND))
#define UNITY_PANEL_IS_INDICATOR_BACKGROUND_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), UNITY_PANEL_TYPE_INDICATOR_BACKGROUND))
#define UNITY_PANEL_INDICATOR_BACKGROUND_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), UNITY_PANEL_TYPE_INDICATOR_BACKGROUND, UnityPanelIndicatorBackgroundClass))

typedef struct _UnityPanelIndicatorBackground UnityPanelIndicatorBackground;
typedef struct _UnityPanelIndicatorBackgroundClass UnityPanelIndicatorBackgroundClass;
typedef struct _UnityPanelIndicatorBackgroundPrivate UnityPanelIndicatorBackgroundPrivate;
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
#define _cairo_destroy0(var) ((var == NULL) ? NULL : (var = (cairo_destroy (var), NULL)))
#define _cairo_pattern_destroy0(var) ((var == NULL) ? NULL : (var = (cairo_pattern_destroy (var), NULL)))
#define _g_error_free0(var) ((var == NULL) ? NULL : (var = (g_error_free (var), NULL)))

struct _UnityPanelIndicatorBackground {
	ClutterCairoTexture parent_instance;
	UnityPanelIndicatorBackgroundPrivate * priv;
};

struct _UnityPanelIndicatorBackgroundClass {
	ClutterCairoTextureClass parent_class;
};

struct _UnityPanelIndicatorBackgroundPrivate {
	gint last_width;
	gint last_height;
	GdkPixbuf* tile;
};


static gpointer unity_panel_indicator_background_parent_class = NULL;

GType unity_panel_indicator_background_get_type (void);
#define UNITY_PANEL_INDICATOR_BACKGROUND_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), UNITY_PANEL_TYPE_INDICATOR_BACKGROUND, UnityPanelIndicatorBackgroundPrivate))
enum  {
	UNITY_PANEL_INDICATOR_BACKGROUND_DUMMY_PROPERTY
};
#define UNITY_PANEL_INDICATOR_BACKGROUND_BG "/usr/share/unity/themes/panel_background.png"
UnityPanelIndicatorBackground* unity_panel_indicator_background_new (void);
UnityPanelIndicatorBackground* unity_panel_indicator_background_construct (GType object_type);
static gboolean unity_panel_indicator_background_update_background (UnityPanelIndicatorBackground* self);
static gboolean _unity_panel_indicator_background_update_background_gsource_func (gpointer self);
static void unity_panel_indicator_background_real_allocate (ClutterActor* base, const ClutterActorBox* box, ClutterAllocationFlags flags);
static GObject * unity_panel_indicator_background_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties);
static void unity_panel_indicator_background_finalize (GObject* obj);



UnityPanelIndicatorBackground* unity_panel_indicator_background_construct (GType object_type) {
	UnityPanelIndicatorBackground * self;
	self = (UnityPanelIndicatorBackground*) g_object_new (object_type, NULL);
	return self;
}


UnityPanelIndicatorBackground* unity_panel_indicator_background_new (void) {
	return unity_panel_indicator_background_construct (UNITY_PANEL_TYPE_INDICATOR_BACKGROUND);
}


static gboolean _unity_panel_indicator_background_update_background_gsource_func (gpointer self) {
	gboolean result;
	result = unity_panel_indicator_background_update_background (self);
	return result;
}


static void unity_panel_indicator_background_real_allocate (ClutterActor* base, const ClutterActorBox* box, ClutterAllocationFlags flags) {
	UnityPanelIndicatorBackground * self;
	gint width;
	gint height;
	ClutterActorBox child = {0};
	gboolean _tmp0_ = FALSE;
	self = (UnityPanelIndicatorBackground*) base;
	width = ((gint) ((*box).x2 - (*box).x1)) + 12;
	height = (gint) ((*box).y2 - (*box).y1);
	child = *box;
	child.x1 = child.x1 - 12;
	child.x2 = child.x2 + 12;
	CLUTTER_ACTOR_CLASS (unity_panel_indicator_background_parent_class)->allocate ((ClutterActor*) CLUTTER_CAIRO_TEXTURE (self), &child, flags);
	if (width != self->priv->last_width) {
		_tmp0_ = TRUE;
	} else {
		_tmp0_ = height != self->priv->last_height;
	}
	if (_tmp0_) {
		self->priv->last_width = width;
		self->priv->last_height = height;
		g_idle_add_full (G_PRIORITY_DEFAULT_IDLE, _unity_panel_indicator_background_update_background_gsource_func, g_object_ref (self), g_object_unref);
	}
}


static gpointer _cairo_pattern_reference0 (gpointer self) {
	return self ? cairo_pattern_reference (self) : NULL;
}


static gboolean unity_panel_indicator_background_update_background (UnityPanelIndicatorBackground* self) {
	gboolean result = FALSE;
	cairo_t* cr;
	cairo_t* _tmp0_;
	g_return_val_if_fail (self != NULL, FALSE);
	cr = NULL;
	clutter_cairo_texture_set_surface_size ((ClutterCairoTexture*) self, (guint) self->priv->last_width, (guint) self->priv->last_height);
	cr = (_tmp0_ = clutter_cairo_texture_create ((ClutterCairoTexture*) self), _cairo_destroy0 (cr), _tmp0_);
	cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
	cairo_paint (cr);
	cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
	cairo_set_line_width (cr, 1.0);
	cairo_move_to (cr, (double) (-1), (double) (-1));
	cairo_line_to (cr, (double) (self->priv->last_width + 1), (double) (-1));
	cairo_line_to (cr, (double) (self->priv->last_width + 1), (double) self->priv->last_height);
	cairo_line_to (cr, (double) 12, (double) self->priv->last_height);
	cairo_arc (cr, 12.0, (double) (self->priv->last_height - 6.0f), (double) 6, (-G_PI) / 2.0, (1.5 * G_PI) / 2.0);
	cairo_line_to (cr, (double) (-1), (double) (-1));
	cairo_set_source_rgb (cr, 61 / 255.0, 60 / 255.0, 56 / 255.0);
	cairo_fill (cr);
	cairo_move_to (cr, (double) (-1), (double) (-1));
	cairo_line_to (cr, (double) (self->priv->last_width + 1), (double) (-1));
	cairo_line_to (cr, (double) (self->priv->last_width + 1), (double) (self->priv->last_height - 3));
	cairo_line_to (cr, (double) 12, (double) (self->priv->last_height - 3));
	cairo_arc (cr, 12.0, (double) (self->priv->last_height - 9.0f), (double) 6, (-G_PI) / 2.0, (1.5 * G_PI) / 2.0);
	cairo_line_to (cr, (double) (-1), (double) (-1));
	if (GDK_IS_PIXBUF (self->priv->tile)) {
		cairo_pattern_t* pat;
		gdk_cairo_set_source_pixbuf (cr, self->priv->tile, (double) 0, (double) 0);
		pat = _cairo_pattern_reference0 (cairo_get_source (cr));
		cairo_pattern_set_extend (pat, CAIRO_EXTEND_REPEAT);
		cairo_fill (cr);
		_cairo_pattern_destroy0 (pat);
	}
	result = FALSE;
	_cairo_destroy0 (cr);
	return result;
}


static GObject * unity_panel_indicator_background_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties) {
	GObject * obj;
	GObjectClass * parent_class;
	UnityPanelIndicatorBackground * self;
	GError * _inner_error_;
	parent_class = G_OBJECT_CLASS (unity_panel_indicator_background_parent_class);
	obj = parent_class->constructor (type, n_construct_properties, construct_properties);
	self = UNITY_PANEL_INDICATOR_BACKGROUND (obj);
	_inner_error_ = NULL;
	{
		{
			GdkPixbuf* _tmp0_;
			GdkPixbuf* _tmp1_;
			_tmp0_ = gdk_pixbuf_new_from_file (UNITY_PANEL_INDICATOR_BACKGROUND_BG, &_inner_error_);
			if (_inner_error_ != NULL) {
				goto __catch3_g_error;
			}
			self->priv->tile = (_tmp1_ = _tmp0_, _g_object_unref0 (self->priv->tile), _tmp1_);
		}
		goto __finally3;
		__catch3_g_error:
		{
			GError * e;
			e = _inner_error_;
			_inner_error_ = NULL;
			{
				g_warning ("panel-indicator-background.vala:46: Unable to load panel background");
				_g_error_free0 (e);
			}
		}
		__finally3:
		if (_inner_error_ != NULL) {
			g_critical ("file %s: line %d: uncaught error: %s (%s, %d)", __FILE__, __LINE__, _inner_error_->message, g_quark_to_string (_inner_error_->domain), _inner_error_->code);
			g_clear_error (&_inner_error_);
		}
	}
	return obj;
}


static void unity_panel_indicator_background_class_init (UnityPanelIndicatorBackgroundClass * klass) {
	unity_panel_indicator_background_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (UnityPanelIndicatorBackgroundPrivate));
	CLUTTER_ACTOR_CLASS (klass)->allocate = unity_panel_indicator_background_real_allocate;
	G_OBJECT_CLASS (klass)->constructor = unity_panel_indicator_background_constructor;
	G_OBJECT_CLASS (klass)->finalize = unity_panel_indicator_background_finalize;
}


static void unity_panel_indicator_background_instance_init (UnityPanelIndicatorBackground * self) {
	self->priv = UNITY_PANEL_INDICATOR_BACKGROUND_GET_PRIVATE (self);
	self->priv->last_width = 0;
	self->priv->last_height = 0;
	self->priv->tile = NULL;
}


static void unity_panel_indicator_background_finalize (GObject* obj) {
	UnityPanelIndicatorBackground * self;
	self = UNITY_PANEL_INDICATOR_BACKGROUND (obj);
	_g_object_unref0 (self->priv->tile);
	G_OBJECT_CLASS (unity_panel_indicator_background_parent_class)->finalize (obj);
}


GType unity_panel_indicator_background_get_type (void) {
	static volatile gsize unity_panel_indicator_background_type_id__volatile = 0;
	if (g_once_init_enter (&unity_panel_indicator_background_type_id__volatile)) {
		static const GTypeInfo g_define_type_info = { sizeof (UnityPanelIndicatorBackgroundClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) unity_panel_indicator_background_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (UnityPanelIndicatorBackground), 0, (GInstanceInitFunc) unity_panel_indicator_background_instance_init, NULL };
		GType unity_panel_indicator_background_type_id;
		unity_panel_indicator_background_type_id = g_type_register_static (CLUTTER_TYPE_CAIRO_TEXTURE, "UnityPanelIndicatorBackground", &g_define_type_info, 0);
		g_once_init_leave (&unity_panel_indicator_background_type_id__volatile, unity_panel_indicator_background_type_id);
	}
	return unity_panel_indicator_background_type_id__volatile;
}




