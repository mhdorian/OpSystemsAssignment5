
//=== MemSim.cpp ============

#include "MemSim.h"
#include <iostream>

using namespace std; 
//--------------------------------------
/*---------/
   CTOR   //-------------------------------------------
--------------------------------------------------------
Called 
INPUT: int inRuns (in) - number of times the simulation 
        function traverses the reference string
       char inOmega (inout) - address of the first character
        in the reference string. 
       int strLen (in) - length of the reference string 
PRE CONDITION(s): Reference string has been generated. Max Delta given
POST CONDITION(s): New instance of MemSim
--------------------------------------------------*/
MemSim::MemSim(int inRuns, char& inOmega, int strLen)
{
	numberRuns = inRuns;
	MainStr = &inOmega;
	MainStrLen = strLen; 
	Delta = 1; 

	resetStats(); //intitialize stats 

	FOUT.open(outFileName.c_str()); 

	
}
//--------------------------------------
//---DTOR---
MemSim::~MemSim()
{
	 
}
//--------------------------------------
//=== Private functions ======

/*---------/
resetStats() //-------------------------------------------
--------------------------------------------------------
Called in CTOR and after each loop through the simulation in order to 
zero-out the stats and reset the window indexed and bools. 

PRE CONDITION(s): Preparing for next traversal through reference string
POST CONDITION(s): initialized MemSim variables 
--------------------------------------------------*/
void MemSim::resetStats()
{
	workingSet = ""; 
	
	numPgFaults = 0; 
	sumWorkingSetSize = 0;

	avePgFaultRate = 0;
	aveWorkingSetSize = 0;

	windowBeg = 0;
	windowEnd = 0; 
	moveBeg = false;
	moveEnd = true;

}
//--------------------------------------

/*-----------/
moveWindow() //-------------------------------------------
--------------------------------------------------------
Checks if ends of the window should start/continue/cease to move
along the reference string. If allowed to move, increments them by one
PRE CONDITION(s): MemSim booleans are initialized by CTOR
POST CONDITION(s): Set to look at next substring in reference string 
--------------------------------------------------*/
void MemSim::moveWindow()
{
	//check if the window is allowed to grow or shrink
	if ((windowEnd +1) == Delta) //if next index will equal max window size 
	{
		moveBeg = true; //can start to move windowBeg
	}
	if ((windowEnd + 1) == MainStrLen) //if next index will move beyond string
	{
		moveEnd = false; //stop moving windowEnd
	}

	//move window index
	if (moveBeg)
	{
		windowBeg ++; 
	}
	if (moveEnd)
	{
		windowEnd ++; 
	}
}
//--------------------------------------------

/*----------------/
createSubstring() //----------------------------------
--------------------------------------------------------
converts desired section of character array pointer to a type string
INPUT: int here (in) - first char index of the substing. 
		used as iterator.
	   int end (in) - last char index of substring. 
OUTPUT: string version of the desired section of the reference
		string
PRE CONDITION(s): here and end are valid indexes within the
				  reference string
POST CONDITION(s): will be able to perform string methods
				with this section of the reference string. 
--------------------------------------------------*/
string MemSim::createSubstring(int here, int end)
{
	string thisStr = ""; 
	for (here; here <= end; here ++)
	{
		thisStr += MainStr[here]; 
	}
	return thisStr; 
}
//--------------------------------------

/*------------/
printResults()//---------------------------------------
--------------------------------------------------------
Prints Delta, numPgFaults, aveWorkingSetSize, and 
avePgFaultRate to console. Floats are rounded to three
decimal places 

PRE CONDITION(s): called from simulationGO()
POST CONDITION(s): Data printed to console
--------------------------------------------------*/
void MemSim::printResults()
{
	cout << left << setw(WIDTH) << Delta \
	<< left << setw(WIDTH) << numPgFaults \
	<< left << setw(WIDTH) << setprecision(4) << aveWorkingSetSize \
	<< left << setw(WIDTH) << setprecision(3) << avePgFaultRate << endl;  
}
//--------------------------------------

/*------------/
write2file() //-----------------------------------------
--------------------------------------------------------
Outputs Delta, numPgFaults, aveWorkingSetSize, and 
1/avePgFaultRate to FOUT channel 
PRE CONDITION(s): FOUT channel is open.
POST CONDITION(S) Data saved to .csv file
--------------------------------------------------*/
void MemSim::write2file()
{
	FOUT << Delta << COMMA << numPgFaults << COMMA \
	<< aveWorkingSetSize << COMMA << 1/avePgFaultRate << endl;
}
//--------------------------------------

//=== Public function =====

/*------------/
simulationGO() //-----------------------------------------
--------------------------------------------------------
Main simulation function. Runs through the memory management 
simulation, calculates the statistics, calls functions to output 
the data, resets the data and runs again for all sizes of delta up 
to the user's maximum.

PRE CONDITION(s): All values are intitialized to CTOR's values 
POST CONDITION(s): Outputs data to table and csv file.
				   MemSim variables are reset to initial values
				   except for Delta which equals numberRuns 
				   FOUT channel is closed
--------------------------------------------------*/
void MemSim::simulationGO()
{
	int toReplace; //indicates if value found in string and where.
	string subString; 

	for (Delta; Delta <= numberRuns; Delta ++)
	{ 

		while (windowBeg < MainStrLen)
		{
			//search working set for end value in reference string. If not found, returns -1
			toReplace = workingSet.find(MainStr[windowEnd]); 

			if (toReplace == NF)
			{
				numPgFaults ++; //increment page fault counter
			}//end if not found in working set

			//append windowEnd to back of working set
			workingSet += MainStr[windowEnd]; //make sure dont need to convert char to str

			//check if the front of the working set is still within the window
			//convert to subsring in order to search it.
			subString = createSubstring(windowBeg, windowEnd); 
			if (subString.find(workingSet[0]) == NF)
			{
				workingSet.erase(0, 1); //remove one char from the front
				toReplace --; //correct shift in toReplace 
			}//end if front does not match window begining

			//remove the found member of the working set 
			//if it wasn't found or deleted from the front, toReplace = -1
			if (toReplace > NF)
			{
				workingSet.erase(toReplace, 1); //remove the char at toReplace
			}//end if toReplace is still in working set

		
			//---finished manipulating workingSet ---
			//Now add to working set size summation and shift window over
			sumWorkingSetSize += workingSet.length(); 
			moveWindow(); 

		}//end while windowBeg is less than the size of the string

		//Finished Simulation. Calculating Averages
		if (MainStrLen > 0)
		{
			aveWorkingSetSize = sumWorkingSetSize / double(MainStrLen);
			avePgFaultRate = numPgFaults / double(MainStrLen); 
		}//end if string is not empty
		else
		{
			cout << "Error: attempted simulation with empty string. " << endl;
		}

		printResults(); 
		write2file(); 
		resetStats();

	}//end for number of runs
	FOUT.close(); 
}
//--------------------------------------

//--------------------------------------