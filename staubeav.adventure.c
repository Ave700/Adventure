/* this is an adveture game in c*/
#include <stdio.h>
#include<string.h> 
#include <unistd.h>
#include <termios.h>
#include <stdlib.h> 
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>
#define DATASIZE 1000
//All our function calls are up here so threads know they exists
char* FindDirectory();
char* MakeFilePath(char* dirname);
char* FindStart(char* roomdirFP, char* connection[]);
int EndRoomCheck(char* curroom, char* roomdir);
int FindConnections(char* curroom, char* roomdir, char* connection[]);
void PrintTime();
void ConnectionCutting(char* connectionline, char* CCtarget);
int ChangeRoom(char* newroom, char* roomdir, char* connection[]);
//All our thread inits. 
pthread_t threadID[2];
//included COND so i could do the do
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 

//This is the timeFunc. It is run as a thread and it dont ever stop so watchout
void* timeFunc(void *arg){
  //Once its started it runs constantly waiting for control
  while(1==1)
  {
  pthread_mutex_lock(&lock);
  //once it gets control it creates a filepath to currentTime.txt
  char formattedTime[DATASIZE] = "";
  char filepath[DATASIZE] = "./";
  strcat(filepath, "currentTime.txt");
  //Here it creates the time struc and variables
  time_t rawTimeOutput;
  struct tm *tmp;
  //grabbing the raw time
  rawTimeOutput = time(NULL);
  tmp = localtime(&rawTimeOutput);
  //one of the only parts of my code with error catching
  //thanks to the manual page that showed how to do this
  if(tmp == NULL){
  perror("problem gettin time");
  exit(99);
  }
  //this formats time to the required format and prints to stderr if something is wrong
  if(strftime(formattedTime, sizeof(formattedTime), "%l:%M%p, %A, %B%e, %Y", tmp) == 0)
  {
    fprintf(stderr, "strftime failed");
    exit(99);
  }

  //now creating file in append mode, writing to file, and closing file
  FILE * fPtr;
  fPtr = fopen(filepath, "w");
  fputs(formattedTime, fPtr);
  fclose(fPtr);
  
  //This signals to gamethread that control is about to become available
  pthread_cond_signal( &cond1);
  //then releases control
  pthread_mutex_unlock(&lock);
  //then sleeps before returning to its never ending loop
  sleep(1);
  }
}


