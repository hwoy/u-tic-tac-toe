#ifndef __COMMON__H_
#define __COMMON__H_

#include "function.h"
#include "opt.h"

#include "ttt.h"
#include "ttt_io.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define BSIZE 1024

#define CBLANK 0x20

enum {
    CH_X = 'X',
    CH_O = 'O'
};

static const char* G[] = { "{ ^ _ ^ }", "[ @ _ @ ]", "[ * _ * ]", NULL };

static const char* winpos[] = { "\n<...... P1 %s Wins ......>\n", "\n<...... P2 %s Wins ......>\n", "\n<...... P1&P2 %s Draw ......>\n", NULL };
static const char* winby[] = { "\tP1 wins by [%u , %u , %u]\n", "\tP2 win by [%u , %u , %u]\n", NULL };

static const char KEYBARP2[] = "%s (%c)> %c\n";
static const char KEYBARP1[] = "				%c <(%c) %s\n";

static ox_player* newgame(ox_game* game, ox_player* wf, ox_player* p1, ox_player* p2)
{
    ox_init(game, WINLIST, TRILIST, NWIN, NELEMENT, NTRI, NTRIELEMENT, p1, p2);

    return (!wf) ? (ox_random(0, 1) ? p1 : p2) : wf;
}

#endif
