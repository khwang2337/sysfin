#ifndef NETWORKING_H
#define NETWORKING_H

#define MESSAGE_BUFFER_SIZE 50

typedef struct character {
  char cname[MESSAGE_BUFFER_SIZE];
  int CLASS_ID;
  int DUNGEON;
  
  int HP;
  int HP_LOST;
  int ATK;
  int MATK;
  int DEF;
  int MDEF;
  
  int MOVE1_ID;
  int MOVE2_ID;
  int MOVE3_ID;
  int MOVE4_ID;
  
  //Player only
  int DC;
  int SPID;
  int current_chat;
  char latest_cname[MESSAGE_BUFFER_SIZE];
  char latest_message[MESSAGE_BUFFER_SIZE];
  
  //Enemy only
  int CD;
  int ELEMENT;
  int MOVE5_ID;
  int MOVE6_ID;
  
  //For battle only
  int DEAD;
  
  int DOT;
  int DOT_TURN;
  int STUN;
  int STUN_TURN;
  
  int ATKBUFF;
  int ATKBUFF_TURNS;
  int MATKBUFF;
  int MATKBUFF_TURNS;
  int DEFBUFF;
  int DEFBUFF_TURNS;
  int MDEFBUFF;
  int MDEFBUFF_TURNS;
  
  int ATKDEB;
  int ATKDEB_TURNS;
  int MATKDEB;
  int MATKDEB_TURNS;
  int DEFDEB;
  int DEFDEB_TURNS;
  int MDEFDEB;
  int MDEFDEB_TURNS;
  
} character;

/*typedef struct message{
  char * intended;
  char * message;
  int mode;
} message;
*/
// #define PLAYER_SIZE (MESSAGE_BUFFER_SIZE+ (sizeof(int)*3))

void error_check( int i, char *s );

int server_setup();
int server_connect(int sd);

int client_connect( char *host );

#endif
