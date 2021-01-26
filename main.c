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
// repl: 3_4 read
// cs261 linked list structure
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

// We only want files with the PREFIX
#define PREFIX "movies_"  
// Make sure file doesn't conflict with PREFIX above
#define FILEEXT ".csv"
// For directory creation
#define DIR_PREFIX "duensing.movies."

// Menu/ processing
void file_menu();
void print_processing_file(char*);
void process_file_name(char*);
void file_not_found_msg();

// File/ Folder Processing, creation, and reading
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

/*   Compile the program as follows:7
*       gcc --std=gnu99 -o movies main.c
*/
int main(int argc, char* argv[])
{
    // Set the seed to current time
    srand(time(0));
    
    /* -------------- DEBUG PLAYGROUND -------------- */
    /* -------------- END DEBUG PLAYGROUND ---------- */
   
    // Holds the user choice
    int choice = 0;
    // For choice 3
    char lang[21];
    // Start main menu
    while (choice != 2) {
        printf("\n1. Select file to process\n");
        printf("2. Exit the program\n");
        printf("\nEnter a choice from 1 or 2: ");
        scanf("%d", &choice);
        printf("\n");

        // Control structure
        switch (choice) {
        case 1:
            file_menu();
            break;
        case 2:
            printf("Goodbye\n");
            break;
        default:
            printf("You entered an incorrect choice.Try again.\n\n");
        }
    }

    return EXIT_SUCCESS;
}

/************************************************
*   File Selection Functions
*
*************************************************/

// Return next entry in the open directory
// param: curDir: current open directory pointer 
// return: see above
//
struct dirent* get_entry(DIR* curDir)
{
    struct dir* entry = readdir(curDir);
    return entry;
}

// -- NOT USED --
// Get the info for the current entry
// param: entry: entry pointer to get info from
//        directory_stat: struct to hold information
void get_stat(struct dirent* entry, struct stat *directory_stat)
{
    stat(entry->d_name, &directory_stat);
}

// Find the largest file w/ PREFIX and update fileName
// Return 0 if not found and 1 if found
//
// param: fileName: pointer to the file name string to be updated
// return:   0 if unseccussful
//           1 if successful
//
int find_largest_file(char* fileName)
{
    // Return if fileFound
    int fileFound = 0;
    // Update w/ current file size, used for comparison and selection
    int fileSize = 0;

    struct stat dirStat;
    struct dirent* ent;
    
    DIR* currDir; 
    // Open the current directory to search through
    if ((currDir = opendir("./")) != NULL) {
        // Get each entry in the directory
        while ((ent = get_entry(currDir)) != NULL) { 
            // Check if file has correct prefix
            if(strncmp(PREFIX, ent->d_name, strlen(PREFIX)) == 0 ){
                // Needed since the executable is named w/ PREFIX found
                // We only want ".csv" files
                char* found;
                if(found = strstr(ent->d_name, ".csv") != NULL){
                    stat(ent->d_name, &dirStat);
                    // Not enough time to implement function pointer for comparison to reduce code length
                    // Compare current file w/ previous found file
                    if(dirStat.st_size > fileSize) 
                    {
                        // Update current largest file size info
                        fileSize = dirStat.st_size;
                        memset(fileName,'\0', sizeof(fileName));
                        strcpy(fileName, ent->d_name);
                    }
                    // File was found. Default is 0 (not found)
                    fileFound = 1;
                }
            }
        }
        // Close the directory we opened earlier
        closedir(currDir);
    }
    else {
        // Couldn't open directory
        perror("");
        return EXIT_FAILURE;
    }
    return fileFound;
}

