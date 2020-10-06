#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>

int bitExtraction(int, int, int);

int main(int argc, char ** argv) {

	std::ifstream inputFile;

	std::string formatStr;
	long long int pcAddress;
	long long int index;
	int GHB = 0;
	int* branchTable;
	char outcome;
	int numentries;
	int i = 0;

	//final stats
	int misspredicts = 0;
	int totalpredicts = 0;
	float missratio = 0;

	int m = atoi(argv[1]);
	int n = atoi(argv[2]);

	inputFile.open(argv[3]);

	numentries = std::pow(2, m);
	branchTable = new int[numentries];

	if (!inputFile) {
	
		std::cout << "input file was unsuccessfully opened terminating program." << std::endl;
		exit(0);
	}

	/*1 = Weakly taken
	  2 = Strongly taken
	  0 = Weakly not taken
	 -1 = strongly not taken
	*/

	for (i = 0; i < numentries; i++) {
	
		branchTable[i] = 1; // initialize to weakly taken
	}
	while (!inputFile.eof()) {

		inputFile >> formatStr;
		inputFile >> outcome;

		pcAddress = stoi(formatStr, nullptr, 16);


		//remove the least significant 2 bits
		pcAddress = pcAddress >> 2;

		//take the M least significant bits from the new PC Address

		pcAddress = bitExtraction(pcAddress, m, 1);


		// XOR the results 
		index = (pcAddress ^ (GHB << (m - n)));
		

		//update GHB, 
		//update branchTable entry 
		//update final states
		switch (branchTable[index]) {

		case -1: // strongly not taken

			if (outcome == 'n' || outcome == 'N') {

				totalpredicts++;
				GHB = GHB >> 1;
			}

			else if (outcome == 't' || outcome == 'T') {

				totalpredicts++;
				misspredicts++;
				GHB = GHB >> 1;
				GHB = GHB + std::pow(2, n - 1);
				branchTable[index] = 0; // weakly not taken

			}
			break;

		case 0: // weakly not taken

			if (outcome == 'n' || outcome == 'N') {

				totalpredicts++;
				GHB = GHB >> 1;
				branchTable[index] = -1; // weakly not taken to strongly not taken
			}

			if (outcome == 't' || outcome == 'T') {

				totalpredicts++;
				misspredicts++;
				GHB = GHB >> 1;
				GHB = GHB + std::pow(2, n - 1);
				branchTable[index] = 1; // weakly taken to weakly taken 
			}
			break;

		case 1: // weakly taken 

			if (outcome == 'n' || outcome == 'N') {


				totalpredicts++;
				misspredicts++;
				GHB = GHB >> 1;
				branchTable[index] = 0; //weakly taken to weakly not taken

			}

			if (outcome == 't' || outcome == 'T') {

				totalpredicts++;
				GHB = GHB >> 1;
				GHB = GHB + std::pow(2, n - 1);
				branchTable[index] = 2; // weakly taken to strongly taken

			}
			break;

		case 2: // strongly taken

			if (outcome == 'n' || outcome == 'N') {


				totalpredicts++;
				misspredicts++;
				GHB = GHB >> 1;
				branchTable[index] = 1;
			}

			if (outcome == 't' || outcome == 'T') {


				totalpredicts++;
				GHB = GHB >> 1;
				GHB = GHB + std::pow(2, n - 1);
			}
			break;
		default:
			std::cout << "the default case" << std::endl;
			break;
		}
	}

	

	std::cout << std::dec << std::fixed << "M: " << m << " N: " << n;

	missratio = misspredicts / (float)totalpredicts;
	missratio = missratio * 100;

	std::cout << std::setprecision(3) << std::fixed << " Miss ratio: " << missratio << "%"<< std::endl;
	

	inputFile.close();
	delete[] branchTable;
	return 0;
}

int bitExtraction(int number, int k, int p){

	return (((1 << k) - 1) & (number >> (p - 1)));
}