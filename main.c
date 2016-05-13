#include <X11/Xlib.h>
#include <assert.h>
#include <unistd.h>

int main() {
        Display *dpy = XOpenDisplay(0);

        assert(dpy);

        Window *w = XCreateWindow(dpy, DefaultRootWindow(dpy), 0, 0,
                        200, 100, 0,
                        CopyFromParent, CopyFromParent, CopyFromParent,
                        0, 0);

        XMapWindow(dpy, w);
        XFlush(dpy);

        sleep(10);
}
