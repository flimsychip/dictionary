#include <iostream>
#include <fstream>
#include <vector>
#include <sys/stat.h> // contains struct for filesize info in getFileSize()
#define DEFAULT_DICT "dictionary1.txt"
#define DEFAULT_OUTPUT_FILE "dict-updated.txt"

using namespace std;

enum Actions { QUIT, COUNT, PRINT, LINEAR_SEARCH, DELETE, SWAP, BUBBLE_SORT, SELECTION_SORT, BINARY_SEARCH, INSERT, MERGE, LOAD, WRITE_TO_FILE }; 

const int min_menu_option = COUNT;
const int max_menu_option = WRITE_TO_FILE;
const int quit_menu_option = QUIT;
const int min_dict_option = 1;
const int max_dict_option = 7;
const double AVERAGE_WORD_LENGTH = 7;   // assumption about size of words in dictionaries
const double CUSHION = 1.25;            // how much beyond projected size should we reserve
const unsigned int DEFAULT_SIZE = 1000; // if we can't estimate, what default value should we use

/* function prototypes */
// helper
int getMenuChoice();
int getDictChoice(const int currDict);
vector<string>::const_iterator findWord (const string word, const vector<string> &vect);
// actions
int countWords(const vector<string> &vect);
void printToScreen(const vector<string> &vect);
void promptFind(const string word, vector<string> &vect, const Actions mode); // handles linear search and delete
bool loadDict(const string filename, vector<string> &vect, bool verbose = false);
void writeToFile(const vector<string> &vect);
// given
double getFileSize(const string &filename);
double getFileSize(const char* filename);

int main() { 
   int choice;
   int temp;
   int dictOption = min_dict_option;
   string choiceStr;
   vector<string> dict;
   loadDict(DEFAULT_DICT, dict);
   
   do {
      choice = getMenuChoice();
      switch(choice) {
         case COUNT:
            cout << "There are " << countWords(dict) << " words in dictionary #" << dictOption << "." << endl;
            break;
         case PRINT:
            printToScreen(dict);
            break;
         case LINEAR_SEARCH:
            cout << "Enter a word to find: ";
            cin >> choiceStr;
            promptFind(choiceStr, dict, LINEAR_SEARCH);
            break;
         case DELETE:
            cout << "Enter a word to delete from the chosen Dictionary: ";
            cin >> choiceStr;
            promptFind(choiceStr, dict, DELETE);
            break;
         case SWAP:
         case BUBBLE_SORT:
         case SELECTION_SORT:
         case BINARY_SEARCH:
         case INSERT:
         case MERGE:
            cout << "Coming soon!" << endl;
            break;
         case LOAD:
            temp = getDictChoice(dictOption);
            if(loadDict("dictionary" + to_string(temp) + ".txt", dict)) { 
               dictOption = temp;
               cout << "Dictionary " << dictOption << " is open." << endl; 
            }
            break;
         case WRITE_TO_FILE:
            writeToFile(dict);
            break;
         default:
            cout << "Thank you! Bye!" << endl;
      }
   } while(choice != quit_menu_option);
	return 0; 
} 
 
int getMenuChoice() {
   int choice;
   do {
      cout << "--------------------------------------------" << endl;
      cout << "Options menu:" << endl;
      cout << "(" << COUNT << ") Count - get number of words in dictionary" << endl;
      cout << "(" << PRINT << ") Print words to screen" << endl;
      cout << "(" << LINEAR_SEARCH << ") Find a word (Linear Search)" << endl;
      cout << "(" << DELETE << ") Find word, delete if found" << endl;
      cout << "(" << SWAP << ") Swap two words" << endl;
      cout << "(" << BUBBLE_SORT << ") Sort words (Bubble Sort)" << endl;
      cout << "(" << SELECTION_SORT << ") Sort words (Selection Sort)" << endl;
      cout << "(" << BINARY_SEARCH << ") Find a word - Binary Search (assumes words are sorted alphabetically)" << endl;
      cout << "(" << INSERT << ") Find word, insert if not found (assumes words are sorted alphabetically)" << endl;
      cout << "(" << MERGE << ") Merge two dictionaries (will sort first)" << endl;
      cout << "(" << LOAD << ") Load a dictionary (closes current dictionary)" << endl;
      cout << "(" << WRITE_TO_FILE << ") Write current dictionary to file" << endl;
      cout << "Enter a number from " << min_menu_option << " to " << max_menu_option << ", or " << quit_menu_option << " to exit: ";
      
      cin >> choice;
      if(choice < quit_menu_option || choice > max_menu_option) {
         cout << "Error! Input must be a number between " << min_menu_option << " and " << max_menu_option << ", or " << quit_menu_option << " to exit." << endl;
      }
   } while(choice < quit_menu_option || choice > max_menu_option);
   return choice;
}

