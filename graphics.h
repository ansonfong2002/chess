#include "graphics.h"

/*Global Variables */
GtkWidget *window;  
GtkWidget *fixed, *fixed2; 
GtkWidget *chess_icon; 
GtkWidget *table, *table2, *table3; 
GtkWidget *box;
char clickPos[3];
/*  Player 1 = white, 2 = black
    Click = 1 or 2
    Mode 1 = PVP, 2 = PVC
*/
int turn, player, AI, click, mode, result = 0;
// gameboard, gametree
pieceNode* gameBoard;
gameList* gameStates;
moveInfo info = (moveInfo){0, 0};
//enum GRID Board[8][8];
enum GRID Board[8][8] = {
    {ROOK_b, PAWN_b, BISHOP_b, QUEEN_b, KING_b, BISHOP_b, KNIGHT_b, ROOK_b},
    {KNIGHT_b, PAWN_b, PAWN_b, PAWN_b, PAWN_b, PAWN_b, PAWN_b, PAWN_b},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {PAWN_w, PAWN_w, PAWN_w, PAWN_w, PAWN_w, PAWN_w, PAWN_w, PAWN_w},
    {ROOK_w, KNIGHT_w, BISHOP_w, QUEEN_w, KING_w, BISHOP_w, KNIGHT_w, ROOK_w}
};
void UpdateBoard(char backendboard[64][3]) {
//updates enum Board given the backend board
    gtk_container_remove(GTK_CONTAINER(window), fixed); 
    table = gtk_table_new (8, 8, TRUE);
    gtk_widget_set_size_request (table, BOARD_WIDTH, BOARD_HEIGHT);
    int i,j;
    for (i = 0; i < 8; i++) {
	    for (j = 0; j < 8; j++){
		    if (strcmp(backendboard[(i*8)+ j], "wk") == 0){Board[i][j] = KING_w;}
		    else if (strcmp(backendboard[(i*8)+ j], "bk") == 0){Board[i][j] = KING_b;}
            else if (strcmp(backendboard[(i*8)+ j], "wq") == 0){Board[i][j] = QUEEN_w;}
            else if (strcmp(backendboard[(i*8)+ j], "bq") == 0){Board[i][j] = QUEEN_b;}
            else if (strcmp(backendboard[(i*8)+ j], "wb") == 0){Board[i][j] = BISHOP_w;}
            else if (strcmp(backendboard[(i*8)+ j], "bb") == 0){Board[i][j] = BISHOP_b;}
            else if (strcmp(backendboard[(i*8)+ j], "wn") == 0){Board[i][j] = KNIGHT_w;}
            else if (strcmp(backendboard[(i*8)+ j], "bn") == 0){Board[i][j] = KNIGHT_b;}
            else if (strcmp(backendboard[(i*8)+ j], "wr") == 0){Board[i][j] = ROOK_w;}
            else if (strcmp(backendboard[(i*8)+ j], "br") == 0){Board[i][j] = ROOK_b;}
            else if (strcmp(backendboard[(i*8)+ j], "wp") == 0){Board[i][j] = PAWN_w;}
            else if (strcmp(backendboard[(i*8)+ j], "bp") == 0){Board[i][j] = PAWN_b;}
		    else {Board[i][j] = EMPTY;}
        }
	} 
}
char* getClick(int x, int y) {
    int x_cal = (x - 10) / 66;
    switch(x_cal) {
    	case 0: clickPos[0] = 'A';
            break;
        case 1: clickPos[0] = 'B';
            break;
        case 2: clickPos[0] = 'C';
            break;
        case 3: clickPos[0] = 'D';
            break;
        case 4: clickPos[0] = 'E';
            break;
        case 5: clickPos[0] = 'F';
            break;
        case 6: clickPos[0] = 'G';
            break;
        case 7: clickPos[0] = 'H';
    }
    int y_cal = (8 - ((y - 10) / 66)) + 48;
    char c = y_cal;
    clickPos[1] = c;
    return clickPos;
}
static gboolean on_delete_event (GtkWidget *widget, GdkEvent  *event, gpointer data) {
    g_print ("Window closed.\n");
    if (result != 0) {
        if (mode == 2) {printf("Player %d wins.\n", AI);}
        else {printf("Player %d wins.\n", player);}
    }
    gtk_main_quit();
    return TRUE;
}
void DrawBoard() {
	int i, j;
	for(i = 0; i < 8; i++) { 
		for(j = 0; j < 8; j++) {
			switch(Board[i][j]) {
				case KING_w:
					if ((i % 2 == 0  && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) {
		                chess_icon = gtk_image_new_from_file("./icons2/WhiteKing_White.png");
            		} else {
		                chess_icon = gtk_image_new_from_file("./icons2/WhiteKing_Black.png");
                	}
					break;
				case KING_b:
					if ((i % 2 == 0  && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) {
                        chess_icon = gtk_image_new_from_file("./icons2/BlackKing_White.png");
                    } else {
                        chess_icon = gtk_image_new_from_file("./icons2/BlackKing_Black.png");
                    }
     				break;
				case QUEEN_w:
					if ((i % 2 == 0  && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) {
                        chess_icon = gtk_image_new_from_file("./icons2/WhiteQueen_White.png");
                    } else {
                        chess_icon = gtk_image_new_from_file("./icons2/WhiteQueen_Black.png");
                    }
                    break;
				case QUEEN_b:
                    if ((i % 2 == 0  && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) {
                        chess_icon = gtk_image_new_from_file("./icons2/BlackQueen_White.png");
                    } else {
                        chess_icon = gtk_image_new_from_file("./icons2/BlackQueen_Black.png");
                    }
                    break;
				case BISHOP_w:
					if ((i % 2 == 0  && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) {
                        chess_icon = gtk_image_new_from_file("./icons2/WhiteBishop_White.png");
                    } else{
                        chess_icon = gtk_image_new_from_file("./icons2/WhiteBishop_Black.png");
                    }
                    break;
        		case BISHOP_b:
					if ((i % 2 == 0  && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) {
                        chess_icon = gtk_image_new_from_file("./icons2/BlackBishop_White.png");
                    } else {
                        chess_icon = gtk_image_new_from_file("./icons2/BlackBishop_Black.png");
                    }
                    break;
                case KNIGHT_w:
					if ((i % 2 == 0  && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)){
                        chess_icon = gtk_image_new_from_file("./icons2/WhiteKnight_White.png");
                    } else {
                        chess_icon = gtk_image_new_from_file("./icons2/WhiteKnight_Black.png");
                    }
                    break;
                case KNIGHT_b:
					if ((i % 2 == 0  && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) {
                        chess_icon = gtk_image_new_from_file("./icons2/BlackKnight_White.png");
                    } else {
                        chess_icon = gtk_image_new_from_file("./icons2/BlackKnight_Black.png");
                    }
                    break;                           
          		case ROOK_b:
					if ((i % 2 == 0  && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) {
                        chess_icon = gtk_image_new_from_file("./icons2/BlackRook_White.png");
                    } else {
                        chess_icon = gtk_image_new_from_file("./icons2/BlackRook_Black.png");
                    }
                    break;
        		case ROOK_w:
					if ((i % 2 == 0  && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) {
                        chess_icon = gtk_image_new_from_file("./icons2/WhiteRook_White.png");
                    } else {
                        chess_icon = gtk_image_new_from_file("./icons2/WhiteRook_Black.png");
                    }
                    break;
         		case PAWN_w:
					if ((i % 2 == 0  && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) {
                        chess_icon = gtk_image_new_from_file("./icons2/WhitePawn _White.png");
                    } else {
                        chess_icon = gtk_image_new_from_file("./icons2/WhitePawn_Black.png");
                    }
                    break;
        		case PAWN_b:
					if ((i % 2 == 0  && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) {
                        chess_icon = gtk_image_new_from_file("./icons2/BlackPawn_White.png");
                    } else {
                        chess_icon = gtk_image_new_from_file("./icons2/BlackPawn_Black.png");
                    }
                    break; 
				default:
					if ((i % 2 == 0  && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) { 
						chess_icon = gtk_image_new_from_file("./icons2/white_square@2x.png");
					} else {
						chess_icon = gtk_image_new_from_file("./icons2/black_square@2x.png");
					}
					break;			
			}		
			gtk_table_attach(GTK_TABLE(table), chess_icon, j, j + 1, i, i + 1, GTK_FILL, GTK_FILL, 0, 0);
		}
	}
    fixed = gtk_fixed_new();
    gtk_fixed_put(GTK_FIXED(fixed), table, 0, 0);
    gtk_container_add(GTK_CONTAINER(window), fixed);
  
    gtk_widget_show_all(window);
}
void outputBoard(pieceNode* pieceList) {
    char board[64][3];
    for (int i = 0; i < 64; i++) {strcpy(board[i], "  ");}  // fill with blanks
    for (int x = 0; x < 32; x++) {
        if (pieceList[x].index) { // captured pieces have index = 0
            board[map(pieceList[x].index)][0] = toColor(pieceList[x].player);
            board[map(pieceList[x].index)][1] = toName(pieceList[x].type); 
        }
    }
    UpdateBoard(board);
    DrawBoard();
}
void processPlayerClick(int coord_x, int coord_y) {
    if (result != 0) {return;}
    int listIndex, sel;
    printf("Click %d:\n", click);
    if (click == 1) {   // first click
        info.initial = getPos(getClick(coord_x, coord_y));
        listIndex = findPiece(info.initial, gameBoard);
        if (listIndex < 0 || gameBoard[listIndex].player != player) {return;} // invalid square, do nothing
        printf("%s\n", getClick(coord_x, coord_y));
        click = 2;  // valid square, switch click
    } else {            // second click
        listIndex = findPiece(info.initial, gameBoard);
        info.target = getPos(getClick(coord_x, coord_y));
        moveNode* moves = generateMoves(gameBoard, listIndex, 1);
        while (moves != NULL) {
            moveNode* next = moves->nextNode;
            if (info.target == moves->target) { // if valid
                char initialStr[3], targetStr[3];
                toRankFile(info.initial, initialStr);
                toRankFile(info.target, targetStr);
                printf("%s\n", getClick(coord_x, coord_y));
                logMove(player, listIndex, gameBoard, initialStr, targetStr);   // log move to log.txt
                int occupied = findPiece(info.target, gameBoard);
                // log captures: target square occupied or pawn moves diagonally
                if (occupied >= 0 || (gameBoard[listIndex].type == PAWN && (abs(info.initial - info.target) == 13 || abs(info.initial - info.target) == 11) )) {logFlag(6);}
                if (gameBoard[listIndex].type == PAWN && ((info.target > 25 && info.target < 34) || (info.target > 109 && info.target < 119))) {
                    printf("Select promotion: N(1), B(2), R(3), Q(4)\n");
                    scanf("%d", &sel);
                    logFlag(sel);
                    gameBoard[listIndex].type = sel;
                } else if (gameBoard[listIndex].type == KING && (abs(info.initial - info.target) == 2)) {logFlag(5);}   // log castle
                freeMoves(moves);
                move(gameBoard, info.initial, info.target); // move
                outputBoard(gameBoard);     // updates and displays board
                logBoard(gameBoard, &gameStates);   // record position in gameTree
                player = toggle(player);    // switch players
                click = 1;     // reset click
                turn++;
                result = findMate(gameBoard, player);   // get result
                if (findTie(gameStates)) {result = 3;}
                if (result != 0) {return;}
                if (mode == 2) {
                    printf("Processing...\n");
                    processAIMove();
                    turn++;
                    if (result != 0) {return;}
                }
                return;
            }
            free(moves);
            moves = next;
        }
        click = 1;
    }
}
void processAIMove() {
    if (turn < 3) {
        if (player == 1) {
            info.initial = 101;
            info.target = 77;
        } else {
            info.initial = 41;
            info.target = 65;
        }
    } else if (turn < 5) {
        if (player == 1) {
            info.initial = 116;
            info.target = 91;
        } else {
            info.initial = 32;
            info.target = 55;
        }
    } else {
        double score = 0;
        treeNode* root = buildTree(gameBoard, 4, player, &score);
        children* moves = root->branches;
        while (moves->node->score != score) {moves = moves->nextChild;} // find the move that gave us bestScore
        info.initial = moves->node->info.initial;
        info.target = moves->node->info.target;
    }
    char position1[3], position2[3];
    toRankFile(info.initial, position1);
    toRankFile(info.target, position2);
    int listIndex = findPiece(info.initial, gameBoard);
    int occupied = findPiece(info.target, gameBoard);   // target occupied?
    logMove(player, listIndex, gameBoard, position1, position2); // log move in logfile
    // log capture
    if (occupied >= 0 || (gameBoard[listIndex].type == PAWN && (abs(info.initial - info.target) == 13 || abs(info.initial - info.target) == 11))) {logFlag(6);}
    // log promotion to queen
    if (gameBoard[listIndex].type == PAWN && ((info.target > 25 && info.target < 34) || (info.target > 109 && info.target < 119))) {
        gameBoard[listIndex].type = QUEEN;
        logFlag(4);
    } else if (gameBoard[listIndex].type == KING && (abs(info.initial - info.target) == 2)) {logFlag(5);}   // log castle
    move(gameBoard, info.initial, info.target);  // make move
    outputBoard(gameBoard);
    logBoard(gameBoard, &gameStates);   // record position in gameTree 
    player = toggle(player);
    result = findMate(gameBoard, player);   // get result
    if (findTie(gameStates)) {result = 3;}
    printf("Moved %s to %s\n", position1, position2);
}
gint area_click (GtkWidget *widget, GdkEvent *event, gpointer data) { 
	if (result != 0) {return TRUE;}
    GdkModifierType state;
    int coord_x, coord_y;
	gdk_window_get_pointer(widget->window, &coord_x, &coord_y, &state); 

    processPlayerClick(coord_x, coord_y);

    // result is reached
    if (result != 0) {
        outputBoard(gameBoard);
        freePieces(gameBoard);
        freeStates(gameStates);
        // returns from gtk main when result reached
        switch(result) {
            case 1: printf("Player 1 wins.\n");
                logResult(result);
                break;
            case 2: printf("Player 2 wins.\n");
                logResult(result);
                break;
            case 3: printf("Stalemate.\n");
                logResult(result);
        }
    }
    return TRUE; 
}
void StartUp(void) { 
    //create a new window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window, WINDOW_WIDTH, WINDOW_HEIGHT); 
	gtk_container_set_border_width (GTK_CONTAINER(window), WINDOW_BORDER); 
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); 
	gtk_window_set_title(GTK_WINDOW(window), "Chess Nutz Chess Game"); 
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE); 	
	gtk_window_fullscreen(GTK_WINDOW(window)); 

    //register event handlers
	g_signal_connect(window, "delete_event", G_CALLBACK(on_delete_event), NULL); 
	gtk_widget_set_events(window, GDK_BUTTON_PRESS_MASK); 
	g_signal_connect(window, "button_press_event", G_CALLBACK(area_click), NULL); 

    //create a table and draw the board
	table = gtk_table_new (8, 8, TRUE); 
	gtk_widget_set_size_request (table, BOARD_WIDTH, BOARD_HEIGHT);
    //draws the initial board
    turn = 1;   // first turn
    player = 1; // first player goes first
    resetLog(); // set log to empty file
    gameBoard = generateBoard();
    outputBoard(gameBoard);
    gameStates = NULL;
    addState(copyBoard(gameBoard), &gameStates);
    if (mode == 2 && AI == 1) {
        processAIMove();
        player = 2;
        turn++;
    }
    click = 1;  // first click
    // enter event-driven game loop
	gtk_main(); 
}
int main(int argc,char *argv[]) {
    gtk_init(&argc, &argv);
    // configurate
    int sel;
    printf("Choose mode:\n(1) PVP\n(2) PVC\n");
    scanf("%d", &sel);
    if (sel == 1) {mode = 1;} 
    else {
        mode = 2;
        printf("Select a color:\n(1) White\n(2) Black\n");
        scanf("%d", &sel);
        AI = toggle(sel);
    }
    // game loop
    StartUp();
}