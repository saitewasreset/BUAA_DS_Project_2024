#include <stdio.h>
#include <string.h>

#define MAX_CODES_LEN 1048576
#define PROGRAM_COUNT 107
#define MAX_PROGRAM_ID_LEN 18

void generateProgramList(char *buffer);

char programList[PROGRAM_COUNT][MAX_PROGRAM_ID_LEN] = {0};

int main(void) {
    char buffer[MAX_CODES_LEN] = {0};
    FILE *f = fopen("codes.txt", "r");

    fread(buffer, sizeof(char), MAX_CODES_LEN, f);
    generateProgramList(buffer);

    // i == 0

    printf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", programList[0],
           programList[8], programList[11], programList[31], programList[32],
           programList[52], programList[54], programList[59], programList[62],
           programList[73], programList[78], programList[84], programList[85],
           programList[86], programList[95], programList[97], programList[106]);

    // i = 6
    printf("\n%s %s %s %s %s", programList[6], programList[61], programList[69],
           programList[76], programList[88]);

    // i = 17
    printf("\n%s %s ", programList[17], programList[98]);
    // i = 22
    printf("\n%s %s %s %s %s %s", programList[22], programList[57],
           programList[71], programList[94], programList[102],
           programList[104]);

    // i = 23
    printf("\n%s %s ", programList[23], programList[64]);

    // i = 34
    printf("\n%s %s %s ", programList[34], programList[46], programList[68]);

    // i = 36
    printf("\n%s %s ", programList[36], programList[72]);

    // i = 39
    printf("\n%s %s ", programList[39], programList[51]);

    // i = 55
    printf("\n%s %s ", programList[55], programList[58]);
    // i = 79
    printf("\n%s %s ", programList[79], programList[106]);

    // i = 90
    printf("\n%s %s ", programList[90], programList[106]);

    return 0;
}

inline void generateProgramList(char *buffer) {
    size_t currentIndex = 0;

    char *current = buffer;
    while (*current != '\0') {
        while (*current == ' ' || *current == '\n' || *current == '\r' ||
               *current == '\t') {
            current++;
        }
        char *currentBegin = current;
        while (*current != '\r') {
            current++;
        }

        memcpy(programList[currentIndex], currentBegin,
               (current - currentBegin));
        programList[currentIndex][current - currentBegin] = '\0';
        currentIndex++;

        while (*current != '\f') {
            current++;
        }
        // skip '\f'
        current++;
    }
}