int getDictChoice(const int currDict) {
   int choice;
   do {
      cout << "Which Dictionary should be opened? Enter a number between \"" << min_dict_option << "\" and \"" << max_dict_option << "\": ";
      cin >> choice;
      if(choice == currDict) { cout << "That dictionary is already open! Pick another." << endl; }
      if(choice < min_dict_option || choice > max_dict_option) { cout << "That number is not in the available range! Pick another." << endl; }
   } while(choice < min_dict_option || choice > max_dict_option || choice == currDict);
   return choice;
}

vector<string>::const_iterator findWord(const string word, const vector<string> &vect) {
   vector<string>::const_iterator IT;
   for(IT = vect.begin(); IT != vect.end(); IT++) {
      if(*IT == word) {
         return IT;   
      }
   }
   return vect.end();
}

int countWords(const vector<string> &vect) {
   int count = 0;
   vector<string>::const_iterator IT;
   for(IT = vect.begin(); IT != vect.end(); IT++) {
      count++;
   }
   return count;
}

void printToScreen(const vector<string> &vect) {
   vector<string>::const_iterator IT;
   for(IT = vect.begin(); IT != vect.end(); IT++) {
      cout << *IT << endl;
   }
}

void promptFind(const string word, vector<string> &vect, const Actions mode) {
   cout << "Your word was '" << word << "'." << endl;
   vector<string>::const_iterator IT = findWord(word, vect);
   if(IT == vect.end()) {
      cout << "We did not find your word." << endl;
      return;
   }
   
   switch(mode) {
      case LINEAR_SEARCH:
         if(IT == vect.begin()) {
            cout << "It is the first word." << endl;
         } else {
            cout << "The previous word would be '" << *(IT - 1) << "'." << endl;
         }
         if(IT == vect.end() - 1) {
            cout << "'" << word << "' is the last word." << endl;
         } else {
            cout << "The next word would be '" << *(IT + 1) << "'." << endl;
         }
         break;
      case DELETE:
         cout << "The word '" << *IT << "' has been deleted." << endl;
         vect.erase(IT);
         break;
   }
}

bool loadDict(const string filename, vector<string> &vect, bool verbose) {
   string str;
   ifstream inFS(filename);
   
   if(!inFS.is_open()) {
      cout << "ERROR! Cannot read chosen dictionary \"" << filename << "\"." << endl;
      return false;
   }
   
   // memory detailed info
   if(verbose) { 
      cout << "## at the start      | .size(): " << vect.size() << "     .capacity(): " << vect.capacity() << " ##" << endl; 
   }
   // CLEAR HAPPENS
   vect.clear();
   double size = (getFileSize(filename) / AVERAGE_WORD_LENGTH);
   if(verbose) {
      cout << "## after .clear()    | .size(): " << vect.size() << "     .capacity(): " << vect.capacity() << " ##" << endl;
      cout << "## before .reserve() | .size(): " << vect.size() << "     .capacity(): " << vect.capacity() << "     filesize: " << getFileSize(filename) << "   estimated reserve: " << (size * CUSHION) << " ##" << endl;
   }
   // ALLOCATION HAPPENS
   vect.reserve(size * CUSHION);
   if(verbose) {
      cout << "## after .reserve()  | .size(): " << vect.size() << "     .capacity(): " << vect.capacity() << endl;
   }
   // FILLING VECTOR
   while(inFS >> str) {
      vect.push_back(str);
   }
   if(verbose) { 
      cout << "## when filled       | .size(): " << vect.size() << "     .capacity(): " << vect.capacity() << " ##" << endl;
   }
   
   inFS.close();
   return true;
}

void writeToFile(const vector<string> &vect) {
   vector<string>::const_iterator IT;
   string filename;
   cout << "Enter name of file (.txt will be appended automatically), or enter 'd' to use default filename: ";
   cin >> filename;
   if(filename == "d") {
      filename = DEFAULT_OUTPUT_FILE;   
   } else {
      filename.append(".txt");
   }
   
   // check if already existing
   ifstream tempFS(filename);
   if(tempFS.is_open()) {
      cout << "Error! The file " << filename << " already exists." << endl;
      return;
   }
   
   cout << "Writing to file..." << endl;
   ofstream outFS(filename);
   for(IT = vect.begin(); IT != vect.end(); IT++) {
      outFS << *IT << endl;
   }
   outFS.close();
   cout << "                   ...Done!" << endl;
}

/* use C-library to get filesize in characters; overloaded to take either C or C++ string
 * getFileSize() takes the name of the file and returns the filesize in characters
 *
 * adapted from examples in K&R and online
 *
 * (K&R) is reference to book: "The ANSI C Programming Language" by Brian Kernighan and Dennis Ritchie
 */
double getFileSize(const string &filename) {
	return getFileSize(filename.c_str());
}

double getFileSize(const char* filename) {
   struct stat fileInfo;
   int fail = stat(filename, &fileInfo);
   if(fail == -1) {         // returns -1 if there is an error (K&R)
      return fail;          // for instance, if the file does not exist
   } 
   return fileInfo.st_size; // st_size is file size in characters (K&R)
}
