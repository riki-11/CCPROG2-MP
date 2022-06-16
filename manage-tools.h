// Max no. of letters per entry
#define MAX_LETTERS 21

// Max no. of pairs per entry
#define MAX_PAIRS 10

// Max no. of entries
#define MAX_ENTRIES 150

// Max no. of languages
#define LANGUAGES 7100

typedef char str[MAX_LETTERS];

// Struct for a single pair
typedef struct pairTag {
    str language;
    str translation;
} pair;

/* Struct for an entry in the database
    aPairs - an array of pairs, max. of 10 per entry
    nPairs - no. of pairs currently in entry, 0 by default
*/
typedef struct entryTag {
    pair aPairs[MAX_PAIRS];
    int nPairs;
} entry;

typedef struct languageTag {
    str language;
    int nLanguageCount;
} languageType;

typedef struct matchingTag {
    str language,
        word;
} matchingType;

// Initialization and Menu-related functions
void displayMainMenu();
void displayMDMenu();
void displayLTMenu();
void getMMInput(int*);
void getMDInput(int*);
void getLTInput(int*);
void switchMainMenu(int, int*, entry*, int*, int*, int*, int*);
void switchMDMenu(int, entry*, int*);
void initDatabase(entry*);
void clearDatabase(entry*, int*);

// Manage Data functions
void inputPair(str, str);
int pairExists(entry*, int, str, str);
void addEntry(entry*, int*);
void addPair(entry*, int, int, str, str);
char makeNewEntry(entry*, int*, str, str);
void addTranslation(entry*, int);
int searchPair(entry*, int, str, str, int*);
void modifyEntry(entry*, int);
void deleteEntry(entry*, int*);
void deleteTranslation(entry*, int*);
void sortEntry(entry*, int);
void displayEntry(entry*, int, int);
void displayAllEntries(entry*, int);
void searchWord(entry*, int);
void searchTranslation(entry*, int);
void export(entry*, int);
void import(entry*, int*);

// Language Tool Functions
void switchLTMenu(int, int*, int*, int*);
void identifyML(int*, int*, int*);
int emptyLanguageMember(languageType[], int);
int emptyMatchingMember(matchingType[], int);
int matchingPairs(str[], matchingType[], int*, int);
void fileReader(str, str[], char[], int*, int*);