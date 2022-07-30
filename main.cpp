#include <iostream>
#include "Hangman.h"

using namespace std;

int main()
{
	system("title Hangman");

	Hangman hangman;
	while (true)
		hangman.Main();

	cin.get();
	return 0;
}