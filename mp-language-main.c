/*********************************************************************************************************
This is to certify that this project is my own work, based on my personal efforts in studying and applying 
the concepts learned. I have constructed the functions and their respective algorithms and corresponding 
code by myself. The program was run, tested, and debugged by my own efforts. I further certify that I have 
not copied in part or whole or otherwise plagiarized the work of other students and/or persons.
Mikkel Gamboa, DLSU ID# <number> | Enrique Lejano, DLSU ID# 12140996
*********************************************************************************************************/

#include "manage-tools.c"

int
main()
{
    int nMMInput = 0, nMLInput = 0, nEntryCount = 0, nInputElem = 0, nLineElem = 0, nFileWords = 0;
    entry aEntries[MAX_ENTRIES];

    initDatabase(aEntries); // Initializes database of entries
    
    // Always go back to Main Menu as long as user doesn't exit program 
    do {
        displayMainMenu(); // Display Main Menu
        getMMInput(&nMMInput); // User chooses between MD and LT

        if (nMMInput != 3)
            switchMainMenu(nMMInput, &nMLInput, aEntries, &nEntryCount, &nInputElem, &nLineElem, &nFileWords); // Program processes user's input
        
    } while (nMMInput != 3);

    return 0;
}
