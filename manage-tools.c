#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "manage-tools.h"

// Displays Main Menu
void
displayMainMenu()
{
    printf("\n------------Main Menu-----------");
    printf("\n  [1] Manage Data Menu");
    printf("\n  [2] Language Tools Menu");
    printf("\n  [3] Exit");
    printf("\n\n");
}

// Displays Manage Data Menu
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

// Displays Language Data Menu
void
displayLTMenu()
{
    printf("\n-------Language Tools Menu------");
    printf("\n  [1] Identify Main Language");
    printf("\n  [2] Simple Translation");
    printf("\n  [3] Back to Main Menu");
    printf("\n\n");
}

/*  Gets Main Menu input
    @param pMMInput - stores menu navigation choice 
*/
void
getMMInput(int *pMMInput)
{
    printf("Choose menu option: ");
    scanf("%d", pMMInput);
}

/*  Gets Manage Data Menu input
    @param pMLInput - stores menu navigation choice 
*/
void
getMDInput(int *pMLInput)
{
    printf("Choose manage option: ");
    scanf("%d", pMLInput);
}

/*  Gets Language Tools Menu input
    @param pMLInput - stores menu navigation choice 
*/
void
getLTInput(int *pMLInput)
{   
    printf("Choose tool option: ");
    scanf("%d", pMLInput);
}

/*  Lets user move around the main menu
    @param nMMInput - stores menu navigation choice (Main)
    @param pMLInput - stores menu navigation choice (Manage/Tools)
    @param aEntries - array of entryTag structs
    @param pCount - pointer to no. of entries in database
    @param pInputElem - number of words in the sentence/phrase of the user input
    @param pLineElem - number of words in the line
    @param pFileWords - number of words in the source file
*/
void
switchMainMenu(int nMMInput, 
               int *pMLInput, 
               entry aEntries[], 
               int *pCount, 
               int *pInputElem, 
               int *pLineElem, 
               int *pFileWords)
{
    switch(nMMInput)
    {
        case 1:
            // keep looping through this until user enters the int 11 to exit MD Menu
            do 
            {
                displayMDMenu();
                getMDInput(pMLInput);
                switchMDMenu(*pMLInput, aEntries, pCount);
            } while (*pMLInput != 11);
            break;

        case 2:
            // keep looping through this until user enters the int 3 to exit LT Menu
            do
            {
                displayLTMenu();
                getLTInput(pMLInput);
                if (*pMLInput != 3)
                    switchLTMenu(*pMLInput, pInputElem, pLineElem, pFileWords);
            } while (*pMLInput != 3);
            break;
        default:
            printf("Invalid Input");
    }
}

/*  Function for moving around Manage Data menu
    @param nMLInput - stores menu navigation choice (Manage/Tools)
    @param aEntries - array of entryTag structs
    @param pCount - pointer to no. of entries in database
*/
void
switchMDMenu(int nMLInput, 
             entry aEntries[], 
             int *pCount)
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
        case 11:
            clearDatabase(aEntries, pCount);
            printf("\nExiting to Main Menu...\n");
            break;
        default:
            printf("\nInvalid input. Please try again.\n");
    }
}

/*  Function for moving around Language Tools menu
    @param nMLInput - stores menu navigation choice (Manage/Tools)
    @param pInputElem - number of words in the sentence/phrase of the user input
    @param pLineElem - number of words in the line
    @param pFileWords - number of words in the source file
*/
void
switchLTMenu(int nMLInput, 
             int *pInputElem, 
             int *pLineElem, 
             int *pFileWords)
{
    char strFilename[31],
         cDump;

    printf("Enter filename: ");
    scanf(" %s%c", strFilename, &cDump); //receives filename and dumps enter key
    
    switch(nMLInput)
    {
        case 1:
            identifyML(pInputElem, pLineElem, pFileWords, strFilename);
            break;
            
        case 2:
            simpleTranslation(strFilename);
            break;

        // Exits LT Menu
        case 3:
            printf("Exiting to Main Menu...\n");
            return;

        default:
            printf("Invalid input\n");
    }
}

/*  Sets the no. of pairs of all entries to 0 at beginning of program
    @param aEntries - array of entryTag structs
*/
void
initDatabase(entry aEntries[])
{
    int i;
    for (i = 0; i < MAX_ENTRIES; i++)
        aEntries[i].nPairs = 0;
}

/*  Clears all entries within database after exiting to Main Menu
    @param aEntries - array of entryTag structs
    @param pCount - pointer to no. of entries in database
*/
void
clearDatabase(entry aEntries[], 
              int *pCount)
{
    int i;
    entry blankEntry;
    blankEntry.nPairs = 0;
    
    if (*pCount >= 1)
        printf("\nClearing database...\n");

    // Replace every entry within database with a blank entry
    for (i = 0; i < *pCount; i++)
        memcpy(&aEntries[i], &blankEntry, sizeof(entry));
    
    *pCount = 0;
}

/*  Function for asking user for language-translation pair
    @param strLanguage - stores input for Language in pair
    @param strWord - stores input for Word in pair
*/
void
inputPair(str strLanguage, 
          str strWord)
{
    do 
    {
        printf("Enter Language: ");
        scanf(" %s", strLanguage);
        printf("Enter Translation: ");
        scanf(" %s", strWord);

        // Language and translation input should both have at least 1 character
        if (strcmp(strLanguage, "") == 0 || strcmp(strWord, "") == 0)
            printf("Language and Translation inputs should have at least 1 character each");
        
    } while (strcmp(strLanguage, "") == 0 || strcmp(strWord, "") == 0);
}


