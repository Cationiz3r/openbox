#include "openbox/actions.h"
#include "openbox/client.h"

/* These match the values for client_snap */
typedef enum {
    LEFT = 0,
    RIGHT = 1
} SnapDirection;

typedef struct {
    SnapDirection dir;
} Options;

static gpointer setup_func(xmlNodePtr node);
static void free_func(gpointer o);
static gboolean run_func_on(ObActionsData *data, gpointer options);
static gboolean run_func_off(ObActionsData *data, gpointer options);

void action_snap_startup(void)
{
    actions_register("Snap", setup_func, free_func, run_func_on);
    actions_register("Unsnap", setup_func, free_func, run_func_off);
}

static gpointer setup_func(xmlNodePtr node)
{
    xmlNodePtr n;
    Options *o;

    o = g_slice_new0(Options);
    o->dir = LEFT;

    if ((n = obt_xml_find_node(node, "direction"))) {
        gchar *s = obt_xml_node_string(n);
        if (!g_ascii_strcasecmp(s, "west") ||
            !g_ascii_strcasecmp(s, "left"))
            o->dir = LEFT;
        else if (!g_ascii_strcasecmp(s, "east") ||
                 !g_ascii_strcasecmp(s, "right"))
            o->dir = RIGHT;
        g_free(s);
    }

    return o;
}

static void free_func(gpointer o)
{
    g_slice_free(Options, o);
}

/* Always return FALSE because its not interactive */
static gboolean run_func_on(ObActionsData *data, gpointer options)
{
    Options *o = options;
    if (data->client) {
        actions_client_move(data, TRUE);
        client_snap(data->client, o->dir);
        actions_client_move(data, FALSE);
    }
    return FALSE;
}

/* Always return FALSE because its not interactive */
static gboolean run_func_off(ObActionsData *data, gpointer options)
{
    Options *o = options;
    if (data->client) {
        actions_client_move(data, TRUE);
        client_unsnap(data->client);
        actions_client_move(data, FALSE);
    }
    return FALSE;
}
