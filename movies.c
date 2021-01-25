#include "movies.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Holds the data about the movie
struct movie
{
    char* title;
    int year;
    // Print to .1 decimal place %.1f
    double rating;
    // Hold the head of the language structure
    struct language* language;
    struct movie* next;
};

struct language
{
    // Individual language
    char* lang;
    struct language* next;
};

struct linkedList
{
    struct movie* frontSentinel;
    struct movie* backSentinel;
    int size;
};

/************************************************************
*   HW2: Create LL structure for movies per year from user
*   selected file
*   - More general LL creation here
************************************************************/

static void init(struct linkedList* list)
{
    // Allocate memory for sentinels 
    list->frontSentinel = (struct movie*) malloc(sizeof(struct movie));
    assert(list->frontSentinel != NULL);
    list->backSentinel = (struct movie*) malloc(sizeof(struct movie));
    assert(list->backSentinel != NULL);
    // Hook up sentinels
    list->frontSentinel->next = list->backSentinel;
    list->backSentinel->next =  list->frontSentinel;
    
    // initialize start size
    list->size = 0;
}

// Allocate memory for a
struct linkedList* linkedListCreate()
{
    printf("inside linkedListCreate()");
    struct linkedList* list = malloc(sizeof(struct linkedList));
    init(list);
    return list;
}

// void linkedListDestroy(struct LinkedList* list)
// {
//     printf("\nInside linkedListDestroy\n");
// }

// void linkedListPrint(struct LinkedList* list)
// {
//     printf("\nInside linkedListPrint\n");
// }

int linkedListIsEmpty(struct linkedList* list)
{
    printf("\nInside linkedListIsEmpty\n");
    assert(list != NULL);
    // Is the LL empty?
    if(list->size == 0){ return 1;}
    else { return 0; }
}
//
//  link = backSentinel
//  newLink = new struct movie
//
static void addLinkBefore(struct linkedList* list, struct movie* link, TYPE newLink)
{
    assert(list != NULL);
    //struct movie* newLink = (struct movie*) malloc(sizeof(struct movie));
    //assert(newLink != NULL);
    assert(link != NULL);
    
    link->next->next = newLink;
    link->next = newLink;

    // increase size
    list->size++;
}

// Add a link to the back of the list
// create a movie and pass it here where TYPE is listed
//
void linkedListAddBack(struct linkedList* list, TYPE value)
{
    assert(list != NULL);
    addLinkBefore(list, list->backSentinel, value);
}



// void linkedListRemoveFront(struct LinkedList* list)
// {
//     printf("\nInside linkedListRemoveFront\n");
// }


/************************************************************
*   HW1: Make LL structure for movies in a .csv file from
*   the command line
************************************************************/

// Return a linked list of movies by parsing data from each line of the specified file.
// param: filePath - file passed in as a command line argument
//
struct movie* processFile(char* filePath)
{
    // Return a pointer to a file
    FILE* movieFile = fopen(filePath, "r");

    // Check the file opened correctly
    if (!movieFile) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    char* currLine = NULL;
    size_t len = 0;
    ssize_t nread;

    // The head of the linked list
    struct movie* head = NULL;
    // The tail of the linked list
    struct movie* tail = NULL;

    // Hold line from file
    char buf[400];
    char mov_title[100];
    char languages[200];
    int year;
    double rating;

    // Discard first line (header) of csv file
    fgets(buf, sizeof(buf), movieFile);

    // counter for movies
    int count = 0;
    // Loop through file line by line
    while (fgets(buf, sizeof(buf), movieFile))
    {
        // Get the data from the line that follows the pattern
        sscanf(buf, "%[^,],%d,%[^,],%lf\n", &mov_title, &year, &languages, &rating);
        // Create a link in the list
        struct movie* currNode = createMovie(&mov_title, year, &languages, rating);

        if (head == NULL) {
            head = currNode;
            tail = currNode;
        }
        else {
            tail->next = currNode;
            tail = currNode;
        }
        count++;
    }
    fclose(movieFile);
    // Get the "movies.csv" part from the filePath
    char* fileName = parse_file_name(filePath);
    printf("Processed file %s and parsed data for %d movies.\n", fileName, count);

    // Head of the LL
    return head;
}

// Create a link in the LL structure
// params: data for each movie
//
struct movie* createMovie(char* mov_title, int year, char* languages, double rating)
{
    // Allocate memory for the node
    struct movie* currMovie = malloc(sizeof(struct movie));

    // Allocate memory for the title
    currMovie->title = malloc((strlen(mov_title) + 1) * sizeof(char));
    //currMovie->title = malloc(110*sizeof(char));
    strcpy(currMovie->title, mov_title);

    currMovie->year = year;

    // Pass the [lang;..;lang] string to create the languages LL stucture. Returns the head of LL 
    currMovie->language = parse_language(languages);

    currMovie->rating = rating;

    currMovie->next = NULL;
    return currMovie;
}

// Free the memory for the LL structure
// param: head of LL movie structure
//
void destroy_list(struct movie* list) {
    // Point temp to the next linke so we don't lose it after freeing list 
    struct movie* temp;
    // Make sure there is another link to link to
    if (list->next != NULL) {
        temp = list->next;
    }

    while (list != NULL) {
        free(list->title);

        // Free the memory for the LL language structure inside struct movie
        destroy_language_list(list->language);

        // Free the link memory
        free(list);
        list = temp;
        if (temp != NULL) {
            temp = temp->next;
        }
    }
}

