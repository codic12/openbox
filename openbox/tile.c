#include "tile.h"
#include "openbox/client.h"
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
    printf("Maximizing a window\n");
    client_maximize(cl_, TRUE, 0);
    return; // lol
  }
  printf("Making a client NOT maximized\n");
  const Rect *screen = screen_physical_area_primary(FALSE);
  client_move_resize(cl_, 0, 0, (screen->width >> 1) - cl_->frame->size.left,
                     screen->height - cl_->frame->size.top);
  size_t i = 0;
  for (it = client_list; it && it->data; it = g_list_next(it)) {
    ObClient *cl = it->data;
    client_maximize(cl, FALSE, 0);
    if (cl == cl_ || cl->type != OB_CLIENT_TYPE_NORMAL) {
      continue;
    }
    ++i;
    // Now we know we have a stack window.
    if ((ll - irregular_windows) == 2) {
      client_move_resize(cl, screen->width >> 1, 0,
                         (screen->width >> 1) - cl->frame->size.left,
                         screen->height - cl->frame->size.top);
    } else {
      client_move_resize(cl, screen->width >> 1,
                         screen->height *
                             ((i - 1) / (float)(ll - 1 - irregular_windows)),
                         (screen->width >> 1) - cl->frame->size.left,
                         (screen->height / (ll - 1 - irregular_windows)) -
                             cl->frame->size.top);
    }
  }
}