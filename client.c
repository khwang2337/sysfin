#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

int checkSYM(char * word) {
  strsep(&word, " !@#$^&*()_-+={[}]|\\:;\"\'<>?,./~`%");
  return word == 0;
}

int opening(int sd, char buffer[]) {
  int check = 1;
  while (check) { //ask client if they have an account
    printf("Do you already have an account? Enter yes or no: ");
    fgets(buffer, MESSAGE_BUFFER_SIZE, stdin);
    if ( buffer[strlen(buffer) - 1] != '\n' ) { //checks for out of bounds
        printf("Error: Out of bounds; character limit is %d\n", MESSAGE_BUFFER_SIZE - 1); 
        int clear;
        while ( (clear = getchar()) != '\n' && clear != EOF); //clears stdin
    }
    else {
      buffer[ strlen(buffer) - 1 ] = 0; //replaces newline
      if ( (! strcmp(buffer,"yes")) || (! strcmp(buffer,"no")) ) {
        write(sd, buffer, strlen(buffer));
        check = 0;
      }
    }
  }
  if (! strcmp(buffer, "yes")) return -1;
  else return 1;
}

void registerC(int sd, char buffer[]) {
  int check = 1;
  
  while (check > 0) {
    read(sd, buffer, MESSAGE_BUFFER_SIZE);
    if (! strcmp(buffer, "makeuser")) {
      while (check) {
        if (check == 2) printf("Too Short! Must be more than 5 characters!\n");
        if (check == 3) printf("Invalid symbol used!\n");
        printf("Preferred username: ");
        fgets(buffer, MESSAGE_BUFFER_SIZE, stdin);
        buffer[ strlen(buffer) - 1] = 0;
        if (strlen(buffer) <= 5) check = 2;
        else if (! checkSYM(buffer)) check = 3;
        else {
          write(sd, buffer, MESSAGE_BUFFER_SIZE);
          check = 0;
        }
      }
      check = 1;
    }
    else if (! strcmp(buffer, "makepass")) {
      while (check) {
        if (check == 2) printf("Too Short! Must be more than 5 characters!\n");
        if (check == 3) printf("Invalid symbol used!\n");
        printf("Preferred password?: ");
        fgets(buffer, MESSAGE_BUFFER_SIZE, stdin);
        if (strlen(buffer) <= 6) check = 2;
        else if (! checkSYM(buffer)) check = 3;
        else {
          write(sd, buffer, MESSAGE_BUFFER_SIZE);
          check = 0;
        }
      }
    }
  }
  //continue register
}

void loginC(int sd, char buffer[]) {
  int check = -1;
  
  while (check < 0) {
    read(sd, buffer, MESSAGE_BUFFER_SIZE);
    if (! strcmp(buffer, "username")) {
      if (check == -2) printf("That username does not exist\n");
      printf("username: ");
      fgets(buffer, MESSAGE_BUFFER_SIZE, stdin);
      buffer[strlen(buffer) - 1] = 0;
      write(sd, buffer, MESSAGE_BUFFER_SIZE);
      check = -2;
    }
    
    else if (! strcmp(buffer, "password")) {
      if (check == -3) printf("Incorrect password\n");
      printf("password: ");
      fgets(buffer, MESSAGE_BUFFER_SIZE, stdin);
      buffer[strlen(buffer) - 1] = 0;
      write(sd, buffer, MESSAGE_BUFFER_SIZE);
      check = -3;
    }
    
    else break; //check = 0;
  }
}

void class_select(int sd, char buffer[]){
  int check = 1;
  
  while (check > 0){
    read(sd, buffer, MESSAGE_BUFFER_SIZE);
    if (! strcmp(buffer, "no class")) {
      printf("1.Warrior\n2.Mage\n3.Hunter\nChoose your class:");
      fgets(buffer,MESSAGE_BUFFER_SIZE,stdin);
      write(sd, buffer, MESSAGE_BUFFER_SIZE);
    }
    else if (! strcmp(buffer, "classy")){
      printf("Class chosen!");
      write(sd,"success",MESSAGE_BUFFER_SIZE);
      check = 0;
    }
  }
}

void chatSYS(int sd, int * chat_set, char * curr_whisp) {
  char buffer[MESSAGE_BUFFER_SIZE];
  
  while (1) {
    //print message and gets message
    if ( (*chat_set) == 0) printf("Lobby: ");
    else if ( (*chat_set) == 1) printf("Party: ");
    else if ( (*chat_set) == 2) printf("Whisper %s: ", curr_whisp);
    fgets(buffer, MESSAGE_BUFFER_SIZE, stdin);
    //gets rid of newline
    buffer[strlen(buffer) - 1] = 0;
    //if it's not a command, append a number at the end
    if (buffer[0] != '/') {
      char temp[MESSAGE_BUFFER_SIZE];
      sprintf(temp, "%d %s", *chat_set, buffer);
      strcpy(buffer, temp);
    }
    write(sd, buffer, MESSAGE_BUFFER_SIZE); //server should write back to print "not a command" if it aint a command
  }
}

void ENDCUR(int sd) { //does graphical display and ncurses 
  char buffer[MESSAGE_BUFFER_SIZE];
  while (1) {
    read(sd, buffer, MESSAGE_BUFFER_SIZE);
    if (buffer[0] == '/') printf("%s\n", buffer);
  }
}

int main( int argc, char *argv[] ) {

  char *host;
  if (argc != 2 ) {
    printf("host not specified, connecting to 127.0.0.1\n");
    host = "127.0.0.1";
  }
  else
    host = argv[1];

  int sd;

  sd = client_connect( host);
  char buffer[MESSAGE_BUFFER_SIZE];
  //char * input = buffer;
  
  int chat_set = 0; //chat_setting for players
  char * curr_whisp = (char *)malloc(MESSAGE_BUFFER_SIZE);
  
  int check = opening(sd, buffer); //asks whether they have an account
  if (check > 0) registerC(sd, buffer); //registers them
  else loginC(sd, buffer); //logs them in
  
  
  printf("Welcome to the Game.\n");
  
  char buffer2[MESSAGE_BUFFER_SIZE];
  class_select(sd, buffer2);
  /*int subPID = fork();
  if (subPID) chatSYS(sd, &chat_set, curr_whisp); //THIS SHOULD WORK RIGHT???
  else ENDCUR(sd);*/
  
  return 0;
}


/*
int pos = 0;
char Btoken, Atoken
Atoken = buffer[pos];
while (buffer[pos] != NULL) {
  Btoken = Atoken;
  Atoken = buffer[pos + 1];
  buffer[pos + 1] = Btoken;
  pos++;
}
buffer[0] = '/';
*/

