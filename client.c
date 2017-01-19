#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

/*void createAccount(char* user,char*pass){ //Should be a small easy example of 
  int f = open("accounts",O_WRONLY);        //an account creation.
  //char * emptybuff[2] = {"\n"};
  write(f,user,sizeof(user));
  write(f,pass,sizeof(pass));
  close(f);
  }*/
/*int * authenticate(char * user, char *pass){//basic log in process
  char buffer[255];                           //Need better process
  char buffer2[255];
  FILE *f = fopen("accounts","rt");
  int checker = 0;
  while(fgets(buffer,255,f)!= NULL){
    buffer[strlen(buffer)-1]='\0';
    if(!strcmp(user,buffer)){
      fgets(buffer2,255,f);
      buffer2[strlen(buffer2)-1] ='\0';
      if(!strcmp(pass,buffer2)){
	checker = 1;
      }
    }
  }
  return checker;
  }*/
int checkSYM(char * word) {
  strsep(&word, " !@#$^&*()_-+={[}]|\\:;\"\'<>?,./~`%");
  return word == 0;
}

int main( int argc, char *argv[] ) {

  char *host;
  if (argc != 2 ) {
    printf("host not specified, conneting to 127.0.0.1\n");
    host = "127.0.0.1";
  }
  else
    host = argv[1];

  int sd;

  struct character player;

  // char cname[MESSAGE_BUFFER_SIZE];  //client name
  //printf("enter client name: ");
  //fgets( player.cname, sizeof(player.cname),stdin);

  //printf("Choose your class:\n Knight \n Mage\n");

  //fgets( player.pclass, sizeof(player.pclass),stdin);

/*
  if(!strcmp(player.pclass, "Knight")){
    player.hp = 100;
    player.mp = 50;
    printf("knight chosen\n");
  }
  if(!strcmp(player.pclass, "Mage")){
    player.hp = 50;
    player.mp = 100;
    printf("mage chosen\n");
  }
*/

  sd = client_connect( host);
  char buffer[MESSAGE_BUFFER_SIZE];
  //char * input = buffer;
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
  if (! strcmp(buffer, "yes")) check = -1;
  else check = 1;
  
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
        if (strlen(buffer) <= 5) check = 2;
        else if (! checkSYM(buffer)) check = 3;
        else {
          write(sd, buffer, MESSAGE_BUFFER_SIZE);
          check = 0;
        }
      }
    }
  }
  
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
      if (check == -3) printf("That password does not exist\n");
      printf("password: ");
      fgets(buffer, MESSAGE_BUFFER_SIZE, stdin);
      buffer[strlen(buffer) - 1] = 0;
      write(sd, buffer, MESSAGE_BUFFER_SIZE);
      check = -3;
    }
    
    else break; //check = 0;
  }
  
  printf("Successful Login!\n");
  
  //write(sd, &player, sizeof(player));
  
  /*while (1) {

    printf("enter message: ");
    fgets( buffer, sizeof(buffer), stdin );
    char *p = strchr(buffer, '\n');
    *p = 0;

    write( sd, buffer, sizeof(buffer) );
    read( sd, buffer, sizeof(buffer) );
    printf( "received: %s\n", buffer );
  }*/

  return 0;
}
