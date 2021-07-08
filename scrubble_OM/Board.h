#ifndef BOARD_H
#define BOARD_H
#include <iterator>
#include <windows.h>
#include "methods.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <ctime> 
#include <fstream>
#include <tuple>
using namespace std;
typedef vector<tuple<string, int>> myTuple;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int calculateLetter(char letter, char modifier);//returns score for a separate letter on the board, including lette modifiers

class Square//Each tile on the board is an object of this class
{
public:
	int pos_x, pos_y;
	char multy;//Multiplier
	char storedLetter = '0'; //in the beginning there is no letters in tiles;

};

class Player
{
public:
	vector<char> playerHand = {};
};

class Bot {
public:
	vector<char> botHand = {};

	void printLetters()
	{
		for (int i = 0; i < botHand.size(); ++i)
		{
			cout << botHand[i] << " ";
		}
		cout << endl;
	}
	
};
class Board//Creates a board full of Square object and holds most of the controls
{
public:
	int player_totalScore = 0;
	int bot_totalScore = 0;
	Square board1[15][15];
	int lettersLeft = 100;//at the start of the game 100 letters are in the bag
	int totalScore = 0;
	bool isFirstTurn = true;
	vector<char> allLetters = {'a','a','a','a','a','a','a','a','a','b','b','c','c','d','d','d','d','e','e','e','e','e','e','e','e','e','e','e','e','f','f','g','g','g',
	'h','h','i','i','i','i','i','i','i','i','i','j','k','l','l','l','l','m','m','n','n','n','n','n','n','o','o','o','o','o','o','o','o','p','p','q','r','r','r','r','r','r',
	's','s','s','s','t','t','t','t','t','t','u','u','u','u','v','v','w','w','x','y','y','z','#','#'};//all the letters in the bag before the game started
	Board() {
		
		for (int i = 0; i < 15; ++i)
		{
			for (int j = 0; j < 15; ++j)
			{
				board1[i][j].pos_x = i;
				board1[i][j].pos_y = j;
				if ((j == 0 || j == 14) && (i == 0 || i == 7 || i == 14) || ((j == 7) && (i == 0 || i == 14)))
				{
					board1[i][j].multy = 'T'; //triple word score
				}
				else if (((j == 5 || j == 9) && (i == 1 || i == 5 || i == 9 || i == 13)) || ((j == 1 || j == 13) && (i == 5 || i == 9)))
				{
					board1[i][j].multy = 't'; //triple letter score
				}
				else if (((j == 3 || j == 11) && (i == 0 || i == 7 || i == 14)) || ((j == 0 || j == 14 || j == 7) && (i == 3 || i == 11)) || ((j == 2 || j == 12) && (i == 6 || i == 8)) || ((j == 3 || j == 11) && (i == 7)) || ((j == 6 || j == 8) && (i == 2 || i == 6 || i == 8 || i == 12)))
				{
					board1[i][j].multy = 'd'; //double letter score
				}
				else if (((j == 1 || j == 13) && (i == 1 || i == 13)) || ((j == 2 || j == 12) && (i == 2 || i == 12)) || ((j == 3 || j == 11) && (i == 3 || i == 11)) || ((j == 4 || j == 10) && (i == 4 || i == 10)) || (i == 7 && j == 7))
				{
					board1[i][j].multy = 'D'; //double word score
				}
				else
				{
					board1[i][j].multy = '_';//no modifiers
				}
			}
		}
		drawBoard(board1);//draw the initial state of the board
	};

	bool binaryLegalCheck(myTuple checkList, string word);
	void drawBoard(Square board[15][15]);//draws the current state of board
	void MakeMove(myTuple checkList, vector<char> &hand);
	void BotMakeMove(myTuple checkList, vector<char>& hand);
	void refreshLetters(vector<char> &hand);
	
};

bool Board::binaryLegalCheck(myTuple checkList, string word)
{
	int low = 0;
	int high = checkList.size() - 1;

	while (low <= high)
	{
		int middle = low + (high - low) / 2;

		if (get<0>(checkList[middle]) == word)
			return true;

		if (get<0>(checkList[middle]) > word)
		{
			high = middle - 1;
		}

		else
			low = middle + 1;
	}

	return false;
}

