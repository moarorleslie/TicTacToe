#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <assert.h>

// Next steps - for time estimates listed, 1 day = 8 hrs (1 work day)
// 1. Bonus 1: M N K 
    // You can set the m,n,k macros below to see a different m,n,k variation
    // I only ran manual tests, so there are probably some bugs
	// Next steps and estimates:
		// write unit tests to validate CheckWin() functions for different mkn variations (1 day)
        // code logic found from tests above (half day)
// 2. Bonus 2: Undo history (half day)
   // Add user key input to undo
   // Create a stack and add each mark to the stack
   // Update the Game Manager data members accordingly
// 3. Finish TODOs left in code (half day)

#define ROW 3 //m
#define COLUMN 3 //n
#define NUM_IN_ROW_WIN 3 //k


class GameManager
{
	enum class GAME_MODE
	{
		Start,
		InProgress,
		End
	};

	enum class GAME_RESULT
	{
		Player1,
		Player2,
		Draw,
		NONE
	};

	enum class MARK_TYPE
	{
		X,
		O,
		NONE
	};

	struct BoardPosition
	{
		int row;
		int column;
		MARK_TYPE markType;
	};

public:

	// Defaults
	GameManager(const GameManager&) = delete;
	GameManager(GameManager&&) = delete;
	GameManager& operator = (const GameManager&) = delete;
	GameManager& operator = (GameManager&&) = delete;
	~GameManager();

	static void Create();
	static void Shutdown();
	static void Update();
	static bool GetQuitStatus();

private:

	static GameManager* GetInstance();
	GameManager();

	void InitializeGame();
	
	//Helpers for display text based on game mode
	void ShowStartText(); 
	void ShowInProgressText(); 
	void ShowEndText(); 

	void UpdateGameMode(const GAME_MODE mode);
	void UpdateGameBoard(const int position);
	void PrintGameBoard();

	const bool isGameOver(const int lastMarked);

	// Helpers to check if anyone won:
		// Takes the last move and check neighboring cells
		// Reusable for mkn solution 
	const bool CheckWinRow(const int lastMarked);
	const bool CheckWinColumn(const int lastMarked);
	const bool CheckWinLeftDiagonal(const int lastMarked); // upper side diagonal starts at left (like this \)
	const bool CheckWinRightDiagonal(const int lastMarked); // upper side diagonal starts at right (like this /)

	//TODO can probably figure out a math formula to replace these with
	//helpers to find edge of the board when checking diagonal wins
	const int GetLeftDiagonalTop(const int lastMarked);
	const int GetLeftDiagonalBottom(const int lastMarked);
	const int GetRightDiagonalTop(const int lastMarked);
	const int GetRightDiagonalBottom(const int lastMarked);

	BoardPosition boardPositions[ROW*COLUMN] = { {0} };
	GAME_MODE gameMode = GAME_MODE::Start;
	GAME_RESULT gameResult = GAME_RESULT::NONE;
	bool isPlayer1Turn = true;
	bool isQuit = false;
	int currentNumTurns = 0;
	int numTotalTurns = ROW * COLUMN - 1;

	int numP1Wins = 0;
	int numP2Wins = 0;
	int numDraws = 0;
};

#endif