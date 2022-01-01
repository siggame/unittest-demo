#define CATCH_DEFINE_TRIANGLE
#include <dlfcn.h>
#include "catch.hpp"
#include "mutation.h"
#include "../triangle.h"
#include <unistd.h>
#include <sys/wait.h>

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
        typedef int (*triangle_tritype_func)(int, int, int);
        triangle_tritype_func func = (triangle_tritype_func) dlsym(library, "triangle_tritype");
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

        REQUIRE(func(0, 0, 0) == TRIANGLE_ERR);
        REQUIRE(func(0, 1, 1) == TRIANGLE_ERR);
        REQUIRE(func(1, 0, 1) == TRIANGLE_ERR);
        REQUIRE(func(1, 1, 0) == TRIANGLE_ERR);
        REQUIRE(func(2, 2, 2) == TRIANGLE_EQUI);
        REQUIRE(func(2, 2, 3) == TRIANGLE_ISO);
        REQUIRE(func(2, 3, 1) == TRIANGLE_SCAL);
        REQUIRE(func(3, 3, 1) == TRIANGLE_ISO);
        REQUIRE(func(3, 3, 2) == TRIANGLE_ISO);

        if (! child) {
            // Exit after mutation test. Report success, which we don't want
            exit(0);
        }
        dlclose(library);
    } while ((librarypath = mutation_get_library("triangle_tritype", i++)) != NULL);

    
    printf("\ntriangle_trytype: Killed %d times out of %d\n", killed, i - 1);

}


