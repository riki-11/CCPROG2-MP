#include "tools.h"

// Function for moving around LT menu
void
switchLTMenu(int nMLInput, int *nInputElem, int *nLineElem, int *nFileWords)
{
    switch(nMLInput)
    {
        case 1:
            identifyML(nInputElem, nLineElem, nFileWords);
            break;
        case 2:

            break;

        // Exits LT Menu
        case 3:
            printf("Exiting to Main Menu...\n");
            return;

        default:
            printf("Invalid input\n");
    }
}

void
splitSentence(char strSentence[], str strWords[], int *nElem)
{
    int i, j = 0, nPunc = 0;

    // loop through all characters of strSentence including the \0
    for (i = 0; i <= strlen(strSentence); i++)
        // if it's a letter, add to array
        if ((strSentence[i] >= 'A' && strSentence[i] <= 'Z') ||
            (strSentence[i] >= 'a' && strSentence[i] <= 'z'))
        {
            // if there has been a punctuation
            if (nPunc > 0)
            {
                strWords[*nElem][j] = '\0';
                j = 0;
                i--;
                (*nElem)++;
                nPunc = 0;
            }
            else
            {
                strWords[*nElem][j] = strSentence[i];
                j++;
            }
        }
        // if it's a space or \0, replace end of word with \0
        else if (strSentence[i] == ' ' || strSentence[i] == '\0')
        {
            // if there is no \0 or a space after the current space or \0, replace (i.e. '  ')
            if (strSentence[i+1] != ' ' || strSentence[i+1] != '\0')
            {
                strWords[*nElem][j] = '\0';
                j = 0;
                (*nElem)++;
                nPunc = 0;  
            }
        }
        // if it's none of the above, increment number of punctuations
        else
            nPunc++;
}


// Function for identifying the main language
void
identifyML(int *nInputElem, int *nLineElem, int *nFileWords)
{
    str             strInputWords[ENTRIES],
                    strFileWords[ENTRIES];
    languageType    aLanguages[LANGUAGES];
    matchingType    aMatches[ENTRIES];
    char            strFilename[ENTRIES],
                    strInputSentence[ENTRIES+1],
                    strFileSentence[ENTRIES+1];
    int             i, j, k, 
                    nMatches, 
                    nLanguages, 
                    nDuplicate, 
                    index = 0,
                    max = 0;

    *nInputElem = 0;
    *nLineElem = 0;
    nMatches = nLanguages = 1;
    memset(aLanguages, 0, sizeof(char)*LANGUAGES);

    do 
    {
        printf("Enter sentence or phrase: \n");
        // gets a sentence string with at most 151 characters through terminal input
        fgets(strInputSentence, 151, stdin); 
        // replaces the newline character with a null character
        strInputSentence[strlen(strInputSentence) - 1] = '\0';

        // Sentence input should have at least 1 character
        if (strcmp(&strInputSentence[0], "\0") == 0)
            printf("Sentence/Phrase should have at least 1 character");
        
    } while (strcmp(&strInputSentence[0], "\0") == 0);

    // split user's sentence input into separate words
    splitSentence(strInputSentence, strInputWords, nInputElem);

    printf("Enter filename: \n");
    scanf(" %s", strFilename);

    FileReader(strFilename, strFileWords, strFileSentence, nLineElem, nFileWords);

    for (i = 0; i < *nInputElem; i++)
    {
        memset(aMatches, 0, sizeof(char)*ENTRIES);
        //printf("Step 3\n");
        for (j = 0; j < *nFileWords; j++)
        {
           // printf("Step 4\n");
            if (strcmp(strInputWords[i], strFileWords[j]) == 0)
            {
                //printf("Step 5\n");
                if (MatchingPairs(strFileWords, aMatches, nFileWords, j) == 2)
                {
                    //printf("Step 6\n");
                    nDuplicate = 0;
                    index = EmptyMatchingMember(aMatches, nMatches);
                   // printf("index1 - %d\n", index);
                    strcpy(aMatches[index].language, strFileWords[j-1]);
                    strcpy(aMatches[index].word, strFileWords[j]);
                    nMatches++;
                   // printf("Language added: %s, Word added: %s\n", aMatches[index].language, aMatches[index].word);

                    for (k = 0; k <= nLanguages; k++)
                    {
                       // printf("Language Word - %s, File Word - %s\n", aLanguages[k].language, strFileWords[j-1]);
                        if (strcmp(aLanguages[k].language, strFileWords[j-1]) == 0 && aLanguages[k].nLanguageCount > 0)
                        {
                            //printf("theinbetween\nLanguages - %s, Count - %d\n", aLanguages[k].language, aLanguages[k].nLanguageCount);
                            aLanguages[index].nLanguageCount++;
                            nDuplicate++;
                        }
                    }
                    if (nDuplicate == 0)
                    {
                        index = EmptyLanguageMember(aLanguages, nLanguages);
                        //printf("index2 - %d\n", index);
                        strcpy(aLanguages[index].language, strFileWords[j-1]);
                        aLanguages[index].nLanguageCount++;
                        nLanguages++;
                        k = nLanguages;
                        //printf("Language added: %s, Language count: %d\n", aLanguages[index].language, aLanguages[index].nLanguageCount);
                    }
                    
                }
            }
        }
    }

    if (nLanguages == 1)
        printf("Cannot determine the language\n");
    
    else if (nLanguages > 1)
    {
        for (i = 0; i < nLanguages; i++)
            if(max < aLanguages[i].nLanguageCount)
                max = i;
        printf("Language: %s\n", aLanguages[max].language);
    }
}