//This was in my Main() until threaded was introduced so basically its important
void* gameFunc(void *arg){
  //Gets mutex CONTROl
  pthread_mutex_lock(&lock);
  
  //Initializing a load of variables
  int i =0;
  //This is finding the newest directory
  char* dirnamePTR;
  dirnamePTR = FindDirectory();
  //this turns it into a filepath
  char* dirnameFP;
  dirnameFP = MakeFilePath(dirnamePTR);
  //creating our connection array with a +1 to actual possible connections
  //this is because i use the NULL as a way to tell when to place a . instead of ,
  //so i needed an extra address to just store NULL
  char* connection[7];
  int connectioncount = 0;
  char* currentRoom;
  //THis sets the currentRoom to the startroom
  currentRoom = FindStart(dirnameFP, connection);

  //COPIED FROM CHANGE ROOM 
  //There should be an explanation of whats happeneing here in changeroom
  //but simply its printing out the connections in a nice format
  for(i=0; i < 7; i++)
  {
    connection[i] = NULL;
  }
  //This is directly from ChangeRoom because yeah. Its not pretty. but this entire program is
  //poorly thoughtout so why not continue 
  connectioncount = FindConnections(currentRoom,dirnameFP,connection);
  printf("POSSILBE CONNECTIONS:");
  for(i=0; i<connectioncount; i++)
  {
    if(connection[i+1] != NULL)
    printf(" %s,",connection[i]);
    else
    printf(" %s.\n",connection[i]);
  }

  //Now the user  has infront of them their current location
  //and a list of connection
  char answer[DATASIZE];
  int stepcount=0;
  //Im hoping it takes less then 100 steps for the user to find the exit
  //If it doesnt this will crash
  char path[100][DATASIZE];
  int okinput = 0;
  int atend = 0;

 while(1 == 1)
  {
    //the loop is inifinite so i have it break if the end was reached last loop
    if(atend == 1)
    break;
    
    //This is an input checking loop
    while(okinput == 0)
     {
       //Prompts user
       printf("WHERE TO? >");
       scanf("%s", answer);
       
       //If the user entered time interesting things happen
       if(strcmp(answer, "time")==0)
       {
         //the gamethread will now wait until it is signaled
         pthread_cond_wait( &cond1, &lock);
         //then unlocks the mutex
         pthread_mutex_unlock(&lock);
         //once it gets control back it locks again
         pthread_mutex_lock(&lock);
         //prints the time the other thread found
         PrintTime();
         //and reposts the users options
         ChangeRoom(currentRoom, dirnameFP, connection);
       }

        //This loop is messy like all my code huh huh huh huh :'(
       for(i=0; i <= connectioncount; i++)
       {
         //since the for loop will go though as many times as there are connections
         //i first ask if answer is "time" then immediatley kill the loop if it is
         if(strcmp(answer, "time")==0)
         {
           //keeping okinput as 0 here since if its 1 it will mess shnap up
         okinput = 0;
         break;
         }
         //if the loop gets through all connections and didnt find a match
         //it will prompt the user to do better and repost the info for them
         if(i == connectioncount)
         {
           printf("\nHEY! PLEASE ENTER AN OPTION I GAVE YOU, YOURE TRYING TO WALK THROUGH WALLS!\n");
           ChangeRoom(currentRoom, dirnameFP, connection);
           okinput = 0;
           break;
         }
         //if the loop finds a match it sets okinput to 1 and gos on truckin
         else if(strcmp(answer, connection[i]) == 0)
         {
           okinput=1;
           break;
         }
       }
    }
     
     //Once the loop has finished it checks to see if the answer was the end
    
    if(EndRoomCheck(answer, dirnameFP) == 1)
    {
      //if so it adds the final room to the path and step counter
      strcpy(path[stepcount], answer);
      stepcount++;
      //then prints the step count and path
      printf("\nYOU HAVE FOUND THE END ROOM. CONGRATULATIONS!");
      printf("\nYOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", stepcount);
      for(i=0; i<stepcount; i++)
      {
        printf("%s\n", path[i]);
      }
      //then we set atend to 1 so it kills our loop
      atend=1;
    }
    //if the input isnt the end 
    if(okinput == 1 && atend == 0)
    {
      //it will add to path, add to step, get new connections, and change
      //the current room incase the user messes up more
      strcpy(path[stepcount], answer);
      stepcount++;
      okinput=0;
      connectioncount = ChangeRoom(answer, dirnameFP, connection);
      strcpy(currentRoom, answer);
    }
  }
}


//Threads make for a pretty main
int main()
{
  //Create the gamefunction thread
  pthread_create(&(threadID[0]), NULL, &gameFunc, NULL);
  //waiting to allow game thread to take control
  sleep(1);
  //creates timefunction thread
  pthread_create(&(threadID[1]), NULL, &timeFunc, NULL);
  //waiting for gamefunction to end
  pthread_join(threadID[0], NULL);
  //ending program
  return 0;
}

//VERY RIGID FUNCTION BAD CODING PRACTICE
//also it works so bite me
void PrintTime()
{
  //This is NOT DYNAMIC. Ideally it would have a file name
  //other then this hardcoded currentTime
  //Opens the file named current time, since it is called after
  //the game thread gets control back it doesnt even check to see if the file
  // exists so really just terrible all around
  char getTime[DATASIZE];
  FILE * fptr;
  fptr = fopen("currentTime.txt", "r");
  fgets(getTime, DATASIZE, fptr);
  printf("\n%s\n", getTime);
}


