#ifndef PLOT_X11_H
#define PLOT_X11_H

#include <X11/keysym.h>
#include <X11/Xlib.h>

typedef struct
{
	unsigned long black;
	unsigned long blue;
	unsigned long green;
	unsigned long cyan;
	unsigned long red;
	unsigned long magenta;
	unsigned long brown;
	unsigned long light_gray;
	unsigned long dark_gray;
	unsigned long light_blue;
	unsigned long light_green;
	unsigned long light_cyan;
	unsigned long yellow;
	unsigned long white;
} COLORS;

#define BLACK (rColors.black)
#define BLUE (rColors.blue)
#define GREEN (rColors.green)
#define CYAN (rColors.cyan)
#define RED (rColors.red)
#define MAGENTA (rColors.magenta)
#define BROWN (rColors.brown)
#define LIGHTGRAY (rColors.light_gray)
#define DARKGRAY (rColors.dark_gray)
#define LIGHTBLUE (rColors.light_blue)
#define LIGHTGREEN (rColors.light_green)
#define LIGHTCYAN (rColors.light_cyan)
#define YELLOW (rColors.yellow)
#define WHITE (rColors.white)

extern COLORS rColors;
extern int nWWidth;
extern int nWHeight;

#define KEY_PRESS_NOTHING 0
#define KEY_PRESS_EXPOSE 1
#define KEY_PRESS_QUIT (-1)

int DrawWindow(void (*DrawWindowContent) (void),
		int (*KeyPressFunction) (int));

void WSetColor(unsigned long color);
void WDrawString(const char *string, int x, int y);
void WDrawPoint(int x, int y);
void WDrawLine(int x_start, int y_start, int x_end, int y_end);
void WDrawRectangle(int x_top_left, int y_top_left,
		int x_bottom_right, int y_bottom_right);
void WFillRectangle(int x_top_left, int y_top_left,
		int x_bottom_right, int y_bottom_right);
void WFillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
void WFillPoligon(XPoint *points, int num);

void WSetFillStyle(int Style);

void WSetStipple(unsigned int Index);
void WSetTile(unsigned int Index);

int WGetColor(unsigned int Red, unsigned int Green, unsigned int Blue, unsigned long *pColor);

void WSetTitle(const char *s);

#define PLOT_X11_ERR_1 1
#define PLOT_X11_ERR_2 2
#define PLOT_X11_ERR_3 3
#define PLOT_X11_ERR_4 4
#define PLOT_X11_ERR_5 5


#define PLOT_X11_ERR_MSG_1 "Невозможно подключиться к X серверу."
#define PLOT_X11_ERR_MSG_2 "Недостаточно памяти для создания структур данных Х Шиндошс"
#define PLOT_X11_ERR_MSG_3 "Невозможно выделить достаточно цветов в Х Шиндошс палитре"
#define PLOT_X11_ERR_MSG_4 "Невозможно выделить достаточно Х битовых карт"
#define PLOT_X11_ERR_MSG_5 "Плохое Шиндош"
#define PLOT_X11_ERR_MSG_DEF "Неизвестная ошибка в коде Х Шиндошс интерфейса."

#endif























