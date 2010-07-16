/* test-director.c generated by valac, the Vala compiler
 * generated from test-director.vala, do not modify */

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
#include <gtk/gtk.h>
#include <float.h>
#include <math.h>
#include <string.h>


#define UNITY_TESTING_TYPE_DIRECTOR (unity_testing_director_get_type ())
#define UNITY_TESTING_DIRECTOR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), UNITY_TESTING_TYPE_DIRECTOR, UnityTestingDirector))
#define UNITY_TESTING_DIRECTOR_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), UNITY_TESTING_TYPE_DIRECTOR, UnityTestingDirectorClass))
#define UNITY_TESTING_IS_DIRECTOR(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), UNITY_TESTING_TYPE_DIRECTOR))
#define UNITY_TESTING_IS_DIRECTOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), UNITY_TESTING_TYPE_DIRECTOR))
#define UNITY_TESTING_DIRECTOR_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), UNITY_TESTING_TYPE_DIRECTOR, UnityTestingDirectorClass))

typedef struct _UnityTestingDirector UnityTestingDirector;
typedef struct _UnityTestingDirectorClass UnityTestingDirectorClass;
typedef struct _UnityTestingDirectorPrivate UnityTestingDirectorPrivate;
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

struct _UnityTestingDirector {
	GObject parent_instance;
	UnityTestingDirectorPrivate * priv;
};

struct _UnityTestingDirectorClass {
	GObjectClass parent_class;
};

struct _UnityTestingDirectorPrivate {
	ClutterStage* _stage;
	gboolean break_loop;
};


static gpointer unity_testing_director_parent_class = NULL;

GType unity_testing_director_get_type (void);
#define UNITY_TESTING_DIRECTOR_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), UNITY_TESTING_TYPE_DIRECTOR, UnityTestingDirectorPrivate))
enum  {
	UNITY_TESTING_DIRECTOR_DUMMY_PROPERTY,
	UNITY_TESTING_DIRECTOR_STAGE
};
UnityTestingDirector* unity_testing_director_new (ClutterStage* stage);
UnityTestingDirector* unity_testing_director_construct (GType object_type, ClutterStage* stage);
static void unity_testing_director_do_event (UnityTestingDirector* self, ClutterActor* actor, ClutterEvent* event, gboolean capture_phase);
void unity_testing_director_do_wait_for_animation (UnityTestingDirector* self, ClutterActor* actor);
static gboolean _lambda37_ (UnityTestingDirector* self);
static gboolean __lambda37__gsource_func (gpointer self);
void unity_testing_director_do_wait_for_timeout (UnityTestingDirector* self, guint32 msecs);
void unity_testing_director_button_press (UnityTestingDirector* self, ClutterActor* actor, guint32 button, gboolean autorelease, float relative_x, float relative_y, gboolean wait_for_animation);
void unity_testing_director_button_release (UnityTestingDirector* self, ClutterActor* actor, guint32 button, float relative_x, float relative_y);
void unity_testing_director_enter_event (UnityTestingDirector* self, ClutterActor* actor, float relative_x, float relative_y);
void unity_testing_director_leave_event (UnityTestingDirector* self, ClutterActor* actor, float relative_x, float relative_y);
void unity_testing_director_motion_event (UnityTestingDirector* self, ClutterActor* actor, float relative_startx, float relative_starty, float relative_endx, float relative_endy);
ClutterStage* unity_testing_director_get_stage (UnityTestingDirector* self);
static void unity_testing_director_set_stage (UnityTestingDirector* self, ClutterStage* value);
static GObject * unity_testing_director_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties);
static void unity_testing_director_finalize (GObject* obj);
static void unity_testing_director_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec);
static void unity_testing_director_set_property (GObject * object, guint property_id, const GValue * value, GParamSpec * pspec);



