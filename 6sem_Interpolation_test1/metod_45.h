#ifndef __METOD_45_H__
#define __METOD_45_H__

void Coeff_45(int n, double a, double b, double *x, double *f_x, double *c, double *v, double *ksi, double *a1, double *c1, double *d1);

double Value_45(double t, int n, double *c, double *ksi);

void SolveSystem(int n, double *a, double *c, double *d, double *b, double *x);

int Init_45(int n, double a, double b);

void Finalize_45(void);

void Input_45(void);

void Delta_45(int number, double delta);

void Calc_45(void);

double f_45(double x);

#endif /* __METOD_45_H__ */
