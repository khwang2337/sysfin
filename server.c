#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

void process( char * s );
void sub_server( int sd );

int main() {

  int sd, connection;
  int count = 0;

  sd = server_setup();

  while (1) {

    connection = server_connect( sd );

    int f = fork();
    if ( f == 0 ) {

      close(sd);
      sub_server( connection );
      count +=1;
      exit(0);
    }
    else {
      close( connection );
    }
  }
  return 0;
}


void sub_server( int sd ) {
  struct character player;
  // char cname[MESSAGE_BUFFER_SIZE];
  /*
  read(sd, &player, sizeof(player));
  printf("%s connected\n",player.cname);

  char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {
    printf("[SERVER %d] from %s received: %s\n", getpid(), player.cname, buffer );
    process( buffer );
    write( sd, buffer, sizeof(buffer));
  }
  */
  char buffer[MESSAGE_BUFFER_SIZE];
  
  read(sd, buffer, MESSAGE_BUFFER_SIZE);
  if (! strcmp(buffer,"yes")) login(sd); //does login procedure
  else registerr(sd); //does register procedure
  
  
}

void registerr(int sd, struct character player) {
  char name[MESSAGE_BUFFER_SIZE];
  int check = 1;
  while (check) {
    write(sd, "send", 5);
    read(sd, name, MESSAGE_BUFFER_SIZE);
    if ( checkAV(name) ) {
      check = 0;
    }
  }
  
}

int checkAV(char * name) {
  //FILE *f = fopen("file.txt", "r");
  //int c;
  //while ((c = getc(f)) != '#' && c != EOF)
  //  putchar(c);
  char NAME[MESSAGE_BUFFER_SIZE];
  int pos = 0;
  FILE *acc = fopen("accounts", "r");
  char token = getc(acc);
  if (token != EOF) {
    NAME[pos++] = token;
    while (token != EOF); {
      while (token = getc(acc) != ',' && token != EOF) NAME[pos++] = token;
      if (strcmp(name, NAME)) return 0;
      while (token = getc(acc) != '\n' && token != EOF); 
      pos = 0;
    }
  }
  return 1;
}


void process( char * s ) {

  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}

char * convert(struct character player);