void Board::refreshLetters(vector<char> &hand)//Gives player letters until he has 7
{
	int dif;
	int randomNumber;
	char temp;
	if (hand.size() < 7)
	{
		dif = 7 - hand.size();//checking how much letters should be dealt to the player
		for (int i = 1; i <= dif; ++i)//randomly assigning letters to the hand
		{
			srand((unsigned)time(0));
			randomNumber = rand() % allLetters.size();
			temp = allLetters[randomNumber];
			allLetters.erase(allLetters.begin() + randomNumber);
			hand.push_back(temp);
		}
	}

	bool swaps = true;//sorting the hand in alphabetical order
	while (swaps)
	{
		swaps = false;
		for (int i = 0; i < hand.size() - 1; i++)
		{
			if (hand[i] > hand[i + 1]) {
				swaps = true;
				swap(hand[i], hand[i + 1]);
			}
		}
	}
}

void Board::drawBoard(Square board[15][15])//draws the current state of board
{
	cout << "    ";
	for (int x = 0; x <= 14; ++x)
	{
		if (x < 10)
		{
			cout << "0";
		}
		cout << x << " ";
	}
		cout << endl;

	for (int i = 0; i < 15; ++i)
	{
		if (i < 10)
			cout << "0";
		cout << i << "  ";
		for (int j = 0; j < 15; ++j)
		{
			if (board[i][j].storedLetter == '0')//If no player has not put any letters into this tile yet
			{
				if (board[i][j].multy == 'T' || board[i][j].multy == 't') {
					SetConsoleTextAttribute(hConsole, 4);//set color of word multipliers to red
					cout << board[i][j].multy << "  ";
					SetConsoleTextAttribute(hConsole, 15);
				}
				else if (board[i][j].multy == 'D' || board[i][j].multy == 'd')
				{
					SetConsoleTextAttribute(hConsole, 14);//set color of word multipliers to blue
					cout << board[i][j].multy << "  ";
					SetConsoleTextAttribute(hConsole, 15);
				}
				else
				{
					cout << board[i][j].multy << "  ";
				}
			}
			else
			{
				SetConsoleTextAttribute(hConsole, 2);//set inserted letters' color to green
				cout << board[i][j].storedLetter << "  ";
				SetConsoleTextAttribute(hConsole, 15);
				board[i][j].multy = '_'; // whenever a letter is writter into a tile we get rid of its multipliers for future score calculations
			}
			if (j == 14)
				cout << endl;
		}
	}
}

