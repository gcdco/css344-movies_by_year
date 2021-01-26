/*
    Name: George Duensing
    Email: duensing@oregonstate.edu
    Course: cs344 Operating Systems
    Homework 2: movies_by_year
*/
// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE
//
// ref:
// repl: 3_5 stat from explorations
// ref: 
//  https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>  // Directories
#include <fcntl.h>  // Files
#include "movies.h"


#define PREFIX "movies_"  
#define FILEEXT ".csv"
#define DIR_PREFIX "duensing.movies."



// Menu
void file_menu();
void print_processing_file(char*);
void process_file_name(char*);
void file_not_found_msg();


// File/ Folder Processing
int find_largest_file(char*);
struct dirent* get_entry(DIR*);
void get_stat(struct dirent*, struct stat*);
int file_by_name(char *entryName);
int find_smallest_file(char*);

int create_directory(char*);
int create_file(char* dirName, char* fileName);
void close_file(int);
void write_to_file(int fileDescriptor, char* title);
void write_movieList_to_file(struct linkedList*, char*, int);

// Random Number
int random_number();

// Function Pointer


/*   Compile the program as follows:7
*       gcc --std=gnu99 -o movies main.c
*/
int main(int argc, char* argv[])
{
    // Set the seed to current time
    srand(time(0));
    
    /* -------------- DEBUG PLAYGROUND -------------- */

    // char fileName[] = "movies_zero.csv";
    // struct movie* list = processFile(fileName);
    // //print_movie_list(list);

    // struct movie* head;
    // head = list->next;
    
    // struct linkedList* newList = linkedListCreate();

    // while (head != NULL)
    // {
    //     printf("Adding: %s\n", head->title);
    //     linkedListAddBack(newList, head);
    //     head = head->next;
    // }
    // printf(" ----------------------------- ");
    // printf(" -------- Printing LL -------- ");
    // printf(" ----------------------------- \n");
    // getchar(); // pause
    // linkedListPrint(newList);
    // linkedListDestroy(newList);

    // destroy_list(list);


    /* -------------- END DEBUG PLAYGROUND -------------- */

    // Create Linked List of structs
    //struct movie *list = processFile(argv[1]);
    
    // Give user choices
    // And Print movie data
    int choice = 0;
    // For choice 3
    char lang[21];
    while (choice != 2) {
        

        //printf("\n");
        printf("1. Select file to process\n");
        printf("2. Exit the program\n");
        printf("\nEnter a choice from 1 or 2: ");
        scanf("%d", &choice);
        printf("\n");

        switch (choice) {
        case 1:
            printf("You selected choice #1.\n\n");
            file_menu();
            break;
        case 2:
            printf("Goodbye\n");
            break;
        default:
            printf("You entered an incorrect choice.Try again.\n\n");
        }
    }

    // Free the movie LL structure allocated
    // destroy_list(list);

    return EXIT_SUCCESS;
}

/************************************************
*   File Selection
*
*************************************************/

struct dirent* get_entry(DIR* curDir)
{
    struct dir* entry = readdir(curDir);
    return entry;
}

void get_stat(struct dirent* entry, struct stat *directory_stat)
{
    stat(entry->d_name, &directory_stat);
}

