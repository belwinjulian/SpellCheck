Belwin Julian (bj273)
Soma Parvathini (svp98)


# SpellCheck
This command-line spell checker helps ensure the accuracy of your text files by cross-referencing them with a word dictionary. It can process single files or scan entire directories (including subdirectories). When it finds a misspelling, it reports the file, line number, column number, and the incorrect word.


#Test Cases

Test Case 1: Basic Functionality

Input: A simple text file with correctly spelled words and intentional misspellings.
Expected Output: A list of potential misspellings with their line and column numbers.
Test Case 2: "MacDonald" Variations

Input: Text containing "MacDonald" in different cases (macdonald, MacDonald, MACDONALD).
Expected Output: Only the lowercase "macdonald" should be reported as a potential misspelling.
Test Case 3: Hyphens

Input: Text with hyphenated words (e.g., "well-known", "self-explanatory").
Expected Output: Hyphenated words should be treated as single words.
Test Case 4: Leading Parentheses

Input: Text with words starting with parentheses (e.g., "(This").
Expected Output: Words with leading parentheses should be processed correctly.



# Features
- **Verifies spelling against a dictionary**: Ensures the accuracy of your text by comparing words against a provided word list. <br>
- **Flexible processing**: Can handle individual files or recursively scan entire directory structures (including subdirectories) for comprehensive spell-checking.  <br>
- **Detailed error reporting**: Pinpoints misspellings by providing the filename, line number, column number, and the incorrect word, simplifying the editing process.  <br>

# Dependencies
- GCC (GNU Compiler Collection) or any other C compiler which is compatible  <br>
- Standard C libraries, which include, stdio.h, stdlib.h, string.h, limits.h, dirent.h, sys/stat.h, fcntl.h, unistd.h, ctype.h  <br>

# Compilation
 - To compile the spell checker, use the command as follows:  <br>
**gcc -o spellchecker spchk.c**

# Usage
After compiling the program, run it using the syntax as follow:  <br>
**./spellchecker <dictionary_path> <file_or_directory_path>...**  <br> <br>
<dictionary_path>:  Specifies the location of the text file containing your word list (one word per line). This file is the reference for correct spellings.  <br>
<file_or_directory_path>: Indicates the text file or the directory you want to spell-check. The program can handle multiple paths for broader analysis.  <br>

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
