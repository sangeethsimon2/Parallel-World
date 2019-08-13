
#include<iostream>

using namespace std;

void initialize(double *u, const int &size){

	cout<< "Initializing its data."<<"\n";

	for(int i=0;i<=size;i++){

		/*
		if(i>=0&&i<=25){
			u[i]=5.0;
		}
		else if(i>25&&i<=50){
			u[i]=10.0;
		}

		else if(i>50&&i<=75){
			u[i]=15.0;
		}

		else if(i>75&&i<=100){
			u[i]=20.0;
		}
		*/

		if(i>=0&&i<=2){
			u[i]=5.0;
		}
		else if(i>2&&i<=5){
			u[i]=10.0;
		}

		else if(i>5&&i<=7){
			u[i]=15.0;
		}

		else if(i>7&&i<=10){
			u[i]=20.0;
		
		}	

	}
}


void boundary_condition( double *u, const int &size){
	cout<<"Setting boundary condition"<<"\n";

	// Set the left boundary solution as u=0

	
	u[0] = 0.0;

	
	// Set the right boundary solution as u=100
	u[size] = 100.0; 
}



int main(){

	// Problem variables

	//Thermal diffusivity
	
	const double K = 0.002;


	//Variable for measuring time 
	double wtime_start,wtime_elapsed;

	// Variables relating to the finite difference equation

	// Length of the domain
	const double L = 1.0;

	// Number of grid points
	const int N_DOF = 10;


	// Obtain grid size 
	double del_x = L/N_DOF;

	//Variable for keeping time
	double time;
	int iter;

	// End time of the computation 
	const double t_end = 1.0;

	// CFL of the computation
	const double CFL = 0.2;	

	// Time steps for the computation
	double del_t = CFL*del_x*del_x/K;


	cout<<del_x<<"\t"<<del_t <<"\n"; 
		  

	//Declare variable to store the temperature values at 'n' and 'n+1' time steps.
	double u[N_DOF+2],u_new[N_DOF+2];
	

	
	// initialize the data
	initialize(u,N_DOF+1);


	// Let the left most and right most processes set their Boundary condition
	boundary_condition(u,N_DOF+1);

	cout<<"Initialized value is" <<"\n";
	for(int i=0;i<=N_DOF+1;i++){
			cout<<u[i]<<"\n";
		}


	// Initialize time
	time = 0.0;

	iter = 0;

	while(iter<100){


		// Update the internal data points based on a standard finite difference discrete equation
		// Time discretization: Forward Euler
		// Space discretization: Second order central difference

		for (int i=1; i<=N_DOF;i++){
			u_new[i] = u[i] + (K*del_t/(del_x*del_x))* (u[i+1]-2.0*u[i]+u[i-1]);
		} 


		// Copy the updated values of each process from the u_new variable to its old working variable u
		for (int i=1; i<=N_DOF;i++){
			u[i] = u_new[i];
		}
		
		
		//Increment the iteration counter 
		iter ++;
	}
	
	cout<<"Computed value is" <<"\n";
	for(int i=0;i<=N_DOF+1;i++){
			cout<<u[i]<<"\n";
		}

	
	return(0);
}





