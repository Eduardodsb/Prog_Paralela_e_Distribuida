/*Autor: Gabriel villares*/
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv) {
    double *x, *y;
    double mySUMx, mySUMy, mySUMxy, mySUMxx, SUMx, SUMy, SUMxy,
           SUMxx, SUMres, res, slope, y_intercept, y_estimate;
    int i,j,n,myid,numprocs,naverage,nremain,mypoints,ishift;
    void *buffer;
    int size, s1, s2;
    double t_inicial, t_final;

    MPI_Status istatus;
    FILE *infile;

    infile = fopen("xydata", "r");
    if (infile == NULL) printf("error opening file\n");

    MPI_Init(&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &myid);
    MPI_Comm_size (MPI_COMM_WORLD, &numprocs);

    /* ----------------------------------------------------------
     * Step 1: Process 0 reads data and sends the value of n
     * ---------------------------------------------------------- */
    if (myid == 0) {
        printf ("Number of processes used: %d\n", numprocs);
        printf ("-------------------------------------\n");
        printf ("The x coordinates on worker processes:\n");

        fscanf (infile, "%d", &n);

        x = (double *) malloc (n*sizeof(double));
        y = (double *) malloc (n*sizeof(double));

        for (i=0; i<n; i++)
            fscanf (infile, "%lf %lf", &x[i], &y[i]);
    }

    t_inicial = MPI_Wtime();

    MPI_Bcast(&n, 1, MPI_INT, /*int root*/0, MPI_COMM_WORLD);

    if(myid != 0) {
        x = (double *) malloc (n*sizeof(double));
        y = (double *) malloc (n*sizeof(double));
    }
    /* ---------------------------------------------------------- */

    naverage = n/numprocs;
    nremain = n % numprocs;

    /* ----------------------------------------------------------
     * Step 2: Process 0 sends subsets of x and y 
     * ---------------------------------------------------------- */
    MPI_Pack_size(2*numprocs, MPI_INT, MPI_COMM_WORLD, &s1);
    MPI_Pack_size(2*numprocs*(naverage + nremain), MPI_DOUBLE, MPI_COMM_WORLD, &s2);

    size = s1 + s2 + 4 * numprocs * MPI_BSEND_OVERHEAD;
    buffer = (void*) malloc(size);

    MPI_Buffer_attach(buffer, size);

    if (myid == 0) {
        for (i=1; i<numprocs; i++) {
            ishift = i*naverage;
            mypoints = (i < numprocs -1) ? naverage : naverage + nremain;

            MPI_Bsend (&ishift, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Bsend (&mypoints, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
            MPI_Bsend (&x[ishift], mypoints, MPI_DOUBLE, i, 3, MPI_COMM_WORLD);
            MPI_Bsend (&y[ishift], mypoints, MPI_DOUBLE, i, 4, MPI_COMM_WORLD);
        }
    }

    else {
        /* ---------------the other processes receive---------------- */
        MPI_Recv (&ishift, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &istatus);
        MPI_Recv (&mypoints, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &istatus);
        MPI_Recv (&x[ishift], mypoints, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD, &istatus);
        MPI_Recv (&y[ishift], mypoints, MPI_DOUBLE, 0, 4, MPI_COMM_WORLD, &istatus);

        /*printf ("id %d: ", myid);

        for (i=0; i<n; i++) printf("%4.2lf ", x[i]);

        printf ("\n");*/
        /* ---------------------------------------------------------- */
    }

    /* ----------------------------------------------------------
     * Step 3: Each process calculates its partial sum
     * ---------------------------------------------------------- */
    mySUMx = 0; mySUMy = 0; mySUMxy = 0; mySUMxx = 0;

    if (myid == 0) {
        ishift = 0;
        mypoints = naverage;
    }

    for (j=0; j<mypoints; j++) {
        mySUMx = mySUMx + x[ishift+j];
        mySUMy = mySUMy + y[ishift+j];
        mySUMxy = mySUMxy + x[ishift+j]*y[ishift+j];
        mySUMxx = mySUMxx + x[ishift+j]*x[ishift+j];
    }

    /* ----------------------------------------------------------
     * Step 4: Process 0 receives partial sums from the others 
     * ---------------------------------------------------------- */

    MPI_Reduce(&mySUMx, &SUMx, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&mySUMy, &SUMy, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&mySUMxy, &SUMxy, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&mySUMxx, &SUMxx, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    t_final = MPI_Wtime();

    /* ----------------------------------------------------------
     * Step 5: Process 0 does the final steps
     * ---------------------------------------------------------- */
    if (myid == 0) {
        slope = ( SUMx*SUMy - n*SUMxy ) / ( SUMx*SUMx - n*SUMxx );
        y_intercept = ( SUMy - slope*SUMx ) / n;

        printf ("\n");
        printf ("The linear equation that best fits the given data:\n");
        printf ("       y = %6.2lfx + %6.2lf\n", slope, y_intercept);
        printf ("--------------------------------------------------\n");
        //printf ("   Original (x,y)     Estimated y     Residual\n");
        //printf ("--------------------------------------------------\n");

        SUMres = 0;

        for (i=0; i<n; i++) {
            y_estimate = slope*x[i] + y_intercept;
            res = y[i] - y_estimate;
            SUMres = SUMres + res*res;

            //printf ("   (%6.2lf %6.2lf)      %6.2lf       %6.2lf\n", x[i], y[i], y_estimate, res);
        }

        printf("--------------------------------------------------\n");
        printf("Residual sum = %6.2lf\n", SUMres);
        printf ("\nTempo de execução %.5f\n", t_final - t_inicial);
    }
    /* ----------------------------------------------------------	*/

    MPI_Finalize();
} 
