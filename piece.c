#include "piece.h"
void addPiece(pieceNode* list, int i, flagVars* f, enum PieceTypes t, int color, int position) {
    list[i].flags = f;
    list[i].index = position;
    list[i].type = t;
    list[i].player = color;
}
void freePieces(pieceNode* list) {
    free(list->flags);
    free(list);
}
int toggle(int num) {
    return (num == 1) ? 2 : 1;
}
int getPos(char position[3]) {
	int pos = position[0] - 39;					// converts ASCII files (A-H) into int columns (26-33)
	int rank = position[1] - '0';				// converts char ranks (1-8) into integers
	return pos + (8 - rank) * 12;
}
pieceNode* generateBoard() {
    // initializes flags
    flagVars* flags = (flagVars*)malloc(sizeof(flagVars));
    flags->bCastleE = 1;
    flags->bCastleW = 1;
    flags->wCastleE = 1;
    flags->wCastleW = 1;
    flags->enPassant = 0;
    flags->check = 0;
    // creates pieceList pointer
    enum PieceTypes initial[] = {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK};
    pieceNode* pieceList = (pieceNode*)malloc(32 * sizeof(pieceNode));
    for (int i = 0; i < 16; i++) {
        if (i < 8) {
            addPiece(pieceList, i, flags, PAWN, 2, 38 + i);
            addPiece(pieceList, i + 16, flags, initial[i], 2, 26 + i);
        } else {
            addPiece(pieceList, i, flags, PAWN, 1, 90 + i); // 98 + i - 8
            addPiece(pieceList, i + 16, flags, initial[i - 8], 1, 102 + i); // 110 + i - 8
        }
    }
	return pieceList;
}
void printBoard(pieceNode* pieceList, int orientation) {
    // board representation for output
	char board[144][3];
    // fills board with blanks
    for (int i = 0 ; i < 144; i++) {strcpy(board[i], "  "); }
    // fills board with pieces
    for (int x = 0; x < 32; x++) {
		// captured pieces will have index = 0
        if(pieceList[x].index) {
            board[pieceList[x].index][0] = toColor(pieceList[x].player);
            board[pieceList[x].index][1] = toName(pieceList[x].type); 
        }
    }
    //  prints board according to orientation
	if (orientation == 1) {
    	int file = 8;
		for (int i = 26; i < 118; i++) {
            if ((i - 9) % 12 == 0) {    // last item in row
                printf("%s |%d\n", board[i], file);
                printf("------------------------\n");
                 file--, i += 4;    // omits out of bounds indicies
            }
            else {printf("%s|", board[i]);}
        }
        printf("A |B |C |D |E |F |G |H \n");
    }
    else {
    	int file = 1;
        for (int i = 117; i > 25; i--) {
            if ((i - 2) % 12 == 0) {    // last item in row
                printf("%s|%d\n", board[i], file);
                printf("------------------------\n");
                file++, i -= 4;     // omits out of bounds indicies
            }
            else {printf("%s|", board[i]);}
        }
        printf("H |G |F |E |D |C |B |A \n");
    }   
}
char toName(enum PieceTypes type) {
    switch(type){
        case PAWN: return 'p'; 
        case KNIGHT: return 'n'; 
        case BISHOP: return 'b'; 
        case ROOK: return 'r';
        case QUEEN: return 'q';
        default: return 'k'; 
    }
}
char toColor(int player) {
    return (player==1)? 'w':'b';
}
void toRankFile(int i, char* s) {
    int mapped = map(i);
    s[1] = '0' + 8 - (mapped / 8);
    s[0] = 'A' + mapped % 8;
    s[2] = '\0';
}

int getMaterial(pieceNode* pieceList, int p) {
    int score = 0;  // score counters
    for(int i = 0; i < 32; i++) {
		// captured pieces have index = 0
        if(pieceList[i].index && pieceList[i].player == p) {
				if (pieceList[i].type == PAWN) {score++;}
                else if (pieceList[i].type == KNIGHT || pieceList[i].type == BISHOP) {score+=3;}
                else if (pieceList[i].type == ROOK) {score+=5;}
                else if (pieceList[i].type == QUEEN) {score+=9;}
        }
    }
    return score;
}
int findPiece(int pos, pieceNode* pieceList) {
    for (int i = 0; i < 32; i++) {
        if (pos == pieceList[i].index) {return i;} // returns index of piece within pieceList
    }
    return -1;
}
int map(int index) {return (index % 12) - 2 + 8*((index / 12) - 2);}