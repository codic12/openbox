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
  // Retrieve the screen size.
  const Rect *screen = screen_physical_area_primary(FALSE);
  ObClient *cl_ = g_list_nth(client_list, 0)->data;
  if (ll == 1) {
    client_maximize(cl_, TRUE, 0);
    return; // lol
  }
  client_maximize(cl_, FALSE, 0);
  client_move_resize(cl_, 0, 0, screen->width >> 1, screen->height);
  size_t i = 0;
  for (it = client_list; it && it->data; it = g_list_next(it)) {
    ObClient *cl = it->data;
    if (cl == cl_) 
      continue;
    ++i;
    // Now we know we have a stack window.
    if (ll == 2) {
      client_move_resize(cl, screen->width >> 1, 0, screen->width >> 1,
                         screen->height);
    } else {
      client_move_resize(cl, screen->width >> 1, screen->height * ((i - 1) / (float)(ll -1)),
                         screen->width >> 1, screen->height / (ll - 1));
    }
  }
}