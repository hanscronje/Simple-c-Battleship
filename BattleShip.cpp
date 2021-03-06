#include "BattleShip.h"

// pre-defs for variables

// The gameboard can be represented using a 1D variable
char playerboard[10][10];
char enemyboard[10][10];

// The enemy hit board is the same concept, except we cannot physically reveal the location of the enemy's pieces
// so the hit board is just a way to "mask" the enemy board between hit, miss, and unfired locations
char enemyhit[10][10];

// to easily calculate how many cells the player/computer have unhit, we store them in a variable
// eg: we know that a player has a total of 5 + 4 + 3 + 3 + 2 (or 17) cells since this just adds up the size of each ship
// everytime we check the board for a hit and it is validated, we display the results on the board as well as subtract 1
// from the corresponding variable. Once these values hit 0 we can determine a winner.
int playerCellsUnhit;
int computerCellsUnhit;

ofstream logfile;

// allows main.cpp to read how many cells each player has left
int getPlayerHealth() { return playerCellsUnhit; }
int getComputerHealth() { return computerCellsUnhit; }


void beginLog()
{
	logfile.open("battleship.log");
}

void endLog()
{
	logfile.close();
}

void writeLog(string data)
{
	logfile << data << endl;
}

void outputCurrentMove(bool player, int x, int y, bool hit)
{
	logfile << (player ? "Player 1's Turn: " : "Player 2's Turn: ") << x << "," << y << (hit ? " HIT!" : " MISS!") << endl;
}

void outputStats()
{
	int alive = 0;

	if (!checkIfSunkShip(true, 'c')) { alive++; }
	if (!checkIfSunkShip(true, 'b')) { alive++; }
	if (!checkIfSunkShip(true, 'r')) { alive++; }
	if (!checkIfSunkShip(true, 's')) { alive++; }
	if (!checkIfSunkShip(true, 'd')) { alive++; }

	logfile << "Player 1's Stats: " << "Ships alive: " << alive << " Cells intact: " << playerCellsUnhit << endl;

	alive = 0;

	if (!checkIfSunkShip(false, 'c')) { alive++; }
	if (!checkIfSunkShip(false, 'b')) { alive++; }
	if (!checkIfSunkShip(false, 'r')) { alive++; }
	if (!checkIfSunkShip(false, 's')) { alive++; }
	if (!checkIfSunkShip(false, 'd')) { alive++; }

	logfile << "Player 2's Stats: " << "Ships alive: " << alive << " Cells intact: " << computerCellsUnhit << endl;
}

void processMove(bool player, int x, int y)
{
	bool hit = checkShot(player, x, y);

	outputCurrentMove(player, x, y, hit);

	if (player)
	{
		updateBoard(player, x, y, hit);

		if (hit)
		{
			computerCellsUnhit--;

			cout << "Player 2's board: " << endl;
			displayBoard(enemyhit);
			cout << (x + 1) << "," << (y + 1) << " is a hit!" << endl;

			if (checkIfSunkShip(false, enemyboard[x][y]))
			{
				string type;
				switch (enemyboard[x][y])
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

				cout << "Enemy " << type << " has been sunk!" << endl;
			}
		}
		else
		{
			cout << "Player 2's board: " << endl;
			displayBoard(enemyhit);
			cout << (x + 1) << "," << (y + 1) << " is a miss!" << endl;
		}
	}
	else
	{
		updateBoard(player, x, y, hit);

		if (hit)
		{
			char c = playerboard[x][y];

			playerCellsUnhit--;

			cout << "Player 1's board: " << endl;
			displayBoard(playerboard);

			cout << (x + 1) << "," << (y + 1) << " is a hit!" << endl;

			if (checkIfSunkShip(true, playerboard[x][y]))
			{
				string type;
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

				cout << "Player " << type << " has been sunk!" << endl;
			}
		}
		else
		{
			cout << "Player 1's board: " << endl;
			displayBoard(playerboard);

			cout << (x + 1) << "," << (y + 1) << " is a miss!" << endl;
		}
	}
}

bool checkShot(bool player, int x, int y)
{
	if (player)
	{
		return enemyhit[x - 1][y - 1] == '-';
	}
	else
	{
		return playerboard[x][y] != '-';
	}
}

bool isWinner()
{
	return playerCellsUnhit < 1 || computerCellsUnhit < 1;
}

void updateBoard(bool player, int x, int y, bool hit)
{
	if (player)
	{
		enemyhit[x][y] = hit ? '*' : 'm';
	}
	else
	{
		playerboard[x][y] = hit ? '*' : 'm';
	}
}

// converts a 2D point into a 1D point for array purposes
int translatePoint(int x, int y)
{
	return x + (y * 10);
}

// draws a board to the console
void displayBoard(char board[][10])
{
	// nested for-loop
	// the outer loop should be cycling through the Y side of the array
	// the inner loop should be cycling through the X side of the array
	cout << "  1 2 3 4 5 6 7 8 9 10";
	for (int i = 0; i < 10; i++)
	{
		cout << endl << (i + 1) << (i < 9 ? " " : "");
		for (int j = 0; j < 10; j++)
		{
			cout << board[j][i] << " ";
		}
		// every time we finish a full line in the board, we write to the next line
	}

	cout << endl;
}

// displays the welcome screen as well as rules
void welcomeScreen()
{
	// Display the welcome message
	// By using endl we create line endings and we just continue the output to the console using "<<" for every new
	// variable we add.
	cout << "***** Welcome to Battleship! *****" << endl <<
		endl <<
		"Rules of the Game :" << endl <<
		"1. This is a two player game." << endl <<
		"2. Player1 is you and Player2 is the computer." << endl <<
		"3. Etc. (You need to list the rest of the rules here.)" << endl <<
		endl <<
		"Hit enter to start the game!";


	// cin.ignore() essentially ignores all input until "Enter" is pressed, this works perfectly for our game start requirement
	cin.ignore();
}

