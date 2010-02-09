/* vim:set et sts=4: */

#include <ibus.h>
#include <stdlib.h>
#include <locale.h>
#include "engine.h"

#define N_(text) text

static IBusBus *bus = NULL;
static IBusFactory *factory = NULL;

/* options */
static gboolean ibus = FALSE;
static gboolean verbose = FALSE;

static const GOptionEntry entries[] =
{
    { "ibus", 'i', 0, G_OPTION_ARG_NONE, &ibus, "component is executed by ibus", NULL },
    { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "verbose", NULL },
    { NULL },
};


static void
ibus_disconnected_cb (IBusBus  *bus,
                      gpointer  user_data)
{
    g_debug ("bus disconnected");
    ibus_quit ();
}


static void
start_component (void)
{
    ibus_init ();

    bus = ibus_bus_new ();
    g_object_ref_sink (bus);
    g_signal_connect (bus, "disconnected", G_CALLBACK (ibus_disconnected_cb), NULL);

    factory = ibus_factory_new (ibus_bus_get_connection (bus));
    g_object_ref_sink (factory);

    ibus_factory_add_engine (factory, "sayura", IBUS_TYPE_SINHALA_ENGINE);

    if (ibus) {
        ibus_bus_request_name (bus, "org.freedesktop.IBus.Sinhala", 0);
    }
    else {
        IBusComponent *component;
        component = ibus_component_new ("org.freedesktop.IBus.Sinhala",
                                        N_("Sinhala input method"),
                                        "0.1.0",
                                        "GPL",
                                        "Pravin Satpute <pravin.d.s@gmail.com>",
                                        "https://fedorahosted.org/ibus-sayura/",
                                        "",
                                        "ibus-sayura");
        ibus_component_add_engine (component,
                                   ibus_engine_desc_new ("sayura",
                                                         N_("Sinhala Input Method"),
                                                         N_("Sinhala Input Method"),
                                                         "si",
                                                         "GPL",
                                                         "Pravin Satpute <pravin.d.s@gmail.com>",
                                                         PKGDATADIR"/icon/ibus-sayura.png",
                                                         "us"));
        ibus_bus_register_component (bus, component);
    }

    ibus_main ();
}

int
main (gint argc, gchar **argv)
{
    GError *error = NULL;
    GOptionContext *context;

    setlocale (LC_ALL, "");

    context = g_option_context_new ("- ibus sayura engine component");

    g_option_context_add_main_entries (context, entries, "ibus-sayura");

    if (!g_option_context_parse (context, &argc, &argv, &error)) {
        g_print ("Option parsing failed: %s\n", error->message);
        exit (-1);
    }

    start_component ();
    return 0;
}
