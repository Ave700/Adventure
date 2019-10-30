#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define ROOMQUANTITY 7
#define DATASIZE 400
//Here i call up the functions i gotta use so that main knows they exist

void MakeRoom(char* name,char* roomdir);
char** GenerateRooms(char* roomdir);
char* GenerateDirectory();
struct Room{
  char roomname[9];
  char c[6][DATASIZE];
  int cCount;
};
void ConnectRooms(struct Room* x, struct Room* y, char* roomdir);
int IsGraphFull(struct Room room[ROOMQUANTITY]);
int GetRandomRoom();
int CanAddConnectionFrom(struct Room* x);
int ConnectionAlreadyExists(struct Room* x, struct Room* y);
int IsSameRoom(struct Room* x, struct Room* y);
void AddRandomConnection(struct Room room[ROOMQUANTITY], char* roomdir);

//I AM GOING TO USE FALSE=0 and TRUE=1 I HOPE THIS WORKS
// Create all connections in graph
int main() 
{
 
 //I make a pointer that will point to the data returned by the
 //generatedirectory funciton. This will allow me to pass the dynamic
 //roomdirectory throughout my program.
 int i =0;
 char *roomdirptr;
 roomdirptr = GenerateDirectory();
 //char* selectednames[DATASIZE];
 char** selectednames; 
 selectednames = GenerateRooms(roomdirptr);

 //Simply makes a array of structs and sets each one's 
 //roomname to what we generated
 struct Room room[ROOMQUANTITY];
 for(i =0; i< ROOMQUANTITY; i++)
 {
   strcpy(room[i].roomname, selectednames[i]);
 }

 AddRandomConnection(room, roomdirptr);
 
 
 //while (IsGraphFull() == 0)
 //{
   //AddRandomConnection();
 //}
}


char* GenerateDirectory()
{
 //This returns a char string that is the location of a file
 //In the process it also creates this new file
 //Create a char to hold the program id
 char cpid[DATASIZE];
 int pid = getpid();
 //get the pid as an int value then print that int as a char
 //into the created cpid variable
 sprintf(cpid, "%d", pid);
 //creating preface for roomdir
 static char roomdir[DATASIZE] = "staubeav.rooms.";
 //concatinating roomdir with the process id
 strcat(roomdir, cpid);
 //If it is possible too this creates a file with full permissions
 //with the given directory route
 if (mkdir(roomdir,0777) == -1)
   fprintf( stderr, "%s", "Error in file creation\n");
  else
   fprintf( stdout, "%s", "\n");
 //I then return the directory route so other functions can
 //know where this new directory lives.
 return roomdir;
}


char** GenerateRooms(char* roomdir)
{
 //This randomly picks 7 names from a precreated array then creates 
 //7 files that are named with those names and contain a bit of text
 //These rooms are placed in the previously created directory
 //Simply creates an array of 10 names
 char *roombank[10];
 roombank[0] = "Zendo"; roombank[1] = "Dereks"; roombank[2] = "Aidans";
 roombank[3] = "Erichs"; roombank[4] = "Jakes"; roombank[5] = "Kitchen";
 roombank[6] = "Porch"; roombank[7] = "Basement"; roombank[8] = "Yard";
 roombank[9] = "Upstairs";

 //setting variable to be used in loop as well as a boolean type beat
 int r = 0; int i = 0; int j = 0; int exist = 0;
 //Starts a array with 7 values all out of range of our name array
 //this was mainly done for testing but if something goes wrong it will go 
 //very wrong and i will know where
 int roomselect[ROOMQUANTITY] = {10,10,10,10,10,10,10};
 //decided this function needs to export the selected names
          //char ** selectednames_func = malloc(10 * sizeof(char*));
  //added srand so its different per run
  srand(time(0));
 //This is the core of the function. It is a slightly builky while loop that counts to 7
 static char* selectednames[ROOMQUANTITY];
 while( i < ROOMQUANTITY )
 { 
   //I pull a random number between 0-9 (the range of my name bank)
   r = rand() % 10;
   //for the size of ints the loop has placed in the selection array
   //it will check to see if the new number already exists
   for(j=0; j < i+1; j++)
   {
     //if the for loop finds a duplicate it sets exist to 1
     if(roomselect[j]==r)
     { 
       exist = 1;
     }
   }
   //if the for loop doesnt find a duplicate it sets the current spot
   //in the array to the random number, then generates a room with that name
   
   if(exist != 1)
   {
     roomselect[i] = r;
     MakeRoom(roombank[roomselect[i]], roomdir);
     selectednames[i] =roombank[roomselect[i]];
     i++;
   }
   //sets exist back to 0 to start it all over again!!
   exist = 0;
 }
 return selectednames;
}


