// Autor: Sander Lääts - 2021

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "display.h"

#ifdef WIN32
#define OS "Windows"

#include <windows.h>

#else
#define OS " "
#endif

//############################################################# Kuvamis funktsioonid ###################################


// Funktsioon, mille abil väljastame ekraanile mängulaua
// Kui oleme windows masinas, saame kuvada värvilist lauda
void printBoard(char board[][10]) {
    if (strcmp(OS, "Windows") == 0) {
        printBoardWindows(board);
    } else {
        printBoardDefault(board);
    }
}//printBoard

// Funktsioon, mille abil kuvame legendi, mis tähendab mida mängulaual
void printLegend(void) {

    // Kui oleme windows masinas,
    if (strcmp(OS, "Windows") == 0) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        printf("\n\n\tGame board icons:");
        printf("\n\t\t* '");
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | BACKGROUND_GREEN);
        printf("#");
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        printf("' - your ship\n\t\t* '");
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | BACKGROUND_BLUE);
        printf("~");
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        printf("' - water\n\t\t* '");
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | BACKGROUND_BLUE);
        printf("?");
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        printf("' - unknown tile\n\t\t* '");
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE |
                                          BACKGROUND_GREEN | BACKGROUND_INTENSITY);
        printf("0");
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        printf("' - bombed water\n\t\t* '");
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | BACKGROUND_RED);
        printf("X");
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        printf("' - bombed ship\n");

        // Kui me pole windows masinas
    } else {
        printf("\n\n\tGame board icons:"
               "\n\t\t* '#' - your ship"
               "\n\t\t* '~' - water"
               "\n\t\t* '?' - unknown tile"
               "\n\t\t* '0' - bombed water"
               "\n\t\t' 'X' - bombed ship\n");
    }
}//printLegend

// Funktsioon värvilise mängulaua kuvamiseks
void printBoardWindows(char board[][10]) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);

    printf("\n    | A B C D E F G H I J");
    putchar('\n');
    for (int i = 0; i < 10; i++) {
        printf("%s""%d""%s", "   ", i, "| ");
        for (int j = 0; j < 10; j++) {

            if (board[i][j] == '#') {

                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | BACKGROUND_GREEN);
                printf("%c", board[i][j]);
                printf(" ");

            }

            if (board[i][j] == '~') {
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | BACKGROUND_BLUE);
                printf("%c", board[i][j]);
                printf(" ");
            }

            if (board[i][j] == 'X') {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | BACKGROUND_RED);
                printf("%c", board[i][j]);
                printf(" ");


            }
            if (board[i][j] == '?') {
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | BACKGROUND_BLUE);
                printf("%c", board[i][j]);
                printf(" ");
            }

            if (board[i][j] == '0') {
                SetConsoleTextAttribute(hConsole,
                                        FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE |
                                        BACKGROUND_GREEN | BACKGROUND_INTENSITY);
                printf("%c", board[i][j]);
                printf(" ");
            }

            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        }
        putchar('\n');
    }
}//printBoardWindows

// Tavalise mängulaua kuvamiseks
void printBoardDefault(char board[][10]) {
    printf("\n    | A B C D E F G H I J");
    putchar('\n');
    for (int i = 0; i < 10; i++) {
        printf("%s""%d""%s", "   ", i, "| ");
        for (int j = 0; j < 10; j++) {
            printf("%c", board[i][j]);
            printf(" ");
        }
        putchar('\n');
    }
}//printBoardDefault

// Funktsioon, mille abil väljastame ekraanile laevade hetkeseisu
void printPlanningAmounts(int shipLens[], int shipAmounts[]) {
    // Näitame laevade indekseid, pikkuseid ja koguseid, lähme tsüklist välja kui kõik laevad on asetatud
    printf("\nships:\t(index)Ship name(size) - amount\n\t(0)Carrier(%d) - %d", shipLens[0], shipAmounts[0]);
    printf("\n\t(1)Battleship(%d) - %d", shipLens[1], shipAmounts[1]);
    printf("\n\t(2)Cruiser(%d) - %d", shipLens[2], shipAmounts[2]);
    printf("\n\t(3)destroyer(%d) - %d", shipLens[3], shipAmounts[3]);
    printf("\n\t(4)submarine(%d) - %d", shipLens[4], shipAmounts[4]);
    printf("\n\n");
}//printPlanningAmounts

// Funktsioon, mis kuvab vaenlase mängulaua
void showEnemyDisplay(char enemyDisplay[][10]) {
    printBoard(enemyDisplay);
}//showEnemyDisplay

// Funktsioon mis kuvab menüüst valitud 'about' valiku, ehk siis programmi kohta info
void displayAbout(void) {
    printf("\nThis game is made by Sander L%c%cts - as a project in the Tartu University course 'Programming in C and assembler"
           "\n\nThe game functions a little bit differently than how it is usually played,"
           "\nfor example, in this game there isn't any info given when one whole ship has been bombed,"
           "\nthat means that neither of the players know that info, so it still is fair."
           "\nThis game also features singleplayer mode, where you can choose between 2 difficulties:"
           "\n\t* Easy - Computer chooses what points to attack randomly"
           "\n\t* Intermediate - Computer chooses what points to attack depending on"
           "\n\t  which coordinates have been sucessfully bombed\n"
           "\n\nIf you're having issues with commands:"
           "\nCommands like 'put' and 'rand' shouldn't have coordinates typed right after them, it will be asked separately,"
           "\nthis also goes for battle mode."
           "\nYou can always try the 'help' command if you're stuck!"
           "\n\n('enter' to exit)", 132, 132);
    getc(stdin);
    system("cls");
    fflush(stdin);
}//displayAbout

// Lahingu ajal antud käsu 'help' kuvamine
void displayBattleHelp(void) {
    system("cls");
    printf("\n\tCommands in battle mode:\n\t* 'LETTER|NUMBER' - Bomb given position\n\t"
           "* 'help' - Show commands in battle mode\n\t* 'lgnd' - Show game board legend"
           "\n\t* 'exit' - exit to main menu\n");
    fflush(stdin);
    printf("\n\t(enter)\n");
    getc(stdin);
    system("cls");
    fflush(stdin);
}//displayBattleHelp