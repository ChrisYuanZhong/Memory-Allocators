#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#define MONSTERLIMIT 10
using namespace std;

int monsterCount = 0;
int nameCount = 0;

class Entity
{
public:
	Entity(void)
	{
		name = NULL;
		x = rand() % 101 - 50;	//Random number -50~50.
		y = rand() % 101 - 50;
	}

	//For creating player.
	Entity(char* name)
	{
		this->name = name;
		x = 0;
		y = 0;
	}

	void setName(char* name)
	{
		this->name = name;
	}

	void setCoordinates()
	{
		x = rand() % 101 - 50;	//Random number -50~50.
		y = rand() % 101 - 50;
	}

	void moveLeft()
	{
		x--;
	}

	void moveRight()
	{
		x++;
	}

	void moveUp()
	{
		y++;
	}

	void moveDown()
	{
		y--;
	}

	void printCoordinates()
	{
		cout << name << " is at " << "[ " << x << ", " << y << " ]" << endl;
	}

private:
	char *name;
	int x;
	int y;
};

Entity* monster = new Entity[monsterCount];	//TODO

void resizeMonster(int newSize)
{
	Entity* newMonster = new Entity[newSize];	//TODO
	for (int i = 0; i < newSize; i++)
		newMonster[i] = monster[i];
	delete[] monster;
	monster = newMonster;
}

void printMonsterInfo()
{
	for (int i = 0; i < monsterCount; i++)
		monster[i].printCoordinates();
}

void moveMonsters()
{
	int movement = 0;
	for (int i = 0; i < monsterCount; i++)
	{
		movement = rand() % 5;	//Generate a random number 0~4.
		switch (movement)
		{
		case 0:
			break;
		case 1:
			monster[i].moveLeft();
			break;
		case 2:
			monster[i].moveRight();
			break;
		case 3:
			monster[i].moveUp();
			break;
		case 4:
			monster[i].moveDown();
			break;
		}
	}
}

void newMonster()
{
	char tempName[] = "Monster0";
	monsterCount++;
	resizeMonster(monsterCount);
	char* newName = new char[9];	//TODO
	for (int i = 0; i < 9; i++)
	{
		newName[i] = tempName[i];
	}
	newName[7] = nameCount + '0';
	nameCount++;
	
	monster[monsterCount-1].setName(newName);
	monster[monsterCount-1].setCoordinates();

}

void killMonster()
{
	monsterCount--;
	nameCount--;
	resizeMonster(monsterCount);
}

int main()
{
	cout << "Welcome to MonsterChase!" << endl << endl;

	int number;
	int i, j;
	char tempName[100] = {'\0'};
	char ch;

	inputNumber:
	cout << "How many monsters to start: ";
	cin >> number;
	
	if (number <= 0)
	{
		cout << "Please Enter a Number > 0!" << endl;
		goto inputNumber;
	}
	/*else if (number > 5)
		monsterLimit = number;*/

	monsterCount = number;
	nameCount += number;
	resizeMonster(monsterCount);

	ch = getchar();	//Using getchar() to consume the '\n' user just inputted. Putting "ch" here so that there will be no warnings of ignoring the return value of getchar().
	for (i = 0; i < monsterCount; i++)
	{
		cout << "Enter a name for monster " << i << ": ";
		
		for (j = 0; (ch = getchar()) != '\n'; j++)
		{
			tempName[j] = ch;
		}
		tempName[j] = '\0';

		//Create a new name string that perfectly fits the inputted name.
		char* name = new char[j];	//TODO
		for (int k = 0; k <= j; k++)
		{
			name[k] = tempName[k];
		}

		monster[i].setName(name);
		monster[i].setCoordinates();
	}

	//Setting the name for player
	cout << "Enter a name for the player: ";
	for (j = 0; (ch = getchar()) != '\n'; j++)
	{
		tempName[j] = ch;
	}
	tempName[j] = '\0';
	
	//Create a new name string that perfectly fits the inputted name.
	char* name = new char[j];	//TODO
	for (int k = 0; k <= j; k++)
	{
		name[k] = tempName[k];
	}

	Entity player(name);

	//First game loop
	cout << endl << "Monsters:" << endl;
	printMonsterInfo();
	cout << endl << "Player:" << endl;
	player.printCoordinates();

	cout << "Press A to move left, D to move right, W to move up, S to move down or Q to quit" << endl << endl;

	//Game loop
	for (ch = '\0'; (ch = _getch()) != 'Q' && ch != 'q';)
	{
		if (ch == 'A' || ch == 'a')
			player.moveLeft();
		else if (ch == 'D' || ch == 'd')
			player.moveRight();
		else if (ch == 'W' || ch == 'w')
			player.moveUp();
		else if (ch == 'S' || ch == 's')
			player.moveDown();
		
		moveMonsters();
		
		i = rand() % 6;	//Generate a random number 0~5
		switch (i)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			if (monsterCount < MONSTERLIMIT)
				newMonster();
			break;
		case 5:
			if (monsterCount > 0)
				killMonster();
			break;
		}

		cout << "Monsters:" << endl;
		printMonsterInfo();
		cout << endl << "Player:" << endl;
		player.printCoordinates();

		cout << "Press A to move left, D to move right, W to move up, S to move down or Q to quit" << endl << endl;
	}

	//delete[] name;
	delete[] monster;
	
	return 0;
}