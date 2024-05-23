#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "main.h"
#include "plot.h"

#include "metod_09.h"
#include "metod_31.h"
#include "metod_45.h"

#define EPS 1e-7

static int n = 20;
static double a = -10;
static double b = +10;

static double max, min;

static int k = 1;

static double x_left, x_right;
static double y_up, y_down;

static int variant = 0;

double F(int k, double x)// OK
{
	if(k == 0)
		return 1.0;
	if(k == 1)
		return x;
	if(k == 2)
		return x*x;
	if(k == 3)
		return x*x*x;
	if(k == 4)
		return x*x*x*x;
	if(k == 5)
		return exp(x);
	if(k == 6)
		return 1.0/(25.0*x*x + 1.0);

	return 1.0;
}

double f(double x)// OK
{
	return F(k, x);
}

double df(double x)// OK
{
	return 0.5 * (f(x + EPS) - f(x - EPS))/EPS;
}

double ddf(double x) // OK
{
	return 0.5 * (df(x + EPS) - df(x - EPS))/EPS;
}

static double Method1(double x) // OK
{
	return f_09(x);
}

static double Method2(double x) // OK
{
	return f_31(x);
}

static double Method3(double x) // OK
{
	return f_45(x);
}

static double ErrorMethod1(double x) // OK
{
	return f(x) - Method1(x);
}

static double ErrorMethod2(double x) // OK
{
	return f(x) - Method2(x);
}

static double ErrorMethod3(double x) // OK
{
	return f(x) - Method3(x);
}

void DrawFunction(int width, int height, double (*func)(double x)) //OK
{
	int i;
	double hx, hy;
	double x, y_start, y_end;

	hx = (x_right - x_left)/(width - 1);
	hy = height/(y_up - y_down);

	y_start = height - (func(x_left) - y_down) * hy;

	for (i = 1; i < width; i++)
	{
		x = x_left + i * hx;
		y_end = height - (func(x) - y_down) * hy;
		xlibDrawLine(i - 1, y_start, i, y_end);
		y_start = y_end;
	}
}

void CalcMaxAndMin(int width, double (*func)(double x), double* maxim, double* minim) // OK
{
	int i;
	double x, y, hx;

	*maxim = *minim = func(x_left);
	hx = (x_right - x_left)/(width - 1);

	for (i = 1; i < width; i++)
	{
		x = x_left + i * hx;
		y = func(x);
		if (y - *maxim > EPS) *maxim = y;
		if (y - *minim < EPS) *minim = y;
	}
}

void DisplayHelp(void)
{
    printf("HELp\n");
	printf("Operations:\n");
	printf("'1' = method\n");
    printf("'2' || '3' = zooming || unzoomig\n");
	printf("'4' || '5' =  increasing || decreasing n\n");
	printf("'6' || '7' = adding || removing values\n");
    printf("'d' = diagnostic information\n");
    printf("'h' = help\n");
    printf("=======================================\n");
}

