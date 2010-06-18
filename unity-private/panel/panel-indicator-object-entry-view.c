/* panel-indicator-object-entry-view.c generated by valac, the Vala compiler
 * generated from panel-indicator-object-entry-view.vala, do not modify */

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
 * Authored by: Neil Jagdish Patel <neil.patel@canonical.com>
 *              Jay Taoko <jay.taoko@canonical.com>
 */

#include <glib.h>
#include <glib-object.h>
#include <clutk/clutk.h>
#include <gtk/gtk.h>
#include <libindicator/indicator.h>
#include <libindicator/indicator-object.h>
#include <libindicator/indicator-service.h>
#include <libindicator/indicator-service-manager.h>
#include <clutter/clutter.h>
#include <float.h>
#include <math.h>
#include <cairo.h>
#include <gdk-pixbuf/gdk-pixdata.h>


#define UNITY_PANEL_INDICATORS_TYPE_INDICATOR_OBJECT_ENTRY_VIEW (unity_panel_indicators_indicator_object_entry_view_get_type ())
#define UNITY_PANEL_INDICATORS_INDICATOR_OBJECT_ENTRY_VIEW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), UNITY_PANEL_INDICATORS_TYPE_INDICATOR_OBJECT_ENTRY_VIEW, UnityPanelIndicatorsIndicatorObjectEntryView))
#define UNITY_PANEL_INDICATORS_INDICATOR_OBJECT_ENTRY_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), UNITY_PANEL_INDICATORS_TYPE_INDICATOR_OBJECT_ENTRY_VIEW, UnityPanelIndicatorsIndicatorObjectEntryViewClass))
#define UNITY_PANEL_INDICATORS_IS_INDICATOR_OBJECT_ENTRY_VIEW(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), UNITY_PANEL_INDICATORS_TYPE_INDICATOR_OBJECT_ENTRY_VIEW))
#define UNITY_PANEL_INDICATORS_IS_INDICATOR_OBJECT_ENTRY_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), UNITY_PANEL_INDICATORS_TYPE_INDICATOR_OBJECT_ENTRY_VIEW))
#define UNITY_PANEL_INDICATORS_INDICATOR_OBJECT_ENTRY_VIEW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), UNITY_PANEL_INDICATORS_TYPE_INDICATOR_OBJECT_ENTRY_VIEW, UnityPanelIndicatorsIndicatorObjectEntryViewClass))

typedef struct _UnityPanelIndicatorsIndicatorObjectEntryView UnityPanelIndicatorsIndicatorObjectEntryView;
typedef struct _UnityPanelIndicatorsIndicatorObjectEntryViewClass UnityPanelIndicatorsIndicatorObjectEntryViewClass;
typedef struct _UnityPanelIndicatorsIndicatorObjectEntryViewPrivate UnityPanelIndicatorsIndicatorObjectEntryViewPrivate;
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

#define TYPE_MENU_MANAGER (menu_manager_get_type ())
#define MENU_MANAGER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_MENU_MANAGER, MenuManager))
#define MENU_MANAGER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_MENU_MANAGER, MenuManagerClass))
#define IS_MENU_MANAGER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_MENU_MANAGER))
#define IS_MENU_MANAGER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_MENU_MANAGER))
#define MENU_MANAGER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_MENU_MANAGER, MenuManagerClass))

typedef struct _MenuManager MenuManager;
typedef struct _MenuManagerClass MenuManagerClass;

#define UNITY_PANEL_INDICATORS_TYPE_INDICATOR_OBJECT_VIEW (unity_panel_indicators_indicator_object_view_get_type ())
#define UNITY_PANEL_INDICATORS_INDICATOR_OBJECT_VIEW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), UNITY_PANEL_INDICATORS_TYPE_INDICATOR_OBJECT_VIEW, UnityPanelIndicatorsIndicatorObjectView))
#define UNITY_PANEL_INDICATORS_INDICATOR_OBJECT_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), UNITY_PANEL_INDICATORS_TYPE_INDICATOR_OBJECT_VIEW, UnityPanelIndicatorsIndicatorObjectViewClass))
#define UNITY_PANEL_INDICATORS_IS_INDICATOR_OBJECT_VIEW(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), UNITY_PANEL_INDICATORS_TYPE_INDICATOR_OBJECT_VIEW))
#define UNITY_PANEL_INDICATORS_IS_INDICATOR_OBJECT_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), UNITY_PANEL_INDICATORS_TYPE_INDICATOR_OBJECT_VIEW))
#define UNITY_PANEL_INDICATORS_INDICATOR_OBJECT_VIEW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), UNITY_PANEL_INDICATORS_TYPE_INDICATOR_OBJECT_VIEW, UnityPanelIndicatorsIndicatorObjectViewClass))

typedef struct _UnityPanelIndicatorsIndicatorObjectView UnityPanelIndicatorsIndicatorObjectView;
typedef struct _UnityPanelIndicatorsIndicatorObjectViewClass UnityPanelIndicatorsIndicatorObjectViewClass;
#define _cairo_destroy0(var) ((var == NULL) ? NULL : (var = (cairo_destroy (var), NULL)))
#define _cairo_pattern_destroy0(var) ((var == NULL) ? NULL : (var = (cairo_pattern_destroy (var), NULL)))
#define _g_free0(var) (var = (g_free (var), NULL))

struct _UnityPanelIndicatorsIndicatorObjectEntryView {
	CtkBox parent_instance;
	UnityPanelIndicatorsIndicatorObjectEntryViewPrivate * priv;
	CtkImage* image;
	CtkText* text;
};

