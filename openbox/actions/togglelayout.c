#include "openbox/actions.h"
#include "openbox/client.h"
#include "openbox/debug.h"
#include "openbox/openbox.h"
#include "openbox/tile.h"

static gboolean run_func(ObActionsData *data, gpointer options);

void action_toggle_layout_startup(void) {
  layout = LAYOUT_FLOATING;
  actions_register("ToggleLayout", NULL, NULL, run_func);
}

/* Always return FALSE because its not interactive */
static gboolean run_func(ObActionsData *data, gpointer options) {
  layout = layout == LAYOUT_FLOATING ? LAYOUT_TILING : LAYOUT_FLOATING;
  ob_debug("following through w/ request to toggle layout");
  if (layout == LAYOUT_TILING) {
    // tile windows here.
    tile_windows();
  }
  return FALSE;
}
