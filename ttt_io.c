#include <stdio.h>
#include <string.h>
#include "ttt_io.h"



static void replacech(char *str,char ch,char rp);


char ox_getch(char *buff,int size,char dkey)
{
		
	fgets(buff,size,stdin);
	replacech(buff,13,0);
	replacech(buff,10,0);
	if(!strlen(buff))
	{
		return dkey;
	}
	return buff[0];
		
}

static void replacech(char *str,char ch,char rp)
{
	int i;
	for(i=0;str[i];i++)
	{
		if(str[i]==ch)
		{
			str[i]=rp;
		}
	}
}

void ox_printtable(const ox_player *p1,const ox_player *p2,const char *squar,char blank)
{
	ox_printtable_tofile(p1,p2,stdout,squar,blank);
}

void ox_printtable_tofile(const ox_player *p1,const ox_player *p2,FILE *fp,const char *squar,char blank)
{
	static char ch[OX_ROW*OX_COL];

	unsigned int i;

	for(i=0;i<(OX_ROW*OX_COL);i++)
	{
		if((POW2A(i)&p1->val))
		{
			ch[i]=p1->ch;
		}
		else if((POW2A(i)&p2->val))
		{
			ch[i]=p2->ch;
		}
		else
		{
			ch[i]=blank;
		}
	}
	fprintf(fp,squar,ch[0],ch[1],ch[2],ch[3],ch[4],ch[5],ch[6],ch[7],ch[8]);
}