void Board::BotMakeMove(myTuple checkList, vector<char>& hand)//This function is responsible for AI moves
{
	string bestWord;
	bool isLetter;
	bool isTop_down = true;
	bool isTripleWord = false;
	bool isDoubleWord = false;
	int wordScore = 0;
	if (isFirstTurn)//if it is the first turn Bot randomly chooses in which direction to write (50/50)
	{
		int randomNumber;
		srand((unsigned)time(0));
		randomNumber = rand() % 2;
		if (randomNumber == 1)
			isTop_down = true;
		else
			isTop_down = false;
	}
	
	
	int x = 7;//Starting position for writing, right now it only works on centre of the field. In future updates program will go through all the tiles on the board and check appropriate position 
	int y = 7;



	myTuple availableWords = {};
	vector<char> tempHand{};
	for (myTuple::const_iterator i = checkList.begin(); i != checkList.end(); ++i)//goes through all the words in the dictionary
	{
		isLetter = false;
		for (int j = 0; j < get<0>(*i).size(); j++)//goes through letters of a word
		{
			if (find(hand.begin(), hand.end(), get<0>(*i)[j]) != hand.end())
			{
				isLetter = true;
				tempHand.push_back(get<0>(*i)[j]);
				hand.erase(find(hand.begin(), hand.end(), get<0>(*i)[j]));
			}
			else if (find(hand.begin(), hand.end(), '#') != hand.end())
			{
				tempHand.push_back('#');
				hand.erase(find(hand.begin(), hand.end(), '#'));
				isLetter = true;
			}
			else
			{
				isLetter = false;
				break;
			}
		}
		if (isLetter == true && get<0>(*i).size() <= 7)
		{
			availableWords.push_back(make_tuple(get<0>(*i), get<1>(*i)));
			for (int i = 0; i < tempHand.size(); ++i)
			{
				hand.push_back(tempHand[i]);
			}
			tempHand = {};
		}
		else
		{
			for (int i = 0; i < tempHand.size(); ++i)
			{
				hand.push_back(tempHand[i]);
			}
			tempHand = {};
		}
	}


	for (myTuple::const_iterator i = availableWords.begin(); i != availableWords.end(); ++i)
	{
		cout << get<0>(*i) << "  " << get<1>(*i) << endl;
	}
	int maxIndex = 0;//starting max algorithm to find which word, among the ones that can be constructed with current hand, gives the most score
	int currentIndex = 1;
	for (myTuple::const_iterator i = availableWords.begin()+1; i != availableWords.end(); ++i)
	{
		if (get<1>(*i) > get<1>(availableWords[maxIndex]))
			maxIndex = currentIndex;

		currentIndex ++;
	}
	bestWord = get<0>(availableWords[maxIndex]);//the most valueble word
	cout << bestWord << endl;

	int index;
	for (int i = 0; i < bestWord.size(); ++i)
	{
		vector<char>::iterator it = find(hand.begin(), hand.end(), bestWord[i]);
		if (it != hand.end())
		{
			index = distance(hand.begin(), it);
			hand.erase(hand.begin() + index);
		}
	}

	for (size_t i = 0; i < bestWord.size(); ++i)//Fills the tiles with letters of the input one by one
	{
		if (isTop_down)//The next two blocks are the almost identical, but first goes top_down and second - left_right
		{
			board1[y + i][x].storedLetter = bestWord[i];//assigning the letter to the Square object to print it on the board later
			wordScore += calculateLetter(bestWord[i], board1[y + i][x].multy);//computing the score of the letter with its modifier
			if (board1[y + i][x].multy == 'T')//checking if the word will hace double or triple score 
				isTripleWord = true;
			else if (board1[y + i][x].multy == 'D')//We can use booleans here because in scrabble no word can possibly have two words modifiers at once
				isDoubleWord = true;
		}
		else//Same block as before but for horizontal placement
		{
			board1[y][x + i].storedLetter = bestWord[i];
			wordScore += calculateLetter(bestWord[i], board1[y][x + i].multy);
			if (board1[y][x + i].multy == 'T')
				isTripleWord = true;
			else if (board1[y][x + i].multy == 'D')
				isDoubleWord = true;
		}
	}
	bot_totalScore += wordScore;
	cout << "Bot's word's score is: " << wordScore << endl;
	cout << "Bot's total Score is: " << bot_totalScore << endl;

	drawBoard(board1);
	isFirstTurn = false;
	refreshLetters(hand);
	cout << "There are " << allLetters.size() << " letters in the bag left." << endl;

}
void Board::MakeMove(myTuple checkList, vector<char> &hand)//This function allows player to make a move, checks if it is allowed and assgins the score
{
	int x;
	int y;
	string enter_word;
	bool isConnected;
	bool isTop_down;
	bool wordInVocab = false;//Checks if word is legal
	bool isTripleWord = false;
	bool isDoubleWord = false;//check if word modifier was activated, so we can account for it near the end of the function

	int wordScore = 0;
LABEL://Whenever user gets an error, the program will return to this point, so that user is never stuck and can always continue playing
	
	cout << "You can use the following letters(# is any letter): " << endl;
	for (int i = 0; i < hand.size(); ++i)
	{
		cout << hand[i] << " ";
	}
	cout << endl;

	cout << "Do you want to skip the turn? (1 - SKIP, 0 - CONTINUE): ";
	bool skip;
	while (!(cin >> skip)) {
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Bad data, try again: ";
	}
	if (skip == 1)
		return;
	

	cout << "Do you want to write top-down direction (1 = top down, 0 = left-right): ";
	while (!(cin >> isTop_down)) {
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Bad data, try again: ";
	}

	if (isFirstTurn)//if it is first turn then the player should start from the centre
	{
		x = 7;
		y = 7;
	}
	else//If it is not the first turn player should choose the starting point himself
	{

		cout << "Enter the coordinates of the starting position.\n";
		cout << "x: ";
		while (!(cin >> x)) {
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Bad data, try again: ";
		}
		cout << "y: ";
		while (!(cin >> y)) {
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Bad data, try again: ";
		}
	}

	cout << "Enter the word: ";
	cin >> enter_word;
	
	if (binaryLegalCheck(checkList, enter_word) == true)
	{
		wordInVocab = true;
	}

	if ((isTop_down == false && enter_word.size() + x >= 16) || (isTop_down == true && enter_word.size() + y >= 16))//if the word would go out of board's boundaries we need to ask for a new word
	{
		wordInVocab = false;
		cout << "This word doesn't fit in the board" << endl;
		goto LABEL;
	}
	else if (wordInVocab == false)
	{
		cout << "Illegal word, try again" << endl;
		goto LABEL;
	}

	if (!isFirstTurn)// if it is the first turn, the words should not be connected
		isConnected = false;
	else
		isConnected = true;


	for (int i = 0; i < enter_word.size(); ++i)//checking wether the word would connect to another existing word 
	{
		if (isTop_down)
		{
			if (board1[y + i][x].storedLetter != '0')
				isConnected = true;
		}
		else
		{
			if (board1[y][x + i].storedLetter != '0')
				isConnected = true;
		}
	}
	if (isConnected == false)
	{
		cout << "At least one letter in a word must be connected to another word." << endl;
		goto LABEL;
	}
	//Following block checks if we have the words necessary for the entered word, taking into account blank letters and the letters that are already on the board
	int index;
	bool hasLetter = true;
	bool usedBlank = false;
	for (int i = 0; i < enter_word.size(); ++i)
	{
		if (isTop_down) {
			if (board1[y + i][x].storedLetter == enter_word[i])
			{
				continue;
			}
			else if (find(hand.begin(), hand.end(), enter_word[i]) != hand.end()) {
				continue;
			}
			else if (find(hand.begin(), hand.end(), '#') != hand.end() && !usedBlank)
			{
				usedBlank = true;
				continue;
			}
			else
			{
				hasLetter = false;
				break;
			}
		}
		else
		{
			if (board1[y][x + i].storedLetter == enter_word[i])
			{
				continue;
			}
			else if (find(hand.begin(), hand.end(), enter_word[i]) != hand.end()) {	
				continue;
			}
			else
			{
				hasLetter = false;
				break;
			}
		}
	}
	if (hasLetter)
	{
		for (int i = 0; i < enter_word.size(); ++i)
		{
			vector<char>::iterator it = find(hand.begin(), hand.end(), enter_word[i]);
			if (it != hand.end())
			{
				index = distance(hand.begin(), it);
				hand.erase(hand.begin() + index);
			}
		}

		if (usedBlank)//if the blank letter was used, we take it out of the hand
		{
			vector<char>::iterator it = find(hand.begin(), hand.end(), '#');
			if (it != hand.end())
			{
				index = distance(hand.begin(), it);
				hand.erase(hand.begin() + index);
			}
		}
			
		
	}
	if (!hasLetter)
	{
		cout << "You don't have the letters required for this word. Try again." <<  endl;
		goto LABEL;
	}
		
	for (size_t i = 0; i < enter_word.size(); ++i)//Fills the tiles with letters of the input one by one
	{
		if (isTop_down)//The next two blocks are the almost identical, but first goes top_down and second - left_right
		{
			board1[y + i][x].storedLetter = enter_word[i];//assigning the letter to the Square object to print it on the board later
			wordScore += calculateLetter(enter_word[i], board1[y + i][x].multy);//computing the score of the letter with its modifier
			if (board1[y + i][x].multy == 'T')//checking if the word will hace double or triple score 
				isTripleWord = true;
			else if (board1[y + i][x].multy == 'D')//We can use booleans here because in scrabble no word can possibly have two words modifiers at once
				isDoubleWord = true;
		}
		else//Same block as before but for horizontal placement
		{
			board1[y][x + i].storedLetter = enter_word[i];
			wordScore += calculateLetter(enter_word[i], board1[y][x + i].multy);
			if (board1[y][x + i].multy == 'T')
				isTripleWord = true;
			else if (board1[y][x + i].multy == 'D')
				isDoubleWord = true;
		}
	}
	if (isDoubleWord)
		wordScore *= 2;
	if (isTripleWord)
		wordScore *= 3;
	player_totalScore += wordScore;
	cout << "Word's score is: " << wordScore << endl;
	cout << "Player's total Score is: " << player_totalScore << endl;

	drawBoard(board1);
	isFirstTurn = false;
	refreshLetters(hand);
	cout << "There are " << allLetters.size() << " letters in the bag left." << endl;
}
#endif // !BOARD.H