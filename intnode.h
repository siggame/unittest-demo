// TODO Create doxygen comment
#ifndef INTNODE_H
#define INTNODE_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>

/**
 * @brief Create a node.
*/
int intnode_create(int **thisptr, int val);
int intnode_print(const int *self, FILE *fp);
int intnode_destroy(int **thisptr);

#ifdef __cplusplus
}
#endif
#endif