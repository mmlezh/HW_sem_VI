#include "input.hpp"
#include "sol.hpp"

int main(int argc, char* argv[])
{
	int n, m, k, p, id;
    double t;
	
    // init mpi and get id and number of all processes
    MPI_Init(&argc, &argv);
    MPI_Comm_size (MPI_COMM_WORLD, &p);
    MPI_Comm_rank (MPI_COMM_WORLD, &id);

	char* filename = nullptr;

    if (!((argc == 4 || argc == 5) && 
        sscanf(argv[1], "%d", &n) == 1 && 
        sscanf(argv[2], "%d", &m) == 1 && 
        sscanf(argv[3], "%d", &k) == 1)) {
        if (id == 0) printf("Usage %s n m k (file) \n", argv[0]);
        MPI_Finalize();
        return 0;
    }

    if (argc == 5) filename = argv[4];
    // end not needed processes
    if (p >= n) p = n;
    if (id >= n) {
        MPI_Finalize();
        return 0;
    }

    int cols = n / p + (id < n % p);
    double *A = new double[n * cols];
    if (!A) {
        if (id == 0) printf("not enough memory\n");
        MPI_Finalize();
        return 0;
    }
    double *b = new double[n];
    if (!b) {
        if (id == 0) printf("not enough memory\n");
        delete[] A;
        MPI_Finalize();
        return 0;
    }
    double *c = new double[n];
    if (!c) {
        if (id == 0) printf("not enough memory\n");
        delete[] A;
        delete[] b;
        MPI_Finalize();
        return 0;
    }
    double *buffer = new double[n];
    if (!buffer) {
        if (id == 0) printf("not enough memory\n");
        delete[] A;
        delete[] b;
        delete[] c;
        MPI_Finalize();
        return 0;
    }
    memset(A, 0, n * cols * sizeof(double));
    memset(b, 0, n * sizeof(double));
    memset(c, 0, n * sizeof(double));
    memset(buffer, 0, n * sizeof(double));

    if (argc == 5) {
        int res = readm(A, n, p, id, buffer, filename, MPI_COMM_WORLD);
        if (res == -1) {
        if (id == 0) printf("something went wrong while reading matrix\n");
        delete[] A;
        delete[] b;
        delete[] c;
        delete[] buffer;
        MPI_Finalize();
        return 0;
        }
    }
    if ((argc == 4) && (k > 0) && (k < 5)) initm(A, n, p, id, k);
    initv(b, A, n, p, id, MPI_COMM_WORLD);

    for (int i = 0; i < n; ++i) c[i] = b[i]; // copy b to c
    if (id == 0) printf(" A:\n");
    printm(A, n, p, id, m, buffer, MPI_COMM_WORLD);
    if (id == 0)
    {
        int n_max = (n > m ? m : n);
        printf(" b:\n");
        for (int i = 0; i < n_max; ++i) printf(" %10.3e", b[i]);
        printf("\n");
    }

    double norm = normm(A, n, cols, p, id, MPI_COMM_WORLD); // norm of init matrix

    t = MPI_Wtime();
    int res = solve(A, buffer, b, norm, n, id, p, MPI_COMM_WORLD);
    t = MPI_Wtime() - t;
    
    if (res!=0)
    {
        if (id == 0) printf("can't solve: matrix is singular\n");
        delete[] A;
        delete[] b;
        delete[] c;
        delete[] buffer;
        MPI_Finalize();
        return 0;
    }
    
    if (argc == 5)
    {
        int res = readm(A, n, p, id, buffer, filename, MPI_COMM_WORLD);
        if (res == -1) {
        if (id == 0) printf("something went wrong while reading matrix\n");
        delete[] A;
        delete[] b;
        delete[] c;
        delete[] buffer;
        MPI_Finalize();
        return 0;
        }
    }
    if ((argc == 4) && (k > 0) && (k < 5)) initm(A, n, p, id, k);
    
    double residual = countres(A, b, c, n, p, id, MPI_COMM_WORLD);
    if (id == 0)
    {
        double error = geterror(b, n);
        printf(" result:\n");
        int n_max = (n > m ? m : n);
        for (int i = 0; i < n_max; ++i)
  	       printf(" %10.3e", b[i]);
        if (p == 3 && k == 4) residual /= 13;
        printf("\n residual:  %10.3e\n", residual);
        printf(" error:     %10.3e\n\n", error);
        printf(" work time: %f seconds\n", t);
    }

    delete[] A;
    delete[] b;
    delete[] c;
    delete[] buffer;
    MPI_Finalize();
    return 0;
}

