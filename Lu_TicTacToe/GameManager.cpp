
// TODO disabled warnings should be moved to a global header file (deferred)
#pragma warning(disable : 6011) // dereferencing NULL pointer <name>, (I used assert instead of the "if" check this warning prefers)

#include "GameManager.h"
#include <iostream>

GameManager::GameManager()
{

}

GameManager::~GameManager()
{

}

void GameManager::Create() 
{
	const GameManager* pManager = GameManager::GetInstance();
	assert(pManager);
}

void GameManager::Shutdown()
{
	//TODO does anything need to be cleaned up on shut down? (deferred)
}

void GameManager::Update()
{
	GameManager* pManager = GameManager::GetInstance();
	assert(pManager != nullptr);

	switch (pManager->gameMode)
	{
	case GAME_MODE::Start:
		pManager->ShowStartText();
		break;
	case GAME_MODE::InProgress:
		pManager->ShowInProgressText();
		break;
	case GAME_MODE::End:
		pManager->ShowEndText();
		break;
	default:
		assert(false);
	}
}

bool GameManager::GetQuitStatus()
{
	GameManager* pManager = GameManager::GetInstance();
	assert(pManager != nullptr);
	return pManager->isQuit;
}

void GameManager::InitializeGame()
{
	GameManager* pManager = GameManager::GetInstance();
	assert(pManager != nullptr);

	pManager->currentNumTurns = 0;

	// reset the game board
	int id = 0;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLUMN; j++)
		{
			pManager->boardPositions[id].row = i;
			pManager->boardPositions[id].column = j;
			pManager->boardPositions[id].markType = MARK_TYPE::NONE;
			id++;
		}
	}

	pManager->isPlayer1Turn = true;
	pManager->UpdateGameMode(GAME_MODE::InProgress);
}

void GameManager::ShowStartText()
{
	printf("Welcome! \n");
	printf("Tic Tac Toe \n");
	std::system("pause"); // says "press any key to continue"

	InitializeGame();
	UpdateGameMode(GAME_MODE::InProgress);
}

void GameManager::ShowInProgressText()
{
	GameManager* pManager = GameManager::GetInstance();
	assert(pManager != nullptr);

	PrintGameBoard();

	if (pManager->isPlayer1Turn)
	{
		printf("PLAYER 1 (X): Place your mark \n");
	}
	else
	{
		printf("PLAYER 2 (O): Place your mark \n");
	}

	bool isValid = false;
	int numSpots = ROW * COLUMN - 1;
	int playerInput = 0;

	printf("Enter a number between 0 and %d\n", numSpots);

	while (!isValid)
	{
		std::cin >> playerInput;
		if (!std::cin)
		{
			// user input was non numerical values
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<int>::max(), '\n'); //Stops the printf line below from repeating
			printf("Invalid input. Enter a number between 0 and %d\n", numSpots);

		}
		else if (playerInput >= 0 && playerInput <= numSpots) 
		{
			//user input is valid
			if (pManager->boardPositions[playerInput].markType != MARK_TYPE::NONE)
			{
				printf("%d is already marked, please select another position\n", playerInput);
			}
			else 
			{
				isValid = true;
			}
		}
		else 
		{
			// user input was a numerical value, but it is out of range
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<int>::max(), '\n'); //Stops the printf line below from repeating
			printf("Invalid input. Enter a number between 0 and %d\n", numSpots);
		}
	}


	UpdateGameBoard(playerInput);

	if (!pManager->isGameOver(playerInput)) 
	{
		pManager->isPlayer1Turn = !pManager->isPlayer1Turn;
		pManager->currentNumTurns++;
	}
	else 
	{
		pManager->UpdateGameMode(GAME_MODE::End);
	}

}

void GameManager::ShowEndText()
{
	GameManager* pManager = GameManager::GetInstance();
	assert(pManager != nullptr);

	PrintGameBoard();

	printf("Game Over! \n");

	switch (pManager->gameResult)
	{
	case GAME_RESULT::Player1:
		printf("Player 1 (X) WINS!\n");
		break;
	case GAME_RESULT::Player2:
		printf("Player 2 (O) WINS!\n");
		break;
	case GAME_RESULT::Draw:
		printf("TIED GAME!\n");
		break;
	case GAME_RESULT::NONE:
		assert(false);
		break;
	}

	printf("\nSCORES: Player1: %d | Player2: %d | Draws: %d\n", pManager->numP1Wins, pManager->numP2Wins, pManager->numDraws);
	printf("\nWould you like to restart?\n");

	// TODO add y/n and exit the game if n is chosen (deferred)
	std::system("pause");

	InitializeGame();
	UpdateGameMode(GAME_MODE::InProgress);
}

