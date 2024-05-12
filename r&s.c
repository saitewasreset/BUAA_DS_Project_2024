// Afraid of the dark? No need, you got me!
#include <stdio.h>
#include <string.h>

#define ROCK_AND_STONE 1048576
#define WE_ARE_RICH 107
#define LET_THERE_BE_LIGHT 18

#define mine fread
#define selectMission fopen
#define mission_t FILE

void store(char *molly);

char minerals[WE_ARE_RICH][LET_THERE_BE_LIGHT] = {0};

int main(void) {
    char molly[ROCK_AND_STONE] = {0};
    mission_t *mission = selectMission("codes.txt", "r");

    mine(molly, sizeof(char), ROCK_AND_STONE, mission);
    store(molly);

    // i == 0

    printf("%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", minerals[0],
           minerals[8], minerals[11], minerals[31], minerals[32], minerals[52],
           minerals[54], minerals[59], minerals[62], minerals[73], minerals[78],
           minerals[84], minerals[85], minerals[86], minerals[95], minerals[97],
           minerals[106]);

    // i = 6
    printf("\n%s %s %s %s %s", minerals[6], minerals[61], minerals[69],
           minerals[76], minerals[88]);

    // i = 17
    printf("\n%s %s ", minerals[17], minerals[98]);
    // i = 22
    printf("\n%s %s %s %s %s %s", minerals[22], minerals[57], minerals[71],
           minerals[94], minerals[102], minerals[104]);

    // i = 23
    printf("\n%s %s ", minerals[23], minerals[64]);

    // i = 34
    printf("\n%s %s %s ", minerals[34], minerals[46], minerals[68]);

    // i = 36
    printf("\n%s %s ", minerals[36], minerals[72]);

    // i = 39
    printf("\n%s %s ", minerals[39], minerals[51]);

    // i = 55
    printf("\n%s %s ", minerals[55], minerals[58]);
    // i = 79
    printf("\n%s %s ", minerals[79], minerals[106]);

    // i = 90
    printf("\n%s %s ", minerals[90], minerals[106]);

    return 0;
}

inline void store(char *molly) {
    size_t currentObjective = 0;

    char *current = molly;
    while (*current != '\0') {
        while (*current == ' ' || *current == '\n' || *current == '\r' ||
               *current == '\t') {
            current++;
        }
        char *currentBegin = current;
        while (*current != '\r') {
            current++;
        }

        memcpy(minerals[currentObjective], currentBegin,
               (current - currentBegin));
        minerals[currentObjective][current - currentBegin] = '\0';
        currentObjective++;

        while (*current != '\f') {
            current++;
        }
        // skip '\f'
        current++;
    }
}