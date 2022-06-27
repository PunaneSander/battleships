// Autor: Sander Lääts - 2021
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "planning.h"

#ifndef VALUES

#include "values.h"

#define VALUES
#endif
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

//############################################### Plaanimise mänguosa ###################################################


# define PLANNING_HELP "\n\tPLANNING Game commands:\n\t'help'\t- View all the commands\n\t'put'\t- Place ship on the board (index will be asked separately)\n\t'exit'\t- Exit to menu\n\t'lgnd'\t- Show the maps legend\n\t'rand'\t- Place all ships with given index randomly (index will be asked separately)"


// Funktsioon, mis viib mängija läbi plaanimise mänguosa
int planning(char board[][10], char header[10]) {
    int shipAmounts[] = {carrierNr, battleshipNr, cruiserNr, destroyerNr, submarineNr};
    int shipLens[] = {carrier, battleship, cruiser, destroyer, submarine};
    int totalShipAmount = 0;
    for (int i = 0; i < 5; ++i) {
        totalShipAmount += shipAmounts[i];
    }

    // Mängu loop - phase 1, laevade asetamine
    while (1) {

        // Kui kõik laevad asetatud, saame siit while tsüklist välja
        if (totalShipAmount == 0) {
            break;
        }

        printf("\n##############  PLANNING - %s ##############\n\n", header);

        // Kujutame laua
        printBoard(board);
        printPlanningAmounts(shipLens, shipAmounts);


        int command = readCommandPlanning();


        // v.t interpreter.c -> tagastab 0, seega kuvame käske
        if (command == 0) {
            system("cls");
            printf(PLANNING_HELP);
            printf("\n\t(enter)\n");
            getc(stdin);
        }

        int skip = 0;
        if (command == 1) {

            // Skip - kas võime minna while lausest välja, kas asetamis käsk on täidetud
            while (!skip) {

                fflush(stdin);
                printf("\nType 'put' arguments: 'INDX LETTER|NUMBER LETTER|NUMBER'\nExample - '0 A0 A4\nType 'e' to exit\n>>");

                int *put = readPutCMD();

                if (put[0] == 101) {
                    skip = 1;
                    break;
                }

                if (shipAmounts[put[0] - 48] <= 0) {
                    printf("\nInsufficient amount of index=%c ships!", put[0]);
                } else {
                    fflush(stdin);
                    // Kontrollime, kas laeva pikkus on kooskõlastatud koordinaatidega
                    if (put[1] != put[3]) {
                        if (abs(put[1] - put[3] - 1) != shipLens[(put[0] - 48)]) {
                            printf("Ship length incorrect!");
                        } else {
                            if (addShip(board, put[1], put[2], put[3], put[4]) == 1) {
                                shipAmounts[(put[0] - 48)]--; // Vähendame valitud laevade kogust
                                totalShipAmount--;
                                system("cls");
                                printf("\n##############  PLANNING - %s ##############\n\n", header);
                                printBoard(board);
                                printPlanningAmounts(shipLens, shipAmounts);
                            }
                        }
                    } else {
                        if (put[2] != put[4]) {
                            if (abs(put[2] - put[4] - 1) != shipLens[(put[0] - 48)]) {
                                printf("Ship length incorrect!");
                            } else {
                                if (addShip(board, put[1], put[2], put[3], put[4]) == 1) {
                                    shipAmounts[(put[0] - 48)]--;
                                    totalShipAmount--;
                                    system("cls");
                                    printf("\n##############  PLANNING - %s ##############\n\n", header);
                                    printBoard(board);
                                    printPlanningAmounts(shipLens, shipAmounts);
                                }
                            }
                        } else {
                            // Kui meil on laev pikkusega 1
                            if (put[2] == put[4] && put[1] == put[3] && put[0] - 48 == 4) {
                                if (addShip(board, put[1], put[2], put[3], put[4]) == 1) {
                                    shipAmounts[(put[0] - 48)]--;
                                    totalShipAmount--;
                                    system("cls");
                                    printf("\n##############  PLANNING - %s ##############\n\n", header);
                                    printBoard(board);
                                    printPlanningAmounts(shipLens, shipAmounts);
                                }


                            } else {
                                printf("Incorrect input!");
                            }
                        }
                    }
                }

                if (totalShipAmount < 1) {
                    skip = 1;
                }

            }
        }


        if (command == 2) {
            system("cls");
            return 0;
        }


        if (command == 3) {
            system("cls");
            printLegend();
            printf("\n\t(enter)\n");
            getc(stdin);
            fflush(stdin);
        } else {
            if (command == 4) {
                int randomShip = getRandomShipIndex();
                if (addShipsRandom(board, shipAmounts[randomShip], shipLens[randomShip]) == 1) {
                    totalShipAmount -= shipAmounts[randomShip];
                    shipAmounts[randomShip] = 0;
                    system("cls");
                    printf("\n##############  PLANNING - %s ##############\n\n", header);
                    printBoard(board);
                    printPlanningAmounts(shipLens, shipAmounts);
                }
            }
        }

        system("cls");
    }
    return 1;
}//planning

// Funktsioon, mis teostab vastase plaanimise üksikmängu korral
int enemyPlanning(char board[][10]) {

    // Teeme kaks järjendit, üks näitab iga laeva kogust, teine laeva pikkust, niiet igal laeval oma indeks
    int shipAmounts[] = {carrierNr, battleshipNr, cruiserNr, destroyerNr, submarineNr};
    int shipLens[] = {carrier, battleship, cruiser, destroyer, submarine};

    // Väline for tsükkel, käima üks laevatüüp korraga läbi
    for (int i = 0; i < 5; i++) {
        if (addShipsRandom(board, shipAmounts[i], shipLens[i]) == 0) {
            printf("\nError while generating enemy player ship placement!");
        }
    }
}//enemyPlanning

// Funktsioon, mis teostab mitmikmängus plaanimise mänguosa
int planningMultiPlayer(char player1Board[][10], char player2Board[][10], char player1Name[], char player2Name[]) {
    fflush(stdin);
    printf("%s - are you ready?", player1Name);
    printf("\n\t(enter)\n");
    getc(stdin);
    system("cls");
    fflush(stdin);
    if (planning(player1Board, player1Name) == 1) {
        fflush(stdin);
        system("cls");
        printf("%s - are you ready?", player2Name);
        printf("\n\t(enter)\n");
        getc(stdin);
        system("cls");
        fflush(stdin);
        if (planning(player2Board, player2Name) == 1) {
            system("cls");
            return 1;
        } else {
            printf("\nQuitting multiplayer session");
            sleep(1);
            printf(".");
            sleep(1);
            printf(".");
            sleep(1);
            printf(".");
            system("cls");
            return 0;
        }
    } else {
        printf("\nQuitting multiplayer session");
        sleep(1);
        printf(".");
        sleep(1);
        printf(".");
        sleep(1);
        printf(".");
        system("cls");
        return 0;
    }
}//planningMultiPlayer