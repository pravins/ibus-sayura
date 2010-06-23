/* vim:set et sts=4: */

#include <ibus.h>
#include <ibusengine.h>
#include <string.h>
#include "engine.h"

typedef struct _IBusSinhalaEngine IBusSinhalaEngine;
typedef struct _IBusSinhalaEngineClass IBusSinhalaEngineClass;

struct _IBusSinhalaEngine {
	IBusEngine parent;

    /* members */
    GArray *buffer;

    IBusProperty    *sinhala_mode_prop;
    IBusPropList    *prop_list;
};

struct _IBusSinhalaEngineClass {
	IBusEngineClass parent;
};

struct {
	gint character;
	gint mahaprana;
	gint sagngnaka;
	int key;
} consonents[] = {
	{0xda4, 0x00, 0x00, IBUS_z},
	{0xda5, 0x00, 0x00, IBUS_Z},
	{0xdc0, 0x00, 0x00, IBUS_w},
	{0x200c, 0x00, 0x00, IBUS_W},
	{0xdbb, 0x00, 0x00, IBUS_r},
	{0xdbb, 0x00, 0x00, IBUS_R},
	{0xdad, 0xdae, 0x00, IBUS_t},
	{0xda7, 0xda8, 0x00, IBUS_T},
	{0xdba, 0x00, 0x00, IBUS_y},
	{0xdba, 0x00, 0x00, IBUS_Y},
	{0xdb4, 0xdb5, 0x00, IBUS_p},
	{0xdb5, 0xdb5, 0x00, IBUS_P},
	{0xdc3, 0xdc2, 0x00, IBUS_s},
	{0xdc1, 0xdc2, 0x00, IBUS_S},
	{0xdaf, 0xdb0, 0xdb3, IBUS_d},
	{0xda9, 0xdaa, 0xdac, IBUS_D},
	{0xdc6, 0x00, 0x00, IBUS_f},
	{0xdc6, 0x00, 0x00, IBUS_F},
	{0xd9c, 0xd9d, 0xd9f, IBUS_g},
	{0xd9f, 0xd9d, 0x00, IBUS_G},
	{0xdc4, 0xd83, 0x00, IBUS_h},
	{0xdc4, 0x00, 0x00, IBUS_H},
	{0xda2, 0xda3, 0xda6, IBUS_j},
	{0xda3, 0xda3, 0xda6, IBUS_J},
	{0xd9a, 0xd9b, 0x00, IBUS_k},
	{0xd9b, 0xd9b, 0x00, IBUS_K},
	{0xdbd, 0x00, 0x00, IBUS_l},
	{0xdc5, 0x00, 0x00, IBUS_L},
	{0xd82, 0x00, 0x00, IBUS_x},
	{0xd9e, 0x00, 0x00, IBUS_X},
	{0xda0, 0xda1, 0x00, IBUS_c},
	{0xda1, 0xda1, 0x00, IBUS_C},
	{0xdc0, 0x00, 0x00, IBUS_v},
	{0xdc0, 0x00, 0x00, IBUS_V},
	{0xdb6, 0xdb7, 0xdb9, IBUS_b},
	{0xdb7, 0xdb7, 0xdb9, IBUS_B},
	{0xdb1, 0x00, 0xd82, IBUS_n},
	{0xdab, 0x00, 0xd9e, IBUS_N},
	{0xdb8, 0x00, 0x00, IBUS_m},
	{0xdb9, 0x00, 0x00, IBUS_M},
	{0, 0, 0, 0}
};

struct {
	gint single0;
	gint double0;
	gint single1;
	gint double1;
	int key;
} vowels[] = {
	{0xd85, 0xd86, 0xdcf, 0xdcf, IBUS_a},
	{0xd87, 0xd88, 0xdd0, 0xdd1, IBUS_A},
	{0xd87, 0xd88, 0xdd0, 0xdd1, IBUS_q},
	{0xd91, 0xd92, 0xdd9, 0xdda, IBUS_e},
	{0xd91, 0xd92, 0xdd9, 0xdda, IBUS_E},
	{0xd89, 0xd8a, 0xdd2, 0xdd3, IBUS_i},
	{0xd93, 0x00, 0xddb, 0xddb, IBUS_I},
	{0xd94, 0xd95, 0xddc, 0xddd, IBUS_o},
	{0xd96, 0x00, 0xdde, 0xddf, IBUS_O},
	{0xd8b, 0xd8c, 0xdd4, 0xdd6, IBUS_u},
	{0xd8d, 0xd8e, 0xdd8, 0xdf2, IBUS_U},
	{0xd8f, 0xd90, 0xd8f, 0xd90, IBUS_Z},
	{0, 0, 0, 0, 0}
};


