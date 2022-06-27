// Autor: Sander Lääts - 2021

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

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

#include "battle.h"

//###################################################### Lahingu funktsioonide C klass ###########################################
// Funktsioon, kus mängija saab ühte vastase koordinaati pommitada.
// Tagastab 0 või 1, vastavalt kas saadi laevale pihta või mitte (2, kui tahetakse mängust väljuda)
// parameeter enemy on vastase mängulaud, enemyDisplay on mängijale kuvatav laud, kuna mängija ei saa näha tervet lauda
int attackEnemyShips(char enemy[][10], char enemyDisplay[][10]) {
    system("cls");
    printf("\n\n##############  BATTLE  ##############\n\n");
    printf("Where will we attack?\n");

    // Kui oleme rünnaku ära teinud, siis on pass 1 (vältimaks olukorda kui pommitad juba teadaolevad koordinaati)
    int pass = 0;
    while (pass == 0) {

        // Näitame vastase pommitatud lauda mängija jaoks
        showEnemyDisplay(enemyDisplay);
        printf("\n>>");

        // Loeme rünnaku positsiooni
        int *attack = readCommandBattle(enemyDisplay);
        if (attack == 0) {
            return 2;
        }
        int row = attack[0];
        int col = attack[1];

        // Kui vastase laua positsioonil ei ole küsimärk, siis see tähendab,
        // et seda on juba pommitatud, seega peab uuesti proovima
        if (enemyDisplay[row][col] != '?') {
            printf("\nCannot attack there!!!");
            system("cls");
            printf("\n\n##############  BATTLE  ##############\n\n");
            printf("Where will we attack?\n");
        } else {

            // Rünnatakse vabat positsiooni, seega pass = 1
            pass = 1;

            // Kui tabas laeva, siis muudame ikooni Xiks ning tagastame 1, et lahutada vaenlase laevapositsioonide arvust see maha
            if (enemy[row][col] == '#') {
                enemy[row][col] = 'X';
                enemyDisplay[row][col] = 'X';
                system("cls");
                printf("\n\n##############  BATTLE  ##############\n\n");
                printf("You hit a ship at %c-%d%s\n", col + 65, row, "!!!");
                showEnemyDisplay(enemyDisplay);
                printf("\n\t(enter)\n");
                getc(stdin);
                fflush(stdin);
                return 1;
            } else {

                // Kui tabatakse aga vett, siis muudame positsiooni nulliks ning tagastame 0
                enemy[row][col] = '0';
                enemyDisplay[row][col] = '0';
                system("cls");
                printf("\n\n##############  BATTLE  ##############\n\n");
                printf("You hit water at %c-%d%s\n", col + 65, row, "...");
                showEnemyDisplay(enemyDisplay);
                printf("\n\t(enter)\n");
                getc(stdin);
                fflush(stdin);
                return 0;
            }
        }
    }
}//attackEnemyShips

