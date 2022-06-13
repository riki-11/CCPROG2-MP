#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "manage.h"

// Function for setting the no. of pairs for all entries to 0
void initDatabase(entry aEntries[])
{
    int i;
    for (i = 0; i < MAX_ENTRIES; i++)
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
        case 4:
            deleteEntry(aEntries, pCount);
            break;
        case 5:
            deleteTranslation(aEntries, pCount);
            break;
        case 6:
            displayAllEntries(aEntries, *pCount);
            break;
        case 7:
            searchWord(aEntries, *pCount);
            break;
        case 8:
            searchTranslation(aEntries, *pCount);
            break;
        case 9:
            export(aEntries, *pCount);
            break;
        case 10:
            import(aEntries, pCount);
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
        @param aEntries - array of entryTag structs
        @param pCount - pointer to no. of entries in database
*/
void addEntry(entry aEntries[], int *pCount)
{
    str sLanguage, sWord;
    char newEntry;
    int nPairFound = 0;

    // Try to add a new entry
    inputPair(sLanguage, sWord);

    // still need input validation (no empty strings)

    // Check if pair already exists in database of entries
    nPairFound = pairExists(aEntries, *pCount, sLanguage, sWord);

    // Check first if max capacity of entries has been reached.
    if (*pCount < MAX_ENTRIES)
    {
        // If pair does NOT exist yet in any entry
        if (!nPairFound) // might have to make this nPairFound < 0
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
    else
    printf("\nDatabase has reached full capacity of 150 entries. "
           "Delete entries using Delete Entry to continue adding entries."
           "\nReturning to Manage Data Menu...\n");
}

/*
    Function for checking if pair to be added already exists in database
        @param aEntries - array of entryTag structs in database
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
        for  (j = 0; j < MAX_PAIRS; j++)
        {
            // Checks language and word case-sensitively
            if (strcmp(sLanguage, aEntries[i].aPairs[j].language) == 0
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
        @param aEntries - array of entryTag structs
        @param pCount - pointer to no. of entries in database
        @param sLanguage - Language to be checked
        @param sWord - Word to be checked

    it will return cRepeat and it might be confusing as to why (i'll try to add an explanation at some point)
*/  
char makeNewEntry(entry aEntries[], int *pCount, str sLanguage, str sWord)
{
    char cRepeat, newEntry;
    int nPairFound;

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
            nPairFound = pairExists(aEntries, *pCount, sLanguage, sWord);

            // If new pair does not exist elsewhere
            if (!nPairFound)
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
                // this acts as a sort of base case since makeNewEntry implements recursion
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
        @param aEntries - array of entryTag structs
        @param nCount - no. of entries in database
        @param nPairCount - no. of pairs in the entry that is being updated
        @param sLanguage - Language to be checked
        @param sWord - Word to be checked
    returns 1 if successfully added a pair and 0 if otherwise
*/
void addPair(entry aEntries[], int nIndex, int nPairCount, str sLanguage, str sWord)
{
    int i;

    // Before adding a pair, check first if the language already exists in this specific entry
    // Go through each language in entry
    for (i = 0; i < nPairCount; i++)
        // Check if the language already exists, irrespective of case
        if (strcasecmp(aEntries[nIndex].aPairs[i].language, sLanguage) == 0)
        {
            printf("\nThe %s language already exists in this entry!\n", sLanguage);
            return;
        }

    // If language was not found, add pair to entry
    // Add a pair to designated entry
    strcpy(aEntries[nIndex].aPairs[nPairCount].language, sLanguage);
    strcpy(aEntries[nIndex].aPairs[nPairCount].translation, sWord);

    // Update no. of pairs
    aEntries[nIndex].nPairs += 1;

    printf("\nSuccessfully added language-translation pair!\n\n");  

    // If current entry reached max no. of pairs, notify user
    if (aEntries[nIndex].nPairs >= 10)
        printf("\nEntry has reached the maximum no. of language-translation pairs\n");
}


/*
    Adds a language-translation pair to an existing entry
*/
void addTranslation(entry aEntries[], int nCount)
{
    char cRepeat;
    str sLanguage, sWord;
    int i, nIndex, nPairs;
    int aDuplicates[MAX_ENTRIES] = {}; // stores indexes of entries that contain the pair being searched
    
    // Ask user for pair to search for
    printf("\n----- Input pair to search ----\n");
    inputPair(sLanguage, sWord);

    printf("\n---- SEARCHING FOR PAIR ----\n");

    // pairExists will return no. of duplicate pairs
    nPairs = pairExists(aEntries, nCount, sLanguage, sWord);
    
    // If pair was NOT found in ANY entry
    if (!nPairs)
    {
        printf("An entry with this pair does not exist. Use the Add Entry option to create a new entry with this pair.\n\n");
        return;
    }

    // If pair was found in exactly ONE entry
    else if (nPairs == 1)
    {
        // find the entry that contains the searched pair
        nIndex = searchPair(aEntries, nCount, sLanguage, sWord, aDuplicates);

        // If entry already has 10 pairs, can't add anymore
        if (aEntries[nIndex].nPairs >= 10)
        {
            printf("\nThis entry already has 10 entries. Returning to Manage Data Menu...\n");
            return;
        }

        printf("\nInput new pair to be added to this entry\n");
        do
        {
            inputPair(sLanguage, sWord);

            // add pair to the entry, nIndex will locate position of entry
            // aEntries[nIndex].nPair will act as an index for the new pair's position (current no. of pairs = index to be inserted at)
            addPair(aEntries, nIndex, aEntries[nIndex].nPairs, sLanguage, sWord);
            
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
            nIndex = searchPair(aEntries, nCount, sLanguage, sWord, aDuplicates);

            // Store the entry no. in its corresponding index (e.x., if 2nd entry contains pair, then aDuplicates[1] = 2)
            aDuplicates[nIndex] = nIndex + 1;
        }

        // After showing the user all the duplicate entries, ask where to add a new pair
        printf("\nWhich entry do you want to add to? ");
        scanf("%d", &nIndex);
        nIndex--; // make sure nIndex matches array positioning

        // Input is valid if aDuplicates found the searched pair at that index inputted by the user and entry has less than 10 pairs
        if (aDuplicates[nIndex] && aEntries[nIndex].nPairs < 10)            
        {
            printf("\nInput new pair to be added to this entry\n"); 
            do
            {
                inputPair(sLanguage, sWord);
                addPair(aEntries, nIndex, aEntries[nIndex].nPairs, sLanguage, sWord);
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
        @param aEntries - array of entryTag structs
        @param nCount - no. of entries in database
        @param sLanguage - Language to be checked
        @param sWord - Word to be checked
        @param aDuplicates - array that keeps track of entries that contain the pair
    returns index of entry where language-translation pair was found, -1 if otherwise
*/
int searchPair(entry aEntries[], int nCount, str sLanguage, str sWord, int aDuplicates[])
{
    int i, j;
    
    // Go through each entry
    for (i = 0; i < nCount; i++)
        // Go through each pair per entry and record any entries containing the pair being searched for
        for  (j = 0; j < MAX_PAIRS; j++)
            // Checks language and word case-sensitively
            // and check if pair has already been found at this entry (aDuplicates[i] = 0 we haven't recorded the entry that contains the pair)
            if (strcmp(sLanguage, aEntries[i].aPairs[j].language) == 0
                && strcmp(sWord, aEntries[i].aPairs[j].translation) == 0 
                && !aDuplicates[i])
                    // Return the index of the entry where the pair was found
                    return i;
    return -1;
}

/*
    Modifies language-translation pair/s of one database entry.
        @param aEntries - array of entryTag structs
        @param nCount - no. of entries in database
*/
void modifyEntry(entry aEntries[], int nCount)
{
    char cModPair, cRepeat = ' ';
    str sLanguage, sWord;
    int nEntryChoice = 0, nPairChoice;

    do
    {
        // Display entries first before asking user which entry to modify
        if (nEntryChoice == 0) 
        {
            printf("\nOpening list of entries... Please exit (X/x) the list before selecting an entry to modify\n");
            displayAllEntries(aEntries, nCount);
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
                displayEntry(&aEntries[nEntryChoice], nEntryChoice, aEntries[nEntryChoice].nPairs);

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
                                printf("\nModify another pair within entry? (Y/N): ");
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
                                printf("\nModify another pair within entry? (Y/N): ");
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

/*
    Allows user to delete one entry in the database
        @param aEntries - array of Entry structs
        @param pCount - pointer to no. of entries in database
*/
void deleteEntry(entry aEntries[], int *pCount)
{
    int i, nEntryChoice = 0;
    entry blankEntry;
    blankEntry.nPairs = 0;

    do
    {
        // Display entries first before asking user which entry to delete
        if (nEntryChoice == 0) 
        {
            printf("\nOpening list of entries... Please exit (X/x) the list before selecting an entry to delete\n");
            displayAllEntries(aEntries, *pCount);
        }

        // If user inputs 0, the conditional statements below are skipped
        printf("Which entry do you wish to delete? Press 0 to view the list of entries again: ");
        scanf("%d", &nEntryChoice);

        // If user inputs a valid entry (i.e., greater than 1 but within no. of entries)
        if (nEntryChoice >= 1 && nEntryChoice <= *pCount)
        {
            printf("\nDeleting entry no. %d\n", nEntryChoice);
            // Adjust nEntryChoice for array-indexing
            nEntryChoice--;

            // Delete an entry by copying the values of the succeeding entry into this one (using memcpy)
            // Also Move succeeding entries one index to the left accordingly
            for (i = nEntryChoice; i < (*pCount) - 1; i++)
                memcpy(&(aEntries[i]), &(aEntries[i + 1]), sizeof(entry));
            
            // Clear last entry in database of its data ('i' will be equal to index of last existing entry in database)
            memcpy(&(aEntries[i]), &blankEntry, sizeof(entry));

            // update no. of entries in database
            *pCount -= 1;

            // Revert nEntryChoice to original input for loop checking.
            nEntryChoice++;

            printf("\nSuccessfully deleted entry!\n");
        }
        else if (nEntryChoice < 0 || nEntryChoice > *pCount)
            printf("Invalid entry. Returning to Manage Data Menu...\n");

    } while (nEntryChoice == 0);
}

/*
    Lets user delete one or more language-translation pairs within entry.
*/
void deleteTranslation(entry aEntries[], int *pCount)
{
    char cRepeat = ' ';
    int i, nEntryChoice = 0, nPairChoice, nPairCount;
    entry blankEntry;
    blankEntry.nPairs = 0;

    do
    {
        // Display entries first before asking user which entry to delete
        if (nEntryChoice == 0) 
        {
            printf("\nOpening list of entries... Please exit (X/x) the list before selecting an entry\n");
            displayAllEntries(aEntries, *pCount);
        }

        // If user inputs 0, the conditional statements below are skipped
        printf("Select the entry containing the pair you wish to delete. Press 0 to view the list of entries again: ");
        scanf("%d", &nEntryChoice);

        // If user inputs a valid entry (i.e., greater than 1 but within no. of entries)
        if (nEntryChoice >= 1 && nEntryChoice <= *pCount)
        {
            // Adjust nEntryChoice for array-indexing
            nEntryChoice--;

            // Let user delete at least one time, but allow them to delete more than once
            do
            {
                nPairCount = aEntries[nEntryChoice].nPairs;

                // Show all information and pairs for selected entry
                printf("\nDeleting a pair within this entry:\n");
                displayEntry(&aEntries[nEntryChoice], nEntryChoice + 1, nPairCount);

                // Prompt user for pair to delete (add feature to let them cancel?)
                printf("\nWhich pair do you wish to delete: ");
                scanf("%d", &nPairChoice);
                
                // If user selected a valid pair within selected entry
                if (nPairChoice >= 1 && nPairChoice <= nPairCount)
                {
                    // Adjust nPairChoice for array-indexing
                    nPairChoice--;
                    
                    // Delete a pair by moving the succeeding pairs one index down (or to the left)
                    for (i = nPairChoice; i < nPairCount - 1; i++)
                    {
                        strcpy(aEntries[nEntryChoice].aPairs[i].language, aEntries[nEntryChoice].aPairs[i+1].language);
                        strcpy(aEntries[nEntryChoice].aPairs[i].translation, aEntries[nEntryChoice].aPairs[i+1].translation);
                    }       

                    // Clear last pair in entry
                    strcpy(aEntries[nEntryChoice].aPairs[i].language, "");       
                    strcpy(aEntries[nEntryChoice].aPairs[i].translation, "");

                    // Update no. of pairs in entry
                    aEntries[nEntryChoice].nPairs -= 1;             
                    
                    printf("\nSuccessfully deleted pair!\n");

                    // If there are pairs remaining, ask user if they wish to delete more
                    if (aEntries[nEntryChoice].nPairs > 0)
                    {
                        do
                        {
                            printf("\nDelete another pair within entry? (Y/N): ");
                            scanf(" %c", &cRepeat);
                        } while (cRepeat != 'Y' && cRepeat != 'y' && cRepeat != 'N' && cRepeat != 'n');        
                    }
                    // If no more pairs remain, delete the entry
                    else
                    {
                        printf("\nNo more pairs left in entry no. %d, Deleting entry...\n", nEntryChoice + 1);
                        memcpy(&aEntries[nEntryChoice], &blankEntry, sizeof(entry));
                        *pCount -= 1;
                        cRepeat = 'N';
                    }
                }
                // If invalid pair selected
                else
                {
                    printf("Invalid pair. Returning to Manage Data Menu...\n");
                    cRepeat = 'N';
                }            
            } while (cRepeat == 'Y' || cRepeat == 'y');
           
            // Revert nEntryChoice to original input for loop checking.
            nEntryChoice++; 
        }
        else if (nEntryChoice < 0 || nEntryChoice > *pCount)
            printf("Invalid entry. Returning to Manage Data Menu...\n");

    } while (nEntryChoice == 0);
}

/*
    Passing an entry into this function will sort the pairs alphabtically by language using Bubble Sort
        @param *Entry is a pointer to an entryTag struct, since we need to modify the values
        @param nPairCount is no. of pairs within this entry
*/
void sortEntry(entry *Entry, int nPairCount)
{
    int i ,j, swapped;
    str s1, s2, sTemp;

    /* 
        Worst case, the languages are arranged backwards, so run the algorithm as many times as there are pairs in the entry.
        Every time an iteration of the outermost for loop is done, at least 1 language will be sorted accordingly.
    */
    for (i = 0; i < nPairCount; i++)
    {
        // Variable for checking whether we found any values that needed to be swapped
        swapped = 0;

        // Go through each language until the last non-sorted language in the entry
        for (j = 0; j < nPairCount - i - 1; j++)
        {
            strcpy(s1, Entry->aPairs[j].language); // Current language
            strcpy(s2, Entry->aPairs[j+1].language); // Language to be compared to

            // if current language comes after (strcmp returns an int > 0) the language beside it, swap their positions
            if (strcmp(s1, s2) > 0)
            {
                // Swapping by changing the values within the entry
                strcpy(Entry->aPairs[j].language, s2);
                strcpy(Entry->aPairs[j+1].language, s1);

                // Also transfer the translations accordingly
                strcpy(sTemp, Entry->aPairs[j].translation); // translation 1
                strcpy(Entry->aPairs[j].translation, Entry->aPairs[j+1].translation); // put translation 2 in translation 1's place
                strcpy(Entry->aPairs[j+1].translation, sTemp); // put translation 1 in translation 2's place

                swapped = 1;
            }
        }

        // If no values were swapped, it means entry is already sorted, so no need to keep going through the entry
        if (swapped == 0)
            return;
    }
}

/* 
    Function for displaying a SINGLE entry
        @param *Entry - pointer to entry to be displayed
        @param nEntryNum - number of entry being displayed
        @param nPairCount - no. of pairs within entry
*/
void displayEntry(entry *Entry, int nEntryNum, int nPairCount)
{
    int i;
    sortEntry(Entry, nPairCount); // Sort the entry before displaying it

    // Print entry and its pairs after sorting
    printf("--------------------------------\n");
    printf("Entry No. %d with %d pair/s\n\n", nEntryNum, Entry->nPairs);
    for (i = 0; i < nPairCount; i++)
        printf("Pair No. %d || Language: %s | Translation: %s\n", i + 1, Entry->aPairs[i].language, Entry->aPairs[i].translation);
    printf("--------------------------------\n");
}

// Function for displaying all entries in database
void displayAllEntries(entry aEntries[], int nCount)
{
    char ch = 'N';
    int i = 0;
    printf("\n--------------------------------\n");
    printf("%d entries in the database\n", nCount);
    printf("--------------------------------\n\n");

    // While there's entries to show and while user hasn't exited the menu
    while (i < nCount && i >= 0 && (ch == 'N' || ch == 'n' || ch == 'P' || ch == 'p'))
    {
        // Print the entry and its pairs
        displayEntry(&aEntries[i], i + 1, aEntries[i].nPairs);
        
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

        // Page-turning-esque feature
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

/*
    Function for searching for specific word in database
*/
void searchWord(entry aEntries[], int nCount)
{
    str sWord;
    char ch = 'N';
    int i, j, nEntryIndex, nFound = 0;
    int aFound[MAX_ENTRIES] = {}; // stores entry no.s where sWord was found

    printf("\nSearch for a word: ");
    scanf("%s", sWord);

    // Linearly search through every entry and pair, since words are not sorted in any way.
    for (i = 0; i < nCount; i++)
        for (j = 0; j < aEntries[i].nPairs; j++)
            // If inputted word matches translation in specific pair, case-sensitively
            if (strcmp(sWord, aEntries[i].aPairs[j].translation) == 0)
            {
                // Store entry no. in aFound
                aFound[nFound] = i;
                nFound++;
            }

    if (nFound == 0)
        printf("\nWord was not found in database. Returning to Manage Data Menu...\n");
    else
    {
        printf("\nFound \"%s\" in %d entries\n", sWord, nFound);
        i = 0;
        // Display each entry stored in aFound while there are entries to show and while user hasn't exited
        while (i < nFound && i >= 0 && (ch == 'N' || ch == 'n' || ch == 'P' || ch == 'p'))
        {
            nEntryIndex = aFound[i];
            
            // Print the entry and its pairs
            displayEntry(&aEntries[nEntryIndex], nEntryIndex + 1, aEntries[nEntryIndex].nPairs);

            // GUI printing
            // If only one entry was found
            if (nFound == 1)
                printf("              Exit              \n");
            // If currently at first of multiple entries
            else if (i == 0 && nFound > 1)
                printf("              Exit          Next\n");
            // If at last of multiple entries
            else if (i == nFound - 1)
                printf("Previous        Exit            \n");
            // If not at last or first of multiple entries
            else
                printf("Previous        Exit        Next\n");

            // Page-turning-esque feature
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
}

/*
    Lets user find a specific language-translation pair
*/
void searchTranslation(entry aEntries[], int nCount)
{
    str sLanguage, sWord;
    char ch = 'N';
    int i, j, nEntryIndex, nFound = 0;
    int aFound[MAX_ENTRIES] = {}; // stores entry no.s where pair was found

    // Ask user for pair to search for
    printf("\n----- Input pair to search ----\n");
    inputPair(sLanguage, sWord);

    printf("\n---- SEARCHING FOR PAIR ----\n");

    // Linearly search through every entry and pair, since words are not sorted in any way.
    for (i = 0; i < nCount; i++)
        for (j = 0; j < aEntries[i].nPairs; j++)
            // If inputted pair matches searched pair, store index of the entry containing the pair
            if (strcmp(sLanguage, aEntries[i].aPairs[j].language) == 0
                && strcmp(sWord, aEntries[i].aPairs[j].translation) == 0)
            {
                // Store entry no. in aFound
                aFound[nFound] = i;
                nFound++;
            }

    if (nFound == 0)
        printf("\nPair was not found in database. Returning to Manage Data Menu...\n");
    else
    {
        printf("\nFound pair in %d entries\n", nFound);
        i = 0;
        // Display each entry stored in aFound while there are entries to show and while user hasn't exited
        while (i < nFound && i >= 0 && (ch == 'N' || ch == 'n' || ch == 'P' || ch == 'p'))
        {
            nEntryIndex = aFound[i];
            
            // Print the entry and its pairs
            displayEntry(&aEntries[nEntryIndex], nEntryIndex + 1, aEntries[nEntryIndex].nPairs);

            // GUI printing
            // If only one entry was found
            if (nFound == 1)
                printf("              Exit              \n");
            // If currently at first of multiple entries
            else if (i == 0 && nFound > 1)
                printf("              Exit          Next\n");
            // If at last of multiple entries
            else if (i == nFound - 1)
                printf("Previous        Exit            \n");
            // If not at last or first of multiple entries
            else
                printf("Previous        Exit        Next\n");

            // Page-turning-esque feature
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
}

/*
    Exports database of entries into a text file.
        REMEMBER HAVE TO CLEAR THE DATABASE ONCE WE EXIT TO MAIN MENU 
*/
void export(entry aEntries[], int nCount)
{
    FILE *fp;
    int i, j;
    fprintf(stdout, "\nExporting...\n");

    if ((fp = fopen("output.txt", "w"))!= NULL)
    {
        // Go through each entry and each pair and write them to the output file
        for (i = 0; i < nCount; i++)
        {
            for (j = 0; j < aEntries[i].nPairs; j++)
                fprintf(fp, "%s: %s\n", aEntries[i].aPairs[j].language, aEntries[i].aPairs[j].translation);
            
            fprintf(fp, "\n");
        }

        fclose(fp);
    }
    else
        printf("Error writing to file.\n");
}

/*
    Function for importing a text file into the current entry database
*/
void import(entry aEntries[], int *pCount)
{
    FILE *fp;
    str sFilename;
    entry newEntry, blankEntry;
    int nPairCount = 0, nNew = 0;
    char cLoad;
    char *sToken, buffer[50];
    
    printf("Input a text file containing entries you wish to import: ");
    scanf("%s", sFilename);

    if ((fp = fopen(sFilename, "r")) != NULL)
    {
        // CASE FOR WHEN CURRENT DATABASE IS EMPTY (redundant w other case for now)
        if (*pCount == 0)
        {
            // while haven't reached end of file, go through each line in the text file
            while (fgets(buffer, 50, fp) != NULL)
            {
                // Only process input while max no. of entries hasn't been reached
                if (*pCount < MAX_ENTRIES)
                {
                    // Set trailing newline to null-terminating char to ignore newlines when taking input
                    buffer[strcspn(buffer, "\n")] = '\0';

                    // If fgets buffer is a null-terminating character, a new entry was just recorded
                    if (buffer[0] == '\0')
                    {
                        *pCount += 1; // increment entry count
                        nPairCount = 0; // reset pair count for new entry
                        // Keep track of how many NEW entries have been added
                        nNew++;
                    }
                    // Record language-translation pairs until we reach a '\0' buffer to signify a new entry
                    else
                    {
                        // strtok basically separates strings based on a delimiter string, in this case, ": "  ;
                        sToken = strtok(buffer, ": ");
                        // Assuming correct formatting, first token should be language
                        if (sToken != NULL)
                            strcpy(aEntries[*pCount].aPairs[nPairCount].language, sToken);
                        
                        // Assuming correct formatting, second token should be language
                        sToken = strtok(NULL, ": ");
                        if (sToken != NULL)
                            strcpy(aEntries[*pCount].aPairs[nPairCount].translation, sToken);
                        
                        aEntries[*pCount].nPairs++; // update no. of pairs in current entry
                        nPairCount++; // update no. of pairs to serve as indexing (ERROR CHECKING IF INPUT IS WRONG?)
                    }
                }
                else
                {
                    printf("\nSuccessfully imported %d new entry/entries coming from \"%s\".\n", nNew, sFilename);
                    printf("\nDatabase has reached full capacity of 150 entries. "
                           "Delete entries using Delete Entry to continue adding entries."
                           "\nReturning to Manage Data Menu...\n");
                    return;
                }
            }

            printf("\nSuccessfully imported %d new entry/entries coming from \"%s\".\n", nNew, sFilename);
        }
        
        // If database currently contains at least 1 but less than maximum entry/entries
        else if (*pCount >= 1 && *pCount < MAX_ENTRIES)
        {
            // Initialize no. of pairs within the "entry" type structs
            newEntry.nPairs = 0;
            blankEntry.nPairs = 0;

            // Showcase each entry from the loaded text file one at a time
            while (fgets(buffer, 50, fp) != NULL)
            {
                // Set trailing newline to null-terminating char to ignore newlines when taking input
                buffer[strcspn(buffer, "\n")] = '\0';

                // A new entry is signified by a blank line in the file
                if (buffer[0] == '\0' && *pCount < MAX_ENTRIES)
                {
                    // Display the entry that's been found
                    displayEntry(&newEntry, 0, nPairCount);

                    do
                    {
                        printf("\nLoad this entry into the database?: ");
                        scanf(" %c", &cLoad);
                        if (cLoad != 'Y' && cLoad != 'y' && cLoad != 'N' && cLoad != 'n')
                            printf("Invalid input. Please try again.\n");
                    } while (cLoad != 'Y' && cLoad != 'y' && cLoad != 'N' && cLoad != 'n');
                    
                    // Load entry into corresponding position within database via memcpy
                    if (cLoad == 'Y' || cLoad == 'y')
                    {
                        printf("\nLoading entry into database...\n");

                        // Copy the input file's entry into a new entry within the database
                        memcpy(&aEntries[*pCount], &newEntry, sizeof(entry));

                        // update no. of entries in database
                        *pCount += 1;

                        // Keep track of how many NEW entries have been added
                        nNew++;
                    }
                    else
                        printf("\nMoving on to next entry...\n");

                    // erase records of old entry by using memcpy and a "blank" entry
                    memcpy(&newEntry, &blankEntry, sizeof(entry));

                    // reset pair count since a new entry will be loaded
                    nPairCount = 0;
                }
                // if no. of entries within database exceeds maximum, stop loading new entries
                else if (*pCount >= MAX_ENTRIES)
                {
                    printf("\nDatabase has reached full capacity of 150 entries. "
                           "Delete entries using Delete Entry to continue adding entries."
                           "\nReturning to Manage Data Menu...\n");
                    return;
                }

                // Record language-translation pairs until we reach a '\0' buffer to signify a new entry
                else
                {
                    sToken = strtok(buffer, ": ");
                    if (sToken != NULL)
                        strcpy(newEntry.aPairs[nPairCount].language, sToken);
                    
                    // Assuming correct formatting, second token should be language
                    sToken = strtok(NULL, ": ");
                    if (sToken != NULL)
                        strcpy(newEntry.aPairs[nPairCount].translation, sToken);

                    // Update pair count of current entry
                    newEntry.nPairs++;
                    nPairCount++;
                }
            }

            printf("\nSuccessfully imported %d new entry/entries coming from \"%s\".\n", nNew, sFilename);
        }
        else
            printf("\nDatabase has reached full capacity of 150 entries. "
            "Delete entries using Delete Entry to continue adding entries."
            "\nReturning to Manage Data Menu...\n");

        fclose(fp);
    }
    else
        printf("Error reading file.\n");

}