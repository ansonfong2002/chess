#ifndef AI_H
#define AI_H
#include "chess.h"
// stores move information: initial/target index
typedef struct moveInfo {
    int initial;
    int target;
} moveInfo;
// list of treeNode pointers pointed to by rootNode: stores a root's children
typedef struct children {
    struct children* nextChild; // ptr to next child
    struct treeNode* node;            // ptr to this branch's treeNode
} children;
// treeNode: stores a possible move, board, score, parent and childList pointers
typedef struct treeNode{
    struct treeNode* parent;    // parent node ptr
    children* branches;         // ptr to list of children
    pieceNode* pieceList;
    double score;   // evaluation at node
    struct moveInfo info; // Keep track of what moved where
} treeNode;
// adds a pointer to a list of children pointers. Requires the edge this pointer will point to.
void addChild(children** childList, treeNode* newNode);
// frees the memory allocated for list of children. calls deleteTree to delete associated edges
void freeChildren(children* first);
// given a simulated board and the start/end index, create and return a new treeNode with the new pieceList
treeNode* makeNode(treeNode* root, pieceNode* pieces, children* branches, int pos, int target, int player);
// recursively deletes entire tree when called on the root node. calls freeChildren to free linked list nodes
void freeNode(treeNode* node);
/* populates the tree of outcomes recursively from a initial position, limited by depth parameter. 
    - populates downward, then laterally.
    - returns the evaluation of the node.
        - eval gets called on DEEPEST nodes only. Scores propagate up the tree based on player to move at each depth
        - if player = 1, this node's evaluation = highest of children
        - if player = 2, this node's evaluation = lowest of children
        - parameters alpha and beta assist in pruning unlikely branches.
            - alpha = maximum lower bound of considered outcomes (white prefers this high)
            - beta = minimum upper bound of considered outcomes (black prefers this low)
        - if checkmate move, set evaluation to +/- 1000
    - returned evaluation is used to set current node's evaluation, which is returned up the tree
*/
double grow(treeNode* root, int depth, int color, double alpha, double beta);
// creates root treeNode and initiates growth
treeNode* buildTree(pieceNode* initial, int depth, int firstMover, double* score);
// Evaluates the board position based on material count, position score 
double eval(pieceNode* pieceList);
// returns the amount of squares controlled by a player
double scorePos(pieceNode* pieceList, int i);
#endif