// Find the smallest file w/ PREFIX and update fileName
// Return 0 if not found and 1 if found
//
// param: fileName: pointer to the file name string to be updated
// return:   0 if unseccussful
//           1 if successful
//
int find_smallest_file(char* fileName)
{
    // Return if fileFound
    int fileFound = 0;
    // Set to arbitrarily large value
    int fileSize = 214748364;
    
    struct stat dirStat;
    struct dirent* ent;
    
    DIR* currDir;
    
    if ((currDir = opendir("./")) != NULL) {
        // Get each entry in the directory
        while ((ent = get_entry(currDir)) != NULL) { 
            // Check if file has correct prefix
            if(strncmp(PREFIX, ent->d_name, strlen(PREFIX)) == 0 ){
                char* found;
                // Needed since the executable is named w/ PREFIX found
                // We only want ".csv" files
                if(found = strstr(ent->d_name, ".csv") != NULL){
                    stat(ent->d_name, &dirStat);
                    
                    if(dirStat.st_size < fileSize) 
                    {
                        // Update current smallest file size info
                        fileSize = dirStat.st_size;
                        memset(fileName,'\0', sizeof(fileName));
                        strcpy(fileName, ent->d_name);
                    }
                }
                // File was found. Default is 0 (not found)
                fileFound = 1;    
            }
        }
        // Close the directory we opened earlier
        closedir(currDir);
    }
    else {
        // Couldn't open directory
        perror("");
        return EXIT_FAILURE;
    }
    return fileFound;
}

