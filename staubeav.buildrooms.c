#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define DATASIZE 400
void MakeRoom(char* name,char* roomdir);
void GenerateRooms(char* roomdir);
char* GenerateDirectory();
/*this is a program in c*/
//I AM GOING TO USE FALSE=0 and TRUE=1 I HOPE THIS WORKS
// Create all connections in graph
int main() 
{

 char *roomdirptr;
 roomdirptr = GenerateDirectory();
 GenerateRooms(roomdirptr);

 //while (IsGraphFull() == 0)
 //{
   //AddRandomConnection();
 //}
}


char* GenerateDirectory()
{
 //Then i create a directory to put rooms in
//Taking the pid and changing it to a string
 //then concatinating the string with my rooms to make a file
 char cpid[DATASIZE];
 int pid = getpid();
 sprintf(cpid, "%d", pid);
 static char roomdir[DATASIZE] = "staubeav.rooms.";
 strcat(roomdir, cpid);
 //creating directory of rooms and checking if it fails
 if (mkdir(roomdir,0777) == -1)
   fprintf( stderr, "%s", "Error in file creation\n");
  else
   fprintf( stdout, "%s", "File Created\n");
 return roomdir;
}

void GenerateRooms(char* roomdir)
{
 //Used const for name array cause they should NEVER CHANGE
 char *roombank[10];
 roombank[0] = "Zendo"; roombank[1] = "Dereks"; roombank[2] = "Aidans";
 roombank[3] = "Erichs"; roombank[4] = "Jakes"; roombank[5] = "Kitchen";
 roombank[6] = "Porch"; roombank[7] = "Basement"; roombank[8] = "Yard";
 roombank[9] = "Upstairs";

 //created a int at 99 so it will error if it isnt set to a random
 int r = 0;
 int i = 0;
 int j = 0;
 int exist = 0;
 //r = rand()%10;
 //printf("%d", r);
 int roomselect[7] = {10,10,10,10,10,10,10};

 while( i < 7 )
 {
   
   r = rand() % 10;

   for(j=0; j < i+1; j++)
   {
     if(roomselect[j]==r)
     {
       exist = 1;
     }
   }
   if(exist != 1)
   {
     roomselect[i] = r;
     printf("%d\n", roomselect[i]);
     MakeRoom(roombank[roomselect[i]]);
     i++;
   }
   
   
   exist = 0;
   
 }
/*
 int roomnumber;
 printf(roombank[roomnumber]);
 int roomcount;
 int *roomselectptr;
 
 
 while(roomcount < 7)
 { 
   roomselectptr = &(roomselect[roomnumber]);
   
   //printf("%d\n", *roomselectptr);
   MakeRoom(roombank[*roomselectptr], roomdir);
   roomcount++;
 }
 */
}

//This is passed the directory to make rooms in and creates rooms
void MakeRoom(char* name, char* roomdir)
{
  //I am not skilled in C
  //This chunk of code is simply creating the filepath of
  // ./staubeav.rooms.XXXXXX/roomname
  char filepath[DATASIZE] = "./";
  char addslash[DATASIZE] = "/";
  strcat(filepath, roomdir);
  strcat(filepath, addslash);
  strcat(filepath, name);
  //Now made a roomname format to add
  char roomname[DATASIZE] = "ROOMNAME: ";
  strcat(roomname, name);
  //now creating file in append mode, writing to file, and closing file
  FILE * fPtr;
  fPtr = fopen(filepath, "a");
  fputs(roomname, fPtr);
  fclose(fPtr);
}

/*
bool IsGraphFull()  
{
  ...
}

// Adds a random, valid outbound connection from a Room to another Room
void AddRandomConnection()  
{
  Room A;  // Maybe a struct, maybe global arrays of ints
  Room B;

  while(1 == 1)
  {
    A = GetRandomRoom();

    if (CanAddConnectionFrom(A) == true)
      break;
  }

  do
  {
    B = GetRandomRoom();
  }
  while(CanAddConnectionFrom(B) == false || IsSameRoom(A, B) == true || ConnectionAlreadyExists(A, B) == true);

  ConnectRoom(A, B);  // TODO: Add this connection to the real variables, 
  ConnectRoom(B, A);  //  because this A and B will be destroyed when this function terminates
}

// Returns a random Room, does NOT validate if connection can be added
Room GetRandomRoom()
{
  ...
}

// Returns true if a connection can be added from Room x (< 6 outbound connections), false otherwise
bool CanAddConnectionFrom(Room x) 
{
  ...
}
// Returns true if a connection from Room x to Room y already exists, false otherwise
bool ConnectionAlreadyExists(x, y)
{
  ...
}

// Connects Rooms x and y together, does not check if this connection is valid
void ConnectRoom(Room x, Room y) 
{
  ...
}

// Returns true if Rooms x and y are the same Room, false otherwise
bool IsSameRoom(Room x, Room y) 
{
  ...
}

*/

