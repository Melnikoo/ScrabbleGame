#ifndef METHODS_H
#define METHODS_H

#include "Board.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <tuple>
using namespace std;


int calculateScore(string word)//insert any string to calculate the score
{
	int sum = 0;

	for (size_t i = 0; i < word.size(); ++i)
	{
		if (word[i] == 'a' || word[i] == 'e' || word[i] == 'i' || word[i] == 'o' || word[i] == 'u' || word[i] == 'l' || word[i] == 'n' || word[i] == 's' || word[i] == 't' || word[i] == 'r')
			sum += 1;
		else if (word[i] == 'd' || word[i] == 'g')
			sum += 2;
		else if (word[i] == 'b' || word[i] == 'c' || word[i] == 'm' || word[i] == 'p')
			sum += 3;
		else if (word[i] == 'f' || word[i] == 'h' || word[i] == 'v' || word[i] == 'w' || word[i] == 'y')
			sum += 4;
		else if (word[i] == 'k')
			sum += 5;
		else if (word[i] == 'j' || word[i] == 'x')
			sum += 8;
		else if (word[i] == 'q' || word[i] == 'z')
			sum += 10;
	}
	return sum;
}

int calculateLetter(char letter, char modifier)//returns score for a separate letter on the board, including lette modifiers
{
	int letterScore = 0;
	if (letter == 'a' || letter == 'e' || letter == 'i' || letter == 'o' || letter == 'u' || letter == 'l' || letter == 'n' || letter == 's' || letter == 't' || letter == 'r')
		letterScore = 1;
	else if (letter == 'd' || letter == 'g')
		letterScore = 2;
	else if (letter == 'b' || letter == 'c' || letter == 'm' || letter == 'p')
		letterScore = 3;
	else if (letter == 'f' || letter == 'h' || letter == 'v' || letter == 'w' || letter == 'y')
		letterScore = 4;
	else if (letter == 'k')
		letterScore = 5;
	else if (letter == 'j' || letter == 'x')
		letterScore = 8;
	else if (letter == 'q' || letter == 'z')
		letterScore = 10;

	if (modifier == 'd')
		letterScore *= 2;
	else if (modifier == 't')
		letterScore *= 3;

	return letterScore;
}

myTuple filterVocabluary()//returns a vector of tuples with the words and their original score accordingly
{
	ifstream vocabluary;// text file
	vocabluary.open("correctVocab.txt");//Opens an already filtered word pool
	string word;//storing values from getline here
	myTuple correctVocab = {};
	if (vocabluary.fail())
	{
		cout << "Could not open the file";
	}
	while (getline(vocabluary, word))
	{
		correctVocab.push_back(make_tuple(word, calculateScore(word)));
	}
	return correctVocab;
}

void createCorrectVocab()// This functions does the pre-computing filtering the given text file full of words and outputing a new one that will be used by this program later on
{
	ifstream vocabluary;
	vocabluary.open("words.txt");
	string word;//storing values from getline here
	vector<string> correctVocab = {};
	bool rightWord;
	if (vocabluary.fail())
	{
		cout << "Could not open the file";
	}
	while (getline(vocabluary, word))//checking each word on wheter it has correct format for our game
	{
		rightWord = true;
		for (size_t i = 0; i < word.size(); ++i)
		{
			if (word[i] < 97 || word[i] > 122)
			{
				rightWord = false;
				break;
			}
		}
		if (rightWord)
		{
			correctVocab.push_back(word);
		}
	}
	ofstream outfile("correctVocab.txt");//new sorted textfile 

	for (int i = 0; i < correctVocab.size(); i++)
	{
		outfile << correctVocab[i] << endl;
	}
	outfile.close();
}
#endif