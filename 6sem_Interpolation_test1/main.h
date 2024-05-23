#ifndef __MAIN_H__
#define __MAIN_H__

/* Function */
double F(int k, double x);

double f(double x);

/* First derivative */
double df(double x);

/* Second derivative */
double ddf(double x);

int Init(void);
void Finalize(void);
void render(int width, int height);
void keyboard(int key);
void DrawFunction(int width, int height, double (*func)(double x));
void CalcMaxAndMin(int width, double (*func)(double x), double* maxim, double* minim);
void DisplayHelp(void);
void DisplayDiag(void);

#endif /* __MAIN_H__ */
