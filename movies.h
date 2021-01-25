#ifndef MOVIES_HPP
#define MOVIES_HPP

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

struct linkedList;

// Holds the languages in a linked list structure that struct movie contains a pointer for
struct language
{
    // Individual language
    char* lang;
    struct language* next;
};

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


/****************************************************************************************************
* More general LL structure
*
****************************************************************************************************/

#ifndef TYPE
// Trying a pointer... Is this right? 12:30am
// This fixed the error's compiling
#define TYPE struct movie*  
#endif

struct linkedList* linkedListCreate();
// void linkedListDestroy(struct linkedList*);
void linkedListPrint(struct linkedList*);
int linkedListIsEmpty(struct linkedList*);
void linkedListAddBack(struct linkedList*, TYPE);
void linkedListRemoveFront(struct linkedList*);

/****************************************************************************************************
* Movie LL structure
*
****************************************************************************************************/

struct movie* processFile(char*); // Create the LL structure
struct movie* createMovie(char* mov_title, int year, char* languages, double rating); // Create a link
void destroy_list(struct movie*); // Free the memory for the LL structure
struct language* parse_language(char* languages); // make LL structure for languages
struct language* createLanguage(char*); // Make the acutal node and return it to parse_language
void print_movie_by_year(struct movie*, int); // Print the movies in a specific year
void print_movie_list(struct movie*); // Testing and debugging
void print_movie_by_language(struct movie*, char*); // Print movies in a specific language
void print_highest_rated(struct movie*); // Highest rated movie per year
char* parse_file_name(char*); // Get the ".csv" file name from the longer file path
void destroy_language_list(struct language*); // Free the memory allocated for the languages
void print_debug_movie(struct movie* thisMovie); // Print a single movie minus language
void print_debug_languages(struct movie*); // Print the languages to make sure it initialized correctly


#endif



