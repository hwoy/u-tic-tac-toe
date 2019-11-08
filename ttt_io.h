#ifndef __TTT_IO_H__

#define __TTT_IO_H__

#include "ttt_engine.h"
char ox_getch(char *buff,int size,char dkey);
void ox_printtable(const ox_player *p1,const ox_player *p2,const char *squar,char blank);
void ox_printtable_tofile(const ox_player *p1,const ox_player *p2,FILE *fp,const char *squar,char blank);

#endif