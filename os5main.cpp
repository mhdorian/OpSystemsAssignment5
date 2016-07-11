/* 
Operating Systems: Project 5 "Faults In My Memory"
Programer: Meghan H Dorian
I, Meghan Dorian, have abided by the Wheaton Honor
Code in this work. 
--------------------------------------------------------
PURPOSE: To simulate a Working-Set Memory management scheme 
		 and to analyse its performance data using a given reference 
		 string (called omega) which represents the sequence of pages
		 in memory a program accesses.  

INPUT: (from console) integer: The maximum delta, or working-set size, the 
		program should generate data for. 
		ie) if MaxDelta = 5, the simulation computes data 
		for working-set sizes in range [1-5].    
OUTPUT: (0)(to console) Formated Table of statistics for each working-set
 		    size including:
 		    (a) The current max working-set size, "Delta"
 		    (b) The number of page faults occured, "P(Delta)"
 		    (c) The average working-set size, "W(Delta)" 
 		    (d) The average page fault rate, "F(Delta)"

 		(1) One comma-separated-value file entitled "Output_os5.csv"
 			for graphing the data. The File contains data members:
 			(a-c) from above, and 
 			(d) 1/the average page fault rate, or the length of the 
 				reference string over the number of page faults.
----------------------------------------------------
NOTES: Average working-set size is still off in the thousandths place.
	   Margin of error increases as delta increases. I suspect that this 
	   has something to do with the way I am moving my window. 
----------------------------------------------------
*/ 
//---Libraries--------------------------------
#include <iostream>
#include <string.h>
#include "MemSim.h"

using namespace std;
//--- Function Prototype -----------------------------
void setupstring (char [], int&); //(from M.Gousie) generates reference string 
void tableHeader(); //prints table header 
//----------------------------------------------------
//--- Constants --------------------------------------
const int MAX = 2000;
const int N   = 10;
//====================================================
int main ()
{
	//generating reference string
	char omega [MAX];
	int omegalength;

	setupstring (omega, omegalength);

	//fetch max delta 
	int DeltaMax; 
	cout << "Enter Max Delta." << endl; 
	cin >> DeltaMax;

	//print table header
	tableHeader(); 

	//new instance of MemSim
	MemSim thisSim(DeltaMax, omega[0], omegalength);
	//Start simulation
	thisSim.simulationGO(); 

	cout << "\nDONE!" << endl; 
	
	return 0;
}
//===============================================

/*-----------/
setupstring()//-------------------------------------------
--------------------------------------------------------
Generates the reference string, which represents the order 
of page numbers a program will access.  
INPUT: char omega[] (inout) - empty character array
	   int omegalen (inout) - uninitialized variable for 
	   array length (passed by reference)

OUTPUT: Full character array omega[], length of array
 		(returned by reference)
--------------------------------------------------*/
void setupstring (char omega[], int& omegalen)
{
   int i, j;

   strcpy (omega, "272722");
   for (i = 0; i < N; i++)
      strcat (omega, "28272272927222");

   strcat (omega, "272722");

   for (i = 0; i < N; i++) {
      strcat (omega, "272733733");
 
      for (j = i; j < N; j++)
         strcat (omega, "373338393373737333");
      strcat (omega, "3637322");
   }

   omegalen = strlen (omega);
}
//------------------------------------------

/*-----------/
tableHeader()//-------------------------------------------
--------------------------------------------------------
Prints header of data table to console
--------------------------------------------------*/
void tableHeader()
{
	cout << left << setw(WIDTH) << "Delta"\
	<< left << setw(WIDTH) << "P(Delta)" \
	<< left << setw(WIDTH) << "W(Delta)" \
	<< left << setw(WIDTH) << "F(Delta)" << endl;

	cout << left << setw(WIDTH*4) << "------------------------------------------" << endl; 
}
//-----------------------------------------------