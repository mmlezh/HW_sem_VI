#ifndef __METOD_09_H__
#define __METOD_09_H__

void Coeff_09(int n, int N, double *x, double *f_x, double *c);

double Value_09(double t, double a, double b, int n, double *c);

int Init_09(int n, double a, double b);

void Finalize_09(void);

void Input_09(void);

void Delta_09(int number, double delta);

void Calc_09(void);

double f_09(double x);

#endif /* __METOD_09_H__ */
