#ifndef __METOD_31_H__
#define __METOD_31_H__

void Coeff_31(int n, double *x, double *f_x, double *c, double *d);

double Value_31(double t, int n, double *c, double *x);

int Init_31(int n, double a, double b);

void Finalize_31(void);

void Input_31(void);

void Delta_31(int number, double delta);

void Calc_31(void);

double f_31(double x);

#endif /* __METOD_31_H__ */
