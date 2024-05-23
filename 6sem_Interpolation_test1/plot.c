#ifndef _WIN32

/* linux
*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "plot.h"

static int wnd_x = 0;
static int wnd_y = 0;
static int wnd_w = 800;
static int wnd_h = 600;
static int done = 0;
static char wnd_title[1024];

static Display *display = NULL;
static int ScrNum;
static Window window;
static Drawable draw;
static GC prGC;
static Font font;

typedef void (* PFRENDER) (int width, int height);
typedef void (* PFKEYBOARD) (int key);

static PFRENDER render = NULL;
static PFKEYBOARD keyboard = NULL;

void xlibInitPosition(int x, int y)
{
	wnd_x = x;
	wnd_y = y;
}

void xlibInitWindowSize(int width, int height)
{
	wnd_w = width;
	wnd_h = height;
}

void xlibRenderFunc(void (*func)(int width, int height))
{
	render = func;
}

void xlibKeyboardFunc(void (*func)(int key))
{
	keyboard = func;
}

void xlibPostRedisplay(void)
{
	XEvent event;

	event.type = Expose;
	event.xexpose.count = 0;
	XSendEvent(display, window, True, 0, &event);
}

void xlibPostExit(void)
{
	done = 1;
}

static void Exit(void)
{
	if (draw) XFreePixmap(display, draw);
	if (font) XUnloadFont(display, font);
	if (prGC) XFreeGC(display, prGC);
	if (window) XDestroyWindow(display, window);
	XFlush(display);
	if (display) XCloseDisplay(display);
}

static int translateKey(int key)
{
	int ret;

	if (key & 0xff00)
	{
		switch (key)
		{
		case XK_Escape:		ret = KEY_ESC;		break;
		case XK_Tab:		ret = KEY_TAB;		break;
		case XK_Return:		ret = KEY_RETURN;	break;
		case XK_BackSpace:	ret = KEY_BACKSPACE;	break;
		case XK_Delete:		ret = KEY_DELETE;	break;
		case XK_Home:		ret = KEY_HOME;		break;
		case XK_End:		ret = KEY_END;		break;
		case XK_Page_Up:	ret = KEY_PGUP;		break;
		case XK_Page_Down:	ret = KEY_PGDOWN;	break;
		case XK_Left:		ret = KEY_LEFT;		break;
		case XK_Right:		ret = KEY_RIGHT;	break;
		case XK_Up:		ret = KEY_UP;		break;
		case XK_Down:		ret = KEY_DOWN;		break;
		case XK_Shift_L:	ret = KEY_SHIFT;	break;
		case XK_Shift_R:	ret = KEY_SHIFT;	break;
		case XK_Control_L:	ret = KEY_CTRL;		break;
		case XK_Control_R:	ret = KEY_CTRL;		break;
		case XK_Alt_L:		ret = KEY_ALT;		break;
		case XK_Alt_R:		ret = KEY_ALT;		break;
		case XK_F1:		ret = KEY_F1;		break;
		case XK_F2:		ret = KEY_F2;		break;
		case XK_F3:		ret = KEY_F3;		break;
		case XK_F4:		ret = KEY_F4;		break;
		case XK_F5:		ret = KEY_F5;		break;
		case XK_F6:		ret = KEY_F6;		break;
		case XK_F7:		ret = KEY_F7;		break;
		case XK_F8:		ret = KEY_F8;		break;
		case XK_F9:		ret = KEY_F9;		break;
		case XK_F10:		ret = KEY_F10;		break;
		case XK_F11:		ret = KEY_F11;		break;
		case XK_F12:		ret = KEY_F12;		break;
		default: ret = (key < 0x06ff) ? (key & 0x00ff) : 0;
		}
	}
	else ret = key;

	if (ret >= 512) ret = 0;

	return ret;
}

void xlibMainLoop(const char *title)
{
	int wnd_depth;
	XEvent event;
	XWindowAttributes attr;
	KeySym key;
	Atom WM_DELETE_WINDOW;

	if ((display = XOpenDisplay(NULL)) == NULL)
	{
		printf("Cannot connect to the X server.\n");
		return;
	}

	ScrNum = DefaultScreen(display);

	window = XCreateSimpleWindow(display, RootWindow(display, ScrNum),
				    wnd_x, wnd_y, wnd_w, wnd_h, 5,
				    BlackPixel(display, ScrNum), WhitePixel(display, ScrNum));

	WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(display, window, &WM_DELETE_WINDOW, 1);

	XSelectInput(display, window, ExposureMask | KeyPressMask);
	XFlush(display);

	strcpy(wnd_title, title);
	XStoreName(display, window, wnd_title);
	XSetIconName(display, window, wnd_title);

	XMapWindow(display, window);

	prGC = XCreateGC(display, window, 0, NULL);
	if ((font = XLoadFont(display, "*fixed*120*koi8-r"))) XSetFont(display, prGC, font);

	if (!XGetWindowAttributes(display, window, &attr))
	{
		printf("Bad window.\n");
		Exit();
		return;
	}

	wnd_w = attr.width;
	wnd_h = attr.height;
	wnd_depth = attr.depth;

	draw = XCreatePixmap(display, window, wnd_w, wnd_h, wnd_depth);
	if (!draw)
	{
		printf("Bad draw.\n");
		Exit();
		return;
	}

	while (!done)
	{
		XNextEvent(display, &event);

		switch (event.type)
		{
		case ClientMessage:
			if (event.xclient.format == 32 && event.xclient.data.l[0] == (long)WM_DELETE_WINDOW)
			{
				Exit();
				return;
			}
			break;
		case Expose:
			if (event.xexpose.count != 0) break;

			if (!XGetWindowAttributes(display, window, &attr))
			{
				printf("Bad window.\n");
				Exit();
				return;
			}

			if (wnd_w != attr.width || wnd_h != attr.height || wnd_depth != attr.depth)
			{
				XFreePixmap(display, draw);
				wnd_w = attr.width;
				wnd_h = attr.height;
				wnd_depth = attr.depth;
				draw = XCreatePixmap(display, window, wnd_w, wnd_h, wnd_depth);
				if (!draw)
				{
					printf("Bad window.\n");
					Exit();
					return;
				}
			}

			render(wnd_w, wnd_h);

			XCopyArea(display, draw, window, prGC, 0, 0, wnd_w, wnd_h, 0, 0);
			XFlush(display);
			break;
		case KeyPress:
			XLookupString(&event.xkey, NULL, 0, &key, NULL);
			key = translateKey(key);
			if (!key) break;

			keyboard(key);
			break;
		}

		while (XCheckMaskEvent(display, KeyPressMask | KeyReleaseMask, &event)) ;
	}

	Exit();
}

void xlibSetTitle(const char *title)
{
	strcpy(wnd_title, title);
	XStoreName(display, window, wnd_title);
	XSetIconName(display, window, wnd_title);
	XFlush(display);
}

void xlibSetColor(double red, double green, double blue)
{
	XColor clr;
	Colormap map = DefaultColormap(display, ScrNum);

	clr.red = red * 65535;
	clr.green = green * 65535;
	clr.blue = blue * 65535;
	clr.flags = DoRed | DoGreen | DoBlue;

	XAllocColor(display, map, &clr);
	XSetForeground(display, prGC, clr.pixel);
}

void xlibDrawString(int x, int y, const char *string, ...)
{
	char buf[4096];

	va_list argptr;
	va_start(argptr, string);
	//vsnprintf(buf, sizeof(buf), string, argptr);
	va_end(argptr);
	XDrawString(display, draw, prGC, x, y, buf, strlen(buf));
}

void xlibDrawPoint(int x, int y)
{
	XDrawPoint(display, draw, prGC, x, y);
}

void xlibDrawLine(int x_start, int y_start, int x_end, int y_end)
{
	XDrawLine(display, draw, prGC, x_start, y_start, x_end, y_end);
}

void xlibDrawRectangle(int x_top_left, int y_top_left, int x_bottom_right, int y_bottom_right)
{
	XDrawRectangle(display, draw, prGC, x_top_left, y_top_left,
		    x_bottom_right - x_top_left + 1,
		    y_bottom_right - y_top_left + 1);
}

void xlibFillRectangle(int x_top_left, int y_top_left, int x_bottom_right, int y_bottom_right)
{
	XFillRectangle(display, draw, prGC, x_top_left, y_top_left,
		    x_bottom_right - x_top_left + 2,
		    y_bottom_right - y_top_left + 2);
}

void xlibDrawTriangle(int x_1, int y_1, int x_2, int y_2, int x_3, int y_3)
{
	XPoint points[3];

	points[0].x = x_1; points[0].y = y_1;
	points[1].x = x_2; points[2].y = y_2;
	points[2].x = x_3; points[2].y = y_3;
	XDrawPoints(display, draw, prGC, points, 3, CoordModeOrigin);
}

void xlibFillTriangle(int x_1, int y_1, int x_2, int y_2, int x_3, int y_3)
{
	XPoint points[3];

	points[0].x = x_1; points[0].y = y_1;
	points[1].x = x_2; points[2].y = y_2;
	points[2].x = x_3; points[2].y = y_3;
	XFillPolygon(display, draw, prGC, points, 3, Convex, CoordModeOrigin);
}

#endif
