#include <stdio.h>
#include <string.h>
#include <strings.h>

// Max no. of letters per entry
#define LETTERS 20

// Max no. of pairs per entry
#define PAIRS 10

// Max no. of entries
#define ENTRIES 150

typedef char str[LETTERS];

// Struct for a single pair (could do something like typedef char str[LETTERS])
typedef struct Pair {
    str language;
    str translation;
} pair;

/* Struct for an entry in the database
    aPairs - an array of pairs, max. of 10 per entry
    nPairs - no. of pairs currently in entry, 0 by default
*/
typedef struct Entry {
    pair aPairs[PAIRS];
    int nPairs;
} entry;

void initDatabase(entry*);
void displayMainMenu();
void displayMDMenu();
void displayLTMenu();
void getMMInput(int*);
void getMDInput(int*);
void getLTInput(int*);
void switchMainMenu(int, int*, entry*, int*);
void switchMDMenu(int, entry*, int*);
void addEntry(entry*, int*);
int pairExists(entry*, int, str, str);
void addPair(entry*, int, int*, char*, char*);
void makeNewEntry(entry*, int*, str, str);
void displayEntries(entry*, int*);

int
main()
{
    int nMMInput = 0, nMLInput = 0, nEntryCount = 0;
    entry aEntries[ENTRIES];

    initDatabase(aEntries); // Initializes database of entries
    
    // Always go back to Main Menu as long as user doesn't exit program 
    do {
        displayMainMenu(); // Display Main Menu
        getMMInput(&nMMInput); // User chooses between MD and LT

        if (nMMInput != 3)
            switchMainMenu(nMMInput, &nMLInput, aEntries, &nEntryCount); // Program processes user's input
        
    } while (nMMInput != 3);

    return 0;
}

// Function for setting the no. of pairs for all entries to 0 (structs can't have default values)
void initDatabase(entry aEntries[])
{
    int i;
    for (i = 0; i < ENTRIES; i++)
        aEntries[i].nPairs = 0;
}

void
displayMainMenu()
{
    printf("------------Main Menu-----------");
    printf("\n  [1] Manage Data Menu");
    printf("\n  [2] Language Tools Menu");
    printf("\n  [3] Exit");
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
    printf("\n  [11] Back to Main Menu");
    printf("\n\n");
}

void
displayLTMenu()
{
    printf("-------Language Tools Menu------");
    printf("\n  [1] Identify Main Language");
    printf("\n  [2] Simple Translation");
    printf("\n  [3] Back to Main Menu");
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
            // keep looping through this until user enters the int 11 to exit MD Menu
            do 
            {
                displayMDMenu();
                getMDInput(nMLInput);
                if (*nMLInput != 11)
                    switchMDMenu(*nMLInput, aEntries, nCount);
            } while (*nMLInput != 11);
            break;

        case 2:
            displayLTMenu();
            getLTInput(nMLInput);
            break;
        
        case 3:
            printf("Terminating Program\n");
            return;

        default:
            printf("Invalid Input");
    }
}

// Function for moving around MD menu
void
switchMDMenu(int nMLInput, entry aEntries[], int *nCount)
{
    // displayEntries(aEntries, nCount);
    switch(nMLInput)
    {
        case 1:
            addEntry(aEntries, nCount);
            break;
        case 6:
            displayEntries(aEntries, nCount);
            break;

        // Exits MD Menu
        case 11:
            return;
        
        default:
            printf("Invalid input\n");
    }
}

/*
    Function called when inputting '1' in the MD menu to add a new entry in database
        @param aEntries - array of Entries
        @param pCount - pointer to no. of entries in database
*/
void addEntry(entry aEntries[], int *pCount)
{
    str sLanguage, sWord;
    char cRepeat, newEntry;
    int pairCount = 0, nFound = 0;

    // Try to add a new entry
    printf("Enter Language: ");
    scanf(" %s", sLanguage);
    printf("Enter Translation: ");
    scanf(" %s", sWord);

    // Check if pair already exists in database of entries
    nFound = pairExists(aEntries, *pCount, sLanguage, sWord);

    // If pair does NOT exist yet in any entry
    if (!nFound)
        makeNewEntry(aEntries, pCount, sLanguage, sWord);

    // If pair DOES exist, ask if this is new entry
    else
    {
        printf("Pair already exists, is this a new entry?\n");
        scanf(" %c", &newEntry);

        if (newEntry == 'Y' || newEntry == 'y')
            makeNewEntry(aEntries, pCount, sLanguage, sWord);
        else
        {
            printf("Returning to menu...\n\n");
            return;
        }
    }
}

