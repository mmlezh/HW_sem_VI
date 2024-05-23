#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include "plot.h"

#define WND_X 0
#define WND_Y 0
#define WND_WDT 800
#define WND_HGT 600
#define WND_MIN_WDT 50
#define WND_MIN_HGT 50
#define WND_BORDER_WDT 5

#define WND_TITLE "Test"
#define WND_ICON_TITLE "Test"
#define PRG_CLASS "Test"

static int
SetWindowManagerHints(Display *prDisplay, 
		const char *psPrgClass,
		Window nWnd,
		int nMinWidth,
		int nMinHeight,
		const char *psTitle,
		const char *psIconTitle,
		Pixmap nIconPixmap)
{
	XSizeHints rSizeHints;
	XWMHints rWMHints;
	XClassHint rClassHint;
	XTextProperty prWindowName, prIconName;

	if(!XStringListToTextProperty ((char**)&psTitle, 1, &prWindowName)
			|| !XStringListToTextProperty ((char**)&psIconTitle, 1, &prIconName))
		return 1;

	rSizeHints.flags = PPosition | PSize | PMinSize;
	rSizeHints.min_width = nMinWidth;
	rSizeHints.min_height = nMinHeight;
	
	rWMHints.flags = StateHint | IconPixmapHint | InputHint;
	rWMHints.initial_state = NormalState;
	rWMHints.input = True;
	rWMHints.icon_pixmap = nIconPixmap;

	rClassHint.res_name = 0;
	rClassHint.res_class = (char*) psPrgClass;

	XSetWMProperties(prDisplay, nWnd, &prWindowName, &prIconName, 0, 0,
			&rSizeHints, &rWMHints, &rClassHint);

	return 0;
}

static int AllocColors(Display *prDisplay, Colormap colormap, COLORS *colors)
{
	XColor rColor;
	XColor rColorBase;

	if(!XAllocNamedColor(prDisplay, colormap, "black", &rColor, &rColorBase))
		return 1;
	colors->black = rColor.pixel;

	if(!XAllocNamedColor(prDisplay, colormap, "blue", &rColor, &rColorBase))
		return 2;
	colors->blue = rColor.pixel;

	if(!XAllocNamedColor(prDisplay, colormap, "green", &rColor, &rColorBase))
		return 3;
	colors->green = rColor.pixel;

	if(!XAllocNamedColor(prDisplay, colormap, "cyan", &rColor, &rColorBase))
		return 4;
	colors->cyan = rColor.pixel;
	
	if(!XAllocNamedColor(prDisplay, colormap, "red", &rColor, &rColorBase))
		return 5;
	colors->red = rColor.pixel;

	if(!XAllocNamedColor(prDisplay, colormap, "magenta", &rColor, &rColorBase))
		return 6;
	colors->magenta = rColor.pixel;

	if(!XAllocNamedColor(prDisplay, colormap, "brown", &rColor, &rColorBase))
		return 7;
	colors->brown = rColor.pixel;

	if(!XAllocNamedColor(prDisplay, colormap, "light gray", &rColor, &rColorBase))
		return 8;
	colors->light_gray = rColor.pixel;
	
	if(!XAllocNamedColor(prDisplay, colormap, "dark gray", &rColor, &rColorBase))
		return 9;
	colors->dark_gray = rColor.pixel;

	if(!XAllocNamedColor(prDisplay, colormap, "light blue", &rColor, &rColorBase))
		return 10;
	colors->light_blue = rColor.pixel;
	
	if(!XAllocNamedColor(prDisplay, colormap, "light green", &rColor, &rColorBase))
		return 11;
	colors->light_green = rColor.pixel;
	
	if(!XAllocNamedColor(prDisplay, colormap, "light cyan", &rColor, &rColorBase))
		return 12;
	colors->light_cyan = rColor.pixel;
	
	if(!XAllocNamedColor(prDisplay, colormap, "yellow", &rColor, &rColorBase))
		return 13;
	colors->yellow = rColor.pixel;
	
	if(!XAllocNamedColor(prDisplay, colormap, "white", &rColor, &rColorBase))
		return 14;
	colors->white = rColor.pixel;

	return 0;
}

