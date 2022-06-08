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
int search(entry*, int, str, str, int*);
void modifyEntry(entry*, int);
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
    printf("\n------------Main Menu-----------");
    printf("\n  [1] Manage Data Menu");
    printf("\n  [2] Language Tools Menu");
    printf("\n  [3] Exit");
    printf("\n\n");
}

void
displayMDMenu()
{
    printf("\n---------Manage Data Menu--------");
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
    printf("\n-------Language Tools Menu------");
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
        case 3:
            modifyEntry(aEntries, *pCount);
            break;
        case 6:
            displayEntries(aEntries, *pCount);
            break;

        // Exits MD Menu
        case 11:
            printf("Exiting to Main Menu...\n");
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
                printf("Found pair at entry no. %d\n\n", i + 1);
                
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
    Function for ACTUALLY creating the entry itself (separate from addEntry)
        @param aEntries - array of Entries
        @param pCount - pointer to no. of entries in database
        @param sLanguage - Language to be checked
        @param sWord - Word to be checked

    it will return cRepeat and it might be confusing as to why (i'll try to add an explanation at some point)
*/  
char makeNewEntry(entry aEntries[], int *pCount, str sLanguage, str sWord)
{
    // create  a sort function
    char cRepeat, newEntry;
    int nFound;

    // Create new entry, add the pair to it, and update no. of entries
    addPair(aEntries, *pCount, aEntries[*pCount].nPairs, sLanguage, sWord);
    *pCount += 1;

    // Let user input more pairs in the SAME entry
    do 
    {
        do
        {
            printf("Do you want to input another pair? (Y/N) ");
            scanf(" %c", &cRepeat);
            if (cRepeat != 'Y' && cRepeat != 'y' && cRepeat != 'N' && cRepeat != 'n')
                printf("Invalid input. Please try again.\n");
        } while (cRepeat != 'Y' && cRepeat != 'y' && cRepeat != 'N' && cRepeat != 'n'); 

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

    return cRepeat;
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
        printf("Entry has reached the maximum no. of language-translation pairs\n");
    
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

    printf("\n---- SEARCHING FOR PAIR ----\n");

    // pairExists will return no. of duplicate pairs
    nPairs = pairExists(aEntries, nCount, sLanguage, sWord);
    
    // Pair was NOT found in ANY entry
    if (!nPairs)
    {
        printf("An entry with this pair does not exist. Use the Add Entry option to create a new entry with this pair.\n\n");
        return;
    }

    // Pair was found in exactly ONE entry
    else if (nPairs == 1)
    {
        // find the entry that contains the searched pair
        nIndex = search(aEntries, nCount, sLanguage, sWord, aDuplicates);

        // If entry already has 10 pairs, can't add anymore
        if (aEntries[nIndex].nPairs >= 10)
        {
            printf("This entry already has 10 entries. Returning to Manage Data Menu...\n");
            return;
        }

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
                do
                {
                    printf("Do you want to input another pair? (Y/N) ");
                    scanf(" %c", &cRepeat);
                    if (cRepeat != 'Y' && cRepeat != 'y' && cRepeat != 'N' && cRepeat != 'n')
                        printf("Invalid input. Please try again.\n");
                } while (cRepeat != 'Y' && cRepeat != 'y' && cRepeat != 'N' && cRepeat != 'n');     
            }
        } while ((cRepeat == 'Y' || cRepeat == 'y') && aEntries[nIndex].nPairs < 10);
        return; // so that any succeeding entries don't need to be checked
    
    }

    // Pair was found in more than one entry
    else 
    {
        // Store which entries contain the pair
        for (i = 0; i < nPairs; i++)
        {
            nIndex = search(aEntries, nCount, sLanguage, sWord, aDuplicates);

            // Store the entry no. in its corresponding index (e.x., if 2nd contains pair, then aDuplicates[1] = 2)
            aDuplicates[nIndex] = nIndex + 1;
        }

        // After showing the user all the duplicate entries, ask where to add a new pair
        printf("\nWhich entry do you want to add to? ");
        scanf("%d", &nIndex);
        nIndex--; // make sure nIndex matches array positioning

        // Input is valid if aDuplicates found the searched pair at that index inputted by the user and entry has less than 10 pairs
        if (aDuplicates[nIndex] && aEntries[nIndex].nPairs < 10)            
        {
            printf("\nInput new pair to be added to this entry\n"); // what if user inputs something already in the entry? 
            do
            {
                inputPair(sLanguage, sWord);
                addPair(aEntries, nIndex, aEntries[nIndex].nPairs, sLanguage, sWord);
                printf("Successfully added language-translation pair!\n");

                do
                {
                    printf("Do you want to input another pair? (Y/N) ");
                    scanf(" %c", &cRepeat);
                    if (cRepeat != 'Y' && cRepeat != 'y' && cRepeat != 'N' && cRepeat != 'n')
                        printf("Invalid input. Please try again.\n");
                } while (cRepeat != 'Y' && cRepeat != 'y' && cRepeat != 'N' && cRepeat != 'n');   
            } while ((cRepeat == 'Y' || cRepeat == 'y') && aEntries[nIndex].nPairs < 10);
        }
        // If desired entry contains 10 pairs already
        else if (aEntries[nIndex].nPairs >= 10)
            printf("This entry already has 10 entries. Returning to Manage Data Menu...\n");
        // If user inputs an entry index that DOESN'T contain the inputted pair
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
    returns index of entry where language-translation pair was found, -1 if otherwise
*/
int search(entry aEntries[], int nCount, str sLanguage, str sWord, int aDuplicates[])
{
    int i, j;
    
    // Go through each entry
    for (i = 0; i < nCount; i++)
        // Go through each pair per entry and record any entries containing the pair being searched for
        for  (j = 0; j < PAIRS; j++)
            // Checks language case-insensitively, checks word case-sensitively
            // and check if pair has already been found at this entry (aDuplicates[i] = 0 we haven't recorded the entry that contains the pair)
            if (strcasecmp(sLanguage, aEntries[i].aPairs[j].language) == 0
                && strcmp(sWord, aEntries[i].aPairs[j].translation) == 0 && !aDuplicates[i])
                    // Return the index of the entry where the pair was found
                    return i;
    return -1;
}


void modifyEntry(entry aEntries[], int nCount)
{
    char cModPair, cRepeat;
    str sLanguage, sWord;
    int i, nEntryChoice = 0, nPairChoice;

    do
    {
        // Display entries first before asking user which entry to modify
        if (nEntryChoice == 0) 
        {
            printf("\nOpening list of entries... Please exit (X/x) the list before selecting an entry to modify\n");
            displayEntries(aEntries, nCount);
        }

        // If user inputs 0, the conditional statements below are skipped
        printf("Which entry do you wish to modify? Press 0 to view the list of entries again: ");
        scanf("%d", &nEntryChoice);

        // If user inputs a valid entry (i.e., greater than 1 but within no. of entries)
        if (nEntryChoice >= 1 && nEntryChoice <= nCount)
        {
            // Adjust nEntryChoice for array-indexing
            nEntryChoice--;

            // Let user modify at least one time, but allow them to modify more than once
            do
            {
                // Show all information and pairs for selected entry
                printf("\nModifying this entry:\n");
                printf("--------------------------------\n");
                printf("Entry No. %d with %d pair/s\n\n", nEntryChoice + 1, aEntries[nEntryChoice].nPairs);
                for (i = 0; i < aEntries[nEntryChoice].nPairs; i++)
                    printf("Pair No. %d || Language: %s | Translation: %s\n", i + 1, aEntries[nEntryChoice].aPairs[i].language, aEntries[nEntryChoice].aPairs[i].translation);
                printf("--------------------------------\n");

                // Prompt user for pair to modify (add feature to let them cancel?)
                printf("\nWhich pair do you wish to modify: ");
                scanf("%d", &nPairChoice);

                // If user selected a valid pair within selected entry
                if (nPairChoice >= 1 &&  nPairChoice <= aEntries[nEntryChoice].nPairs)
                {
                    // Adjust nPairChoice for array-indexing
                    nPairChoice--;

                    printf("Modify the language or translation? ('L' - language | 'T' - translation | 'X' - Exit): ");
                    scanf(" %c", &cModPair);
                    
                    // MAKE THIS SECTION LESS REDUNDANT
                    switch (cModPair)
                    {
                        case 'L':
                        case 'l':
                            printf("Input new language to replace \"%s\": ", aEntries[nEntryChoice].aPairs[nPairChoice].language);
                            scanf("%s", sLanguage);
                            // add checking if string is valid? (i.e., at least 1 char)
                            printf("Replacing \"%s\" with \"%s\"...\n\n", aEntries[nEntryChoice].aPairs[nPairChoice].language, sLanguage);
                            strcpy(aEntries[nEntryChoice].aPairs[nPairChoice].language, sLanguage);
                            // Ask user if they wish to modify more pairs within the entry
                            do
                            {
                                printf("Modify another pair within entry? (Y/N): ");
                                scanf(" %c", &cRepeat);
                            } while (cRepeat != 'Y' && cRepeat != 'y' && cRepeat != 'N' && cRepeat != 'n');
                            break;
                        
                        case 'T':
                        case 't':
                            printf("Input new word to replace \"%s\": ", aEntries[nEntryChoice].aPairs[nPairChoice].translation);
                            scanf("%s", sWord);
                            // add checking if string is valid? (i.e., at least 1 char)
                            printf("Replacing \"%s\" with \"%s\"...\n\n", aEntries[nEntryChoice].aPairs[nPairChoice].translation, sWord);
                            strcpy(aEntries[nEntryChoice].aPairs[nPairChoice].translation, sWord);
                            // Ask user if they wish to modify more pairs within the entry
                            do
                            {
                                printf("Modify another pair within entry? (Y/N): ");
                                scanf(" %c", &cRepeat);
                            } while (cRepeat != 'Y' && cRepeat != 'y' && cRepeat != 'N' && cRepeat != 'n');
                            break;

                        case 'X':
                        case 'x':
                            printf("\nReturning to Main Menu...\n\n");
                            break;
                            
                        default:
                            printf("\nInvalid input. Returning to Main Menu...\n\n");
                            break;
                    }
                }
                else
                    printf("Invalid pair. Returning to Manage Data Menu...\n");
    
            } while (cRepeat == 'Y' || cRepeat == 'y');

            // Revert nEntryChoice to original input for loop checking.
            nEntryChoice++;
        }
        // If an invalid entry is selected
        else if (nEntryChoice < 0 || nEntryChoice > nCount)
            printf("Invalid entry. Returning to Manage Data Menu...\n");

    } while (nEntryChoice == 0);
}

// Function for displaying all entries in database
void displayEntries(entry aEntries[], int nCount)
{
    char ch = 'N';
    int i = 0, j = 0;
    printf("\n--------------------------------\n");
    printf("%d entries in the database\n", nCount);
    printf("--------------------------------\n\n");

    // While there's entries to show and while user hasn't exited function
    while (i < nCount && i >= 0 && (ch == 'N' || ch == 'n' || ch == 'P' || ch == 'p'))
    {
        // Print the entry and its pairs
        printf("--------------------------------\n");
        printf("Entry No. %d with %d pair/s\n\n", i + 1, aEntries[i].nPairs);
        for (j = 0; j < aEntries[i].nPairs; j++)
            printf("Pair No. %d || Language: %s | Translation: %s\n", j + 1, aEntries[i].aPairs[j].language, aEntries[i].aPairs[j].translation);
        printf("--------------------------------\n");

        // GUI printing
        // If only one entry
        if (nCount == 1)
            printf("              Exit              \n");
        // If currently at first of multiple entries
        else if (i == 0 && nCount > 1)
            printf("                Exit        Next\n");
        // If at last of multiple entries
        else if (i == nCount - 1)
            printf("Previous        Exit            \n");
        // If not at last or first of multiple entries
        else
            printf("Previous        Exit        Next\n");

        // Section for the "page-turning"-esque specification.
        do
        {
            scanf(" %c", &ch);
            if (ch == 'N' || ch == 'n')
                i++;
            else if (ch == 'P' || ch == 'p')
                i--;
            else if (ch == 'X' || ch == 'x')
            {
                printf("Exiting...\n\n");
                return;
            }
            
            if (ch != 'N' && ch != 'n' && ch != 'P' && ch != 'p' && ch != 'X' && ch != 'x')
                printf("Invalid input.\n");

        } while (ch != 'N' && ch != 'n' && ch != 'P' && ch != 'p' && ch != 'X' && ch != 'x');
    }
}