/*
    Function for checking if pair to be added already exists in database
        @param aEntries - array of entries in database
        @param nCount - no. of entries in database
        @param sLanguage - Language to be checked
        @param sWord - Word to be checked
    returns 1 if duplicate is found, 0 otherwise
*/
int pairExists(entry aEntries[], int nCount, str sLanguage, str sWord)
{
    int i, j, k, nFound = 0;

    // Go through each entry
    for (i = 0; i < nCount; i++)
    {
        // Go through each pair in the entry, and print any duplicate pairs
        for  (j = 0; j < PAIRS; j++)
        {
            // Checks language case-insensitively, checks word case-sensitively
            if (strcasecmp(sLanguage, aEntries[i].aPairs[j].language) == 0
                && strcmp(sWord, aEntries[i].aPairs[j].translation) == 0)
            {
                printf("--------------------------------\n");
                printf("Found duplicate @ entry no. %d\n", i);
                
                // Print this entry's information
                for (k = 0; k < aEntries[i].nPairs; k++)
                    printf("Language: %s | Translation: %s\n", aEntries[i].aPairs[k].language, aEntries[i].aPairs[k].translation);
                
                printf("--------------------------------\n");
                // Declare that a duplicate has been found
                nFound = 1;
            }
        }
    }
    return nFound;
}

/*
    Function for adding a pair to an entry (maybe separate from addTranslation??? idk yet)
        @param aEntries - array of Entries
        @param nCount - no. of entries in database
        @param pPairCount - pointer to no. of pairs in the entry that is being updated
        @param sLanguage - Language to be checked
        @param sWord - Word to be checked
*/
void addPair(entry aEntries[], int nCount, int *pPairCount, str sLanguage, str sWord)
{
    // Add a pair to designated entry
    strcpy(aEntries[nCount].aPairs[*pPairCount].language, sLanguage);
    strcpy(aEntries[nCount].aPairs[*pPairCount].translation, sWord);

    // Update no. of pairs
    aEntries[nCount].nPairs += 1;
    *pPairCount += 1;
}

/*
    Function for ACTUALLY creating the entry itself (separate from addEntry)
        @param aEntries - array of Entries
        @param pCount - pointer to no. of entries in database
        @param sLanguage - Language to be checked
        @param sWord - Word to be checked
*/
void makeNewEntry(entry aEntries[], int *pCount, str sLanguage, str sWord)
{
    char cRepeat, newEntry;
    int nFound, pairCount = 0;

    // Create new entry, add the pair to it, and update no. of entries
    addPair(aEntries, *pCount, &pairCount, sLanguage, sWord);
    *pCount += 1;

    // Let user input more pairs in the SAME entry
    do 
    {
        printf("Do you wish to input another pair? (Y/N) ");
        scanf(" %c", &cRepeat);
        
        if (cRepeat == 'Y' || cRepeat == 'y')
        {
            printf("Enter Language: ");
            scanf(" %s", sLanguage);
            printf("Enter Translation: ");
            scanf(" %s", sWord);

            // Check if pair already exists in database
            nFound = pairExists(aEntries, *pCount, sLanguage, sWord);

            // If new pair does not exist elsewhere
            if (!nFound)
                // Add to aEntries[nCount-1] because no. of entries was already incremented earlier
                addPair(aEntries, (*pCount)-1, &pairCount, sLanguage, sWord);

            // If new pair DOES exist elsewhere
            else
            {
                printf("Pair already exists, is this a new entry?\n");
                scanf(" %c", &newEntry);

                // Just call the function itself again
                if (newEntry == 'Y' || newEntry == 'y')
                    makeNewEntry(aEntries, pCount, sLanguage, sWord);
                else
                {
                    printf("Returning to menu...\n\n");
                    return;
                }
            }
        }

    // While user wants to place new pairs AND while max no. of pairs hasn't been reached
    } while((cRepeat == 'Y' || cRepeat == 'y') && aEntries[*pCount-1].nPairs < 10);
}

void displayEntries(entry aEntries[], int *nCount)
{
    int i, j;

    printf("\n--------------------------------\n");
    printf("%d entries in the database\n", *nCount);
    printf("--------------------------------\n\n");

    for (i = 0; i < *nCount; i++)
    {
        printf("Entry No. %d with pair count %d\n", i, aEntries[i].nPairs);
        for (j = 0; j < aEntries[i].nPairs; j++)
            printf("Pair No. %d || Language: %s | Translation: %s\n", j, aEntries[i].aPairs[j].language, aEntries[i].aPairs[j].translation);
        printf("--------------------------------\n");
    }

    printf("--------------------------------\n\n");
}