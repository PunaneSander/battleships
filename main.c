// Autor: Sander Lääts - 2021
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef LOGIC

#include "logic.h"

#define LOGIC
#endif
#ifndef INTERPRETER

#include "interpreter.h"

#define INTERPRETER
#endif
#ifndef ENEMY

#include "enemy.h"

#define ENEMY
#endif
#ifndef DISPLAY
#define DISPLAY

#include "display.h"

#endif
#ifndef PLANNING
#define PLANNING

#include "planning.h"

#endif
#ifndef BATTLE
#define BATTLE

#include "battle.h"

#endif
# define LOGO "\n\t______   ___  _____  _____  _      _____  _____  _   _  _____ ______ \n\t| ___ \\ / _ \\|_   _||_   _|| |    |  ___|/  ___|| | | ||_   _|| ___ \\\n\t| |_/ // /_\\ \\ | |    | |  | |    | |__  \\ `--. | |_| |  | |  | |_/ /\n\t| ___ \\|  _  | | |    | |  | |    |  __|  `--. \\|  _  |  | |  |  __/ \n\t| |_/ /| | | | | |    | |  | |____| |___ /\\__/ /| | | | _| |_ | |    \n\t\\____/ \\_| |_/ \\_/    \\_/  \\_____/\\____/ \\____/ \\_| |_/ \\___/ \\_|    \n"
# define INTRO "\tTo view planning commands, type 'help'\n\tCommands like 'put' will ask details separately"

// Kui oleme Windows masinas, siis on mängul ka värviline väljund
#ifdef WIN32
#define OS "Windows"

#include <windows.h>

#else
#define OS " "
#endif

//############################################# Põhi mängu tsükkel  ###################################################
// Sisaldab menüüd ning üksik ja mitmikmängu puhul viib mängija planeerimisse ja sõtta


// Meetod, kui menüüst on valitud üksikmäng, käib läbi nii plaanimise kui sõja
void singlePlayerGame(void) {

    // Alguses valime vastase raskusastme, kas võtab kõik rünnakud 100% suvaliselt või on mingite eeldustega
    system("cls");
    int difficulty = chooseDifficulty();

    // Näitame legendi ning 'help' käsku, et hiljem oleks abiks
    printLegend();
    printf(INTRO);
    printf("\n\t(enter)\n");
    getc(stdin);
    fflush(stdin);
    system("cls");

    // Loome tabeli - read, veerud on 0,1,2...,9
    static char board[10][10];
    static char enemyBoard[10][10];

    // Alguses kõik koordinaadid tühjad, kuna pole ühtegi laeva, paneme kõik positsioonid veeks
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            board[i][j] = '~';
            enemyBoard[i][j] = '~';
        }
    }


    static char header[10] = "1 player";
    printf("\nWe have no ships out, go place em!!!\nTo place a ship, use the command \"put\"\n(without ship index)\n");

    // Nüüd lähme plaanimise ossa, kui tagastab 1, siis on plaanimine edukalt kulgenud,
    // kui aga midagi muud, siis lähme menüüsse tagasi, näiteks exit käsu puhul
    if (planning(board, header) == 1) {
        fflush(stdin);
        printf("\nPlanning done, now let's generate our enemies map");
        printf("\n\t(enter)\n");
        getc(stdin);
        system("cls");

        // Genereerime vaenlase mängulaua
        enemyPlanning(enemyBoard);
    } else {
        return;
    }


    fflush(stdin);
    printf("\nEnemy ships have arrived!");
    printf("\n\t(enter)\n");
    getc(stdin);
    system("cls");
    fflush(stdin);

    // Planeeringud on kõik tehtud, seega nüüd lähme sõtta
    // Kui meetod tagastab 0 - on mängija võitnud
    // Kui meetod tagastab 1 - on vastane võitnud
    int victor = battleSinglePlayer(board, 28, enemyBoard, 28, difficulty);
    system("cls");
    if (victor == 2) {
        fflush(stdin);
        return;
    }
    // Mängija võitis
    if (victor == 0) {
        printf("\n\nYou are victorious!");
    }

        // vastane võitis
    else {
        printf("\n\nYou look to the sea, only to see all your ships going under, it is a dark day today\n\nYou have lost...");
    }
    printf("\n\t(enter)\n");
    getc(stdin);
    system("cls");
    fflush(stdin);
}//singlePlayerGame


