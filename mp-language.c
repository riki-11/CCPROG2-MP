#include <stdio.h>
#include <string.h>

// Max no. of letters per entry
#define LETTERS 20

// Max no. of pairs per entry
#define PAIRS 10

// Max no. of entries
#define ENTRIES 150

// Struct for a single pair
typedef struct Pair {
    char language[LETTERS];
    char translation[LETTERS];
} pair;

/* Struct for an entry in the database
    aPairs - an array of pairs, there are a max. of 10 per entry
    nCount - no. of pairs currently in entry, 0 by default
*/
typedef struct Entry {
    pair aPairs[PAIRS];
    int nCount;
} entry;

void initDatabase(entry aEntries[]);
void displayMainMenu();
void displayMDMenu();
void displayLTMenu();
void getMMInput(int* nMMInput);
void getMDInput(int* nMLInput);
void getLTInput(int* nMLInput);
void switchMainMenu(int nMMInput, int* nMLInput, entry aEntries[], int*);
void switchMDMenu(int nMLInput, entry aEntries[], int*);
void addEntry(entry aEntries[], int*);
void displayEntries(entry aEntries[], int*);

int
main()
{
    int nMMInput = 0, nMLInput = 0, nEntryCount = 0;
    entry aEntries[ENTRIES];

    initDatabase(aEntries); // Initializes database of entries
    displayMainMenu(); // Display Main Menu
    getMMInput(&nMMInput); // User chooses between MD and LT
    switchMainMenu(nMMInput, &nMLInput, aEntries, &nEntryCount); // Program processes user's input
    
    return 0;
}

// Function for setting the no. of pairs for all entries to 0 (structs can't have default values)
void initDatabase(entry aEntries[])
{
    int i;
    for (i = 0; i < ENTRIES; i++)
        aEntries[i].nCount = 0;
}

void
displayMainMenu()
{
    printf("------------Main Menu-----------");
    printf("\n  [1] Manage Data Menu");
    printf("\n  [2] Language Tools Menu");
    printf("\n\n");
}

void
displayMDMenu()
{
    printf("---------Manage Data Menu--------");
    printf("\n   [1] Add Entry");
    printf("\n   [2] Add Transations");
    printf("\n   [3] Modify Entry");
    printf("\n   [4] Delete Entry");
    printf("\n   [5] Delete Translation");
    printf("\n   [6] Display All Entries");
    printf("\n   [7] Search Word");
    printf("\n   [8] Search Translation");
    printf("\n   [9] Export");
    printf("\n  [10] Import");
    printf("\n  [11] Exit");
    printf("\n\n");
}

void
displayLTMenu()
{
    printf("-------Language Tools Menu------");
    printf("\n  [1] Identify Main Language");
    printf("\n  [2] Simple Translation");
    printf("\n  [3] Exit");
    printf("\n\n");
}

void
getMMInput(int* nMMInput)
{
    printf("Choose menu option: ");
    scanf("%d", nMMInput);
}

void
getMDInput(int* nMLInput)
{
    printf("Choose manage option: ");
    scanf("%d", nMLInput);
}

void
getLTInput(int* nMLInput)
{
    printf("Choose tool option: ");
    scanf("%d", nMLInput);
}

// Function for moving around main menu
void
switchMainMenu(int nMMInput, int* nMLInput, entry aEntries[], int *nCount)
{

    switch(nMMInput)
    {
        case 1:
            displayMDMenu();
            getMDInput(nMLInput);
            switchMDMenu(*nMLInput, aEntries, nCount);
            break;
        case 2:
            displayLTMenu();
            getLTInput(nMLInput);
            break;
        default:
            printf("Invalid Input");
    }
}

// Function for moving around MD menu
void
switchMDMenu(int nMLInput, entry aEntries[], int *nCount)
{
    // put this on some sort of loop so that if we exit out of a menu option we come back
    switch(nMLInput)
    {
        case 1:
            addEntry(aEntries, nCount);
            displayEntries(aEntries, nCount); // add nCount
            break;
    }
}

void addEntry(entry aEntries[], int *nCount)
{
    char cRepeat, sLanguage[LETTERS], sWord[LETTERS];
    int pairCount = 0;
    
    do {
        // Let user input a language and translation pair (stored in sLanguage and sWord first because not sure if gonna add it yet)
        printf("Language: ");
        scanf(" %s", sLanguage);
        printf("Translation: ");
        scanf(" %s", sWord);
        

        // if pair NOT already exists
            // Add pair to NEW entry and update no. of pairs inside current entry
            strcpy(aEntries[*nCount].aPairs[pairCount].language, sLanguage);
            strcpy(aEntries[*nCount].aPairs[pairCount].translation, sWord);

            *nCount += 1;
            pairCount++;
        // end of block

        // Terminate function if user no longer wishes to add pairs
        printf("Do you wish to input another pair? (Y/N) ");
        scanf(" %c", &cRepeat);

    } while (cRepeat == 'Y' || cRepeat == 'y');


    /* check if pair already exists in some entry (language first, then word)
        if PAIR EXISTS
            if new entry
                yes - create a new entry
                no - go back to menu
        if PAIR DOESN'T EXIST (DONE!)
            add pair to new entry
    */
}

void displayEntries(entry aEntries[], int *nCount)
{
    printf("There are currently %d entries in the database\n", *nCount);
    printf("Language: %s Translation: %s\n", aEntries[0].aPairs[0].language, aEntries[0].aPairs[0].translation);
    printf("Language: %s Translation: %s\n", aEntries[1].aPairs[1].language, aEntries[1].aPairs[1].translation);
    printf("Language: %s Translation: %s\n", aEntries[2].aPairs[2].language, aEntries[2].aPairs[2].translation);
}