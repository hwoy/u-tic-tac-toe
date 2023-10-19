#include "ttt_io.h"
#include <stdio.h>
#include <string.h>

static void replacech(char* str, char ch, char rp);

char ox_getch(char* buff, int size, char dkey)
{

    fgets(buff, size, stdin);
    replacech(buff, 13, 0);
    replacech(buff, 10, 0);
    if (!strlen(buff)) {
        return dkey;
    }
    return buff[0];
}

static void replacech(char* str, char ch, char rp)
{
    int i;
    for (i = 0; str[i]; i++) {
        if (str[i] == ch)
            str[i] = rp;
    }
}

void ox_printtable(unsigned int val1, unsigned int val2, char ch1, char ch2, const char* squar, char blank)
{
    ox_printtable_tofile(val1, val2, ch1, ch2, stdout, squar, blank);
}

void ox_printtable_tofile(unsigned int val1, unsigned int val2, char ch1, char ch2, FILE* fp, const char* squar, char blank)
{
    static char ch[OX_ROW * OX_COL];

    unsigned int i;

    for (i = 0; i < (OX_ROW * OX_COL); i++)
        ch[i] = ((POW2A(i) & val1)) ? ch1 : ((POW2A(i) & val2)) ? ch2
                                                                : blank;

    fprintf(fp, squar,
        ch[0], ch[1], ch[2],
        ch[3], ch[4], ch[5],
        ch[6], ch[7], ch[8]);
}
