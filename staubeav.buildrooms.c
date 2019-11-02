#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define ROOMQUANTITY 7
#define DATASIZE 1000
//Here i call up the functions i gotta use so that main knows they exist

void MakeRoom(char* name,char* roomdir);
char** GenerateRooms(char* roomdir);
char* GenerateDirectory();
//struct proto is called before the rest of the functions since they all rely on it
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
void InstallOrder(struct Room room[ROOMQUANTITY], char* roomdir);

//I AM GOING TO USE FALSE=0 and TRUE=1 I HOPE THIS WORKS (it did work)
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
   //cCount must be set to 0 cause it messed up my program otherwise
   room[i].cCount = 0;
 }
 
 //while there arent enough connections just add more
 //simple
 while (IsGraphFull(room) == 0)
 {
   AddRandomConnection(room, roomdirptr);
 }
 //once all connections are made we add the room types and thats IT
 InstallOrder(room, roomdirptr);
}


// returns a string of a directory name created with the PID
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
  //can be modified if other statement is prefered.
   fprintf( stdout, "%s", "");
 //I then return the directory route so other functions can
 //know where this new directory lives.
 return roomdir;
}

//calls MakeRoom on a randomly selected list of 7 roomnames
//lots of info inside
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

//creates a file named the passed name inside the passed directory
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

//returns true if every room has 3 connections
int IsGraphFull(struct Room room[ROOMQUANTITY])  
{
  //this walks through my struct array
  int i = 0;
  int fail = 0;
  //for every room it checks to see if the cCount (such a solid idea imo) is less then 3
  //if it isnt then keep on GOING!!!
  for(i = 0; i < ROOMQUANTITY; i++)
  {
    if(room[i].cCount < 3)
      fail = 1;
  }
  if(fail == 0)
   return 1;
  else
   return 0;
}


// Adds a random, valid outbound connection from a Room to another Room
//this was mostly given to me i just changed a few things
void AddRandomConnection(struct Room room[ROOMQUANTITY], char* roomdir)  
{
  //i pass my entire struct array so it can have a ball
  //start the numbers at values that will give me seg faults
  int roomX = -99;
  int roomY = -99;

  //until the function can add connection it calls more rooms(random ints)
  while(1 == 1)
  {
    roomX = GetRandomRoom();

    if (CanAddConnectionFrom(&room[roomX]) == 1)
      break;
  }
 
 //similarly till we can connect the two rooms call more ints
  do
  {
    roomY = GetRandomRoom();
  }
  while(CanAddConnectionFrom(&room[roomY]) == 0 || IsSameRoom(&room[roomX], &room[roomY]) == 1 || ConnectionAlreadyExists(&room[roomX], &room[roomY]) == 1);

  //once a connection can be made this slaps the two together in one neat lil function there is more info in ConnectRooms();
  ConnectRooms(&room[roomX], &room[roomY], roomdir); 
}

// Returns a random number, honestly didnt need a function for this but whatever
int GetRandomRoom()
{
  //because of my implimentation i believe its easier to just return a random
  //number between 0-6 and use that in my struct array call. so ima do that
  int r=0;
  r = rand() % 7;
  return r;
}


// Returns 1 if a connection can be added from Room x (< 6 outbound connections), false otherwise
int CanAddConnectionFrom(struct Room* x) 
{
  //my cCount variable is useful. It is a counter to see how many connections each room has made thusfar
  //since i have this i can simply check to make sure the room has less the 6 connections
  //meaning another can be added.
  //returns true if it can add
  if(x->cCount < 6)
   return 1;
  else
   return 0;
}


// Returns 1 if a connection from Room x to Room y already exists, false otherwise
int ConnectionAlreadyExists(struct Room* x, struct Room* y)
{
  //I use exists as a bool
  int exists = 0;
  int i;
  //i only check one side because since the connections are mirroed
  //if the connection exists in one room it has to exist in the other room
  //the for loop counts only the amount of connections the room has made. 
  //this way it doest waste time
  for(i = 0; i < x->cCount; i++)
  {
    //my for logic it simply does a string compare to see
    //if the name of the second room exists anywhere in the already made connections

    if(strcmp(x->c[i], y->roomname)==0)
     exists=1;
  }
  //if the for loop doesnt change the exists then it returns false (connection does NOT already exist)
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
  char roomXFP[DATASIZE] = "";
  char roomYFP[DATASIZE] = "";
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
  
  FILE * fPtr2;
  fPtr2 = fopen(roomYFP, "a");
  fputs(newconnectionY, fPtr2);
  fclose(fPtr2);
  
}

//I really liked this function name, creates room order and adds to the files
void InstallOrder(struct Room room[ROOMQUANTITY], char* roomdir)
{
  //The code this function uses is explained elsewhere 
  //Its my implementation of writing to file, ive used it for
  //MakeRoom and COnnectRoom
  int i;
  char addslash[DATASIZE] = "/";
  char startRM[DATASIZE] = "\nROOM TYPE: START_ROOM";
  //This places the start room call on the first room in array
  char roomSFP[DATASIZE] = "";
  strcat(roomSFP, roomdir);
  strcat(roomSFP, addslash);
  strcat(roomSFP, room[0].roomname);
  FILE * fPtr;
  fPtr = fopen(roomSFP, "a");
  fputs(startRM, fPtr);
  fclose(fPtr);
  //this fills all middle elements with the mid room call
  char endRM[DATASIZE] = "\nROOM TYPE: END_ROOM";
  char middleRM[DATASIZE] = "\nROOM TYPE: MID_ROOM";
  for(i=1; i<ROOMQUANTITY-1; i++)
  {
   char roomMFP[DATASIZE] = "";
   strcat(roomMFP, roomdir);
   strcat(roomMFP, addslash);
   strcat(roomMFP, room[i].roomname);
   fPtr = fopen(roomMFP, "a");
   fputs(middleRM, fPtr);
   fclose(fPtr);
  }
  //This places the end room call on the final room in the array
  char roomEFP[DATASIZE] = "";
  strcat(roomEFP, roomdir);
  strcat(roomEFP, addslash);
  strcat(roomEFP, room[ROOMQUANTITY-1].roomname);
  fPtr = fopen(roomEFP, "a");
  fputs(endRM, fPtr);
  fclose(fPtr);
}

// Returns 1 if Rooms x and y are the same Room, false otherwise
int IsSameRoom(struct Room* x, struct Room* y) 
{
  //This compares the names of the two passed room structs
  //If they are equal then it returns 1(which is true for me)
  //otherwise it returns 0
  int sameRoomTest = strcmp(x->roomname, y->roomname);
  if(sameRoomTest == 0)
    return 1;
  else
    return 0;
}

