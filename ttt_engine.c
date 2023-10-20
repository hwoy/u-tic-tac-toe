#include "ttt_engine.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int ox_log2a(unsigned int num)
{
    unsigned int i;
    for (i = 0; (num >> i) > 1; i++)
        ;

    return i;
}

void ox_genpow2a(unsigned int* win, const void* winlist, unsigned int nwin, unsigned int nelement)
{
    unsigned int i, j;

    for (j = 0; j < nwin; j++) {
        win[j] = 0;
        for (i = 0; i < nelement; i++) {
            win[j] |= POW2A(*((unsigned int*)winlist + j * nelement + i));
        }
    }
}

int ox_iswin(const ox_game* game, const ox_player* player)
{
    unsigned int i;

    for (i = 0; i < (game->nwin); i++) {
        if (game->win[i] == (player->val & (game->win[i]))) {
            return i;
        }
    }
    return -1;
}

int ox_getwillwin(const ox_game* game, const ox_player* p1, const ox_player* p2)
{
    unsigned int i, j, k, test, rival;

    test = p2->val;

    rival = p1->val;

    for (i = 0, j = 0; i < (game->nwin); i++) {
        if (ox_getbit(test & (game->win[i])) == game->nelement - 1) {
            k = (test & (game->win[i])) ^ (game->win[i]);

            if (k != (k & rival)) {
                j++;
            }
        }
    }
    return j;
}

unsigned int ox_getbit(unsigned int test)
{
    unsigned int i, j;

    for (i = 0, j = 0; i < sizeof(unsigned int) * 8; i++) {
        if ((test & (POW2A(i)))) {
            j++;
        }
    }
    return j;
}

int ox_iswillwin(const ox_game* game, const ox_player* p1, const ox_player* p2)
{
    unsigned int i, k, test, rival;

    test = p2->val;

    rival = p1->val;

    for (i = 0; i < (game->nwin); i++) {
        if (ox_getbit(test & (game->win[i])) == game->nelement - 1) {
            k = (test & (game->win[i])) ^ (game->win[i]);
            if (k != (k & rival)) {
                return ox_log2a(k);
            }
        }
    }
    return -1;
}

int ox_triwin(const ox_game* _game, const ox_player* p1, const ox_player* _p2)
{
    unsigned int i, j, k, rival, *test;

    int m;

    ox_player p2 = *_p2;
    ox_game game = *_game;

    test = &p2.val;
    rival = p1->val;

    for (i = 0; i < game.ntri; i++) {
        if (ox_getbit(*test & (game.tri[i])) == game.ntrielement - 1) {
            j = (*test & (game.tri[i])) ^ (game.tri[i]);
            if (j != (j & rival)) {
                k = *test;
                *test |= j;
                m = ox_getwillwin(&game, p1, &p2);
                *test = k;

                if (m > 1) {
                    return ox_log2a(j);
                }
            }
        }
    }
    return -1;
}

unsigned int
ox_rand(ox_game* game)
{
    return glibcrng(game->random);
}

unsigned int ox_random(ox_game* game, unsigned int min, unsigned int max)
{
    return min <= max ? min + (ox_rand(game) % (max - min + 1)) : -1UL;
}

ox_game ox_creatgame(unsigned int seed)
{
    ox_game game;
    glibcrnginit(game.random, seed);
    return game;
}

void ox_init(ox_game* game, const void* winlist, const void* trilist, unsigned int nwin, unsigned int nelement,
    unsigned int ntri, unsigned int ntrielement, ox_player* p1, ox_player* p2)
{
    p1->val = 0;

    p2->val = 0;

    game->nwin = nwin;
    game->nelement = nelement;
    game->ntri = ntri;
    game->ntrielement = ntrielement;

    ox_genpow2a(game->win, winlist, nwin, nelement);
    ox_genpow2a(game->tri, trilist, ntri, ntrielement);
}

int ox_testrandomselect(ox_game* game, unsigned int test)
{
    int buff[9], j, i;

    for (i = 0, j = 0; i < 9; i++) {
        if (!(POW2A(i) & test)) {
            buff[j++] = i;
        }
    }

    return j > 0 ? buff[ox_random(game, 0, j - 1)] : -1;
}

ox_gameid ox_gameplay(const ox_game* game, const ox_player* p1, ox_player* p2, unsigned int val)
{
    int i;

    if (val > 8) {
        return ox_idoutofrange;
    }

    else if (POW2A(val) == (POW2A(val) & (p1->val | p2->val))) {
        return ox_idvalueexist;
    }

    p2->val |= POW2A(val);

    if ((i = ox_iswin(game, p2)) > -1) {
        return ox_idwin;
    }

    else if ((p1->val | p2->val) == ALLBIT) {
        return ox_iddraw;
    }

    return ox_idgame;
}

