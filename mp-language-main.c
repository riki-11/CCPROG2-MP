/*********************************************************************************************************
This is to certify that this project is our own work, based on our personal efforts in studying and 
applying the concepts learned. We have constructed the functions and their respective algorithms 
and corresponding code by ourselves. The program was run, tested, and debugged by our own 
efforts. We further certify that we have not copied in part or whole or otherwise plagiarized the 
work of other students and/or persons.
                                    Mikkel Gamboa, DLSU ID# 12132195 | Enrique Lejano, DLSU ID# 12140996
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

    printf("\nTerminating Program\n");
    return 0;
}
