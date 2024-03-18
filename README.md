# SpellCheck
This spell checker is a command-line utility designed to verify the spelling of words in text files against a given dictionary. It supports processing individual files as well as all files within specified directories, including nested directories. The program prints the location (filename, line, and column) and the incorrect word for each spelling mistake found.

# Features
- Checks the spelling of words in text files against a provided dictionary. <br>
- Processes single files or entire directories recursively.  <br>
- Displays the filename, line number, column number, and the misspelled word.  <br>

# Dependencies
- GCC (GNU Compiler Collection) or any compatible C compiler  <br>
- Standard C libraries (stdio.h, stdlib.h, string.h, limits.h, dirent.h, sys/stat.h, fcntl.h, unistd.h, ctype.h)  <br>

# Compilation
 - To compile the spell checker, use the following command in the terminal:  <br>
**gcc -o spellchecker spellchecker.c**

# Usage
After compiling the program, run it using the following syntax:  <br>
**./spellchecker <dictionary_path> <file_or_directory_path>...**  <br> <br>
<dictionary_path>: Path to the text file containing the dictionary words, one per line.  <br>
<file_or_directory_path>: Path to a text file or directory to check. Multiple paths can be specified.  <br>

# Program Structure
### cmp
**Purpose**: The cmp function is designed for comparing two strings. It serves as a callback function for both qsort (Quick Sort) and bsearch (Binary Search) standard library functions. <br>
**Operation**: It takes two const void* arguments, casts them to const char** (pointer to pointer to char), and uses strcmp to compare the strings they point to. It returns an integer less than, equal to, or greater than zero if the first string is found, respectively, to be less than, to match, or be greater than the second string. <br>
**Significance**: This function is essential for sorting the dictionary in lexicographical order and efficiently searching for words within the sorted dictionary, thereby optimizing the spell-checking process. <br>
### load_dictionary
**Purpose**: load_dictionary is tasked with loading the words from a specified dictionary file into memory, preparing the spell checker to verify text against these loaded words. <br>
**Operation**: It opens the provided dictionary file for reading and dynamically allocates memory for storing each word. It reads the file character by character, constructing words and storing them in an array of string pointers until either the file ends or the maximum number of words (MAX_WORDS) is reached. <br>
**Significance**: Proper loading and storing of dictionary words are crucial for the spell-checking functionality. This function sets up the necessary data structure (a sorted array of words) that enables fast and efficient spell checking across the provided texts. <br>
### find
**Purpose**: This function checks whether a given word exists in the loaded dictionary, employing a binary search algorithm for this purpose. <br>
**Operation**: It takes the word to be searched, the dictionary array, and its size as arguments. By using binary search, it significantly reduces the search time compared to linear search, especially for large dictionaries. <br>
**Significance**: The find function is pivotal in identifying whether each word in the text being checked is spelled correctly according to the dictionary, directly influencing the accuracy and performance of the spell checker. <br>
### process_file
**Purpose**: process_file processes a single text file, examining each word against the dictionary and reporting any misspellings along with their location in the text. <br>
**Operation**: It reads the file content character by character, identifying words based on spaces, punctuation, and line breaks. Each identified word is then checked against the dictionary using the find function. If a word is not found, its occurrence (filename, line, and column) is printed to standard output. <br>
**Significance**: This function directly implements the spell-checking feature for individual files, providing detailed feedback on spelling errors, which is essential for users to correct their texts. <br>
### process_directory
**Purpose**: process_directory extends the spell-checking capability to directories, allowing the program to recursively process all text files within a given directory and its subdirectories. <br>
**Operation**: It uses directory traversal techniques to iterate over all entries within a directory. When it encounters a file, it processes it with process_file; when it finds a directory (excluding special directories . and ..), it recursively calls itself to process that directory. <br>
**Significance**: By supporting directory traversal, this function greatly enhances the usability of the spell checker, allowing for batch processing of files and facilitating its integration into workflows that require checking numerous files or entire projects. <br>
### main
**Purpose**: The main function serves as the entry point of the program. It orchestrates the overall spell-checking process by parsing command-line arguments, loading the dictionary, and initiating the processing of files or directories as specified by the user. <br>
**Operation**: It first checks for the correct number of arguments, loads the dictionary into memory, sorts it for efficient searching, and then processes each specified file or directory according to the arguments provided by the user. <br>
**Significance**: As the coordinating function, main is crucial for the correct operation of the spell checker. It ensures that the program is used correctly and fulfills its purpose by providing a user-friendly command-line interface and managing the flow of the spell-checking process. <br>

# Note:
This program does not dynamically allocate memory for each word in the dictionary or the text files. It uses fixed-size buffers, which means extremely long words may not be processed correctly. 
