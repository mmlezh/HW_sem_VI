#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "methods.h"

/*double sq(double x)
{
	return x*x;
}

double Det(double t)
{
	double lambda1, lambda3;
	lambda1 = sqrt( (sqrt(5.0) - 1.0)/2.0 );
	lambda3 = sqrt( (sqrt(5.0) + 1.0)/2.0 );
	return 
		lambda1 * lambda3 * sq(cos(lambda1 * t) ) - 2 * lambda1 * lambda3 * cos(lambda1 * t) * cosh(lambda3 * t) + lambda1 * lambda3 * sq( cosh(lambda3 * t) )
		+ lambda1 * lambda3 * sq( sin(lambda1 * t) ) + sin(lambda1 * t) * sinh(lambda3 * t) - lambda1 * lambda3 * sq( sinh(lambda3 * t) );
}*/

double F(int k, double x)
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

void Initial_F(int n, int k, double *f, double *x, double a, double b)
{
	for(int i = 0; i < n; ++i)
	{
		x[i] = a + (b-a) / (n-1) * i;
		f[i] = F(k, x[i]);
	}
	return ;
}
void PL_construct(int n, double *x, double *f, double *c)
{
	for(int i = 0; i < n-1; ++i)
		c[i] = (f[i+1] - f[i]) / (x[i+1] - x[i]);
	return ;
}

double PL_val(int n, double *x, double *f, double *c, double t)//, double a, double b)
{
	for(int i = 0; i < n - 1; ++i)
		if(t <= x[i+1])
			return f[i] + (t - x[i]) * c[i];
	return f[n-2] + (t - x[n-2]) * c[n-2];
}

double dFdx(int k, double x)
{
	//double h = 0.1;
	return (F(k, x + 1e-7) - F(k, x)) / 1e-7;
}

double d2Fdx2(int k, double x)
{
	return (dFdx(k, x + 1e-13) - dFdx(k, x)) / 1e-13;
}

void Ermit_construct(int n, int k, double *x, double *f, double *d)
{
	for(int i = 0; i < n; ++i)
	{
		f[i] += 0;
		d[i] = dFdx(k, x[i]);
	}
	return ;
}

double Ermit_val(int n, double *x, double *f, double *d, double t)
{
	double ans = 0, dx, f_xi_xi1;
	for(int i = 0; i < n-1; ++i)
	{
		if(t <= x[i+1])
		{
			f_xi_xi1 = (f[i+1] - f[i]) / (x[i+1] - x[i]);
			dx = t - x[i];
			ans = f[i] + d[i] * dx +
			       	(3.0 * f_xi_xi1 - 2.0*d[i] - d[i+1]) / (x[i+1]-x[i]) * dx * dx 
				+ (d[i] + d[i+1] - 2.0 * f_xi_xi1) / ((x[i+1] - x[i]) * (x[i+1] - x[i]))
			       	* dx * dx * dx; 
			return ans;
		}
	}
	f_xi_xi1 = (f[n-1] - f[n-2]) / (x[n-1] - x[n-2]);
	dx = t - x[n-2];
	ans = f[n-2] + d[n-2] * dx +
	      	(3.0 * f_xi_xi1 - 2.0*d[n-2] - d[n-1]) / (x[n-1]-x[n-2]) * dx * dx 
		+ (d[n-2] + d[n-1] - 2.0 * f_xi_xi1) / ((x[n-1] - x[n-2]) * (x[n-1] - x[n-2]))
	       	* dx * dx * dx; 
	return ans;
}

