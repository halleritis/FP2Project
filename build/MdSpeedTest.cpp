#include <iostream>

#include <sstream>
#include <string>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <fstream>
#include <math.h>

#include "MDSystem.hpp"

using namespace std;
void readFromFile();

int numOfParticles = 250, dimOfSystem = 3, sizeOfSys = 8;
double tempOfSystem = 0.1;
float particleMass = 1;

int main()
{
	// initialise random seed	
	srand( time(NULL) );		// different seeds
	// srand( 0 );						// same seed 
	// measuring the calculation time

	System MD( numOfParticles, dimOfSystem, tempOfSystem, sizeOfSys, particleMass );


	ofstream file;
	file.open( "Snapshots.txt" );

	struct timeval start, end;
	gettimeofday(&start, NULL);
	
	for ( int j = 0; j < 1000; j++){
	
	file << "Snapshot_" << j << "------------------------------------" 
		<< endl << endl;
	file << "Potential_Energy: " << MD.GetEnergy() << endl;
	file << "Temperature: " << MD.GetKinEnergy() * 2 / (numOfParticles 
		 * dimOfSystem)	<< endl;
	file << "Total_Energy: " << MD.GetKinEnergy() + MD.GetEnergy() << endl;
	file << "Coordinates" << endl;
	file << "X\tY\tZ" << endl;
	for ( int i = 0; i < numOfParticles; i++ ) {
		for ( int k = 0; k < dimOfSystem; k++ ) {
			file << MD.GetCoordinate(i,k) << "\t";
		}
		file << endl;
	}
	file << endl;
	}
	file.close();

	gettimeofday(&end, NULL);
	cout << "Time needed to do 1000 Printings: " 
		<< ( (end.tv_sec  - start.tv_sec )*1000000 + 
				  end.tv_usec - start.tv_usec ) / 1000000. << endl;

	gettimeofday(&start, NULL);

	for ( int i = 0; i < 1000; i++) 
		MD.VeloVerletStepMD( 0.01 );

	gettimeofday(&end, NULL);
	cout << "Time needed to do 1000 VeloVerlet steps: " 
		<< ( (end.tv_sec  - start.tv_sec )*1000000 + 
				  end.tv_usec - start.tv_usec ) / 1000000. << endl;

	gettimeofday(&start, NULL);

	for ( int i = 0; i < 1000; i++)
		MD.AdjustVelos();

	gettimeofday(&end, NULL);
	cout << "Time needed to do 1000 Velo adjustments: " 
		<< ( (end.tv_sec  - start.tv_sec )*1000000 + 
				  end.tv_usec - start.tv_usec ) / 1000000. << endl;

	/*stringstream ss;
	ss << j;   // To Print in seperate Files
	string s = "Snapshot" + ss.str() + ".txt";
	MD.PrintCoordinates( s );*/

	
	return 0;
}

/*--------------------------------------------------------------------
 * Read system settings from input file
 *------------------------------------------------------------------*/	
void readFromFile() {
	ifstream inputFile("SystemSpecs.txt");
	string line;
	getline( inputFile, line ); cout << line << endl;
	getline( inputFile, line ); cout << line << endl;
	getline( inputFile, line ); cout << line << endl;
	if ( inputFile.is_open() ) {
		getline( inputFile, line ); cout << line << " ";
		getline( inputFile, line );
		cout << (numOfParticles = atoi( line.c_str() )) << endl;
		
		getline( inputFile, line ); cout << line << " "; 
		getline( inputFile, line );
		cout << ( dimOfSystem = atoi( line.c_str() ) ) << endl;
		
		inputFile.close();
	}
}

/*--------------------------------------------------------------------
 * Print Coordinates to a *.txt file
 *------------------------------------------------------------------*/
void System::PrintCoordinates( string fileName ) const {
	ofstream file;
	file.open( (string("snapshots/")+fileName).c_str() );
	file << "Potential_Energy: " << GetEnergy() << endl;
	file << "Kinetic_Energy: " << GetKinEnergy() << endl;
	file << "Total_Energy: " << GetKinEnergy() + GetEnergy() << endl;
	file << "Coordinates" << endl;
	file << "X\tY\tZ" << endl;
	for ( int i = 0; i < numberOfParticles; i++ ) {
		for ( int j = 0; j < dimOfSystem; j++ ) {
			file << coords[i*dimOfSystem + j] << "\t";
		}
		file << endl;
	}
	file.close();
}
