#include "methods.h"
#include "Board.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <tuple>
using namespace std;


int main()
{
	myTuple correctVocab = filterVocabluary();//creating a vector of tuple which contains all the legal words ant their base scores	
	cout << "NUmber of legal words: " << correctVocab.size() << endl;
	Player player1;
	Bot bot1;
	Board b1;//constructor of this class creates and draws the board 
	b1.refreshLetters(player1.playerHand);//dealing the initial hands
	b1.refreshLetters(bot1.botHand);
	
	bot1.printLetters();//prints what letters the bot can use
	b1.BotMakeMove(correctVocab, bot1.botHand);//The first turn is bot's
	while (b1.allLetters.size() > 0)//Game continues as long as there are letters left in the bag
	{
		b1.MakeMove(correctVocab, player1.playerHand);
	}
	
	return 0;
}