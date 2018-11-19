#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

// pre-defs for functions
void welcomeScreen();
void initializeBoard();
bool selectWhoStarsFirst();
void manuallyPlaceShipsOnBoard();
void randomlyPlaceShipsOnBoard();
bool checkIfSunkShip(bool player, char ship);
void displayBoard(char board[][10]);

void beginLog();
void endLog();
void writeLog(string data);
void outputCurrentMove(bool player, int x, int y, bool hit);
void outputStats();

void processMove(bool playerTurn, int x, int y);
bool checkShot(bool player, int x, int y);

bool isWinner();

void updateBoard(bool player, int x, int y, bool hit);

void processPlayerTurn();
void processEnemyTurn();

int getPlayerHealth();
int getComputerHealth();

#endif