/* functions prototype */
static void	ibus_sinhala_engine_class_init   (IBusSinhalaEngineClass  *klass);
static void	ibus_sinhala_engine_init		    (IBusSinhalaEngine		*sinhala);
static GObject*
            ibus_sinhala_engine_constructor  (GType                   type,
                                             guint                   n_construct_params,
                                             GObjectConstructParam  *construct_params);
static void	ibus_sinhala_engine_destroy		(IBusSinhalaEngine		*sinhala);
static gboolean
			ibus_sinhala_engine_process_key_event
                                            (IBusEngine             *engine,
                                             guint               	 keyval,
                                             guint               	 keycode,
                                             guint               	 modifiers);
static void ibus_sinhala_engine_focus_in     (IBusEngine             *engine);
static void ibus_sinhala_engine_focus_out    (IBusEngine             *engine);
static void ibus_sinhala_engine_reset        (IBusEngine             *engine);
static void ibus_sinhala_engine_enable       (IBusEngine             *engine);
static void ibus_sinhala_engine_disable      (IBusEngine             *engine);
static void ibus_sinhala_engine_page_up      (IBusEngine             *engine);
static void ibus_sinhala_engine_page_down    (IBusEngine             *engine);
static void ibus_sinhala_engine_cursor_up    (IBusEngine             *engine);
static void ibus_sinhala_engine_cursor_down  (IBusEngine             *engine);
static void ibus_sinhala_engine_flush        (IBusSinhalaEngine       *sinhala);
static void ibus_sinhala_engine_update_preedit_text
			(IBusSinhalaEngine       *sinhala);

//sinhala functions

static int ibus_sinhala_find_consonent_by_key(int k);
static int ibus_sinhala_find_vowel_by_key(gunichar k);
static int ibus_sinhala_find_consonent(gunichar k);
static int ibus_sinhala_is_consonent(gunichar c);
static int ibus_sinhala_commit_preedit_to_ibus(IBusSinhalaEngine *sinhala);
static gboolean ibus_sinhala_handle_consonant_pressed(IBusSinhalaEngine *sinhala, guint keyval,
					int c);
static gboolean ibus_sinhala_handle_vowel_pressed(IBusSinhalaEngine *sinhala, guint keyval,
					int c);

static IBusEngineClass *parent_class = NULL;

GType
ibus_sinhala_engine_get_type (void)
{
	static GType type = 0;

	static const GTypeInfo type_info = {
		sizeof (IBusSinhalaEngineClass),
		(GBaseInitFunc)		NULL,
		(GBaseFinalizeFunc) NULL,
		(GClassInitFunc)	ibus_sinhala_engine_class_init,
		NULL,
		NULL,
		sizeof (IBusSinhalaEngine),
		0,
		(GInstanceInitFunc)	ibus_sinhala_engine_init,
	};

	if (type == 0) {
		type = g_type_register_static (IBUS_TYPE_ENGINE,
									   "IBusSinhalaEngine",
									   &type_info,
									   (GTypeFlags) 0);
	}

	return type;
}

static void
ibus_sinhala_engine_class_init (IBusSinhalaEngineClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
	IBusObjectClass *ibus_object_class = IBUS_OBJECT_CLASS (klass);
	IBusEngineClass *engine_class = IBUS_ENGINE_CLASS (klass);

	parent_class = (IBusEngineClass *) g_type_class_peek_parent (klass);

    object_class->constructor = ibus_sinhala_engine_constructor;
	ibus_object_class->destroy = (IBusObjectDestroyFunc) ibus_sinhala_engine_destroy;

    engine_class->process_key_event = ibus_sinhala_engine_process_key_event;

    engine_class->reset = ibus_sinhala_engine_reset;
    engine_class->enable = ibus_sinhala_engine_enable;
    engine_class->disable = ibus_sinhala_engine_disable;

    engine_class->focus_in = ibus_sinhala_engine_focus_in;
    engine_class->focus_out = ibus_sinhala_engine_focus_out;

    engine_class->page_up = ibus_sinhala_engine_page_up;
    engine_class->page_down = ibus_sinhala_engine_page_down;

    engine_class->cursor_up = ibus_sinhala_engine_cursor_up;
    engine_class->cursor_down = ibus_sinhala_engine_cursor_down;
}

