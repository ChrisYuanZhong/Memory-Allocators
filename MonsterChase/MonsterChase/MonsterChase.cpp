#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#define SET_NAME(name) test##name
using namespace std;

int monsterLimit = 5;
int monsterCount = 0;

class Entity
{
public:
	Entity(void)
	{
		name = NULL;
		x = rand() % 101 - 50;	//Random number between -50 and 50.
		y = rand() % 101 - 50;
	}
	Entity(char *name, bool property)
	{
		this->name = name;

		//Property means the role of the entity, 0 means monster, 1 means player.
		if (property = 1)
		{
			x = 0;
			y = 0;
		}
		else
		{
			x = rand() % 101 - 50;	//Random number between -50 and 50.
			y = rand() % 101 - 50;
		}
	}

	void setName(char* name)
	{
		this->name = name;
	}

	void setCoordinates(int x, int y)
	{
		this->x = x;
		this->y = y;
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

void printMonsterInfo(Entity *monster)
{
	for (int i = 0; i < monsterCount; i++)
		monster[i].printCoordinates();
}

void moveMonsters(Entity *monster)
{
	int movement = 0;
	for (int i = 0; i < monsterCount; i++)
	{
		movement = rand() % 5;
		switch (movement)
		{
		case 0: break;
		case 1: monster[i].moveLeft(); break;
		case 2: monster[i].moveRight(); break;
		case 3: monster[i].moveUp(); break;
		case 4: monster[i].moveDown(); break;
		}
	}
}

void newMonster(Entity* monster)
{
	//monster[monsterCount++].setName();
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
	else if (number > 5)
		monsterLimit = number;

	monsterCount = number;
	Entity *monster = new Entity[monsterLimit];	//TODO

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
		char *name = new char[j];	//TODO
		for (int k = 0; k <= j; k++)
		{
			name[k] = tempName[k];
		}

		monster[i].setName(name);
	}

	//Setting the name for player
	cout << "Enter a name for the player: ";
	for (j = 0; (ch = getchar()) != '\n'; j++)
	{
		tempName[j] = ch;
	}
	tempName[j] = '\0';
	
	//Create a new name string that perfectly fits the inputted name.
	char *name = new char[j];	//TODO
	for (int k = 0; k <= j; k++)
	{
		name[k] = tempName[k];
	}
	//free(tempName);

	Entity player(name, 1);

	//First game loop
	cout << endl << "Monsters:" << endl;
	printMonsterInfo(monster);
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

		moveMonsters(monster);
		cout << "Monsters:" << endl;
		printMonsterInfo(monster);
		cout << endl << "Player:" << endl;
		player.printCoordinates();

		cout << "Press A to move left, D to move right, W to move up, S to move down or Q to quit" << endl << endl;
	}

	//delete[] name;
	delete[] monster;
	
	return 0;
}