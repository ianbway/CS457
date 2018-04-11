// Ian Braudaway
// Assignment 7 Question 5

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include "da.h"
#include "scanner.h"
#include "string.h"

DA *processInput(FILE *);
bool isCKeyInsideOfMe(char *);
bool isPrime(char *);
bool isSubSequence(char *, char *);
bool check3NF(DA *);
bool check2NF(DA *);
void copyCKeyArray(DA *, DA*);

char *primaryKey;
DA *candidateKeysGlobal;

void
copyCKeyArray(DA *cKeys, DA* destGlobal)
{
    // Erase prior global array
    if (sizeDA(destGlobal) > 0)
    {
        extractDA(destGlobal);
    }

    // Load up all elements of cKeys into destGlobal
    int i;
    for (i = 0; i < sizeDA(cKeys); i++)
    {
        insertDA(destGlobal, getDA(cKeys, i));
    }
}

DA *
processInput(FILE *file)
{
    // Initialize Arrays to hold values
    char *attributes;
    
    DA *candidateKeys = newDA(displaySTRING);

    // Read in attributes first
    attributes = readToken(file);

    // End of File
    if (attributes == 0)
    {
        exit(0);
    }

    // Read in primary key, it is next token. Put it in candidate Key array as first element.
    primaryKey = readToken(file);
    insertDA(candidateKeys, newSTRING(primaryKey));

    char *cKey;
    
    // If there is at least one candidate key after the primary key
    // Load up all subsequent candidate keys into an array
    if (primaryKey[strlen(primaryKey) - 1] != ';')
    {
        cKey = readToken(file);

        insertDA(candidateKeys, newSTRING(cKey));

        while (cKey[strlen(cKey) - 1] != ';')
        {
            cKey = readToken(file);
            insertDA(candidateKeys, newSTRING(cKey));
        }

        // Strip final ; from candidate key
        cKey[strlen(cKey)-1] = 0;
    }

    // Strip the ; from primary key, replace with null
    else
    {
        primaryKey[strlen(primaryKey)-1] = 0;
    }
    
    // Make candidate keys global
    copyCKeyArray(candidateKeys, candidateKeysGlobal);

    // Read dependencies token by token until end of current table. Strip ; from dependency.
    DA *dependencies = newDA(displaySTRING);
    char *dependency = readToken(file);
    dependency[strlen(dependency)-1] = 0;

    // Insert all dependencies, ; stripped, into dependecy array which will be returned.
    while (dependency[0] != 'x')
    {
        insertDA(dependencies, newSTRING(dependency));
        dependency = readToken(file);
        dependency[strlen(dependency)-1] = 0;
    }

    return dependencies; 
}

// For each candidate key in our global array, check to see if it is equal to or fits within the key we pass in.
bool
isCKeyInsideOfMe(char *checkWithinMe)
{
    int i;
    for (i = 0; i < sizeDA(candidateKeysGlobal); i++)
    {
        char *cKeyToPutIn = getSTRING(getDA(candidateKeysGlobal, i));

        if ((compareSTRING(newSTRING(cKeyToPutIn), newSTRING(checkWithinMe)) == 0) || (isSubSequence(cKeyToPutIn, checkWithinMe) == true))
        {
            return true;
        }
    }

    return false;
}

bool
isPrime(char *amIPrime)
{
    int i;
    for (i = 0; i < sizeDA(candidateKeysGlobal); i++)
    {
        char *cKeyToLookIn = getSTRING(getDA(candidateKeysGlobal, i));

        if (isSubSequence(amIPrime, cKeyToLookIn) == true)
        {
            return true;
        }
    }

    return false;
}

// Returns true if stringInside is a subsequence of superString.
bool 
isSubSequence(char *smallString, char *largeString)
{
    int i = 0;
    int j = 0;
    int smallLen = strlen(smallString);
    int largeLen = strlen(largeString);

    if (smallLen > largeLen)
    {
        return false;
    }

    for (i=0; i < largeLen && j < smallLen; i++)
    {
        if (smallString[j] == largeString[i])
        {
            j++;
        }
    }

    return (j==smallLen);
}

bool
check3NF(DA *dependentArray)
{
    bool notInCKeys = false;

    // Testing for breaking 3NF
    int i;
    for (i = 0; i < sizeDA(dependentArray); i++)
    {
        STRING *pair = getDA(dependentArray, i);
        char *pairVal = getSTRING(pair); 
        
        char *parens = strtok(pairVal, ")(");

        if (isCKeyInsideOfMe(parens) == false)
        {
            notInCKeys = true;
        }

        while (parens != NULL)
        {
            parens = strtok(NULL, ")(");

            if ((parens != NULL) && (notInCKeys == true) && (isPrime(parens) != true))
            {
                return false;
            }
        }
    }

    // Else 3NF
    return true;
}

bool
check2NF(DA *dependentArray)
{
    bool primeFlag = false;

    // Testing for breaking 2NF
    int i;
    for (i = 0; i < sizeDA(dependentArray); i++)
    {
        STRING *pair = getDA(dependentArray, i);
        char *pairVal = getSTRING(pair); 
        
        char *parens = strtok(pairVal, ")(");

        if ((isPrime(parens) == true) && (isCKeyInsideOfMe(parens) == false))
        {
            primeFlag = true;
        }

        while (parens != NULL)
        {
            parens = strtok(NULL, ")(");
            
            if ((parens != NULL) && (primeFlag == true) && (isPrime(parens) != true))
            {
                return false;
            }
        }
    }

    // Else it is 2NF
    return true;
}

int 
main(void)
{

    // Read in file
    FILE *fp = fopen("./HW7in.txt", "r");

    if (!fp)
    {
        printf("No input file.\n");
        exit(-1);
    }

    candidateKeysGlobal = newDA(displaySTRING);

    while (!feof(fp))
    {
        // Process input into a global array of candidate keys and a local array of dependencies
        DA *dependencies = processInput(fp);

        // Check for 3NF
        if (check3NF(dependencies) == true)
        {
            printf("3NF\n");
        }

        // Check for 2NF
        else if (check2NF(dependencies) == true)
        {
            printf("2NF\n");
        }

        // Else 1NF.
        else
        {
            printf("1NF\n");
        }
    }

    return 0;
}