static void
ibus_sinhala_engine_init (IBusSinhalaEngine *sinhala)
{
    sinhala->sinhala_mode_prop = ibus_property_new ("sinhala_mode_prop",
                                           PROP_TYPE_NORMAL,
                                           NULL,
                                           NULL,
                                           NULL,
                                           TRUE,
                                           FALSE,
                                           0,
                                           NULL);
    g_object_ref_sink (sinhala->sinhala_mode_prop);

    sinhala->prop_list = ibus_prop_list_new ();
    g_object_ref_sink (sinhala->prop_list);
    ibus_prop_list_append (sinhala->prop_list,  sinhala->sinhala_mode_prop);
    sinhala->buffer = g_array_new (TRUE, TRUE, sizeof(gint));
}

static GObject*
ibus_sinhala_engine_constructor (GType                   type,
                                guint                   n_construct_params,
                                GObjectConstructParam  *construct_params)
{
    IBusSinhalaEngine *sinhala;

    sinhala = (IBusSinhalaEngine *) G_OBJECT_CLASS (parent_class)->constructor (type,
                                                       n_construct_params,
                                                       construct_params);

    return (GObject *)sinhala;
}


static void
ibus_sinhala_engine_destroy (IBusSinhalaEngine *sinhala)
{
    if (sinhala->prop_list) {
        g_object_unref (sinhala->prop_list);
        sinhala->prop_list = NULL;
    }

    if (sinhala->sinhala_mode_prop) {
        g_object_unref (sinhala->sinhala_mode_prop);
        sinhala->sinhala_mode_prop = NULL;
    }

    if (sinhala->buffer) {
        g_array_free (sinhala->buffer, TRUE);
        sinhala->buffer = NULL;
    }

    IBUS_OBJECT_CLASS (parent_class)->destroy ((IBusObject *)sinhala);
}

static void
ibus_sinhala_engine_update_preedit_text (IBusSinhalaEngine *sinhala)
{
    IBusText *text;
    static gunichar *uni_array;

    if(sinhala->buffer->len>0){
        uni_array = (gunichar *)sinhala->buffer->data;
        text = ibus_text_new_from_ucs4(uni_array);
        ibus_text_append_attribute (text, IBUS_ATTR_TYPE_UNDERLINE, 1, 0, -1);
//	ibus_text_append_attribute (text, IBUS_ATTR_TYPE_BACKGROUND, 0x00000000, 0, -1);
	ibus_engine_update_preedit_text ((IBusEngine *)sinhala,
	    	                                         text,
		                                         ibus_text_get_length (text),
	    	                                         TRUE);
    }
    else{
        text = ibus_text_new_from_static_string ("");
        ibus_engine_update_preedit_text ((IBusEngine *)sinhala, text, 0, FALSE);
    } 
}

static gboolean
ibus_sinhala_engine_process_key_event (IBusEngine     *engine,
                                      guint           keyval,
                                      guint               	 keycode,
                                      guint           modifiers)
{
    IBusSinhalaEngine *sinhala = (IBusSinhalaEngine *) engine;
    int c;

    if (modifiers & IBUS_RELEASE_MASK)
        return FALSE;

    if ((keyval == IBUS_Escape) &&(modifiers==0)) {
	ibus_sinhala_engine_reset ( engine);
        return TRUE;
    } 

    if (modifiers & (IBUS_CONTROL_MASK | IBUS_MOD1_MASK))
        return FALSE;

    if ((keyval == IBUS_BackSpace) && (modifiers==0) && (sinhala->buffer->len!=0)) {
	if(sinhala->buffer){
		g_array_remove_index(sinhala->buffer, (sinhala->buffer->len)-1);
		ibus_sinhala_engine_update_preedit_text (sinhala);
	}
        return TRUE;
    }


    if (keyval == IBUS_space && modifiers == 0 && sinhala->buffer->len >0) {
        ibus_sinhala_commit_preedit_to_ibus(sinhala);
        return FALSE;
    }

    c = ibus_sinhala_find_consonent_by_key(keyval);
    if (c >= 0) /* a consonent is pressed. */
    	return ibus_sinhala_handle_consonant_pressed (sinhala, keyval, c);

    c = ibus_sinhala_find_vowel_by_key(keyval);
    if (c >= 0) /* a consonent is pressed. */
        return ibus_sinhala_handle_vowel_pressed (sinhala, keyval, c);

    if (keyval == IBUS_Shift_L || keyval == IBUS_Shift_L ) {
        return FALSE;
    }

    if (sinhala->buffer->len >0)
	        ibus_sinhala_commit_preedit_to_ibus(sinhala);

    return FALSE;
}

