#ifndef LOGFILE_H
#define LOGFILE_H
#include <stdio.h>
#include "piece.h"
void logFlag(int flag);
void logMove(int p, int i, pieceNode* piece, char initial[], char final[]);
void logResult(int result);
void resetLog();
#endif