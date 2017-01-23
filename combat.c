#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <time.h>

/*
IN FILE:
password
cname
player.CLASS_ID
player.DUNGEON   
player.HP
player.ATK
player.MATK
player.DEF
player.MDEF
player.MOVE1_ID
player.MOVE2_ID
player.MOVE3_ID
player.MOVE4_ID
*/
int endDungeon(int a){
    return !a;
}
//LOL had this in server already btw
void convertC(character *player, char *info) {
  printf("convertC used");
  char *fixInfo = strdup(info);
  player->CLASS_ID = atoi(strsep(&info," "));
  player->DUNGEON = atoi(strsep(&info," "));
  player->HP = atoi(strsep(&info," "));
  player->ATK = atoi(strsep(&info," "));
  player->MATK = atoi(strsep(&info," "));
  player->DEF = atoi(strsep(&info," "));
  player->MDEF = atoi(strsep(&info," "));
  player->MOVE1_ID = atoi(strsep(&info," "));
  player->MOVE2_ID = atoi(strsep(&info," "));
  player->MOVE3_ID = atoi(strsep(&info," "));
  player->MOVE4_ID = atoi(strsep(&info," "));
  free(fixInfo);
} //converts string to player struct


/*
void processStats(character a, char stats){
    if(counter ==1){
        a.cname = stats;
    }
    if(counter == 2){
        a.HP = atoi(stats);
    }
    if(counter ==3){
        a.ATK = atoi(stats);
    }
    if(counter ==4){
        a.MATK = atoi(stats);
    }
    if(counter ==5){
        a.DEF = atoi(stats);
    }
    if(counter ==6){
        a.MOVE1_ID = atoi(stats);
    }
    if(counter ==7){
        a.MOVE2_ID = atoi(stats);
    }
    if(counter ==8){
        a.MOVE3_ID = atoi(stats);
    }
    if(counter ==9){
        a.MOVE4_ID = atoi(stats);
    }
}
*/

int isDead(character a){
    if(a.HP ==0){
        return 1;
    }
    else{
        return 0;
    }
}
int formula(int atk, int atkbuff,int atkdeb,int multipler,int def,int defbuff,int defdeb){
    return ((atk * atk * (1-atkdeb) * (1+atkbuff)*multipler)/(def * defbuff * defdeb));
}

int attack(character* player,character* target,int move){
//	check player's class;
//	decrease all the timers on debuff and buffs for both player and target(if a timer reaches zero, turn buff/debuff back to 0)
//	decrease timers on stun and poison

    int cID = player.classID;
	if (cID == 1){
	    check//Let's pretend 1 is "Warrior"
		 if (move == 1){					//This would be "Swing"
		 	if (player.MOVE1_ID == 1) {			//This would be base skill
				target.HP_LOST += formula(player.ATK,player.ATKBUFF,player.ATKDEB,1,target.DEF,target.DEFBUFF,target.DEFDEB)
			}
			if (player.MOVE1_ID == 2) {			//This would be branch 1 of first upgrade. "Swing Break"
				target.HP_LOST += ......
				target.DEFDEB = 20;
				target.DEFDEB_TURNS = 3;
			}
			if player.MOVE1_ID == 3) {			//This would be branch 2 of first upgrade. "Heavy Swing"
				target.HP_LOST += ......
				//More damage or something
			}
			//ETC.
	}
	if (cID == 2){
		.....
	}
	if(player->ATKBUFF_TURNS > 0) (player->ATKBUFF_TURNS)--;
	if(player->)
}
void action(character player, int sd, int a, int b){
    int move;
    if (a == 1) move = player.MOVE1_ID;
    if (a == 2) move = player.MOVE2_ID;
    if (a == 3) move = player.MOVE3_ID;
    if (a == 4) move = player.MOVE4_ID;
    int type = check(player.CLASS_ID,move);
    
    //single + buff 
    if (type == 0) move(sd, player, player, a, b, 0);
    if (type == 1) move(sd, player, player, a, b, 1);
    if (type == 2) move(sd, player, enemy, a, b, 2);
}

//literally only sets everything up
void startBattle(character party[],character enemy, character attacker, int move, int target){
    srand(time(NULL));
    int deathCount = 0;
    int inProgress = 1;
    if (enemy.CD_NOW == 0){
        int r = rand()%5;
        determine (enemy, party, enemy, r);
        enemy.CD_NOW = enemy.CD;
    }
        
    //Checks for death of party
    for(k;k<sizeof(party);k++){
        if(isDead(party[k])) deathCount ++;
    }
    if(deathCount == sizeof(party)){
        printf("All your party members are dead!\n");
        return 0;
    }
    deathCount = 0;
    
    //Checks for death of enemy
    if (isDead(enemy)){
        return 2;
    }
}

void startDungeon(character player, int dungeonDifficulty){
    printf("You have entered %d\n", dungeonDifficulty);//welcome msg, subject to change
    chdir("dungeons");
    char dungeonName[];
    char *enemyStat;
    character enemy
    
    if(dungeonDifficulty ==1){
        strcpy(dungeonName,"dungeon1.txt");
        player.DUNGEON = 1;
    }
    else if(dungeonDifficulty ==2){
        strcpy(dungeonName,"dungeon2.txt");
        player.DUNGEON = 2;
    }
    else{
        strcpy(dungeonName,"dungeon3.txt");
        player.DUNGEON = 3;
    }
        
    FILE *fd = fopen(dungeonName, "r");
    int counter = 0;
    int numberOfFloors = 0;
    char *enemy;
    int statCounter = 0;
    if(file != NULL){
        char *line;
        while(fgets(line,sizeof(line),fp)!=NULL){
            counter++;
            if (!(counter%2) && counter > 1){
                processStats(*enemy,line);
                printf("You have encountered %s!\n",enemy.cname);
                numberOfFloors--;
            }
            if(counter == 1){
                numberofFloors = atoi(line);
            }
            if(!strcmp("Floor 1",line)){
                printf("This is the first floor\n");
            }
            if(endDungeon(numberOfFloors)){
                printf("You have reached the end of %s\n",dungeonName);
            }
            if(!strcmp("Floor 2",line)){
                printf("This is the second floor\n");
            }
            startBattle(character party[],enemy);
            numberOfFloors --;
            if(endDungeon(numberOfFloors)){
               printf("You have reached the end of %s\n",dungeonName); 
            }
            if(!strcmp("Floor 3",line)){
                printf("This is the third floor\n");
            }
                        strcpy(enemyStat,line);
            char * token;
            const char* str = strdup(enemy);
            while((token = strsep(&str," "))){
                statCounter++;
                processStats(enemy,*token,statCounter);
            }
            printf("You have encountered %s!\n",enemy.cname);
            startBattle(character party[],enemy);
            numberOfFloors --;
            if(endDungeon(numberOfFloors)){
               printf("You have reached the end of %s\n",dungeonName); 
            }
            
        }
        printf("You have conquered the dungeon!\n Returning to lobby");
        
    }
    
    char buffer[MESSAGE_BUFFER_SIZE];
    
    read(fd,buffer,MESSAGE_BUFFER_SIZE);
}