void Spline_construct(int n, int k, double *x, double *f, double *v, double *xi)
{
	double d2f;
	double *matrix, *vec;
	xi[0] = 3.0*x[0]/2.0 - x[1]/2.0;
	for(int i = 1; i < n; ++i)
	{
		xi[i] = (x[i-1] + x[i]) / 2.0;
	}
	xi[n] = 3.0 * x[n-1] / 2.0 - x[n-2]/2.0;
	
	
	matrix = (double*)malloc((n+1) * (n+1) * sizeof(double));
	vec = (double*)malloc((n+1) * sizeof(double));
	
	for(int i = 0; i < n+1; ++i)
	{
		for(int j = 0; j < n+1; ++j)
			matrix[i*(n+1) + j] = 0;
		vec[i] = 0;
	}
	
	//Сверить составление матрицы с книжкой Богачёва
	for(int i = 1; i < n; ++i)
	{
		matrix[i * (n+1) + i-1] = 1.0 / (x[i-1] - xi[i-1]) - 1.0/ (xi[i] - xi[i-1]);
		matrix[i * (n+1) + i] = 1.0 / (xi[i] - x[i-1]) + 1.0 / (xi[i] - xi[i-1]) + 1.0 / (x[i] - xi[i]) + 1.0 / (xi[i+1] - xi[i]);
		matrix[i * (n+1) + i + 1] = 1.0 / (xi[i+1] - x[i]) - 1.0 / (xi[i+1] - xi[i]);
		vec[i] = (1.0 / (x[i-1] - xi[i-1]) + 1.0 / (xi[i] - x[i-1]) ) * f[i-1] + ( 1.0 / (x[i] - xi[i]) + 1.0 / (xi[i+1] - x[i]) ) * f[i];
	}
	
	
	d2f = d2Fdx2(k, x[0]);
	matrix[0 * (n+1) + 0] = 2.0 / ( (xi[0+1] - xi[0]) * (x[0] - xi[0]) );
	matrix[0 * (n+1) + 1] = 2.0 / ( (xi[0+1] - xi[0]) * (xi[0+1] - x[0]) );
	vec[0] = d2f + f[0] * 2.0 / (xi[0+1] - xi[0]) * (1.0 / (x[0] - xi[0]) + 1.0 / (xi[0+1] - x[0]));
	
	d2f = d2Fdx2(k, x[0]);
	matrix[n * (n+1) + n-1] = 2.0 / ( (xi[(n-1)+1] - xi[n-1]) * (x[n-1] - xi[n-1]) );
	matrix[n * (n+1) + n] = 2.0 / ( (xi[(n-1)+1] - xi[n-1]) * (xi[(n-1)+1] - x[n-1]) );
	vec[n] = d2f + f[n-1] * 2.0 / (xi[(n-1)+1] - xi[n-1]) * (1.0 / (x[n-1] - xi[n-1]) + 1.0 / (xi[(n-1)+1] - x[n-1]));
	
	//for(int i = 0; i < n+1; ++i)
	//{
	//	for(int j = 0; j < n+1; ++j)
	//		printf("%10.3e ", matrix[i*(n+1) + j]);
	//	printf("| %10.3e\n", vec[i]);
	//}
	

	solve(n+1, matrix, vec, v);
	
	//for(int i = 0; i < n+1; ++i)
	//	printf("%10.3e\n", v[i]);
	free(matrix);
	free(vec);
	return ;
}
double Spline_val(int n, double *x, double *f, double *v, double *xi, double t)
{
	double ans = 0;
	double c1 = 0, c2 = 0, c3 = 0, dx;
	for(int i = 0; i < n; ++i)
	{
		if(t <= xi[i+1])
		{
			dx = t - xi[i];
			c1 = v[i];
			c2 = (f[i] - v[i]) / (x[i] - xi[i]) - (x[i] - xi[i]) / (xi[i+1] - xi[i]) * ((v[i+1]-f[i])/(xi[i+1]-x[i]) - (f[i]-v[i])/(x[i] - xi[i] ));
			c3 = ( (v[i+1] - f[i]) / (xi[i+1] - x[i]) - (f[i] - v[i]) / (x[i] - xi[i]) ) / (xi[i+1] - xi[i]);
			/*if(fabs(c1 - 1.0) < 1e-14)
				c1 = 1.0;
			if(fabs(c2) < 1e-14)
				c2 = 0;
			if(fabs(c3) < 1e-14)
				c3 = 0;*/
			
			ans = c1 + c2 * dx + c3 * dx * dx;
			return ans;
		}
	}
	dx = t - xi[n-1];
	c1 = v[n-1];
	c2 = (f[n-1] - v[n-1]) / (x[n-1] - xi[n-1]) - (x[n-1] - xi[n-1]) / (xi[n] - xi[n-1]) * ((v[n]-f[n-1])/(xi[n]-x[n-1]) - (f[n-1]-v[n-1])/(x[n-1] - xi[n-1] ));
	c3 = ( (v[n] - f[n-1]) / (xi[n] - x[n-1]) - (f[n-1] - v[n-1]) / (x[n-1] - xi[n-1]) ) / (xi[n] - xi[n-1]);
	ans = c1 + c2 * dx + c3 * dx * dx;
	return ans;
}

void solve(int n, double *a, double *b, double *x)
{
	double d = 0, l = 0, h = 0;
	double sk = 0;

	for(int k = 0; k < n-1; ++k)
	{
		sk = 0;

		for(int j = k+1; j < n; ++j)
			sk += a[j*n + k] * a[j*n + k];

		d = sqrt(sk + a[k*n + k] * a[k*n + k]);

		a[k*n + k] = a[k*n + k] - d;

		l = sqrt(a[k*n + k] * a[k*n + k] + sk);


		for(int j = k; j < n; ++j)
			a[j*n + k] = a[j*n + k] / l;


		//Согласно лемме 10 произведение матрицы отражений на вектор вычисляется как: U(x)*y = y - 2(y, x)*x

		for(int i = k+1; i < n; ++i)
		{
			h = 0;
			for(int j = k; j < n; ++j)
				h += a[j*n + i] * a[j*n + k];

			h = 2 * h;
			for(int j = k; j < n; ++j)
				a[j*n + i] = a[j*n + i] - a[j*n + k] * h;

		}

		h = 0;
		for(int j = k; j < n; ++j)
			h += b[j] * a[j*n + k];

		h = 2 * h;

		for(int i = k; i < n; ++i)
			b[i] = b[i] - a[i*n + k] * h;

		a[k*n + k] = d;

		//for(int i = k + 1; i < n; ++i)
		//	a[i*n + k] = 0;

	}




	Gauss_inv(a, b, x, n);


	return ;
}

void Gauss_inv(double *a, double *b, double *x, int n)
{
	for(int i = n - 1; i >= 0; --i)
	{
		x[i] = b[i];
		if(fabs(a[i*n + i]) < 1e-100 * b[i])
		{
			printf("Matrix is degenerate!\n");
			return ;
		}

		for(int j = i + 1; j < n; ++j)
			x[i] = x[i] - a[i * n + j] * x[j];

		x[i] = x[i] / a[i*n + i];
	}

	return ;
}


