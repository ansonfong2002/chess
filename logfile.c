#include "logfile.h"
void logFlag(int flag) {
    FILE* log = fopen("log.txt", "a");
    switch(flag) {
        case 1: fprintf(log, "--Promoted to Knight\n");
            break;
        case 2: fprintf(log, "--Promoted to Bishop\n");
            break;
        case 3: fprintf(log, "--Promoted to Rook\n");
            break;
        case 4: fprintf(log, "--Promoted to Queen\n");
            break;
        case 5: fprintf(log, "--Castle\n");
            break;
        case 6: fprintf(log, "--Capture\n");
    }
    fclose(log);
}
void logMove(int p, int i, pieceNode* piece, char initial[], char final[]) {
    FILE* log = fopen("log.txt", "a");
    fprintf(log, "Player %d: ", p);
    switch(piece[i].type) {
        case PAWN: fprintf(log, "Pawn ");
            break;
        case KNIGHT: fprintf(log, "Knight ");
            break;
        case BISHOP: fprintf(log, "Bishop ");
            break;
        case ROOK: fprintf(log, "Rook ");
            break;
        case QUEEN: fprintf(log, "Queen ");
            break;
        case KING: fprintf(log, "King ");
    }
    fprintf(log, "%s, %s\n", initial, final);
    fclose(log);
}
void logResult(int result) {
    FILE* log = fopen("log.txt", "a");
    switch(result) {
        case 1: fprintf(log, "Player 1 wins\n");
            break;
        case 2: fprintf(log, "Player 2 wins\n");
            break;
        case 3: fprintf(log, "Stalemate\n");
    }
    fclose(log);
}
void resetLog() {
    FILE* log = fopen("log.txt", "w");
    fprintf(log, "Chess Log File\n");
    fclose(log);
}