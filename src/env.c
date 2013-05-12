#include "pos.h"
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    extern char **environ;
    char **command = NULL;

    int i;
    int i_option;

    --argc; ++argv;

    if (argc > 0 && strcmp(argv[0], "-i") == 0) {
        --argc; ++argv;
        i_option = 1;
    } else {
        i_option = 0;
    }

    if (i_option && clearenv() != 0) {
        die_fn("clearenv");
    }

    for (i = 0; i < argc; i++) {
        if (strchr(argv[i], '=')) {
            if (putenv(argv[i]) != 0) {
                die_fn("putenv");
            }
        } else {
            command = &argv[i];
            break;
        }
    }

    if (command) {
        if (execvp(*command, command) == -1) {
            die_fn("execvp");
        }
    }

    if (environ) {
        for (i = 0; environ[i]; i++) {
            printf("%s\n", environ[i]);
        }
    }

    exit(EXIT_SUCCESS);
}
