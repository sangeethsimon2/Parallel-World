#include "mpi.h"
#include<iostream>

using namespace std;

#define Master 0

//void initialize(double *, int );

void initialize(const int &id, double *u, const int &size){

	cout<<"The process: "<<id<<" is initializing its data."<<"\n";

	if(id==0){
		for(int i=0;i<=size;i++){
			u[i]=5.0;
		}
	}

	else if(id==1){
		for(int i=0;i<=size;i++){
			u[i]=10.0;
		}
	
	}

	
	else if(id==2){
		for(int i=0;i<=size;i++){
			u[i]=15.0;
		}
	
	}

	
	else{
		for(int i=0;i<=size;i++){
			u[i]=20.0;
		}
	
	}
}

void boundary_condition(const int &id, double *u, const int &size, const int &N_Procs){
	cout<<"The process: "<<id<<" is setting its boundary condition"<<"\n";

	// If left most process, then set the left boundary solution as u=0
	if(id==0){
	
		u[0] = 0.0;
	}

	
	// If right most process, then set the right boundary solution as u=100
	if(id==(N_Procs-1)){
		u[size+1] = 100.0; 
	}

}

int main(){

	// Problem variables

	//Thermal diffusivity
	
	const double K = 0.002;


	//Variable for MPI tasks
	int taskid, numtasks;
	double wtime_start,wtime_elapsed;

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

	//Variable for keeping time
	double time;
	int iter;

	// End time of the computation 
	const double t_end = 1.0;

	// CFL of the computation
	const double CFL = 0.2;	

	// Time steps for the computation
	double del_t = CFL*del_x*del_x/K;
	
	// Initialize the MPI enviornment
		

	MPI_Init(NULL,NULL);

	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	
	MPI_Comm_rank(MPI_COMM_WORLD,&taskid);

	cout<<"Process "<<taskid<<" has started \n";

	// Declare a variable to store the MPI_Send and Recv statuses
	MPI_Status status;	

	// Record the starting time
	if ( taskid == 0 ) {
	    wtime_start = MPI_Wtime();
	  }

	


	
	//Declare variables that each process initializes

	//Declare variable to store the temperature values. Size as per predecieded load of each processor. Two extra memory bits provided to store neighbour information
	double u [N_Load+2],u_new[N_Load+2];
	



	// Let all process initialize their data
	initialize(taskid,u,N_Load+1);


	// Let the left most and right most processes set their Boundary condition
	boundary_condition(taskid,u,N_Load,N_Procs);

	if(taskid==3){
		
		cout<<"Initialized value for process "<<taskid<< " is" <<"\n";
		for(int i=0;i<=N_Load+1;i++){
			cout<<u[i]<<"\n";
		}
		
	}
	
	/*
	if(taskid==3){
		
		cout<<"Initialized value is:"<<u[26]<<"\n";
	}*/	


	// Initialize time
	time = 0.0;

	iter = 0;

	// All processes Loop till end time is reached 
	//while(time<t_end){
	while(iter<100){

		// Allow processes to exchange information at their boundaries
	

		// [] @ @ ...@ []  [] @ @ ... @ [] [] @ @ ... @ [] [] @ @ ... @ [] 

		// |Processor 1	|  |Processor 2  | | Processor 3 | |Processor 4 | 

		// [] - boundary buckets for collecting neighbour information
		
		// @ - internal node value

		// tag 1 = Information tag from taskid > 0 to previous tasks
		
		// tag 2 = Information tag from taskid < N_Procs - 1 to forward tasks



		// Configure all processes with taskid > 0 to MPI_Send their u[1] data to the previous task

		if(taskid > 0){
			MPI_Send(&u[1], 1, MPI_DOUBLE, taskid-1, 1, MPI_COMM_WORLD );
		}

		// Configure all processes except the last one to MPI_Recieve the u[1] data being send by the processes ahead

		if(taskid < N_Procs-1){
			MPI_Recv(&u[N_Load+1],1, MPI_DOUBLE, taskid+1, 1, MPI_COMM_WORLD,&status );
		
		} 


		// Configure all processes except the last one to MPI_Send u[N_Load] data to the process ahead
		if(taskid < N_Procs-1){

			MPI_Send(&u[N_Load],1,MPI_DOUBLE,taskid+1,2,MPI_COMM_WORLD );
		}	

		// Configure all processes with taskid > 0 to MPI_Recv u[N_Load] data from the previous processes
		if( taskid > 0){

			MPI_Recv(&u[0],1,MPI_DOUBLE,taskid-1,2,MPI_COMM_WORLD,&status);
		}


		// Each process now updates its internal data points based on a standard finite difference discrete equation
		// Time discretization: Forward Euler
		// Space discretization: Second order central difference

		for (int i=1; i<=N_Load;i++){
			u_new[i] = u[i] + (K*del_t/(del_x*del_x))* (u[i+1]-2.0*u[i]+u[i-1]);
		} 


		// Copy the updated values of each process from the u_new variable to its old working variable u
		for (int i=1; i<=N_Load;i++){
			u[i] = u_new[i];
		}
		
		
		//Increment the iteration counter 
		iter ++;
	}
	
	//##################Test code########################
	
	if(taskid==3){
		
		cout<<"Computed value for process "<<taskid<< " is" <<"\n";
		for(int i=0;i<=N_Load+1;i++){
			cout<<u[i]<<"\n";
		}
		
	}
	// ##################################
	
	

	//measure the elapsed wall clock time
	/*
	if ( taskid == 0 ){
    		wtime_elapsed = MPI_Wtime () - wtime_start;
		cout << "\n";       
    		cout << "  Wall clock elapsed seconds = " << wtime_elapsed << "\n";      
  	}*/

	

	// Close the MPI enviornment
	MPI_Finalize();
		

	return(0);
}

