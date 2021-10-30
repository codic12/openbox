#include "tile.h"
#include "config.h"
#include "openbox/client.h"
#include "openbox/config.h"
#include "openbox/frame.h"
#include "openbox/openbox.h"
#include "openbox/screen.h"
#include "screen.h"
#include <X11/Xlib.h>
#include <glib.h>

enum Layout layout;

void tile_windows() {
  size_t ll = g_list_length(client_list);
  if (ll == 0)
    return;
  /*
    The rule with tiling is pretty simple. The first window managed (aka the
    first window in client_list) is the one that is treated as the "master"
    window on the left side. The other windows are treated as "stack" windows on
    the right side.
  */
  GList *it;
  ObClient *cl_ = NULL;
  size_t irregular_windows = 0;
  for (it = client_list; it && it->data; it = g_list_next(it)) {
    ObClient *cl = it->data;
    if (cl->type == OB_CLIENT_TYPE_NORMAL && cl->desktop == screen_desktop)
      cl_ = it->data;
    else
      ++irregular_windows;
  }
  if (cl_ == NULL)
    return;
  // Retrieve the screen size.
  if ((ll - irregular_windows) == 1) {
    client_maximize(cl_, TRUE, 0);
    return; // lol
  }
  const Rect *screen = screen_physical_area_primary(FALSE);
  client_move_resize(cl_, 0, config_margins.top,
                     (screen->width >> 1) - cl_->frame->size.left,
                     screen->height - cl_->frame->size.top -
                         config_margins.top - config_margins.bottom);
  size_t i = 0;
  for (it = client_list; it && it->data; it = g_list_next(it)) {
    ObClient *cl = it->data;
    if (cl == cl_ || cl->type != OB_CLIENT_TYPE_NORMAL) {
      client_maximize(cl, FALSE, 0);
      continue;
    }
    if (cl->desktop != screen_desktop)
      continue; // no maximize
    client_maximize(cl, FALSE, 0);
    ++i;
    // Now we know we have a stack window.
    if ((ll - irregular_windows) == 2) {
      client_move_resize(cl, screen->width >> 1, config_margins.top,
                         (screen->width >> 1) - cl->frame->size.left,
                         screen->height - cl->frame->size.top -
                             config_margins.top - config_margins.bottom);
    } else {
      client_move_resize(
          cl, screen->width >> 1,
          screen->height * ((i - 1) / (float)(ll - 1 - irregular_windows)) +
              config_margins.top,
          (screen->width >> 1) - cl->frame->size.left,
          ((screen->height - config_margins.top - config_margins.bottom - ((ll - 1 - irregular_windows )* cl->frame->size.top)) / (ll - 1 - irregular_windows)));
    }
  }
}