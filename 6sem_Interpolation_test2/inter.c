#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "methods.h"
#include "plot.h"

static int variant = 0, n = 100, k = 0, s = 0, p = 0, delta = 0;
static double *f = NULL, *x = NULL, a = -1, b = 1;
static double *c = NULL;
static double *d = NULL;
static double *v = NULL;
static double *xi = NULL;

static double sc = 1;

double FindMax(int width, int height, double (*f)(double t))
{
	double mx = f(sc * a);
	for(int i = 0; i <= width; ++i)	
	{
		if(mx < f(sc * (b-a)/width * i + sc * a))
			mx = f(sc * (b-a)/width * i + sc * a);
	}
	return mx;
}

double FindMin(int width, int height, double (*f)(double t))
{
	double mn = f(sc * a);
	for(int i = 0; i <= width; ++i)	
	{
		if(mn > f(sc * (b-a)/width * i + sc * a))
			mn = f(sc * (b-a)/width * i + sc * a);
	}
	return mn;
}

double PL(double t)
{
	return PL_val(n, x, f, c, t);
}

double Ermit(double t)
{
	return Ermit_val(n, x, f, d, t);
}

double Spline(double t)
{
	return Spline_val(n, x, f, v, xi, t);
}

double Fk(double t)
{
	return F(k, t);
}

double PL_pog(double t)
{
	return Fk(t) - PL(t);
}

double Ermit_pog(double t)
{
	return Fk(t) - Ermit(t);
}

double Spline_pog(double t)
{
	return Fk(t) - Spline(t);
}

void DrawF(int width, int height, double mx, double mn, double (*g)(double t))
{
	double x_start, y_start, x_end, y_end;
	for(int i = 0; i < width; ++i)
	{
		x_start = i + 0.0;
		x_end = i + 1.0;
		
		y_start = sc * ( (b-a) / width * x_start + a );
		y_end = sc * ( (b-a) / width * x_end + a );
		
		y_start = -height / (mx - mn) * g(y_start) + mx * height / (mx - mn);
		y_end = -height / (mx - mn) * g(y_end) + mx * height / (mx - mn);

		if(y_start == height)
			--y_start;
		if(y_end == height)
			--y_end;
		WDrawLine(x_start, y_start, x_end, y_end);
	}
	
	return ;
}