// Free the memory in the movie struct that holds the list of languages for the movie
// param: list - LL structure inside of the individual movie struct that holds the movies
//
void destroy_language_list(struct language* list) {
    // Point temp to the next linke so we don't lose it after freeing list 
    struct language* temp;
    // Make sure there is another link to link to
    if (list != NULL && list->next != NULL) {
        temp = list->next;
    }

    while (list != NULL) {
        // Free memory for the language
        free(list->lang);
        // Free the memory for the struct link
        free(list);
        list = temp;
        if (temp != NULL) {
            temp = temp->next;
        }
    }
}

// Get the ".csv" file name from the longer file path
// param: fpath - the /nfs/.../vsprojects/../movies.csv file path
// return pointer to parsed filename
//
char* parse_file_name(char* fpath) {
    // Save the place in the file where we left off
    char* saveptr;
    // Get the token. This will be NULL when the end of the string is reached
    char* p;
    // The token right before the final NULL to return
    char* returnP;
    // Initial call. Subsequent need NULL
    p = strtok_r(fpath, "/", &saveptr);
    // Loop until end of string
    while (p != NULL) {
        // Save the last useable token before NULL
        returnP = p;
        p = strtok_r(NULL, "/", &saveptr);
    }
    return returnP;
}

// Create the LL structure that holds the languages for the movie
// param: language string
// return: head of the LL structure
//
struct language* parse_language(char* languages) {
    // The head of the linked list
    struct language* head = NULL;
    // The tail of the linked list
    struct language* tail = NULL;

    // Set-up for strtok_r
    char* savePtr; // Save place in file
    char* token; // Indvidual language in string

    // Initialize strtok_r for multiple uses
    // Delimiters
    token = strtok_r(languages, "[;]", &savePtr);
    while (token != NULL) {
        // Don't send the address as it is a pointer
        // Create a node for the LL 
        struct language* currNode = createLanguage(token);

        if (head == NULL) {
            head = currNode;
            tail = currNode;
        }
        else {
            tail->next = currNode;
            tail = currNode;
        }
        // Get next. Or NULL if finished
        token = strtok_r(NULL, "[;]", &savePtr);
    }
    return head;
}

// Create an individual node in the LL
// param: token - language
// return: link to individual language
//
struct language* createLanguage(char* token) {
    // Make a new struct to hold the languages
    struct language* currLanguage = malloc(sizeof(struct language));
    //currLanguage->lang = malloc(21*sizeof(char));
    currLanguage->lang = malloc((strlen(token) + 1) * sizeof(char));
    strcpy(currLanguage->lang, token);
    currLanguage->next = NULL;
    return currLanguage;
}

// Print movie released in the specified year
// param: year - print movies in this year only
//
void print_movie_by_year(struct movie* list, int year) {
    // Keep track of whether any movies were released that year
    int count = 0;
    while (list != NULL) {
        if (list->year == year) {
            printf("%d | %s\n", list->year, list->title);
            count++;
        }
        list = list->next;
    }
    if (count == 0) {
        printf("No data about movies released in the year %d", year);
    }
}


// Print movies for a specified language
// param: lang - language to search for and print movies for
//
void print_movie_by_language(struct movie* list, char* lang) {
    // Don't want to destroy the LL structure
    struct movie* head = list;
    // Keep track of whether any movies were found
    int count = 0;
    while (head != NULL) {
        // Don't want to destroy the LL structure
        struct language* currLanguage = head->language;
        while (currLanguage != NULL) {
            if ((strcmp(currLanguage->lang, lang)) == 0) {
                printf("%d %s\n", head->year, head->title);
                count++;
            }
            currLanguage = currLanguage->next;
        }
        head = head->next;
    }
    if (count == 0) {
        printf("No data about movies released in %s\n", lang);
    }
}

// Print the highest rated for each year
//
void print_highest_rated(struct movie* list) {
    // Lets not destroy the LL structure
    struct movie* head;

    // Loop through the given inclusive year range
    for (int year = 1900; year <= 2021; year++) {
        char currTitle[200];
        double currRating = 0.0;
        head = list;
        while (head != NULL) {
            if (head->year == year) {
                if (head->rating > currRating) {
                    strcpy(currTitle, head->title);
                    currRating = head->rating;
                }
            }
            head = head->next;
        }
        // If there was a movie for the year, print it.
        if (currRating != 0.0) {
            printf("%d %.1lf %s\n", year, currRating, currTitle);
        }
    }
}

// Testing and debugging - Print the list
//
void print_movie_list(struct movie* list) {
    while (list != NULL) {
        print_debug_movie(list);
        list = list->next;
    }
}

// Print a single movie minus the language. Debugging
// param: thisMovie - movie to print
//
void print_debug_movie(struct movie* thisMovie) {
    printf("%s | ", thisMovie->title);
    printf("%d | ", thisMovie->year);
    // Print double to single decimal place
    printf("%.1lf\n", thisMovie->rating);
}


// Print the languages to make sure it initialized correctly
// Debuging function
//
void print_debug_languages(struct movie* list) {
    // Don't want to destroy the LL structure
    struct movie* head = list;
    while (head != NULL) {
        // Don't want to destroy the LL structure
        struct language* head_language = head->language;
        print_debug_movie(head);

        while (head_language != NULL) {
            printf("=> %s ", head_language->lang);
            head_language = head_language->next;
        }
        printf("\n-----------\n");

        head = head->next;
    }
}