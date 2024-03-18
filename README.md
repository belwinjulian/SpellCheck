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
**./spellchecker <dictionary_path> <file_or_directory_path>...**  <br>
<dictionary_path>: Path to the text file containing the dictionary words, one per line.  <br>
<file_or_directory_path>: Path to a text file or directory to check. Multiple paths can be specified.  <br>

# Program Structure
cmp: Compares two strings, used by qsort and bsearch for sorting and searching in the dictionary.  <br>
load_dictionary: Loads words from a dictionary file into memory. Words are expected to be separated by newlines or spaces.  <br>
find: Checks if a given word exists in the dictionary using binary search.  <br>
process_file: Processes a single file, checks each word against the dictionary, and prints out any words not found in the dictionary along with their location.  <br>
process_directory: Recursively processes all files in a given directory and its subdirectories.  <br>
main: Entry point of the program. Parses command-line arguments, loads the dictionary, and processes specified files or directories.  <br>

# Note:
This program does not dynamically allocate memory for each word in the dictionary or the text files. It uses fixed-size buffers, which means extremely long words may not be processed correctly. 