static void DrawWindowContent(void)
{
	int width = nWWidth;
	int height = nWHeight;
	double x_start, y_start, x_end, y_end;
	
	double mx, mn, maxF;
	char txt[256];
	WSetColor(DARKGRAY);
	WFillRectangle(0, 0, width, height);
	
	
	//Считываем нажатие клавиш
	switch(variant)
	{
		case 0:
			++k;
			k = k % 7;
			break;
		case 1:
			++p;
			p = p % 5;
			break;
		case 2:
			++s;
			sc /= 2;
			//a /= 2;
			//b /= 2;
			break;
		case 3:
			--s;
			sc *= 2;
			//a *= 2;
			//b *= 2;
			break;
		case 4:
			n *= 2;
			free(x);
			free(f);
			free(c);
			free(d);
			free(v);
			free(xi);
			x = (double*)malloc(n * sizeof(double));
			f = (double*)malloc(n * sizeof(double));
			c = (double*)malloc(n * sizeof(double));
			d = (double*)malloc(n * sizeof(double));
			v = (double*)malloc((n+1) * sizeof(double));
			xi = (double*)malloc((n+1) * sizeof(double));
			break;
		case 5:
			n /= 2;
			free(x);
			free(f);
			free(c);
			free(d);
			x = (double*)malloc(n * sizeof(double));
			f = (double*)malloc(n * sizeof(double));
			c = (double*)malloc(n * sizeof(double));
			d = (double*)malloc(n * sizeof(double));
			v = (double*)malloc((n+1) * sizeof(double));
			xi = (double*)malloc((n+1) * sizeof(double));
			break;
		case 6:
			++delta;
			break;
		case 7:
			--delta;
			break;
	}
	
	WSetColor(BLACK);
	if(k == 0)
		WDrawString("k = 0, f(x) = 1", 10, 20);
	if(k == 1)
		WDrawString("k = 1, f(x) = x", 10, 20);
	if(k == 2)
		WDrawString("k = 2, f(x) = x^2", 10, 20);
	if(k == 3)
		WDrawString("k = 3, f(x) = x^3", 10, 20);
	if(k == 4)
		WDrawString("k = 4, f(x) = 4", 10, 20);
	if(k == 5)
		WDrawString("k = 5, f(x) = e^x", 10, 20);
	if(k == 6)
		WDrawString("k = 6, f(x) = 1/(25x^2 + 1)", 10, 20);
	
	Initial_F(n, k, f, x, a, b);
	maxF = F(k, a);
	for(int i = 0; i <= width; ++i)
		if(maxF < F(k, sc * (b-a)/width * i + a*sc))
			maxF = F(k, sc * (b-a)/width * i + a*sc);
	
	f[n/2] += 0.1 * maxF * delta;
	
	//Построение приближённой функции
	PL_construct(n, x, f, c);
	if(n <= 50)
	{
		Ermit_construct(n, k, x, f, d);
		Spline_construct(n, k, x, f, v, xi);
	}
	
	
	
	//Вычисление минимума и максимума отображаемых функций
	mx = FindMax(width, height, Fk);
	if((p == 0) || (p == 3))
		if(mx < FindMax(width, height, PL))
			mx = FindMax(width, height, PL);
	
	if(n <= 50 && ((p == 1) || (p == 3)) )
		if(mx < FindMax(width, height, Ermit))
			mx = FindMax(width, height, Ermit);

	if(n <= 50 && ((p == 2) || (p == 3)) )
		if(mx < FindMax(width, height, Spline))
			mx = FindMax(width, height, Spline);
	
	if(p == 4)
	{
		if(mx < FindMax(width, height, PL_pog))
			mx = FindMax(width, height, PL_pog);
		if(n <= 50)
		{
			if(mx < FindMax(width, height, Ermit_pog))
				mx = FindMax(width, height, Ermit_pog);
			if(mx < FindMax(width, height, Spline_pog))
				mx = FindMax(width, height, Spline_pog);
		}
	}
	
	mn = FindMin(width, height, Fk);
	if((p == 0) || (p == 3))
		if(mn > FindMin(width, height, PL))
			mn = FindMin(width, height, PL);
	
	if(n <= 50 && ((p == 1) || (p == 3)) )
		if(mn > FindMin(width, height, Ermit))
			mn = FindMin(width, height, Ermit);

	if(n <= 50 && ((p == 2) || (p == 3)) )
		if(mn > FindMin(width, height, Spline))
			mn = FindMin(width, height, Spline);
	
	if(p == 4)
	{
		if(mn > FindMin(width, height, PL_pog))
			mn = FindMin(width, height, PL_pog);
		if(n <= 50)
		{
			if(mn > FindMin(width, height, Ermit_pog))
				mn = FindMin(width, height, Ermit_pog);
			if(mn > FindMin(width, height, Spline_pog))
				mn = FindMin(width, height, Spline_pog);
		}
	}

	
	if(fabs(mn - mx) < 2*1e-13)
		mn = -mx;
	
	
	
	//Рисуем оси
	WSetColor(BLACK);
	WDrawLine(0, height*mx/(mx - mn), width, height*mx/(mx - mn));
	WDrawLine(-a * width / (b-a), 0, -a * width / (b-a), height);
	//WDrawString(">", width-5, height*mx / (mx-mn) + 5);
	//WSetColor(RED);
	//WDrawString("X", width-8, height*mx / (mx - mn) + 18);
	
	//WSetColor(BLACK);
	//WDrawString("/\\", -a*width / (b-a)-5, 10);
	//WSetColor(YELLOW);
	//WDrawString("Y", -a*width / (b-a) + 12, 12);
	
	//Выводим информацию из требований
	WSetColor(BLACK);
	sprintf(txt, "max|F| = %10.15e", fabs(mx) > fabs(mn) ? fabs(mx) : fabs(mn));
	WDrawString(txt, 10, 40);
	
	sprintf(txt, "Scale increased in 2^%d times", s);
	WDrawString(txt, 10, 60);
	
	sprintf(txt, "n = %d", n);
	WDrawString(txt, 10, 80);

	sprintf(txt, "p = %d", delta);
	WDrawString(txt, 10, 100);
	
	//Рисуем функции
	
	
	WSetColor(BLUE);
	DrawF(width, height, mx, mn, Fk);

	WSetColor(GREEN);
	if( (p == 0) || (p == 3))
		DrawF(width, height, mx, mn, PL);

	WSetColor(RED);
	if(n <= 50 && ((p == 1) || (p == 3)) )
		DrawF(width, height, mx, mn, Ermit);

	WSetColor(MAGENTA);
	if(n <= 50 && ((p == 2) || (p == 3)) )
		DrawF(width, height, mx, mn, Spline);
	
	if(p == 4)
	{
		WSetColor(GREEN);
		DrawF(width, height, mx, mn, PL_pog);
		if(n <= 50)
		{
			WSetColor(RED);
			DrawF(width, height, mx, mn, Ermit_pog);
			WSetColor(MAGENTA);
			DrawF(width, height, mx, mn, Spline_pog);
		}
	}
}