struct _UnityPanelIndicatorsIndicatorObjectEntryViewClass {
	CtkBoxClass parent_class;
};

struct _UnityPanelIndicatorsIndicatorObjectEntryViewPrivate {
	IndicatorObjectEntry* _entry;
	ClutterCairoTexture* bg;
	gboolean menu_is_open;
	guint32 click_time;
	float last_found_entry_x;
	float last_width;
	float last_height;
};


static gpointer unity_panel_indicators_indicator_object_entry_view_parent_class = NULL;

GType unity_panel_indicators_indicator_object_entry_view_get_type (void);
#define UNITY_PANEL_INDICATORS_INDICATOR_OBJECT_ENTRY_VIEW_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), UNITY_PANEL_INDICATORS_TYPE_INDICATOR_OBJECT_ENTRY_VIEW, UnityPanelIndicatorsIndicatorObjectEntryViewPrivate))
enum  {
	UNITY_PANEL_INDICATORS_INDICATOR_OBJECT_ENTRY_VIEW_DUMMY_PROPERTY,
	UNITY_PANEL_INDICATORS_INDICATOR_OBJECT_ENTRY_VIEW_ENTRY
};
UnityPanelIndicatorsIndicatorObjectEntryView* unity_panel_indicators_indicator_object_entry_view_new (IndicatorObjectEntry* _entry);
UnityPanelIndicatorsIndicatorObjectEntryView* unity_panel_indicators_indicator_object_entry_view_construct (GType object_type, IndicatorObjectEntry* _entry);
static void unity_panel_indicators_indicator_object_entry_view_position_menu (UnityPanelIndicatorsIndicatorObjectEntryView* self, GtkMenu* menu, gint* x, gint* y, gboolean* push_in);
IndicatorObjectEntry* unity_panel_indicators_indicator_object_entry_view_get_entry (UnityPanelIndicatorsIndicatorObjectEntryView* self);
GType menu_manager_get_type (void);
MenuManager* menu_manager_get_default (void);
void menu_manager_register_visible_menu (MenuManager* self, GtkMenu* menu);
static void _unity_panel_indicators_indicator_object_entry_view_position_menu_gtk_menu_position_func (GtkMenu* menu, gint* x, gint* y, gboolean* push_in, gpointer self);
void unity_panel_indicators_indicator_object_entry_view_menu_shown (UnityPanelIndicatorsIndicatorObjectEntryView* self);
void unity_panel_indicators_indicator_object_entry_view_show_menu (UnityPanelIndicatorsIndicatorObjectEntryView* self);
GType unity_panel_indicators_indicator_object_view_get_type (void);
IndicatorObject* unity_panel_indicators_indicator_object_view_get_indicator_object (UnityPanelIndicatorsIndicatorObjectView* self);
static gboolean unity_panel_indicators_indicator_object_entry_view_on_scroll_event (UnityPanelIndicatorsIndicatorObjectEntryView* self, ClutterEvent* e);
gboolean unity_panel_indicators_indicator_object_entry_view_on_button_press_event (UnityPanelIndicatorsIndicatorObjectEntryView* self, ClutterEvent* e);
gboolean menu_manager_menu_is_open (MenuManager* self);
gboolean unity_panel_indicators_indicator_object_entry_view_on_motion_event (UnityPanelIndicatorsIndicatorObjectEntryView* self, ClutterEvent* e);
void unity_panel_indicators_indicator_object_entry_view_menu_key_moved (UnityPanelIndicatorsIndicatorObjectEntryView* self, GtkMenuDirectionType type);
static void _unity_panel_indicators_indicator_object_entry_view_menu_key_moved_gtk_menu_shell_move_current (GtkMenuShell* _sender, GtkMenuDirectionType direction, gpointer self);
void unity_panel_indicators_indicator_object_entry_view_menu_vis_changed (UnityPanelIndicatorsIndicatorObjectEntryView* self);
static void _unity_panel_indicators_indicator_object_entry_view_menu_vis_changed_g_object_notify (GObject* _sender, GParamSpec* pspec, gpointer self);
static gboolean unity_panel_indicators_indicator_object_entry_view_update_bg (UnityPanelIndicatorsIndicatorObjectEntryView* self);
static gboolean _unity_panel_indicators_indicator_object_entry_view_update_bg_gsource_func (gpointer self);
static void unity_panel_indicators_indicator_object_entry_view_real_allocate (ClutterActor* base, const ClutterActorBox* box, ClutterAllocationFlags flags);
gboolean unity_panel_indicators_indicator_object_entry_view_is_open (UnityPanelIndicatorsIndicatorObjectEntryView* self);
static void unity_panel_indicators_indicator_object_entry_view_real_paint (ClutterActor* base);
static void unity_panel_indicators_indicator_object_entry_view_real_map (ClutterActor* base);
static void unity_panel_indicators_indicator_object_entry_view_real_unmap (ClutterActor* base);
static void unity_panel_indicators_indicator_object_entry_view_set_entry (UnityPanelIndicatorsIndicatorObjectEntryView* self, IndicatorObjectEntry* value);
static gboolean _unity_panel_indicators_indicator_object_entry_view_on_button_press_event_clutter_actor_button_press_event (ClutterActor* _sender, ClutterEvent* event, gpointer self);
static gboolean _unity_panel_indicators_indicator_object_entry_view_on_motion_event_clutter_actor_motion_event (ClutterActor* _sender, ClutterEvent* event, gpointer self);
static gboolean _unity_panel_indicators_indicator_object_entry_view_on_scroll_event_clutter_actor_scroll_event (ClutterActor* _sender, ClutterEvent* event, gpointer self);
static void _lambda0_ (UnityPanelIndicatorsIndicatorObjectEntryView* self);
static void __lambda0__g_object_notify (GObject* _sender, GParamSpec* pspec, gpointer self);
static void _lambda1_ (UnityPanelIndicatorsIndicatorObjectEntryView* self);
static void __lambda1__g_object_notify (GObject* _sender, GParamSpec* pspec, gpointer self);
static void _lambda2_ (UnityPanelIndicatorsIndicatorObjectEntryView* self);
static void __lambda2__gtk_icon_theme_changed (GtkIconTheme* _sender, gpointer self);
static void _lambda3_ (UnityPanelIndicatorsIndicatorObjectEntryView* self);
static void __lambda3__g_object_notify (GObject* _sender, GParamSpec* pspec, gpointer self);
static GObject * unity_panel_indicators_indicator_object_entry_view_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties);
static void unity_panel_indicators_indicator_object_entry_view_finalize (GObject* obj);
static void unity_panel_indicators_indicator_object_entry_view_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec);
static void unity_panel_indicators_indicator_object_entry_view_set_property (GObject * object, guint property_id, const GValue * value, GParamSpec * pspec);



