# Adventure!(Design)

## DSL:

### Vocab:
PID - process ID

room type - the value of a room ie start_room or end_room

room name - name of a room, 10 possible options

### Format:
all function types will be used in camelCase ie createDirectory

structs will be shown as name_x

ROOM NAME : XYZZY 

CONNECTION 1 : PLOVER 

CONNECTION 2 : Dungeon 

CONNECTION 3 : twisty 

ROOM TYPE : START_ROOM

## Room Builder:
### Description:
This program will create a series of files that hold descriptions
of game rooms.

### Requirements:
FILE GENERATION:

-create a directory to contain the 7 files named " YOUR STUDENT ONID USERNAME.rooms.PROCESS ID OF ROOMS PROGRAM
> How to get the process ID? - You can get a process id by calling _getpid(get process id), make sure to include the unistd.h head file

> How to create a directory/folder? - Use call to _mkdir(make directory), make sure there is no current directory and to include header files unistd.h, <sys/types.h> and <sys/stat.h>

-This program must generate 7 different room files 

-There must be a list of at least 10 different possible room names for the computer to choose from.

>change to shuffle algorithm
>How to take a unique 7 random files for the array fileNames? - The file names will be represented as an array, the computer will pick any random number from 1 - 10 7 times, after a space in the array has been chose copy it to a different array of 7(fileNames) and change the value to NULL, if the computer picks a NULL value it will pick a new random number


OUTBOUND CONNECTIONS:

-There must be from 3 - 6 (also randomly decided) outbound connections to other rooms.

-Each outbound connection will be randomly assigned

-Outbound connections should be able to go forward and backward

-A room cannot have an outbound connection that points to itself.

-A room cannot have more than one outbound connection to the same room.

-A room cannot have a connection to itself.

ROOM TYPES:
-I will create a list of the right amount of room types and shuffle that list. Then I will take a type of the top of that list and assign it to a random room.

-Along with a name each room must have a value i.e END_ROOM, MID_ROOM or START_ROOM (only one start or end room)

-Each room type will be randomly assigned

-The connections will be represented with a boolean if a user picks a connection it will be marked true and not picked connections will be marked false(note: after the connection is picked all connections should be reset to NULL because certain rooms may be entered twice)

TYPES:

```C
//room value types
typedef enum _roomType_e{
    START_ROOM,
    MID_ROOM,
    END_ROOM
}   roomType_e

//representation of a room
typedef struct _room_t{
    char* name;
    roomType_e roomType;
    int connections[6];
//have each connection in a specific spot it the array.
//look for the connection directly and not sequentially. 
    size_t numOfConnections;
}   room_t

//array of file names


const char* fileNames[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"};     
const char* fileNameExtention = "txt";
const char* roomNames [] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"};
const roomType_e roomTypes [] = {START_ROOM, MID_ROOM, MID_ROOM, MID_ROOM, MID_ROOM, MID_ROOM, END_ROOM};
//May want to add static_asserts to insure room names and room types will be in sync
const room_t rooms [6];
```

FUNCTIONS:
createDirectory: createDirectory will check to see if there is a current directory, if there is it will return a value if not it will create a new directory using the mkdir call

`
bool createDirectory();
`

pickNames: pickNames will take the array of 10 possible room names and pick a random not repeating 7(more detail above)

`
void** pickNames(char*[] names, int count)
`

createRoom: createRoom will create 1 standard room(calling past functions) it will create a room with one of the 7 picked room names(each room name unique) and will assign a semi-random room value(one start_room one end_room, the rest middle rooms) this function will also generate this rooms connections to other rooms(3-6)

`
room_t* createRoom();
`

createRooms: createRooms will create 7 standard rooms using createRoom

`
room_t* [] createRooms();
`

### Format: 
-The game will you current location then a list of possible locations then a prompt for where to go:

-it will look something like this:
Current Location: ExampleRoom1
Possible Connections ExampleRoom2, ExampleRoom3, ExampleRoom4
Where to? >

-Each room name should only be one word

-If the user enters a room that is not on the possible connections list return: Huh? I don't understand that room! Try again.

-If/when the user reaches the end room return:
You have found the end room! Congratulations!
You took 2 steps. You path to victory was:
ExampleRoom1
ExampleRoom4

### Functions:
welcome: The welcome function will print a welcome to the player and the games name it will ask the user to begin or read the rules

`
bool welcome();
`

prompt: The prompt function will print out the current room and the possible connections and the prompt for which room to go to. It will also take the connection input and handle a bad input

`
bool prompt();
`

Winner: isWinner will check is the player is in the end room. If he/she is then print a win statement else do nothing

`
bool Winner(room_t);
`

replay: replay will be called if isWinner is true. It will prompt the user if he/she wants to play again and return true or false

`
bool replay();
`

