#ifndef PIECE_H
#define PIECE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// piece type enumerator: 0-5 int representations
enum PieceTypes { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};
/*  Flag variables for the board's state: used by generateMoves
    - 1 = eligible castle, 0 = no castle
	- enPassant holds last double jump index.
*/
typedef struct flagVars {
    int enPassant;
    int wCastleE;
    int wCastleW;
    int bCastleE;
    int bCastleW;
    int check;
} flagVars;
// pieceNode circular linked list, stores index, type, player, pointer to flags, pointer to next piece
typedef struct pieceNode {
    enum PieceTypes type;
	int index;
	int player;
	flagVars* flags;
} pieceNode;
// adds pieceNode to list, should only be used in generateBoard()
void addPiece(pieceNode* list, int i, flagVars* f, enum PieceTypes t, int color, int position);
// frees the array of pieces, as well as the flag pointer
void freePieces(pieceNode* list);
// switches between players
int toggle(int num);
// given string (ie E4), convert to index in 12x12 array
int getPos(char position[3]);
// initializes and returns 32 piece list with initial starting positions
pieceNode* generateBoard();
// given an orientation (who's turn), print the entire board, file/rank grid
void printBoard(pieceNode* pieceList, int orientation);
// returns char representation of a piece's type
char toName(enum PieceTypes type);
// returns char representation of a piece's color
char toColor(int player);
// puts rank and file notation for index (in 144 array) into string parameter
void toRankFile(int i, char* s);
// prints the material sum of each player
int getMaterial(pieceNode* pieceList, int p);
// iterates through list: returns index within pieceList if found, -1 if not found
int findPiece(int pos, pieceNode* head);
// maps 12x12 index to a 8x8 index, discarding buffer squares
int map(int index);
#endif