UnityPanelIndicatorsIndicatorObjectEntryView* unity_panel_indicators_indicator_object_entry_view_construct (GType object_type, IndicatorObjectEntry* _entry) {
	UnityPanelIndicatorsIndicatorObjectEntryView * self;
	g_return_val_if_fail (_entry != NULL, NULL);
	self = (UnityPanelIndicatorsIndicatorObjectEntryView*) g_object_new (object_type, "entry", _entry, "orientation", CTK_ORIENTATION_HORIZONTAL, "spacing", 3, "homogeneous", FALSE, "reactive", TRUE, NULL);
	return self;
}


UnityPanelIndicatorsIndicatorObjectEntryView* unity_panel_indicators_indicator_object_entry_view_new (IndicatorObjectEntry* _entry) {
	return unity_panel_indicators_indicator_object_entry_view_construct (UNITY_PANEL_INDICATORS_TYPE_INDICATOR_OBJECT_ENTRY_VIEW, _entry);
}


static void unity_panel_indicators_indicator_object_entry_view_position_menu (UnityPanelIndicatorsIndicatorObjectEntryView* self, GtkMenu* menu, gint* x, gint* y, gboolean* push_in) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (menu != NULL);
	*y = (gint) clutter_actor_get_height ((ClutterActor*) self);
	*x = (gint) self->priv->last_found_entry_x;
}


static void _unity_panel_indicators_indicator_object_entry_view_position_menu_gtk_menu_position_func (GtkMenu* menu, gint* x, gint* y, gboolean* push_in, gpointer self) {
	unity_panel_indicators_indicator_object_entry_view_position_menu (self, menu, x, y, push_in);
}


void unity_panel_indicators_indicator_object_entry_view_show_menu (UnityPanelIndicatorsIndicatorObjectEntryView* self) {
	g_return_if_fail (self != NULL);
	if (GTK_IS_MENU (self->priv->_entry->menu)) {
		MenuManager* _tmp0_;
		self->priv->last_found_entry_x = (clutter_actor_get_x ((ClutterActor*) self) + clutter_actor_get_x (clutter_actor_get_parent ((ClutterActor*) self))) + clutter_actor_get_x (clutter_actor_get_parent (clutter_actor_get_parent ((ClutterActor*) self)));
		menu_manager_register_visible_menu (_tmp0_ = menu_manager_get_default (), self->priv->_entry->menu);
		_g_object_unref0 (_tmp0_);
		gtk_menu_popup (self->priv->_entry->menu, NULL, NULL, _unity_panel_indicators_indicator_object_entry_view_position_menu_gtk_menu_position_func, self, (guint) 1, clutter_get_current_event_time ());
		self->priv->click_time = clutter_get_current_event_time ();
		unity_panel_indicators_indicator_object_entry_view_menu_shown (self);
	}
}


static gpointer _g_object_ref0 (gpointer self) {
	return self ? g_object_ref (self) : NULL;
}


static gboolean unity_panel_indicators_indicator_object_entry_view_on_scroll_event (UnityPanelIndicatorsIndicatorObjectEntryView* self, ClutterEvent* e) {
	gboolean result = FALSE;
	ClutterScrollEvent event;
	ClutterActor* _tmp0_;
	UnityPanelIndicatorsIndicatorObjectView* parent;
	IndicatorObject* object;
	g_return_val_if_fail (self != NULL, FALSE);
	event = (*e).scroll;
	parent = _g_object_ref0 ((_tmp0_ = clutter_actor_get_parent ((ClutterActor*) self), UNITY_PANEL_INDICATORS_IS_INDICATOR_OBJECT_VIEW (_tmp0_) ? ((UnityPanelIndicatorsIndicatorObjectView*) _tmp0_) : NULL));
	object = unity_panel_indicators_indicator_object_view_get_indicator_object (parent);
	g_signal_emit_by_name (object, "scroll", 1, event.direction, NULL);
	result = TRUE;
	_g_object_unref0 (parent);
	return result;
}