void DisplayDiag(void)
{
    char s[256];
    
    switch (k)
	{
	case 0:
		sprintf(s, "f(x) = 1");
		return;
	case 1:
		sprintf(s, "f(x) = x");
		return;
	case 2:
		sprintf(s, "f(x) = x*x");
		break;
	case 3:
        sprintf(s, "f(x) = x*x*x");
		break;
	case 4:
		sprintf(s, "f(x) = x*x*x*x");
		break;
	case 5: 
		sprintf(s, "f(x) = e^x");
		break;
	case 6: 
		sprintf(s, "f(x) = 1/(25x * x + 1)");
		break;
	}
    
    printf("DIAGNOSTIc\n");
    switch(variant)
    {
        default:
           break;
        case 1: 
            printf("Интерполяция многочленами Чебышева по методу наименьших квадратов\n");
            printf("%s, a = %.2f, b = %.2f, n = %d\n", s, a, b, n);
            
            printf("=======================================\n");
            break;
        case 2:
            printf("Кусочная интерполяция кубическими многочленами с использованием разделенных разностей с определением недостающих гарничных условий из естественных граничных условий\n");
            printf("%s, a = %.2f, b = %.2f, n = %d\n", s, a, b, n);
            
            printf("=======================================\n");
            break;
        case 3:
            printf("Интерполяция параболическими сплайнами с определением недостающих граничных условий при помощи дополнительного узла в приграничных узлах\n");
            printf("%s, a = %.2f, b = %.2f, n = %d\n", s, a, b, n);
            
            printf("=======================================\n");
            break;
        case 4: 
            printf("График погрешности первого метода\n");
            printf("%s, a = %.2f, b = %.2f, n = %d\n", s, a, b, n);
             
            printf("=======================================\n");
            break;
        case 5: 
            printf("График погрешности второго метода\n");
            printf("%s, a = %.2f, b = %.2f, n = %d\n", s, a, b, n);
            
            printf("=======================================\n");
            break;
        case 6:
            printf("График погрешности третьего метода\n");
            printf("%s, a = %.2f, b = %.2f, n = %d\n", s, a, b, n);
            
            printf("=======================================\n");
            break;
            
    }
}

