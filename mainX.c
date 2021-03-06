// gcc `pkg-config --libs xcb xcb-image` main.c -o main

??=include <stdio.h>
%:include <stdlib.h>
%:include <aio.h>

??=include <xcb/xcb.h>
??=include <xcb/xcb_image.h>

%:include <fdebug.h>

%:include <inttypes.h>


%:define WW 800
%:define WH 800

float window_width = WW;
float window_height = WH;
uint64_t window_width_int = WW;
uint64_t window_height_int = WH;

??=undef WW
??=undef WH

float rotationY = 0;
float rotationX = 0;

xcb_connection_t *connection;
xcb_screen_t *screen;
xcb_pixmap_t pixmap;

xcb_image_t *image;

void * ptr;

void clear(void);
void redrawWrapper(xcb_gcontext_t graphics_context, xcb_window_t window);

xcb_window_t create_window() {
        uint32_t mask;
        uint32_t values[2];

        xcb_window_t window;
        xcb_void_cookie_t cookie;

        mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
        values[0] = screen->white_pixel;
        values[1] = XCB_EVENT_MASK_EXPOSURE
                | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_BUTTON_1_MOTION;

        window = xcb_generate_id(connection);
        cookie = xcb_create_window(connection,
                XCB_COPY_FROM_PARENT, window, screen->root,
                0, 0, window_width, window_height,
                0,
                XCB_WINDOW_CLASS_INPUT_OUTPUT,
                screen->root_visual,
                mask, values
        );
        UNUSED(cookie);

        xcb_map_window(connection, window);
        return window;
}

xcb_gcontext_t create_graphics_context(xcb_pixmap_t drawable) {
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
                        return fmt;
                }
        return 0;
}

void fillimage(unsigned char *p, int width, int height) {
        int i, j;
        for(i=0; i < width; i++) {
                for(j=0; j < height; j++) {
                        *p++= !(j%10000)?0:250;
                        *p++= !(i%10000)?0:250;
                        *p++=250;
                        p++;
                }
        }
}


void putpixel(unsigned char *p, int x, int y, int width) {
        log_warn1("calling c implementation");
        p += 4* y * width;
        p += 4* x;

        debug("pp at %p", p);
        *(p+0) = 0;
        *(p+1) = 0;
        *(p+2) = 0;
}

void
drawlineXD(unsigned char *p, int x0, int y0, int x1, int y1, int width) {
        log_warn1("calling c implementation");
        debug("(%d, %d) -> (%d, %d)", x0, y0, x1, y1);
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
        unsigned char *image32 = (unsigned char *)malloc(width*height*4);
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


#define CLEANMASK(mask) (mask & ~(XCB_MOD_MASK_LOCK))

#define KEY_LEFT 38
#define KEY_RIGHT 40
#define KEY_UP 25
#define KEY_DOWN 39

#define KEY_ESC 9


void
event_loop(xcb_window_t window, xcb_gcontext_t graphics_context,
                xcb_pixmap_t pixmap) {
        xcb_generic_event_t *event;

        xcb_key_press_event_t *press;

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

                        case XCB_MOTION_NOTIFY:
                        return;

                        case XCB_KEY_PRESS:
                        press = (xcb_key_press_event_t *)event;
                        switch(press->detail) {
                                case KEY_LEFT:
                                rotationY += 0.05;
                                clear();
                                redrawWrapper(graphics_context, window);
                                break;

                                case KEY_RIGHT:
                                rotationY -= 0.05;
                                clear();
                                redrawWrapper(graphics_context, window);
                                break;

                                case 27:
                                clear();
                                rotationX = 0;
                                rotationY = 0;
                                redrawWrapper(graphics_context, window);
                                debug1("reset");
                                break;

                                case KEY_DOWN:
                                clear();
                                rotationX -= 0.05;
                                redrawWrapper(graphics_context, window);
                                break;

                                case KEY_UP:
                                clear();
                                rotationX += 0.05;
                                redrawWrapper(graphics_context, window);
                                break;

                                case KEY_ESC:
                                case 24:
                                return;

                                default:
                                log_info("unrecognized keypress : %d\n", press->detail);
                        }
                }
        }
}

void redraw(unsigned char *p, float angleX, float angleY);
void sa_redraw(unsigned char *p, float angleX, float angleY);
void a_redraw(unsigned char *p, float angleX, float angleY);

struct aiocb aiocb;
unsigned retries = 0;
#define BUF_SIZE 512
char buf[BUF_SIZE];

void redrawWrapper(xcb_gcontext_t graphics_context, xcb_window_t window) {
        /*
        if(aio_error(&aiocb) == EINPROGRESS) {
                retries++;
        } else {
                aiocb.aio_fildes = 1;
                aiocb.aio_buf = buf;
                aiocb.aio_nbytes = BUF_SIZE;

                snprintf(buf, BUF_SIZE, "updating [%u]\n", retries);

                aio_write(&aiocb);      // we don't really care
                                        // that much about
                                        // completion

                retries = 1;
        }
        */

        ptr = image->data;
        a_redraw(image->data, rotationX, rotationY);

        xcb_image_put(connection, pixmap, graphics_context, image, 0, 0, 0);

        xcb_copy_area(connection,
                pixmap, window, graphics_context,
                0, 0, 0, 0,
                window_width, window_height
        );
        xcb_flush(connection);
}


void clear(void) {
        fillimage(image->data, window_width, window_height);
}

int main(void) {
        xcb_window_t window;
        xcb_gcontext_t graphics_context;

        connection = xcb_connect(NULL, NULL);

        screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;

        window = create_window();

        image = CreateTrueColorImage(connection, window_width, window_height);

        pixmap = xcb_generate_id(connection);
        xcb_create_pixmap(connection, 24,
                pixmap, window, image->width, image->height);

        graphics_context = create_graphics_context(pixmap);

        xcb_image_put(connection, pixmap, graphics_context, image, 0, 0, 0);

        xcb_flush(connection);

	redrawWrapper(graphics_context, window);

        event_loop(window, graphics_context, pixmap);

        log_info1("bye!");

        return EXIT_SUCCESS;
}
