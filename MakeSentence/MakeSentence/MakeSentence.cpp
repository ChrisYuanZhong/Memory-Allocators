#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#else
#include <stdlib.h>
#endif // _DEBUG

char* MakeSentence(char** strings)
{
	int charCount = 0;
	int i, j;
	char* sentence = NULL;
	char* tempString = NULL;

	for (i = 0; strings[i] != NULL; i++)
	{
		for (j = 0; strings[i][j] != '\0'; j++)
		{
			tempString = (char*)realloc(sentence, (charCount + 1) * sizeof(char));
			sentence = tempString;

			sentence[charCount] = strings[i][j];
			charCount++;
		}
		tempString = (char*)realloc(sentence, (charCount + 1) * sizeof(char));
		sentence = tempString;

		sentence[charCount] = ' ';
		charCount++;
	}
	tempString = (char*)realloc(sentence, (charCount + 1) * sizeof(char));
	sentence = tempString;

	sentence[charCount - 1] = '.';
	sentence[charCount] = '\0';

	return sentence;
}

// This is only a simple unit test. 

int main(int i_argc, char** i_argl)
{
	int i, j;
	char ch;
	char** strings = { NULL };
	char** temp = { NULL };
	char* tempString = NULL;
	char* tempTempString = NULL;

	for (i = 0; ;i++)
	{
		printf("Enter a word for the sentence, empty to end: ");
		for (j = 0; (ch = getchar()) != '\n'; j++)
		{
			tempTempString = (char*)realloc(tempString, (j + 1) * sizeof(char));
			tempString = tempTempString;

			tempString[j] = ch;
		}

		//If the user inputs only Enter, then stop the loop.
		if (j == 0 && ch == '\n')
		{
			break;
		}

		tempTempString = (char*)realloc(tempString, (j + 1) * sizeof(char));
		tempString = tempTempString;

		tempString[j] = '\0';

		temp = (char**)realloc(strings, (i + 1) * sizeof(char*));
		strings = temp;

		strings[i] = tempString;
		
		tempString = (char*)malloc(0);
	}

	//realloc one more space to store a NULL, so that the for loop in MakeSentence() could know when to stop.
	temp = (char**)realloc(strings, (i + 1) * sizeof(char*));
	strings = temp;
	strings[i] = NULL;
	
	/*for (i = 0; strings[i] != NULL; i++)
	{
		cout << strings[i];
	}*/

	char* pSentence = MakeSentence(strings);

	printf("The Sentence is \"%s\"", pSentence);

	//freeing spaces
	for (i = 0; strings[i] != NULL; i++)
	{
		free(strings[i]);
	}
	free(strings[i]);
	free(strings);
	free(tempString);
	free(pSentence);

#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
}