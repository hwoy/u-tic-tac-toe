#ifndef __MAIN_H__

#define __MAIN_H__

#include "common.h"


static const char TKEY[]="[(%c)NewGame (%c)Exit, Choose(0-8)] (%c)> ";
static const char AKEY[]="\nPress any key to continue (%c)Exit -> ";



enum{
	OPT_X,
	OPT_O,
	OPT_C,
	OPT_P2,
	OPT_P1,
	OPT_T
};

enum{
	K_EXIT = '-',
	K_NEW = '+',
	K_YES = 'y',
	K_NO = 'n'
};


static const char *opt[]={"-x","-o","-c:","-p2","-p1","-t",NULL};

static const char *optstr[]={"P2 get X (Default).","P2 get O.","Custom (Default -c:OX).","P2 go first.","P1 go first.","WINLIST testing.",NULL};



static void listtest(const unsigned int trilist[NTRI][NTRIELEMENT],unsigned int ntri,unsigned int ntrielement,char ch1,char ch2)
{
	unsigned int i,j,k;
	unsigned int val1,val2;

	val2=0;

	for(i=0;i<ntri;i++)
	{
		printf("%u:[",i);
		for(j=0,k=0;j<ntrielement;j++)
		{
			k|=POW2A(trilist[i][j]);
			printf(" %u ",trilist[i][j]);
		}
		printf("] = %u\n",k);
		val1=k;
		ox_printtable(val1,val2,ch1,ch2,OX_SQUAR,CBLANK);
	}
}


static const char *croppath(const char *path)
{
	const char *gpath=path;
	
	for(;*path;++path) if(*path=='\\' || *path=='/') gpath = path+1;

	return gpath;    
}

static int showhelp(const char *path,const char *opt[],const char *optstr[])
{
	unsigned int i;
	
	fprintf(stderr,"\n%s is a Tic Tac Toe that you can not beat!.\n\n",path);
	
	for(i=0;opt[i] && optstr[i];++i)
		fprintf(stderr,"%s\t=\t%s\n",opt[i],optstr[i]);
		
	return 1;
}

static char playerinput(const ox_player *player,char *buff,char ch)
{
	char tmp;
	do
	{
		printf(TKEY,K_NEW,K_EXIT,ch);
		tmp=ox_getch(buff,BSIZE,0);
			
	}while(strlen(buff)!=1);

	return tmp;
}

#endif
