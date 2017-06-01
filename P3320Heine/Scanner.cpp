#include <iostream>
#include <fstream>
#include <set>
#include "RestrictedMap.h"
#include "ScannerForIdents.h"

using namespace std;

int main()
{
	RestrictedMap<string, set<int>> xRefTable;
	string infileName = "Test.cpp";
	string outfileName = "output.txt";
	fstream infile;
	fstream outfile;

	cout << "Please enter a filename to read identifiers from: ";
	cin >> infileName;
	infile.open(infileName, fstream::in);
	if (infile.fail())
	{
		exit(1);
	}

	string lexene;
	ScannerForIdents aScanner(infile);

	int nextToken;
	int currentLineNum;
	while (nextToken = aScanner.getIdent(infile, lexene) == 1)
	{
		if (!isReserved(lexene))
		{
			currentLineNum = aScanner.getNumberOfLines();
			xRefTable[lexene].insert(currentLineNum);
		}
	}

	cout << "Please enter a filename to output the identifier table to: ";
	cin >> outfileName;
	outfile.open(outfileName, fstream::out);
	if (outfile.fail())
	{
		exit(1);
	}
	
	printCrossRefTable(xRefTable, outfile);
	infile.close();
	outfile.close();
}