// Created by Johannes Cronje 2018, all rights reserved.

#include "stdafx.h"
#include <string>
#include <iostream>
#include <ctime>

// pre-defs for variables

// The gameboard can be represented using a 1D variable
// by using the formula id = x + (y * 10) we can translate 2D points into 1D points
char *playerboard = new char[100];
char *enemyboard = new char[100];

// The enemy hit board is the same concept, except we cannot physically reveal the location of the enemy's pieces
// so the hit board is just a way to "mask" the enemy board between hit, miss, and unfired locations
char *enemyhit = new char[100];

// to easily calculate how many cells the player/computer have unhit, we store them in a variable
// eg: we know that a player has a total of 5 + 4 + 3 + 3 + 2 (or 17) cells since this just adds up the size of each ship
// everytime we check the board for a hit and it is validated, we display the results on the board as well as subtract 1
// from the corresponding variable. Once these values hit 0 we can determine a winner.
int playerCellsUnhit;
int computerCellsUnhit;

// Essentially checks if the player goes first or not
bool isPlayerTurn;

// pre-defs for functions
void welcomeScreen();
void initializeBoard();
void selectWhoGoesFirst();
void manuallyPlaceShipsOnBoard();
void randomlyPlaceShipsOnBoard();
bool checkIfSunkShip(bool player, char ship);

void processPlayerTurn();
void processEnemyTurn();

// converts a 2D point into a 1D point for array purposes
int translatePoint(int x, int y)
{
	return x + (y * 10);
}

// draws a board to the console
void displayBoard(char *board)
{
	// nested for-loop
	// the outer loop should be cycling through the Y side of the array
	// the inner loop should be cycling through the X side of the array
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			std::cout << board[translatePoint(j, i)];
		}
		// every time we finish a full line in the board, we write to the next line
		std::cout << std::endl;
	}
}

// where all the magic happens
int main()
{
	// Set the randomized seed to the current time, this way we can get different outputs every time
	srand(std::time(0));

	// Display the welcome screen
	welcomeScreen();

	while (true)
	{
		// clear the screen
		std::system("CLS");

		// Afterwards, we should initialize everything
		initializeBoard();

		// And then we allow the player to manually place their ships
		manuallyPlaceShipsOnBoard();

		// Next we should randomly place enemy pieces on the board
		randomlyPlaceShipsOnBoard();

		// Now we randomly choose who goes first
		selectWhoGoesFirst();

		// process game here

		// loop until one of the player's ships are all sunk
		while (playerCellsUnhit > 0 && computerCellsUnhit > 0)
		{

			// Process turns, based on who's turn it is
			if (isPlayerTurn)
			{
				processPlayerTurn();
			}
			else
			{
				processEnemyTurn();
			}

			// flip the turn variable
			isPlayerTurn = !isPlayerTurn;
		}

		std::cout << "Game Over! " << (playerCellsUnhit > 1 ? "Player1" : "Player2") << " has won!" << std::endl;
		std::cout << "Enter Y if you would like to play again: ";

		char answer;

		std::cin >> answer;

		// if the player types in anything besides Y, exit game, otherwise reset everything
		if (!(answer == 'Y' || answer == 'y'))
		{
			break;
		}
	}

    return 0;
}

// displays the welcome screen as well as rules
void welcomeScreen()
{
	// Display the welcome message
	// By using std::endl we create line endings and we just continue the output to the console using "<<" for every new
	// variable we add.
	std::cout << "***** Welcome to Battleship! *****" << std::endl <<
		std::endl <<
		"Rules of the Game :" << std::endl <<
		"1. This is a two player game." << std::endl <<
		"2. Player1 is you and Player2 is the computer." << std::endl <<
		"3. Etc. (You need to list the rest of the rules here.)" << std::endl <<
		std::endl <<
		"Hit enter to start the game!";


	// std::cin.ignore() essentially ignores all input until "Enter" is pressed, this works perfectly for our game start requirement
	std::cin.ignore();
}

// initializes the gameboard
void initializeBoard()
{
	// As mentioned above, each player has a maximum of 17 cells that can be considered "unhit" for their ships, so we reset those variables
	playerCellsUnhit = 17;
	computerCellsUnhit = 17;

	// Set each gameboard tile to '-'
	for (int i = 0; i < 100; i++) { playerboard[i] = '-'; enemyboard[i] = '-'; enemyhit[i] = '-'; }
}

