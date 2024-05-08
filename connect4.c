#include <stdio.h>
#include <stdbool.h>

#define ROWS 6
#define COLUMNS 7
#define FILENAME "saved_game.txt"
#define MAX_NAME_LEN 100

void printMenu();
void printBoard(char board[ROWS][COLUMNS]);
void initializeBoard(char board[ROWS][COLUMNS]);
int getChoice();
void playGame(char board[ROWS][COLUMNS], char startingPlayer);
void loadGame(char board[ROWS][COLUMNS]);
void saveGame(char board[ROWS][COLUMNS]);
int makeMove(char board[ROWS][COLUMNS], int col, char token);
bool checkWin(char board[ROWS][COLUMNS], char token);
bool checkDraw(char board[ROWS][COLUMNS]);

int main() {
    char board[ROWS][COLUMNS];
    int choice;

    initializeBoard(board);

    do {
        printMenu();
        choice = getChoice();

        switch (choice) {
            case 1:
                playGame(board, 'X');  
                initializeBoard(board);
                break;
            case 2:
                loadGame(board);
                break;
            case 3:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);

    return 0;
}

void printMenu() {
    printf("\nConnect Four Menu\n");
    printf("1. Play New Game\n");
    printf("2. Load Game\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
}

int getChoice() {
    int choice;
    scanf("%d", &choice);
    return choice;
}

void initializeBoard(char board[ROWS][COLUMNS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            board[i][j] = '.';
        }
    }
}

void printBoard(char board[ROWS][COLUMNS]) {
    printf("\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    printf("1 2 3 4 5 6 7\n");
}

void playGame(char board[ROWS][COLUMNS], char startingPlayer) {
    char player1[MAX_NAME_LEN], player2[MAX_NAME_LEN];
    printf("Enter name for Player 1 (X): ");
    scanf(" %99[^\n]", player1); // Read up to 99 characters or until newline
    printf("Enter name for Player 2 (O): ");
    scanf(" %99[^\n]", player2);

    char token = startingPlayer;
    int col;
    bool gameOver = false;

    while (!gameOver) {
        printBoard(board);
        printf("%s's turn (Enter -1 to save and quit). Choose column (1-7) to drop '%c': ", (token == 'X' ? player1 : player2), token);
        scanf("%d", &col);

        if (col == -1) {
            saveGame(board);
            printf("Game saved and exiting...\n");
            return;
        }

        col--; 
        if (makeMove(board, col, token)) {
            if (checkWin(board, token)) {
                printBoard(board);
                printf("Congratulations %s, you have won!\n", (token == 'X' ? player1 : player2));
                gameOver = true;
            } else if (checkDraw(board)) {
                printBoard(board);
                printf("The game is a draw.\n");
                gameOver = true;
            }

            token = (token == 'X' ? 'O' : 'X'); 
        } else {
            printf("Invalid move. Try again.\n");
        }
    }
}

void loadGame(char board[ROWS][COLUMNS]){
	FILE *file = fopen(FILENAME, "r");
	if (file == NULL){
		printf("No saved game found.\n");
		return;
	}

	char token = 'X';
	int xCount = 0, oCount = 0;
	for (int i = 0; i < ROWS; i++){
		for (int j = 0; j < COLUMNS; j++){
			fscanf(file, " %c", &board[i][j]);
			if (board[i][j] == 'X') xCount++;
			if (board[i][j] == 'O') oCount++;
		}
	}
	fclose(file);
	printf("Game loaded successfully.\n");

	token = (xCount > oCount) ? 'O' : 'X';
	playGame(board, token);
}

void saveGame(char board[ROWS][COLUMNS]){
	FILE *file = fopen(FILENAME, "w");
	if (file == NULL){
		printf("Error saving game.\n");
		return;
	}
	
	for (int i = 0; i < ROWS; i++){
		for (int j = 0; j < COLUMNS; j++){
			fprintf(file, " %c", board[i][j]);
		}
		fprintf(file, "\n");
	}
	fclose(file);
}

int makeMove(char board[ROWS][COLUMNS], int col, char token) {
    if (col < 0 || col >= COLUMNS) {
        return false;
    }

    for (int i = ROWS - 1; i >= 0; i--) {
        if (board[i][col] == '.') {
            board[i][col] = token;
            return true;
        }
    }

    return false;
}

bool checkWin(char board[ROWS][COLUMNS], char token) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (j <= COLUMNS - 4 && token == board[i][j] && token == board[i][j+1] && token == board[i][j+2] && token == board[i][j+3])
                return true;
            if (i <= ROWS - 4 && token == board[i][j] && token == board[i+1][j] && token == board[i+2][j] && token == board[i+3][j])
                return true;
            if (i <= ROWS - 4 && j <= COLUMNS - 4 && token == board[i][j] && token == board[i+1][j+1] && token == board[i+2][j+2] && token == board[i+3][j+3])
                return true;
            if (i >= 3 && j <= COLUMNS - 4 && token == board[i][j] && token == board[i-1][j+1] && token == board[i-2][j+2] && token == board[i-3][j+3])
                return true;
        }
    }
    return false;
}

bool checkDraw(char board[ROWS][COLUMNS]) {
    for (int i = 0; i < COLUMNS; i++) {
        if (board[0][i] == '.') {
            return false;
        }
    }
    return true;
}
