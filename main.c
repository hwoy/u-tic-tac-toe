#include "main.h"
#include "opt.h"
#include "function.h"

int main(int argc,const char *argv[])
{
	static char buff[BSIZE+1];
	char ch,ch_yn;
	unsigned int index;
	ox_gameid gameid;
	ox_game game;

/*====================== Init default values =======================*/
	ox_player p1,p2;
	ox_player *player=NULL;

	char ch1=CH_O;	
	char ch2=CH_X;
		

/*====================== Parameters Processing ======================*/

    {
        int i;
        unsigned int ui_cindex;

		for (ui_cindex = DSTART; (i = opt_action(argc, argv, opt, buff,BSIZE, DSTART))!= e_optend;ui_cindex++) {

        switch (i) {
				
            case OPT_X: ch2=CH_X; ch1=CH_O;
                break;
				
            case OPT_O: ch2=CH_O; ch1=CH_X;
                break;

            case OPT_C: ch2=buff[0]; ch1=buff[1];
                break;

			case OPT_P2: player=&p2;
                break;
				
			case OPT_P1: player=&p1;
                break;
				
			case OPT_T:
						listtest(TRILIST,NTRI,NTRIELEMENT,ch1,ch2);
							return 0;
				
            default:
						return showhelp(croppath(argv[0]),opt,optstr);
			}
        }
    }

/*====================== End of Parameters Processing ======================*/
ox_srandom();

player=newgame(&game,player,&p1,&p2);

do
{

	ox_printtable(p1.val,p2.val,ch1,ch2,OX_SQUAR,CBLANK);

	#ifdef _PVP_
		ch = (player==&p1) ? playerinput(&p1,buff,ch1) : playerinput(&p2,buff,ch2) ;
	#else
		ch = (player==&p1) ? ox_ai(&game,&p2,&p1)+'0' : playerinput(&p2,buff,ch2) ;
	#endif

	
	if(ch==K_EXIT)
	{
		do
		{
			printf("Would you like to exit this game?(%c/%c)-> ",toupper(K_YES),K_NO);
			ch_yn=ox_getch(buff,BSIZE,K_YES);

			
		}while(ch_yn!=K_NO && ch_yn!=K_YES);

		if(ch_yn==K_YES ) break;
		else continue;
	}
		
	else if(ch==K_NEW)
	{
		player=newgame(&game,NULL,&p1,&p2);continue;
	}

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
					printf(winby[index],WINLIST[player->indexwin][0],WINLIST[player->indexwin][1],WINLIST[player->indexwin][2]);
				

				printf(AKEY,K_EXIT);
				if(ox_getch(buff,BSIZE,0)==K_EXIT)
				{	
					ch=K_EXIT; ch_yn=K_YES;
				}

				else
				{
					player = player==&p2? &p1:&p2;
					
					player=newgame(&game,player,&p1,&p2);
				}
				
				
				break;

		case ox_idoutofrange:
		case ox_idvalueexist:
				break;

		default:
				player = player==&p1? &p2:&p1;
				break;

	}
	
}while(ch!=K_EXIT || ch_yn!=K_YES);

	return 0;
}
