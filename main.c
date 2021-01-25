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
#include <dirent.h>
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

void create_directory();
void create_file();

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

    char fileName[] = "movies_zero.csv";
    struct movie* list = processFile(fileName);
    //print_movie_list(list);

    struct movie* head;
    head = list->next;
    
    struct linkedList* newList = linkedListCreate();

    while (head != NULL)
    {
        printf("Adding: %s\n", head->title);
        linkedListAddBack(newList, head);
        head = head->next;
    }
    printf(" ----------------------------- ");
    printf(" -------- Printing LL -------- ");
    printf(" ----------------------------- \n");
    linkedListPrint(newList);

    destroy_list(list);


    /* -------------- END DEBUG PLAYGROUND -------------- */

    // // Create Linked List of structs
    // //struct movie *list = processFile(argv[1]);
    
    // // Give user choices
    // // And Print movie data
    // int choice = 0;
    // // For choice 3
    // char lang[21];
    // while (choice != 2) {
        

    //     //printf("\n");
    //     printf("1. Select file to process\n");
    //     printf("2. Exit the program\n");
    //     printf("\nEnter a choice from 1 or 2: ");
    //     scanf("%d", &choice);
    //     printf("\n");

    //     switch (choice) {
    //     case 1:
    //         printf("You selected choice #1.\n\n");
    //         file_menu();
    //         break;
    //     case 2:
    //         printf("Goodbye\n");
    //         break;
    //     default:
    //         printf("You entered an incorrect choice.Try again.\n\n");
    //     }
    // }

    // Free the movie LL structure allocated
    //destroy_list(list);

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

    // Create Linked List of structs
    struct movie *list = processFile(fileName);
    // Debug:
    // print_movie_list(list);

    struct movie* head;
    head = list;
    while(head != NULL)
    {
        print_debug_movie(head);
        head = head->next;
    }


    // Free the memory
    destroy_list(list);
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
//
// permissions for directory: rwx r-x ---  ==> 0750
//
void create_directory()
{
    char *dirName[256];
    // Make the directory name
    sprintf(dirName, "%s%d", DIR_PREFIX,random_number());
    if(mkdir(dirName, 0750) == 0) {
        printf("Created directory with name %s\n", dirName);
    } else {
        printf("ERROR: could not create directory.\n");
    }

}


/************************************************
*   Files
*
*************************************************/

void create_file()
{
    printf("Inside create_file\n");
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