// selects if the player or computer goes first
void selectWhoGoesFirst()
{
	// this is a simple check, if rand() is an even number, the player goes first, if it's odd, the AI goes first
	isPlayerTurn = rand() % 2 == 0;

	std::cout << (isPlayerTurn ? "Player1" : "Player2 (Computer)") << " has been selected to go first." << std::endl;
	std::cin.ignore();
}

// Marks a ship onto the game board
void markShip(char letter, int shipSize, int x, int y, int endx, int endy, bool player)
{
	// normalize the positions from player friendly to being grid friendly
	// eg: 1,1 being the upper left corner in player's eyes but 0,0 being the upper right corner in the grid's eyes
	x--;
	y--;
	endx--;
	endy--;

	for (int i = 0; i < shipSize; i++)
	{
		int id;

		if (endx > x)
		{
			id = translatePoint(x + i, y);
		}
		else if (endx < x)
		{
			id = translatePoint(x - i, y);
		}
		else if (endy > y)
		{
			id = translatePoint(x, y + i);
		}
		else if (endy < y)
		{
			id = translatePoint(x, y - i);
		}

		if (player)
		{
			playerboard[id] = letter;
		}
		else
		{
			enemyboard[id] = letter;
		}
	}
}

// Allows the player to manually select the locations of their ship
void manuallyPlaceShipsOnBoard()
{
	// variables to store user input
	int x, y;
	char direction;

	// used to calculate if the positioning is valid
	int endx, endy;

	// used for looping purposes until a valid position was given
	bool isValid = false;

	std::cout << "*****       Place your Ships!     *****" << std::endl <<
		"* A cell should be entered as such: x-pos y-pos direction" << std::endl <<
		"* the x-pos is the position from left to right, the y-pos is the position from top to bottom" << std::endl <<
		"* and the direction is either North, South, East, West, except abbreviated to the first letter in the direction" << std::endl << std::endl <<
		"** 1 5 S places the ship at left most column, the 5th row from the top and going downwards or to the south" << std::endl << std::endl;

	while (!isValid)
	{
		std::cout << "Please enter the cell of the to place the 5 cell long Carrier and a direction" << std::endl;

		std::cin >> x >> y >> direction;

		switch (direction)
		{
		case 'N':
		case 'n':
			endx = x;
			endy = y - 5;
			break;
		case 'S':
		case 's':
			endx = x;
			endy = y + 5;
			break;
		case 'E':
		case 'e':
			endx = x - 5;
			endy = y;
			break;
		case 'W':
		case 'w':
			endx = x + 5;
			endy = y;
		default:
			break;
		}

		isValid = !(x < 1 || y < 1 || endx < 1 || endy < 1 || x > 10 || y > 10 || endx > 10 || endy > 10);

		if (!isValid)
		{
			std::cout << "The positioning of your unit is not valid!" << std::endl;
		}
	}

	markShip('c', 5, x, y, endx, endy, true);

	isValid = false;

	while (!isValid)
	{
		std::cout << "Please enter the cell of the to place the 4 cell long Battleship and a direction" << std::endl;

		std::cin >> x >> y >> direction;

		switch (direction)
		{
		case 'N':
		case 'n':
			endx = x;
			endy = y - 4;
			break;
		case 'S':
		case 's':
			endx = x;
			endy = y + 4;
			break;
		case 'E':
		case 'e':
			endx = x - 4;
			endy = y;
			break;
		case 'W':
		case 'w':
			endx = x + 4;
			endy = y;
		default:
			break;
		}

		isValid = !(x < 1 || y < 1 || endx < 1 || endy < 1 || x > 10 || y > 10 || endx > 10 || endy > 10);

		if (!isValid)
		{
			std::cout << "The positioning of your unit is not valid!" << std::endl;
		}
	}

	markShip('b', 4, x, y, endx, endy, true);

	isValid = false;

	while (!isValid)
	{
		std::cout << "Please enter the cell of the to place the 3 cell long Cruiser and a direction" << std::endl;

		std::cin >> x >> y >> direction;

		switch (direction)
		{
		case 'N':
		case 'n':
			endx = x;
			endy = y - 3;
			break;
		case 'S':
		case 's':
			endx = x;
			endy = y + 3;
			break;
		case 'E':
		case 'e':
			endx = x - 3;
			endy = y;
			break;
		case 'W':
		case 'w':
			endx = x + 3;
			endy = y;
		default:
			break;
		}

		isValid = !(x < 1 || y < 1 || endx < 1 || endy < 1 || x > 10 || y > 10 || endx > 10 || endy > 10);

		if (!isValid)
		{
			std::cout << "The positioning of your unit is not valid!" << std::endl;
		}
	}

	markShip('r', 3, x, y, endx, endy, true);

	isValid = false;

	while (!isValid)
	{
		std::cout << "Please enter the cell of the to place the 3 cell long Submarine and a direction" << std::endl;

		std::cin >> x >> y >> direction;

		switch (direction)
		{
		case 'N':
		case 'n':
			endx = x;
			endy = y - 3;
			break;
		case 'S':
		case 's':
			endx = x;
			endy = y + 3;
			break;
		case 'E':
		case 'e':
			endx = x - 3;
			endy = y;
			break;
		case 'W':
		case 'w':
			endx = x + 3;
			endy = y;
		default:
			break;
		}

		isValid = !(x < 1 || y < 1 || endx < 1 || endy < 1 || x > 10 || y > 10 || endx > 10 || endy > 10);

		if (!isValid)
		{
			std::cout << "The positioning of your unit is not valid!" << std::endl;
		}
	}

	markShip('s', 3, x, y, endx, endy, true);

	isValid = false;

	while (!isValid)
	{
		std::cout << "Please enter the cell of the to place the 2 cell long Destroyer and a direction" << std::endl;

		std::cin >> x >> y >> direction;

		switch (direction)
		{
		case 'N':
		case 'n':
			endx = x;
			endy = y - 2;
			break;
		case 'S':
		case 's':
			endx = x;
			endy = y + 2;
			break;
		case 'E':
		case 'e':
			endx = x - 2;
			endy = y;
			break;
		case 'W':
		case 'w':
			endx = x + 2;
			endy = y;
		default:
			break;
		}

		isValid = !(x < 1 || y < 1 || endx < 1 || endy < 1 || x > 10 || y > 10 || endx > 10 || endy > 10);

		if (!isValid)
		{
			std::cout << "The positioning of your unit is not valid!" << std::endl;
		}
	}

	markShip('d', 2, x, y, endx, endy, true);

	std::cin.ignore();
}

