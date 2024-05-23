#ifndef __input__
#define __input__
#include "mpi.h"

void initm(double* a, int n, int p, int id, int k);
int readm(double *a, int n, int p, int id, double *buf, char *filename, MPI_Comm comm);
double f(int k, int n, int i, int j);
void printm(double* a, int n, int p, int id, int max_print, double* buffer, MPI_Comm comm);
void initv(double *b, double *a, int n, int p, int id, MPI_Comm comm);

#endif