/*  Checks if pair to be added already exists in database
    @param aEntries - array of entryTag structs
    @param nCount - no. of entries in database
    @param strLanguage - Language to be checked
    @param strWord - Word to be checked

    returns no. of entries where specific pair is found, 0 if none
*/
int
pairExists(entry aEntries[], 
           int nCount, 
           str strLanguage, 
           str strWord)
{
    int i, j, k, nFound = 0;

    // Go through each entry
    for (i = 0; i < nCount; i++)
    {
        // Go through each pair in the entry, and print any duplicate pairs
        for  (j = 0; j < MAX_PAIRS; j++)
        {
            // Checks language and word case-sensitively
            if (strcmp(strLanguage, aEntries[i].aPairs[j].language) == 0
                && strcmp(strWord, aEntries[i].aPairs[j].translation) == 0)
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

/*  Allows user to add 1 or potentially more entries to the database
    @param aEntries - array of entryTag structs
    @param pCount - pointer to no. of entries in database
*/
void
addEntry(entry aEntries[], 
         int *pCount)
{
    str strLanguage, strWord;
    char newEntry;
    int nPairFound = 0;
    
    inputPair(strLanguage, strWord);

    // Check if pair already exists in database of entries
    nPairFound = pairExists(aEntries, *pCount, strLanguage, strWord);

    // Check if max capacity of entries has been reached
    if (*pCount < MAX_ENTRIES)
    {
        // If pair does NOT exist yet in any entry
        if (nPairFound == 0)
            makeNewEntry(aEntries, pCount, strLanguage, strWord);

        // If pair DOES exist, ask if this is a new entry
        else
        {
            printf("Pair already exists, is this a new entry? (Y/N): ");
            scanf(" %c", &newEntry);

            if (newEntry == 'Y' || newEntry == 'y')
                makeNewEntry(aEntries, pCount, strLanguage, strWord);
            else
            {
                printf("Returning to Manage Data Menu...\n\n");
                return;
            }
        }
    }
    else
        printf("\nDatabase has reached full capacity of 150 entries. "
               "Delete entries using Delete Entry to continue adding entries."
               "\nReturning to Manage Data Menu...\n");
}

/*  Function for adding a pair to an entry
    @param aEntries - array of entryTag structs
    @param nCount - no. of entries in database
    @param nPairCount - no. of pairs in the entry that is being updated
    @param strLanguage - Language to be checked
    @param strWord - Word to be checked
    returns 1 if successfully added a pair and 0 if otherwise
*/
void
addPair(entry aEntries[], 
        int nIndex, 
        int nPairCount, 
        str strLanguage, 
        str strWord)
{
    int i;

    // Before adding a pair, check first if the language already exists in this specific entry
    for (i = 0; i < nPairCount; i++)
        // Check if the language already exists
        if (strcmp(aEntries[nIndex].aPairs[i].language, strLanguage) == 0)
        {
            printf("\nThe %s language already exists in this entry!\n", strLanguage);
            return;
        }

    // If language was not found, add pair to entry
    strcpy(aEntries[nIndex].aPairs[nPairCount].language, strLanguage);
    strcpy(aEntries[nIndex].aPairs[nPairCount].translation, strWord);

    // Update no. of pairs
    aEntries[nIndex].nPairs += 1;

    printf("\nSuccessfully added language-translation pair!\n");  

    // If current entry reached max no. of pairs, notify user
    if (aEntries[nIndex].nPairs >= 10)
        printf("\nEntry has reached the maximum no. of language-translation pairs\n");
}

/*  Function for actually creating the new entry (separate from addEntry)
    @param aEntries - array of entryTag structs
    @param pCount - pointer to no. of entries in database
    @param strLanguage - Language to be checked
    @param strWord - Word to be checked

    @returns a char to act as a base case to indicate when to stop making new entries
*/  
char
makeNewEntry(entry aEntries[], 
             int *pCount, 
             str strLanguage, 
             str strWord)
{
    char cRepeat, newEntry;
    int nPairFound;

    // Create new entry, add the pair to it, and update no. of entries
    addPair(aEntries, *pCount, aEntries[*pCount].nPairs, strLanguage, strWord);
    *pCount += 1;

    // Let user input more pairs in the SAME entry
    do 
    {
        do
        {
            printf("Do you want to input another pair? (Y/N): ");
            scanf(" %c", &cRepeat);
            if (cRepeat != 'Y' && cRepeat != 'y' && cRepeat != 'N' && cRepeat != 'n')
                printf("Invalid input. Please try again.\n");
        } while (cRepeat != 'Y' && cRepeat != 'y' && cRepeat != 'N' && cRepeat != 'n'); 

        if (cRepeat == 'Y' || cRepeat == 'y')
        {
            inputPair(strLanguage, strWord);

            // Check if pair already exists in database
            nPairFound = pairExists(aEntries, *pCount, strLanguage, strWord);

            // If new pair does not exist elsewhere
            if (!nPairFound)
                // Add to aEntries[nCount-1] because no. of entries was already incremented earlier
                // aEntries[(*pCount)-1].nPair indicates the index where we'll insert the pair
                addPair(aEntries, (*pCount)-1, aEntries[(*pCount)-1].nPairs, strLanguage, strWord);

            // If new pair DOES exist elsewhere
            else
            {
                printf("Pair already exists, is this a new entry? (Y/N): ");
                scanf(" %c", &newEntry);

                /*  Just call the function itself again (concept of recursion)
                    cRepeat takes the value of makeNewEntry, so that it knows when to stop making new entries.
                    makeNewEntry will eventually return 'N' or 'n' to all the recursive function calls.
                */
                if (newEntry == 'Y' || newEntry == 'y')
                    cRepeat = makeNewEntry(aEntries, pCount, strLanguage, strWord);
                else
                {
                    printf("Returning to Manage Data Menu...\n\n");
                    return cRepeat;
                }
            }
        }

    // While user wants to place new pairs AND while max no. of pairs hasn't been reached
    } while ((cRepeat == 'Y' || cRepeat == 'y') && aEntries[*pCount-1].nPairs < 10);

    return cRepeat;
}

/*  Finds entries that contain the language-translation pair being searched for
    @param aEntries - array of entryTag structs
    @param nCount - no. of entries in database
    @param sLanguage - Language to be checked
    @param sWord - Word to be checked
    @param aDuplicates - array that keeps track of entries that contain the pair

    @returns index of entry where language-translation pair was found, -1 if otherwise
*/
int
searchPair(entry aEntries[], 
           int nCount, 
           str strLanguage, 
           str strWord, 
           int aDuplicates[])
{
    int i, j;
    
    // Go through each entry
    for (i = 0; i < nCount; i++)
        // Go through each pair per entry and record any entries containing the pair being searched for
        for  (j = 0; j < MAX_PAIRS; j++)
            // Check if pair has already been found at this entry (aDuplicates[i] = 0 we haven't recorded the entry that contains the pair)
            if (strcmp(strLanguage, aEntries[i].aPairs[j].language) == 0
                && strcmp(strWord, aEntries[i].aPairs[j].translation) == 0 
                && !aDuplicates[i])
                    // Return the index of the entry where the pair was found
                    return i;
    return -1;
}

/*  Adds a language-translation pair to an existing entry
    @param aEntries - array of entryTag structs
    @param nCount - no. of entries in database
*/
void
addTranslation(entry aEntries[], 
               int nCount)
{
    char cRepeat;
    str strLanguage, strWord;
    int i, nIndex, nPairs;
    int aDuplicates[MAX_ENTRIES] = {}; // stores indexes of entries that contain the pair being searched
    
    // Ask user for pair to search for
    printf("\n----- Input pair to search ----\n");
    inputPair(strLanguage, strWord);

    printf("\n---- SEARCHING FOR PAIR ----\n");

    // pairExists will return no. of duplicate pairs
    nPairs = pairExists(aEntries, nCount, strLanguage, strWord);
    
    // If pair was NOT found in ANY entry
    if (!nPairs)
    {
        printf("\nAn entry with this pair does not exist. Use the Add Entry option to create a new entry with this pair.\n\n");
        return;
    }

    // If pair was found in exactly ONE entry
    else if (nPairs == 1)
    {
        // find the entry that contains the searched pair
        nIndex = searchPair(aEntries, nCount, strLanguage, strWord, aDuplicates);

        // If entry already has 10 pairs, can't add anymore
        if (aEntries[nIndex].nPairs >= 10)
        {
            printf("\nThis entry already has 10 entries.\n");
            printf("\nReturning to Manage Data Menu...\n");
            return;
        }

        printf("\nInput new pair to be added to this entry\n");
        do
        {
            inputPair(strLanguage, strWord);

            // add pair to the entry, nIndex will locate position of entry
            // aEntries[nIndex].nPair will act as an index for the new pair's position (current no. of pairs = index to be inserted at)
            addPair(aEntries, nIndex, aEntries[nIndex].nPairs, strLanguage, strWord);
            
            // If haven't reached max no. of pairs
            if (aEntries[nIndex].nPairs < 10)
            {
                do
                {
                    printf("Do you want to input another pair? (Y/N): ");
                    scanf(" %c", &cRepeat);
                    if (cRepeat != 'Y' && cRepeat != 'y' && cRepeat != 'N' && cRepeat != 'n')
                        printf("Invalid input. Please try again.\n");
                } while (cRepeat != 'Y' && cRepeat != 'y' && cRepeat != 'N' && cRepeat != 'n');     
            }
        } while ((cRepeat == 'Y' || cRepeat == 'y') && aEntries[nIndex].nPairs < 10);

        printf("\nReturning to Manage Data Menu...\n");
        return; // so that any succeeding entries don't need to be checked
    }

    // Pair was found in more than one entry
    else 
    {
        // Store which entries contain the pair
        for (i = 0; i < nPairs; i++)
        {
            nIndex = searchPair(aEntries, nCount, strLanguage, strWord, aDuplicates);

            // Store the entry no. in its corresponding index (e.x., if 2nd entry contains pair, then aDuplicates[1] = 2)
            aDuplicates[nIndex] = nIndex + 1;
        }

        // After showing the user all the duplicate entries, ask where to add a new pair
        printf("\nWhich entry do you want to add to?: ");
        scanf("%d", &nIndex);
        nIndex--; // make sure nIndex matches array positioning

        // Input is valid if aDuplicates found the searched pair at that index inputted by the user and entry has less than 10 pairs
        if (aDuplicates[nIndex] && aEntries[nIndex].nPairs < 10)            
        {
            printf("\nInput new pair to be added to this entry\n"); 
            do
            {
                inputPair(strLanguage, strWord);
                addPair(aEntries, nIndex, aEntries[nIndex].nPairs, strLanguage, strWord);
                do
                {
                    printf("Do you want to input another pair? (Y/N): ");
                    scanf(" %c", &cRepeat);
                    if (cRepeat != 'Y' && cRepeat != 'y' && cRepeat != 'N' && cRepeat != 'n')
                        printf("Invalid input. Please try again.\n");
                } while (cRepeat != 'Y' && cRepeat != 'y' && cRepeat != 'N' && cRepeat != 'n');   
            } while ((cRepeat == 'Y' || cRepeat == 'y') && aEntries[nIndex].nPairs < 10);
        }
        // If selected entry contains 10 pairs already
        else if (aEntries[nIndex].nPairs >= 10)
            printf("This entry already has 10 entries. Returning to Manage Data Menu...\n");
        // If user inputs an entry index that DOESN'T contain the inputted pair
        else
            printf("Invalid entry, pair was not found there.\n"); // in future, can add a loop to let user input again for better UX.
    }
}


/*  Modifies language-translation pair/s of one database entry.
    @param aEntries - array of entryTag structs
    @param nCount - no. of entries in database
*/
void 
modifyEntry(entry aEntries[], 
            int nCount)
{
    char cModPair, cRepeat = ' ';
    str strLanguage, strWord;
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
                sortEntries(aEntries, nCount);
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
                            scanf("%s", strLanguage);
                            // add checking if string is valid? (i.e., at least 1 char)
                            printf("Replacing \"%s\" with \"%s\"...\n\n", aEntries[nEntryChoice].aPairs[nPairChoice].language, strLanguage);
                            strcpy(aEntries[nEntryChoice].aPairs[nPairChoice].language, strLanguage);
                            // Ask user if they wish to modify more pairs within the entry
                            do
                            {
                                printf("\nModify another pair within entry? (Y/N): ");
                                scanf(" %c", &cRepeat);
                            } while (cRepeat != 'Y' && cRepeat != 'y' && cRepeat != 'N' && cRepeat != 'n');
                            break;
                        
                        case 'T':
                        case 't':
                            printf("\nInput new word to replace \"%s\": ", aEntries[nEntryChoice].aPairs[nPairChoice].translation);
                            scanf("%s", strWord);
                            // add checking if string is valid? (i.e., at least 1 char)
                            printf("\nReplacing \"%s\" with \"%s\"...\n\n", aEntries[nEntryChoice].aPairs[nPairChoice].translation, strWord);
                            strcpy(aEntries[nEntryChoice].aPairs[nPairChoice].translation, strWord);
                            // Ask user if they wish to modify more pairs within the entry
                            do
                            {
                                printf("\nModify another pair within entry? (Y/N): ");
                                scanf(" %c", &cRepeat);
                            } while (cRepeat != 'Y' && cRepeat != 'y' && cRepeat != 'N' && cRepeat != 'n');
                            break;

                        case 'X':
                        case 'x':
                            break;
                            
                        default:
                            printf("\nInvalid input.\n\n");
                            break;
                    }
                }
                else
                    printf("\n\nInvalid pair.\n");
    
            } while (cRepeat == 'Y' || cRepeat == 'y');

            // Revert nEntryChoice to original input for loop checking.
            nEntryChoice++;
        }
        // If an invalid entry is selected
        else if (nEntryChoice < 0 || nEntryChoice > nCount)
            printf("\nInvalid entry.\n");

    } while (nEntryChoice == 0);

    printf("\nReturning to Manage Data Menu...\n");
}

