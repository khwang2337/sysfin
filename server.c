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
  char name[MESSAGE_BUFFER_SIZE]; //name buffer
  int check = 1; //while conditional
  while (check) { //checks conditional
    write(sd, "send", 5); //sends message to client telling it to prompt
    read(sd, name, MESSAGE_BUFFER_SIZE); //read name the client sends
    if ( checkAV(name) ) { //checks availability
      check = 0; //if available, end loop
    }
  }// else continue asking for name
  
}

int checkAV(char * name) {
  char NAME[MESSAGE_BUFFER_SIZE]; //account name buffer
  int pos = 0; //pos for char
  FILE *acc = fopen("accounts", "r"); //open file stream
  char token = getc(acc); //gets the first char
  if (token != EOF) { //checks if first char is EOF, if so return 1
    NAME[pos++] = token; //else put char at start of NAME
    while (token != EOF); { //conditional for EOF
      while (token = getc(acc) != ' ' && token != EOF) NAME[pos++] = token; //adds chars into NAME until the first space or EOF (not really expecting an EOF)
      if (! strcmp(name, NAME)) return 0; //check name if name already exists, return 0
      while (token = getc(acc) != '\n' && token != EOF); //pushes token past \n or EOF
      pos = 0; //resets pos
    }
  } 
  return 1; //either first token is EOF or name is not in file, good to go
}


void process( char * s ) {

  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}

//THESE WILL HAVE TO BE USED CRUCIALLY FOR SAVE PROGRESS AND COMMUNICATION
char * convertS(struct character player); //converts player struct to a string we can right into accounts
void convertC(struct character player); //converts string to player struct
