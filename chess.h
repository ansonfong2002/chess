#ifndef CHESS_H
#define CHESS_H
#include "piece.h"
// singly linked list. Valid moves store target index, point to next valid move or NULL.
typedef struct moveNode {
    int target;
    struct moveNode* nextNode;
} moveNode;
// prepends a new move node
void addMove(moveNode** head, int t);
// frees memory allocated for moveList
void freeMoves(moveNode* head);
// singly linked list. States point to previous game states
typedef struct gameList {
    pieceNode* position;
    struct gameList* previous;
} gameList;
// appends a new game state
void addState(pieceNode* board, gameList** previousState);
// frees memory allocated for gameList
void freeStates(gameList* state);
// creates copy of current state, calls addState
void logBoard(pieceNode* pieceList, gameList** previous);
// looks for a tie by travesing gameStates: 3 occurences = tie
int findTie(gameList* state);
// finds piece to move and updates with new index. takes care of captures and special flags/moves
void move(pieceNode* pieceList, int i, int t);
//returns a copy of pieceList
pieceNode* copyBoard(pieceNode* pieceList);
// returns a copy of pieceList with the updated moves and flags
pieceNode* simulateMove(int i, int t, pieceNode* pieceList);
// given a pieceList, return 1 if given player delivered check.
int findCheck(pieceNode* pieceList, int p);
// given a pieceList, find if given player is in checkmate. Return winner if mate, 3 if stalemate, 0 otherwise.
int findMate(pieceNode* pieceList, int p);
/*  given a pieceNode, add nodes to a linked list of valid moves.
    Returns list head.
    - use simulateMove and findCheck to ensure moves don't create discovered checks
    - depth: limits recursion in generateMoves/findCheck. 1 = basic, 2 = maximum depth
*/
moveNode* generateMoves(pieceNode* piece, int listIndex, int depth);
// generates Queen's valid pieces, add nodes to a linked list of valid moves, returns the list's head
void genQueen(pieceNode* piece, int listIndex, moveNode** move, int depth);
// generates Rook valid moves, add nodes to a linked list of valid moves, returns the list's head
void genRook(pieceNode* piece, int listIndex, moveNode** move, int depth);
// generates Bishop valid moves, add nodes to a linked list of valid moves, returns the list's head
void genBishop(pieceNode* piece, int listIndex, moveNode** move, int depth);
// generates Knight valid moves, add nodes to a linked list of valid moves, returns the list's head
void genKnight(pieceNode* piece, int listIndex, moveNode** move, int depth);
// generates Pawn valid moves, add nodes to a linked list of valid moves, returns the list's head
void genPawn(pieceNode* piece, int listIndex, moveNode** move, int depth);
// generates King valid moves, add nodes to a linked list of valid moves, returns the list's head
void genKing(pieceNode* piece, int listIndex, moveNode** move, int depth);
#endif