pastRooms: This function will keep an array of rooms that the player has been into(not including the start room) when the game is over it will print this array.

`
room_t[] pastRooms(room_t);
`

steps: this function will create a variable count and increment it by one every time the play enters a room(the step counter goes up if the play goes into the same room twice) This function will be printed at the end of the game along with pastRooms

`
size_t steps();
`

## Files: 
1. use: FILE* fopen(const char *filename, const char *mode);
to open a specific file name that is associated with a certain room
- all parameters passed in are NULL pointers

2. use: size_t fwrite( const void *buffer, size_t size, size_t count, FILE *stream );
to write the correct room to the file 
- the buffer is a pointer to the first soon to be written element in the array
- size is how big each object is
- the count is the number of objects to be written
- stream is the point to the output stream

3. close the file with: int fclose( FILE *stream );
- the stream is the file stream to be closed

I will put the files in a for loop that will run NUM_FILES times
and increment/change each parameter as needed

for(int i = 0; i < NUM_FILES; i++){

FILE* fp = fopen(fileNames[i], "wb");

size_t r1 = fwrite(const void* buffer!, size_t size!, size_t count!, fp);

fclose(fp);```

## Game:

### Requirements: 

- The game will present a interface to the player
- The room data must be read back into the program(the room program may be ran multiple times so you need to select the most recent file.)
- to use the most recent files use the stat() function and use the returned st_mtime component to find the most recent files
- The interface should display where you currently are, your possible connections and a prompt:
```
CURRENT LOCATION: XYZZY 
POSSIBLE CONNECTIONS: PLOVER, Dungeon, twisty. 
WHERE TO? > 
```
- If the user types the exact name of a listed connection the program should print a updated interface with the new current location and possible connections
- If the user types anything but a valid room name(consider using toUpper) the program will output:
```
HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.
```
- Entering a incorrect location will not increment the path history or step count
- When the user reaches the end room the game should indicate it has been reached
- It will also print the path you have taken to get to this point(should not include the start room) and the number of steps taken (not the number of rooms visited)
- Then end screen should also return a congratulation screen then exit
- use the punctuation in the example (see page 6 and 7 on design reference)
- ignore time keeping for the time being

### Reading a file back in:
- create a var FILE*
- open a file using fopen() and set the file to that var
- check that the var != NULL
- use  fscanf function to read/write from the file(normally these functions are placed in a loop)
- [Reference for reading file back in](https://www.cs.utah.edu/~germain/PPS/Topics/C_Language/file_IO.html)

To read in the file:
- look for the text CURRENT LOCATION:
- add a char* name for the following text
- look for the text that reads CONNECTION...
- take the following connection and place it in a array(max 7 connections)
- repeat previous set until the text no longer reads CONECTION...
- take the line where the text reads ROOM TYPE: and add the following text to a char* room type
- use:
```
assert(fscanf != 0);
```
- non whitespace multi byte characters except %: each format character form the format string has exactly one identical character from the input stream.
- if any of the characters don't match it causes the function to fail(if the function fails you can move on to reading the next line of the file, if not then you can continue to read for the same characters)
- white space characters: each white space character consumes all available white space characters from the input.
https://en.cppreference.com/w/c/io/fscanf 

```
typedef struct{
    char* name;
    char* connections[MAX_CONNECTIONS];
    char* roomType;
}   room_t;
room_t rooms[MAX_ROOMS];
```

### Main game loop:
```
//Read files returns start_room
var currentRoom = readFiles(rooms);
var steps = 0;
char* path[MAX_PATH_LENGTH];
while(currentRoom.room_type != end_room){
    userInput = interface(currentRoom);
    isValidMove = validMove(currentRoom, userInput);
    if(isValidmove){
        currentRoom = userInput;
        path[steps] = currentRoom;
        steps++;
    }
    else{
        printf("HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.");
    }
 }
printf("You win!");
printf("Your Number of steps is: %d", steps);
printf("Your path is:");
for(int i = 0; i < MAX_PATH_LENGTH; i++){
    printf("%s", path[i]);
}
```

### Read Files

```

```

### isValidMove

```
signature: bool isVaildMove(currentRoom, userInput);

bool isValidRoom = false;
for(int = 0; i < MAX_CONNECTIONS; i++){
    if(user_input == currentRoom.connections[i]){
        isValidRoom = true;
        break;
    }
}
return isValidRoom;
```

### interface

```
signature: char* interface(currentRoom);

char* userInput;
printf("CURRENT LOCATION: %s", currentRoom.name);
for(int i = 0; i < MAX_CONNECTIONS; i++;){
    if(currentRoom.connections[i] == NULL){
        break;
    }

    else{
        printf("POSSIBLE CONNECTIONS:);
        printf("%s", currentRoom.connections[i]);
    }
}
printf("WHERE TO? > ");
scanf_s("%s", userInput);
return userInput;
```