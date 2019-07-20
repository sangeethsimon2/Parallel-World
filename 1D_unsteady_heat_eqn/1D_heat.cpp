#include "mpi.h"
#include<iostream>

using namespace std;

#define Master 0

int main()

{

	// Problem variables

	//Thermal diffusivity
	
	const double K = 0.1;


	//Variable for MPI tasks
	int taskid, numtasks;

	// Variables relating to the finite difference equation

	// Length of the domain
	const double L = 1.0;

	// Number of grid points
	const int N_Dof = 100;

	// Number of processes
	int N_Procs = 4;

	// Load for each process
	int N_Load = N_Dof/N_Procs;

	// Obtain grid size 
	double del_x = L/N_Dof;

	// End time of the computation 
	const double t_end = 1.0;

	// CFL of the computation
	const double CFL = 0.5;	

	// Time steps for the computation
	double del_t = CFL*del_x/K;
	
	// Initialize the MPI enviornment
		

	MPI_Init(NULL,NULL);

	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	
	MPI_Comm_rank(MPI_COMM_WORLD,&taskid);

	cout<<"Process "<<taskid<<" has started \n";

	
		

	
	// Close the MPI enviornment
	MPI_Finalize();
		

	return(0);
}

