// gcc `pkg-config --libs xcb xcb-image` main.c -o main

#include <stdio.h>
#include <stdlib.h>

#include <xcb/xcb.h>
#include <xcb/xcb_image.h>

#include <fdebug.h>

#define neko_width 32
#define neko_height 32

unsigned window_width = 800;
unsigned window_height = 600;

xcb_connection_t *connection;
xcb_screen_t *screen;

xcb_image_t *image;

xcb_window_t
create_window() {
        // http://www.x.org/releases/current/doc/xproto/x11protocol.html#requests:CreateWindow
        // http://www.x.org/archive/current/doc/man/man3/xcb_create_window.3.xhtml
        //
        // N.B. xcb's order corresponds to the order of the wire.
        // You can look at the protocol encoding: http://www.x.org/releases/current/doc/xproto/x11protocol.html#Encoding::Requests

        uint32_t mask;
        uint32_t values[2];

        xcb_window_t window;
        xcb_void_cookie_t cookie;

        mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
        values[0] = screen->white_pixel;
        values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS;

        window = xcb_generate_id(connection);
        cookie = xcb_create_window(connection,
                XCB_COPY_FROM_PARENT, window, screen->root,
                0, 0, window_width, window_height,
                0,
                XCB_WINDOW_CLASS_INPUT_OUTPUT,
                screen->root_visual,
                mask, values
        );

        xcb_map_window(connection, window);
        return window;
}

xcb_gcontext_t
create_graphics_context(xcb_pixmap_t drawable) {
        xcb_gcontext_t graphics_context;

        uint32_t mask;
        uint32_t values[3];

        mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND
                | XCB_GC_GRAPHICS_EXPOSURES;
        values[0] = screen->black_pixel;
        values[1] = screen->white_pixel;
        values[2] = 0;

        graphics_context = xcb_generate_id(connection);
        xcb_create_gc(connection,
                graphics_context, drawable,
                mask, values
        );

        return graphics_context;
}

static xcb_format_t *
find_format (xcb_connection_t * c, uint8_t depth, uint8_t bpp) {
        const xcb_setup_t *setup = xcb_get_setup(c);
        xcb_format_t *fmt = xcb_setup_pixmap_formats(setup);
        xcb_format_t *fmtend = fmt + xcb_setup_pixmap_formats_length(setup);
        for(; fmt != fmtend; ++fmt)
                if((fmt->depth == depth) && (fmt->bits_per_pixel == bpp)) {
                        /* printf("fmt %p has pad %d depth %d, bpp %d\n",
                        fmt,fmt->scanline_pad, depth,bpp); */
                        return fmt;
                }
        return 0;
}

void fillimage(unsigned char *p, int width, int height) {
        int i, j;
        for(i=0; i < width; i++) {
                for(j=0; j < height; j++) {
                        *p++= !(j%100)?0:250; //rand()%256; // blue
                        *p++= !(i%100)?0:250; //rand()%256; // green
                        *p++=250; //rand()%256; // red

                        p++; /* unused byte */
                }
        }
}

void putpixel(unsigned char *p, int x, int y, int width) {
        p += 4* y * width;
        p += 4* x;
        *(p+0) = 0;
        *(p+1) = 0;
        *(p+2) = 0;
}

void
drawlineXD(unsigned char *p, int x0, int y0, int x1, int y1, int width) {
        const unsigned resolution = 1000;
        float delta = 1.0/resolution;
        int dx = x1 - x0;
        int dy = y1 - y0;
        for(unsigned i = 0; i < resolution; i++) {
                putpixel(p,
                        x0 + dx * delta * i,
                        y0 + dy * delta * i,
                        width
                );
        }
}

xcb_image_t *
CreateTrueColorImage(xcb_connection_t *c, int width, int height) {
        const xcb_setup_t *setup = xcb_get_setup(c);
        unsigned char *image32=(unsigned char *)malloc(width*height*4);
        xcb_format_t *fmt = find_format(c, 24, 32);
        if (fmt == NULL)
                return NULL;

        fillimage(image32, width, height);

        return xcb_image_create(width,
                height,
                XCB_IMAGE_FORMAT_Z_PIXMAP,
                fmt->scanline_pad,
                fmt->depth,
                fmt->bits_per_pixel,
                0,
                setup->image_byte_order,
                XCB_IMAGE_ORDER_LSB_FIRST,
                image32,
                width*height*4,
                image32
        );
}


void
event_loop(xcb_window_t window, xcb_gcontext_t graphics_context, xcb_pixmap_t pixmap) {
        xcb_generic_event_t *event;

        while((event = xcb_wait_for_event(connection))) {
                switch(event->response_type) {
                        case XCB_EXPOSE:
                        xcb_copy_area(connection,
                                pixmap, window, graphics_context,
                                0, 0, 0, 0,
                                window_width, window_height
                        );
                        xcb_flush(connection);
                        break;
                        case XCB_KEY_PRESS:
                        return;
                }
        }
}

int main(void) {
        xcb_window_t window;
        xcb_gcontext_t graphics_context;
        xcb_pixmap_t pixmap;

        connection = xcb_connect(NULL, NULL); // Callers need to use xcb_connection_has_error() to check for failure.
        screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;

        window = create_window();

        image = CreateTrueColorImage(connection, window_width, window_height);

        pixmap = xcb_generate_id(connection);
        xcb_create_pixmap(connection, 24,
                pixmap, window, image->width, image->height);

        graphics_context = create_graphics_context(pixmap);

        drawlineXD(image->data, 10, 10, 200, 300, window_width);

        xcb_image_put(connection, pixmap, graphics_context, image, 0, 0, 0);

        xcb_flush(connection);

        event_loop(window, graphics_context, pixmap);

        log_info("bye!");

        return EXIT_SUCCESS;
}
