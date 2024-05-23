#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "metod_45.h"

static int n;
static double a;
static double b;

static double *x = NULL;
static double *f_x = NULL;
static double *c = NULL;
static double *v = NULL;
static double *ksi = NULL;
static double *a1 = NULL;
static double *c1 = NULL;
static double *d1 = NULL;

/*static */void SolveSystem(int n, double *a, double *c, double *d, double *b, double *x)
{
	int i;

	c[0] /= a[0];
	for (i = 1; i < n - 1; i++)
	{
		a[i] -= d[i - 1] * c[i - 1];
		c[i] /= a[i];
	}
	a[n - 1] -= d[n - 2] * c[n - 2];

	x[0] = b[0] / a[0];
	for (i = 1; i < n; i++)
		x[i] = (b[i] - d[i - 1] * x[i - 1]) / a[i];

	for (i = n - 2; i >= 0; i--)
		x[i] -= c[i] * x[i + 1];
}

void Coeff_45(int n, double a, double b, double *x, double *f_x, double *c, double *v, double *ksi, double *a1, double *c1, double *d1)
{
	int i, j;
	double tmp1;

	for (i = 1; i < n; i++)
		ksi[i] = 0.5 * (x[i - 1] + x[i]);

	ksi[0] = a - (ksi[2] - ksi[1]);
	ksi[n] = b + (ksi[n - 1] - ksi[n - 2]);

	for (i = 1; i < n; i++)
	{
		a1[i] = 1.0/(ksi[i] - x[i - 1]) + 1.0/(ksi[i] - ksi[i - 1]) + 1.0/(x[i] - ksi[i]) + 1.0/(ksi[i + 1] - ksi[i]);
		c1[i] = 1.0/(ksi[i + 1] - x[i]) - 1.0/(ksi[i + 1] - ksi[i]);
		d1[i - 1] = 1.0/(x[i - 1] - ksi[i - 1]) - 1.0/(ksi[i] - ksi[i - 1]);
		c[i] = f_x[i - 1] * (1.0/(x[i - 1] - ksi[i - 1]) + 1.0/(ksi[i] - x[i - 1])) + 
			f_x[i] * (1.0/(x[i] - ksi[i]) + 1.0/(ksi[i + 1] - x[i]));
	}

	a1[0] = 1.0;
	c1[0] = 0.0;
	a1[n] = 1.0;
	d1[n - 1] = 0.0;

	c[0] = f(ksi[0]);
	c[n] = f(ksi[n]);

	SolveSystem(n + 1, a1, c1, d1, c, v);

	j = 0;

	for (i = 0; i < n; i ++)
	{
		c[j + 0] = v[i];
		tmp1 = ((v[i + 1] - f_x[i]) / (ksi[i + 1] - x[i]) - (f_x[i] - v[i]) / (x[i] - ksi[i])) / (ksi[i + 1] - ksi[i]);
		c[j + 1] = (f_x[i] - v[i]) / (x[i] - ksi[i]) - (x[i] - ksi[i]) * tmp1;
		c[j + 2] = tmp1;
		j += 3;
	}
}

double Value_45(double t, int n, double *c, double *ksi)
{
	int i;

	for (i = 0; i < n - 1; i++) if (t <= ksi[i + 1]) break;

	return c[3 * i] + c[3 * i + 1] * (t - ksi[i]) + c[3 * i + 2] * (t - ksi[i]) * (t - ksi[i]);
}


int Init_45(int n_, double a_, double b_)
{
	n = n_;
	a = a_;
	b = b_;

	x = (double*)malloc(n * sizeof(double));
	f_x = (double*)malloc(n * sizeof(double));
	c = (double*)malloc(3 * n * sizeof(double));
	v = (double*)malloc((n + 1) * sizeof(double));
	ksi = (double*)malloc((n + 1) * sizeof(double));
	a1 = (double*)malloc((n + 1) * sizeof(double));
	c1 = (double*)malloc(n * sizeof(double));
	d1 = (double*)malloc(n * sizeof(double));

	if (!(x && f_x && c && v && ksi && a1 && c1 && d1)) return 0;

	return 1;
}

void Finalize_45(void)
{
	if (x) free(x); 
	if (f_x) free(f_x);
	if (c) free(c); 
	if (v) free(v); 
	if (ksi) free(ksi); 
	if (a1) free(a1); 
	if (c1) free(c1);
	if (d1) free(d1); 
    x = NULL;
    f_x = NULL;
    c = NULL;
    v = NULL;
    ksi = NULL;
    a1 = NULL;
    c1 = NULL;
    d1 = NULL;
}

void Input_45(void)
{
	int i;
	double h;

	h = (b - a)/(n - 1);
	for (i = 0; i < n; i++)
	{
		x[i] = a + i * h;
		f_x[i] = f(x[i]);
	}
}

void Delta_45(int number, double delta)
{
	f_x[number] += delta;
}

void Calc_45(void)
{
	Coeff_45(n, a, b, x, f_x, c, v, ksi, a1, c1, d1);
}

double f_45(double t)
{
	return Value_45(t, n, c, ksi);
}