// Lahingu mängu tsükkel üksikmängus,
// võtab sisendiks own - mängija laud
// ownShips - laevapositsioonide arv, enemy - vaenlase laud, enemyShips - vaenlase laevapositsioondie arv, difficulty - raskusaste
int battleSinglePlayer(char own[10][10], int ownShips, char enemy[][10], int enemyShips, int difficulty) {

    // turn - kelle kord on, 0 = mängija; 1 = arvuti
    unsigned seed = (unsigned) time(NULL);
    srand(seed);
    int turn = (rand() % (2));

    if (turn == 1) {
        printf("\nFirst up - it's the CPU's turn!");
    } else {
        printf("\nFirst up - it's your turn!");
    }

    sleep(3);


    // winner - kes võitis, 0 = mängija; 1 = arvuti; 2 = ei tea (mäng veel käib)
    int winner = 2;

    // Loome ühe mängulaua, kus alguses kõik koordinaadid tundmatud
    char enemyDisplay[10][10];
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            enemyDisplay[i][j] = '?';
        }
    }

    int point = 0;

    // Lahingu mängu-tsükkel, lõppeb kuniks keegi võidab, ehk winner on väiksem kui 2
    while (winner == 2) {

        // Mängija kord
        if (turn == 0) {
            system("cls");
            fflush(stdin);
            printf("\n\tPlayer 1, are you ready?");
            printf("\n\t(enter)\n");
            getc(stdin);

            // Kui saadakse laeva pihta, siis on järgmine kord sama mängija käes
            // attackEnemyShips - rünnaku funktsioon
            point = attackEnemyShips(enemy, enemyDisplay);
            if (point == 1) {
                enemyShips--;
            } else {
                // Exit käsk - välju menüüsse
                if (point == 2) {
                    return 2;
                } else {
                    // Kui ei saadud laevale pihta, siis on vaenlase kord, seega turn = 1, saame liita ühe
                    turn++;
                }
            }
        }


        // Vaenlase kord
        if (turn == 1) {
            system("cls");
            printf("\n\tCPU's Turn");
            printf("\n\t(enter)\n");
            fflush(stdin);
            getc(stdin);

            // Vaenlase rünnak (asub enemy.c)
            point = enemyAttack(own, difficulty);
            if (point == 1) {
                ownShips--;
            } else {
                // Kui vaenlane ei lase laevale pihta, siis turn - 1, ehk turn = 0 ning mängija kord
                turn--;
            }
            printBoard(own);
            printf("\n\t(enter)\n");
            fflush(stdin);
            getc(stdin);
            fflush(stdin);
        }
        // Kui on kummagil laevad kõik õhku lastud, siis teine võidab
        // 1 - vastane võitis
        if (ownShips == 0) {
            winner = 1;
        }
        // 0 - mängija võitis
        if (enemyShips == 0) {
            winner = 0;
        }
    }
    return winner;
}//battleSinglePlayer


// Mitmikmängu funtsioon
// player1Board ja player2Board - mängijate lauad
// player1Name ja player2Name - mängijate nimed
// player1Ships ja player2Ships - mängijate laevapositsioonide arv
int multiPlayerBattle(char player1Board[][10], char player2Board[][10], char player1Name[10], char player2Name[10],
                      int player1Ships, int player2Ships) {
// turn - kelle kord on
    unsigned seed = (unsigned) time(NULL);
    srand(seed);
    int turn = (rand() % (2));

    if (turn == 0) {
        printf("\nFirst up - it's %s's turn!", player1Name);
    } else {
        printf("\nFirst up - it's %s's turn!", player2Name);
    }

    sleep(3);

    int winner = 2;

    // Loome display lauad, ehk mida teine mängija võib näha
    char player1Display[10][10];
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            player1Display[i][j] = '?';
        }
    }
    char player2Display[10][10];
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            player2Display[i][j] = '?';
        }
    }

    int point = 0;

    // Lahingu mängutsükkel
    while (winner == 2) {
        if (turn == 0) {
            fflush(stdin);
            system("cls");
            printf("\n%s, are you ready?", player1Name);
            printf("\n\t(enter)\n");
            getc(stdin);
            fflush(stdin);
            system("cls");

            // Kui saadakse laeva pihta, siis on järgmine kord sama mängija käes
            point = attackEnemyShips(player2Board, player2Display);
            if (point == 1) {
                player2Ships--;
                point = 0;
            } else {
                if (point == 2) {
                    return 2;
                }
                turn = 1;

            }
        }
        if (turn == 1) {
            fflush(stdin);
            system("cls");
            printf("\n%s, are you ready?\n", player2Name);
            printf("\n\t(enter)\n");
            getc(stdin);
            fflush(stdin);
            system("cls");
            // Kui saadakse laeva pihta, siis on järgmine kord sama mängija käes
            point = attackEnemyShips(player1Board, player1Display);
            if (point == 1) {
                player1Ships--;
                point = 0;
            } else {
                if (point == 2) {
                    return 2;
                }
                turn = 0;
            }
            // Kui on kummagil laevad kõik õhku lastud, siis teine võidab
            // 1 - vastane võitis

        }
        if (player2Ships == 0) {
            winner = 1;
        }
        // 0 - mängija võitis
        if (player1Ships == 0) {
            winner = 0;
        }
    }
    return winner;
}//multiPlayerBattle