//Returns the name of the biggest directory in the parent directory
//CAUTION it does not return a filepath, there are no backslashes in the return
char* FindDirectory()
{
  //calling all variables
  char* dirname;
  char* greatest;
  int greatesttime =0;
  struct dirent *dirselect;
  struct stat current;
  int time;
  //so these tests will return the most recently modified directory in the parent directory.
  DIR *parentdir = opendir(".");
  //It starts with asking if the parent directory exists, which if it doesnt is concerning
  if (parentdir == NULL)
  {
    printf("problems ranger");
  }
  //this is the real core, it opens the parent dir and reads through till there isnt any more
   while((dirselect = readdir(parentdir)) != NULL)
   {
     //then during this it grabs each file actually and pulls stat on it.
     dirname = dirselect->d_name;
     stat(dirname, &current);
     //then this if statment checks if its a directory and that it isnt the parent directory
     if(S_ISDIR(current.st_mode) == 1 && strcmp(dirname, ".") != 0)
     {
       //if it isnt it pulls the time on the new directory
      time = current.st_mtime;
      //then if time is greater then the currently selected biggest dir it will change it
      if(time > greatesttime)
      {
        greatesttime = time;
        greatest = dirname;
      }  
     }
    }
    //at the end we have the biggest dir, we return that and close our parentd
  closedir(parentdir);
  return greatest;
}

//Returns a formatted filepath staring with ./ and ending with the pasted dirname
//does not end with a /
char* MakeFilePath(char* dirname)
{
  //This really just adds two things to collected dirname and returns it
 static char filepath[DATASIZE] = "./";
 char addslash[2] = "/";
 strcat(filepath, dirname);
 return filepath;
}

char* MakeRoomFilePath(char* roomname, char* dirname)
{
  //This just takes a room name and makes it a filepath to that room
  //All my functions could have started with something similar to this
  //they do, but i didnt make functions for them so sorry
 static char filepath[DATASIZE] = "./";
 char addslash[2] = "/";
 strcat(filepath, dirname);
 strcat(filepath,addslash);
 strcat(filepath,roomname);
 return filepath;
}

int FindConnections(char* curroom, char* roomdir, char* connection[])
{ 
  //Again starts with the basic filepath creation all mine do
  //If i cared this should have been its own function
 int connectioncount=0;
 char FCtarget[DATASIZE] = "CONNECTION:";
 char addslash[2] = "/";
  char filepath[DATASIZE] = "";
 char roomname[6][DATASIZE];
 static char filelines[DATASIZE] = "";
 char typecheck[DATASIZE] = "";
 strcat(filepath,roomdir);
 strcat(filepath, addslash);
 strcat(filepath, curroom);
 FILE * fptr;
 fptr = fopen(filepath, "r");

//this is the meat of the function
     while(fgets(filelines, DATASIZE, fptr) !=NULL)
     {
       //It goes through the given file and if CONNECTION exists in the line
       //Then it continues through the if statment
       if(strstr(filelines, FCtarget) != NULL)
       {
         //Once in the if statemnnt it sends the line off to chopping 
         ConnectionCutting(filelines, FCtarget);
         //Then it brings back the file and copies it into a setaside 
         //string array just for this function
         //the reason is because memory is hard and i dont understand it
         strcpy(roomname[connectioncount], filelines);
         connection[connectioncount] = roomname[connectioncount];
         connectioncount++;
       }   
     }
 fclose(fptr);
 return connectioncount;
}

//given a line from a file returns only the string of the connected room
void ConnectionCutting(char* connectionline, char* CCtarget)
{
  //this little bit of code takes an entire line and gets the length
  int length = strlen(CCtarget);
  char *targetaquired;
  //then it looks for the passed target which in this case is
  //"CONNECTION:" it then puts the null character at the end of that and returns 
  // the next string after its placed null character
  while((targetaquired= strstr(connectionline, CCtarget)))
  {
    *targetaquired = '\0';
    strcat(connectionline, targetaquired+length+1);
    //this removes the newline cause heckin dont want that
    connectionline = strtok(connectionline, "\n");
  }
}


