#include "mpi.h"
#include <stdio.h>
#define NBIN 1000000000
/* #define NPERP 1000000000 */

int nprocs;  /* Number of processes */
int myid;    /* My rank */

double global_sum(double partial) {
  /* Write your hypercube algorithm here */	
	double hisdone;
	MPI_Status status;
	double mydone = partial;
	for (int bitvalue=1; bitvalue<nprocs; bitvalue*=2)
	{
		int partner = myid ^ bitvalue;
		//send mydone to partner;
		MPI_Send(&mydone, 1, MPI_DOUBLE, partner, bitvalue, MPI_COMM_WORLD);
		//receive hisdone from partner;
		MPI_Recv(&hisdone, 1, MPI_DOUBLE, partner, bitvalue, MPI_COMM_WORLD, &status);
		mydone = mydone + hisdone;
	}
	return mydone;
}

int main(int argc, char *argv[]) {
  //double partial, sum, avg;
  double partial;
  double cpu1, cpu2;
  long long i;
  double step, x, sum=0.0, pi;
  //long long NBIN;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  //partial = (double) myid;
  //printf("Node %d has partial value %le\n", myid, partial);

  cpu1 = MPI_Wtime();
    //NBIN = (long long)NPERP * nprocs;
  	step = 1.0/NBIN;
	//for (i=0; i<NBIN; i++) {
	for (i=myid; i<NBIN; i+=nprocs) {
		x = (i+0.5)*step;
		sum += 4.0/(1.0+x*x);
	}
	//pi = sum*step;
	partial = sum*step;
  //sum = global_sum(partial);
  pi = global_sum(partial);
  cpu2 = MPI_Wtime();

  if (myid == 0) {
    //avg = sum/nprocs;
    //printf("Global average = %le\n", avg);
	printf("Pi = %le\n", pi);
    //printf("Execution time (s) = %le\n",cpu2-cpu1);
	printf("Nprocs & Execution time (s) = %d\t%le\n", nprocs, cpu2-cpu1);
  }

  MPI_Finalize();
  return 0;
}
