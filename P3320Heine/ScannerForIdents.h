#pragma once
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string> // STL string class
#include "RestrictedMap.h"

using namespace std;

class ScannerForIdents
{
public:
	explicit ScannerForIdents(iostream &fin) : numOfLines{ 0 }, numofChars{ 0 }, ch{ '	' }, lexene{ "" },
		closedComment{ false }, chLiteral{ false }, strLiteral{false}
	{
		lexene = "";
		currLine = "";
		sIter = currLine.begin();
	}

	int getIdent(fstream& fin, string& lexene)
	{
		if (fin.eof())
		{
			return 0;
		}

		lexene.clear();

		if (sIter == currLine.end())
		{
			getline(fin, currLine);
			sIter = currLine.begin();
			numOfLines++;
		}

		while (sIter != currLine.end())
		{
			// If it is also a closed comment
			// on a new line
			if (closedComment == true)
			{
				while (sIter != currLine.end() && (*sIter != '*' || (*(sIter + 1) != currLine.size() && *(sIter + 1) != '/')))
					sIter++;
				if (sIter == currLine.end())
					break;
				closedComment = false;
				sIter += 2;
				continue;
			}

			// If char literal still on new line
			if (chLiteral == true)
			{
				while (sIter != currLine.end() && *sIter != '\'')
					sIter++;
				if (sIter == currLine.end())
					break;
				chLiteral = false;
				sIter += 1;
				continue;
			}

			// If string literal is still on new line
			if (strLiteral == true)
			{
				while (sIter != currLine.end() && *sIter != '\"')
					sIter++;
				if (sIter == currLine.end())
					break;
				strLiteral = false;
				sIter += 1;
				continue;
			}

			// Checking for comments
			if (*sIter == '/')
			{
				sIter++;
				if (*sIter == '*')
				{
					closedComment = true;
					sIter++;
					while (sIter != currLine.end() && (*sIter != '*' || (*(sIter + 1) != currLine.size() && *(sIter + 1) != '/')))
						sIter++;
					if (sIter == currLine.end())
						break;
					closedComment = false;
					sIter += 2;
				}
				if (sIter == currLine.end() || *sIter == '/')
				{
					while (sIter != currLine.end())
						sIter++;
					break;
				}
			}

			// Check for character literals
			if (*sIter == '\'')
			{
				chLiteral = true;
				sIter++;
				while (sIter != currLine.end() && *sIter != '\'')
					sIter++;
				if (sIter == currLine.end())
					break;
				chLiteral = false;
				sIter++;
				continue;
			}

			// Check for character literals
			if (*sIter == '\"')
			{
				strLiteral = true;
				sIter++;
				while (sIter != currLine.end() && *sIter != '\'')
					sIter++;
				if (sIter == currLine.end())
					break;
				strLiteral = false;
				sIter++;
				continue;
			}

			// If the line begins with an underscore or
			// an alphabetic character
			if ((*sIter == '_' || isalpha(*sIter)) && closedComment != true)
			{
				lexene += *sIter;
				sIter++;
				while ((sIter != currLine.end() && isalpha(*sIter)||*sIter == '_'))
				{
					lexene += *sIter;
					sIter++;
				}
				break;
			}

			// If the line begins with a digit
			if (isdigit(*sIter))
			{
				while (isalnum(*sIter))
					sIter++;
			}
			sIter++;
		}
		return 1;
	}

	int getNumberOfLines() const
	{
		return numOfLines;
	}
private:
	unsigned int numOfLines;
	unsigned int numofChars;
	unsigned int numOfIdents;
	bool closedComment;
	bool chLiteral;
	bool strLiteral;
	char ch;
	string lexene;
	string currLine;
	string::iterator sIter;
};

bool isInvalidIdent(string &lexene)
{
	if (lexene.empty())
		return true;

	string::const_iterator lIter = lexene.end() - 1;
	while (lIter != lexene.begin())
	{
		if (!isalnum(*lIter) && *lIter != '_')
		if (lIter == (lexene.end() - 1))
		{
			lexene.erase(lIter);
			lIter--;
			continue;
		}
		else
			return true;
		lIter--;
	}
	return false;
}

bool isReserved(string lexene)
{
	const vector<string> reservedWords = { "and", "and_eq", "asm", "auto", "bitand", "bitor", "bool", "break",
		"case", "catch", "char", "class", "compl", "const", "const_cast", "continue",
		"default", "delete", "do", "double", "dynamic_cast", "else", "enum", "explicit",
		"extern", "false", "float", "for", "friend", "goto", "if", "inline", "int", "long",
		"mutable", "namespace", "new", "not", "not_eq", "operator", "or", "or_eq", "private",
		"protected", "public", "register", "reinterpret-cast", "return", "short", "signed",
		"sizeof", "static", "static_cast", "struct", "switch", "template", "this", "throw",
		"true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual",
		"void", "volatile", "wchar_t", "while", "xor", "xor_eq" };

	for (auto i : reservedWords)
	{
		if (lexene == i)
		{
			return true;
		}
	}
	return isInvalidIdent(lexene);
}

template<typename Key, typename T>
void printCrossRefTable(RestrictedMap<Key, T> &xRefTable, fstream &fout)
{
	fout << "Ident" << setw(18) << "Line Numbers" << endl;
	for (RestrictedMap<Key, T>::map_iterator mIter = xRefTable.begin(); mIter != xRefTable.end(); mIter++)
	{
		fout << setw(10) << left << (*mIter).first;
		for (auto tIter = (*mIter).second.begin(); tIter != (*mIter).second.end(); *tIter++)
		{
			fout << " " << (*tIter);
		}
		fout << endl;
	}
}