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
void addPair(entry*, int, int, char*, char*);
char makeNewEntry(entry*, int*, str, str);
void addTranslation(entry*, int);
void search(entry*, int, str, str, int*);
void displayEntries(entry*, int);

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

// Function for setting the no. of pairs for all entries to 0
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
    printf("\n   [2] Add Translations");
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
switchMainMenu(int nMMInput, int* nMLInput, entry aEntries[], int *pCount)
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
                    switchMDMenu(*nMLInput, aEntries, pCount);
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
switchMDMenu(int nMLInput, entry aEntries[], int *pCount)
{
    switch(nMLInput)
    {
        case 1:
            addEntry(aEntries, pCount);
            break;
        case 2:
            addTranslation(aEntries, *pCount);
            break;
        case 6:
            displayEntries(aEntries, *pCount);
            break;

        // Exits MD Menu
        case 11:
            return;
        
        default:
            printf("Invalid input\n");
    }
}

/*
    Function for asking user for language-translation pair
*/
void inputPair(str sLanguage, str sWord)
{
    do 
    {
        printf("Enter Language: ");
        scanf(" %s", sLanguage);
        printf("Enter Translation: ");
        scanf(" %s", sWord);

        // Language and translation input should both have at least 1 character
        if (strcmp(sLanguage, "") == 0 || strcmp(sWord, "") == 0)
            printf("Language and Translation inputs should have at least 1 character each");
        
    } while (strcmp(sLanguage, "") == 0 || strcmp(sWord, "") == 0);
}

/*
    Function called when inputting '1' in the MD menu to add a new entry in database
        @param aEntries - array of Entries
        @param pCount - pointer to no. of entries in database
*/
void addEntry(entry aEntries[], int *pCount)
{
    str sLanguage, sWord;
    char newEntry;
    int nFound = 0;

    // Try to add a new entry
    inputPair(sLanguage, sWord);

    // still need input validation (no empty strings)

    // Check if pair already exists in database of entries
    nFound = pairExists(aEntries, *pCount, sLanguage, sWord);

    // If pair does NOT exist yet in any entry
    if (!nFound) // might have to make this nFound < 0
        makeNewEntry(aEntries, pCount, sLanguage, sWord);

    // If pair DOES exist, ask if this is new entry
    else
    {
        printf("Pair already exists, is this a new entry? (Y/N) ");
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
    returns no. of entries where specific pair is found
*/
int pairExists(entry aEntries[], int nCount, str sLanguage, str sWord)
{
    int i, j, k, nFound = 0;

    // array that keeps track of indexes where duplicate pairs are found

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
                printf("Found pair @ entry no. %d\n", i + 1);
                
                // Print this entry's information
                for (k = 0; k < aEntries[i].nPairs; k++)
                    printf("Language: %s | Translation: %s\n", aEntries[i].aPairs[k].language, aEntries[i].aPairs[k].translation);
                
                printf("--------------------------------\n");
                // Increment no. of duplicate pairs found
                nFound++;
            }
        }
    }
    return nFound;
}

/*
    Function for adding a pair to an entry
        @param aEntries - array of Entries
        @param nCount - no. of entries in database
        @param nPairCount - no. of pairs in the entry that is being updated
        @param sLanguage - Language to be checked
        @param sWord - Word to be checked
*/
void addPair(entry aEntries[], int nIndex, int nPairCount, str sLanguage, str sWord)
{
    // Add a pair to designated entry
    strcpy(aEntries[nIndex].aPairs[nPairCount].language, sLanguage);
    strcpy(aEntries[nIndex].aPairs[nPairCount].translation, sWord);

    // Update no. of pairs
    aEntries[nIndex].nPairs += 1;

    // If current entry reached max no. of pairs, notify user
    if (aEntries[nIndex].nPairs >= 10)
        printf("Entry has reached the maximum no. of language-translation pairs");
    
}

/*
    Function for ACTUALLY creating the entry itself (separate from addEntry)
        @param aEntries - array of Entries
        @param pCount - pointer to no. of entries in database
        @param sLanguage - Language to be checked
        @param sWord - Word to be checked

    it will return cRepeat and it might be confusing as to why (i'll try to add an explanation at some point)
*/  
char makeNewEntry(entry aEntries[], int *pCount, str sLanguage, str sWord)
{
    char cRepeat, newEntry;
    int nFound, pairCount = 0;

    // Create new entry, add the pair to it, and update no. of entries
    addPair(aEntries, *pCount, aEntries[*pCount].nPairs, sLanguage, sWord);
    *pCount += 1;

    // Let user input more pairs in the SAME entry
    do 
    {
        printf("Do you want to input another pair? (Y/N) ");
        scanf(" %c", &cRepeat);
        
        if (cRepeat == 'Y' || cRepeat == 'y')
        {
            inputPair(sLanguage, sWord);

            // Check if pair already exists in database
            nFound = pairExists(aEntries, *pCount, sLanguage, sWord);

            // If new pair does not exist elsewhere
            if (!nFound)
                // Add to aEntries[nCount-1] because no. of entries was already incremented earlier
                // aEntries[(*pCount)-1].nPair indicates the index where we'll insert the pair
                addPair(aEntries, (*pCount)-1, aEntries[(*pCount)-1].nPairs, sLanguage, sWord);

            // If new pair DOES exist elsewhere
            else
            {
                printf("Pair already exists, is this a new entry? (Y/N) ");
                scanf(" %c", &newEntry);

                // Just call the function itself again
                // cRepeat takes the value of makeNewEntry to indicate when to stop making new entries
                // this acts as a sort of base case under the concept of recursion
                if (newEntry == 'Y' || newEntry == 'y')
                    cRepeat = makeNewEntry(aEntries, pCount, sLanguage, sWord);
                else
                {
                    printf("Returning to menu...\n\n");
                    return cRepeat;
                }
            }
        }

    // While user wants to place new pairs AND while max no. of pairs hasn't been reached
    } while ((cRepeat == 'Y' || cRepeat == 'y') && aEntries[*pCount-1].nPairs < 10);

    if (aEntries[*pCount-1].nPairs >= 10)
        printf("MAX. NO. OF PAIRS REACHED!\n");

    return cRepeat;
}

