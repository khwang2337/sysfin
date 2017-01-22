#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

#include "networking.h"

void process( char * s );
void sub_server( int sd );




int main() {

  int sd, connection;
  int count = 0;

  sd = server_setup();
  
  int shmd = shmget(420, 1024, IPC_CREAT | 0664);
  
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

void registerr(int sd, character player) {
  char name[MESSAGE_BUFFER_SIZE]; //name buffer
  char buffer[MESSAGE_BUFFER_SIZE];
  int check = 1; //while conditional
  
  while (check) { //checks conditional
    write(sd, "makeuser", 9); //sends message to client telling it to prompt
    read(sd, name, MESSAGE_BUFFER_SIZE); //read name the client sends
    if ( checkUSER(name, 0) ) { //checks availability
      printf("Username: %s is available\n", name);
      check = 0; //if available, end loop
      strcpy(player.cname, name);
    }
  }// else continue asking for name
  
  write(sd, "makepass", 9);
  read(sd, buffer, MESSAGE_BUFFER_SIZE);
  int fd = open(name, O_WRONLY, 0);
  write(fd, buffer, strlen(buffer)); //we want newline
  
  //REST WILL BE FILLED IN
}

int checkPASS(char * name, char * pass) {
  FILE * fp = fopen(name, "r");
  if (fp == NULL) return 0;
  else {
    char buffer[MESSAGE_BUFFER_SIZE];
    char token;
    char pos = 0;
    rewind(fp);
    while ( (token = getc(fp)) != '\n' && token != EOF) {
      buffer[pos++] = token;
    }
    buffer[pos] = 0;
    if (! strcmp(buffer, pass)) return 1;
    else return 0;
  }
}

void login(int sd, character player) {
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

//THESE WILL HAVE TO BE USED CRUCIALLY FOR SAVE PROGRESS AND COMMUNICATION
char * convertS(character player, char buffer[]) {
  printf("convertS used");
  sprintf(buffer, "%d %d %d %d %d %d %d %d %d %d %d %c",
  player.CLASS_ID,
  player.DUNGEON,   
  player.HP,
  player.ATK,
  player.MATK,
  player.DEF,
  player.MDEF,
  
  player.MOVE1_ID,
  player.MOVE2_ID,
  player.MOVE3_ID,
  player.MOVE4_ID,
  0);
  
  return buffer;
}
//converts player struct to a string we can write into accounts


void convertC(character player, char *info) {
  printf("convertC used");
  player.CLASS_ID = atoi(strsep(&info," "));
  player.DUNGEON = atoi(strsep(&info," "));
  player.HP = atoi(strsep(&info," "));
  player.ATK = atoi(strsep(&info," "));
  player.MATK = atoi(strsep(&info," "));
  player.DEF = atoi(strsep(&info," "));
  player.MDEF = atoi(strsep(&info," "));
  player.MOVE1_ID = atoi(strsep(&info," "));
  player.MOVE2_ID = atoi(strsep(&info," "));
  player.MOVE3_ID = atoi(strsep(&info," "));
  player.MOVE4_ID = atoi(strsep(&info," "));
} //converts string to player struct

void command(int sd, char buffer[], character * player) {
  //save
  //whisper /w chat setting, last whispered
  //party /p
  //lobby /a
  //
}
void request(int sd, char buffer[]) {
/*void chat(int sd, char buffer[]) {
  if (current_chat == 0) {
     
  }
  else if (current_chat == 1) {
    
  }
  else {
    
  }
*/
}

int getsPID(char * name) {
  
}

void chat(int sd, char buffer[], character player) {
  int shmd = shmget(420, 1024, 0);
  char MEM[1024] = shmat(shmd, 0, 0);
  char temp[MESSAGE_BUFFER_SIZE];
  int mode;
  if (buffer[0] == '0') {
    //sscanf(buffer, "%d %s", mode, temp);
    //sprintf(MEM, "%d/%s/all/%s", mode, player.cname, temp);
  }
  else if (buffer[0] == '1') {
    //sscanf(buffer, "%d %s", mode, temp);
    //sprintf(MEM, "%d/%s/%s %s %s %s /%s", mode, player.cname, party[0], party[1], party[2], party[3], temp);
  }
  else if (buffer[0] == '2') {
    char whispName[MESSAGE_BUFFER_SIZE];
    //sscanf(buffer, "%d %s %s", mode, whispName, temp);
    //sprintf(MEM, "%d/%s/%s/%s", mode, player.cname, player.cname, whispName, temp);
  }
  shmdt(m);
}


void interpret(int sd, character player) {
  char buffer[MESSAGE_BUFFER_SIZE];
  
  while (1) {
    read(sd, buffer, MESSAGE_BUFFER_SIZE);
    if (buffer[0] == '/') command(sd, buffer, player);
    else chat(sd, buffer, player);
  }
  
}

void update(int sd, character player) {
  int shmd = shmget(420, 1024, 0);
  char MEM[1024] = shmat(shmd, 0, 0); //struct message * m, address is just message m <-- pointer
  char name[MESSAGE_BUFFER_SIZE] = {0};
  char buffer[MESSAGE_BUFFER_SIZE];
  char message[MESSAGE_BUFFER_SIZE];
  char intended[MESSAGE_BUFFER_SIZE];
  char author[MESSAGE_BUFFER_SIZE];
  int mode;
  
  strcpy(name, player.cname);
  name[strlen(name)] = ' ';
  
  while (1) {
    //check shared memory
    if (strstr(MEM, name) != NULL && strcmp(player.last_message, MEM)) {
      strcpy(player.last_message, MEM);
      sscanf(MEM, "%d/%s/%s/%s", mode, author, intended, message);
      
      if (mode == 0) sprintf(buffer, "%d/%s: %s", mode, author, message);
      else if (mode == 1 && strstr(intended, name) != NULL) sprintf(buffer, "%d/%s: %s", mode, author, message);
      else if (mode == 2 && ( (! strcmp(intended, player.cname)) || (! strcmp(author, player.cname)) ) ) {
        if (! strcmp(author, player.cname) sprintf(buffer, "%d/To %s: %s", mode, intended, temp);
        else sprintf(buffer, "%d/From %s: %s", mode, author, temp);
      }
      
    }
    
    
    //update
    
  }
}

void save(character player) {
  
}

//THIS SHIT DONT WORK :D PLEASE HELP ME :-:
void setup(character player, int sd) {
  char buffer[MESSAGE_BUFFER_SIZE];
  int check = 1;
  
  while (check > 0){
    if (player.CLASS_ID a == 0){
      write(sd, "no class", MESSAGE_BUFFER_SIZE);
      read(sd, buffer, MESSAGE_BUFFER_SIZE);
      int classChoice = atoi(buffer);
      if (classChoice > 3 || classChoice <= 0 ) printf("Invalid Choice!");
      else if (classChoice == 1) convertC(player,"1 1 500 100 100 100 100 1 1 1 1");//Warrior
      else if (classChoice == 2) convertC(player,"2 1 500 100 100 100 100 1 1 1 1");//Mage
      else if (classChoice == 3) convertC(player,"3 1 500 100 100 100 100 1 1 1 1");//Hunter
    }
    else if (player.CLASS_ID != 0) {
      write(sd, "classy", MESSAGE_BUFFER_SIZE);
      read(sd, buffer, MESSAGE_BUFFER_SIZE);
      if (!strcmp(buffer, "success")){
        int fd = open(player.cname,O_CREAT|O_APPEND|O_WRONLY,0644);
        char * a = convertS(player,buffer);
        printf("yolo%s\nyolo",a);
        write(fd, a ,MESSAGE_BUFFER_SIZE);
        close(fd);
      }
    }
  }
}

void sub_server( int sd ) {
  character player = {0};
  strcpy(player.cname, "");
  strcpy(player.latest_cname, "");
  strcpy(player.latest_message, "");

  char buffer[MESSAGE_BUFFER_SIZE];
  
  read(sd, buffer, MESSAGE_BUFFER_SIZE);
  if (! strcmp(buffer,"yes")) login(sd, player); //does login procedure
  else registerr(sd, player); //does register procedure
  
  setup(player, sd);
  
  int pid = fork();
  if (pid) interpret(sd, player);
  else update(sd, player);
  
}



void process( char * s ) {

  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}


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