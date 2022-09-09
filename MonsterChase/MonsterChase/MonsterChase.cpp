#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <Engine.h>
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
		coordinates.setx(rand() % 101 - 50);	//Random number -50~50.
		coordinates.sety(rand() % 101 - 50);
	}

	//For creating player.
	Entity(char* name)
	{
		this->name = name;
	}

	~Entity()
	{
		delete[] name;
	}

	void setName(char* name)
	{
		this->name = name;
	}

	void setCoordinates()
	{
		coordinates.setx(rand() % 101 - 50);	//Random number -50~50.
		coordinates.sety(rand() % 101 - 50);
	}

	void moveLeft()
	{
		coordinates -= Point2D(1, 0);
	}

	void moveRight()
	{
		coordinates += Point2D(1, 0);
	}

	void moveUp()
	{
		coordinates += Point2D(0, 1);
	}

	void moveDown()
	{
		coordinates -= Point2D(0, 1);
	}

	void printCoordinates()
	{
		cout << name << " is at " << "[ " << coordinates.getx() << ", " << coordinates.gety() << " ]" << endl;
	}

private:
	char *name;
	Point2D coordinates = Point2D(0, 0);
};

Entity* monster = new Entity[monsterCount];

void enlargeMonster(int newSize)
{
	Entity* newMonster = new Entity[newSize];
	for (int i = 0; i < newSize; i++)
		newMonster[i] = monster[i];
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
	enlargeMonster(monsterCount);
	char* newName = new char[9];
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
	monster[monsterCount - 1].~Entity();
	monsterCount--;
	nameCount--;
}

int main()
{
	cout << "Welcome to MonsterChase!" << endl << endl;

	int number;
	int i, j;
	char* tempName= new char[100] {'\0'};
	char ch;

	inputNumber:
	cout << "How many monsters to start: ";
	cin >> number;
	
	if (number <= 0)
	{
		cout << "Please Enter a Number > 0!" << endl;
		goto inputNumber;
	}

	monsterCount = number;
	nameCount += number;
	enlargeMonster(monsterCount);

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
		char* name = new char[j+1];
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
	char* name = new char[j+1];
	for (int k = 0; k <= j; k++) 
	{
		name[k] = tempName[k];
	}

	delete[] tempName;
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

	//Releasing allocated memory
	for (i = 0; i < monsterCount; i++)
		monster[i].~Entity();

	return 0;
}