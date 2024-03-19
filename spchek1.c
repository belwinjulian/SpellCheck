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

char** dictionary;
char** Udictionary;

// Function to compare two strings for qsort and bsearch
int cmp(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

// Modified load_dictionary function to use read
void load_dictionary(char* dict_path, int* dict_size) {
    dictionary = malloc(MAX_WORDS * sizeof(char*));
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
}

void initializeUdictionary(int dict_size) {
    Udictionary = malloc(MAX_WORDS * sizeof(char*)); // Allocate space for Udictionary

    for (int i = 0; i < dict_size; ++i) {
        int wordLen = strlen(dictionary[i]);
        Udictionary[i] = malloc((wordLen + 1) * sizeof(char)); // Allocate space for each word

        // Convert each character to uppercase and copy it to Udictionary
        for (int j = 0; j <= wordLen; ++j) {
            Udictionary[i][j] = toupper((unsigned char)dictionary[i][j]);
        }
    }
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
    int column_number = 1; // Start at 1 for the first character
    int start_column = 1; // Correctly set for the start of the first word

    while (read(fd, &buffer, 1) > 0) {
        if (buffer == '\n') {
            if (word_index > 0) { // Check if there's a word before the newline
                word[word_index] = '\0'; // Null-terminate the word
                // Directly check the original word against the dictionary
                int found = find(dictionary, dict_size, word);

                if(!found) {
                    found = find(Udictionary, dict_size, word);
                }

                // If not found and the word is not already lowercase, check lowercase version
                if (!found && word[0] != tolower(word[0])) {
                    char wordLower[MAX_WORD_LENGTH];
                    for (int i = 0; word[i]; i++) {
                        wordLower[i] = tolower(word[i]);
                    }
                    wordLower[word_index] = '\0';
                    found = find(dictionary, dict_size, wordLower);
                }
               


                if (!found) {
                    printf("%s (%d,%d): %s\n", filename, line_number, start_column, word);
                }
                word_index = 0; // Reset word index for the next word
                memset(word, 0, MAX_WORD_LENGTH); // Clear the word buffer
            }
            line_number++;
            column_number = 1; // Reset to 0 because it's incremented at the end of the loop
            continue;
        }

        if (isspace(buffer) || ispunct(buffer)) {
            if (word_index > 0) {
                word[word_index] = '\0';
                // Repeat the check logic for words followed by space or punctuation
                int found = find(dictionary, dict_size, word);

                if(!found) {
                    found = find(Udictionary, dict_size, word);
                }

                if (!found && word[0] != tolower(word[0])) {
                    char wordLower[MAX_WORD_LENGTH];
                    for (int i = 0; word[i]; i++) {
                        wordLower[i] = tolower(word[i]);
                    }
                    wordLower[word_index] = '\0';
                    found = find(dictionary, dict_size, wordLower);
                }

                if (!found) {
                    printf("%s (%d,%d): %s\n", filename, line_number, start_column, word);
                }

                word_index = 0;
                memset(word, 0, MAX_WORD_LENGTH);
            }

            column_number++;
            if (buffer != '-') { // Adjust start_column correctly
                start_column = column_number + 1; // Next character starts a new word
            }
        } else {
            if (word_index == 0) {
                start_column = column_number; // Set at the start of a new word
            }
            word[word_index++] = buffer;
            column_number++;
        }
    }

    // Handle the last word in the file, repeating the check logic
    if (word_index > 0) {
        word[word_index] = '\0';
        int found = find(dictionary, dict_size, word);

        if(!found) {
                    found = find(Udictionary, dict_size, word);
                }

        if (!found && word[0] != tolower(word[0])) {
            char wordLower[MAX_WORD_LENGTH];
            for (int i = 0; word[i]; i++) {
                wordLower[i] = tolower(word[i]);
            }
            wordLower[word_index] = '\0';
            found = find(dictionary, dict_size, wordLower);
        }

        if (!found) {
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
    load_dictionary(argv[1], &dict_size);
    initializeUdictionary(dict_size);
    qsort(Udictionary, dict_size, sizeof(char*), cmp);



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