void GameManager::UpdateGameMode(const GAME_MODE mode)
{
	GameManager* pManager = GameManager::GetInstance();
	assert(pManager != nullptr);

	pManager->gameMode = mode;
}

void GameManager::UpdateGameBoard(const int position)
{
	GameManager* pManager = GameManager::GetInstance();
	assert(pManager != nullptr);

	if (pManager->isPlayer1Turn)
	{
		pManager->boardPositions[position].markType = MARK_TYPE::X;
	}
	else
	{
		pManager->boardPositions[position].markType = MARK_TYPE::O;
	}
}

void GameManager::PrintGameBoard()
{
	GameManager* pManager = GameManager::GetInstance();
	assert(pManager != nullptr);

	printf("\n\n");

	int id = 0;
	for (int i = 0; i < ROW; i++)
	{
		printf("			|");
		for (int j = 0; j < COLUMN; j++)
		{
			switch(pManager->boardPositions[id].markType)
			{
			case MARK_TYPE::NONE:
				printf("%d", id);
				break;
			case MARK_TYPE::X:
				printf("X");
				break;
			case MARK_TYPE::O:
				printf("O");
				break;
			}
			
			id++;
			printf("|");
		}
		printf("			\n");
		printf("			-------");
		printf("\n");
	}

	printf("\n\n");
}


// checks if the game ended and updates stats if so
const bool GameManager::isGameOver(const int lastMarked)
{
	GameManager* pManager = GameManager::GetInstance();
	assert(pManager != nullptr);

	if (CheckWinRow(lastMarked) || CheckWinColumn(lastMarked) || 
		CheckWinLeftDiagonal(lastMarked) || CheckWinRightDiagonal(lastMarked))
	{
		// k in a row detected, we have a winner
		if (pManager->boardPositions[lastMarked].markType == MARK_TYPE::X)
		{
			pManager->gameResult = GAME_RESULT::Player1;
			pManager->numP1Wins++;
		}
		else 
		{
			pManager->gameResult = GAME_RESULT::Player2;
			pManager->numP2Wins++;
		}

		return true;
	}
	//Check if there are any spaces left
	else if (pManager->currentNumTurns == pManager->numTotalTurns)
	{
		//Total # of turns reach, game ends in draw
		pManager->gameResult = GAME_RESULT::Draw;
		pManager->numDraws++;

		return true;
	}
	return false;
}

const bool GameManager::CheckWinRow(const int lastMarked)
{
	GameManager* pManager = GameManager::GetInstance();
	assert(pManager != nullptr);

	MARK_TYPE currMark = pManager->boardPositions[lastMarked].markType;

	int currNumInRow = 1;

	int currRow = pManager->boardPositions[lastMarked].row;
	int leftIDClamp = currRow * COLUMN; //formula is currRow*n
	int rightIDClamp = (ROW-1) + currRow*COLUMN; //formula is (m-1) + currRow*n
	int leftIDTracer = lastMarked - 1;
	int rightIDTracer = lastMarked + 1;
	bool isLeftSearchDone = false;
	bool isRightSearchDone = false;
	bool isCheckDone = false;

	while (!isCheckDone) 
	{
		//check the leftside
		if (!isLeftSearchDone) 
		{
			if (leftIDTracer < leftIDClamp) { isLeftSearchDone = true; }
			else if (pManager->boardPositions[leftIDTracer].markType == currMark)
			{
				leftIDTracer--;
				currNumInRow++;
			}
			else { isLeftSearchDone = true; }
		}
	   // check right side
		if (!isRightSearchDone)
		{
			if (rightIDTracer > rightIDClamp) { isRightSearchDone = true; }
			else if (pManager->boardPositions[rightIDTracer].markType == currMark)
			{
				rightIDTracer++;
				currNumInRow++;
			}
			else { isRightSearchDone = true; }
		}
		if (currNumInRow == NUM_IN_ROW_WIN)
		{
			int player = (int)currMark + 1;
			//printf("row win found for player: %d \n", player);
			return true;
		} 

		else if (isLeftSearchDone && isRightSearchDone){isCheckDone = true;}
	}

	return false;
}

