// Zach Heine
// CS 318-01
// November 17th, 2015
// Project 4 Adjacent Word Finder
// Reads in a dictionary file into an STL Container
// in order to find words of the same length that 
// differ by only one character

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <windows.h>

using namespace std;

template <class Function>
__int64 time_call(Function&& f);
void readInputIntoSet(ifstream& in, set<string>& dict);
void getNumWordLengths(set<string> dict, map<size_t, set<string>>& distr, size_t& maxWordSize);
void printDistribution(map<size_t, set<string>> distr);
void getAdjacentWords(map<size_t, set<string>> distr, size_t maxWordSize, map<string, set<string>>& finalAdjWords);
void printAdjacencies(map<string, set<string>> adj, unsigned int numAdj);

int main()
{
	set<string> dictionary;
	set<string>::iterator itr;
	map<size_t, set<string>> distribution;
	map<string, set<string>> adjacency;
	size_t greatWordSize;
	ifstream infile;
	__int64 elapsed;
	unsigned int numAdjacent;
	cout << "This program prints words with their respective adjacent words in a dictionary\n"
		<< "that have at least N adjacent words." << endl;

	cout << "Please enter N: ";
	cin >> numAdjacent;

	// Function call using lambda expression for another function 
    // in order to get the time it takes to run the second function
	elapsed = time_call([&]{readInputIntoSet(infile, dictionary); });
	infile.close();
	cout << "Loading words into RAM took: " << elapsed << " ms." << endl;

	getNumWordLengths(dictionary, distribution, greatWordSize);
	printDistribution(distribution);

	elapsed = time_call([&]{getAdjacentWords(distribution, greatWordSize, adjacency); });
	printAdjacencies(adjacency, numAdjacent);

	cout << "Finding adjacent words took: " << elapsed << " ms." << endl;

	return 0;
}

// Function to get the time it takes for a particular function to run
template <class Function>
__int64 time_call(Function&& f)
{
	__int64 begin = GetTickCount();
	f();
	return GetTickCount() - begin;
}

// Grab the data from the file store it into a temporary string before it
// gets put into an STL Container.
void readInputIntoSet(ifstream& in, set<string>& dict)
{
	string currentWord;
	in.open("dctnryWords.txt");

	// Check if the file is a file in that directory
	// otherwise print an error message and exit the
	// program.
	if (in.fail() == true)
	{
		cout << "Cannot open file! ";
		cout << "Aborting!!!" << endl;
		exit(1);
	}

	while (!in.eof())
	{
		while (getline(in, currentWord))
		{
			dict.insert(currentWord);
		}
	}
}

// Separate the contents of the initial container into
// another container using the size of the strings as 
// keys in a map
void getNumWordLengths(set<string> dict, map<size_t, set<string>>& distr, size_t& maxWordSize)
{
	string temp;
	for (auto i : dict)
	{
		temp = i;
		distr[temp.length()].insert(temp);
	}
	// Because the lengths are the keys the size is equivalent to the highest word length
	// we can grab the size of the map and use it for the getAdjacentWords function
	maxWordSize = distr.size();
}

// Prints out the size of each location in the map of string lengths
void printDistribution(map<size_t, set<string>> distr)
{
	cout << "The words only dictionary has a following distribution of word lengths..." << endl;
	for (int i = 1; distr[i].size() != 0; i++)
	{
		cout << "The number of words of length " << i << " = " << distr[i].size() << endl;
	}
}

void getAdjacentWords(map<size_t, set<string>> distr, size_t maxWordSize, map<string,set<string>>& finalAdjWords)
{
	string initWord;

	// Iterate through until the maxWordSize or the size of the map
	for (size_t i = 0; i < maxWordSize; i++)
	{
		// Create a temporary map that will hold the n-1 strings as keys
		// and the original strings as values
		// Use a pair to avoid the matchings getting mixed up 
		map<pair<size_t, string>, set<string>> tempAdjWords;
		// Go through the different sets in the map
		for (set<string>::iterator setItr = distr[i].begin(); setItr != distr[i].end(); setItr++)
		{
			for (size_t j = 0; j < (*setItr).length(); j++)
			{
				// Create a string that holds each different word
				// and then removes characters at each different 
				// position
				string initWord = *setItr;
				initWord.erase(j,1);
				// Store the initial string at key-pair for the position removed
				// and the n-1 string at that position.
				tempAdjWords[make_pair(j,initWord)].insert(*setItr);
			}
			
		}
		// Go through the sets inside the map again this time to insert the
		// initial strings into the final adjacency map.
		for (set<string>::iterator setIter = distr[i].begin(); setIter != distr[i].end(); setIter++)
		{	
			for (size_t j = 0; j < (*setIter).length(); j++)
			{
				string initWord = *setIter;
				initWord.erase(j, 1);
				// Perform the operation if the temp map has values in it
				if (tempAdjWords[make_pair(j, initWord)].size() != 0)
				{
					// Iterate through the temp Map
					for (set<string>::iterator tempSetIter = tempAdjWords[make_pair(j, initWord)].begin(); tempSetIter != tempAdjWords[make_pair(j, initWord)].end(); tempSetIter++)
					{
						// If the current word in the temp map is not equal to the current word in the lengths map
						// insert that word into the final map
						if (*tempSetIter != *setIter)
							finalAdjWords[*setIter].insert(*tempSetIter);
					}
				}
			}
		}
	}
}

void printAdjacencies(map<string, set<string>> adj, unsigned int numAdj)
{
	set<string>::const_iterator setItr;
	for (map<string, set<string>>::const_iterator mapItr = adj.begin(); mapItr != adj.end(); mapItr++)
	{
		// Iterate through the map checking if the current set's size is greater than
		// or equal to the number of adjacents entered by the user
		if (mapItr->second.size() >= numAdj)
		{
			// If so print the word the size of it's set of adjacents which is how
			// many words are adjacent to it and each adjacent word.
			cout << mapItr->first << "(" << mapItr->second.size() << "): ";
			for (setItr = mapItr->second.begin(); setItr != mapItr->second.end(); setItr++)
			{
				cout << *setItr << " ";
			}
			cout << endl;
		}
	}
}