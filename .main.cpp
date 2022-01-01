#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef int (*triangle_tritype)(int, int, int);

int main()
{
    void *library;
    triangle_tritype func;

    char envbuffer[1024]; memset(envbuffer, 0, sizeof(envbuffer));

    const char * ld_library_path = getenv("LD_LIBRARY_PATH"); 
    size_t size = strlen(ld_library_path);
    memcpy(envbuffer, ld_library_path, size);
    envbuffer[size++] = ':';
    getcwd(envbuffer + size, sizeof(envbuffer) - size);
    strcat(envbuffer, "/lib");
    
    puts(envbuffer);
    setenv("LD_LIBRARY_PATH", envbuffer, 1);

    for (int i = 0; i < 100; i++) {
        if (! (library = dlopen("triangle.so", RTLD_LAZY))) {
            printf("%s\n", dlerror());
            return 1;
        }

        if (! (func = (triangle_tritype) dlsym(library, "triangle_tritype"))) {
            printf("%s\n", dlerror());
            return 1;
        }
        
        printf("%d\n", func(1, 1, 1));
        dlclose(library);
    }
}
