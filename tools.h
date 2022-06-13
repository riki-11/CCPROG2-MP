#include "manage.c"

// Max no. of languages
#define LANGUAGES 7100

typedef struct languageTag {
    str language;
    int nLanguageCount;
} languageType;

typedef struct matchingTag {
    str language,
        word;
} matchingType;

void switchLTMenu(int, int*, int*, int*);
void identifyML(int*, int*, int*);
void FileReader(str, str[], char[], int*, int*);
int MatchingPairs(str[], matchingType[], int*, int);
int EmptyLanguageMember(languageType[], int);
int EmptyMatchingMember(matchingType[], int);