static int KeyPressFunction(int nKeySym)
{
	switch(nKeySym)
	{
		case XK_Q:
		case XK_q:
			return KEY_PRESS_QUIT;
		case XK_F1:
		case XK_1:
			variant = 1;
			WSetTitle("Function 1");
			break;
		case XK_F2:
		case XK_2:
			variant = 2;
			WSetTitle("Function 2");
			break;
		case XK_F3:
		case XK_3:
			variant = 3;
			WSetTitle("Function 3");
			break;
		case XK_F4:
		case XK_4:
			variant = 4;
			WSetTitle("Function 4");
			break;

		case XK_F5:
		case XK_5:
			variant = 5;
			WSetTitle("Function 5");
			break;

		case XK_F6:
		case XK_6:
			variant = 6;
			WSetTitle("Function 6");
			break;
		case XK_F7:
		case XK_7:
			variant = 7;
			break;
		case XK_0:
			variant = 0;
			WSetTitle("Default 0");
			break;
		default:
			return KEY_PRESS_EXPOSE;

	}
	return KEY_PRESS_EXPOSE;
}



int main(int argc, char **argv)
{
	int ret_code;
	
	
	if(argc != 5)
	{
		printf("Должно быть 4 аргумента, а у вас их %d!\n", argc-1);
		return 0;
	}
	a = atof(argv[1]);
	b = atof(argv[2]);
	n = atoi(argv[3]);
	k = atoi(argv[4]);
	k = k + 6;
	k = k%7;
	
	f = (double*)malloc(n * sizeof(double));
	x = (double*)malloc(n * sizeof(double));
	c = (double*)malloc(n * sizeof(double));
	d = (double*)malloc(n * sizeof(double));
	v = (double*)malloc((n+1) * sizeof(double));
	xi = (double*)malloc((n+1) * sizeof(double));
	
	Initial_F(n, k, f, x, a, b);
	
	/*FILE *fin;
	fin = fopen("data.txt", "r");
	for(int i = 0; i < n; ++i)
	{
		fscanf(fin, "%lf", &f[i]);
	}*/

	ret_code = DrawWindow(DrawWindowContent, KeyPressFunction);
	
	//double s, t;
	//Spline_construct(n, k, f, x, v, xi);
	//printf("Введите число:\n");
	//scanf("%lf", &s);
	//t = Spline_val(n, x, f, v, xi, s);
	//printf("%10.3e\n", t);
	//t = F(k, s);
	//printf("%10.3e\n", t);

	printf("%d\n", ret_code);
	
	free(f);
	free(x);
	free(c);
	free(d);
	free(v);
	free(xi);
	return 0;
}
