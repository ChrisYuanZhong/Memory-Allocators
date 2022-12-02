#include <string.h>

bool IsPalindrome(const char* i_pString)
{
	int i = 0;
	int j = 0;

	for (; i_pString[i] != '\0'; i++);
	i--;
	for (; j < i; j++, i--)
	{
		if (i_pString[j] != i_pString[i])
			return false;
	}

	return true;
}

int main()
{
	char a[] = "madam";
	
	return IsPalindrome(a);
}