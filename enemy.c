// Autor: Sander Lääts - 2021
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "enemy.h"

//############################################### Vaenlase käitumine/'aju' ############################################

// Funktsioon, mis otsustab, mis positsiooni ründab üksikmängus vaenlane
// Raskusaste  0 = lihte, seega kõik sammud, mis ta teeb on 100% random
// Raskusaste 1 = raske, kui ta enne sai laevale pihta, siis sihib ta selle ümbrusesse enne
int enemyAttack(char own[10][10], int difficulty) {

    // initsialiseerime rünnatava rea ja veeru
    int col;
    int row;

    // Lihtsa raskusastmega võtame suvalise positsiooni, mis on vaba
    if (difficulty == 1) {

        // Genereerime suvalised rida ja veeru koordinaadid
        unsigned seed = (unsigned) time(NULL);
        srand(seed);
        col = (rand() % (10));
        row = (rand() % (10));

        // Nüüd vaatame, et ega sellel koordinaadil juba ei ole rünnatud
        // Kui on, siis lähme rekursiivselt taas funktsiooni, et genereerida uus positsioon
        if (own[row][col] == '0' || own[row][col] == 'X') {
            return enemyAttack(own, difficulty);
        }
    }

        // Kui on raskema raskusastmega rünne, ehk difficulty = 2
    else {

        // Keerulisem rünne

        // Põhimõte:
        // Käime ülevalt vasakult mängulauast alla paremale välja ning vaatame, kas oleme kohal, kus on X.
        // Kui oleme indeksiga X kohal, siis vaatame, kas täidab järgmisi tingimusi ning lisame siis prioriteeditabelisse
        // ja pärast valime kõige soodsama valikute seast suvalise positsiooni
        // Tingimused on järgmised:
        // Kui vasak == ?:
        //      * Kui paremal on X, siis 1 samm vasakule saab + 2 lisandi andmestruktuuri
        //      * Kui paremal on ?, siis nii vasak kui ka parem saavad +1
        //      * Kui paremal on 0, siis saab ainult vasak +1
        // Kui parem == ?: - analoog ülemisele
        // Kui alumine == ?:
        //      * Kui üleval on X, siis 1 samm alla saab +2 lisandi
        //      * Kui üleval on ?, siis nii ülemine kui alumine +1
        //      * Kui ülemine 0, siis saab ainult alumine +1
        // Kui ülemine == ?: - analoog eelmisele
        // Üks positsioon võib saada endale prioriteedi 0-4, 4 juhul,
        // kui ? vasakul on 2 kõrvuti Xi ja paremal on 2 kõrvuti X vms

        // Kui positsiooni leidmisega läheb kauem aega, siis kuvame ka sõnumi, et mängija ei läheks segadusse
        printf("\n\tEnemy is thinking...\n");

        // Teeme laua kus on prioriteedid, alguses on kõik 0 prioriteediga
        int priorityBoard[10][10];
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                priorityBoard[i][j] = 0;
            }
        }

        // Nüüd käime järjest kõik kohad läbi ning vaatame, kuidas vastavad tingimustele
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {

                // Kui oleme Xi kohal, siis hakkame seda positsiooni uurima
                if (own[i][j] == 'X') {


                    // Kui vasakul X:
                    if (own[i][j - 1] == 'X' && j > 0 && j < 9) {

                        // Vasakul on X ning paremal on tundmatu, siis prio +2 paremal asuval kohal
                        if (own[i][j + 1] == '~' || own[i][j + 1] == '#') {
                            priorityBoard[i][j + 1] += 2;
                        }
                    }


                    // Kui paremal on X:
                    if (own[i][j + 1] == 'X' && j > 0 && j < 9) {

                        // Paremal on X ning vasakul on tundmatu, siis prio +2 vasakul asuval kohal
                        if (own[i][j - 1] == '~' || own[i][j - 1] == '#') {
                            priorityBoard[i][j - 1] += 2;
                        }
                    }


                    // Kui üleval on X
                    if (own[i - 1][j] == 'X' && i > 1 && i < 9) {

                        // Üleval X ning all ?, siis lisame prio +2
                        if (own[i + 1][j] == '~' || own[i + 1][j] == '#') {
                            priorityBoard[i + 1][j] += 2;
                        }
                    }

                    // Kui all on X
                    if (own[i + 1][j] == 'X' && i > 1 && i < 9) {

                        // All X ning üleval ?, siis lisame prio +2
                        if (own[i - 1][j] == '~' || own[i - 1][j] == '#') {
                            priorityBoard[i - 1][j] += 2;
                        }
                    }

                    // Kui vasakul ja paremal on ? või 0, siis vasakule +1 ja paremale +1, kui pole 0
                    if (own[i][j - 1] != 'X' && own[i][j + 1] != 'X') {

                        if (own[i][j - 1] != '0' && j > 0) {
                            priorityBoard[i][j - 1]++;
                        }
                        if (own[i][j + 1] != '0' && j < 9) {
                            priorityBoard[i][j + 1]++;
                        }
                    }

                    // Kui üleval ja all on ? või 0, siis üles +1 ja alla +1, kui pole 0
                    if (own[i - 1][j] != 'X' && own[i + 1][j] != 'X') {

                        if (own[i - 1][j] != '0' && i > 0) {
                            priorityBoard[i - 1][j]++;
                        }
                        if (own[i + 1][j] != '0' && i < 9) {
                            priorityBoard[i + 1][j]++;
                        }
                    }
                }
            }
        } // Väline for tsükkel prio arvutamise jaoks


        // Nüüd on prioriteeditabel täidetud, loeme üle, kui palju mis prioriteediga positsioone meil on
        int priorityAmounts[5];


        // Teeme kõik liikmed nullideks, kuna muidu ei saa me õiget prioriteetide kogust lugeda
        for (int i = 0; i < 5; ++i) {
            priorityAmounts[i] = 0;
        }

        // Loeme prioriteetide kogused üle
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                // Prioriteedid saavad olla 0-4, tegime ka järjendi, mis on 4 pikk,
                // seega lisame igale vastavale indeksile prioriteedikoguse
                priorityAmounts[priorityBoard[i][j]]++;
            }
        }

        // Nüüd leiame kõige soodsama võimaluse, mis saaks olla kõige rohkem 4
        if (priorityAmounts[4] == 0) {
            if (priorityAmounts[3] == 0) {
                if (priorityAmounts[2] == 0) {
                    if (priorityAmounts[1] == 0) {

                        // Kui oleme siia jõudnud, siis on kõik kohad prioriteediga 0, seega teeme täiesti suvalise rünnaku
                        return enemyAttack(own, 1);
                    }

                        // Kui meil on prioriteediga 1 koht/kohad:
                    else {

                        // Teeme massiivi, kuhu paneme kõik positsioonid, mis on prioriteediga 1
                        int prioLocation[priorityAmounts[1]][2];
                        int index = 0;
                        for (int i = 0; i < 10; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if (priorityBoard[i][j] == 1) {
                                    prioLocation[index][0] = i;
                                    prioLocation[index][1] = j;
                                    index++;
                                }
                            }
                        }

                        // Kui meil on ainult üks positsioon prioriteediga 1, siis seda rünnataksegi
                        if (priorityAmounts[1] == 1) {
                            row = prioLocation[0][0];
                            col = prioLocation[0][1];
                        } else {

                            // Võtame suvalise koha prioLocation massiivist
                            unsigned seed = (unsigned) time(NULL);
                            srand(seed);
                            int location = (rand() % priorityAmounts[1]);
                            row = prioLocation[location][0];
                            col = prioLocation[location][1];
                        }
                    }
                }
                    // Kui meil on prioriteediga 2 koht:
                else {
                    int prioLocation[priorityAmounts[2]][2];
                    int index = 0;

                    for (int i = 0; i < 10; ++i) {
                        for (int j = 0; j < 10; ++j) {
                            if (priorityBoard[i][j] == 2) {
                                prioLocation[index][0] = i;
                                prioLocation[index][1] = j;
                                index++;
                            }
                        }
                    }

                    if (priorityAmounts[2] == 1) {
                        row = prioLocation[0][0];
                        col = prioLocation[0][1];
                    } else {

                        // Võtame suvalise koha prioLocation massiivist
                        unsigned seed = (unsigned) time(NULL);
                        srand(seed);
                        int location = (rand() % priorityAmounts[2]);
                        row = prioLocation[location][0];
                        col = prioLocation[location][1];

                    }
                }
            }
                // Kui meil on prioriteediga 3 koht:
            else {

                int prioLocation[priorityAmounts[3]][2];
                int index = 0;

                for (int i = 0; i < 10; ++i) {
                    for (int j = 0; j < 10; ++j) {
                        if (priorityBoard[i][j] == 3) {
                            prioLocation[index][0] = i;
                            prioLocation[index][1] = j;
                            index++;
                        }
                    }
                }

                if (priorityAmounts[3] == 1) {
                    row = prioLocation[0][0];
                    col = prioLocation[0][1];
                } else {
                    // Võtame suvalise koha prioLocation massiivist
                    unsigned seed = (unsigned) time(NULL);
                    srand(seed);
                    int location = (rand() % priorityAmounts[3]);
                    row = prioLocation[location][0];
                    col = prioLocation[location][1];
                }
            }
        }
            // Kui meil on prioriteediga 4 koht:
        else {

            int prioLocation[priorityAmounts[4]][2];
            int index = 0;

            for (int i = 0; i < 10; ++i) {
                for (int j = 0; j < 10; ++j) {
                    if (priorityBoard[i][j] == 4) {
                        prioLocation[index][0] = i;
                        prioLocation[index][1] = j;
                        index++;
                    }
                }
            }

            if (priorityAmounts[4] == 1) {
                row = prioLocation[0][0];
                col = prioLocation[0][1];
            } else {

                // Võtame suvalise koha prioLocation massiivist
                unsigned seed = (unsigned) time(NULL);
                srand(seed);
                int location = (rand() % priorityAmounts[4]);
                row = prioLocation[location][0];
                col = prioLocation[location][1];
            }
        }
    }// keerulisema rünnaku kood
    system("cls");

    // Kui rünnatav koht on tegelikult 0, siis proovime uuesti (igaksjuhuks, kui peaks nii kuidagi juhtuma)
    if (own[row][col] == '0') {
        return enemyAttack(own, 2);
    }

    // Edastame rünnaku otsuse ja tulemuse mängijale
    printf("\n\n##############  BATTLE  ##############\n\n");

    if (own[row][col] == '~') {
        own[row][col] = '0';
        printf("\nEnemy shot the water at %c-%d\n", col + 65, row);
        return 0;

    }
    if (own[row][col] == '#') {
        own[row][col] = 'X';
        printf("\nEnemy shot your ship at %c-%d!!!\n", col + 65, row);
        return 1;
    }
    printf("\nError while calculating enemy attack coordinate...\n");
    return 0;
}//enemyAttack