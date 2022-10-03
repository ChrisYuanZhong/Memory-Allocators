#include <iostream>
using namespace std;

int main()
{
	const char* strings[] = {
		"This",
		"is",
		"a",
		"test",
		NULL
	};

	const char sentence[] = "This is a test";

	if (strings[4] == NULL)
		return 1;
	
	return 0;
}