const bool GameManager::CheckWinColumn(const int lastMarked)
{
	GameManager* pManager = GameManager::GetInstance();
	assert(pManager != nullptr);

	MARK_TYPE currMark = pManager->boardPositions[lastMarked].markType;

	int currNumInRow = 1; 

	int currColumn = pManager->boardPositions[lastMarked].column;
	int topIDClamp = currColumn; 
	int bottomIDClamp = currColumn + ((ROW-1)*COLUMN); //formula currCol + ((m-1)*n)
	int topIDTracer = lastMarked - COLUMN;
	int bottomIDTracer = lastMarked + COLUMN;
	bool isTopSearchDone = false;
	bool isBottomSearchDone = false;
	bool isCheckDone = false;

	while (!isCheckDone)
	{
		//check the leftside
		if (!isTopSearchDone)
		{
			if (topIDTracer < topIDClamp) { isTopSearchDone = true; }
			else if (pManager->boardPositions[topIDTracer].markType == currMark)
			{
				topIDTracer -= COLUMN;
				currNumInRow++;
			}
			else { isTopSearchDone = true; }
		}
		// check right side
		if (!isBottomSearchDone)
		{
			if (bottomIDTracer > bottomIDClamp) { isBottomSearchDone = true; }
			else if (pManager->boardPositions[bottomIDTracer].markType == currMark)
			{
				bottomIDTracer+=COLUMN;
				currNumInRow++;
			}
			else { isBottomSearchDone = true; }
		}
		if (currNumInRow == NUM_IN_ROW_WIN) 
		{ 
			int player = (int)currMark + 1;
			//printf("column win found for player: %d \n", player);
			return true; 
		}
		else if (isTopSearchDone && isBottomSearchDone) { isCheckDone = true; }
	}

	return false;
}

const int GameManager::GetLeftDiagonalTop(const int lastMarked)
{
	GameManager* pManager = GameManager::GetInstance();
	assert(pManager != nullptr);

	int result = lastMarked;
	bool edgeReached = false;

	while (!edgeReached)
	{
		if (pManager->boardPositions[result].column <= 0 || pManager->boardPositions[result].row <= 0)
		{
			edgeReached = true;
		}
		else {
			result -= (COLUMN + 1); 
		}
	}

	if (lastMarked == result)
	{
		//last marked is already at the edge of board
		result = -1;
	}

	return result;
}

