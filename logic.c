// Autor: Sander Lääts - 2021
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "logic.h"

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


//############################################# Loogika-ülesanded  ###################################################
// Siin klassis on funktsioonid nagu laevade lisamine ning lahingufunktsioon

// let1 ja let2 - tähtkoordinaadid, ehk pmst x1 ja x2
// num1 ja num2 - numberkoordinaadid, ehk pmst y1 ja y2
int addShip(char board[10][10], int let1, int num1, int let2, int num2) {
    int len;
    int start;

    num1 = num1 - 48;// Kui nt number on 48, tähendab, et see on null, seega lahutame 48ga,
    num2 = num2 - 48;// et saaks neid kergemini võrrelda


    // Kuna laevad on põhimõtteliselt piirkonnad, siis saame tegutseda järgnevalt:
    // Vaatame, kas muutuja on täht või number

    // Kui muutuvad on tähed
    if (let1 != let2) {

        // Nüüd peame leidma miinimumväärtuse, kustmaalt hakkame laeva joonistama ning pikkuse
        if (let1 > let2) {
            start = let2;
            len = let1 - let2;
        } else {
            start = let1;
            len = let2 - let1;
        }

        // Peame kontrollima, et kas kõik ikoonid on vabad
        for (int i = 0; i <= len; i++) {
            if (board[num1][(start + i)] != '~') {
                printf("\nSelected area is not free for ship!\n");
                sleep(2);
                return 0;
            }

        }

        // Nüüd muudame ikoonid mängulaual laevadeks
        // paiknemine : board[0-9][A-J]

        for (int i = 0; i <= len; i++) {
            board[num1][(start + i)] = '#';
        }
        printf("\nShip placed succesfully!\n");
        printf("\n\t(enter)\n");
        getc(stdin);
        fflush(stdin);
        return 1;

        // Kui muutuvad on numbrid
    } else {
        if (num2 > num1) {
            start = num1;
            len = num2 - num1;
        } else {
            start = num2;
            len = num1 - num2;
        }

        // Kontrollime, kas ala on vaba
        for (int i = 0; i <= len; i++) {
            if (board[(start + i)][let1] != '~') {
                fflush(stdin);
                printf("\nSelected area is not free for ship!\n");
                printf("\n\t(enter)\n");
                getc(stdin);
                return 0;
            }
        }

        // Muudame ikoonid
        for (int i = 0; i <= len; i++) {
            board[(start + i)][let1] = '#';
        }
        fflush(stdin);
        printf("\nShip placed succesfully!\n");
        printf("\n\t(enter)\n");
        getc(stdin);
        return 1;
    }

}

// Funktsioon, mis asetab antud indeksiga laevad suvaliselt
int addShipsRandom(char board[][10], int amount, int len) {

    if (amount == 0) {
        printf("\nNo ships to place!");
        return 0;
    }

    int bottom = 0;
    int maximum = 10;
    int col;
    int row;
    int decision;
    int placed;
    int round;
    unsigned seed = (unsigned) time(NULL);
    for (int i = amount; i > 0; i--) {
        srand(seed);
        seed++;

        placed = 0;
        round = 0;

        // Genereerime tähe ja numbri koordinaadid (täht - veerg, number - rida)

        col = (rand() % (11));//(double) rand() / RAND_MAX * (10); - Vana viis, tundub et praegune töötab paremini
        row = (rand() % (11));//(double) rand() / RAND_MAX * (10);

        // decision, ehk kuhu suunas asetame laeva: 0-paremale; 1-alla; 2-vasakule; 3-üles
        decision = (rand() % (5));//(double) rand() / RAND_MAX * (4);

        while (placed == 0) {

            // Kui laev ei mahu üheski suuans ära, peame uued arvud genereerima
            if (round > 3) {
                placed = 1;
                i++;
            }

            // Kui alguskoordinaadil on laev olemas, siis lisame i'le ühe juurde,
            // kuna me ei asetanud midagi ning genereerime uued kooridnaadid
            if (board[row][col] == '#') {
                placed = 1;
                i++;
                round = 4;
            }

            if (decision > 3) {
                decision = 0;
            }

            // Nüüd kontrollime, kas terve piirkonna jooksul on tee vaba, liigume paremale
            if (decision == 0) {


                for (int j = 0; j < len; ++j) {
                    if (board[row][col + j] != '~' || col + j > 9) {
                        decision++;
                        round++;
                    }
                }

                // Kui jätkuvalt oleme decision=0 juures, siis on tee vaba, peame nüüd ainult laeva asetama
                if (decision == 0) {
                    for (int j = 0; j < len; ++j) {
                        board[row][col + j] = '#';
                        placed = 1;
                    }

                }
            } // paremale - 0

            // Liigume alla
            if (decision == 1) {

                for (int j = 0; j < len; ++j) {
                    if (board[row - j][col] != '~' || row - j < 0) {
                        decision++;
                        round++;
                    }
                }

                if (decision == 1) {
                    for (int j = 0; j < len; ++j) {
                        board[row - j][col] = '#';
                        placed = 1;
                    }
                }
            } // alla - 1

            // Liigume vasakule
            if (decision == 2) {
                for (int j = 0; j < len; ++j) {
                    if (board[row][col - j] != '~' || col - j < 0) {
                        decision++;
                        round++;
                    }
                }

                if (decision == 2) {
                    for (int j = 0; j < len; ++j) {
                        board[row][col - j] = '#';
                        placed = 1;
                    }
                }
            } // alla - 2

            // Liigume üles
            if (decision == 3) {
                for (int j = 0; j < len; ++j) {
                    if (board[row + j][col] != '~' || row + j > 9) {
                        decision++;
                        round++;
                    }
                }

                if (decision == 3) {
                    for (int j = 0; j < len; ++j) {
                        board[row + j][col] = '#';
                        placed = 1;
                    }
                }
            } // üles - 3

        }


    }

    return 1;
}//addShipsRandom