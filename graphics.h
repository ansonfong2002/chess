#include <time.h>
#include <string.h>
#include <gtk/gtk.h>
#include "ai.h"
#include "logfile.h"

#define WINDOW_BORDER 10
#define MAX_MSGLEN 100
#define SQUARE_SIZE 66
#define BOARD_BORDER 10
#define BOARD_WIDTH  (2*SQUARE_SIZE)
#define BOARD_HEIGHT (2*SQUARE_SIZE)
#define WINDOW_WIDTH  900
#define WINDOW_HEIGHT 600

//all the options in the GRID enum
enum GRID {
	BLACK, 
	WHITE,
	KING_w,
	KING_b,
	QUEEN_w,
	QUEEN_b,
	BISHOP_w,
	BISHOP_b,
	KNIGHT_w,
	KNIGHT_b,
	ROOK_w,
	ROOK_b,
	PAWN_w,
	PAWN_b,
	EMPTY

};
//Takes in the current backend board and updates each GRID Board value (GUI board) to have an enum value based on the array given
void UpdateBoard(char backendboard[64][3]); 
// returns rank and file for xy coordinates on screen
char* getClick(int x, int y);
//loops through the Board and based on the Board value assigns the correct image to the value
void DrawBoard();
// convert pieceList into 1D array of chars representing board, updates graphics, draws board
void outputBoard(pieceNode* pieceList);
// handles player moves
void processPlayerClick(int coord_x, int coord_y);
// handles AI moves
void processAIMove();
//function that calls gtk_main_quit, allowing for window to close
static gboolean on_delete_event (GtkWidget *widget,  GdkEvent  *event, gpointer data);
//function gets user clicks and checks to see if the click is in the board, if so, sets ready = 1. Function gets called in Startup()
gint area_click (GtkWidget *widget, GdkEvent  *event,  gpointer  data);
//return a char* which calculates and returns the "A1" translation based on where the user clicked on the board
char* GetClick();
//helper function that deletes the window, gets called in Startup Function
static gboolean on_delete_event (GtkWidget *widget, GdkEvent  *event, gpointer data);
//Initializes the window, initializes the board and Draws Initial Board setup, listens for clicks, checks if user closes the window
void StartUp(void);
//Functions initializes GTK and calls Startup()
int main(int argc,char *argv[]);