// initializes the gameboard
void initializeBoard()
{
	// As mentioned above, each player has a maximum of 17 cells that can be considered "unhit" for their ships, so we reset those variables
	playerCellsUnhit = 17;
	computerCellsUnhit = 17;

	// Set each gameboard tile to '-'
	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i < 10; i++)
		{
			playerboard[i][j] = '-'; enemyboard[i][j] = '-'; enemyhit[i][j] = '-';
		}
	}
}

// selects if the player or computer goes first
bool selectWhoStarsFirst()
{
	// this is a simple check, if rand() is an even number, the player goes first, if it's odd, the AI goes first
	bool isPlayerTurn = rand() % 2 == 0;

	cout << (isPlayerTurn ? "Player1" : "Player2 (Computer)") << " has been selected to go first." << endl;
	cin.ignore();

	return isPlayerTurn;
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
		if (player)
		{
			playerboard[x][y] = letter;
		}
		else
		{
			enemyboard[x][y] = letter;
		}

		if (endx > x)
		{
			x++;
		}
		else if (endx < x)
		{
			x--;
		}
		else if (endy > y)
		{
			y++;
		}
		else if (endy < y)
		{
			y--;
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

	cout << "*****       Place your Ships!     *****" << endl <<
		"* A cell should be entered as such: x-pos y-pos direction" << endl <<
		"* the x-pos is the position from left to right, the y-pos is the position from top to bottom" << endl <<
		"* and the direction is either North, South, East, West, except abbreviated to the first letter in the direction" << endl << endl <<
		"** 1 5 S places the ship at left most column, the 5th row from the top and going downwards or to the south" << endl << endl;

	while (!isValid)
	{
		cout << "Please enter the cell of the to place the 5 cell long Carrier and a direction" << endl;

		cin >> x >> y >> direction;

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
			cout << "The positioning of your unit is not valid!" << endl;
		}
	}

	markShip('c', 5, x, y, endx, endy, true);

	isValid = false;

	while (!isValid)
	{
		cout << "Please enter the cell of the to place the 4 cell long Battleship and a direction" << endl;

		cin >> x >> y >> direction;

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
			cout << "The positioning of your unit is not valid!" << endl;
		}
	}

	markShip('b', 4, x, y, endx, endy, true);

	isValid = false;

	while (!isValid)
	{
		cout << "Please enter the cell of the to place the 3 cell long Cruiser and a direction" << endl;

		cin >> x >> y >> direction;

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
			cout << "The positioning of your unit is not valid!" << endl;
		}
	}

	markShip('r', 3, x, y, endx, endy, true);

	isValid = false;

	while (!isValid)
	{
		cout << "Please enter the cell of the to place the 3 cell long Submarine and a direction" << endl;

		cin >> x >> y >> direction;

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
			cout << "The positioning of your unit is not valid!" << endl;
		}
	}

	markShip('s', 3, x, y, endx, endy, true);

	isValid = false;

	while (!isValid)
	{
		cout << "Please enter the cell of the to place the 2 cell long Destroyer and a direction" << endl;

		cin >> x >> y >> direction;

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
			cout << "The positioning of your unit is not valid!" << endl;
		}
	}

	markShip('d', 2, x, y, endx, endy, true);

	cin.ignore();
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

		int tx = x;
		int ty = y;

		for (int i = 0; i < size; i++)
		{
			if (enemyboard[tx][ty] != '-')
			{
				canMark = -1;
			}

			if (endx > x)
			{
				tx++;
			}
			else if (endx < x)
			{
				tx--;
			}
			else if (endy > y)
			{
				ty++;
			}
			else if (endy < y)
			{
				ty--;
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

	cout << "Player2 (Computer's) board has been generated." << endl;
}

// a crude way of checking if a specific ship has been sunk
// we check each and every cell to see if a specific ship exists in the cell, if it does then we return false, otherwise true
bool checkIfSunkShip(bool player, char ship)
{
	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i < 10; i++)
		{
			if ((player && playerboard[i][j] == ship) || (!player && enemyboard[i][j] == ship && enemyhit[i][j] == '-'))
			{
				return false;
			}
		}
	}

	return true;
}

// processes the player's turn
void processPlayerTurn()
{
	system("CLS");

	cout << "Player 1's board: " << endl;
	displayBoard(playerboard);

	cout << "Press Enter to continue" << endl;
	cin.ignore();

	int x, y;

	system("CLS");

	cout << "Player 2's board: " << endl;
	displayBoard(enemyhit);

	while (true)
	{
		cout << "Enter a target:";
		cin >> x >> y;
		cin.ignore();

		system("CLS");

		// check to see if position is not already hit
		if (enemyhit[x - 1][y - 1] == '-')
		{
			break;
		}

		cout << "Player 2's board: " << endl;
		displayBoard(enemyhit);

		cout << "You cannot hit a location previously hit before!" << endl;
	}

	system("CLS");

	x--;
	y--;

	processMove(true, x, y);	

	cout << "Press Enter to continue" << endl;
	cin.ignore();
}

// processes the enemy's turn
void processEnemyTurn()
{
	system("CLS");

	cout << "Player 2's board: " << endl;
	displayBoard(enemyhit);

	int x = -1, y = -1;

	// randomly select a position that has not yet been hit yet
	while (x < 0 || playerboard[x][y] == '*' || playerboard[x][y] == 'm')
	{
		x = rand() % 10;
		y = rand() % 10;
	}

	system("CLS");

	processMove(false, x, y);

	cout << "Press Enter to continue" << endl;
	cin.ignore();
}