static int CreateBitmaps(Display *prDisplay, Drawable nWnd, Pixmap *prPixmap)
{
	unsigned char psBitmapBits[17][8] = {
		{//0
			0xff,
			0xff,
			0xff,
			0xff,
			0xff,
			0xff,
			0xff,
			0xff
		},
		{//1
			0xff,
			0xff,
			0xff,
			0xee,
			0xff,
			0xff,
			0xff,
			0xee
		},
		{//2
			0xff,
			0xbb,
			0xff,
			0xee,
			0xff,
			0xbb,
			0xff,
			0xee
		},
		{//3
			0xff,
			0xaa,
			0xff,
			0xee,
			0xff,
			0xaa,
			0xff,
			0xee
		},
		{//4
			0xff,
			0xaa,
			0xff,
			0xaa,
			0xff,
			0xaa,
			0xff,
			0xaa
		},
		{//5
			0xff,
			0xaa,
			0xdd,
			0xaa,
			0xff,
			0xaa,
			0xdd,
			0xaa
		},
		{//6
			0xdd,
			0xaa,
			0xdd,
			0xaa,
			0xdd,
			0xaa,
			0xdd,
			0xaa
		},
		{//7
			0x55,
			0xaa,
			0xdd,
			0xaa,
			0x55,
			0xaa,
			0xdd,
			0xaa
		},
		{//8
			0x55,
			0xaa,
			0x55,
			0xaa,
			0x55,
			0xaa,
			0x55,
			0xaa
		},
		{//9
			0xaa,
			0x55,
			0x22,
			0x55,
			0xaa,
			0x55,
			0x22,
			0x55
		},
		{//10
			0x22,
			0x55,
			0x22,
			0x55,
			0x22,
			0x55,
			0x22,
			0x55
		},
		{//11
			0x00,
			0x55,
			0x22,
			0x55,
			0x00,
			0x55,
			0x22,
			0x55
		},
		{//12
			0x00,
			0x55,
			0x00,
			0x55,
			0x00,
			0x55,
			0x00,
			0x55
		},
		{//13
			0x00,
			0x55,
			0x00,
			0x11,
			0x00,
			0x55,
			0x00,
			0x11
		},
		{//14
			0x00,
			0x44,
			0x00,
			0x11,
			0x00,
			0x44,
			0x00,
			0x11
		},
		{//15
			0x00,
			0x00,
			0x00,
			0x11,
			0x00,
			0x00,
			0x00,
			0x11
		},
		{//16
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00
		},
	};
	
	for(int i = 0; i <= 16; ++i)
		if(!(prPixmap[i] = XCreateBitmapFromData(prDisplay, nWnd, 
						(char*)(psBitmapBits[i]), 8, 8)))
			return i+1;
	return 0;
}


static void FreeBitmaps(Display *prDisplay, Pixmap *prPixmap)
{
	for(int i = 0; i <= 16; ++i)
		XFreePixmap(prDisplay, prPixmap[i]);
}

static Display *prDisplay;
static int nScreenNum;
static GC prGC;
static Window nWnd;
static Drawable draw;
static Pixmap prPixmap[17];

COLORS rColors;
int nWWidth = WND_WDT;
int nWHeight = WND_HGT;