// Find the largest file w/ PREFIX and update fileName
// Return 0 if not found and 1 if found
//
int find_largest_file(char* fileName)
{
    int fileFound = 0;
    int fileSize = 0;
    //char ext[5] = ".csv";
    
    //char entryName[256];

    struct stat dirStat;
    struct dirent* ent;
    
    DIR* currDir; //vsprojects/movies/
    
    if ((currDir = opendir("./")) != NULL) {
        // Print the files in currDir
        while ((ent = get_entry(currDir)) != NULL) { //while ((ent = readdir(currDir)) != NULL)
            //printf("%s\n",ent->d_name);
            if(strncmp(PREFIX, ent->d_name, strlen(PREFIX)) == 0 ){
                char* found;
                if(found = strstr(ent->d_name, ".csv") != NULL){
                    stat(ent->d_name, &dirStat);//get_stat(ent, &dirStat);
                    // Function pointer:
                    // int (*cmp_file_sizeGreater)(struct stat, int)
                    if(dirStat.st_size > fileSize) 
                    {
                        fileSize = dirStat.st_size;
                        memset(fileName,'\0', sizeof(fileName));
                        strcpy(fileName, ent->d_name);
                    }
                    fileFound =1;
                }
                
            }
        }
        //printf("%s | %d\n", fileName, fileSize);
        closedir(currDir);
    }
    else {
        // Couldn't open directory
        perror("");
        return EXIT_FAILURE;
    }
    return fileFound;
}

int find_smallest_file(char* fileName)
{
    int fileFound = 0;
    // Set to arbitrarily large value
    int fileSize = 214748364;
    //char entryName[256];

    struct stat dirStat;
    struct dirent* ent;
    
    DIR* currDir; //vsprojects/movies/
    
    if ((currDir = opendir("./")) != NULL) {
        // Print the files in currDir
        while ((ent = get_entry(currDir)) != NULL) { //while ((ent = readdir(currDir)) != NULL)
            //printf("%s\n",ent->d_name);
            if(strncmp(PREFIX, ent->d_name, strlen(PREFIX)) == 0 ){
                char* found;
                if(found = strstr(ent->d_name, ".csv") != NULL){
                    stat(ent->d_name, &dirStat);//get_stat(ent, &dirStat);
                    // Function pointer:
                    // int (*cmp_file_sizeGreater)(struct stat, int)
                    if(dirStat.st_size < fileSize) 
                    {
                        fileSize = dirStat.st_size;
                        memset(fileName,'\0', sizeof(fileName));
                        strcpy(fileName, ent->d_name);
                    }
                }
            fileFound = 1;    
            }
        }
        closedir(currDir);
    }
    else {
        // Couldn't open directory
        perror("");
        return EXIT_FAILURE;
    }
    return fileFound;
}

int file_by_name(char *entryName)
{
    //char entryName[256];
    int foundFile = 0;
    struct dirent* ent;
    
    DIR* currDir; //vsprojects/movies/
    
    if ((currDir = opendir("./")) != NULL) {
        // Print the files in currDir
        while ((ent = get_entry(currDir)) != NULL) { //while ((ent = readdir(currDir)) != NULL)
            //printf("%s\n",ent->d_name);
            if(strncmp(entryName, ent->d_name, strlen(entryName)) == 0 ){
                foundFile = 1;
            }
        }
        
        closedir(currDir);
    }
    else {
        // Couldn't open directory
        perror("");
        return EXIT_FAILURE;
    }
    return foundFile;
}


/************************************************
*   menu
*
*************************************************/

void file_menu()
{
    int choice = 0;
    char fileName[256];
    while (choice != 4) {

        printf("Which file do you want to process?\n");
        printf("1. Pick the largest file\n");
        printf("2. Pick the smallest file\n");
        printf("3. Specify the name of a file\n");
        printf("4. Debug Option: Quit\n");
        printf("\nEnter a choice from 1 or 3: ");
        scanf("%d", &choice);
        printf("\n");

        switch (choice) {
        case 1:
            printf("You selected choice #1.\n\n");
            if(find_largest_file(fileName) == 1) {
                process_file_name(fileName);
                
            } else { file_not_found_msg(); }            
            break;
        case 2:
            printf("You selected choice #2.\n\n");
            if(find_smallest_file(fileName) == 1){
                process_file_name(fileName);
            } else { file_not_found_msg(); }
            break;
        case 3:
            printf("Enter a name of a file: ");
            scanf("%s", fileName);
            if(file_by_name(fileName) == 1)
            {
                process_file_name(fileName);
            } else { file_not_found_msg(); }
            break;
        case 4:
            printf("Goodbye\n");
            choice = 4;
            break;
        default:
            printf("You entered an incorrect choice.Try again.\n\n");
        }
        // struct movie *list = processFile(fileName);
        // destroy_list(list);
    }
}