gboolean unity_panel_indicators_indicator_object_entry_view_on_button_press_event (UnityPanelIndicatorsIndicatorObjectEntryView* self, ClutterEvent* e) {
	gboolean result = FALSE;
	g_return_val_if_fail (self != NULL, FALSE);
	if (GTK_IS_MENU (self->priv->_entry->menu)) {
		if (self->priv->menu_is_open) {
			gtk_menu_popdown (self->priv->_entry->menu);
			self->priv->menu_is_open = FALSE;
			result = TRUE;
			return result;
		} else {
			MenuManager* _tmp0_;
			self->priv->last_found_entry_x = (clutter_actor_get_x ((ClutterActor*) self) + clutter_actor_get_x (clutter_actor_get_parent ((ClutterActor*) self))) + clutter_actor_get_x (clutter_actor_get_parent (clutter_actor_get_parent ((ClutterActor*) self)));
			menu_manager_register_visible_menu (_tmp0_ = menu_manager_get_default (), self->priv->_entry->menu);
			_g_object_unref0 (_tmp0_);
			gtk_menu_popup (self->priv->_entry->menu, NULL, NULL, _unity_panel_indicators_indicator_object_entry_view_position_menu_gtk_menu_position_func, self, (guint) (*e).button.button, (*e).button.time);
			self->priv->click_time = clutter_get_current_event_time ();
			self->priv->menu_is_open = TRUE;
			unity_panel_indicators_indicator_object_entry_view_menu_shown (self);
		}
	}
	result = TRUE;
	return result;
}


gboolean unity_panel_indicators_indicator_object_entry_view_on_motion_event (UnityPanelIndicatorsIndicatorObjectEntryView* self, ClutterEvent* e) {
	gboolean result = FALSE;
	gboolean _tmp0_ = FALSE;
	g_return_val_if_fail (self != NULL, FALSE);
	if (GTK_IS_MENU (self->priv->_entry->menu)) {
		MenuManager* _tmp1_;
		_tmp0_ = menu_manager_menu_is_open (_tmp1_ = menu_manager_get_default ());
		_g_object_unref0 (_tmp1_);
	} else {
		_tmp0_ = FALSE;
	}
	if (_tmp0_) {
		unity_panel_indicators_indicator_object_entry_view_show_menu (self);
		result = TRUE;
		return result;
	}
	result = FALSE;
	return result;
}


static void _unity_panel_indicators_indicator_object_entry_view_menu_key_moved_gtk_menu_shell_move_current (GtkMenuShell* _sender, GtkMenuDirectionType direction, gpointer self) {
	unity_panel_indicators_indicator_object_entry_view_menu_key_moved (self, direction);
}


static void _unity_panel_indicators_indicator_object_entry_view_menu_vis_changed_g_object_notify (GObject* _sender, GParamSpec* pspec, gpointer self) {
	unity_panel_indicators_indicator_object_entry_view_menu_vis_changed (self);
}


void unity_panel_indicators_indicator_object_entry_view_menu_shown (UnityPanelIndicatorsIndicatorObjectEntryView* self) {
	g_return_if_fail (self != NULL);
	if (GTK_IS_MENU (self->priv->_entry->menu)) {
		g_signal_connect_object ((GtkMenuShell*) self->priv->_entry->menu, "move-current", (GCallback) _unity_panel_indicators_indicator_object_entry_view_menu_key_moved_gtk_menu_shell_move_current, self, 0);
		g_signal_connect_object ((GObject*) self->priv->_entry->menu, "notify::visible", (GCallback) _unity_panel_indicators_indicator_object_entry_view_menu_vis_changed_g_object_notify, self, 0);
		clutter_actor_animate ((ClutterActor*) self->priv->bg, (gulong) CLUTTER_EASE_OUT_QUAD, (guint) 200, "opacity", 255, NULL);
	}
}


void unity_panel_indicators_indicator_object_entry_view_menu_vis_changed (UnityPanelIndicatorsIndicatorObjectEntryView* self) {
	gboolean vis;
	g_return_if_fail (self != NULL);
	vis = (GTK_WIDGET_FLAGS ((GtkWidget*) self->priv->_entry->menu) & GTK_VISIBLE) != 0;
	if (vis == FALSE) {
		guint _tmp0_;
		GQuark _tmp2_;
		guint _tmp1_;
		clutter_actor_animate ((ClutterActor*) self->priv->bg, (gulong) CLUTTER_EASE_OUT_QUAD, (guint) 200, "opacity", 0, NULL);
		g_signal_parse_name ("move-current", GTK_TYPE_MENU_SHELL, &_tmp0_, NULL, FALSE);
		g_signal_handlers_disconnect_matched ((GtkMenuShell*) self->priv->_entry->menu, G_SIGNAL_MATCH_ID | G_SIGNAL_MATCH_FUNC | G_SIGNAL_MATCH_DATA, _tmp0_, 0, NULL, (GCallback) _unity_panel_indicators_indicator_object_entry_view_menu_key_moved_gtk_menu_shell_move_current, self);
		g_signal_parse_name ("notify::visible", G_TYPE_OBJECT, &_tmp1_, &_tmp2_, TRUE);
		g_signal_handlers_disconnect_matched ((GObject*) self->priv->_entry->menu, G_SIGNAL_MATCH_ID | G_SIGNAL_MATCH_DETAIL | G_SIGNAL_MATCH_FUNC | G_SIGNAL_MATCH_DATA, _tmp1_, _tmp2_, NULL, (GCallback) _unity_panel_indicators_indicator_object_entry_view_menu_vis_changed_g_object_notify, self);
		self->priv->menu_is_open = FALSE;
	}
}


void unity_panel_indicators_indicator_object_entry_view_menu_key_moved (UnityPanelIndicatorsIndicatorObjectEntryView* self, GtkMenuDirectionType type) {
	gboolean _tmp0_ = FALSE;
	g_return_if_fail (self != NULL);
	if (type != GTK_MENU_DIR_PARENT) {
		_tmp0_ = type != GTK_MENU_DIR_CHILD;
	} else {
		_tmp0_ = FALSE;
	}
	if (_tmp0_) {
		return;
	}
	g_signal_emit_by_name (self, "menu-moved", type);
}