// places an enemy piece based on size and direction
void placeEnemyPiece(int size, int direction, char letter)
{
	int x, y, endx, endy;
	int canMark = 0;

	while (true)
	{
		switch (direction)
		{
		case 0:
			// Verticle

			// x-position doesn't matter when placing vertically
			x = (rand() % 10) + 1;
			endx = x;

			// y-position is more sensitive in this case
			// we cannot place the piece too close to the top or the piece will go out of bounds
			// therefore we crop the available position to stay within the board
			y = (rand() % (10 - size)) + size + 1;
			endy = y - size;
			break;

		case 1:
			// Horizontal

			// y-position doesn't matter when placing horizontally
			y = (rand() % 10) + 1;
			endy = y;

			// x-position is more sensitive in this case
			// we cannot place the piece too close to the left or the piece will go out of bounds
			// therefore we crop the available position to stay within the board
			x = (rand() % (10 - size)) + size + 1;
			endx = x - size;

			break;
		}

		for (int i = 0; i < size; i++)
		{
			int id;

			if (endx > x)
			{
				id = translatePoint(x + i - 1, y - 1);
			}
			else if (endx < x)
			{
				id = translatePoint(x - i - 1, y - 1);
			}
			else if (endy > y)
			{
				id = translatePoint(x - 1, y + i - 1);
			}
			else if (endy < y)
			{
				id = translatePoint(x - 1, y - i - 1);
			}

			if (enemyboard[id] != '-')
			{
				canMark = -1;
			}
		}

		if (canMark == 0)
		{
			break;
		}
		else
		{
			canMark = 0;
		}
	}

	markShip(letter, size, x, y, endx, endy, false);
}