int ox_ai(ox_game* game, const ox_player* p1, const ox_player* p2)
{
    int n;

    if ((n = ox_iswillwin(game, p1, p2)) > -1)
        return n;

    else if ((n = ox_iswillwin(game, p2, p1)) > -1)
        return n;

    switch (ox_getbit(p1->val | p2->val)) {

    case 0:
        switch (ox_random(game, 1, 7)) {
        case 1:
        case 2:
        case 3:
            return ox_testrandomselect(game, SELECT(POW2A(0) | POW2A(2) | POW2A(6) | POW2A(8))); /* Border delta strategy */
        case 4:
            return ox_testrandomselect(game, SELECT(POW2A(1) | POW2A(3) | POW2A(5) | POW2A(7))); /* Small delta strategy & Cross strategy */
        case 5:
        case 6:
        default:
            return 4; /* Center delta strategy */
        }

    case 2: /*if gamecount=2 Offensive*/
        if (ox_log2a(p2->val) % 2)

        { /* Cross strategy */
            if ((p2->val & (POW2A(1) | POW2A(3))) && ((p1->val) & (POW2A(0) | POW2A(4))))
                return ox_log2a((POW2A(1) | POW2A(3)) ^ (p2->val));
            else if ((p2->val & (POW2A(1) | POW2A(5))) && ((p1->val) & (POW2A(2) | POW2A(4))))
                return ox_log2a((POW2A(1) | POW2A(5)) ^ (p2->val));
            else if ((p2->val & (POW2A(7) | POW2A(3))) && ((p1->val) & (POW2A(4) | POW2A(6))))
                return ox_log2a((POW2A(7) | POW2A(3)) ^ (p2->val));
            else if ((p2->val & (POW2A(7) | POW2A(5))) && ((p1->val) & (POW2A(4) | POW2A(8))))
                return ox_log2a((POW2A(7) | POW2A(5)) ^ (p2->val));

            /* Small delta strategy will punish */
            else if ((p2->val == POW2A(3)) && ((p1->val) & (POW2A(2) | POW2A(8))))
                return (p1->val == POW2A(2)) ? 0 : 6;
            else if ((p2->val == POW2A(1)) && ((p1->val) & (POW2A(6) | POW2A(8))))
                return (p1->val == POW2A(6)) ? 0 : 2;
            else if ((p2->val == POW2A(5)) && ((p1->val) & (POW2A(0) | POW2A(6))))
                return (p1->val == POW2A(0)) ? 2 : 8;
            else if ((p2->val == POW2A(7)) && ((p1->val) & (POW2A(0) | POW2A(2))))
                return (p1->val == POW2A(0)) ? 6 : 8;

            /* Small delta strategy */
            else
                return 4;

        }

        /* Center delta strategy */
        else if (p2->val == POW2A(4)) {
            switch (ox_log2a(p1->val)) {
            case 0:
                return 8;
            case 2:
                return 6;
            case 6:
                return 2;
            case 8:
                return 0;

            case 1: /*      will be Punished         */
            case 3:
                return ox_testrandomselect(game, SELECT(POW2A(6) | POW2A(8))); /*      will be Punished         */
            case 5: /*      will be Punished         */
            case 7:
                return ox_testrandomselect(game, SELECT(POW2A(0) | POW2A(2))); /*      will be Punished         */
            }
        }

        else if (p2->val & (POW2A(0) | POW2A(2) | POW2A(6) | POW2A(8))) {
            /* Border delta strategy may be punished in gamecount==4*/
            if (p1->val == POW2A(4)) {
                switch (p2->val) {
                case POW2A(0):
                    return ox_testrandomselect(game, SELECT(POW2A(8) | POW2A(7) | POW2A(5)));
                case POW2A(2):
                    return ox_testrandomselect(game, SELECT(POW2A(6) | POW2A(7) | POW2A(3)));
                case POW2A(6):
                    return ox_testrandomselect(game, SELECT(POW2A(2) | POW2A(1) | POW2A(5)));
                case POW2A(8):
                    return ox_testrandomselect(game, SELECT(POW2A(0) | POW2A(1) | POW2A(3)));
                }
            }
            /*Border delta*/
            /*else if(m==0 || m==2 || m==6 || m==8) will be punish*/
            else if (p1->val & (POW2A(0) | POW2A(2) | POW2A(6) | POW2A(8))) {
                switch (p2->val | p1->val) {
                case POW2A(0) | POW2A(2):
                    return ox_testrandomselect(game, SELECT(POW2A(6) | POW2A(8)));
                case POW2A(6) | POW2A(8):
                    return ox_testrandomselect(game, SELECT(POW2A(0) | POW2A(2)));

                case POW2A(0) | POW2A(6):
                    return ox_testrandomselect(game, SELECT(POW2A(2) | POW2A(8)));
                case POW2A(2) | POW2A(8):
                    return ox_testrandomselect(game, SELECT(POW2A(0) | POW2A(6)));

                case POW2A(2) | POW2A(6):
                    return ox_testrandomselect(game, SELECT(POW2A(0) | POW2A(8)));
                case POW2A(0) | POW2A(8):
                    return ox_testrandomselect(game, SELECT(POW2A(2) | POW2A(6)));

                /*Border delta or Small delta can beat*/
                default:
                    return 4;
                }
            }

            else if (p1->val & (POW2A(1) | POW2A(3) | POW2A(5) | POW2A(7))) {

                /*Border delta will be punish*/
                switch (p2->val | p1->val) {
                case POW2A(8) | POW2A(5):
                case POW2A(0) | POW2A(1):
                    return ox_testrandomselect(game, SELECT(POW2A(4) | POW2A(6)));

                case POW2A(8) | POW2A(7):
                case POW2A(0) | POW2A(3):
                    return ox_testrandomselect(game, SELECT(POW2A(4) | POW2A(2)));

                case POW2A(6) | POW2A(3):
                case POW2A(2) | POW2A(1):
                    return ox_testrandomselect(game, SELECT(POW2A(4) | POW2A(8)));

                case POW2A(6) | POW2A(7):
                case POW2A(2) | POW2A(5):
                    return ox_testrandomselect(game, SELECT(POW2A(4) | POW2A(0)));

                case POW2A(0) | POW2A(5):
                case POW2A(0) | POW2A(7):
                    return ox_testrandomselect(game, SELECT(POW2A(0) | POW2A(2) | POW2A(4) | POW2A(6) | POW2A(8)) EXCEPT((p2->val) | POW2A(8)));

                case POW2A(2) | POW2A(3):
                case POW2A(2) | POW2A(7):
                    return ox_testrandomselect(game, SELECT(POW2A(0) | POW2A(2) | POW2A(4) | POW2A(6) | POW2A(8)) EXCEPT((p2->val) | POW2A(6)));

                case POW2A(6) | POW2A(1):
                case POW2A(6) | POW2A(5):
                    return ox_testrandomselect(game, SELECT(POW2A(0) | POW2A(2) | POW2A(4) | POW2A(6) | POW2A(8)) EXCEPT((p2->val) | POW2A(2)));

                case POW2A(8) | POW2A(1):
                case POW2A(8) | POW2A(3):
                    return ox_testrandomselect(game, SELECT(POW2A(0) | POW2A(2) | POW2A(4) | POW2A(6) | POW2A(8)) EXCEPT((p2->val) | POW2A(0)));
                }
            }

            return 4;
        }

    case 1: /* Cross strategy and Small delta srategy Defendsive*/
        switch (ox_log2a(p1->val)) {
        /* Cross strategy defensive*/
        case 1:
            return ox_testrandomselect(game, SELECT(POW2A(0) | POW2A(2) | POW2A(6) | POW2A(8) | POW2A(4)) EXCEPT(POW2A(6) | POW2A(8)));
        case 5:
            return ox_testrandomselect(game, SELECT(POW2A(0) | POW2A(2) | POW2A(6) | POW2A(8) | POW2A(4)) EXCEPT(POW2A(0) | POW2A(6)));
        case 7:
            return ox_testrandomselect(game, SELECT(POW2A(0) | POW2A(2) | POW2A(6) | POW2A(8) | POW2A(4)) EXCEPT(POW2A(0) | POW2A(2)));
        case 3:
            return ox_testrandomselect(game, SELECT(POW2A(0) | POW2A(2) | POW2A(6) | POW2A(8) | POW2A(4)) EXCEPT(POW2A(2) | POW2A(8)));
        case 4:
            return ox_testrandomselect(game, SELECT(POW2A(0) | POW2A(2) | POW2A(6) | POW2A(8))); /* Center delta defendsive*/
        default:
            return 4; /* Border delta defendsive*/
        }

    case 3:
        switch (p1->val) {
        /* Border delta defendsive */
        case POW2A(2) | POW2A(6):
        case POW2A(0) | POW2A(8):
            return ox_testrandomselect(game, SELECT(POW2A(1) | POW2A(3) | POW2A(5) | POW2A(7)));

        /* Center delta or Border delta defendsive */
        case POW2A(4) | POW2A(0):
        case POW2A(4) | POW2A(8):
            return ox_testrandomselect(game, SELECT(POW2A(2) | POW2A(6)));
        case POW2A(4) | POW2A(2):
        case POW2A(4) | POW2A(6):
            return ox_testrandomselect(game, SELECT(POW2A(0) | POW2A(8)));

        case POW2A(1) | POW2A(5):
            if (!(p2->val & (POW2A(4) | POW2A(2))))
                return 6;
            break;
        case POW2A(1) | POW2A(3):
            if (!(p2->val & (POW2A(4) | POW2A(0))))
                return 8;
            break;
        case POW2A(3) | POW2A(7):
            if (!(p2->val & (POW2A(4) | POW2A(6))))
                return 2;
            break;
        case POW2A(5) | POW2A(7):
            if (!(p2->val & (POW2A(4) | POW2A(8))))
                return 0;
            break;
        }

    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        return ((n = ox_triwin(game, p2, p1)) != -1) ? n : ((n = ox_triwin(game, p1, p2)) != -1) ? n
                                                                                                 : ox_testrandomselect(game, p1->val | p2->val);
    }

    return -1;
}
