#include "input.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

void initm(double* a, int n, int p, int id, int k)
{
   int cols = (n%p <= id ? n/p : n/p + 1);
   for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < cols; j++)
        {
          int global_j = j * p + id;
          a[i * cols + j] = f (k, n, i, global_j);
        }
    }
}

static int readarray(FILE * fp, double *a, int len)
{
  for (int i = 0; i < len; i++)
  {
    if (fscanf (fp, "%lf", a + i) != 1)
      return -1;
  }
  return 0;
}


int readm(double *a, int n, int p, int id, double *buf, char *filename, MPI_Comm comm)
{
  int main_id = 0, err = 0;
  FILE *in = nullptr;
  
  if (id == main_id)
    {
      in = fopen(filename, "r");
      if (in == nullptr) err = -1;
    }
    
  MPI_Bcast (&err, 1, MPI_INT, main_id, comm);
  if (err) return err;

  double *var = new double[1];
  int cols = n / p + (id < n % p);
  for (int b = 0; b < n; b++)
  {
      if (id == main_id)
        {
          err += readarray(in, buf, n);
          for (int j = 0; j < n; j++)
            {
              int owner = j % p;
              int j_loc = j / p;

              var[0] = buf[j];
              if (owner == id)
                  a[b*n + j_loc] = var[0];
              else
                {
                  MPI_Send (var, 1, MPI_DOUBLE, owner, 0, comm);
                }
            }
        }
      else
        {
          for (int j = 0; j < n; j++)
            {
              int owner = j % p;
              int j_loc = j / p;
              if (owner == id)
              {
                MPI_Status st;
                MPI_Recv (var, 1, MPI_DOUBLE, main_id, 0, comm, &st);
                a[b*cols + j_loc] = var[0];
              }
            }
        }
  }

  if (id == main_id)
  {
    fclose (in);
    in = nullptr;
  }
  MPI_Bcast (&err, 1, MPI_INT, main_id, comm);
  delete[] var;
  if (err != 0) return err;
  return 0;
}

double f(int k, int n, int i, int j)
{
  double max = i + 1;
  if(i < j) max = j + 1;

  ++i;
  ++j;

  switch(k)
  {
    case 1:
        return n - max + 1;
    case 2:
        return max;
    case 3:
        return abs(i - j);
    case 4:
        return 1. / (i + j - 1.0);
    default:
        return 0;
  }
}

void printm(double* a, int n, int p, int id, int max_print, double* buf, MPI_Comm comm)
{
  int main_id = 0, p_n = (max_print < n ? max_print : n);
  int cols = n / p + (id < n % p);
  
  for (int i = 0; i < p_n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      int owner = j % p;
      int j_loc = j / p;
      if (id == main_id)
      {
        if (owner == main_id)
        {
          if (j < max_print) printf(" %10.3e", (a + i * cols + j_loc)[0]);
        }
        else
        {
          MPI_Status st;
          MPI_Recv (buf, 1, MPI_DOUBLE, owner, 0, comm, &st);
          if (j < max_print) printf (" %10.3e", buf[0]);
        }
      }
      else
      {
        if (owner == id)
          MPI_Send (a + i * cols + j_loc, 1, MPI_DOUBLE, main_id, 0, comm);
      }
    }
    if (id == main_id) printf ("\n");
  }
}

void initv(double *b, double *a, int n, int p, int id, MPI_Comm comm)
{
  int cols = n / p + (id < n % p);
  double *var = new double[1];
  
	for (int i = 0; i < n; i++)
	{
		double s = 0;
		for (int j_loc = 0; j_loc < cols; j_loc++)
		{
		    int j_glob = j_loc*p + id;
		    if (j_glob % 2 == 0 && j_glob < n)
		       s += a[i * cols + j_loc];
		}
        b[i] = s;
        var[0] = s;
        for (int process = 0; process < p; process++)
        if (process != id)
          MPI_Send (var, 1, MPI_DOUBLE, process, 0, comm);
        MPI_Status st;
        for (int process = 0; process < p; process++)
        {
          if (process != id)
            {
              MPI_Recv (var, 1, MPI_DOUBLE, process, 0, comm, &st);
              b[i] += var[0];
            }
        }
	}
  
  delete[] var;
}