UnityTestingDirector* unity_testing_director_construct (GType object_type, ClutterStage* stage) {
	UnityTestingDirector * self;
	g_return_val_if_fail (stage != NULL, NULL);
	self = (UnityTestingDirector*) g_object_new (object_type, "stage", stage, NULL);
	return self;
}


UnityTestingDirector* unity_testing_director_new (ClutterStage* stage) {
	return unity_testing_director_construct (UNITY_TESTING_TYPE_DIRECTOR, stage);
}


static void unity_testing_director_do_event (UnityTestingDirector* self, ClutterActor* actor, ClutterEvent* event, gboolean capture_phase) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (actor != NULL);
	clutter_actor_event (actor, event, capture_phase);
	while (TRUE) {
		if (!gtk_events_pending ()) {
			break;
		}
		gtk_main_iteration ();
	}
}


static gpointer _g_object_ref0 (gpointer self) {
	return self ? g_object_ref (self) : NULL;
}


void unity_testing_director_do_wait_for_animation (UnityTestingDirector* self, ClutterActor* actor) {
	ClutterAnimation* anim;
	g_return_if_fail (self != NULL);
	g_return_if_fail (actor != NULL);
	anim = _g_object_ref0 (clutter_actor_get_animation (actor));
	while (TRUE) {
		gboolean _tmp0_ = FALSE;
		if (CLUTTER_IS_ANIMATION (anim)) {
			_tmp0_ = clutter_timeline_is_playing (clutter_animation_get_timeline (anim));
		} else {
			_tmp0_ = FALSE;
		}
		if (!_tmp0_) {
			break;
		}
		gtk_main_iteration ();
	}
	_g_object_unref0 (anim);
}


static gboolean _lambda37_ (UnityTestingDirector* self) {
	gboolean result = FALSE;
	self->priv->break_loop = TRUE;
	result = FALSE;
	return result;
}


static gboolean __lambda37__gsource_func (gpointer self) {
	gboolean result;
	result = _lambda37_ (self);
	return result;
}


void unity_testing_director_do_wait_for_timeout (UnityTestingDirector* self, guint32 msecs) {
	g_return_if_fail (self != NULL);
	self->priv->break_loop = FALSE;
	g_timeout_add_full (G_PRIORITY_DEFAULT, (guint) msecs, __lambda37__gsource_func, g_object_ref (self), g_object_unref);
	while (TRUE) {
		if (!(self->priv->break_loop != TRUE)) {
			break;
		}
		gtk_main_iteration ();
	}
}


void unity_testing_director_button_press (UnityTestingDirector* self, ClutterActor* actor, guint32 button, gboolean autorelease, float relative_x, float relative_y, gboolean wait_for_animation) {
	float actor_x = 0.0F;
	float actor_y = 0.0F;
	ClutterButtonEvent event = {0};
	ClutterActor* _tmp0_;
	ClutterEvent _tmp1_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (actor != NULL);
	memset (&event, 0, sizeof (ClutterButtonEvent));
	clutter_actor_get_transformed_position (actor, &actor_x, &actor_y);
	event.type = CLUTTER_BUTTON_PRESS;
	event.time = clutter_get_current_event_time ();
	event.flags = event.flags & CLUTTER_EVENT_FLAG_SYNTHETIC;
	event.stage = (_tmp0_ = clutter_actor_get_stage (actor), CLUTTER_IS_STAGE (_tmp0_) ? ((ClutterStage*) _tmp0_) : NULL);
	event.source = actor;
	event.x = actor_x + relative_x;
	event.y = actor_y + relative_y;
	event.button = button;
	unity_testing_director_do_event (self, actor, (_tmp1_ = (ClutterEvent) event, &_tmp1_), FALSE);
	if (wait_for_animation) {
		unity_testing_director_do_wait_for_animation (self, actor);
	}
	if (autorelease) {
		ClutterEvent _tmp2_;
		event.type = CLUTTER_BUTTON_RELEASE;
		event.time = clutter_get_current_event_time ();
		unity_testing_director_do_event (self, actor, (_tmp2_ = (ClutterEvent) event, &_tmp2_), FALSE);
		if (wait_for_animation) {
			unity_testing_director_do_wait_for_animation (self, actor);
		}
	}
}