/*  Allows user to delete one entry in the database
    @param aEntries - array of entryTag structs
    @param pCount - pointer to no. of entries in database
*/
void
deleteEntry(entry aEntries[], 
            int *pCount)
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
            printf("\nInvalid entry.\n");

    } while (nEntryChoice == 0);

    printf("\nReturning to Manage Data Menu...\n");
}


/*  Lets user delete one or more language-translation pairs within entry.
    @param aEntries - array of entryTag structs
    @param pCount - pointer to no. of entries in database
*/
void
deleteTranslation(entry aEntries[], 
                  int *pCount)
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
                sortEntries(aEntries, *pCount);
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
                    printf("\nInvalid pair.\n");
                    cRepeat = 'N';
                }            
            } while (cRepeat == 'Y' || cRepeat == 'y');
           
            // Revert nEntryChoice to original input for loop checking.
            nEntryChoice++; 
        }
        else if (nEntryChoice < 0 || nEntryChoice > *pCount)
            printf("\nInvalid entry.\n");

    } while (nEntryChoice == 0);

    printf("\nReturning to Manage Data Menu...\n");
}

/*  Sorts all pairs within all entries alphabetically by language using Bubble Sort
    @param aEntries - array of entryTag structs
    @param nCount - no. of entries in database
*/
void
sortEntries(entry aEntries[], 
            int nCount)
{
    int i, j, k, swapped, nPairCount;
    str s1, s2, strTemp;

    // Go through each entry in database
    for (i = 0; i < nCount; i++)
    {
        nPairCount = aEntries[i].nPairs;
        j = 0;
        // Go through each pair in entry 
        do
        {
            // Variable for checking whether we found any values that needed to be swapped            
            swapped = 0;

            // Go through each language until the last non-sorted language in the entry
            for (k = 0; k < nPairCount - j - 1; k++)
            {
                strcpy(s1, aEntries[i].aPairs[k].language); // Current language
                strcpy(s2, aEntries[i].aPairs[k+1].language); // Language to be compared to

                // if current language comes after (strcmp returns an int > 0) the language beside it, swap their positions
                if (strcmp(s1, s2) > 0)
                {
                    // Swapping by changing the values within the entry                   
                    strcpy(aEntries[i].aPairs[k].language, s2);
                    strcpy(aEntries[i].aPairs[k+1].language, s1);

                    // Also transfer the translations accordingly
                    strcpy(strTemp, aEntries[i].aPairs[k].translation); // translation 1
                    strcpy(aEntries[i].aPairs[k].translation, aEntries[i].aPairs[k+1].translation); // put translation 2 in translation 1's place
                    strcpy(aEntries[i].aPairs[k+1].translation, strTemp); // put translation 1 in translation 2's place

                    swapped = 1;
                }
            }

            j++;
        // If no values were swapped, it means entry is already sorted, so no need to keep going through the entry
        } while (swapped == 1 && j < nPairCount);
    }
}