static gboolean _unity_panel_indicators_indicator_object_entry_view_update_bg_gsource_func (gpointer self) {
	gboolean result;
	result = unity_panel_indicators_indicator_object_entry_view_update_bg (self);
	return result;
}


static void unity_panel_indicators_indicator_object_entry_view_real_allocate (ClutterActor* base, const ClutterActorBox* box, ClutterAllocationFlags flags) {
	UnityPanelIndicatorsIndicatorObjectEntryView * self;
	float width = 0.0F;
	float height = 0.0F;
	ClutterActorBox _tmp0_ = {0};
	ClutterActorBox child_box;
	gboolean _tmp1_ = FALSE;
	self = (UnityPanelIndicatorsIndicatorObjectEntryView*) base;
	CLUTTER_ACTOR_CLASS (unity_panel_indicators_indicator_object_entry_view_parent_class)->allocate ((ClutterActor*) CTK_BOX (self), box, flags);
	width = floorf ((*box).x2 - (*box).x1);
	height = floorf ((*box).y2 - (*box).y1);
	child_box = (_tmp0_.x1 = (float) 0, _tmp0_);
	child_box.x1 = (float) 0;
	child_box.x2 = width;
	child_box.y1 = (float) 0;
	child_box.y2 = height;
	if (width != self->priv->last_width) {
		_tmp1_ = TRUE;
	} else {
		_tmp1_ = height != self->priv->last_height;
	}
	if (_tmp1_) {
		self->priv->last_width = width;
		self->priv->last_height = height;
		g_idle_add_full (G_PRIORITY_DEFAULT_IDLE, _unity_panel_indicators_indicator_object_entry_view_update_bg_gsource_func, g_object_ref (self), g_object_unref);
	}
	clutter_actor_allocate ((ClutterActor*) self->priv->bg, &child_box, flags);
}


gboolean unity_panel_indicators_indicator_object_entry_view_is_open (UnityPanelIndicatorsIndicatorObjectEntryView* self) {
	gboolean result = FALSE;
	g_return_val_if_fail (self != NULL, FALSE);
	result = (GTK_WIDGET_FLAGS ((GtkWidget*) self->priv->_entry->menu) & GTK_VISIBLE) != 0;
	return result;
}


static void unity_panel_indicators_indicator_object_entry_view_real_paint (ClutterActor* base) {
	UnityPanelIndicatorsIndicatorObjectEntryView * self;
	self = (UnityPanelIndicatorsIndicatorObjectEntryView*) base;
	clutter_actor_paint ((ClutterActor*) self->priv->bg);
	CLUTTER_ACTOR_CLASS (unity_panel_indicators_indicator_object_entry_view_parent_class)->paint ((ClutterActor*) CTK_BOX (self));
}


static void unity_panel_indicators_indicator_object_entry_view_real_map (ClutterActor* base) {
	UnityPanelIndicatorsIndicatorObjectEntryView * self;
	self = (UnityPanelIndicatorsIndicatorObjectEntryView*) base;
	CLUTTER_ACTOR_CLASS (unity_panel_indicators_indicator_object_entry_view_parent_class)->map ((ClutterActor*) CTK_BOX (self));
	clutter_actor_map ((ClutterActor*) self->priv->bg);
}


static void unity_panel_indicators_indicator_object_entry_view_real_unmap (ClutterActor* base) {
	UnityPanelIndicatorsIndicatorObjectEntryView * self;
	self = (UnityPanelIndicatorsIndicatorObjectEntryView*) base;
	CLUTTER_ACTOR_CLASS (unity_panel_indicators_indicator_object_entry_view_parent_class)->unmap ((ClutterActor*) CTK_BOX (self));
	clutter_actor_unmap ((ClutterActor*) self->priv->bg);
}


static gboolean unity_panel_indicators_indicator_object_entry_view_update_bg (UnityPanelIndicatorsIndicatorObjectEntryView* self) {
	gboolean result = FALSE;
	cairo_t* cr;
	gint width;
	gint height;
	cairo_t* _tmp0_;
	cairo_pattern_t* pat;
	g_return_val_if_fail (self != NULL, FALSE);
	cr = NULL;
	width = (gint) self->priv->last_width;
	height = (gint) self->priv->last_height;
	clutter_cairo_texture_set_surface_size (self->priv->bg, (guint) width, (guint) height);
	cr = (_tmp0_ = clutter_cairo_texture_create (self->priv->bg), _cairo_destroy0 (cr), _tmp0_);
	cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
	cairo_paint (cr);
	cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
	cairo_set_line_width (cr, 1.0);
	cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.2);
	cairo_move_to (cr, (double) 1, (double) height);
	cairo_line_to (cr, (double) 1, (double) 7);
	cairo_curve_to (cr, (double) 1, (double) 2, (double) 1, (double) 2, (double) 10, (double) 2);
	cairo_line_to (cr, (double) (width - 10), (double) 2);
	cairo_curve_to (cr, (double) width, (double) 2, (double) width, (double) 2, (double) width, (double) 7);
	cairo_line_to (cr, (double) width, (double) height);
	cairo_line_to (cr, (double) 1, (double) height);
	pat = cairo_pattern_create_linear ((double) 1, (double) 0, (double) 1, (double) height);
	cairo_pattern_add_color_stop_rgba (pat, 0.0, (double) 0.8509f, (double) 0.8196f, (double) 0.7294f, (double) 1.0f);
	cairo_pattern_add_color_stop_rgba (pat, 1.0, (double) 0.7019f, (double) 0.6509f, (double) 0.5137f, (double) 1.0f);
	cairo_set_source (cr, pat);
	cairo_fill (cr);
	result = FALSE;
	_cairo_destroy0 (cr);
	_cairo_pattern_destroy0 (pat);
	return result;
}


