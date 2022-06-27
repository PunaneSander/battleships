// Autor: Sander Lääts - 2021

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "interpreter.h"

#ifndef DISPLAY
#define DISPLAY

#include "display.h"

#endif


//############################################# Sisendi tõlgendamis-funktsioonid  ###################################################

// Funktsioon menüü valiku tõlgendamiseks
int readMenuInput(void) {
    int input = getc(stdin);

    // Kui sisestatud pole arv 0-4
    if (input < 49 || input > 52) {
        printf("\nIncorrect input, please only type number 1-4 and press 'enter'\n>>");
        fflush(stdin);
        return readMenuInput();
    }

    return input - 48;
}//readMenuInput

// Kontrollib, kas on tegu tähega, kui meil on väike täht, siis tagastab suure
char checkLetter(char c) {
    if (c > 96 && c < 123) {
        return c - 32;
    } else {
        return c;
    }
}//checkLetter

// Funktsioon mis kuvab raskusastmete valiku üksikmängus ning tõlgendab antud sisendit
int chooseDifficulty(void) {
    printf("\n\n\tChoose difficulty setting:\n\n\t"
           "(1) Easy\n\t"
           "       - Enemy chooses what tile to bomb randomly\n\t"
           "(2) Intermediate\n\t"
           "       - Enemy chooses what tile to bomb depending on if the last bombing was succesful\n>>");

    // Loeme sisendi
    int input = getc(stdin);

    // Kui sisend pole 1 või 2
    if (input < 49 || input > 50) {
        printf("\nIncorrect input, please only type number 0-1 and press 'enter'");
        printf("\n\t(enter)\n");
        getc(stdin);
        fflush(stdin);
        system("cls");
        return chooseDifficulty();
    }
    fflush(stdin);
    system("cls");

    // Tagastame input-48 kuna input on number 1 või 2 ASCII kood, 0 ASCII on 48, seega lahutame selle ning saame int väärtuse,
    // mis on vastavalt kas 1 või 2
    return input - 48;
}//chooseDifficulty

// Funktsioon laeva asetamiskäsu tõlgendamiseks planeerimisel
// Tagastab pointer'i massiivile
int *readPutCMD(void) {

    // cmd massiiv = [laeva nr, tähe koordinaat, nr koordinaat, tht2, nr2]
    static int cmd[5];
    // cmdi - cmd indeks
    int cmdi = 0;
    char c[7];
    for (int i = 0; i < 7; i++) {
        c[i] = getc(stdin);
        if (i != 1 && i != 4) {
            if (i == 0 && c[i] == 101) {
                cmd[0] = 101;
                return cmd;
            }
            cmd[cmdi] = c[i];
            cmdi++;
        }
    }

    cmd[1] = checkLetter(cmd[1]);
    cmd[3] = checkLetter(cmd[3]);

    // int - ASCII
    // 65 - A
    // 74 - J
    // 48 - 0
    // 52 - 4
    // 57 - 9

    // Nüüd kontrollime, kas antud käsk vastab parameetritele

    // Kui ei sisalda laeva indeksit, siis küsime uuesti
    if (cmd[0] < 48 || cmd[0] > 52) {
        printf("\nShip index incorrect, please try again!\n>>");
        fflush(stdin);
        return readPutCMD();
    }
    if (cmd[1] < 65 || cmd[1] > 74) {
        printf("\nFirst letter coordinate incorrect, please try again!\n>>");
        fflush(stdin);
        return readPutCMD();
    }
    if (cmd[2] < 48 || cmd[2] > 57) {
        printf("\nFirst number coordinate incorrect, please try again!\n>>");
        fflush(stdin);
        return readPutCMD();
    }
    if (cmd[3] < 65 || cmd[3] > 74) {
        printf("\nSecond letter coordinate incorrect, please try again!\n>>");
        fflush(stdin);
        return readPutCMD();
    }
    if (cmd[4] < 48 || cmd[4] > 57) {
        printf("\nSecond number coordinate incorrect, please try again!\n>>");
        fflush(stdin);
        return readPutCMD();
    }
    // Kui lahutame 65, siis nt A väärtus = 65, see on ka meie ruudustiku alguskoordinaat,
    // mis võiks edaspidi olla 0, seega lahutame 65
    cmd[1] = cmd[1] - 65;
    cmd[3] = cmd[3] - 65;

    return cmd;
}//readPutCMD

// Funktsioon, mis loeb ning tõlgendab käske, mis on antud planeerimise mänguosas
int readCommandPlanning(void) {
    fflush(stdin);
    printf(">>");

    // Käsu max pikkus = 5 tähte
    char c[5];
    for (int i = 0; i < 5; i++) {
        c[i] = getc(stdin);
        c[i] = checkLetter(c[i]);
        if (c[i] == 10) {
            c[i] = 0;
            i = 5;
        }
    }
    fflush(stdin);

    // Käskude kood:
    // 0 - help
    // 1 - put
    // 2 - exit
    // 3 - show legend

    // Käskude kuvamiseks
    if (strcmp("HELP", c) == 0) {
        return 0;
    }
    // Laeva asetamiseks
    if (strcmp("PUT", c) == 0) {
        return 1;
    }
    // Planeerimisest menüüsse väljumine
    if (strcmp("EXIT", c) == 0) {
        return 2;
    }
    // Mängulaua legendi kuvamiseks
    if (strcmp("LGND", c) == 0) {
        return 3;
    }
    // Laevade, mis on ühe indeksiga, suvaline asetamine lauale
    if (strcmp("RAND", c) == 0) {
        return 4;
    }
    fflush(stdin);
    printf("Unknown command!");
    printf("\n\t(enter)\n");
    getc(stdin);
    fflush(stdin);

    // Kui pole ükski natud käsk, tagastame 99
    return 99;
}//readCommandPlanning

