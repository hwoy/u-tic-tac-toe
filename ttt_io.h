#ifndef __TTT_IO_H__
#define __TTT_IO_H__

#include "ttt_engine.h"
#include <stdio.h>

char ox_getch(char* buff, int size, char dkey);
void ox_printtable(unsigned int val1, unsigned int val2, char ch1, char ch2, const char* squar, char blank);
void ox_printtable_tofile(unsigned int val1, unsigned int val2, char ch1, char ch2, FILE* fp, const char* squar, char blank);

#endif
