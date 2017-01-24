#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

int checkSYM(char * word) {
  strsep(&word, " !@#$^&*()_-+={[}]|\\:;\"\'<>?,./~`%");
  return word == 0;
}

int opening(int sd) {
  char buffer[MESSAGE_BUFFER_SIZE];
  
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

void registerC(int sd) {
  char buffer[MESSAGE_BUFFER_SIZE] = {0};
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

void loginC(int sd) {
  char buffer[MESSAGE_BUFFER_SIZE] = {0};
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

void chatSYS(int sd, int * chat_set, char * curr_whisp, int * can_do) {
  char buffer[MESSAGE_BUFFER_SIZE];
  char buffer2[MESSAGE_BUFFER_SIZE];
  char * temp;
  char * command;
  //char * check;
  
  while (1) {
    if (! *can_do) {
      if ( (*chat_set) == 0) printf(WHT "Lobby: " RESET);
      else if ( (*chat_set) == 1) printf(GRN "Party: " RESET);
      else if ( (*chat_set) == 2) printf(MAG "Whisper %s: " RESET, curr_whisp);
      fgets(buffer, MESSAGE_BUFFER_SIZE, stdin);
      *can_do = 1;
      buffer[strlen(buffer) - 1] = 0;
      
      //printf("I'm here!\n");
      
      if (buffer[0] != '/') {
        if (*chat_set == 2) sprintf(buffer2, "%d %s %s", *chat_set, curr_whisp, buffer);
        else sprintf(buffer2, "%d %s", *chat_set, buffer);
        //printf("I'm here!\n");
        strcpy(buffer, buffer2);
        //printf("I'm here!\n");
      }
      else {
        //IS A COMMAND CHECK FOR CERTAIN COMMANDS
        printf("I'm in commandcheck!\n");
        strcpy(temp, buffer);
        strsep(&temp, "/");
        command = strsep(&temp, " ");
        
        if ( (! strcmp(command, "lobby")) || (! strcmp(command, "l")) ) {
          *chat_set = 0;
          break;
        }
        else if ( (! strcmp(command, "party")) || (! strcmp(command, "p")) ) {
          *chat_set = 1;
          break;
        }
        else if ( (! strcmp(command, "whisper")) || (! strcmp(command, "w")) ) {
          *chat_set = 2;
          strcpy(curr_whisp, temp); 
          break;
        }
        else if (! strcmp(command, "help")) printf("%s\n", HELP);
      }
      //printf("I'm performing the write\n");
      //printf("sent message %s\n", buffer);
      write(sd, buffer, MESSAGE_BUFFER_SIZE);
      //printf("I've done the write!!\n");
    }
  }
}

void ENDCUR(int sd, int * chat_set, char * curr_whisp, int * can_do) { //does graphical display and ncurses 
  char buffer[MESSAGE_BUFFER_SIZE];
  char * buffer2 = (char *)malloc(MESSAGE_BUFFER_SIZE);
  char * lead;
  char * command;
  int L;
  
  while (1) {
      read(sd, buffer, MESSAGE_BUFFER_SIZE);
      //printf("I've read something new!!!!\n");
      //printf("new message: %s\n", buffer);
      strcpy(buffer2, buffer);
      //printf("buffer2: %s\n", buffer2); 
      lead = strsep(&buffer2, " ");
      //sscanf(buffer, "%d %s", &L, buffer2);
      //L = buffer[0];
      //printf("lead: %s\n", lead);
      //printf("buffer2: %s\n", buffer2);
      if (strcmp(lead, "/")) { //DO COLORS
        if (! strcmp(lead, "0")) printf(WHT "%s\n" RESET, buffer2);
        else if (! strcmp(lead, "1")) printf(GRN "%s\n" RESET, buffer2);
        else if (! strcmp(lead, "2")) printf(MAG "%s\n" RESET, buffer2);
        else if (! strcmp(lead, "3")) printf(CYN "%s\n" RESET, buffer2);
        *can_do = 0;
      }
      else {
        command = strsep(&buffer2, " ");
        if (! strcmp(command, "whisper")) {
          curr_whisp = strsep(&buffer2, " ");
          *chat_set = 2;
        }
      }
    }
  //else server command
}

int main( int argc, char *argv[] ) {
  int can_do = 0;
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
  
  printf(CYN);
  
  int check = opening(sd); //asks whether they have an account
  if (check > 0) registerC(sd); //registers them
  else loginC(sd); //logs them in
  
  
  printf("Welcome to the Game.\n" RESET);
  
  char buffer2[MESSAGE_BUFFER_SIZE];
  //class_select(sd, buffer2);
  int subPID = fork();
  if (subPID) chatSYS(sd, &chat_set, curr_whisp, &can_do); //THIS SHOULD WORK RIGHT???
  else ENDCUR(sd, &chat_set, curr_whisp, &can_do);
  
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

