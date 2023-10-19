#ifndef __TTT_ENGINE_H__

#define __TTT_ENGINE_H__

#define NWIN 8
#define NELEMENT 3

#define NTRI (8 * 6)
#define NTRIELEMENT NELEMENT

#define OX_ROW 3
#define OX_COL 3

#define POW2A(x) (1 << (x))

#define ALLBIT (POW2A(0) | POW2A(1) | POW2A(2) | POW2A(3) | POW2A(4) | POW2A(5) | POW2A(6) | POW2A(7) | POW2A(8))
#define SELECT(x) (ALLBIT ^ (x))
#define EXCEPT(x) | (x)

typedef enum {
    ox_idwin,
    ox_iddraw,
    ox_idgame,
    ox_idoutofrange,
    ox_idvalueexist

} ox_gameid;

typedef struct
{
    unsigned int val;
    unsigned int indexwin;

} ox_player;

typedef struct
{

    unsigned int win[NWIN];
    unsigned int tri[NTRI];
    unsigned int nwin, ntri;
    unsigned int nelement, ntrielement;

} ox_game;

void ox_genpow2a(unsigned int* win, const void* winlist, unsigned int nwin, unsigned int nelement);
unsigned int ox_log2a(unsigned int num);

int ox_iswin(const ox_game* game, const ox_player* player);
int ox_getwillwin(const ox_game* game, const ox_player* p1, const ox_player* p2);
unsigned int ox_getbit(unsigned int test);
int ox_iswillwin(const ox_game* game, const ox_player* p1, const ox_player* p2);
int ox_triwin(const ox_game* _game, const ox_player* p1, const ox_player* p2);

void ox_init(ox_game* game, const void* winlist, const void* trilist, unsigned int nwin, unsigned int nelement,
    unsigned int ntri, unsigned int ntrielement, ox_player* p1, ox_player* p2);

void ox_srandom(void);
unsigned int ox_rand(void);
unsigned int ox_random(unsigned int min, unsigned int max);

int ox_testrandomselect(unsigned int test);
ox_gameid ox_gameplay(ox_game* game, const ox_player* p1, ox_player* p2, unsigned int val);
int ox_ai(const ox_game* game, const ox_player* p1, const ox_player* p2);

#endif
