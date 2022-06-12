#include "manage.c"

int
main()
{
    int nMMInput = 0, nMLInput = 0, nEntryCount = 0;
    entry aEntries[MAX_ENTRIES];

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