static void
ibus_sinhala_engine_flush (IBusSinhalaEngine *sinhala)
{
    IBusText *text;
    if(sinhala->buffer->len){
	ibus_sinhala_commit_preedit_to_ibus(sinhala);

    }
    else{
	text = ibus_text_new_from_static_string ("");
	ibus_engine_update_preedit_text ((IBusEngine *)sinhala, text, 0, FALSE);
    }
    ibus_engine_hide_preedit_text ((IBusEngine *) sinhala);


}


static void
ibus_sinhala_engine_focus_in (IBusEngine *engine)
{
    IBusSinhalaEngine *sinhala = (IBusSinhalaEngine *) engine;

    ibus_engine_register_properties (engine, sinhala->prop_list);

    parent_class->focus_in (engine);

    if(sinhala->buffer->len){
        ibus_sinhala_engine_update_preedit_text (sinhala);
    }
  
}

static void
ibus_sinhala_engine_focus_out (IBusEngine *engine)
{
    IBusSinhalaEngine *sinhala = (IBusSinhalaEngine *) engine;

    ibus_sinhala_engine_flush (sinhala);

    parent_class->focus_out ((IBusEngine *) sinhala);
}

static void
ibus_sinhala_engine_reset (IBusEngine *engine)
{
    IBusSinhalaEngine *sinhala = (IBusSinhalaEngine *) engine;
    if(sinhala->buffer->len > 0 ){
	g_array_remove_range(sinhala->buffer, 0, sinhala->buffer->len);
    }

    ibus_engine_hide_preedit_text ((IBusEngine *) sinhala);
    parent_class->reset (engine);
}

static void
ibus_sinhala_engine_enable (IBusEngine *engine)
{
    parent_class->enable (engine);
}

static void
ibus_sinhala_engine_disable (IBusEngine *engine)
{
    ibus_sinhala_engine_focus_out (engine);
    parent_class->disable (engine);
}

static void
ibus_sinhala_engine_page_up (IBusEngine *engine)
{
    parent_class->page_up (engine);
}

static void
ibus_sinhala_engine_page_down (IBusEngine *engine)
{
    parent_class->page_down (engine);
}

static void
ibus_sinhala_engine_cursor_up (IBusEngine *engine)
{
    parent_class->cursor_up (engine);
}

static void
ibus_sinhala_engine_cursor_down (IBusEngine *engine)
{
    parent_class->cursor_down (engine);
}

// Sinhala Related Functions
static int ibus_sinhala_find_consonent_by_key(int k)
{
    int i = -1;
    while (consonents[++i].character)
    	if (consonents[i].key == k)
	    return i;
    return -1;
}

static int ibus_sinhala_find_consonent(gunichar c)
{
    int i = -1;
    while (consonents[++i].character)
	if ((consonents[i].character == c)
            || (consonents[i].mahaprana == c)
            || (consonents[i].sagngnaka == c))
	    return i;
    return -1;
}

static int ibus_sinhala_is_consonent(gunichar c)
{
    return (c >= 0x0d9a) && (c <= 0x0dc6) ? 1 : 0;
}

int ibus_sinhala_find_vowel_by_key(gunichar k)
{
    int i = -1;
    while (vowels[++i].single0)
	if (vowels[i].key == k)
	    return i;
    return -1;
}

