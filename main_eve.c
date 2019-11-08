#include "common.h"

int main(int argc,const char *argv[])
{
	unsigned int index,count=0;
	char ch;
	ox_gameid gameid;
	ox_game game;

/*====================== Init default values =======================*/
	ox_player p1,p2;
	ox_player *player=NULL;

	char ch1=CH_O;	
	char ch2=CH_X;
		

	ox_srandom();

	player=newgame(&game,player,&p1,&p2);

	do
	{

		ox_printtable(p1.val,p2.val,ch1,ch2,OX_SQUAR,CBLANK);


		ch = (player==&p1) ? ox_ai(&game,&p2,&p1)+'0' : ox_ai(&game,&p1,&p2)+'0' ;


		if(player==&p1)
			printf(KEYBARP1,ch,ch1,G[1]);

		else if(player==&p2)
			printf(KEYBARP2,G[0],ch2,ch);
			
			
		gameid= player==&p1? ox_gameplay(&game,&p2,&p1,ch-'0') : ox_gameplay(&game,&p1,&p2,ch-'0');

		switch (gameid)
		{
			case ox_idwin:
			case ox_iddraw:
					index = gameid==ox_idwin?(player==&p1?0:1):2;

					ox_printtable(p1.val,p2.val,ch1,ch2,OX_SQUAR,CBLANK);

					printf(winpos[index],G[index]);
					
					if(gameid==ox_idwin)
					{
						printf(winby[index],WINLIST[player->indexwin][0],WINLIST[player->indexwin][1],WINLIST[player->indexwin][2]);
						++count;
					}
				
					player = player==&p2? &p1:&p2;
						
					player=newgame(&game,player,&p1,&p2);
						
					break;

			case ox_idoutofrange:
			case ox_idvalueexist:
					fprintf(stderr,"\n\nox_idoutofrange or ox_idvalueexist\n");
					return 1;

			default:
					player = player==&p1? &p2:&p1;
					break;

		}
		
	}while(count<1);

	return 0;
}
