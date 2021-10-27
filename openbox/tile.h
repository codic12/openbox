#include <glib.h>

enum Layout { LAYOUT_FLOATING, LAYOUT_TILING };
extern enum Layout layout;

void tile_windows();