static gboolean ibus_sinhala_handle_consonant_pressed(IBusSinhalaEngine *sinhala,
		                                      guint keyval,
					int c)
{
    int  l1;
    gint val;
    
    if (sinhala->buffer->len == 0) {

	g_array_append_val(sinhala->buffer, consonents[c].character);
	ibus_sinhala_engine_update_preedit_text (sinhala);
	return TRUE;
    }
    
    /* do modifiers first. */
    l1 = ibus_sinhala_find_consonent(g_array_index(sinhala->buffer, gunichar,0));
    /* do modifiers only if there is a valid character before */
    if (l1 >= 0) {
        if (keyval == IBUS_w) {
		val = 0x0dca;
		g_array_append_val(sinhala->buffer, val);
		ibus_sinhala_engine_update_preedit_text (sinhala);
        	return TRUE;
	}

        if (keyval == IBUS_W) {
            /* bandi hal kireema */
		val = 0x0dca;
		g_array_append_val(sinhala->buffer, val);
		ibus_sinhala_commit_preedit_to_ibus(sinhala);
		val = 0x200d;
		g_array_append_val (sinhala->buffer, val);
		ibus_sinhala_engine_update_preedit_text (sinhala);

		return TRUE;
	}

        if ((keyval == IBUS_H) && (consonents[l1].mahaprana)) {
		if(sinhala->buffer){
			g_array_remove_index(sinhala->buffer, (sinhala->buffer->len)-1);
			g_array_append_val (sinhala->buffer, consonents[l1].mahaprana);
			ibus_sinhala_engine_update_preedit_text (sinhala);
		}
		return TRUE;
	}
        
	if ((keyval == IBUS_G) && (consonents[l1].sagngnaka)) {
		if(sinhala->buffer){
			g_array_remove_index(sinhala->buffer, (sinhala->buffer->len)-1);
			g_array_append_val (sinhala->buffer, consonents[l1].sagngnaka);
			ibus_sinhala_engine_update_preedit_text (sinhala);
		}
		return TRUE;
	}

        if (keyval == IBUS_R) {
		val = 0x0dca;
		g_array_append_val (sinhala->buffer, val);
		val = 0x200d;
		g_array_append_val (sinhala->buffer, val);
		ibus_sinhala_commit_preedit_to_ibus(sinhala);
		val = 0x0dbb;
		g_array_append_val (sinhala->buffer, val);
		ibus_sinhala_engine_update_preedit_text (sinhala);
		return TRUE;
	}

        if (keyval == IBUS_Y) {
		/* yansaya */
		val = 0x0dca;
		g_array_append_val (sinhala->buffer, val);
		val = 0x200d;
		g_array_append_val (sinhala->buffer, val);
		ibus_sinhala_commit_preedit_to_ibus(sinhala);
		val = 0x0dba;
		g_array_append_val (sinhala->buffer, val);
		ibus_sinhala_engine_update_preedit_text (sinhala);
		return TRUE;
	}
    
	}

	ibus_sinhala_commit_preedit_to_ibus(sinhala);
	g_array_append_val (sinhala->buffer, consonents[c].character);
	ibus_sinhala_engine_update_preedit_text (sinhala);

    return TRUE;
}

static int ibus_sinhala_commit_preedit_to_ibus(IBusSinhalaEngine *sinhala)
{
	int i;
	IBusText *text;
	if(sinhala->buffer){
		for(i=0; i<sinhala->buffer->len; i++){
			text = ibus_text_new_from_unichar(g_array_index(sinhala->buffer, gunichar,i));
			ibus_engine_commit_text ((IBusEngine *)sinhala, text);		
		}
		if(sinhala->buffer->len > 0){
			g_array_remove_range(sinhala->buffer, 0, sinhala->buffer->len);
			ibus_sinhala_engine_update_preedit_text(sinhala);
		}
	}
	
	return sinhala->buffer->len;
	
}

static gboolean ibus_sinhala_handle_vowel_pressed(IBusSinhalaEngine *sinhala,
		                                      guint keyval,
					int c)
{
	int c1;
	gint val;

        if (sinhala->buffer->len == 0) {
		ibus_engine_show_preedit_text ((IBusEngine *)sinhala);
		g_array_append_val(sinhala->buffer, vowels[c].single0);
		ibus_sinhala_engine_update_preedit_text (sinhala);
        	return TRUE;
	}        
        else { 
            /* look for a previous character first. */
		c1 = g_array_index(sinhala->buffer, gunichar,(sinhala->buffer->len-1));

		if (ibus_sinhala_is_consonent(c1)) {
			g_array_append_val(sinhala->buffer, vowels[c].single1);
			ibus_sinhala_engine_update_preedit_text (sinhala);
			return TRUE;
            		}
		else if (c1 == vowels[c].single0) {
			if(sinhala->buffer){
				g_array_remove_index(sinhala->buffer, (sinhala->buffer->len)-1);
				g_array_append_val(sinhala->buffer, vowels[c].double0);
				ibus_sinhala_engine_update_preedit_text (sinhala);
				return TRUE;
			}

		}
		else if (c1 == vowels[c].single1) {
			if(sinhala->buffer){
				g_array_remove_index(sinhala->buffer, (sinhala->buffer->len)-1);
				g_array_append_val(sinhala->buffer, vowels[c].double1);
				ibus_sinhala_engine_update_preedit_text (sinhala);
				return TRUE;
			}

		}
		else if(((c1 == 0x0d86) || (c1 == 0x0d87)) && (c == 0x0)){
			g_array_remove_index(sinhala->buffer, (sinhala->buffer->len)-1);
			val = vowels[c].single0+1;
			g_array_append_val(sinhala->buffer, val );
			ibus_sinhala_engine_update_preedit_text (sinhala);
			return TRUE;
		}


	return TRUE;
       	}
}
