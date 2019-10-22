/*this is a program in c*/
//I AM GOING TO USE FALSE=0 and TRUE=1 I HOPE THIS WORKS
// Create all connections in graph
int main() 
{
  int pid = getpid();
 //creating directory of rooms
 if (mkdir("Staubeav.rooms", pid) == -1)
   fprintf( stderr, "%s", "Error in file creation\n");
  else
   fprintf( stout, "%s", "File Created\n");
 //while (IsGraphFull() == 0)
 //{
   //AddRandomConnection();
 //}
}
// Returns true if all rooms have 3 to 6 outbound connections, false otherwise
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
<<<<<<< HEAD
=======
*/
>>>>>>> a2822c1246ff0abfe029d464d43a5d15211190de