void unity_testing_director_button_release (UnityTestingDirector* self, ClutterActor* actor, guint32 button, float relative_x, float relative_y) {
	float actor_x = 0.0F;
	float actor_y = 0.0F;
	ClutterButtonEvent event = {0};
	ClutterActor* _tmp0_;
	ClutterEvent _tmp1_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (actor != NULL);
	memset (&event, 0, sizeof (ClutterButtonEvent));
	clutter_actor_get_transformed_position (actor, &actor_x, &actor_y);
	event.type = CLUTTER_BUTTON_RELEASE;
	event.time = clutter_get_current_event_time ();
	event.flags = event.flags & CLUTTER_EVENT_FLAG_SYNTHETIC;
	event.stage = (_tmp0_ = clutter_actor_get_stage (actor), CLUTTER_IS_STAGE (_tmp0_) ? ((ClutterStage*) _tmp0_) : NULL);
	event.source = actor;
	event.x = actor_x + relative_x;
	event.y = actor_y + relative_y;
	event.button = button;
	unity_testing_director_do_event (self, actor, (_tmp1_ = (ClutterEvent) event, &_tmp1_), FALSE);
}


void unity_testing_director_enter_event (UnityTestingDirector* self, ClutterActor* actor, float relative_x, float relative_y) {
	float actor_x = 0.0F;
	float actor_y = 0.0F;
	ClutterCrossingEvent event = {0};
	ClutterActor* _tmp0_;
	ClutterEvent _tmp1_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (actor != NULL);
	memset (&event, 0, sizeof (ClutterCrossingEvent));
	clutter_actor_get_transformed_position (actor, &actor_x, &actor_y);
	event.type = CLUTTER_ENTER;
	event.time = clutter_get_current_event_time ();
	event.flags = event.flags & CLUTTER_EVENT_FLAG_SYNTHETIC;
	event.stage = (_tmp0_ = clutter_actor_get_stage (actor), CLUTTER_IS_STAGE (_tmp0_) ? ((ClutterStage*) _tmp0_) : NULL);
	event.source = (ClutterActor*) event.stage;
	event.x = actor_x + relative_x;
	event.y = actor_y + relative_y;
	unity_testing_director_do_event (self, actor, (_tmp1_ = (ClutterEvent) event, &_tmp1_), FALSE);
}


void unity_testing_director_leave_event (UnityTestingDirector* self, ClutterActor* actor, float relative_x, float relative_y) {
	float actor_x = 0.0F;
	float actor_y = 0.0F;
	ClutterCrossingEvent event = {0};
	ClutterActor* _tmp0_;
	ClutterEvent _tmp1_;
	g_return_if_fail (self != NULL);
	g_return_if_fail (actor != NULL);
	memset (&event, 0, sizeof (ClutterCrossingEvent));
	clutter_actor_get_transformed_position (actor, &actor_x, &actor_y);
	event.type = CLUTTER_LEAVE;
	event.time = clutter_get_current_event_time ();
	event.flags = event.flags & CLUTTER_EVENT_FLAG_SYNTHETIC;
	event.stage = (_tmp0_ = clutter_actor_get_stage (actor), CLUTTER_IS_STAGE (_tmp0_) ? ((ClutterStage*) _tmp0_) : NULL);
	event.source = (ClutterActor*) event.stage;
	event.x = actor_x + relative_x;
	event.y = actor_y + relative_y;
	unity_testing_director_do_event (self, actor, (_tmp1_ = (ClutterEvent) event, &_tmp1_), FALSE);
}


void unity_testing_director_motion_event (UnityTestingDirector* self, ClutterActor* actor, float relative_startx, float relative_starty, float relative_endx, float relative_endy) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (actor != NULL);
}


