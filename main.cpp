#include <iostream>
#include <cstdio>
/*
Chess board =
K = King
Q = Queen
p = Pawn
R = Rook
H = Knight
B = Bishop
Board is an 8x8 matrix of ints
each "Tile" should encompass a set amount of state within 1 int:
* Occupied?
* If so, Type?
* Color of Tile
* Location of tile on board (row and column)
each "Piece" should express some amount of state:
* Type
* Color
* Location
_________________
Board[0][0]	-->	|_|_|_|_|_|_|_|_| <-- Board[0][7]
|_|_|_|_|_|_|_|_|
|_|_|_|_|_|_|_|_|
|_|_|_|_|_|_|_|_|
|_|_|_|_|_|_|_|_|
|_|_|_|_|_|_|_|_|
|_|_|_|_|_|_|_|_|
Board[7][0]	-->	|_|_|_|_|_|_|_|_| <-- Board[7][7]
_________________
Board[0][0]	-->	|B|H|R|Q|K|R|H|B| <-- Board[0][7]
|p|p|p|p|p|p|p|p|
|_|_|_|_|_|_|_|_|
|_|_|_|_|_|_|_|_|
|_|_|_|_|_|_|_|_|
|_|_|_|_|_|_|_|_|
|_|_|_|_|_|_|_|_|
Board[7][0]	-->	|_|_|_|_|_|_|_|_| <-- Board[7][7]
State:
Types of Move:
* Attack a piece
* En Passant
* Move a piece
* Upgrade a pawn
* Castle
*/
#define ANSI_BACKGROUND_DARK_GRAY 	"\x1b[100m"
#define ANSI_BACKGROUND_LIGHT_GRAY  "\x1b[47m"
#define ANSI_COLOR_WHITE		 				"\x1b[37m"
#define ANSI_COLOR_BRIGHT_WHITE 		"\x1b[97m"
#define ANSI_COLOR_BLACK						"\x1b[30m"
#define ANSI_COLOR_RED							"\x1b[31m"
#define ANSI_COLOR_RESET		 				"\x1b[0m"
#define ANSI_BOLD 									"\x1b[1m"
#define ANSI_BLINK									"\x1b[5m"
enum Color
{
	WHITE = 0,
	BLACK = 1
};

enum Piece
{
	PAWN = 1,
	KNIGHT = 2,
	ROOK = 3,
	BISHOP = 4,
	QUEEN = 5,
	KING = 6
};
const unsigned short OCCUPIED = 0x8000;
unsigned const terminalWidth = 80;

class GamesBoard
{
public:
	// EACH UNSIGNED INT HAS THE FOLLOWING BIT STRUCTURE: LSB = COLOR (0 = WHITE, 1 = BLACK). BITS 1-3 = TYPE. BITS 4-7 = COLUMN. BITS 8-11 = ROW. BIT 15 AKA MSB = OCCUPIED OR NOT
	unsigned short gameBoardShort[8][8] = { 0 };

	GamesBoard()
	{
		for (int x = 0; x < 8; x++)
		{
			for (int y = 0; y < 8; y++)
			{
				gameBoardShort[x][y] = y << 4 | x << 8;
			}
		}
		// Initialize Pieces
		for (int j = 0; j < 8; j++)
		{
			gameBoardShort[1][j] |= PAWN << 1 | BLACK | OCCUPIED;
			gameBoardShort[6][j] |= PAWN << 1 | WHITE | OCCUPIED;

		}
		gameBoardShort[0][0] |= BISHOP << 1 | BLACK | OCCUPIED;
		gameBoardShort[0][7] |= BISHOP << 1 | BLACK | OCCUPIED;
		gameBoardShort[7][0] |= BISHOP << 1 | WHITE | OCCUPIED;
		gameBoardShort[7][7] |= BISHOP << 1 | WHITE | OCCUPIED;

		gameBoardShort[0][1] |= KNIGHT << 1 | BLACK | OCCUPIED;
		gameBoardShort[0][6] |= KNIGHT << 1 | BLACK | OCCUPIED;
		gameBoardShort[7][1] |= KNIGHT << 1 | WHITE | OCCUPIED;
		gameBoardShort[7][6] |= KNIGHT << 1 | WHITE | OCCUPIED;

		gameBoardShort[0][2] |= ROOK << 1 | BLACK | OCCUPIED;
		gameBoardShort[0][5] |= ROOK << 1 | BLACK | OCCUPIED;
		gameBoardShort[7][2] |= ROOK << 1 | WHITE | OCCUPIED;
		gameBoardShort[7][5] |= ROOK << 1 | WHITE | OCCUPIED;

		gameBoardShort[0][4] |= KING << 1 | BLACK | OCCUPIED;
		gameBoardShort[0][3] |= QUEEN << 1 | BLACK | OCCUPIED;
		gameBoardShort[7][3] |= QUEEN << 1 | WHITE | OCCUPIED;
		gameBoardShort[7][4] |= KING << 1 | WHITE | OCCUPIED;
	}

