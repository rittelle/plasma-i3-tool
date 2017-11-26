#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define DESKTOP_WM_NAME "Desktop"
#define DESKTOP_WM_CLASS_CLASS "plasmashell"
#define DESKTOP_WM_CLASS_NAME "plasmashell"

int find_plasma_desktop(Display *display, Window window, Window *window_return);

int setup_plasma_desktop(Display *display, Window window);

int main() {
    int ret = 0;
    Display *display = XOpenDisplay(NULL);
    assert(display != NULL);
    XSynchronize(display, True);

    Window root = DefaultRootWindow(display); // TODO: Multi monitor setups
    Window desktop;

    if (find_plasma_desktop(display, root, &desktop)) {
        printf("Plasma window found\n");

        if (!setup_plasma_desktop(display, desktop)) {
            printf("Setup failed\n");
        }
    } else {
        printf("Plasma window not found\n");

        ret = 1;
    }

    XCloseDisplay(display);
    return ret;
}

int find_plasma_desktop(Display *display, Window window, Window *window_return) {
    int ret = 1;
    // Return values from XQueryTree
    Window root;
    Window parent;
    Window *children = NULL;
    unsigned int children_n;

    // Return values of XGetWMName
    XTextProperty textProperty = {.value = NULL};

    // Return value of XGetClassHint
    XClassHint classHint = {.res_class = NULL, .res_name = NULL};

    if (!XQueryTree(display, window, &root, &parent, &children, &children_n)) { // TODO: Handle BadWindow
        fprintf(stderr, "XQueryTree failed\n");
        goto fail;
    }

    if (XGetWMName(display, window, &textProperty) &&
        strncmp((const char *) textProperty.value, DESKTOP_WM_NAME, strlen(DESKTOP_WM_NAME)) == 0 &&
        XGetClassHint(display, window, &classHint) &&
        strncmp(classHint.res_class, DESKTOP_WM_CLASS_CLASS, strlen(DESKTOP_WM_CLASS_CLASS)) == 0 &&
        strncmp(classHint.res_name, DESKTOP_WM_CLASS_NAME, strlen(DESKTOP_WM_CLASS_NAME)) == 0) {
        *window_return = window;
        goto cleanup;
    }

    if (children != NULL) {
        for (int i = 0; i < children_n; ++i) {
            if (find_plasma_desktop(display, children[i], window_return)) {
                goto cleanup;
            }
        }
    }

    fail:
    ret = 0;
    goto cleanup;

    cleanup:
    if (children != NULL) {
        XFree(children);
    }
    if (textProperty.value != NULL) {
        XFree(textProperty.value);
    }
    if (classHint.res_class != NULL) {
        XFree(classHint.res_class);
    }
    if (classHint.res_name != NULL) {
        XFree(classHint.res_name);
    }
    return ret;
}

int setup_plasma_desktop(Display *display, Window window) {
    // Return value for XGetWindowAttributes
    XSetWindowAttributes setWindowAttributes;

    setWindowAttributes.override_redirect = 1;
    if (!XChangeWindowAttributes(display, window, CWOverrideRedirect, &setWindowAttributes)) {
        return 0;
    }

    if (!XUnmapWindow(display, window)) {
        return 0;
    }

    if (!XMapWindow(display, window)) {
        return 0;
    }

    if (!XMoveWindow(display, window, 0, 0)) {
        return 0;
    }

    if (!XLowerWindow(display, window)) {
        return 0;
    }

    return 1;
}


