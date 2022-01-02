#ifndef MUTATION_H
#define MUTATION_H

#ifdef __cplusplus
extern "C" {
#endif
#include <dlfcn.h>
#include <unistd.h>
#include <sys/wait.h>

void *__mutation_symbol(void * handle, const char *name);
void *__mutation_open(const char *name);
void __mutation_close();


const char * mutation_get_library(const char *symbol, int i);

#define MUTATION_START(symbol, orig) \
do { \
    const void * __mutation_symbol_linked = (void *) symbol; \
    const char * __mutation_search_symbol = #symbol; \
    const char * __mutation_librarypath = orig;\
    int __mutation_mutant_num = 0, __mutation_mutants_killed = 0;\
    do { \
        pid_t __mutation_child; \
        if (__mutation_mutant_num) { \
            if ((__mutation_child = fork())) { \
                int __mutation_status; waitpid(__mutation_child, &__mutation_status, 0); \
                if (WEXITSTATUS(__mutation_status) != 0 || WIFSIGNALED(__mutation_status)) { \
                    __mutation_mutants_killed ++; \
                    continue; \
                } \
            } else { close(1), close(2); } \
        }
#define test_function() (__mutation_mutant_num ? __mutation_symbol(__mutation_open(__mutation_librarypath), __mutation_search_symbol) : __mutation_symbol_linked)

#define MUTATION_END \
        if (__mutation_mutant_num) __mutation_close;\
        if (! __mutation_child && __mutation_mutant_num) exit(0);\
    } while ((__mutation_librarypath = mutation_get_library(__mutation_search_symbol, __mutation_mutant_num++)) != 0);\
    fprintf(stderr, "%s : Killed %d out of %d\n", __mutation_search_symbol, __mutation_mutants_killed, __mutation_mutant_num - 1);\
} while (0);


#ifdef __cplusplus
}
#endif
#endif