/*  Function for displaying a SINGLE entry
    @param *Entry - pointer to entry to be displayed
    @param nEntryNum - number of entry being displayed
    @param nPairCount - no. of pairs within entry
*/
void displayEntry(entry *Entry, 
                  int nEntryNum, 
                  int nPairCount)
{
    int i;
    // Print entry and its pairs after sorting
    printf("--------------------------------\n");
    printf("Entry No. %d with %d pair/s\n\n", nEntryNum, Entry->nPairs);
    for (i = 0; i < nPairCount; i++)
        printf("Pair No. %d || Language: %s | Translation: %s\n", i + 1, Entry->aPairs[i].language, Entry->aPairs[i].translation);
    printf("--------------------------------\n");
}

/*  Function for displaying all entries in database
    @param aEntries - array of entryTag structs
    @param nCount - no. of entries in database
*/
void
displayAllEntries(entry aEntries[], 
                  int nCount)
{
    char ch = 'N';
    int i = 0;
    printf("\n--------------------------------\n");
    printf("%d entries in the database\n", nCount);
    printf("--------------------------------\n\n");

    sortEntries(aEntries, nCount);

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


/*  Function for searching for specific word in database
    @param aEntries - array of entryTag structs
    @param nCount - no. of entries in database
*/
void
searchWord(entry aEntries[], 
           int nCount)
{
    str strWord;
    char ch = 'N';
    int i, j, nEntryIndex, nFound = 0;
    int aFound[MAX_ENTRIES] = {}; // stores entry no.s where strWord was found

    printf("\nSearch for a word: ");
    scanf("%s", strWord);

    // Linearly search through every entry and pair, since words are not sorted in any way.
    for (i = 0; i < nCount; i++)
        for (j = 0; j < aEntries[i].nPairs; j++)
            // If inputted word matches translation in specific pair, case-sensitively
            if (strcmp(strWord, aEntries[i].aPairs[j].translation) == 0)
            {
                // Store entry no. in aFound
                aFound[nFound] = i;
                nFound++;
            }

    if (nFound == 0)
        printf("\nWord was not found in database. Returning to Manage Data Menu...\n");
    else
    {
        printf("\nFound \"%s\" in %d entry/entries\n", strWord, nFound);
        i = 0;
        // Display each entry stored in aFound while there are entries to show and while user hasn't exited
        while (i < nFound && i >= 0 && (ch == 'N' || ch == 'n' || ch == 'P' || ch == 'p'))
        {
            nEntryIndex = aFound[i];
            
            // Print the entry and its pairs
            sortEntries(aEntries, nCount);
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

/*  Lets user find a specific language-translation pair
    @param aEntries - array of entryTag structs
    @param nCount - no. of entries in database
*/
void
searchTranslation(entry aEntries[], 
                  int nCount)
{
    str strLanguage, strWord;
    char ch = 'N';
    int i, j, nEntryIndex, nFound = 0;
    int aFound[MAX_ENTRIES] = {}; // stores entry no.s where pair was found

    // Ask user for pair to search for
    printf("\n----- Input pair to search ----\n");
    inputPair(strLanguage, strWord);

    printf("\n---- SEARCHING FOR PAIR ----\n");

    // Linearly search through every entry and pair, since words are not sorted in any way.
    for (i = 0; i < nCount; i++)
        for (j = 0; j < aEntries[i].nPairs; j++)
            // If inputted pair matches searched pair, store index of the entry containing the pair
            if (strcmp(strLanguage, aEntries[i].aPairs[j].language) == 0
                && strcmp(strWord, aEntries[i].aPairs[j].translation) == 0)
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
            sortEntries(aEntries, nCount);
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

/*  Exports database of entries into a text file.
    @param aEntries - array of entryTag structs
    @param nCount - no. of entries in database
*/
void
export(entry aEntries[], 
       int nCount)
{
    FILE *fp;
    int i, j;
    char strFilename[31];

    do 
    {
        strcpy(strFilename, "");
        printf("Input filename (including extension): ");
        scanf("%s", strFilename);

        if (strlen(strFilename) >= 30)
            printf("\nFilename is too long, maximum of 30 characters only.\n");

    } while (strlen(strFilename) >= 30);

    printf("\nExporting...\n");
    
    if ((fp = fopen(strFilename, "w")) != NULL)
    {
        // Go through each entry and each pair and write them to the output file
        for (i = 0; i < nCount; i++)
        {
            for (j = 0; j < aEntries[i].nPairs; j++)
                fprintf(fp, "%s: %s\n", aEntries[i].aPairs[j].language, aEntries[i].aPairs[j].translation);
            
            fprintf(fp, "\n");
        }
        printf("\nSuccessfully exported %s.\n", strFilename);
        fclose(fp);
    }
    else
        printf("Error writing to file.\n");
}

/*  Function for importing a text file into the current entry database
    @param aEntries - array of entryTag structs
    @param pCount - pointer to no. of entries in database
*/
void
import(entry aEntries[], 
       int *pCount)
{
    FILE *fp;
    str sFilename;
    entry newEntry, blankEntry;
    int nPairCount = 0, nNew = 0;
    char cLoad, *sToken, buffer[50];
    
    printf("Input a text file containing entries you wish to import: ");
    scanf("%s", sFilename);

    if ((fp = fopen(sFilename, "r")) != NULL)
    {
        // If database is still empty
        if (*pCount == 0)
        {
            // while haven't reached end of file, go through each line in the text file
            while (fgets(buffer, 50, fp) != NULL)
            {
                // Only process input while max no. of entries hasn't been reached
                if (*pCount < MAX_ENTRIES)
                {
                    // Set trailing newline to null-terminating char to ignore newlines when taking input
                    // strcspn returns the no. of chars before the 1st occurence of '\n'
                    buffer[strcspn(buffer, "\n")] = '\0';

                    // If only char in buffer is a null-terminating character, a new entry was just recorded
                    if (buffer[0] == '\0')
                    {
                        *pCount += 1; // increment entry count
                        nPairCount = 0; // reset pair count for new entry
                        nNew++; // Keep track of how many NEW entries have been added
                    }
                    // Record language-translation pairs until we reach a '\0' buffer to signify a new entry
                    else
                    {
                        // strtok parses a string using the delimiter string
                        sToken = strtok(buffer, ": ");

                        // First token per line should be language
                        if (sToken != NULL)
                            strcpy(aEntries[*pCount].aPairs[nPairCount].language, sToken);
                        
                        // Second token per line should be translation
                        sToken = strtok(NULL, ": ");  // strtok(NULL, delim) moves on to next token in the line
                        if (sToken != NULL)
                            strcpy(aEntries[*pCount].aPairs[nPairCount].translation, sToken);
                        
                        aEntries[*pCount].nPairs++; // update no. of pairs in current entry
                        nPairCount++; // update no. of pairs to serve as indexing
                    }
                }
                // Inform user if database reaches max capacity during importing
                else
                {
                    printf("\nSuccessfully imported %d new entry/entries coming from \"%s\".\n", nNew, sFilename);
                    printf("\nDatabase has reached full capacity of 150 entries. "
                           "Delete entries using Delete Entry to continue adding entries."
                           "\nReturning to Manage Data Menu...\n");
                    return;
                }
            }

            // Tell user how many entries were imported
            printf("\nSuccessfully imported %d new entry/entries coming from \"%s\".\n", nNew, sFilename);
        }
        
        // If database contains at least 1 but less than maximum entry/entries
        else if (*pCount >= 1 && *pCount < MAX_ENTRIES)
        {
            // Initialize no. of pairs within the "entry" type structs
            newEntry.nPairs = 0;
            blankEntry.nPairs = 0;

            // Showcase each entry from the loaded text file one at a time
            while (fgets(buffer, 50, fp) != NULL)
            {
                if (*pCount < MAX_ENTRIES)
                {
                    // Set trailing newline to null-terminating char to ignore newlines when taking input
                    buffer[strcspn(buffer, "\n")] = '\0';

                    // If only char in buffer is a null-terminating character, a new entry was just recorded
                    if (buffer[0] == '\0')
                    {
                        // Display the entry that's been found
                        sortEntries(aEntries, *pCount);
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
                // if no. of entries within database exceeds maximum, stop loading new entries
                else
                {
                    printf("\nDatabase has reached full capacity of 150 entries. "
                           "Delete entries using Delete Entry to continue adding entries."
                           "\nReturning to Manage Data Menu...\n");
                    return;
                }
            }

            printf("\nSuccessfully imported %d new entry/entries coming from \"%s\".\n", nNew, sFilename);
        }
        // If database was full prior to importing
        else
            printf("\nDatabase has reached full capacity of 150 entries. "
            "Delete entries using Delete Entry to continue adding entries."
            "\nReturning to Manage Data Menu...\n");

        fclose(fp);
    }
    else
        printf("Error reading file.\n");
}

/* splitSentence splits the sentence string into different elements
   of an array of words while also increasing the element count. It
   also removes any and all characters that are not letters.
   @param strSentence - string containing a sentence/phrase
   @param strWords - 2D array of words separated into different
                     elements of the array
   @param pElem - number of elements in the 2D array

   @return the words of the sentence, split up into different elements
*/
void
splitSentence(char strSentence[], str strWords[], int *pElem)
{
    int i, j = 0, nPunc = 0;

    // loop through all characters of strSentence including the \0
    for (i = 0; i <= strlen(strSentence); i++)

        // if it's a letter, add to array
        if ((strSentence[i] >= 'A' && strSentence[i] <= 'Z') ||
            (strSentence[i] >= 'a' && strSentence[i] <= 'z'))
        {
            // if there has been a punctuation previously
            if (nPunc > 0)
            {
                strWords[*pElem][j] = '\0'; // ends the previous word
                j = 0; // resets letter space to begin a new word
                i--; // moves back to evaluate letter again
                (*pElem)++; // moves to the next word element
                nPunc = 0; // resets number of punctations
            }
            else
            {
                strWords[*pElem][j] = strSentence[i]; // places letter in word
                j++; // moves to next letter space
            }
        }

        // if it's a space or \0, replace end of word with \0
        else if (strSentence[i] == ' ' || strSentence[i] == '\0')
        {
            // if there is no \0 or a space after the current space or \0, replace with \0
            if (strSentence[i+1] != ' ' || strSentence[i+1] != '\0')
            {
                strWords[*pElem][j] = '\0'; // ends the previous word
                j = 0; // resets letter space to begin a new word
                (*pElem)++; // moves to the next word element
                nPunc = 0; // resets number of punctations
            }
        }
        // if it's none of the above
        else
            nPunc++; // increments number of punctuations
}

/* splitSentenceSpecs splits the sentence string into different 
   elements of an array of words while also increasing the element 
   count. It only removes commas, periods, question marks, and
   exclamation points.
   @param strSentence - string containing a sentence/phrase
   @param strWords - 2D array of words separated into different
                     elements of the array
   @param pElem - number of elements in the 2D array

   @return the words of the sentence, split up into different elements
*/
void
splitSentenceSpecs(char strSentence[], str strWords[], int *pElem)
{
    int i, j = 0, nPunc = 0;

    // loop through all characters of strSentence including the \0
    for (i = 0; i <= strlen(strSentence); i++)

        // if it's a space or \0
        if (strSentence[i] == ' ' || strSentence[i] == '\0')
        {
            // if there is no space or \0 after the current space or \0, replace with \0
            if (strSentence[i+1] != ' ' || strSentence[i+1] != '\0')
            {
                strWords[*pElem][j] = '\0'; // ends the previous word
                j = 0; // resets letter space to begin a new word
                (*pElem)++; // moves to the next word element
                nPunc = 0; // resets number of punctations  
            }
        }

        // if it's a comma, period, exclamation point or question mark
        else if (strSentence[i] == ',' || strSentence[i] == '.' ||
                 strSentence[i] == '!' || strSentence[i] == '?')
        {
            nPunc++; //increment number of punctuations
        }
        
        // if it's none of the above
        else 
        {
            // if there has been a punctuation
            if (nPunc > 0)
            {
                strWords[*pElem][j] = '\0';// ends the previous word
                j = 0; // resets letter space to begin a new word
                i--; // moves back to evaluate letter again
                (*pElem)++; // moves to the next word element
                nPunc = 0; // resets number of punctations
            }

            // if no punctuation
            else
            {
                strWords[*pElem][j] = strSentence[i]; // places letter in word
                j++; // moves to next letter space
            }
        }
}

/* fileReader safely opens and closes the file while separating all the
   words into different elements of one array excluding all punctuation marks.
   @param strFilename - string of the filename to be opened
   @param strFileWords - 2D array containing all the words in the file without
                         punctuation marks
   @param strFileSentence - string containing one entire line in the file
   @param pLineElem - number of elements in the line
   @param pFileWords - number of elements in the source file

   @return the words of the sentence, split up into different elements
*/
void
fileReader(str strFilename, str strFileWords[MAX_ENTRIES], char strFileSentence[151], int *pLineElem, int *pFileWords)
{
    FILE *pText;
    str  strLineWords[MAX_LETTERS];
    int i, j = 0;

    // checks if file can be opened and read
    if ((pText = fopen(strFilename, "r")) != NULL)
    {
        // while the file has not reached the end of file
        while(!feof(pText))
        {
            // gets a sentence string with at most 151 characters of the current line in the file
            fgets(strFileSentence, 151, pText);
            // removes the \n and replaces it with a \0
            strFileSentence[strlen(strFileSentence) - 1] = '\0';

            // if the first element is not empty
            if (strcmp(&strFileSentence[0], "") != 0)
            {
                // split file's current line into separate words
                splitSentence(strFileSentence, strLineWords, pLineElem);
                (*pFileWords) += (*pLineElem); // number of words in the line is incremented to the number of words in the file
                (*pLineElem) = 0; // reset number of words in the line
           
                // loop for Language and Translation
                for (i = 0; i < 2; i++)
                {
                    // if the word in the line is not empty and is not a new line
                    if (strcmp(strLineWords[i], "") != 0 && strcmp(strLineWords[i], "\n") != 0)
                    {
                        strcpy(strFileWords[j], strLineWords[i]); // copy word in the line to array of all the words
                        j++; // increment position to edit
                    }
                }
            }
        }
        fclose(pText); // close file after using
    }
    // if file cannot be opened
    else printf("Cannot open file for reading.\n");
}

/* matchingPairs checks if there are any matching language and word pairs
   in an array
   @param strFileWords - 2D array containing all the words in the file without
                         punctuation marks
   @param aMatches - array of words to check matching against
   @param pFileWords - number of elements in the source file
   @param j - index of current FileWord to be checked

   @return 1 if there is a match, return 2 if none
*/
int
matchingPairs(str strFileWords[], matchingType aMatches[], int *pFileWords, int j)
{
    int i, nMatchCount = 0;

    // loop for total number of words in the file
    for (i = 0; i < *pFileWords; i++)
    {
        // if there is a match
        if (strcmp(strFileWords[j], aMatches[i].word) == 0 &&
            strcmp(strFileWords[j-1], aMatches[i].language) == 0)
            nMatchCount++; // increment number of matches
    }

    // if there is a match
    if(nMatchCount > 0)
        return 1;
    // if there are no matches
    else
        return 2;
}

/* empyMatchingMember looks for an empty element in the array with the matches
   @param aMatches - array of words to check matching against
   @param nMatches - number of matches

   @return index of empty element, otherwise return -1
*/
int
emptyMatchingMember(matchingType aMatches[], int nMatches)
{
    int i;

    // loop for number of matches
    for (i = 0; i < nMatches; i++)
    {
        // if there is an empty member in the array
        if (strcmp(aMatches[i].language, "") == 0 &&
            strcmp(aMatches[i].word, "") == 0)
            return i; // return index of empty member
    }

    // if there is none return -1
    return -1;
}

/* empyLanguageMember looks for an empty element in the array with the languages
   @param aLanguages - array of languages found in the sentence string
   @param nLanguages - number of elements in the aLanguages array

   @return index of empty element, otherwise return -1
*/
int
emptyLanguageMember(languageType aLanguages[], int nLanguages)
{
    int i;

    // loop for number of languages
    for (i = 0; i < nLanguages; i++)
        // if there is an empty member in the array
        if (strcmp(aLanguages[i].language, "") == 0)
            return i; // return index of empty member
    
    // if there is none return -1
    return -1;
}

/* getSentence gets a sentence or phrase from the terminal input
   @param strInputSentence - array to store the sentence/phrase

   @return array filled with the inputted sentence
*/
void
getSentence(char strInputSentence[])
{
    do 
    {
        printf("Enter sentence or phrase: ");
        // gets a sentence string with at most 151 characters through terminal input
        fgets(strInputSentence, 151, stdin); 
        // replaces the newline character with a null character
        strInputSentence[strlen(strInputSentence) - 1] = '\0';

        // Sentence input should have at least 1 character
        if (strcmp(&strInputSentence[0], "\0") == 0)
            printf("Sentence/Phrase should have at least 1 character\n");
        
    } while (strcmp(&strInputSentence[0], "\0") == 0);
}


/* identifyML identifies the main language of a sentence/phrase
   @param pInputElem - number of elements in the inputted sentence/phrase
   @param pLineElem - number of elements in the line
   @param pFileWords - number of elements in the source file
   @param strFilename - string of the filename to be opened

   @return the language with the most matching entries
*/
void
identifyML(int *pInputElem, int *pLineElem, int *pFileWords, char strFilename[])
{
    str             strInputWords[MAX_ENTRIES],
                    strFileWords[MAX_ENTRIES];
    languageType    aLanguages[LANGUAGES];
    matchingType    aMatches[MAX_ENTRIES];
    char            strFileSentence[MAX_ENTRIES+1],
                    strInputSentence[MAX_ENTRIES+1];
    int             i, j, k, 
                    nMatches, 
                    nLanguages, 
                    nDuplicate, 
                    index = 0,
                    max = 0;

    // resetting variables
    *pInputElem = 0;
    *pLineElem = 0;
    nMatches = nLanguages = 1;
    memset(aLanguages, 0, sizeof(char)*LANGUAGES);
    memset(strFileWords, 0, sizeof(char)*MAX_ENTRIES);

    // gets the sentence/phrase from the user
    getSentence(strInputSentence);

    // split user's sentence input into separate words
    splitSentenceSpecs(strInputSentence, strInputWords, pInputElem);

    // reads the file and places all the words in one array
    fileReader(strFilename, strFileWords, strFileSentence, pLineElem, pFileWords);

    // loop for the number of words in the sentence/phrase
    for (i = 0; i < *pInputElem; i++)
    {
        memset(aMatches, 0, sizeof(char)*MAX_ENTRIES); // reset matches per word
        nDuplicate = 0; // reset number of duplicates
        
        // loop for number of words in the file
        for (j = 0; j < *pFileWords; j++)
        {
           // if sentence word and file word match
            if (strcmp(strInputWords[i], strFileWords[j]) == 0)
            {
                // if there are no matching pairs of the same word
                if (matchingPairs(strFileWords, aMatches, pFileWords, j) == 2)
                {
                    index = emptyMatchingMember(aMatches, nMatches); // finds the index of an empty member
                    strcpy(aMatches[index].language, strFileWords[j-1]); // places language into the matching array
                    strcpy(aMatches[index].word, strFileWords[j]); // places word into the matching array
                    nMatches++; // increments number of matches

                    // loop for the number of languages 
                    for (k = 0; k < nLanguages-1; k++)
                    {
                        // if language already has a count
                        if (strcmp(aLanguages[k].language, strFileWords[j-1]) == 0 && aLanguages[k].nLanguageCount > 0)
                        {
                            aLanguages[k].nLanguageCount++; // increment count of the language
                            nDuplicate++; // increment number of duplicates
                        }
                    }

                    // if there are no duplicates
                    if (nDuplicate == 0)
                    {
                        index = emptyLanguageMember(aLanguages, nLanguages); // finds the index of an empty member
                        strcpy(aLanguages[index].language, strFileWords[j-1]); // places language into the language array
                        aLanguages[index].nLanguageCount++; // increments the language in the language array
                        nLanguages++; // increments total number of languages
                    }
                    
                }
            }
        }
    }

    // if there are no existing languages
    if (nLanguages == 1)
        printf("Cannot determine the language\n");
    
    // if there are existing languages
    else if (nLanguages > 1)
    {
        max = 0;
        for (i = 0; i < nLanguages-1; i++)
            if (aLanguages[max].nLanguageCount < aLanguages[i].nLanguageCount)
                max = i;
        printf("Main Language: %s\n", aLanguages[max].language);
    }
}

/* findTranslation looks for a matching translation and returns the index
   @param strSourceLanguage - string of the source language
   @param strFileTranslations - array of all the words in the file separated
                                by translation
   @param strCurrentWord - word from the user's input that is being evaluated

   @return index of translation, otherwise returns -1
*/

int
findTranslation(str strSourceLanguage, str strFileTranslations[][MAX_ENTRIES], str strCurrentWord)
{
    int i, j;

    for (i = 0; i < MAX_ENTRIES; i++)
    {
        for (j = 0; j < MAX_PAIRS*2; j++)
        {
            if (strcmp(strSourceLanguage, strFileTranslations[i][j]) == 0 &&
                strcmp(strCurrentWord, strFileTranslations[i][j+1]) == 0)
                {
                    return i;
                }
        }
    }
    return -1;
}

/* simpleTranslation translates a sentence according to the available entry
   @param strFilename - string of the filename to be opened
   
   @return the translated sentence
*/

void
simpleTranslation(char strFilename[])
{
    FILE *pText;
    str  strSourceLanguage,
         strTranslatedLanguage,
         strInputWords[MAX_ENTRIES],
         strTempWords[MAX_ENTRIES],
         strFileTranslations[MAX_ENTRIES][MAX_ENTRIES];
    char strInputSentence[MAX_ENTRIES+1],
         strFileSentence[MAX_ENTRIES+1],
         cDump,
         cOpt;
    int  i = 0, j = 0,
         nWordCount = 0,
         nTempCount = 0,
         index = 0;
    
    printf("Enter the language of the source text: ");
    scanf(" %s%c", strSourceLanguage, &cDump); // receives source language and dumps \n
    printf("Enter the language to translate to: ");
    scanf(" %s%c", strTranslatedLanguage, &cDump); // receives translated language and dumps \n

    do
    {

    // resets variables
    memset(strInputWords, 0, sizeof(char)*MAX_ENTRIES);
    nWordCount = 0;

    // retrieves sentence/phrase from user
    getSentence(strInputSentence);
    // splits the sentence/phrase from user into different words
    splitSentenceSpecs(strInputSentence, strInputWords, &nWordCount);

    // if file can be opened and read
    if ((pText = fopen(strFilename, "r")) != NULL)
    {
        // while the file has not reached the end of file
        while(!feof(pText))
        {
            // retrieves the current line until \n
            fgets(strFileSentence, 151, pText);
            // removes the \n and replaces it with \0
            strFileSentence[strlen(strFileSentence) - 1] = '\0';

            // if the string is not empty
            if (strcmp(&strFileSentence[0], "") != 0)
            {
                // split file's current line into separate words
                splitSentence(strFileSentence, strTempWords, &nTempCount);
                strcpy(strFileTranslations[i][j], strTempWords[nTempCount-2]);
                strcpy(strFileTranslations[i][j+1], strTempWords[nTempCount-1]);
                j+=2;
            }

            // the string is empty
            else if (strcmp(&strFileSentence[0], "") == 0)
            {
                i++;
                j = 0;
            }
        }
        fclose(pText); // close file
    }
    // if file cannot be opened
    else printf("Cannot open file for reading.\n");
    
    // loop for number of words
    for (i = 0; i < nWordCount; i++)
    {
        // retrieve index of translation
        index = findTranslation(strSourceLanguage, strFileTranslations, strInputWords[i]);
        // if index is found
        if (index > -1)
            // loop for max number of language and word count
            for (j = 0; j < MAX_PAIRS*2; j++)
                // if translation language is found
                if (strcmp(strTranslatedLanguage, strFileTranslations[index][j]) == 0)
                    strcpy(strInputWords[i], strFileTranslations[index][j+1]); // replace word with translated word
    }

    // prints the translated sentence
    printf("Translated Sentence: ");
    for (i = 0; i < nWordCount; i++)
        printf("%s ", strInputWords[i]);
    
    // asks user if they want to translate a different sentence
    printf("\nDo you want to translate a different sentence? [Y/N]: ");
    scanf("%c%c", &cOpt, &cDump);
    
    } while ((cOpt == 'Y' || cOpt == 'y'));
}