// Find a file by a specific name entered by user
// param:  Name of file to look for
// return: 1 if file found, 0 if file not found 
//
int file_by_name(char *entryName)
{
    // To return, default not found
    int foundFile = 0;
    struct dirent* ent;
    
    DIR* currDir;
    // Open current directory
    if ((currDir = opendir("./")) != NULL) {
        // Go through each file in currDir
        while ((ent = get_entry(currDir)) != NULL) { 
            // Check if the file matches the user specified file
            if(strncmp(entryName, ent->d_name, strlen(entryName)) == 0 ){
                // Found
                foundFile = 1;
            }
        }
        // Close Directory
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
*   Menu - submenu
*
*************************************************/

// Control for submenu
// Get user specified file, process the file, and then return to main menu
// 
void file_menu()
{
    // User choice
    int choice = 0;
    // User fileName choice
    char fileName[256];
    while (choice != 4) {

        printf("Which file do you want to process?\n");
        printf("1. Pick the largest file\n");
        printf("2. Pick the smallest file\n");
        printf("3. Specify the name of a file\n");
        printf("\nEnter a choice from 1 or 3: ");
        scanf("%d", &choice);
        printf("\n");

        switch (choice) {
        case 1:
            // Check for the largest file
            if(find_largest_file(fileName) == 1) 
            {
                process_file_name(fileName);
                return; // return to main menu
            } else { file_not_found_msg(); }            
            break;
        case 2:
            // Check for smallest file
            if(find_smallest_file(fileName) == 1)
            {
                process_file_name(fileName);
                return; // return to main menu
            } else { file_not_found_msg(); }
            break;
        case 3:
            // Let user specify file
            printf("Enter a name of a file: ");
            scanf("%s", fileName);
            if(file_by_name(fileName) == 1)
            {
                process_file_name(fileName);
                return; // return to main menu
            } else { file_not_found_msg(); }
            break;
        default:
            printf("You entered an incorrect choice. Try again.\n\n");
        }
    }
}


/************************************************
*   Process File
*   
*************************************************/

// Main function to process the fileName into a LL / 
// create directories and files per year in fileName.csv
// param: fileName: name of file to process
//
void process_file_name(char* fileName)
{
    // Print the message requirement stating which file we are processing
    print_processing_file(fileName);    

    // Create Directory & print name of directory (onid.movies.random#)
    char *dirName[256];
    if(create_directory(dirName) == 1) {
        printf("Created directory with name %s\n\n", dirName);
    } else {
        printf("ERROR: could not create directory.\n");
    }

    // List to hold the movies from the user specified file
    // Using LL from hw1 for this
    struct movie* list = processFile(fileName);
    // Lets not destroy the LL structure
    struct movie* head;
    // Loop through the given inclusive year range
    for (int year = 1900; year <= 2021; year++) {
        // List to hold movies in a given year
        // New LL structure adapted from cs261
        struct linkedList* yearList = NULL;
        // Get head of LL structure
        head = list;
        // Loop through the list checking for movies in year
        while (head != NULL) {
            // Got one
            if (head->year == year) {
                // Create a list for the year @ 1st instance of year
                if(yearList == NULL) {
                    yearList = linkedListCreate();
                }
                // Add the movie to the list
                // Copy info from first LL structure (hw1) to the new LL structure
                linkedListAddBack(yearList, head);                
            }
            head = head->next;
        }
        // If we found a movie, and then created a list
        if(yearList != NULL){
            // Write info to the file
            write_movieList_to_file(yearList, dirName, year);
            // Free memory
            linkedListDestroy(yearList);
        }
        
    }
    // Free memory from hw1 LL structure
    destroy_list(list);
}

// Write the movies to a file
// params: list: LL structure containing movie info for a given year
//         dirName: directory to create the file in
//         year: current year we are processing
//
void write_movieList_to_file(struct linkedList* list, char* dirName, int year)
{
    // Convert int year to string
    char st_year[5];
    sprintf(st_year,"%d", year);
    // Create a file (2020.txt). Permissions: rw-r-----
    // file descriptor for subsequent file writing
    int fd;
    fd = create_file(dirName, st_year);
    
    // Start writing movies to the file
    // Loop through LL containing movies per year
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
// param: fileName: file we are processing. Rubric requirement.
//
void print_processing_file(char *fileName)
{
    printf("Now processing the chosen file named %s\n", fileName);
}

// Alert the user to bad choice
//
void file_not_found_msg()
{
    printf("You entered an incorrect choice.Try again.\n\n");
}


/************************************************
*   Directories
*
*************************************************/
// Create a directory and return 1 if successful and 0 if not
// permissions for directory: rwx r-x ---  ==> 0750 
// param: dirName: char* which is the name of the directory to create
// return: 0 if failed, and 1 if successful 
//
int create_directory(char* dirName)
{
    // Make the directory name
    sprintf(dirName, "%s%d", DIR_PREFIX,random_number());
    if(mkdir(dirName, 0750) == 0) {
        // File was successfully created
        return 1;
    } else {
        // File was not created successfully
        return 0;
    }
}


/************************************************
*   Files
*   ref: 3_5_open_close.c from canvas
*   Permissions: rw-r----- (0640)
*************************************************/
// Create a file name w/ given name in a given directory
// param: dirName: directory to make file in
//        fileName: name of file to create (year.txt)
// return: fileDescriptor to access file for writing subsequently
//
int create_file(char* dirName, char* fileName)
{
    int fileDescriptor;
    // Create path
    char filePath_name[256];
    sprintf(filePath_name, "%s/%s.txt", dirName, fileName);
    // Open the file, creating it if necessary. Append to end of file
    fileDescriptor = open(filePath_name, O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0640);
    return fileDescriptor;
}

// Close a file base on the file's file descriptor
// param: fileDescriptor: ID of file to close
//
void close_file(int fileDescriptor)
{
    close(fileDescriptor);
}

// Write entry to the file
// param: fileDescriptor: ID of file to write to
//        title: Name of movie to write in file
//
void write_to_file(int fileDescriptor, char* title)
{
    write(fileDescriptor, title, strlen(title));
    write(fileDescriptor, "\n", strlen("\n"));
}

/********************************************************************************
*   Random Number
*   ref: https://www.geeksforgeeks.org/generating-random-number-range-c/
*********************************************************************************/

// Create a random number in range. Seeded at start of main.c with current time.
// return: random number
//
int random_number()
{
    int randNumber;

    randNumber = (rand() % (99999 - 0 + 1) + 0);

    return randNumber;
}