void MakeRoom(char* name, char* roomdir)
{
  //This is passed the directory to make rooms in and creates rooms
  //I am not skilled in C
  //This chunk of code is simply creating the filepath of
  // ./staubeav.rooms.PID/roomname
  char filepath[DATASIZE] = "./";
  char addslash[DATASIZE] = "/";
  strcat(filepath, roomdir);
  strcat(filepath, addslash);
  strcat(filepath, name);
  //Now made a roomname format to add
  char roomname[DATASIZE] = "ROOMNAME: ";
  //concatinating the roomname with the rooms name lol 
  strcat(roomname, name);
  //now creating file in append mode, writing to file, and closing file
  FILE * fPtr;
  fPtr = fopen(filepath, "a");
  fputs(roomname, fPtr);
  fclose(fPtr);
}


int IsGraphFull(struct Room room[ROOMQUANTITY])  
{
  int i = 0;
  int fail = 0;
  for(i = 0; i < ROOMQUANTITY; i++)
  {
    if(room[i].cCount < 3)
      fail = 1;
  }
  if(fail == 1)
   return 0;
  else
   return 1;
}


// Adds a random, valid outbound connection from a Room to another Room
void AddRandomConnection(struct Room room[ROOMQUANTITY], char* roomdir)  
{
  int roomX;
  int roomY;

  while(1 == 1)
  {
    roomX = GetRandomRoom();

    if (CanAddConnectionFrom(&room[roomX]) == 1)
      break;
  }

  do
  {
    roomY = GetRandomRoom();
  }
  while(CanAddConnectionFrom(&room[roomY]) == 0 || IsSameRoom(&room[roomX], &room[roomY]) == 1 || ConnectionAlreadyExists(&room[roomX], &room[roomY]) == 1);

  //ConnectRooms(&room[roomX], &room[roomY], roomdir);  // TODO: Add this connection to the real variables, 
  
}

// Returns a random Room, does NOT validate if connection can be added
int GetRandomRoom()
{
  //because of my implimentation i believe its easier to just return a random
  //number between 0-6 and use that in my struct array call. so ima do that
  srand(time(0));
  int r = rand() % ROOMQUANTITY;
  return r;
}


// Returns true if a connection can be added from Room x (< 6 outbound connections), false otherwise
int CanAddConnectionFrom(struct Room* x) 
{
  if(x->cCount < 6)
   return 1;
  else
   return 0;
}


// Returns true if a connection from Room x to Room y already exists, false otherwise
int ConnectionAlreadyExists(struct Room* x, struct Room* y)
{
  int exists = 0;
  int i;
  for(i = 0; i < x->cCount; i++)
  {
    if(strcmp(x->c[i], y->roomname)==0)
     exists=1;
  }

  if(exists == 0)
   return 0;
  else
   return 1;
}

// Connects Rooms x and y together, does not check if this connection is valid
void ConnectRooms(struct Room* x, struct Room* y, char* roomdir) 
{
  //This function might have excessive code but does it look LIKE I CARE 
  //This portion is for internal within program memory keeping
  //I copy the roomname from the opposite room struct and place it inside the c[i] of first
  //and vice versa. In this case i is determined by how many connections are made already inside
  // of the individual struc
  strcpy(x->c[x->cCount], y->roomname);
  x->cCount++;
  strcpy(y->c[y->cCount], x->roomname);
  y->cCount++;
  //This is very similar to the MakeRooms file manipulation
  //But it is done doubly so both rooms' files are modified.
  char addslash[DATASIZE] = "/";
  char roomXFP[DATASIZE];
  char roomYFP[DATASIZE];
  strcat(roomXFP, roomdir);
  strcat(roomYFP, roomdir);
  strcat(roomXFP, addslash);
  strcat(roomYFP, addslash);
  strcat(roomXFP, x->roomname);
  strcat(roomYFP, y->roomname);
  char newconnectionX[DATASIZE] = "\nCONNECTION: ";
  char newconnectionY[DATASIZE] = "\nCONNECTION: ";
  //each room gets the opposite rooms name in its file just like the above internal
  // memory stuff i did. sorry i lost track of my sentance. 
  strcat(newconnectionX, y->roomname);
  strcat(newconnectionY, x->roomname);
  //file accessing to append
  FILE * fPtr;
  fPtr = fopen(roomXFP, "a");
  fputs(newconnectionX, fPtr);
  fclose(fPtr);
  //now for the second room
  fPtr = fopen(roomYFP, "a");
  fputs(newconnectionY, fPtr);
  fclose(fPtr);
}

// Returns true if Rooms x and y are the same Room, false otherwise

int IsSameRoom(struct Room* x, struct Room* y) 
{
  int sameRoomTest = strcmp(x->roomname, y->roomname);
  if(sameRoomTest == 0)
    return 1;
  else
    return 0;
}

