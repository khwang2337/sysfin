#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

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

int checkUSER(char * name, int num) { 
  chdir("accounts/");
  int fd;
  if (num) fd = open(name, O_RDWR, 0);
  else fd = open(name, O_CREAT | O_EXCL, 0644);
  if (fd == -1) return 0;
  else return 1;
}

void registerr(int sd, struct character player) {
  char name[MESSAGE_BUFFER_SIZE]; //name buffer
  char buffer[MESSAGE_BUFFER_SIZE];
  int check = 1; //while conditional
  
  while (check) { //checks conditional
    write(sd, "makeuser", 9); //sends message to client telling it to prompt
    read(sd, name, MESSAGE_BUFFER_SIZE); //read name the client sends
    if ( checkUSER(name, 0) ) { //checks availability
      printf("Username: %s is available\n", name);
      check = 0; //if available, end loop
    }
  }// else continue asking for name
  
  write(sd, "makepass", 9);
  read(sd, buffer, MESSAGE_BUFFER_SIZE);
  int fd = open(name, O_WRONLY, 0);
  write(fd, buffer, strlen(buffer)); //we want newline
  
  //REST WILL BE FILLED IN
  
}


void login(int sd, struct character player) {
  char buffer[MESSAGE_BUFFER_SIZE];
  char name[MESSAGE_BUFFER_SIZE];
  int check = 1;
  
  while (check) {
    write(sd, "username", 9);
    read(sd, name, MESSAGE_BUFFER_SIZE);
    if ( checkUSER(name, 1) ) {
      printf("username accepted\n");
      check = 0;
    }
  }
  
  check = 1;
  while (check) {
    write(sd, "password", 9);
    read(sd, buffer, MESSAGE_BUFFER_SIZE);
    if ( checkPASS(name, buffer) ) {
      printf("password accepted");
      check = 0;
    }
  }
  
  write(sd, "successful login", 17);
  
}

int checkPASS(char * name, char * pass) {
  FILE * fp = fopen(name, "r");
  if (fp == NULL) return 0;
  else {
    char buffer[MESSAGE_BUFFER_SIZE];
    char token;
    char pos = 0;
    while (token = getc(fp) != '\n' && token != EOF) {
      buffer[pos++];
    }
    buffer[pos] = 0;
    return strcmp(buffer, pass);
  }
}

void sub_server( int sd ) {
  struct character player;

  char buffer[MESSAGE_BUFFER_SIZE];
  
  read(sd, buffer, MESSAGE_BUFFER_SIZE);
  if (! strcmp(buffer,"yes")) login(sd, player); //does login procedure
  else registerr(sd, player); //does register procedure
  
  
}



void process( char * s ) {

  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}

//THESE WILL HAVE TO BE USED CRUCIALLY FOR SAVE PROGRESS AND COMMUNICATION
char * convertS(struct character player); //converts player struct to a string we can write into accounts
void convertC(struct character player); //converts string to player struct

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