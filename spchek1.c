#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h> // For open
#include <unistd.h> // For read, close
#include <ctype.h>
#define MAX_WORD_LENGTH 100
#define MAX_WORDS 150000

// Function to compare two strings for qsort and bsearch
int cmp(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

// Modified load_dictionary function to use read
char** load_dictionary(char* dict_path, int* dict_size) {
    char** dictionary = malloc(MAX_WORDS * sizeof(char*));
    int fd = open(dict_path, O_RDONLY);
    if (fd == -1) {
        printf("Could not open dictionary file %s\n", dict_path);
        exit(EXIT_FAILURE);
    }

    char buffer;
    char word[MAX_WORD_LENGTH];
    int word_index = 0;
    memset(word, 0, MAX_WORD_LENGTH);

    while (read(fd, &buffer, 1) > 0 && *dict_size < MAX_WORDS) {
        if (buffer == '\n' || buffer == ' ') {
            if (word_index > 0) {
                word[word_index] = '\0'; // Null-terminate the word
                dictionary[*dict_size] = malloc(strlen(word) + 1);
                strcpy(dictionary[*dict_size], word);
                (*dict_size)++;
                word_index = 0;
                memset(word, 0, MAX_WORD_LENGTH);
            }
        } else {
            if (word_index < MAX_WORD_LENGTH - 1) {
                word[word_index++] = buffer;
            }
        }
    }

    close(fd);
    return dictionary;
}

// Function to check if a word is in the dictionary using binary search
int find(char** dictionary, int dict_size, char* word) {
    return bsearch(&word, dictionary, dict_size, sizeof(char*), cmp) != NULL;
}

void process_file(char* filename, char** dictionary, int dict_size) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        printf("Could not open file %s\n", filename);
        return;
    }

    char buffer;
    char word[MAX_WORD_LENGTH];
    int word_index = 0;
    memset(word, 0, MAX_WORD_LENGTH);

    int line_number = 1;
    int column_number = 1;
    int start_column = 1;

    while (read(fd, &buffer, 1) > 0) {
        if (buffer == '\n') {
            line_number++;
            column_number = 1;
            continue;
        }

        if (isspace(buffer) || ispunct(buffer)) {
            if (word_index > 0) { 
                word[word_index] = '\0'; 

                char wordLower[MAX_WORD_LENGTH];
                char wordUpper[MAX_WORD_LENGTH];
                strcpy(wordLower, word);
                strcpy(wordUpper, word);

                // Convert word to lowercase for the comparison
                for (int i = 0; wordLower[i]; i++) {
                    wordLower[i] = tolower(wordLower[i]);
                }

                // Convert word to uppercase for the comparison
                for (int i = 0; wordUpper[i]; i++) {
                    wordUpper[i] = toupper(wordUpper[i]);
                }

                // Original check remains, add checks for lowercase and uppercase versions
                if (!find(dictionary, dict_size, word) &&
                    !find(dictionary, dict_size, wordLower) &&
                    !find(dictionary, dict_size, wordUpper)) {
                    printf("%s (%d,%d): %s\n", filename, line_number, start_column, word);
                }

                word_index = 0; 
                memset(word, 0, MAX_WORD_LENGTH); 
            }

            column_number++;
            start_column = column_number;
        } else {
            if (word_index == 0) { // Update start_column at the start of a word
                start_column = column_number;
            }
            if (word_index < MAX_WORD_LENGTH - 1) {
                word[word_index++] = buffer;
            }
            column_number++;
        } 
    }

    // Handle last word in file, if any
    if (word_index > 0) {
        word[word_index] = '\0'; 

        // Replicate the capitalization logic for the last word
        char wordLower[MAX_WORD_LENGTH];
        char wordUpper[MAX_WORD_LENGTH];
        strcpy(wordLower, word);
        strcpy(wordUpper, word);

        for (int i = 0; wordLower[i]; i++) {
            wordLower[i] = tolower(wordLower[i]);
        }

        for (int i = 0; wordUpper[i]; i++) {
            wordUpper[i] = toupper(wordUpper[i]);
        }

        if (!find(dictionary, dict_size, word) &&
            !find(dictionary, dict_size, wordLower) &&
            !find(dictionary, dict_size, wordUpper)) {
            printf("%s (%d,%d): %s\n", filename, line_number, start_column, word);
        }
    }

    close(fd);
}


// Function to process a directory
void process_directory(char* dirname, char** dictionary, int dict_size) {
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        printf("Could not open directory %s\n", dirname);
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        char path[PATH_MAX];
        snprintf(path, PATH_MAX, "%s/%s", dirname, entry->d_name);
        if (entry->d_type == DT_REG) {
            process_file(path, dictionary, dict_size);
        } else if (entry->d_type == DT_DIR) {
            if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                process_directory(path, dictionary, dict_size);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: %s <dictionary> <file/dir>...\n", argv[0]);
        return 1;
    }

    // Load the dictionary
    int dict_size = 0;
    char** dictionary = load_dictionary(argv[1], &dict_size);

    // Sort the dictionary for binary search
    qsort(dictionary, dict_size, sizeof(char*), cmp);

    // Process the files/directories
    struct stat s;
    for (int i = 2; i < argc; i++) {
        if (stat(argv[i], &s) == 0) {
            if (s.st_mode & S_IFDIR) {
                // it's a directory
                process_directory(argv[i], dictionary, dict_size);
            } else if (s.st_mode & S_IFREG) {
                // it's a file
                process_file(argv[i], dictionary, dict_size);
            } else {
                // something else
                printf("Not a file or directory: %s\n", argv[i]);
            }
        } else {
            // error
            printf("Error accessing: %s\n", argv[i]);
        }
    }

    return 0;
}