// Randomly places enemy pieces on the map
void randomlyPlaceShipsOnBoard()
{
	placeEnemyPiece(5, rand() % 2, 'c');
	placeEnemyPiece(4, rand() % 2, 'b');
	placeEnemyPiece(3, rand() % 2, 'r');
	placeEnemyPiece(3, rand() % 2, 's');
	placeEnemyPiece(2, rand() % 2, 'd');

	std::cout << "Player2 (Computer's) board has been generated." << std::endl;
}

// a crude way of checking if a specific ship has been sunk
// we check each and every cell to see if a specific ship exists in the cell, if it does then we return false, otherwise true
bool checkIfSunkShip(bool player, char ship)
{
	char *board = player ? playerboard : enemyboard;
	for (int i = 0; i < 100; i++)
	{
		if (board[i] == ship)
		{
			return false;
		}
	}

	return true;
}

// processes the player's turn
void processPlayerTurn()
{
	std::system("CLS");

	std::cout << "Player 1's board: " << std::endl;
	displayBoard(playerboard);

	std::cout << "Press Enter to continue" << std::endl;
	std::cin.ignore();

	int x, y;

	std::system("CLS");

	std::cout << "Player 2's board: " << std::endl;
	displayBoard(enemyhit);

	std::cout << "Enter a target:";
	std::cin >> x >> y;
	std::cin.ignore();

	std::system("CLS");

	int id = translatePoint(x - 1, y - 1);

	// Record shot here

	if (enemyboard[id] != '-')
	{
		enemyhit[id] = '*';
		computerCellsUnhit--;

		std::cout << "Player 2's board: " << std::endl;
		displayBoard(enemyhit);
		std::cout << x << "," << y << " is a hit!" << std::endl;

		if (checkIfSunkShip(false, enemyboard[id]))
		{
			std::string type;
			switch (enemyboard[id])
			{
			case 'c':
				type = "Carrier";
				break;
			case 'b':
				type = "Battleship";
				break;
			case 'r':
				type = "Cruiser";
				break;
			case 's':
				type = "Submarine";
				break;
			case 'd':
				type = "Destroyer";
				break;
			default:
				type = "Ship";
				break;
			}

			std::cout << "Enemy " << type << " has been sunk!" << std::endl;
		}
	}
	else
	{
		enemyhit[id] = 'm';
		std::cout << "Player 2's board: " << std::endl;
		displayBoard(enemyhit);
		std::cout << x << "," << y << " is a miss!" << std::endl;
	}

	std::cout << "Press Enter to continue" << std::endl;
	std::cin.ignore();
}

// processes the enemy's turn
void processEnemyTurn()
{
	std::system("CLS");

	std::cout << "Player 2's board: " << std::endl;
	displayBoard(enemyhit);

	int x = -1, y = -1;

	// randomly select a position that has not yet been hit yet
	while (x < 0 || playerboard[translatePoint(x, y)] == '*' || playerboard[translatePoint(x, y)] == 'm')
	{
		x = rand() % 10;
		y = rand() % 10;
	}

	int id = translatePoint(x, y);

	// Record shot here

	std::system("CLS");

	if (playerboard[id] != '-')
	{
		char c = playerboard[id];

		playerboard[id] = '*';
		playerCellsUnhit--;

		std::cout << "Player 1's board: " << std::endl;
		displayBoard(playerboard);

		std::cout << (x + 1) << "," << (y + 1) << " is a hit!" << std::endl;

		if (checkIfSunkShip(true, playerboard[id]))
		{
			std::string type;
			switch (c)
			{
			case 'c':
				type = "Carrier";
				break;
			case 'b':
				type = "Battleship";
				break;
			case 'r':
				type = "Cruiser";
				break;
			case 's':
				type = "Submarine";
				break;
			case 'd':
				type = "Destroyer";
				break;
			default:
				type = "Ship";
				break;
			}

			std::cout << "Player " << type << " has been sunk!" << std::endl;
		}
	}
	else
	{
		playerboard[id] = 'm';
		std::cout << "Player 1's board: " << std::endl;
		displayBoard(playerboard);

		std::cout << x << "," << y << " is a miss!" << std::endl;
	}

	std::cout << "Press Enter to continue" << std::endl;
	std::cin.ignore();
}