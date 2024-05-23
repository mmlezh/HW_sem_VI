void Initial_F(int n, int k, double *f, double *x, double a, double b);

void PL_construct(int n, double *x, double *f, double *c);
double PL_val(int n, double *x, double *f, double *c, double t);//, double b, double t);

void Ermit_construct(int n, int k, double *x, double *f, double *d);
double Ermit_val(int n, double *x, double *f, double *d, double t);

void Spline_construct(int n, int k, double *x, double *f, double *v, double *xi);
double Spline_val(int n, double *x, double *f, double *v, double *xi, double t);

double sq(double x);
double Det(double x);
double F(int k, double x);
double dFdx(int k, double x);

void solve(int n, double *a, double *b, double *x);
void Gauss_inv(double *a, double *b, double *x, int n);