IndicatorObjectEntry* unity_panel_indicators_indicator_object_entry_view_get_entry (UnityPanelIndicatorsIndicatorObjectEntryView* self) {
	IndicatorObjectEntry* result;
	g_return_val_if_fail (self != NULL, NULL);
	result = self->priv->_entry;
	return result;
}


static void unity_panel_indicators_indicator_object_entry_view_set_entry (UnityPanelIndicatorsIndicatorObjectEntryView* self, IndicatorObjectEntry* value) {
	g_return_if_fail (self != NULL);
	self->priv->_entry = value;
	g_object_notify ((GObject *) self, "entry");
}


static gboolean _unity_panel_indicators_indicator_object_entry_view_on_button_press_event_clutter_actor_button_press_event (ClutterActor* _sender, ClutterEvent* event, gpointer self) {
	gboolean result;
	result = unity_panel_indicators_indicator_object_entry_view_on_button_press_event (self, event);
	return result;
}


static gboolean _unity_panel_indicators_indicator_object_entry_view_on_motion_event_clutter_actor_motion_event (ClutterActor* _sender, ClutterEvent* event, gpointer self) {
	gboolean result;
	result = unity_panel_indicators_indicator_object_entry_view_on_motion_event (self, event);
	return result;
}


static gboolean _unity_panel_indicators_indicator_object_entry_view_on_scroll_event_clutter_actor_scroll_event (ClutterActor* _sender, ClutterEvent* event, gpointer self) {
	gboolean result;
	result = unity_panel_indicators_indicator_object_entry_view_on_scroll_event (self, event);
	return result;
}


static void _lambda0_ (UnityPanelIndicatorsIndicatorObjectEntryView* self) {
	GdkPixbuf* _tmp1_;
	GdkPixbuf* _tmp0_ = NULL;
	gboolean _tmp2_;
	if ((_tmp2_ = GDK_IS_PIXBUF (_tmp1_ = (g_object_get (self->priv->_entry->image, "pixbuf", &_tmp0_, NULL), _tmp0_)), _g_object_unref0 (_tmp1_), _tmp2_)) {
		GdkPixbuf* _tmp4_;
		GdkPixbuf* _tmp3_ = NULL;
		GdkPixbuf* _tmp6_;
		GdkPixbuf* _tmp5_ = NULL;
		g_object_set (self->image, "pixbuf", _tmp4_ = (g_object_get (self->priv->_entry->image, "pixbuf", &_tmp3_, NULL), _tmp3_), NULL);
		_g_object_unref0 (_tmp4_);
		ctk_image_set_size (self->image, gdk_pixbuf_get_width (_tmp6_ = (g_object_get (self->priv->_entry->image, "pixbuf", &_tmp5_, NULL), _tmp5_)));
		_g_object_unref0 (_tmp6_);
	}
}


static void __lambda0__g_object_notify (GObject* _sender, GParamSpec* pspec, gpointer self) {
	_lambda0_ (self);
}


static void _lambda1_ (UnityPanelIndicatorsIndicatorObjectEntryView* self) {
	char* _tmp1_;
	char* _tmp0_ = NULL;
	gboolean _tmp2_;
	if ((_tmp2_ = (_tmp1_ = (g_object_get (self->priv->_entry->image, "icon-name", &_tmp0_, NULL), _tmp0_)) != NULL, _g_free0 (_tmp1_), _tmp2_)) {
		char* _tmp4_;
		char* _tmp3_ = NULL;
		g_object_set (self->image, "stock-id", _tmp4_ = (g_object_get (self->priv->_entry->image, "icon-name", &_tmp3_, NULL), _tmp3_), NULL);
		_g_free0 (_tmp4_);
		ctk_image_set_size (self->image, 22);
	}
}


static void __lambda1__g_object_notify (GObject* _sender, GParamSpec* pspec, gpointer self) {
	_lambda1_ (self);
}


static void _lambda2_ (UnityPanelIndicatorsIndicatorObjectEntryView* self) {
	char* _tmp1_;
	char* _tmp0_ = NULL;
	gboolean _tmp2_;
	if ((_tmp2_ = (_tmp1_ = (g_object_get (self->priv->_entry->image, "icon-name", &_tmp0_, NULL), _tmp0_)) != NULL, _g_free0 (_tmp1_), _tmp2_)) {
		char* _tmp4_;
		char* _tmp3_ = NULL;
		g_object_set (self->image, "stock-id", _tmp4_ = (g_object_get (self->priv->_entry->image, "icon-name", &_tmp3_, NULL), _tmp3_), NULL);
		_g_free0 (_tmp4_);
	}
}


static void __lambda2__gtk_icon_theme_changed (GtkIconTheme* _sender, gpointer self) {
	_lambda2_ (self);
}


static void _lambda3_ (UnityPanelIndicatorsIndicatorObjectEntryView* self) {
	clutter_text_set_text ((ClutterText*) self->text, gtk_label_get_label (self->priv->_entry->label));
}


static void __lambda3__g_object_notify (GObject* _sender, GParamSpec* pspec, gpointer self) {
	_lambda3_ (self);
}


