#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <poll.h>
#include <sys/mman.h>
#define BUFFER_SIZE 256
#define min(x,y) (x<y?x:y)
typedef struct Line Line;
struct Line {
        int len;
        int offset;
};


Line* buildTable(char *file, off_t fileLen, int *curLine) {
        int maxLines = 256;
        Line* lines = (Line*)malloc(sizeof(Line) * maxLines);
        if(lines == NULL) {
                perror("malloc failed");
                return NULL;
        }
        *curLine = 0;
        lines[0].offset = 0;
        lines[0].len = 0;

        char c;
        int n;
        for(off_t b = 0; b < fileLen; b++) {
                c = file[b];
                if(*curLine + 1 == maxLines) {
                        maxLines+=256;
                        Line *newLines = realloc(lines, maxLines);
                        if(newLines == NULL) {
                                free(lines);
                                perror("realloc failed");
                                return NULL;
                        }
                        lines = newLines;
                }
                lines[*curLine].len++;
                if(c == '\n') {
                        lines[(*curLine) + 1].offset = lines[*curLine].offset + lines[*curLine].len;
                        lines[(*curLine) + 1].len = 0;
                        (*curLine)++;
                }
        }
        return lines;
}

void printTable(Line* lines, int linesCnt) {
        for(int i = 0; i < linesCnt; i++) {
                printf("len = %d, offset = %d\n", lines[i].len, lines[i].offset);
        }
}


char *createMappedFile(char *fileName, off_t *fileLen) {
        int desc = open(fileName, O_RDONLY);
        if(desc == -1) {
                perror(fileName);
                return NULL;
        }
        *fileLen = lseek(desc, 0, SEEK_END);
        if(*fileLen == - 1) {
                close(desc);
                perror("lseek failed");
                return NULL;
        }
        char *mappedFile = mmap(NULL, *fileLen, PROT_READ, MAP_PRIVATE, desc, 0);
        if(mappedFile == MAP_FAILED) {
                close(desc);
                perror("mmap failed");
                return NULL;
        }
        close(desc);
        return mappedFile;
}

int main(int argc, char **argv) {
        if(argc < 2) {
                fprintf(stderr, "need filename\n");
                return -1;
        }
        off_t fileLen = 0;
        char* mappedFile = createMappedFile(argv[1], &fileLen);
        if(mappedFile == NULL) {
                fprintf(stderr, "mapping failed");
                return -1;
        }
        int linesCnt = 0;
        Line* lines = buildTable(mappedFile, fileLen, &linesCnt);
        if(lines == NULL) {
                return -1;
        }
        printf("lines = %d\n", linesCnt);
        printTable(lines, linesCnt);

        int idx = 0;
        struct pollfd fds[1];
        fds[0].fd = 0;
        fds[0].events = POLLIN;

        do {
                printf("enter line number\n");

                int ready = poll(fds, 1, 5000);
                if(ready == -1) {
                        perror("poll failed");
                        break;
                }
                if(ready == 0) {
                        printf("timeout\n");
                        write(2, mappedFile, fileLen);
                        break;
                }

                int v = scanf("%d", &idx);
                if (v != 1 || idx < 0) {
                        fprintf(stderr, "incorrect input\n");
                        fflush(stdin);
                        continue;
                }
                if (idx > linesCnt) {
                        fprintf(stderr, "number too big\n");
                        continue;
                }
                if (idx == 0){
                        printf("stopped\n");
                        break;
                }
                write(2, mappedFile + lines[idx-1].offset, lines[idx-1].len);
        } while(1);
        munmap(mappedFile, fileLen);
        free(lines);
        return 0;
}

