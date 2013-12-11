#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/cursorfont.h>
#include <X11/Xatom.h>
#include <stdio.h>

int main() {
        Display *dpy;
        XEvent event;

        if(!(dpy = XOpenDisplay(NULL)))
                return 1;

        Window root = DefaultRootWindow(dpy);
        XSetWindowAttributes wa;
        wa.cursor = XCreateFontCursor(dpy, XC_left_ptr);
        XChangeWindowAttributes(dpy, root, CWCursor, &wa);
        XSelectInput(dpy, root, SubstructureNotifyMask|SubstructureRedirectMask);

        XGrabKey(dpy, 53, Mod4Mask, root, True, GrabModeAsync, GrabModeAsync);
        XGrabKey(dpy, 45, Mod4Mask, root, True, GrabModeAsync, GrabModeAsync);

        for (;;) {
                XNextEvent(dpy, &event);
                if (event.type == MapRequest) {
                        XMapRequestEvent mapreq = event.xmaprequest;
                        XMapWindow(dpy, mapreq.window);
                        XSetInputFocus(dpy, mapreq.window, RevertToPointerRoot, CurrentTime);
                        XMoveResizeWindow(dpy, mapreq.window, 0, 0, 1920, 1080);
                } else if (event.type == KeyRelease) {
                        XKeyEvent keyev = event.xkey;
                        if (keyev.keycode == 45) {
                                Window focus;
                                int revert;
                                XGetInputFocus(dpy, &focus, &revert);
                                XDestroyWindow(dpy, focus);
                        } else {
                                system("dmenu_run");
                        }
                }
        }
}
