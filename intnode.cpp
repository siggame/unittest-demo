#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "intnode.h"

int intnode_create(int **thisptr, int val)
{
    void *mem = malloc(sizeof(int));
    memcpy(mem, &val, sizeof(int));
    memcpy(thisptr, &mem, sizeof(int *));
    return 0; 
}

int intnode_print(const int * self, FILE *fp)
{
    return fprintf(fp, "%d", *self); 
}

int intnode_destroy(int **thisptr)
{
    return 0;
}
