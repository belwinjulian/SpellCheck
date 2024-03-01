#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>
#define MAX_WORD_LENGTH 100
#define MAX_WORDS 105000

// Function to compare two strings for qsort and bsearch
int cmp(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

// Function to load the dictionary into an array
char** load_dictionary(char* dict_path, int* dict_size) {
    char** dictionary = malloc(MAX_WORDS * sizeof(char*));
    FILE* file = fopen(dict_path, "r");
    if (file == NULL) {
        printf("Could not open dictionary file %s\n", dict_path);
        exit(EXIT_FAILURE);
    }

    char word[MAX_WORD_LENGTH];
    while (fscanf(file, "%s", word) != EOF && *dict_size < MAX_WORDS) {
        dictionary[*dict_size] = malloc(strlen(word) + 1);
        strcpy(dictionary[*dict_size], word);
        (*dict_size)++;
    }

    fclose(file);
    return dictionary;
}

// Function to check if a word is in the dictionary using binary search
int find(char** dictionary, int dict_size, char* word) {
    return bsearch(&word, dictionary, dict_size, sizeof(char*), cmp) != NULL;
}

// Function to process a file
void process_file(char* filename, char** dictionary, int dict_size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return;
    }

    char word[MAX_WORD_LENGTH];
    int line_number = 1; // Initialize line number
    int column_number = 1; // Initialize column number


    while (fscanf(file, "%s", word) != EOF) {
        // Remove trailing punctuation
        int len = strlen(word);
        while (len > 0 && ispunct(word[len - 1])) {
            word[len - 1] = '\0';
            len--;
        }

        // Ignore quotation marks and brackets at the start
        while (len > 0 && (word[0] == '\'' || word[0] == '"' || word[0] == '(' || word[0] == '[' || word[0] == '{')) {
            memmove(word, word + 1, len);
            len--;
        }

        // Handle hyphenated words
        char* token = strtok(word, "-");
        while (token != NULL) {
            if (!find(dictionary, dict_size, token)) {
                printf("%s (%d,%d): %s\n", filename, line_number, column_number, token);
            }
            token = strtok(NULL, "-");
            column_number += strlen(word) + 1; // Update column number

        }

        line_number++; // Move to the next line
    }

    fclose(file);
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

