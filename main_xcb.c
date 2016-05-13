#include <xcb/xcb.h>

int main(int argc, char const *argv[]) {
        xcb_connection_t *c = xcb_connect(0, 0);

        xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(c)).data;

        xcb_window_t win = xcb_generate_id(c);

        xcb_create_window(c,
                        XCB_COPY_FROM_PARENT,
                        win,
                        screen->root,
                        0, 0,
                        150, 150,
                        10,
                        XCB_WINDOW_CLASS_INPUT_OUTPUT,
                        screen->root_visual,
                        0, 0);

        xcb_map_window(c, win);

        xcb_flush(c);

        getchar();

        xcb_disconnect(c);
        return 0;
}