static GObject * unity_panel_indicators_indicator_object_entry_view_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties) {
	GObject * obj;
	GObjectClass * parent_class;
	UnityPanelIndicatorsIndicatorObjectEntryView * self;
	parent_class = G_OBJECT_CLASS (unity_panel_indicators_indicator_object_entry_view_parent_class);
	obj = parent_class->constructor (type, n_construct_properties, construct_properties);
	self = UNITY_PANEL_INDICATORS_INDICATOR_OBJECT_ENTRY_VIEW (obj);
	{
		CtkPadding _tmp0_ = {0};
		CtkPadding _tmp1_;
		ClutterCairoTexture* _tmp2_;
		GtkIconTheme* theme;
		ctk_actor_set_padding ((CtkActor*) self, (_tmp1_ = (_tmp0_.top = (float) 0, _tmp0_.right = 4.0f, _tmp0_.bottom = (float) 0, _tmp0_.left = 4.0f, _tmp0_), &_tmp1_));
		g_signal_connect_object ((ClutterActor*) self, "button-press-event", (GCallback) _unity_panel_indicators_indicator_object_entry_view_on_button_press_event_clutter_actor_button_press_event, self, 0);
		g_signal_connect_object ((ClutterActor*) self, "motion-event", (GCallback) _unity_panel_indicators_indicator_object_entry_view_on_motion_event_clutter_actor_motion_event, self, 0);
		g_signal_connect_object ((ClutterActor*) self, "scroll-event", (GCallback) _unity_panel_indicators_indicator_object_entry_view_on_scroll_event_clutter_actor_scroll_event, self, 0);
		self->priv->bg = (_tmp2_ = g_object_ref_sink ((ClutterCairoTexture*) clutter_cairo_texture_new ((guint) 10, (guint) 10)), _g_object_unref0 (self->priv->bg), _tmp2_);
		clutter_actor_set_parent ((ClutterActor*) self->priv->bg, (ClutterActor*) self);
		clutter_actor_set_opacity ((ClutterActor*) self->priv->bg, (guint8) 0);
		clutter_actor_show ((ClutterActor*) self->priv->bg);
		if (GTK_IS_IMAGE (self->priv->_entry->image)) {
			CtkImage* _tmp3_;
			char* _tmp5_;
			char* _tmp4_ = NULL;
			gboolean _tmp6_;
			GdkPixbuf* _tmp10_;
			GdkPixbuf* _tmp9_ = NULL;
			gboolean _tmp11_;
			self->image = (_tmp3_ = g_object_ref_sink ((CtkImage*) ctk_image_new ((guint) 22)), _g_object_unref0 (self->image), _tmp3_);
			clutter_container_add_actor ((ClutterContainer*) self, (ClutterActor*) self->image);
			clutter_actor_show ((ClutterActor*) self->image);
			if ((_tmp6_ = (_tmp5_ = (g_object_get (self->priv->_entry->image, "icon-name", &_tmp4_, NULL), _tmp4_)) != NULL, _g_free0 (_tmp5_), _tmp6_)) {
				char* _tmp8_;
				char* _tmp7_ = NULL;
				g_object_set (self->image, "stock-id", _tmp8_ = (g_object_get (self->priv->_entry->image, "icon-name", &_tmp7_, NULL), _tmp7_), NULL);
				_g_free0 (_tmp8_);
			}
			if ((_tmp11_ = (_tmp10_ = (g_object_get (self->priv->_entry->image, "pixbuf", &_tmp9_, NULL), _tmp9_)) != NULL, _g_object_unref0 (_tmp10_), _tmp11_)) {
				GdkPixbuf* _tmp13_;
				GdkPixbuf* _tmp12_ = NULL;
				GdkPixbuf* _tmp15_;
				GdkPixbuf* _tmp14_ = NULL;
				g_object_set (self->image, "pixbuf", _tmp13_ = (g_object_get (self->priv->_entry->image, "pixbuf", &_tmp12_, NULL), _tmp12_), NULL);
				_g_object_unref0 (_tmp13_);
				ctk_image_set_size (self->image, gdk_pixbuf_get_width (_tmp15_ = (g_object_get (self->priv->_entry->image, "pixbuf", &_tmp14_, NULL), _tmp14_)));
				_g_object_unref0 (_tmp15_);
			}
		}
		g_signal_connect_object ((GObject*) self->priv->_entry->image, "notify::pixbuf", (GCallback) __lambda0__g_object_notify, self, 0);
		g_signal_connect_object ((GObject*) self->priv->_entry->image, "notify::icon-name", (GCallback) __lambda1__g_object_notify, self, 0);
		theme = gtk_icon_theme_get_default ();
		g_signal_connect_object (theme, "changed", (GCallback) __lambda2__gtk_icon_theme_changed, self, 0);
		if (GTK_IS_LABEL (self->priv->_entry->label)) {
			CtkText* _tmp16_;
			ClutterColor _tmp17_ = {0};
			ClutterColor _tmp18_;
			self->text = (_tmp16_ = g_object_ref_sink ((CtkText*) ctk_text_new ("")), _g_object_unref0 (self->text), _tmp16_);
			clutter_text_set_color ((ClutterText*) self->text, (_tmp18_ = (_tmp17_.red = (guint8) 233, _tmp17_.green = (guint8) 216, _tmp17_.blue = (guint8) 200, _tmp17_.alpha = (guint8) 255, _tmp17_), &_tmp18_));
			clutter_container_add_actor ((ClutterContainer*) self, (ClutterActor*) self->text);
			clutter_actor_show ((ClutterActor*) self->text);
			clutter_text_set_text ((ClutterText*) self->text, gtk_label_get_label (self->priv->_entry->label));
			g_signal_connect_object ((GObject*) self->priv->_entry->label, "notify::label", (GCallback) __lambda3__g_object_notify, self, 0);
		}
	}
	return obj;
}


