#define CATCH_DEFINE_TRIANGLE
#include <dlfcn.h>
#include "catch.hpp"
#include "mutation.h"
#include "../triangle.h"
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
TEST_CASE("triangle tritype")
{
    const char *librarypath = "triangle.so";
    int i = 0;
    int killed = 0;

    do {
        void *library = dlopen(librarypath, RTLD_LAZY);
        if (!library) {
            printf("%s\n", dlerror());
            return;
        }

        
        // If the first run (of unmutated function) create a fork for each
        // mutation
        pid_t child;
        if (i && (child = fork())) { // Parent process waits for child to complete
            int status;
            waitpid(child, &status, 0);
            if (WEXITSTATUS(status) != 0 || WIFSIGNALED(status)) {
                killed ++;
            }
            continue;
        }
        else if (i) { // Mutated process closes stdout and stderr so we don't see the child output
            close(1);
            close(2);
        }
        
        typedef int (*triangle_tritype_func)(int, int, int);
        triangle_tritype_func func = (triangle_tritype_func) dlsym(library, "triangle_tritype");

        REQUIRE(func(0, 0, 0) == TRIANGLE_ERR);
        REQUIRE(func(0, 1, 1) == TRIANGLE_ERR);
        REQUIRE(func(1, 0, 1) == TRIANGLE_ERR);
        REQUIRE(func(1, 1, 0) == TRIANGLE_ERR);
        REQUIRE(func(2, 2, 2) == TRIANGLE_EQUI);
        REQUIRE(func(2, 2, 3) == TRIANGLE_ISO);
        REQUIRE(func(2, 3, 1) == TRIANGLE_SCAL);
        REQUIRE(func(3, 3, 1) == TRIANGLE_ISO);
        REQUIRE(func(3, 3, 2) == TRIANGLE_ISO);

        if (! child && i) {
            // Exit after mutation test. Report success, which we don't want
            exit(0);
        }
        dlclose(library);
    } while ((librarypath = mutation_get_library("triangle_tritype", i++)) != NULL);

    
    printf("\ntriangle_trytype: Killed %d times out of %d\n", killed, i - 1);

}


TEST_CASE("triangle area")
{
    const char *librarypath = "triangle.so";
    int i = 0;
    int killed = 0;

    do {
        void *library = dlopen(librarypath, RTLD_LAZY);
        if (!library) {
            printf("%s\n", dlerror());
            return;
        }
        typedef double (*triangle_tritype_func)(double, double, double);
        triangle_tritype_func func = (triangle_tritype_func) dlsym(library, "triangle_area");
        
        if (! func) {
            printf("%s\n", dlerror());
            return;
        }

        pid_t child;
        if (i && (child = fork())) {
            int status;
            waitpid(child, &status, 0);
            if (WEXITSTATUS(status) != 0 || WIFSIGNALED(status)) {
                killed ++;
            }
            continue;
        }
        else if (i) {
            close(1);
            close(2);
        }

       
        REQUIRE(func(0, 0, 0) == 0);
        REQUIRE(0.432 < func(1, 1, 1));
        REQUIRE(func(1, 1, 1) < 0.434); 

        if (! child && i) {
            // Exit after mutation test. Report success, which we don't want
            exit(0);
        }
        dlclose(library);
    } while ((librarypath = mutation_get_library("triangle_area", i++)) != NULL);

    printf("\ntriangle_area: Killed %d times out of %d\n", killed, i - 1);
}


