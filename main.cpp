#include "BattleShip.h"

// where all the magic happens
int main()
{
	beginLog();
	
	writeLog("Initializing Game.");

	// Set the randomized seed to the current time, this way we can get different outputs every time
	srand(time(0));

	// Display the welcome screen
	welcomeScreen();

	while (true)
	{
		

		// clear the screen
		system("CLS");

		// Afterwards, we should initialize everything
		initializeBoard();

		// And then we allow the player to manually place their ships
		manuallyPlaceShipsOnBoard();

		// Next we should randomly place enemy pieces on the board
		randomlyPlaceShipsOnBoard();

		// Now we randomly choose who goes first
		bool isPlayerTurn = selectWhoStarsFirst();

		// loop until one of the player's ships are all sunk
		while (!isWinner())
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

		// output stats now that the game is over
		outputStats();

		cout << "Game Over! " << (getPlayerHealth() > 1 ? "Player1" : "Player2") << " has won!" << endl;
		cout << "Enter Y if you would like to play again: ";

		char answer;

		cin >> answer;

		// if the player types in anything besides Y, exit game, otherwise reset everything
		if (!(answer == 'Y' || answer == 'y'))
		{
			writeLog("Closing game.");
			break;
		}
		else
		{
			writeLog("Starting new game.");
		}
	}

	endLog();

	return 0;
}