const int GameManager::GetLeftDiagonalBottom(const int lastMarked)
{
	GameManager* pManager = GameManager::GetInstance();
	assert(pManager != nullptr);

	int result = lastMarked;
	bool edgeReached = false;

	while (!edgeReached)
	{
		if (pManager->boardPositions[result].column >= (COLUMN - 1) || pManager->boardPositions[result].row >= (ROW - 1))
		{
			edgeReached = true;
		}
		else {
			result += (COLUMN + 1); 
		}
	}

	if (lastMarked == result)
	{
		//last marked is already at the edge of board
		result = -1;
	}
	return result;
}
const int GameManager::GetRightDiagonalTop(const int lastMarked)
{
	GameManager* pManager = GameManager::GetInstance();
	assert(pManager != nullptr);

	int result = lastMarked;
	bool edgeReached = false;

	while (!edgeReached)
	{
		if (pManager->boardPositions[result].column >= (COLUMN - 1) || pManager->boardPositions[result].row <= 0)
		{
			edgeReached = true;
		}
		else {
			result = (result - COLUMN) + 1;
		}
	}

	if (lastMarked == result)
	{
		//last marked is already at the edge of board
		result = -1;
	}

	return result;
}
const int GameManager::GetRightDiagonalBottom(const int lastMarked)
{
	GameManager* pManager = GameManager::GetInstance();
	assert(pManager != nullptr);

	int result = lastMarked;
	bool edgeReached = false;

	while (!edgeReached)
	{
		if (pManager->boardPositions[result].column <= 0 || pManager->boardPositions[result].row >= ROW - 1)
		{
			edgeReached = true;
		}
		else {
			result += (COLUMN - 1);
		}
	}
	if (lastMarked == result)
	{
		//last marked is already at the edge of board
		result = -1;
	}
	return result;
}
const bool GameManager::CheckWinLeftDiagonal(const int lastMarked)
{
	GameManager* pManager = GameManager::GetInstance();
	assert(pManager != nullptr);

	MARK_TYPE currMark = pManager->boardPositions[lastMarked].markType;

	int currNumInRow = 1;
	int currColumn = pManager->boardPositions[lastMarked].column;
	int topIDClamp = GetLeftDiagonalTop(lastMarked); //returns -1 if last mark is already at edge
	int bottomIDClamp = GetLeftDiagonalBottom(lastMarked); //returns -1 if last mark is already at edge
	int topIDTracer = lastMarked - (COLUMN + 1);
	int bottomIDTracer = lastMarked + (COLUMN + 1);
	bool isTopSearchDone = false;
	bool isBottomSearchDone = false;
	bool isCheckDone = false;

	while (!isCheckDone)
	{
		//check the leftside
		if (!isTopSearchDone)
		{
			if (topIDTracer < topIDClamp || topIDClamp == -1) { isTopSearchDone = true; }
			else if (pManager->boardPositions[topIDTracer].markType == currMark)
			{
				topIDTracer -= (COLUMN + 1);
				currNumInRow++;
			}
			else { isTopSearchDone = true; }
		}
		// check right side
		if (!isBottomSearchDone)
		{
			if (bottomIDTracer > bottomIDClamp || topIDClamp == -1) { isBottomSearchDone = true; }
			else if (pManager->boardPositions[bottomIDTracer].markType == currMark)
			{
				bottomIDTracer += (COLUMN + 1);
				currNumInRow++;
			}
			else { isBottomSearchDone = true; }
		}
		if (currNumInRow == NUM_IN_ROW_WIN) 
		{ 
			int player = (int)currMark + 1;
			/*printf("left diagonal win found for player: %d \n", player);
			printf("left diagonal top: %d \n", topIDClamp);
			printf("left diagonal bottom: %d \n", bottomIDClamp);*/
			return true; 
		} 
		else if (isTopSearchDone && isBottomSearchDone) { isCheckDone = true; }
	}

	return false;
}

const bool GameManager::CheckWinRightDiagonal(const int lastMarked)
{
	GameManager* pManager = GameManager::GetInstance();
	assert(pManager != nullptr);

	MARK_TYPE currMark = pManager->boardPositions[lastMarked].markType;

	int currNumInRow = 1;
	int currColumn = pManager->boardPositions[lastMarked].column;
	int topIDClamp = GetRightDiagonalTop(lastMarked); //returns -1 if last mark is already at edge
	int bottomIDClamp = GetRightDiagonalBottom(lastMarked); //returns -1 if last mark is already at edge
	int topIDTracer = lastMarked - (COLUMN - 1);
	int bottomIDTracer = lastMarked + (COLUMN - 1);
	bool isTopSearchDone = false;
	bool isBottomSearchDone = false;
	bool isCheckDone = false;


	while (!isCheckDone)
	{
		//check the leftside
		if (!isTopSearchDone)
		{
			if (topIDTracer < topIDClamp || topIDClamp == -1) { isTopSearchDone = true; }
			else if (pManager->boardPositions[topIDTracer].markType == currMark)
			{
				topIDTracer -= (COLUMN - 1);
				currNumInRow++;
			}
			else { isTopSearchDone = true; }
		}
		// check right side
		if (!isBottomSearchDone)
		{
			if (bottomIDTracer > bottomIDClamp || bottomIDClamp == -1) { isBottomSearchDone = true; }
			else if (pManager->boardPositions[bottomIDTracer].markType == currMark)
			{
				bottomIDTracer += (COLUMN - 1);
				currNumInRow++;
			}
			else { isBottomSearchDone = true; }
		}
		if (currNumInRow == NUM_IN_ROW_WIN) 
		{ 
			int player = (int)currMark + 1;
			/*printf("right diagonal win found for player: %d \n", player);
			printf("right diagonal top: %d \n", topIDClamp);
			printf("right diagonal bottom: %d \n", bottomIDClamp);*/
			return true; 
		}
		else if (isTopSearchDone && isBottomSearchDone) { isCheckDone = true; }
	}

	return false;
}

GameManager* GameManager::GetInstance()
{
	static GameManager gameMan;
	return &gameMan;
}
