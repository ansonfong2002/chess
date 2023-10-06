#include "chess.h"
void addMove(moveNode** head, int t) {
    moveNode* newNode = (moveNode*)malloc(sizeof(moveNode));
    newNode->nextNode = *head;
    *head = newNode;
    newNode->target = t;
}
void freeMoves(moveNode* head) {
    moveNode* current = head;
    while (current != NULL) {
        moveNode* next = current->nextNode;
        free(current);
        current = next;
    }
}
void addState(pieceNode* board, gameList** previousState) {
    gameList* newState = (gameList*)malloc(sizeof(gameList));
    newState->position = board;
    newState->previous = *previousState;
    *previousState = newState;
}
void freeStates(gameList* state) {
    gameList* current = state;
    while(current != NULL) {
        gameList* next = current->previous;
        freePieces(current->position);
        free(current);
        current = next;
    }
}
void logBoard(pieceNode* pieceList, gameList** previous) {
    pieceNode* logBoard = copyBoard(pieceList);  // make a copy of current board
    addState(logBoard, previous);
}
int findTie(gameList* state) {
    int repeats = 0;
    gameList* current = state->previous;
    while (current != NULL) {
        for (int i = 0; i < 32; i++) {
            if (state->position[i].index != current->position[i].index) {break;}    // one piece doesn't match
            else if (i == 31) {repeats++;}  // all pieces match
        }
        if (repeats == 2) {return 1;}   // stalemate
        current = current->previous;    // next state
    }
    return 0;   // no stalemate
}
void move(pieceNode* pieceList, int i, int t) {
    int target = findPiece(t, pieceList);
    if (target >= 0) {
        if (pieceList[target].type == ROOK) {   // capture rook, no more castle
            if (t == 117) {pieceList->flags->wCastleE = 0;}
            else if (t == 110) {pieceList->flags->wCastleW = 0;}
            else if (t == 33) {pieceList->flags->bCastleE = 0;}
            else if (t == 26) {pieceList->flags->bCastleW = 0;}
        }
        pieceList[target].index = 0;    // if target square occupied, move is a capture. Set target piece's index to 0.
    }    
    int listIndex = findPiece(i, pieceList);
    pieceList[listIndex].index = t;       // make the move
    pieceList->flags->check = findCheck(pieceList, 1) || findCheck(pieceList, 2);   // move creates check?
    // is this move en passant
    if (pieceList[listIndex].type == PAWN) {
        if (abs(pieceList->flags->enPassant - t) == 12 && abs(pieceList->flags->enPassant - i) == 1) {
            int ep = findPiece(pieceList->flags->enPassant, pieceList);  // find pawn on enPassant square
            pieceList[ep].index = 0;               // capture pawn
            pieceList->flags->enPassant = 0;    // reset enPassant
            return;
        }
        if (abs(i - t) == 24) {
            pieceList->flags->enPassant = t;    // set enPassant
            return;
        }
    }
    pieceList->flags->enPassant = 0;    // reset enpassant: players' double jumps set enPassant until next player's turn reaches this point.
    // change castle flags if necessary. Handle rook movement on castle
    if (pieceList[listIndex].player == 1 && pieceList[listIndex].type == KING) {
        if (pieceList->flags->wCastleE && i == 114 && t == 116) {
            pieceList[31].index = 115;         // move east rook
        } else if (pieceList->flags->wCastleW && i == 114 && t == 112) {
            pieceList[24].index = 113;         // move west rook
        }
        pieceList->flags->wCastleE = 0;     // white king: lose castles
        pieceList->flags->wCastleW = 0;
    } else if (pieceList[listIndex].player == 2 && pieceList[listIndex].type == KING) {
        if (pieceList->flags->bCastleE && i == 30 && t == 32) {
            pieceList[23].index = 31;         // move east rook
        } else if (pieceList->flags->bCastleW && i == 30 && t == 28) {
            pieceList[16].index = 29;         // move west rook
        }
        pieceList->flags->bCastleE = 0;     // black king: lose castles
        pieceList->flags->bCastleW = 0;
    } else if (pieceList[listIndex].type == ROOK) {
        if (pieceList->flags->wCastleE && i == 117) {pieceList->flags->wCastleE = 0;}       // remove castle for correct rook
        else if (pieceList->flags->wCastleW && i == 110) {pieceList->flags->wCastleW = 0;}
        else if (pieceList->flags->bCastleE && i == 33) {pieceList->flags->bCastleE = 0;}   // remove castle for correct rook
        else if (pieceList->flags->bCastleW && i == 26) {pieceList->flags->bCastleW = 0;}
    }
}
pieceNode* simulateMove(int i, int t, pieceNode* pieceList) {
    pieceNode* newList = copyBoard(pieceList);  // creates temp list of pieces
    move(newList, i, t);    // call move
    return newList; // returns updated COPY of pieceList
}
pieceNode* copyBoard(pieceNode* pieceList) {
    // create new instance of flags for the simulated board
    flagVars* flags = (flagVars*)malloc(sizeof(flagVars));
    *flags = *(pieceList->flags);
    // create new temp list of pieces
    pieceNode* copy = (pieceNode*)malloc(32 * sizeof(pieceNode));
    // add copies of each piece to the new list
    for (int x = 0; x < 32; x++) {
        addPiece(copy, x, flags, pieceList[x].type, pieceList[x].player, pieceList[x].index);
    }
    return copy; 
}
int findCheck(pieceNode* pieceList, int p) {
    moveNode* head = NULL;
    moveNode* current = NULL;
    int kingPos = (p == 1) ? pieceList[20].index : pieceList[28].index; // location of opponent king
    for (int x = 0; x < 32; x++) {
        if (pieceList[x].player == p) {
            // add player's piece's attacks
            current = generateMoves(pieceList, x, 2);
            while (current != NULL) {
                addMove(&head, current->target);
                moveNode* next = current->nextNode;
                free(current);  // free temporary move list as it's copied to "head"
                current = next;
            }
        }
    }
    current = head;
    while (current != NULL) {
        if (current->target == kingPos) {
            freeMoves(head);    // free the move list
            return 1;
        }
        current = current->nextNode;
    }
    freeMoves(head);    // free the move list
    return 0;
}
int findMate(pieceNode* pieceList, int p) {
    for (int i = 0; i < 32; i++) {
        if (pieceList[i].player == p && pieceList[i].index) {
            moveNode* moves = generateMoves(pieceList, i, 1);  // generate moves for all p's pieces
            if (moves != NULL) {
                freeMoves(moves);
                return 0;   // if a single move exists, not checkmate
            }
            freeMoves(moves);
        }
    }
    if (pieceList->flags->check) {
        return toggle(p);   // return winner: other player
    }
    return 3;   // stalemate
}
moveNode* generateMoves(pieceNode* piece, int listIndex, int depth) {
    moveNode* head = NULL;
    switch (piece[listIndex].type) {
        case QUEEN: genQueen(piece, listIndex, &head, depth);
            break;
        case ROOK: genRook(piece, listIndex, &head, depth);
            break;
        case BISHOP: genBishop(piece, listIndex, &head, depth);
            break;
        case KNIGHT: genKnight(piece, listIndex, &head, depth);
            break;
        case PAWN: genPawn(piece, listIndex, &head, depth);
            break;
        default: genKing(piece, listIndex, &head, depth);
    }
    return head;
}
void genQueen(pieceNode* piece, int listIndex, moveNode** move, int depth) {
    // save values of piece in question since pieceList will be iterated through
    int pos = piece[listIndex].index;
    int p = piece[listIndex].player;
    int discovered;
    int directions[] = {-13, -12, -11, -1, 1, 11, 12, 13};
    for (int i = 0; i < 8; i++) {
        int newIndex = pos + (directions[i]);
        int m = 1, capture = 0;
        while (newIndex > 25 && newIndex < 118 && newIndex % 12 > 1 && newIndex % 12 < 10) {
            int occupied = findPiece(newIndex, piece);
            if (occupied >= 0) {
                if (piece[occupied].player == p) {break;}    //occupied by self
                capture = 1;    // occupied by opponent
            }
            if (depth == 1) {
                pieceNode* simPieces = simulateMove(pos, newIndex, piece);
                discovered = findCheck(simPieces, toggle(p));
                freePieces(simPieces);  // free the simulated list
            } else {discovered = 0;}
            if (!discovered) {addMove(move, newIndex);}
            if (capture) {break;}
            m++;
            newIndex = pos + (directions[i] * m);
        }
    }
}
void genRook(pieceNode* piece, int listIndex, moveNode** move, int depth) {
    // save values of piece in question since pieceList will be iterated through
    int pos = piece[listIndex].index;
    int p = piece[listIndex].player;
    int discovered;
    int directions[] = {-12, -1, 1, 12};
    for (int i = 0; i < 4; i++) {
        int newIndex = pos + (directions[i]);
        int m = 1, capture = 0;
        while (newIndex > 25 && newIndex < 118 && newIndex % 12 > 1 && newIndex % 12 < 10) {
            int occupied = findPiece(newIndex, piece);
            if (occupied >= 0) {
                if (piece[occupied].player == p) {break;}    //occupied by self
                capture = 1;    // occupied by opponent
            }
            if (depth == 1) {
                pieceNode* simPieces = simulateMove(pos, newIndex, piece);
                discovered = findCheck(simPieces, toggle(p));
                freePieces(simPieces);  // free the simulated list
            } else {discovered = 0;}
            if (!discovered) {addMove(move, newIndex);}
            if (capture) {break;}
            m++;
            newIndex = pos + (directions[i] * m);
        }
    }
}
void genBishop(pieceNode* piece, int listIndex, moveNode** move, int depth) {
    // save values of piece in question since pieceList will be iterated through
    int pos = piece[listIndex].index;
    int p = piece[listIndex].player;
    int discovered;
    int directions[] = {-13, -11, 11, 13};
    for (int i = 0; i < 4; i++) {
        int newIndex = pos + (directions[i]);
        int m = 1, capture = 0;
        while (newIndex > 25 && newIndex < 118 && newIndex % 12 > 1 && newIndex % 12 < 10) {
            int occupied = findPiece(newIndex, piece);
            if (occupied >= 0) {
                if (piece[occupied].player == p) {break;}    //occupied by self
                capture = 1;    // occupied by opponent
            }
            if (depth == 1) {
                pieceNode* simPieces = simulateMove(pos, newIndex, piece);
                discovered = findCheck(simPieces, toggle(p));
                freePieces(simPieces);  // free the simulated list
            } else {discovered = 0;}
            if (!discovered) {addMove(move, newIndex);}
            if (capture) {break;}
            m++;
            newIndex = pos + (directions[i] * m);
        }
    }
}
void genKnight(pieceNode* piece, int listIndex, moveNode** move, int depth) {
    // save values of piece in question since pieceList will be iterated through
    int pos = piece[listIndex].index;
    int p = piece[listIndex].player;
    int discovered;
    int directions[] = {-14, -25, -23, -10, 14, 25, 23, 10};
    for (int i = 0; i < 8; i++) {
        int newIndex = pos + (directions[i]);
        // final OR case: piece either points to piece in the way or itself. Add move if piece->player = toggle(p)
        if (newIndex > 25 && newIndex < 118 && newIndex % 12 > 1 && newIndex % 12 < 10) {
            int occupied = findPiece(newIndex, piece);
            if (occupied >= 0 && piece[occupied].player == p) {continue;}   // occupied by self
            if (depth == 1) {
                pieceNode* simPieces = simulateMove(pos, newIndex, piece);
                discovered = findCheck(simPieces, toggle(p));
                freePieces(simPieces);  // free the simulated list
            } else {discovered = 0;}
            if (!discovered) {addMove(move, newIndex);}
        }
    }
}
void genPawn(pieceNode* piece, int listIndex, moveNode** move, int depth) {
    // save values of piece in question since pieceList will be iterated through
    int pos = piece[listIndex].index;
    int p = piece[listIndex].player;
    int discovered, occupied;
    /*  findCheck is looking for attacks on king. attacks on king = any move landing on king
    - depth makes it so that all moves are added, even if they discover check on themself
    - for all pieces, set discovered = 0 since discovered checks don't matter
    - but for pawns, set discovered = 1 on advance and double advance since those moves cannot be used to attack the king
    */
    if (p == 1) {
        // normal advance
        if (pos > 25 && findPiece(pos - 12, piece) < 0) {
            if (depth == 1) {
                pieceNode* simPieces = simulateMove(pos, pos - 12, piece);
                discovered = (depth > 1) ? 1 : findCheck(simPieces, toggle(p));
                freePieces(simPieces);  // free the simulated list
            } else {discovered = 0;}
            if (!discovered) {addMove(move, pos - 12);}
        }
        // en passant & captures. if enPassant square set, target square MUST already be empty, no need to check
        // findPiece will return 1 if occupied, with piece now pointing to the occupying piece
        occupied = findPiece(pos - 13, piece);
        if (pos - 1 == piece->flags->enPassant || (occupied >= 0 && piece[occupied].player == 2)) {
            if (depth == 1) {
                pieceNode* simPieces = simulateMove(pos, pos - 13, piece);
                discovered = (depth > 1) ? 0 : findCheck(simPieces, toggle(p));
                freePieces(simPieces);  // free the simulated list
            } else {discovered = 0;}
            if (!discovered) {addMove(move, pos - 13);}
        }
        occupied = findPiece(pos - 11, piece);
        if (pos + 1 == piece->flags->enPassant || (occupied >= 0 && piece[occupied].player == 2)) {
            if (depth == 1) {
                pieceNode* simPieces = simulateMove(pos, pos - 11, piece);
                discovered = (depth > 1) ? 0 : findCheck(simPieces, toggle(p));
                freePieces(simPieces);  // free the simulated list
            } else {discovered = 0;}
            if (!discovered) {addMove(move, pos - 11);}
        }
        // double advance
        if (pos > 97 && pos < 106 && findPiece(pos - 24, piece) < 0 && findPiece(pos - 12, piece) < 0) {
            if (depth == 1) {
                pieceNode* simPieces = simulateMove(pos, pos - 24, piece);
                discovered = (depth > 1) ? 1 : findCheck(simPieces, toggle(p));
                freePieces(simPieces);  // free the simulated list
            } else {discovered = 0;}
            if (!discovered) {addMove(move, pos - 24);}
        }
    } else {
        // normal advance
        if (pos < 118 && findPiece(pos + 12, piece) < 0) {
            if (depth == 1) {
                pieceNode* simPieces = simulateMove(pos, pos + 12, piece);
                discovered = (depth > 1) ? 1 : findCheck(simPieces, toggle(p));
                freePieces(simPieces);  // free the simulated list
            } else {discovered = 0;}
            if (!discovered) {addMove(move, pos + 12);}
        }
        // en passant & captures. if enPassant square set, target square MUST already be empty, no need to check
        // findPiece will return 1 if occupied, with piece now pointing to the occupying piece
        occupied = findPiece(pos + 11, piece);
        if (pos - 1 == piece->flags->enPassant || (occupied >= 0 && piece[occupied].player == 1)) {
            if (depth == 1) {
                pieceNode* simPieces = simulateMove(pos, pos + 11, piece);
                discovered = (depth > 1) ? 0 : findCheck(simPieces, toggle(p));
                freePieces(simPieces);  // free the simulated list
            } else {discovered = 0;}
            if (!discovered) {addMove(move, pos + 11);}
        }
        occupied = findPiece(pos + 13, piece);
        if (pos + 1 == piece->flags->enPassant || (occupied >= 0 && piece[occupied].player == 1)) {
            if (depth == 1) {
                pieceNode* simPieces = simulateMove(pos, pos + 13, piece);
                discovered = (depth > 1) ? 0 : findCheck(simPieces, toggle(p));
                freePieces(simPieces);  // free the simulated list
            } else {discovered = 0;}
            if (!discovered) {addMove(move, pos + 13);}
        }
        // double advance
        if (pos > 37 && pos < 46 && findPiece(pos + 24, piece) < 0 && findPiece(pos + 12, piece) < 0) {
            if (depth == 1) {
                pieceNode* simPieces = simulateMove(pos, pos + 24, piece);
                discovered = (depth > 1) ? 1 : findCheck(simPieces, toggle(p));
                freePieces(simPieces);  // free the simulated list
            } else {discovered = 0;}
            if (!discovered) {addMove(move, pos + 24);}
        }
    }
}
void genKing(pieceNode* piece, int listIndex, moveNode** move, int depth) {
    // save values of piece in question since pieceList will be iterated through
    int pos = piece[listIndex].index;
    int p = piece[listIndex].player;
    int discovered;
    int directions[] = {-1, -13, -12, -11, 1, 13, 12, 11};
    for (int i = 0; i < 8; i++) {
        int newIndex = pos + (directions[i]);
        // !findPiece will iterate through, possibly incompletely, the entire list. But we don't need the initial head pointer anymore
        // final OR case: piece either points to piece in the way or itself. Add move if piece->player = toggle(p)
        if (newIndex > 25 && newIndex < 118 && newIndex % 12 > 1 && newIndex % 12 < 10) {
            int occupied = findPiece(newIndex, piece);
            if (occupied >= 0 && piece[occupied].player == p) {continue;}   // occupied by self
            if (depth == 1) {
                pieceNode* simPieces = simulateMove(pos, newIndex, piece);
                discovered = findCheck(simPieces, toggle(p));
                freePieces(simPieces);  // free the simulated list
            } else {discovered = 0;}
            if (!discovered) {addMove(move, newIndex);}
        }
    }
    // add valid castles
    if (!(piece->flags->check)) {
        switch(p) {
            case 1: 
                if (piece->flags->wCastleE && findPiece(115, piece) < 0 && findPiece(116, piece) < 0) {addMove(move, 116);}
                if (piece->flags->wCastleW && findPiece(113, piece) < 0 && findPiece(112, piece) < 0 && findPiece(111, piece) < 0) {addMove(move, 112);}
                break;
            default:
                if (piece->flags->bCastleE && findPiece(31, piece) < 0 && findPiece(32, piece) < 0) {addMove(move, 32);}
                if (piece->flags->bCastleW && findPiece(29, piece) < 0 && findPiece(28, piece) < 0 && findPiece(27, piece) < 0) {addMove(move, 28);}
        }
    }
}