#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {

    printf("real user id %ld\n", getuid());
    printf("effective user id %ld\n", geteuid());

    if (argc != 2) {
        fprintf(stderr, "need file name\n");
        return 1;
    }

    FILE *f1;
    f1 = fopen(argv[1], "r");
    if (f1 == NULL) {
        perror("file open error");
    }
    else {
        fclose(f1);
    }

    uid_t uid = getuid();
    seteuid(uid);

    printf("after setuid \n");
    printf("real user id %ld\n", getuid());
    printf("effective user id %ld\n", geteuid());

    f1 = fopen(argv[1], "r");
    if (f1 == NULL) {
        perror("file open error");
    }
    else {
        fclose(f1);
    }

    return 0;
}

