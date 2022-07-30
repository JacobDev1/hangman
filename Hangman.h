#pragma once
#include <iostream>
#include <string>
#include <conio.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>

using namespace std;

enum PrintMode{Default, Center, Fill};

class Hangman
{
public:
	Hangman();
	~Hangman();

	void Main();

private:
	void PrintLine(string l_message, PrintMode l_mode = PrintMode::Default, char l_bondariesSign = '|');

	string GetLetters(char l_from, char l_to);
	string GetWord();
	string GetCleanWord();

	void Reset();

	int m_guessCount;
	bool m_guessed;
	int const m_maxGuessCount = 7;

	string m_word;
	string m_takenLetters;

	vector<string> m_wordList;	
	const int m_maxWordLength = 23;

	const int SCREEN_WIDTH;
};