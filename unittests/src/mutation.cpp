#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <string.h>
#include <vector>
#include <unordered_map>
#include "../mutation.h"

using std::unordered_map;
using std::string;
using std::vector;

static int globerr(const char *epath, int eerrno) 
{
  fprintf(stderr, "glob error: %s\n", epath);
  return 0;
}

typedef vector<string> PathList;

struct __db {

    glob_t globbuf;
    unordered_map<string, PathList> db;

    __db()
    {
        // Create path glob
        char pattern[1024]; memset(pattern, 0, 1024);
        strcat(pattern, getenv("WORKSPACE_FOLDER"));
        strcat(pattern, "/*.mutdb");

        // Search for all .mutdb files
        glob(pattern, GLOB_NOSORT, globerr, &globbuf);
        
        if (globbuf.gl_pathc) {
         
            for (int i = 0; i < globbuf.gl_pathc; i++) {
                char *path = globbuf.gl_pathv[i];
                // Process the file
                FILE *fp;
                size_t size;

                if (! (fp = fopen(path, "r"))) {
                    perror("fopen");
                    continue;
                }
                // Get the file size by seeking to the end and getting
                // the offset
                fseek(fp, 0, SEEK_END);
                size = ftell(fp);
                // Rewind to the beginning of the file
                fseek(fp, 0, SEEK_SET);

                // Allocate a buffer to hold the file contents
                char *buffer = (char *) calloc(1, size + 1);
                fread(buffer, size, 1, fp);

                char *line = buffer;
                char *end;
                while ((end = strchr(line, '\n'))) {
                    vector<char *> fields;
                    *end = 0;
                    char *token = strtok(line, " ");
                    while (token) {
                        fields.push_back(token);
                        token = strtok(NULL, " ");
                    }
                    line = end + 1;
                    char *funcname = fields[2];
                    char *filename = fields[6];
                    if (! db.count(funcname)) {
                        db[funcname] = PathList();
                    }
                    db[funcname].push_back(filename);
                }
                free(buffer);
                fclose(fp);
            }
        }

    }

    ~__db()
    {
        globfree(&globbuf);
    }
} db;

const char *mutation_get_library(const char *symbol, int i)
{
    if (! getenv("MUTATION_TEST")) {
        puts("not mutating");
        return NULL;
    }

    
    if (! db.db.count(symbol)) {
        puts("no such symbol");
        exit(1);
    }
    
    if (i < db.db[symbol].size()) {
        return db.db[symbol][i].c_str();
    }
    return NULL;
}