void render(int width, int height)
{
	double hx, hy;
	double r1, r2;


	xlibSetColor(0.0, 0.0, 0.0);
	xlibFillRectangle(0, 0, width, height);
//  DisplayHelp();

	xlibSetColor(1.0, 1.0, 1.0);
// 	xlibDrawString(50, 20, "%s, a = %.2f, b = %.2f, n = %d, k = %d, variant = %d", s, a, b, n, k, variant);
// 	xlibDrawString(50, 40, "'q'/'esc' - exit");

// 	xlibDrawString(50, 60, "x_left = %.2f  x_right = %.2f", x_left, x_right);

	switch(variant)
	{
		case 0:
		case 1:
		case 2:
		case 3:
			CalcMaxAndMin(width, f, &max, &min);
			y_up = max + 1.0;
			y_down = min - 1.0;
			break;
		case 4:
			CalcMaxAndMin(width, ErrorMethod1, &r1, &r2);
			y_up = r1 + 1.0;
			y_down = r2 - 1.0;
			if (fabs(r1) - fabs(r2) < EPS) r1 = r2;
			r1 = fabs(r1);
            printf("Погрешность первого метода = %e\n", r1);
// 			xlibDrawString(50, 80, "error metod 09 = %e", r1);

// 			CalcMaxAndMin(width, ErrorMethod2, &r1, &r2);
// 			if (r1 + 1.0 - y_up > EPS) y_up = r1 + 1.0;
// 			if (r2 - 1.0 - y_down < EPS) y_down = r2 - 1.0;
// 			if (fabs(r1) - fabs(r2) < EPS) r1 = r2;
// 			r1 = fabs(r1);
// 			xlibDrawString(50, 100, "error metod 31 = %e", r1);
			break;
		case 5:
			CalcMaxAndMin(width, ErrorMethod2, &r1, &r2);
			y_up = r1 + 1.0;
			y_down = r2 - 1.0;
			if (fabs(r1) - fabs(r2) < EPS) r1 = r2;
			r1 = fabs(r1);
            printf("Погрешность второго метода = %e\n", r1);
// 			xlibDrawString(50, 80, "error metod 31 = %e", r1);

// 			CalcMaxAndMin(width, ErrorMethod3, &r1, &r2);
// 			if (r1 + 1.0 - y_up > EPS) y_up = r1 + 1.0;
// 			if (r2 - 1.0 - y_down < EPS) y_down = r2 - 1.0;
// 			if (fabs(r1) - fabs(r2) < EPS) r1 = r2;
// 			r1 = fabs(r1);
// 			xlibDrawString(50, 100, "error metod 45 = %e", r1);
			break;
		case 6:
			CalcMaxAndMin(width, ErrorMethod1, &r1, &r2);
			y_up = r1 + 1.0;
			y_down = r2 - 1.0;
			if (fabs(r1) - fabs(r2) < EPS) r1 = r2;
			r1 = fabs(r1);
            printf("Погрешность третьего метода = %e\n", r1);
// 			xlibDrawString(50, 80, "error metod 09 = %e", r1);

// 			CalcMaxAndMin(width, ErrorMethod3, &r1, &r2);
// 			if (r1 + 1.0 - y_up > EPS) y_up = r1 + 1.0;
// 			if (r2 - 1.0 - y_down < EPS) y_down = r2 - 1.0;
// 			if (fabs(r1) - fabs(r2) < EPS) r1 = r2;
// 			r1 = fabs(r1);
// 			xlibDrawString(50, 100, "error metod 45 = %e", r1);
			break;
	}

	hx = (x_right - x_left)/(width - 1);
	hy = height/(y_up - y_down);

	xlibDrawLine(0, y_up * hy, width, y_up * hy);
	xlibDrawLine(1 - x_left/hx, 0, 1 - x_left/hx, height);

	switch (variant)
	{
	case 1:
		xlibSetColor(1.0, 0.0, 0.0);
		DrawFunction(width, height, Method1);

		xlibSetColor(1.0, 1.0, 1.0);
		DrawFunction(width, height, f);
		break;
	case 2:
		xlibSetColor(0.0, 1.0, 0.0);
		DrawFunction(width, height, Method2);

		xlibSetColor(1.0, 1.0, 1.0);
		DrawFunction(width, height, f);
		break;
	case 3: 
		xlibSetColor(0.0, 0.0, 1.0);
		DrawFunction(width, height, Method3);

		xlibSetColor(1.0, 1.0, 1.0);
		DrawFunction(width, height, f);
		break;
	case 4:
        xlibSetColor(0.0, 1.0, 1.0);
		DrawFunction(width, height, ErrorMethod1);
// 		xlibSetColor(1.0, 0.0, 0.0);
// 		DrawFunction(width, height, Method1);
// 
// 		xlibSetColor(0.0, 1.0, 0.0);
// 		DrawFunction(width, height, Method2);
// 
// 		xlibSetColor(1.0, 1.0, 1.0);
// 		DrawFunction(width, height, f);
		break;
	case 5: 
        xlibSetColor(1.0, 0.0, 1.0);
		DrawFunction(width, height, ErrorMethod2);
// 		xlibSetColor(0.0, 1.0, 0.0);
// 		DrawFunction(width, height, Method2);
// 
// 		xlibSetColor(0.0, 0.0, 1.0);
// 		DrawFunction(width, height, Method3);
// 
// 		xlibSetColor(1.0, 1.0, 1.0);
// 		DrawFunction(width, height, f);
		break;
	case 6:
        xlibSetColor(1.0, 1.0, 0.0);
		DrawFunction(width, height, ErrorMethod3);
// 		xlibSetColor(1.0, 0.0, 0.0);
// 		DrawFunction(width, height, Method1);
// 
// 		xlibSetColor(0.0, 0.0, 1.0);
// 		DrawFunction(width, height, Method3);
// 
// 		xlibSetColor(1.0, 1.0, 1.0);
// 		DrawFunction(width, height, f);
		break;
	}
}