//should be given roomdir and find the file name with START_ROOM tag
char* FindStart(char* roomdirFP, char* connection[])
{
 char FStarget[DATASIZE] = "ROOM TYPE: START_ROOM";
 DIR *roomdir = opendir(roomdirFP);
 FILE *fptr;
 struct stat currentfile;
 char addslash[2] = "/";
 static char startroom[DATASIZE] = "";
 char filepath[DATASIZE];
 struct dirent *selectedfile;
//This loop will read through a given directory and open every file
//In each file it will read everyline and if the line contains
//the Target strings (ROOM TYPE: START_ROOM) then it will continue with setup
//for the user
 while((selectedfile = readdir(roomdir)) != NULL)
 {
   //This ugly block exists multiple other places in my program, it is creating a filepath
   //this shouldve been its own function
   char filepath[DATASIZE] = ""; char FSfilelines[DATASIZE] = ""; char typecheck[DATASIZE] = "";
   strcat(filepath,roomdirFP); strcat(filepath,addslash); strcat(filepath,selectedfile->d_name);
   stat(filepath, &currentfile);

   //At this point the filepath is made and weve created a stat struct to that file
   //the next part makes sure its trying to openeing a file
   if(S_ISREG(currentfile.st_mode) == 1)
   {
     //then we open the file in read
     fptr = fopen(filepath, "r");
     //get everysingle line
     while(fgets(FSfilelines, DATASIZE, fptr) !=NULL)
     {
       sprintf(typecheck, "%s", FSfilelines);
       //if we find our target in the line we want we will
       //set the startroom to that file and also print the first line
       //of code the program prints
       if(strcmp(typecheck, FStarget) == 0)
       {
        strcpy(startroom, selectedfile->d_name);
        printf("\nCURRENT LOCATION: %s\n", selectedfile->d_name);
       }
     }
   }
 }
  //closes all opened files and dirs then returns the name of startroom, which
  //actually doesnt get used.
 fclose(fptr);
 closedir(roomdir);
 return startroom;
}

//switches rooms and prints out possible connections formatted well
int ChangeRoom(char* newroom, char* roomdir, char* connection[])
{
  //This sets all elements of connection to NULL so my loop later will work
  int i;
  int connectioncount = 0;
  for(i=0; i < 6; i++)
  {
    connection[i] = NULL;
  }
  //Prints the locaiton for the user, added newline for easier UI
  printf("\nCURRENT LOCATION: %s\n", newroom);
  //Finds the connections to this new rooms and also graps the count
  connectioncount = FindConnections(newroom,roomdir,connection);
  printf("POSSILBE CONNECTIONS:");
  //this will print the possible connections in proper format to the user
  for(i=0; i<connectioncount; i++)
  {
    //if the next element is null its the end therefor we print the . instead of , and also newline
    if(connection[i+1] != NULL)
    printf(" %s,",connection[i]);
    else
    printf(" %s.\n",connection[i]);
  }
  //i return connection count because its flying everywhere. if you gotta change this
  //it could break the program. see shes very fragile.
  return connectioncount;
}

//should check if current room is end room
int EndRoomCheck(char* curroom, char* roomdir)
{ 
  //This first block is my typical filepath creation nonesense
 int isEND=0;
 char target[DATASIZE] = "ROOM TYPE: END_ROOM";
 char addslash[2] = "/";
 char filepath[DATASIZE] = "";
 char filelines[DATASIZE] = "";
 char typecheck[DATASIZE] = "";
 strcat(filepath,roomdir);
 strcat(filepath, addslash);
 strcat(filepath, curroom);

 //this is the first interesting part of the code
 FILE * fptr;
 fptr = fopen(filepath, "r");
     while(fgets(filelines, DATASIZE, fptr) !=NULL)
     {
       //This actually is similar to other roomcheck functions
       //It is just trying to find a line that matches the Target
       //Which is "ROOM TYPE: END_ROOM"
       //if it does it returns 1 if it doesnt it returns 0
       sprintf(typecheck, "%s", filelines);
       if(strcmp(typecheck, target) == 0)
       isEND=1;
     }
     //never leave a file open
 fclose(fptr);
 return isEND;
}