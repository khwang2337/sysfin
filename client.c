#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

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
  printf("enter client name: ");
  fgets( player.cname, sizeof(player.cname),stdin);

  printf("Choose your class:\n Knight \n Mage\n");

  fgets( player.pclass, sizeof(player.pclass),stdin);

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

  sd = client_connect( host);

  char buffer[MESSAGE_BUFFER_SIZE];
  write(sd, &player, sizeof(player));

  while (1) {

    printf("enter message: ");
    fgets( buffer, sizeof(buffer), stdin );
    char *p = strchr(buffer, '\n');
    *p = 0;

    write( sd, buffer, sizeof(buffer) );
    read( sd, buffer, sizeof(buffer) );
    printf( "received: %s\n", buffer );
  }

  return 0;
}