int
EmptyLanguageMember(languageType aLanguages[], int nLanguages)
{
    int i;

    for (i = 0; i < nLanguages; i++)
        if (strcmp(aLanguages[i].language, "") == 0)
            return i;
    return 0;
}

int
EmptyMatchingMember(matchingType aMatches[], int nMatches)
{
    int i;

    for (i = 0; i < nMatches; i++)
    {
        if (strcmp(aMatches[i].language, "") == 0 &&
            strcmp(aMatches[i].word, "") == 0)
            return i;
    }
    return 0;
}

void
FileReader(str strFilename, str strFileWords[ENTRIES], char strFileSentence[151], int *nLineElem, int *nFileWords)
{
    FILE *pText;
    str  strLineWords[LETTERS];
    int i, j = 0;

    if ((pText = fopen(strFilename, "r")) != NULL)
    {
        while(!feof(pText))
        {
            // gets a sentence string with at most 151 characters of the current line in the file
            fgets(strFileSentence, 151, pText);
            strFileSentence[strlen(strFileSentence) - 1] = '\0';

            if (strcmp(&strFileSentence[0], "") != 0)
            {
                // split file's current line into separate words
                splitSentence(strFileSentence, strLineWords, nLineElem);
                (*nFileWords) += (*nLineElem);
                (*nLineElem) = 0;
            }

            for (i = 0; i < 2; i++)
            {
                if (strcmp(strLineWords[i], "") != 0 && strcmp(strLineWords[i], "\n") != 0)
                {
                    strcpy(strFileWords[j], strLineWords[i]);
                    j++;
                }
            }
        }
        fclose(pText);
    }
    else printf("Cannot open file for writing.\n");
}

int
MatchingPairs(str strFileWords[], matchingType aMatches[], int *nFileWords, int j)
{
    int i, nMatchCount = 0;

    for (i = 0; i < *nFileWords; i++)
    {
        if (strcmp(strFileWords[j], aMatches[i].word) == 0 &&
            strcmp(strFileWords[j-1], aMatches[i].language) == 0)
            nMatchCount++;
    }
        if(nMatchCount > 0)
            return 1;
        else
            return 2;
}
