#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

int main()
{
	int letterCount = 0;
	int j, k;
	char ch;
	char* sentence = (char*) malloc(0);
	char* temp = new char[100] {'\0'};

	for (; ;)
	{
		cout << "Enter a word for the sentence, empty to end: ";
		for (j = 0; (ch = getchar()) != '\n'; j++)
		{
			temp[letterCount + j] = ch;
		}
		if (j == 0 && ch == '\n')
		{
			break;
		}
		temp[letterCount + j] = ' ';
		letterCount += j + 1;
	}
	sentence = (char*) malloc(letterCount * sizeof(char));
	for (k = 0; k < letterCount; k++)
	{
		sentence[k] = temp[k];
	}
	sentence[letterCount - 1] = '\0';
	cout << "The sentence is \"" << sentence << ".\"";

	free(sentence);
	delete[] temp;

	return 0;
}