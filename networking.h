#ifndef NETWORKING_H
#define NETWORKING_H

#define MESSAGE_BUFFER_SIZE 50

struct character {
  char cname[MESSAGE_BUFFER_SIZE];
  char pclass[MESSAGE_BUFFER_SIZE];
  int hp;
  int mp;
  
};

// #define PLAYER_SIZE (MESSAGE_BUFFER_SIZE+ (sizeof(int)*3))

void error_check( int i, char *s );

int server_setup();
int server_connect(int sd);

int client_connect( char *host );

#endif