static void unity_panel_indicators_indicator_object_entry_view_class_init (UnityPanelIndicatorsIndicatorObjectEntryViewClass * klass) {
	unity_panel_indicators_indicator_object_entry_view_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (UnityPanelIndicatorsIndicatorObjectEntryViewPrivate));
	CLUTTER_ACTOR_CLASS (klass)->allocate = unity_panel_indicators_indicator_object_entry_view_real_allocate;
	CLUTTER_ACTOR_CLASS (klass)->paint = unity_panel_indicators_indicator_object_entry_view_real_paint;
	CLUTTER_ACTOR_CLASS (klass)->map = unity_panel_indicators_indicator_object_entry_view_real_map;
	CLUTTER_ACTOR_CLASS (klass)->unmap = unity_panel_indicators_indicator_object_entry_view_real_unmap;
	G_OBJECT_CLASS (klass)->get_property = unity_panel_indicators_indicator_object_entry_view_get_property;
	G_OBJECT_CLASS (klass)->set_property = unity_panel_indicators_indicator_object_entry_view_set_property;
	G_OBJECT_CLASS (klass)->constructor = unity_panel_indicators_indicator_object_entry_view_constructor;
	G_OBJECT_CLASS (klass)->finalize = unity_panel_indicators_indicator_object_entry_view_finalize;
	g_object_class_install_property (G_OBJECT_CLASS (klass), UNITY_PANEL_INDICATORS_INDICATOR_OBJECT_ENTRY_VIEW_ENTRY, g_param_spec_pointer ("entry", "entry", "entry", G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB | G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	g_signal_new ("menu_moved", UNITY_PANEL_INDICATORS_TYPE_INDICATOR_OBJECT_ENTRY_VIEW, G_SIGNAL_RUN_LAST, 0, NULL, NULL, g_cclosure_marshal_VOID__ENUM, G_TYPE_NONE, 1, GTK_TYPE_MENU_DIRECTION_TYPE);
}


static void unity_panel_indicators_indicator_object_entry_view_instance_init (UnityPanelIndicatorsIndicatorObjectEntryView * self) {
	self->priv = UNITY_PANEL_INDICATORS_INDICATOR_OBJECT_ENTRY_VIEW_GET_PRIVATE (self);
	self->priv->menu_is_open = FALSE;
	self->priv->last_found_entry_x = 0.0f;
	self->priv->last_width = (float) 0;
	self->priv->last_height = (float) 0;
}


static void unity_panel_indicators_indicator_object_entry_view_finalize (GObject* obj) {
	UnityPanelIndicatorsIndicatorObjectEntryView * self;
	self = UNITY_PANEL_INDICATORS_INDICATOR_OBJECT_ENTRY_VIEW (obj);
	_g_object_unref0 (self->priv->bg);
	_g_object_unref0 (self->image);
	_g_object_unref0 (self->text);
	G_OBJECT_CLASS (unity_panel_indicators_indicator_object_entry_view_parent_class)->finalize (obj);
}


GType unity_panel_indicators_indicator_object_entry_view_get_type (void) {
	static volatile gsize unity_panel_indicators_indicator_object_entry_view_type_id__volatile = 0;
	if (g_once_init_enter (&unity_panel_indicators_indicator_object_entry_view_type_id__volatile)) {
		static const GTypeInfo g_define_type_info = { sizeof (UnityPanelIndicatorsIndicatorObjectEntryViewClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) unity_panel_indicators_indicator_object_entry_view_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (UnityPanelIndicatorsIndicatorObjectEntryView), 0, (GInstanceInitFunc) unity_panel_indicators_indicator_object_entry_view_instance_init, NULL };
		GType unity_panel_indicators_indicator_object_entry_view_type_id;
		unity_panel_indicators_indicator_object_entry_view_type_id = g_type_register_static (CTK_TYPE_BOX, "UnityPanelIndicatorsIndicatorObjectEntryView", &g_define_type_info, 0);
		g_once_init_leave (&unity_panel_indicators_indicator_object_entry_view_type_id__volatile, unity_panel_indicators_indicator_object_entry_view_type_id);
	}
	return unity_panel_indicators_indicator_object_entry_view_type_id__volatile;
}


static void unity_panel_indicators_indicator_object_entry_view_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec) {
	UnityPanelIndicatorsIndicatorObjectEntryView * self;
	self = UNITY_PANEL_INDICATORS_INDICATOR_OBJECT_ENTRY_VIEW (object);
	switch (property_id) {
		case UNITY_PANEL_INDICATORS_INDICATOR_OBJECT_ENTRY_VIEW_ENTRY:
		g_value_set_pointer (value, unity_panel_indicators_indicator_object_entry_view_get_entry (self));
		break;
		default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}


static void unity_panel_indicators_indicator_object_entry_view_set_property (GObject * object, guint property_id, const GValue * value, GParamSpec * pspec) {
	UnityPanelIndicatorsIndicatorObjectEntryView * self;
	self = UNITY_PANEL_INDICATORS_INDICATOR_OBJECT_ENTRY_VIEW (object);
	switch (property_id) {
		case UNITY_PANEL_INDICATORS_INDICATOR_OBJECT_ENTRY_VIEW_ENTRY:
		unity_panel_indicators_indicator_object_entry_view_set_entry (self, g_value_get_pointer (value));
		break;
		default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}




