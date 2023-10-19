#include "glibcrng.h"

static void lshift(PGLIBCRNG begin, PGLIBCRNG end)
{
    while (begin < end - 1) {
        *begin = *(begin + 1);
        ++begin;
    }
}

PGLIBCRNG glibcrnginit(PGLIBCRNG buffer, URND32 seed)
{
    unsigned int i;
    buffer[0] = seed;
    for (i = 1; i < 31; i++) {
        buffer[i] = (16807LL * buffer[i - 1]) % 2147483647;
        if (buffer[i] < 0) {
            buffer[i] += 2147483647;
        }
    }
    for (i = 31; i < 34; i++) {
        RND32 tmp = buffer[31 - 31];
        lshift(buffer, buffer + 31);
        buffer[30] = tmp;
    }

    for (i = 34; i < 344; i++) {
        RND32 tmp = buffer[31 - 31] + buffer[31 - 3];
        lshift(buffer, buffer + 31);
        buffer[30] = tmp;
    }

    return buffer;
}

URND32 glibcrng(PGLIBCRNG buffer)
{

    RND32 tmp = buffer[31 - 31] + buffer[31 - 3];
    lshift(buffer, buffer + 31);
    buffer[30] = tmp;

    return ((URND32)tmp) >> 1;
}