// Funktsioon lahingu ajal käsu lugemiseks
// Tagastab rünnatava koordinaadi indeksid
int *readCommandBattle(char enemyDisplay[][10]) {
    fflush(stdin);
    char c[5];
    int pass = 0;
    while (!pass) {
        for (int i = 0; i < 2; ++i) {
            c[i] = getc(stdin);
            c[i] = checkLetter(c[i]);

            // Kui loetav käsk ei ole number
            if (i == 0 && (c[i] > 64 && c[i] < 91)) {
                i++;
                for (int j = i; j < 4; ++j) {
                    c[j] = getc(stdin);
                    if (c[j] == '\n'){
                        c[j] = '\0';
                        j = 4;
                    }
                    c[j] = checkLetter(c[j]);
                }
                c[4] = '\0';
                pass++;
                i = 4;
                if (strcmp("HELP", c) == 0) {
                    displayBattleHelp();
                    printf("\n\n##############  BATTLE  ##############\n\n");
                    showEnemyDisplay(enemyDisplay);
                    printf("\n>>");
                    return readCommandBattle(enemyDisplay);
                }
                if (strcmp("LGND", c) == 0) {
                    system("cls");
                    printLegend();
                    fflush(stdin);
                    getc(stdin);
                    system("cls");
                    printf("\n\n##############  BATTLE  ##############\n\n");
                    showEnemyDisplay(enemyDisplay);
                    printf("\n>>");
                    return readCommandBattle(enemyDisplay);
                }
                if (strcmp("EXIT", c) == 0) {
                    return 0;
                } else {
                    if (c[2] != '\0'){
                        printf("Error, invalid command!");
                    }
                }
            }
        }
        if (c[0] > 64 && c[0] < 75 && c[1] > 47 && c[1] < 58) {
            pass++;
        } else {
            printf("Error, invalid command!\n>>");
            return readCommandBattle(enemyDisplay);
        }
        fflush(stdin);
    }
    // Teisel kohal on number, ASCIIs on 0 48, seega lahutame 48 ning meie int väärtus ongi meile käes
    // Esimesel kohal on täht, sellest lahutame 65 samal põhimõttel
    int row = c[1] - 48;
    int col = c[0] - 65;

    static int coordinates[2];
    coordinates[0] = row;
    coordinates[1] = col;
    return coordinates;
}//readCommandBattle

// Funktsioon, mis kogub kahe mängija nimed
void getMultiPlayerNames(char playerName[10]) {
    int chosen = 0;
    char name[10];
    while (chosen == 0) {
        fflush(stdin);
        printf("\n\n\tPlease enter your name:\n\t(MAX 10 characters)\n\tAllowed input: A-Z, a-z, 0-9, '-'\n>>");
        char input;


        // Viimased kaks märki peaksid olema /n, kui aga ei ole, siis on nimi üle kümne märgi
        for (int i = 0; i <= 11; ++i) {
            input = getc(stdin);

            if (i == 10) {
                // Kui on uus rida, siis on nimi kirjas
                if (input == '\n') {
                } else {
                    printf("\nIncorrect input, check length!");
                    sleep(2);
                    system("cls");
                    fflush(stdin);
                }
            }

            // Kui on lühem nimi, siis on newline esimene sümbol juba sisendis
            if (input == '\n') {
                chosen = 1;
                for (i; i < 10; i++) {
                    name[i] = ' ';
                }
                i++;
            }

            // Kontrollime, kas sisend vastab tingimustele
            if ((input < 91 && input > 64) || (input > 96 && input < 123) || input == 45 ||
                (input > 47 && input < 58) && i < 10) {
                name[i] = input;
            } else {
                if (chosen != 1) {
                    i = 12;
                    printf("\nIncorrect input, try again!");
                    sleep(2);
                    system("cls");
                    fflush(stdin);
                }
            }
        }
    }
    system("cls");
    for (int i = 0; i < 10; ++i) {
        playerName[i] = name[i];
    }
}//getMultiPlayerNames

// Funktsioon, mis loeb suvalise asetamise käsu
int getRandomShipIndex(void) {
    printf("\nEnter ship index:\n");
    int ship = getc(stdin);
    if (ship > 47 && ship < 53) {
        ship -= 48;
        printf("\nPlacing ship %d randomly...", ship);
        return ship;
    } else {
        printf("\nIncorrect input!\n");
        fflush(stdin);
        return getRandomShipIndex();
    }
}//getRandomShipIndex





