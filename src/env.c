#include "posix-tools.h"
#include <unistd.h>
#include <string.h>

const char *program_name;

int main(int argc, char *argv[]) {
    extern char **environ;
    char **command = NULL;

    int i;
    int i_option;

    program_name = argv[0];

    --argc; ++argv;

    if (argc > 0 && strcmp(argv[0], "-i") == 0) {
        --argc; ++argv;
        i_option = 1;
    } else {
        i_option = 0;
    }

    if (i_option) {
        environ = NULL;
    }

    for (i = 0; i < argc; i++) {
        if (strchr(argv[i], '=')) {
            if (putenv(argv[i]) != 0) {
                pt_die_fn("putenv");
            }
        } else {
            command = &argv[i];
            break;
        }
    }

    if (command) {
        if (execvp(*command, command) == -1) {
            pt_die_fn("execvp");
        }
    }

    if (environ) {
        for (i = 0; environ[i]; i++) {
            printf("%s\n", environ[i]);
        }
    }

    exit(EXIT_SUCCESS);
}