/************************************************
*   Process file name
*   Main
*
*************************************************/

// Main function to process the fileName into an LL / 
// create directories and files per year in fileName.csv
//
void process_file_name(char* fileName)
{
    // Print the message requirement stating which file we are processing
    print_processing_file(fileName);    

    // Create Directory & print name of directory (onid.movies.random#)
    char *dirName[256];
    if(create_directory(dirName) == 1) {
        printf("Created directory with name %s\n", dirName);
    } else {
        printf("ERROR: could not create directory.\n");
    }

    // List to hold the movies from the user specified file
    struct movie* list = processFile(fileName);
    // Lets not destroy the LL structure
    struct movie* head;
    // Loop through the given inclusive year range
    for (int year = 1900; year <= 2021; year++) {
        // List to hold movies in a given year
        struct linkedList* yearList;
        head = list;
        while (head != NULL) {
            if (head->year == year) {
                // Create a list for the year @ 1st instance of year
                if(yearList == NULL) {
                    yearList = linkedListCreate();
                }
                // Add the movie to the list
                linkedListAddBack(yearList, head);                
            }
            head = head->next;
        }
        if(yearList != NULL){
            write_movieList_to_file(yearList, dirName, year);
            linkedListDestroy(yearList);
        }
        
    }
    // Free memory
    destroy_list(list);
}

// Write the movies to a file
void write_movieList_to_file(struct linkedList* list, char* dirName, int year)
{
    // Convert int year to string
    char st_year[5];
    sprintf(st_year,"%d", year);
    // Create a file (2020.txt). Permissions: rw-r-----
    int fd;
    fd = create_file(dirName, st_year);
    // Start writing movies to the file
    struct movieLink* head = list->frontSentinel->next; 
    while (head != list->backSentinel)
    {
        // Write data to the file
        write_to_file(fd, head->title);
        head = head->next;
    }

    // Close the file
    close_file(fd);
}


// Print the name of the file to be processed
void print_processing_file(char *fileName)
{
    printf("Now processing the chosen file named %s\n", fileName);
}

void file_not_found_msg()
{
    printf("You entered an incorrect choice.Try again.\n\n");
}


/************************************************
*   Directories
*
*************************************************/
// Create a directory and return 1 if successful and 0 if not
// param: char* which is the name of the created directory
// permissions for directory: rwx r-x ---  ==> 0750
//
int create_directory(char* dirName)
{
    // Make the directory name
    sprintf(dirName, "%s%d", DIR_PREFIX,random_number());
    if(mkdir(dirName, 0750) == 0) {
        return 1;
        //printf("Created directory with name %s\n", dirName);
    } else {
        return 0;
        //printf("ERROR: could not create directory.\n");
    }
}


/************************************************
*   Files
*   ref: 3_5_open_close.c from canvas
*   Permissions: rw-r----- (0640)
*************************************************/

int create_file(char* dirName, char* fileName)
{
    int fileDescriptor;
    // Create path
    char filePath_name[256];
    sprintf(filePath_name, "%s/%s.txt", dirName, fileName);
    fileDescriptor = open(filePath_name, O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0640);
    return fileDescriptor;
}

// close a file base on the file's file descriptor
void close_file(int fileDescriptor)
{
    printf("inside close file\n");
    close(fileDescriptor);
}

void write_to_file(int fileDescriptor, char* title)
{
    write(fileDescriptor, title, strlen(title));
}

/********************************************************************************
*   Random Number
*   ref: https://www.geeksforgeeks.org/generating-random-number-range-c/
*********************************************************************************/

int random_number()
{
    int randNumber;

    randNumber = (rand() % (99999 - 0 + 1) + 0);

    return randNumber;
}