ClutterStage* unity_testing_director_get_stage (UnityTestingDirector* self) {
	ClutterStage* result;
	g_return_val_if_fail (self != NULL, NULL);
	result = self->priv->_stage;
	return result;
}


static void unity_testing_director_set_stage (UnityTestingDirector* self, ClutterStage* value) {
	ClutterStage* _tmp0_;
	g_return_if_fail (self != NULL);
	self->priv->_stage = (_tmp0_ = _g_object_ref0 (value), _g_object_unref0 (self->priv->_stage), _tmp0_);
	g_object_notify ((GObject *) self, "stage");
}


static GObject * unity_testing_director_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties) {
	GObject * obj;
	GObjectClass * parent_class;
	UnityTestingDirector * self;
	parent_class = G_OBJECT_CLASS (unity_testing_director_parent_class);
	obj = parent_class->constructor (type, n_construct_properties, construct_properties);
	self = UNITY_TESTING_DIRECTOR (obj);
	{
	}
	return obj;
}


static void unity_testing_director_class_init (UnityTestingDirectorClass * klass) {
	unity_testing_director_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (UnityTestingDirectorPrivate));
	G_OBJECT_CLASS (klass)->get_property = unity_testing_director_get_property;
	G_OBJECT_CLASS (klass)->set_property = unity_testing_director_set_property;
	G_OBJECT_CLASS (klass)->constructor = unity_testing_director_constructor;
	G_OBJECT_CLASS (klass)->finalize = unity_testing_director_finalize;
	g_object_class_install_property (G_OBJECT_CLASS (klass), UNITY_TESTING_DIRECTOR_STAGE, g_param_spec_object ("stage", "stage", "stage", CLUTTER_TYPE_STAGE, G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB | G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}


static void unity_testing_director_instance_init (UnityTestingDirector * self) {
	self->priv = UNITY_TESTING_DIRECTOR_GET_PRIVATE (self);
	self->priv->break_loop = FALSE;
}


static void unity_testing_director_finalize (GObject* obj) {
	UnityTestingDirector * self;
	self = UNITY_TESTING_DIRECTOR (obj);
	_g_object_unref0 (self->priv->_stage);
	G_OBJECT_CLASS (unity_testing_director_parent_class)->finalize (obj);
}


GType unity_testing_director_get_type (void) {
	static volatile gsize unity_testing_director_type_id__volatile = 0;
	if (g_once_init_enter (&unity_testing_director_type_id__volatile)) {
		static const GTypeInfo g_define_type_info = { sizeof (UnityTestingDirectorClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) unity_testing_director_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (UnityTestingDirector), 0, (GInstanceInitFunc) unity_testing_director_instance_init, NULL };
		GType unity_testing_director_type_id;
		unity_testing_director_type_id = g_type_register_static (G_TYPE_OBJECT, "UnityTestingDirector", &g_define_type_info, 0);
		g_once_init_leave (&unity_testing_director_type_id__volatile, unity_testing_director_type_id);
	}
	return unity_testing_director_type_id__volatile;
}


static void unity_testing_director_get_property (GObject * object, guint property_id, GValue * value, GParamSpec * pspec) {
	UnityTestingDirector * self;
	self = UNITY_TESTING_DIRECTOR (object);
	switch (property_id) {
		case UNITY_TESTING_DIRECTOR_STAGE:
		g_value_set_object (value, unity_testing_director_get_stage (self));
		break;
		default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}


static void unity_testing_director_set_property (GObject * object, guint property_id, const GValue * value, GParamSpec * pspec) {
	UnityTestingDirector * self;
	self = UNITY_TESTING_DIRECTOR (object);
	switch (property_id) {
		case UNITY_TESTING_DIRECTOR_STAGE:
		unity_testing_director_set_stage (self, g_value_get_object (value));
		break;
		default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}