// Kui on valitud mitmikmäng, siis samuti lähme menüü kaudu selle funktsiooni
void multiPlayerGame(void) {

    // initsialiseerime mängulauad
    static char player1Board[10][10];
    static char player2Board[10][10];

    // Küsime ka mängijate nimed, siis on selgem, kelle kord on
    static char player1InitName[10];
    static char player2InitName[10];

    // Rakendame String'i ehk char massiivi
    printf("\nPlayer 1:");
    getMultiPlayerNames(player1InitName);
    printf("\nPlayer 2:");
    getMultiPlayerNames(player2InitName);


    // Töötlemne allpool nimede massivid, et oleksid sama pikad kui nimed ise
    int index = 0;
    for (int i = 0; i < 10; ++i) {
        if (player1InitName[i] == ' ') {
            index = i;
            i = 10;
        }
    }
    char player1Name[index++];
    for (int i = 0; i < index; ++i) {
        player1Name[i] = player1InitName[i];
    }
    player1Name[index] = '\0';
    index = 0;
    for (int i = 0; i < 10; ++i) {
        if (player2InitName[i] == ' ') {
            index = i;
            i = 10;
        }
    }
    char player2Name[index++];
    for (int i = 0; i < index; ++i) {
        player2Name[i] = player2InitName[i];
    }
    player2Name[index] = '\0';


    // Teeme mõlemad mängulauad "vett täis"
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            player1Board[i][j] = '~';
            player2Board[i][j] = '~';
        }
    }


    int winner;

    // Üksikmängu sõja analoog, vaenlase asemele lihtsalt teine mängija
    if (planningMultiPlayer(player1Board, player2Board, player1Name, player2Name) == 1) {
        winner = multiPlayerBattle(player1Board, player2Board, player1Name, player2Name, 28, 28);
    } else {
        return;
    }

    if (winner == 0) {
        printf("Congratulations! player %s has won!!!", player1Name);
        sleep(3);
        system("cls");
    } else {
        if (winner == 2) {
            system("cls");
            fflush(stdin);
            return;
        } else {
            printf("Congratulations! player %s has won!!!", player2Name);
            sleep(3);
            system("cls");
        }
    }
}//multiPlayerGame

// Menüü, peale logo oleme menüüs kus on 4 valikut:
// 1 - üksikmäng
// 2 - mitmikmäng
// 3 - info
// 4 - välju
// Tagastab valitud tegevust numbri
int menu(void) {

    printf("\n\n\t   BATTLESHIP v1.0   \n"
           "\t       Menu\n\n\t"
           "#####################\n\t"
           "#(1) Singleplayer   #\n\t"
           "#(2) Multiplayer    #\n\t"
           "#(3) Help/About     #\n\t"
           "#(4) Exit           #\n\t"
           "#####################"
           "\n\n>>");
    return readMenuInput();
}//menu

// main funktsioon
// Näitab alguses logot ning käitleb menüü tegevuse indeksit
int main(void) {

    // Kui meil on windows süsteem, siis saame väljundit kuvada värviliselt
    if (strcmp(OS, "Windows") == 0) {
        SetConsoleTitle("Battleship 1.0");
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
        printf(LOGO);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    } else {
        printf(LOGO);
    }


    printf("\n\t\t\t\tBy: Sander L%c%cts\n\n\n\n\n\t\t\t(press ENTER to continue)\n", 132, 132);
    getc(stdin);
    system("cls");
    fflush(stdin);

    int exit = 1;
    int chosen;

    // While loop - menüü sisendi tõlgendamine, (tähendust v.t menüü funktsiooni)
    while (exit == 1) {
        chosen = menu();
        if (chosen == 1) {
            system("cls");
            fflush(stdin);
            singlePlayerGame();
        }
        if (chosen == 2) {
            system("cls");
            fflush(stdin);
            multiPlayerGame();
        }
        if (chosen == 3) {
            system("cls");
            fflush(stdin);
            displayAbout();
        }
        if (chosen == 4) {
            exit--;
        }
    }
    return 0;
}//main