	void PrintBoard()
	{
		std::cout << "  ";
		for (int x = 0; x < 16; x++)
		{
			std::cout << "_";
		}
		std::cout << std::endl;
		for (int i = 0; i < 8; i++)
		{
			std::cout << 8 - i << " ";
			for (int j = 0; j < 8; j++)
			{
				PrintSquare(gameBoardShort[i][j]);
			}
			std::cout << std::endl;
		}
		std::cout << "   A B C D E F G H" << std::endl;
	}

	char PrintPiece(unsigned short tmp)
	{
		switch (tmp)
		{
		case PAWN:
		{
			return 'P';
		}
		case KNIGHT: {
			return 'N';
		}
		case BISHOP: {
			return 'B';
		}
		case ROOK: {
			return 'R';
		}
		case QUEEN: {
			return 'Q';
		}
		case KING: {
			return 'K';
		}
		default:
			return ' ';
			break;
		}
	}

	int getRowValue(unsigned short tile)
	{
		return (tile & 0xf00) >> 8;
	}

	int getColValue(unsigned short tile)
	{
		return (tile & 0xf0) >> 4;
	}


	void PrintSquare(unsigned short squareval)
	{
		bool white = true;
		unsigned short pieceval = squareval & 0xf;
		if (pieceval % 2 == 1) // is LSB 0 or 1 (LSB = color)
		{
			white = false;
		}
		pieceval = pieceval >> 1;
		char c = PrintPiece(pieceval);
		int rowval = getRowValue(squareval);
		int colval = getColValue(squareval);

		if (white == true)
		{
			if ((rowval + colval) % 2 == 0)
			{
				printf(ANSI_BACKGROUND_LIGHT_GRAY ANSI_BOLD ANSI_COLOR_BRIGHT_WHITE "%*c" ANSI_COLOR_RESET, 2, c);
			}
			else {
				printf(ANSI_BACKGROUND_DARK_GRAY  ANSI_BOLD ANSI_COLOR_BRIGHT_WHITE "%*c" ANSI_COLOR_RESET, 2, c);
			}
		}
		else {
			if ((rowval + colval) % 2 == 0)
			{
				printf(ANSI_BACKGROUND_LIGHT_GRAY ANSI_COLOR_BLACK "%*c" ANSI_COLOR_RESET, 2, c);
			}
			else {
				printf(ANSI_BACKGROUND_DARK_GRAY  ANSI_COLOR_BLACK "%*c" ANSI_COLOR_RESET, 2, c);
			}
		}
	}
	void PrintHexBoardValues()
	{
		std::cout << std::endl;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				std::cout << "Row: " << i << "\tCol: " << j << "\tHex: " << std::hex << gameBoardShort[i][j] << std::endl;
			}
		}
	}

	int movePiece(int row, int col, unsigned short dest_row, unsigned short dest_col)
	{
		int status = 0;
		if (occupied(gameBoardShort[row][col]) && !occupied(gameBoardShort[dest_row][dest_col]))
		{

			gameBoardShort[dest_row][dest_col] |= (gameBoardShort[row][col] & 0x800F);
			gameBoardShort[row][col] = gameBoardShort[row][col] & 0x7ff0;

		}



		return status;
	}
	/* 
		basic movement paradigms (assuming 8A is [0][0]):
		diagonal movement = 4 paradigms:
			* NW : - / -
			* NE : - / +
			* SW : + / -
			* SE : + / +
		straight movement = 2 paradigms:
			* row movement: + / 0 or - / 0
			* col movement: 0 / + or 0 / -
		+/- x |  -/+ x
	*/

	int movePiece(unsigned short& tile, unsigned short& dest_tile)
	{
		int status = 0;
		Piece type = (Piece)(getPieceType(tile));
		dest_tile = dest_tile & 0x0FF0;
		dest_tile = dest_tile | (tile & 0x800F);
		tile = tile & 0x0FF0;
		return status;
	}

	int getPieceType(unsigned short tile)
	{
		return (tile & 0xe) >> 1;
	}

	void getInput(Color color)
	{


	}
	void takeTurn(Color playerColor)
	{
		int status = 0;	
		unsigned short&	tile = selectPiece(playerColor);

		do {
			unsigned short& dest = getDestTile();
			if (dest != 0xFFFF)
			{
				status = movePiece(tile, dest);
			}
			else
			{
				status = -1;
			}
		} while (status != 0);
	}

	unsigned short& getDestTile()
	{
		int status = 0, actRow, actCol;
		char col;

		do {
			status = 0;
			int row;
			std::cout << "Select row: ";
			std::cin >> row;
			actRow = 8 - row; // board[0] == row 8 etc...
			if (actRow > 7 || actRow < 0)
			{
				std::cout << "Row " << actRow << " is not valid" << std::endl;
				status = -1;
			}
		} while (status != 0);
	
		do {
			status = 0;
			std::cout << "Select col: ";
			std::cin.clear();
			std::cin >> col;
			col = toupper(col);
			actCol = col - 65;
			if (actCol > 7 || actCol < 0)
			{
				std::cout << "Column " << actCol << " is not valid" << std::endl;
				status = -1;
			}
		} while (status != 0);
		unsigned short& tmp = gameBoardShort[actRow][actCol];
		return tmp;
	}

	unsigned short& getTileRef()
	{
		int status = 0, actRow, actCol;
		char col;

		do {
			status = 0;
			int row;
			std::cout << "Select row: ";
			std::cin >> row;
			actRow = 8 - row; // board[0] == row 8 etc...
			if (actRow > 7 || actRow < 0)
			{
				std::cout << "Row " << actRow << " is not valid" << std::endl;
				status = -1;
			}
		} while (status != 0);

		do {
			status = 0;
			std::cout << "Select col: ";
			std::cin.clear();
			std::cin >> col;
			col = toupper(col);
			actCol = col - 65;
			if (actCol > 7 || actCol < 0)
			{
				std::cout << "Column " << actCol << " is not valid" << std::endl;
				status = -1;
			}
		} while (status != 0);
		unsigned short& tmp = gameBoardShort[actRow][actCol];
		return tmp;
	}

	unsigned short& selectPiece(Color playerColor)
	{
		int status = 0;
		unsigned short& tile = getTileRef();

		do {
			if (!(occupied(tile)))
			{
				std::cout << "Tile is unoccupied" << std::endl;
				status = -1;
				tile = getTileRef();
			}
			else if (tile & 0x1 != playerColor)
			{
				std::cout << "Incorrect piece color" << std::endl;
				status = -1;
				tile = getTileRef();
			}
		} while (status != 0);

		return tile;
	}

	bool occupied(unsigned short tile)
	{
		unsigned short tmp = tile & 0x8000;
		if (tmp > 0)
		{
			return true;
		}
		return false;
	}
};

class Player
{

};
void PollInput()
{

}

/*
Game State to keep track of:
* En Passant (can probably do logic manually each time, need to make sure pawn cant be taken in later turns)
* Check (can't move other pieces when in check other than ones that block check)
* Castling (Has king been in check before?/ movement portion relatively simple, except whether or not pass a tile in check when performing movement)

*/



int main()
{
	int turnCounter = 0, y = 0;
	std::cout << "Welcome to the Chess Game!" << std::endl;
	GamesBoard board;
	board.PrintBoard();
	board.movePiece(0, 3, 5, 6);
	unsigned short* tmp = &board.gameBoardShort[0][0];
	printf("\x1b[2J"); // erase display
	board.PrintBoard();
	*tmp = 0x8006;
	printf("\x1b[2J"); // erase display
	board.PrintBoard();

	bool gameOver = false;
	for (turnCounter = 0; turnCounter >= 0; turnCounter++)
	{
		if (turnCounter % 2 == 1)
		{
			std::cout << "Black's turn: " << std::endl;
			board.takeTurn(BLACK);
		}
		else {
			std::cout << "White's turn: " << std::endl;
			board.takeTurn(WHITE);
		}

		printf("\x1b[2J"); // erase display
		board.PrintBoard();

	}
	
}
