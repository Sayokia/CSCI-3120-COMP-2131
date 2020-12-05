#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// function to display the whole board
void display(int board[3][3]){
    for (int i = 0; i < 3 ; i++) {
        if(i>0){
            printf("\n---|---|---\n");
        }

        for (int j = 0; j < 3; j++) {

            switch (board[i][j]){
                case -1:printf("   ");break;
                case 0:printf(" X ");break;
                case 1:printf(" O ");break;
            }
            if (j<2){
                printf("|");
            }
        }

    }
}

// Function check if there is a winner
int check(int board[3][3]){
    // Check rows
    for(int i=0; i<3; i++) {
        if (board[i][0] == board[i][1] &&
            board[i][2] == board[i][1]) {
            return board[i][0];
        }
    }
    //check column
    for(int i=0; i<3; i++) {
        if (board[0][i] == board[1][i] &&
            board[1][i] == board[2][i]) {
            return board[0][i];
        }
    }
    //check diagonal
    if(board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        return board[0][0];
    }
    if(board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        return board[0][2];
    }

    return -1;
}

int main() {
    // Ask user's name
    char name[100];
    printf("What is your name?\n");
    scanf("%s",&name);
    printf("Hello %s ... Let's have fun !!! You = X | Computer = O\n", name);

    //initialize board and turn
    //in board -1 represents empty; 0 represents player; 1 represents computer
    int turn, board[3][3];
    turn = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = -1;
        }
    }

    // display the empty board for user
    display(board);

    // The maximum turn is five
    while (turn<5){
        // use userValid to check if the move is valid
        int userValid =0;
        //ask user to get the x,y coordinates for his next move
        int x,y;
        printf("\nEnter X,Y coordinates for your move:\n");
        scanf("%d%d",&x,&y);
        if(x>3 || x<0 || y>3 || x<0){
            printf("Input out bound! Please enter a number between 0 and 3\n");
            userValid = -1;
        }
        if (userValid ==0){
            //check if the move is valid
            if(board[x][y]!=-1){
                printf("Invalid move! Please move to an empty space!\n");
                userValid = -1;
            }
            else{
                board[x][y] = 0;
                userValid = 0;
            }

            // check the result after each move
            int result = check(board);
            switch (result){
                case 0:printf("\n******\nYou win!\n******\n");display(board);printf("\n");return -1;
                case 1:printf("\n******\nYou lose!\n******\n");display(board);printf("\n");return -1;
            }

        }



        // check if computer move is valid
        int computerValid = -1;

        //When it comes to the fifth turn, the turn counter should be 4 at that time
        // there should be 2*4 = 8 marks in the board, therefore, there should not be a computer
        // move in the fifth turn
        if (userValid == 0 && turn !=4){
            // generate two random number n and m for the coordinates of computer move
            int n,m;
            srand( (unsigned)time( NULL ) );
            n = rand()%3;
            m = rand()%3;

            // if the position is not empty then the move if not valid
            if (board[n][m]!=-1){
                computerValid = -1;
            }
            else{
                board[n][m]= 1;
                computerValid = 0;
            }


            // when the move is not valid, there is a loop to generate a new pair of coordinates
            // until the move is valid
                while (computerValid == -1){
                    srand( (unsigned)time( NULL ) );
                    n = rand()%3;
                    m = rand()%3;
                    // if the position is not empty then the move if not valid
                    if (board[n][m]!=-1){
                        computerValid = -1;
                    }
                    else{
                        board[n][m]= 1;
                        computerValid = 0;
                    }
            }
        }


        // We go to next turn only when both user and computer move are valid
        if (userValid == 0 && computerValid ==0){
            turn++;


            // check the result after each move
            int result = check(board);
            switch (result){
                case 0:printf("\n******\nYou win!\n******\n");display(board);printf("\n");return -1;
                case 1:printf("\n******\nYou lose!\n******\n");display(board);printf("\n");return -1;
            }
            display(board);
        }




    }
    // It is draw if no winner comes out after five turns
    printf("\n******\nDraw!\n******\n");
}