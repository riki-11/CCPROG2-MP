#include <stdio.h>
#include <string.h>
#include <strings.h>

// Max no. of letters per entry
#define LETTERS 20

// Max no. of pairs per entry
#define PAIRS 10

// Max no. of entries
#define ENTRIES 150

// Struct for a single pair (could do something like typedef char string[LETTERS])
typedef struct Pair {
    char language[LETTERS];
    char translation[LETTERS];
} pair;

/* Struct for an entry in the database
    aPairs - an array of pairs, max. of 10 per entry
    nPairs - no. of pairs currently in entry, 0 by default
*/
typedef struct Entry {
    pair aPairs[PAIRS];
    int nPairs;
} entry;

void initDatabase(entry aEntries[]);
void displayMainMenu();
void displayMDMenu();
void displayLTMenu();
void getMMInput(int* nMMInput);
void getMDInput(int* nMLInput);
void getLTInput(int* nMLInput);
void switchMainMenu(int nMMInput, int* nMLInput, entry*, int*);
void switchMDMenu(int nMLInput, entry*, int*);
void addEntry(entry*, int*);
int pairExists(int, entry*, char*, char*);
void displayEntries(entry*, int*);

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
        aEntries[i].nPairs = 0;
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
            displayEntries(aEntries, nCount);
            break;
    }
}

void addEntry(entry aEntries[], int *nCount)
{
    char cRepeat, sLanguage[LETTERS], sWord[LETTERS];
    int pairCount = 0, nFound = 0, newEntry = 1, i = 0;

    // Try to add a new entry
    printf("Enter Language: ");
    scanf(" %s", sLanguage);
    printf("Enter Translation: ");
    scanf(" %s", sWord);

    // Check if pair already exists in database of entries
    while (i < *nCount && !nFound)
    {
        nFound = pairExists(i, aEntries, sLanguage, sWord);
        i++;
    }

    // If pair does NOT exist yet
    if (!nFound)
    {
        // Make NEW entry with this pair 
        strcpy(aEntries[*nCount].aPairs[pairCount].language, sLanguage);
        strcpy(aEntries[*nCount].aPairs[pairCount].translation, sWord);
        
        // Add new entry to database, and update no. of pairs
        aEntries[*nCount].nPairs += 1;
        pairCount++;
        
        // Let user input more pairs if they wish
        do {
            printf("Do you wish to input another pair? (Y/N) ");
            scanf(" %c", &cRepeat);
            
            // Basically repeat the stuff above, except we're just adding to the current pair (note to self: make code less repetitive)
            if (cRepeat == 'Y' || cRepeat == 'y')
            {
                printf("Enter Language: ");
                scanf(" %s", sLanguage);
                printf("Enter Translation: ");
                scanf(" %s", sWord);

                strcpy(aEntries[*nCount].aPairs[pairCount].language, sLanguage);
                strcpy(aEntries[*nCount].aPairs[pairCount].translation, sWord);

                aEntries[*nCount].nPairs += 1;
                pairCount++;
            }
        } while((cRepeat == 'Y' || cRepeat == 'y') && aEntries[*nCount].nPairs < 10);

        // Update no. of entries in the database
        *nCount += 1;
    }
}

// Returns 0 if pair doesn't exist, 1 if it does
int pairExists(int nIndex, entry aEntries[], char *sLanguage, char *sWord)
{
    int i = 0, j;

    while (i < PAIRS)
    {
        if (strcasecmp(sLanguage, aEntries[nIndex].aPairs[i].language) == 0 
            && strcmp(sWord, aEntries[nIndex].aPairs[i].translation) == 0)
        {
            printf("Found duplicate @ entry no. %d\n", nIndex);
            for (j = 0; j < aEntries[nIndex].nPairs; j++)
                printf("Language: %s | Translation: %s\n", aEntries[nIndex].aPairs[j].language, aEntries[nIndex].aPairs[j].translation);
            
            // Immediately exit out of function
            return 1;
        }
        i++;
    }
    return 0;
}

void displayEntries(entry aEntries[], int *nCount)
{
    int i, j;
    printf("---------------\n");
    printf("There are currently %d entries in the database\n", *nCount);

    // fix this for loop later to properly show all the pairs inside an entry
    for (i = 0; i < *nCount; i++)
    {
        printf("Entry No. %d\n", i);
        for (j = 0; j < aEntries[i].nPairs; j++)
            printf("Pair No. %d || Language %s | Translation: %s\n", j, aEntries[i].aPairs[j].language, aEntries[i].aPairs[j].translation);
    }
}