void keyboard(int key)
{
	double tmp;
	int number;
	double delta;

	switch (key)
	{
	case 'q':
		xlibPostExit();
		return;
    case 'h':
        DisplayHelp();
        break;
    case 'd':
        DisplayDiag();
        break;
	case '0':
		k++;
		k = k % 7;
		break;
	case '1':
		variant++;
		variant = variant % 7;
		if (!variant) break;

		switch(variant)
		{
			case 0:
				break;
			case 1:
				Input_09();
				Calc_09();
				break;
			case 2: 
				Input_31();
				Calc_31();
				break;
			case 3:
				Input_45();
				Calc_45();
				break;
			case 4:
				Input_09();
				Calc_09();
				Input_31();
				Calc_31();
				break;
			case 5:
				Input_31();
				Calc_31();
				Input_45();
				Calc_45();
				break;
			case 6:
				Input_09();
				Calc_09();
				Input_45();
				Calc_45();
				break;
		}
		break;
	case '2':
	case '3':
		tmp = (x_right - x_left);
		if (key == '2') tmp /= 4.0;
		else tmp /= -2.0;
		x_left += tmp;
		x_right -= tmp;
		break;
	case '4':
	case '5':
		Finalize();

		if (key == '4') n *= 2;
		else if (n > 5) n /= 2;

		if (!Init_09(n, a, b) || !Init_31(n, a, b) || !Init_45(n, a, b))
		{
			printf("---ОШИБКА: Недостаточно памяти---\n");
			Finalize();
			xlibPostExit();
			return;
		}
		switch(variant)
		{
			case 1:
				Input_09();
				Calc_09();
				break;
			case 2: 
				Input_31();
				Calc_31();
				break;
			case 3:
				Input_45();
				Calc_45();
				break;
			case 4:
				Input_09();
				Calc_09();
				Input_31();
				Calc_31();
				break;
			case 5:
				Input_31();
				Calc_31();
				Input_45();
				Calc_45();
				break;
			case 6:
				Input_09();
				Calc_09();
				Input_45();
				Calc_45();
				break;
		}

		break;
	case '6':
	case '7':
		number = n/3;
		if (key == '6') delta = +3.0;
		else delta = -3.0;


		switch(variant)
		{
			case 1:
				Delta_09(number, delta);
				Calc_09();
				break;
			case 2: 
				Delta_31(number, delta);
				Calc_31();
				break;
			case 3:
				Delta_45(number, delta);
				Calc_45();
				break;
			case 4:
				Delta_09(number, delta);
				Calc_09();
				Delta_31(number, delta);
				Calc_31();
				break;
			case 5:
				Delta_31(number, delta);
				Calc_31();
				Delta_45(number, delta);
				Calc_45();
				break;
			case 6:
				Delta_09(number, delta);
				Calc_09();
				Delta_45(number, delta);
				Calc_45();
				break;
		}		

		break;
	case KEY_LEFT:
		x_left -= 1.0;
		x_right -= 1.0;
		break;
	case KEY_RIGHT:
		x_left += 1.0;
		x_right += 1.0;
		break;
	default:
		return;
	}

	xlibPostRedisplay();
}


int Init(void)
{
	if (b <= a || n < 3)
	{
		printf("---ОШИБКА: Some input error---\n");
		return 0;
	}

	x_left = a;
	x_right = b;

	if (!Init_09(n, a, b) || !Init_31(n, a, b) || !Init_45(n, a, b))
	{
		printf("---ОШИБКА: Недостаточно памяти---\n");
		Finalize();
		return 0;
	}

	return 1;
}

void Finalize(void)
{
	Finalize_09();
	Finalize_31();
	Finalize_45();
}

int main(int argc, char **argv)
{

	int width = 750;
	int height = 550;

	if(argc != 5)
	{
		printf("---ОШИБКА: %d/4 аргументов---\n", argc-1);
	}

	a = atof(argv[1]);
	b = atof(argv[2]);
	n = atoi(argv[3]);
	k = atoi(argv[4]);
	k = k + 6;
	k = k % 7;
    
	xlibInitPosition(0, 0);
	xlibInitWindowSize(width, height);
    
    DisplayHelp();
    
	xlibRenderFunc(render);
	xlibKeyboardFunc(keyboard);

	if (!Init()) return -1;

	xlibMainLoop("Interpolation");

	Finalize();

	return 0;
}
