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

#define ANSI_COLOR_WHITE		 "\x1b[37m"
#define ANSI_COLOR_WHITE		 "\x1b[30m"
#define ANSI_COLOR_RESET		 "\x1b[0m"
enum Color 
{
	WHITE = 0,
	BLACK = 4
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

unsigned const terminalWidth = 80;

class GamePiece
{
public:
	Color color;
//	TileState& location;
	Piece type;
	void Print() {}
//	virtual void Print();
//	virtual void Move();
};

class Pawn : GamePiece
{
public:
	bool hasMoved;
	Pawn(Color col)
	{
		hasMoved = false;
		color = col;
	}
	void Move()
	{

	}
	void Print()
	{
		if (color == WHITE)
			std::cout << "P";
		else if (color == BLACK)
			std::cout << "p";
	}
};

class Knight : GamePiece
{
	void Move()
	{
	}
	void Print()
	{
		if (color == WHITE)
			std::cout << "H";
		else if (color == BLACK)
			std::cout << "h";
	}

};
class Rook : GamePiece
{
	void Move()
	{
	}
	void Print()
	{
		if (color == WHITE)
			std::cout << "R";
		else if (color == BLACK)
			std::cout << "r";
	}

};
class Bishop : GamePiece
{
	void Move()
	{
	}
	void Print()
	{
		if (color == WHITE)
			std::cout << "B";
		else if (color == BLACK)
			std::cout << "b";
	}

};
class King : GamePiece
{
	bool inCheck;
	void Move()
	{
	}
	void Print()
	{
		if (color == WHITE)
			std::cout << "K";
		else if (color == BLACK)
			std::cout << "k";
	}

};
class Queen : GamePiece
{
	void Move()
	{
	}
	void Print()
	{
		if (color == WHITE)
			std::cout << "Q";
		else if (color == BLACK)
			std::cout << "q";
	}

};
class TileState
{
public:
	bool tileOccupied;

	// recall that a matrix or double array is addressed as follows Board[row][col]
	static int Row;
	static int Col;

	 Color tileColor;
	 GamePiece type;
	 TileState() {}
	TileState(int row, int col, Color color)
	{
		Row = row;
		Col = col;
		tileColor = color;
		tileOccupied = false;
	}
	TileState(int row, int col, Color color, GamePiece piece)
	{
		Row = row;
		Col = col;
		tileColor = color;
		type = piece;
		tileOccupied = true;
	}
	void print()
	{
		if (tileOccupied)
		{
			type.Print();
		}
		else
		{
			std::cout << "_";
		}
	}
	TileState Add(TileState tile, GamePiece piece)
	{
		tile.type = piece;
		tile.tileOccupied = true;
		return tile;
	}
};


class GameState
{
};
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
			gameBoardShort[1][j] |= PAWN << 1 | 0x8001;
			gameBoardShort[6][j] |= PAWN << 1 | 0x8000;

		}
		gameBoardShort[0][0] |= BISHOP << 1 | 0x8001;
		gameBoardShort[0][7] |= BISHOP << 1 | 0x8001;
		gameBoardShort[7][0] |= BISHOP << 1 | 0x8000;
		gameBoardShort[7][7] |= BISHOP << 1 | 0x8000;
		gameBoardShort[0][1] |= KNIGHT << 1 | 0x8001;
		gameBoardShort[0][6] |= KNIGHT << 1 | 0x8001;
		gameBoardShort[7][1] |= KNIGHT << 1 | 0x8000;
		gameBoardShort[7][6] |= KNIGHT << 1 | 0x8000;

		gameBoardShort[0][2] |= ROOK << 1 | 0x8001;
		gameBoardShort[0][5] |= ROOK << 1 | 0x8001;
		gameBoardShort[7][2] |= ROOK << 1 | 0x8000;
		gameBoardShort[7][5] |= ROOK << 1 | 0x8000;

		gameBoardShort[0][4] |= KING << 1 | 0x8001;
		gameBoardShort[0][3] |= QUEEN << 1 | 0x8001;
		gameBoardShort[7][3] |= QUEEN << 1 | 0x8000;
		gameBoardShort[7][4] |= KING << 1 | 0x8000;
	}

	void PrintBoard()
	{
		std::cout << "  ";
		for (int x = 0; x < 17; x++)
		{
			std::cout << "_";
		}
		std::cout << std::endl;
		for (int i = 0; i < 8; i++)
		{
			std::cout << 8 - i << " ";
			for (int j = 0; j < 8; j++)
			{
				printf(ANSI_COLOR_WHITE " " ANSI_COLOR_RESET);
				PrintPiece(gameBoardShort[i][j]);
			}
			std::cout << "|" << std::endl;
		}
		std::cout << "   A B C D E F G H" << std::endl;
	}

	void PrintPiece(unsigned short piece)
	{
		bool white = true;
		unsigned short tmp = piece & 0xf;
		if (tmp % 2 == 1)
		{
			white = false;
		}
		tmp = tmp >> 1;
		switch (tmp)
		{
		case PAWN:
		{
			if (white)
				std::cout << "P";
			else
				std::cout << "p";
			break;
		}
		case KNIGHT: {
			if (white)
				std::cout << "H";
			else
				std::cout << "h";
			break;
		}
		case BISHOP: {
			if (white)
				std::cout << "B";
			else
				std::cout << "b";
			break;
		}
		case ROOK: {
			if (white)
				std::cout << "R";
			else
				std::cout << "r";
			break;
		}
		case QUEEN: {
			if (white)
				std::cout << "Q";
			else
				std::cout << "q";
			break;
		}
		case KING: {
			if (white)
				std::cout << "K";
			else
				std::cout << "k";
			break;
		}
		default:
			std::cout << "_";
			break;
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




int main()
{
	int turnCounter = 1;
	std::cout << "Welcome to the Chess Game!" << std::endl;
	GamesBoard board;
	board.PrintBoard();
	board.movePiece(0, 3, 5, 6);
	board.PrintBoard();
//	board.PrintHexBoardValues();
}
