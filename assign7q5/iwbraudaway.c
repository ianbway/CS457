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

char *primaryKey;
void **candidateKeysGlobal;

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

    // Read in primary key, it is next token
    primaryKey = readToken(file);

    char *cKey;
    
    // If there is at least one candidate key after the primary key
    // Load up all subsequent candidate keys into an array
    if (primaryKey[strlen(primaryKey) - 1] != ';')
    {
        cKey = readToken(file);
        insertDA(candidateKeys, newSTRING(cKey));
        insertDA(candidateKeys, newSTRING("|"));
        while (cKey[strlen(cKey) - 1] != ';')
        {
            cKey = readToken(file);
            insertDA(candidateKeys, newSTRING(cKey));
            insertDA(candidateKeys, newSTRING("|"));
        }

        candidateKeysGlobal = extractDA(candidateKeys);
    }

    // Strip the ; from primary key, replace with null
    else
    {
        primaryKey[strlen(primaryKey)-1] = 0;
    }
    
    // Read dependencies token by token until end of current table
    DA *dependencies = newDA(displaySTRING);
    char *dependency = readToken(file);

	while (compareSTRING(newSTRING(dependency), newSTRING("x;")) != 0)
	{
        insertDA(dependencies, newSTRING(dependency));
        dependency = readToken(file);
	}

	return dependencies; 
}

bool
checkTransitive(DA *dependentArray)
{

    // Testing for transitivity
    int i;
    for (i = 0; i < sizeDA(dependentArray); i++)
    {
        STRING *pair = getDA(dependentArray, i);
        char *pairVal = getSTRING(pair); 
        
        char *parens = strtok(pairVal, ")(");

        // Not allowed to have transitivity, key is not the same as primary key
        if (compareSTRING(newSTRING(parens), newSTRING(primaryKey)) != 0)
        {
            return true;
        }

        while (parens != NULL)
        {
            parens = strtok(NULL, ")(");
        }
    }

    // Else transitivity does not happen here
 	return false;
}

bool
checkPartial(DA *dependentArray)
{
    // Testing for Partiality
    int i;
    for (i = 0; i < sizeDA(dependentArray); i++)
    {
        STRING *pair = getDA(dependentArray, i);
        char *pairVal = getSTRING(pair); 
        
        char *parens = strtok(pairVal, ")(");

        // Not allowed to have partial dependency, partial key has dependents
        if (strlen(parens) > 1)
        {
            int j;
            int k;
            for (j = 0; parens[j] != 0; j++)
            {
                for (k = 0; k < sizeDA(dependentArray); k++)
                {
                    STRING *pairDeep = getDA(dependentArray, k);
                    char *pairValDeep = getSTRING(pairDeep); 

                    char *parensDeep = strtok(pairValDeep, ")(");
                    char charParensJ = parens[j];
                    char *sParens = malloc(sizeof(char));
                    *sParens = charParensJ;

                    if (compareSTRING(newSTRING(sParens), newSTRING(parensDeep)) == 0)
                    {
                        return true;
                    }

                    while (parensDeep != NULL)
                    {
                        parensDeep = strtok(NULL, ")(");
                    }
                }
            }
        }

        while (parens != NULL)
        {
            parens = strtok(NULL, ")(");
        }
        
    }

    // Else partiality does not happen here
    return false;
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

    while (!feof(fp))
    {
        DA *dependencies = processInput(fp);

        // Check for Partial, 2NF?
        if(checkPartial(dependencies) == false)
        {
            // If partial, maybe transitive 3NF?
            if (checkTransitive(dependencies) == false)
            {
                printf("3NF\n");
            }

            // Nope, just partial. 2NF
            else
            {
                printf("2NF\n");
            }
            
        }

        // Else 1NF.
        else
        {
            printf("1NF\n");
        }

    }

	return 0;
}