int DrawWindow(void (*DrawWindowContent) (void), int (*KeyPressFunction) (int))
{
	XEvent rEvent;
	XWindowAttributes rAttributes;
	KeySym nKeySym;
	int nDepth;

	if((prDisplay = XOpenDisplay(NULL)) == NULL)
		return PLOT_X11_ERR_1;
	
	nScreenNum = DefaultScreen(prDisplay);
	nWnd = XCreateSimpleWindow(prDisplay, RootWindow(prDisplay, nScreenNum),
			WND_X, WND_Y, WND_WDT, WND_HGT, WND_BORDER_WDT,
			BlackPixel(prDisplay, nScreenNum),
			WhitePixel(prDisplay, nScreenNum));

	if(SetWindowManagerHints(prDisplay, PRG_CLASS, nWnd, WND_MIN_WDT,
				WND_MIN_HGT, WND_TITLE, WND_ICON_TITLE, 0))
		return PLOT_X11_ERR_2;

	XSelectInput(prDisplay, nWnd, ExposureMask | KeyPressMask);
	XMapWindow(prDisplay, nWnd);

	prGC = XCreateGC(prDisplay, nWnd, 0, NULL);
	if(AllocColors(prDisplay, DefaultColormap(prDisplay, nScreenNum), &rColors))
		return PLOT_X11_ERR_3;
	if(CreateBitmaps(prDisplay, nWnd, prPixmap))
		return PLOT_X11_ERR_4;

	if(XGetWindowAttributes(prDisplay, nWnd, &rAttributes))
	{
		nWWidth = rAttributes.width;
		nWHeight = rAttributes.height;
		nDepth = rAttributes.depth;
	}
	else
		return PLOT_X11_ERR_5;

	draw = XCreatePixmap(prDisplay, nWnd, nWWidth, nWHeight, nDepth);
	if(!draw)
		return PLOT_X11_ERR_4;
	for(;;)
	{
		XNextEvent(prDisplay, &rEvent);
		switch(rEvent.type)
		{
			case Expose:
				if(rEvent.xexpose.count != 0)
					break;
				if(XGetWindowAttributes(prDisplay, nWnd, &rAttributes))
				{
					if(nWWidth != rAttributes.width ||
						nWHeight != rAttributes.height ||
						nDepth != rAttributes.depth)
					{
						XFreePixmap(prDisplay, draw);
						nWWidth = rAttributes.width;
						nWHeight = rAttributes.height;
						nDepth = rAttributes.depth;
						draw = XCreatePixmap(prDisplay, nWnd,
							nWWidth, nWHeight, nDepth);
						if(!draw)
							return PLOT_X11_ERR_4;
					}
				}
				else
					return PLOT_X11_ERR_5;

				DrawWindowContent();
				XCopyArea(prDisplay, draw, nWnd, prGC, 0, 0,
						nWWidth, nWHeight, 0, 0);
				break;
			case KeyPress:
				XLookupString(&rEvent.xkey, NULL, 0, &nKeySym, NULL);
				switch(KeyPressFunction(nKeySym))
				{
					case KEY_PRESS_NOTHING:
						break;
					case KEY_PRESS_EXPOSE:
						rEvent.type = Expose;
						rEvent.xexpose.count = 0;
						XSendEvent(prDisplay, 
								nWnd, True, 0, &rEvent);
						break;
					default:
					case KEY_PRESS_QUIT:
						XFreePixmap(prDisplay, draw);
						FreeBitmaps(prDisplay, prPixmap);
						XFreeGC(prDisplay, prGC);
						XCloseDisplay(prDisplay);
						return 0;
				}
			break;
		}
		while(XCheckMaskEvent(prDisplay, KeyPressMask | KeyReleaseMask,
					&rEvent));
	}
	return 0;
}

void WSetColor(unsigned long color)
{
	XSetForeground(prDisplay, prGC, color);
}

void WDrawString(const char *string, int x, int y)
{
	XDrawString(prDisplay, draw, prGC, x, y, string, strlen(string));
}

void WDrawPoint(int x, int y)
{
	XDrawPoint(prDisplay, draw, prGC, x, y);
}

void WDrawLine(int x_start, int y_start, int x_end, int y_end)
{
	XDrawLine(prDisplay, draw, prGC, x_start, y_start, x_end, y_end);
}

void WDrawRectangle(int x_top_left, int y_top_left,
		int x_bottom_right, int y_bottom_right)
{
	XDrawRectangle(prDisplay, draw, prGC, x_top_left, y_top_left,
			x_bottom_right - x_top_left + 2,
			y_bottom_right - y_top_left + 2);
}

void WFillRectangle(int x_top_left, int y_top_left, int x_bottom_right,
	int y_bottom_right)
{
	XFillRectangle(prDisplay, draw, prGC, x_top_left, y_top_left,
		x_bottom_right - x_top_left + 2,
		y_bottom_right - y_top_left + 2);
}

void WFillTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	XPoint points[3] = { {x1, y1}, {x2, y2}, {x3, y3} };
	XFillPolygon(prDisplay, draw, prGC, points, 3, Convex, CoordModeOrigin);
}

void WFillPoligon(XPoint *points, int num)
{
	XFillPolygon(prDisplay, draw, prGC, points, num, Convex, CoordModeOrigin);
}

void WSetFillStyle(int Style)
{
	XSetFillStyle(prDisplay, prGC, Style);
}

void WSetStipple(unsigned int Index)
{
	XSetStipple(prDisplay, prGC, prPixmap[Index]);
}

void WSetTile(unsigned int Index)
{
	XSetTile(prDisplay, prGC, prPixmap[Index]);
}

int WGetColor(unsigned int Red, unsigned int Green, unsigned int Blue, unsigned long *pColor)
{
	XColor rColor;
	rColor.red = Red << 8;
	rColor.green = Green << 8;
	rColor.blue = Blue << 8;
	rColor.flags = DoRed | DoGreen | DoBlue;

	if(XAllocColor(prDisplay, DefaultColormap(prDisplay, nScreenNum), &rColor) == 0)
		return 1;
	*pColor = rColor.pixel;
	return 0;
}

void WSetTitle(const char *s)
{
	XTextProperty tProp;
	XStringListToTextProperty((char**)&s, 1, &tProp);
	XSetWMName(prDisplay, nWnd, &tProp);
}





