#include "Hangman.h"


Hangman::Hangman() : SCREEN_WIDTH(50),
m_guessCount(0),
m_guessed(false),
m_takenLetters("")
{
	srand(time(NULL));

	fstream file("dictionary.txt", ios::in);

	if (file.good())
	{
		string line;
		while (getline(file, line))
			m_wordList.push_back(line);

		file.close();
	}
	else
	{
		m_wordList.insert(m_wordList.end(), { "violin", "orchestra", "book"});
	}

	//Filter out and prepare words
	for (int i = 0; i < m_wordList.size(); i++)
	{
		//Discard too long sentences
		if (m_wordList[i].length() > m_maxWordLength)
			m_wordList.erase(m_wordList.begin() + i);
		
		//Convert them to uppercase
		for (auto &i : m_wordList)
			for (auto &j : i)
				j = toupper(j);
	}

	Reset();
}

Hangman::~Hangman(){}

void Hangman::Main()
{
	/*******************************
	* Display
	*******************************/
	PrintLine("-", PrintMode::Fill, '+');
	if (m_guessCount > 2)	PrintLine("|", PrintMode::Center);				else PrintLine(" ", PrintMode::Fill);
	if (m_guessCount > 3)	PrintLine("|", PrintMode::Center);				else PrintLine(" ", PrintMode::Fill);
	if (m_guessCount > 4)	PrintLine("O", PrintMode::Center);				else PrintLine(" ", PrintMode::Fill);
	if (m_guessCount > 5)	PrintLine("/|\\", PrintMode::Center);			else PrintLine(" ", PrintMode::Fill);
	if (m_guessCount > 6)	PrintLine("/ \\", PrintMode::Center);			else PrintLine(" ", PrintMode::Fill);
	if (m_guessCount > 1)	PrintLine("-----------", PrintMode::Center);	else PrintLine(" ", PrintMode::Fill);
	if (m_guessCount > 0)	PrintLine("|         |", PrintMode::Center);	else PrintLine(" ", PrintMode::Fill);
	PrintLine("-", PrintMode::Fill);

	if (!m_guessed)
	{
		bool is_sentence = (m_word.find(' ') != string::npos ? true : false);

		if (m_guessCount < m_maxGuessCount)
		{
			PrintLine("Available Letters", PrintMode::Center);
			PrintLine(GetLetters('A', 'M'), PrintMode::Center);
			PrintLine(GetLetters('N', 'Z'), PrintMode::Center);

			PrintLine("-", PrintMode::Fill);
			if(is_sentence)
				PrintLine("Guess The Sentence", PrintMode::Center);
			else
				PrintLine("Guess The Word", PrintMode::Center);

			PrintLine(GetWord(), PrintMode::Center);
		}
		else
		{
			if(is_sentence)
				PrintLine("GAME OVER - The Sentence Was", PrintMode::Center);
			else
				PrintLine("GAME OVER - The Word Was", PrintMode::Center);

			PrintLine(GetCleanWord(), PrintMode::Center);
			PrintLine("Press [Enter] to Start Over", PrintMode::Center);
			Reset();
		}
	}
	else
	{
		PrintLine("You Guessed it Correctly!", PrintMode::Center);
		PrintLine(GetWord(), PrintMode::Center);
		PrintLine("-", PrintMode::Fill);

		PrintLine("Press [Enter] to Restart", PrintMode::Center);
		Reset();
	}

	PrintLine("-", PrintMode::Fill, '+');

	/*******************************
	* Input
	*******************************/
	char answerSign = toupper(_getch());
	if (m_guessCount < m_maxGuessCount && answerSign >= 'A' && answerSign <= 'Z')
	{
		//If character is not already in m_takenLetters
		if (m_takenLetters.find(answerSign) == string::npos)
		{
			m_takenLetters += answerSign;

			if (m_word.find(answerSign) == string::npos)	//If there's no such a sign in m_word
				m_guessCount++;
		}

		//Checking for win
		m_guessed = true;
		for (int i = 0; i < m_word.size(); i++)
		{
			for (int j = 0; j < m_takenLetters.size(); j++)
				if (m_takenLetters.find(m_word[i]) == string::npos && m_word[i] != ' ')
					m_guessed = false;
		}
	}
	system("cls");
}

string Hangman::GetWord()
{
	string temp = "";

	for (int i = 0; i < m_word.size(); i++)
		if (m_word[i] == ' ')
			temp += "  ";
		else
			temp += "_ ";


	for (int i = 0; i < m_takenLetters.size(); i++)	//We're looking for a match with a letter from m_takenLetters
	{
		for (int j = 0; j < temp.size(); j += 2)
			if (m_word[j / 2] == m_takenLetters[i])
				temp[j] = m_takenLetters[i];
	}

	return temp;
}

string Hangman::GetCleanWord()
{
	string temp = "";
	for (int i = 0; i < m_word.size(); i++)
	{
		if (m_word[i] != ' ')
		{
			temp += m_word[i];
			temp += " ";
		}
		else
			temp += m_word[i];
	}

	return temp;
}

void Hangman::Reset()
{
	m_guessCount = 0;
	m_guessed = false;
	m_takenLetters = "";

	//Picking random word
	int random = rand() % m_wordList.size();

	if(m_wordList.size() > 1)
		while (m_word == m_wordList[random])	//Checking for repetition
			random = rand() % m_wordList.size();

	m_word = m_wordList[random];
}

string Hangman::GetLetters(char l_from, char l_to)
{
	string temp = "";
	for (char i = l_from; i <= l_to; i++)
	{
		if (m_takenLetters.find(i) == string::npos)	//If the char i is not in m_takenLetters
		{
			temp += i;
			temp += " ";
		}
		else //If the char i is in m_takenLetters, We add only whitespace
			temp += "  ";
	}

	return temp;
}

void Hangman::PrintLine(string l_message, PrintMode l_mode, char l_boundariesSign)
{
	//In case of an empty string
	if (l_message.size() == 0)
		l_mode = PrintMode::Default;

	cout << l_boundariesSign;
	if (l_mode == PrintMode::Default)
	{
		if (l_message.size() < SCREEN_WIDTH - 2)	//-2 because boundaries char
		{
			cout << l_message;
			for (int i = 0; i < SCREEN_WIDTH - 2 - l_message.size(); i++)
				cout << " ";
		}
		else
		{
			cout << "[ERROR] Message is too long";
			for (int i = 0; i < SCREEN_WIDTH - string("[ERROR] Message is too long").size() - 2; i++)
				cout << " ";
		}
	}
	else if (l_mode == PrintMode::Center)
	{
		if (l_message.size() < SCREEN_WIDTH - 2)
		{
			int size = SCREEN_WIDTH - l_message.size() - 2;
			for (int i = 0; i < size / 2; i++)
				cout << " ";
			cout << l_message;
			for (int i = 0; i < size / 2; i++)
				cout << " ";

			//In case if we couldn't center (we'd be missing a single char). This is the fix
			if ((size % 2) != 0)
				cout << " ";
		}
		else
		{
			cout << "[ERROR] Message is too long";
			for (int i = 0; i < SCREEN_WIDTH - string("[ERROR] Message is too long").size() - 2; i++)
				cout << " ";
		}
	}
	else if (l_mode == PrintMode::Fill)
	{
		for (int i = 0; i < (SCREEN_WIDTH - 2)/l_message.size(); i++)
		{
			for (int i = 0; i < l_message.size(); i++)
				cout << l_message[i];
		}
	}
	cout << l_boundariesSign << endl;
}
