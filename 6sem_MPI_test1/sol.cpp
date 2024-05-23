#include "sol.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

static int get_main_block(double *a, int n, int p, int cols, int step, double eps, double &buf)
{
  double max_norm = 1e-100, tmp = 0.;
  int ind, res = -1;
  for (int i = step; i < n; ++i)
  {
    ind = i * cols + step/p;
	  if (fabs(a[ind]) <= eps) continue; // sing_matrix

    tmp = a[ind];
    if (fabs(tmp) > max_norm)
    {
      max_norm = fabs(tmp);
      res = i;
    }
  }

  if (res != -1) buf = 1 / a[res * cols + step / p];
  return res;
}

int solve(double *a, double *a_buf, double *b, double norm, int n, int id, int p, MPI_Comm comm)
{
  double eps = 1e-22 * norm;
  int cols = n / p + (id < n % p);
  double buf = 0.;
	
	for (int step = 0; step < n; ++step)
	{
	    int owner = step % p;
	    int res = 0;
	    
	    if (owner == id) {
          for (int i = 0; i < n; ++i)
            a_buf[i] = a[i * cols + step / p];
            }
	    MPI_Bcast(a_buf, n, MPI_DOUBLE, owner, comm);
	    
	    if (owner == id)
	      res = get_main_block(a, n, p, cols, step, eps, buf);
	    MPI_Bcast(&res, 1, MPI_INT, owner, comm);
	    if (res < 0)
	        return res;
	    MPI_Bcast (&buf, 1, MPI_DOUBLE, owner, comm);
	    
	    for (int w = step / p + (owner == id); w < cols; ++w)
                a[res * cols + w] *= buf;
            b[res] *= buf;
	  
	    for (int i = 0; i < n; ++i)
	    {
	        if (i == res) continue;
                buf = a_buf[i];
	        if (fabs(buf) < eps) continue;
	        for (int w = step / p + (owner == id); w < cols; ++w)
		          a[i * cols + w] -= a[res * cols + w] * buf;
                b[i] -= buf * b[res];
	    }
	    if (res != step) 
            {
                double tmp = 0.;
	        for (int i = step / p + (owner == id); i < cols; ++i)
	        {
               int ind = step*cols + i;
               tmp = a[ind];
               a[ind] = a[res*cols + i];
               a[res*cols + i] = tmp;
	        }

            tmp = b[step];
            b[step] = b[res];
            b[res] = tmp;
            }
	}

  return 0;
}

static void multiply_mv(double *a, double *b, double *res, int n, int p, int id, MPI_Comm comm)
{
  int cols = n/ p + (id < n % p);
  double var = 0;
  for (int i = 0; i < n; i++)
    {
      double s = 0;
      for (int j_loc = 0; j_loc < cols; j_loc++)
        {
          int j_glob = j_loc * p + id;
          if (j_glob < n)
            s += a[i * cols + j_loc] * b[j_glob];
        }
      res[i] = s;
      var = s;
      for (int process = 0; process < p; process++)
        if (process != id)
          MPI_Send (&var, 1, MPI_DOUBLE, process, 0, comm);

      MPI_Status st;
      for (int process = 0; process < p; process++)
        {
          if (process != id)
            {
              MPI_Recv (&var, 1, MPI_DOUBLE, process, 0, comm, &st);
              res[i] += var;
            }
        }     
    }
}

double countres(double *a, double *b, double *c, int n, int p, int id, MPI_Comm comm)
{
  double *rsd = new double[n];
  multiply_mv(a, b, rsd, n, p, id, comm);
  for (int i = 0; i < n; i++)
		rsd[i] -= c[i];

  double norm = normv(c, n);
  if (norm >= 0 && norm <= 0)
    {
      delete [] rsd;
      return -1;
    }
  double res = normv(rsd, n) / norm;
  delete [] rsd;
  return res;
}
	
double normv(double *b, int n)
{
    double res = 0;
    for (int i = 0; i < n; i++)
    {
    	double check = fabs(b[i]);
        if (check > res) res = check;
    }
    return res;
}

double normm(double *a, int n, int cols, int p, int id, MPI_Comm comm)
{
  double res = 0;
  double check = 0;
  for (int j = 0; j < cols; j++) 
  {
    if (j*p + id < n) check += fabs(a[j]);
    else break;
  }

    for (int process = 0; process < p; process++)
      if (process != id)
        MPI_Send (&check, 1, MPI_DOUBLE, process, 0, comm);

    MPI_Status st;
    double var = 0;
    for (int process = 0; process < p; process++)
    {
        if (process != id)
        {
            MPI_Recv(&var, 1, MPI_DOUBLE, process, 0, comm, &st);
            check += var;
        }
    }     
    if (check > res) res = check;

  return res;
}

double geterror(double* b, int n)
{
  double max = 1e-16;
  double* vec = new double[n];

  for (int i = 0; i < n; ++i)
    vec[i] = (i%2 == 0) ? 1 : 0;

  for (int i = 0; i < n; ++i) {
    double tmp = fabs(b[i] - vec[i]);
    if (tmp > max) max = tmp;
  }
  return max;
}
