#ifndef __sol__
#define __sol__
#include "mpi.h"
#include <cstring>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>

int solve(double *a, double *a_buf, double *b, double norm, int n, int id, int p, MPI_Comm comm);
double countres(double *a, double *b, double *c, int n, int p, int id, MPI_Comm comm);
double normv(double *b, int n);
double normm(double *a, int n, int cols, int p, int id, MPI_Comm comm);
double geterror(double* b, int n);
double get_full_time();
double get_cpu_time();

#endif