/*
    Adds a language-translation pair to an existing entry
*/
void addTranslation(entry aEntries[], int nCount)
{
    char cRepeat;
    str sLanguage, sWord;
    int i, nIndex, nPairs;
    int aDuplicates[ENTRIES] = {}; // stores indexes of entries that contain the pair being searched
    
    // Ask user for pair to search for
    printf("----- Input pair to search ----\n");
    inputPair(sLanguage, sWord);

    printf("\n---- FINDING PAIR ----\n");

    // pairExists will return no. of duplicate pairs
    nPairs = pairExists(aEntries, nCount, sLanguage, sWord);
    
    // will add a do while loop here (wait why, past self?)

    // If no existing entry contains indicated pair
    if (!nPairs)
    {
        printf("An entry with this pair does not exist. Use the Add Entry option to create a new entry with this pair.\n\n");
        return;
    }

    // If exactly one entry contains this pair
    else if (nPairs == 1)
    {
        // find the entry that contains the searched pair
        search(aEntries, nCount, sLanguage, sWord, aDuplicates);

        // add a new pair to this entry by searching aDuplicates for a non-zero value, indicating a duplicate pair was found there
        // could still be optimized
        for (nIndex = 0; nIndex < nCount; nIndex++)
        {
            if (aDuplicates[nIndex])
            {
                printf("\nPair was found at entry no. %d\n", nIndex + 1);
                printf("\nInput new pair to be added to this entry\n"); // what if user inputs something already in the entry?
                do
                {
                    inputPair(sLanguage, sWord);

                    // add pair to the entry, nIndex will locate position of entry
                    // aEntries[nIndex].nPair will act as an index for the new pair's position (current no. of pairs = index to be inserted at)
                    addPair(aEntries, nIndex, aEntries[nIndex].nPairs, sLanguage, sWord);
                    printf("Successfully added language-translation pair!\n");    
                    
                    // If haven't reached max no. of pairs
                    if (aEntries[nIndex].nPairs < 10)
                    {
                        printf("Do you want to input another pair? (Y/N) ");
                        scanf(" %c", &cRepeat);
                    }
                } while ((cRepeat == 'Y' || cRepeat == 'y') && aEntries[nIndex].nPairs < 10);
                return; // so that any succeeding entries don't need to be checked
            }
            
        }
    }

    // pair was found in more than one entry
    else 
    {
        // aDuplicates has the positions of all the entries containing the searched pair
        search(aEntries, nCount, sLanguage, sWord, aDuplicates);
        
        // After showing the user all the duplicate entries, ask where to add a new pair
        printf("\nWhich entry do you want to add to?\n");
        scanf("%d", &nIndex);
        nIndex--; // make sure nIndex matches array positioning

        // Input is valid if aDuplicates found the searched pair at that index inputted by the user
        if (aDuplicates[nIndex])            
        {
            printf("\nInput new pair to be added to this entry\n"); // what if user inputs something already in the entry? 
            do
            {
                inputPair(sLanguage, sWord);
                addPair(aEntries, nIndex, aEntries[nIndex].nPairs, sLanguage, sWord);
                printf("Successfully added language-translation pair!\n");

                printf("Do you want to input another pair? (Y/N) ");
                scanf(" %c", &cRepeat);
            } while ((cRepeat == 'Y' || cRepeat == 'y') && aEntries[nIndex].nPairs < 10);
        }
        else
            printf("Invalid entry, pair was not found there.\n"); // in future, can add a loop to let user input again for better UX.
    }
}

/*
    Finds entries that contain the language-translation pair being searched for
        @param aEntries - array of Entries
        @param nCount - no. of entries in database
        @param sLanguage - Language to be checked
        @param sWord - Word to be checked
        @param aDuplicates - array that keeps track of entries that contain the pair
*/
void search(entry aEntries[], int nCount, str sLanguage, str sWord, int aDuplicates[])
{
    int i, j;
    
    // Go through each entry
    for (i = 0; i < nCount; i++)
        // Go through each pair per entry and record any entries containing the pair being searched for
        for  (j = 0; j < PAIRS; j++)
            // Checks language case-insensitively, checks word case-sensitively
            if (strcasecmp(sLanguage, aEntries[i].aPairs[j].language) == 0
                && strcmp(sWord, aEntries[i].aPairs[j].translation) == 0)
                // Record no. of entry if duplicate pair is found here
                aDuplicates[i] = i + 1;
    printf("{");
    for (i = 0; i < nCount; i++)
        printf(" %d ", aDuplicates[i]);
    printf("}\n");
}


// Function for displaying all entries in database
void displayEntries(entry aEntries[], int nCount)
{
    int i, j;

    printf("\n--------------------------------\n");
    printf("%d entries in the database\n", nCount);
    printf("--------------------------------\n\n");

    for (i = 0; i < nCount; i++)
    {
        printf("ENTRY NO. %d\n\n", i + 1, aEntries[i].nPairs);
        for (j = 0; j < aEntries[i].nPairs; j++)
            printf("Pair No. %d || Language: %s | Translation: %s\n", j + 1, aEntries[i].aPairs[j].language, aEntries[i].aPairs[j].translation);
        printf("--------------------------------\n\n");
    }
    printf("\n--------------------------------\n\n");
}