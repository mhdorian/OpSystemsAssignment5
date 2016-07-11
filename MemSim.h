
//=== MemSim.h =====================

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> 

using namespace std; 

const int NF = -1; //"not found"
const int WIDTH = 10; // formatting table
const string COMMA = ","; 
const string outFileName = "Output_os5.csv"; //name of output file

class MemSim
{
private:
	//variables
	int numberRuns; //number of times the simulation will loop  
	int Delta; //maxumum size of the window and working-set 
	int MainStrLen; //size of the refernce string
	char * MainStr; //pointer to generated string (omega) 
	string workingSet; //Memoery's working-set string 

	int numPgFaults, sumWorkingSetSize; //total number of page faults, sumation of working set string size 
	double avePgFaultRate, aveWorkingSetSize; //average page fault rate, average working-set size 

	int windowBeg, windowEnd; //holds index values of current window placement in reference string
	//(beg holds the lower index and end holds the higher)
	bool moveBeg, moveEnd; //check if pointers can travel the reference string 

	ofstream FOUT; //output file std

	//functions
	void resetStats();
	void moveWindow();
	string createSubstring(int here, int end); 
	void printResults(); 
	void write2file();  
	 

public:
	//CTOR
	MemSim(int inRuns, char& inOmega, int strLen);
	//DTOR
	~MemSim();

	//function
	void simulationGO(); 
	
};