// Afraid of the dark? No need, you got me!
// Avg: 6.43651 std: 1.17552
// Avg: 6.55407 std: 1.19309
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define LET_THERE_BE_LIGHT 1024 * 1024
#define INVENTORY_CAPACITY 6
#define CREDIT 327

#define selectMission open
#define Mining_Expedition O_RDONLY
#define mission_t int
#define mine mmap
#define Mission_Selection_Terminal "codes.txt"
#define return_to_drop_pod return 0
#define deep_rock_galactic main
#define gg int
#define terrain_scan strchr
#define deposit fwrite

void store(char *molly);

int nitraPos[106] = {0,   -1,  -1,  -1,  -1,  -1,  120, -1,  7,   -1,  -1,  14,
                     -1,  -1,  -1,  -1,  -1,  156, -1,  -1,  -1,  -1,  171, 214,
                     -1,  -1,  -1,  -1,  -1,  -1,  -1,  21,  28,  -1,  229, -1,
                     251, -1,  -1,  266, -1,  -1,  -1,  -1,  -1,  -1,  236, -1,
                     -1,  -1,  -1,  273, 35,  -1,  42,  281, -1,  178, 288, 49,
                     -1,  127, 56,  -1,  221, -1,  -1,  -1,  243, 134, -1,  185,
                     258, 63,  -1,  -1,  141, -1,  70,  296, -1,  -1,  -1,  -1,
                     77,  84,  91,  -1,  148, -1,  311, -1,  -1,  -1,  192, 98,
                     -1,  105, 163, -1,  -1,  -1,  199, -1,  206, -1};

char mollyStorage[CREDIT] = {0};

gg deep_rock_galactic(void) {
    mission_t mission =
        selectMission(Mission_Selection_Terminal, Mining_Expedition);

    char *molly =
        mine(NULL, LET_THERE_BE_LIGHT, PROT_READ, MAP_PRIVATE, mission, 0);

    // Lighting up!
    store(molly);

    mollyStorage[119] = '\n';
    mollyStorage[155] = '\n';
    mollyStorage[170] = '\n';
    mollyStorage[213] = '\n';
    mollyStorage[228] = '\n';
    mollyStorage[250] = '\n';
    mollyStorage[265] = '\n';
    mollyStorage[280] = '\n';
    mollyStorage[295] = '\n';
    mollyStorage[310] = '\n';
    mollyStorage[325] = '\n';

    deposit(mollyStorage, sizeof(char), CREDIT, stdout);
    // First!
    return_to_drop_pod;
}

inline void store(char *molly) {

    size_t currentObjective = 0;

    char *current = molly;
    while (*current != '\0') {
        while (*current == ' ' || *current == '\n' || *current == '\r' ||
               *current == '\t') {
            current++;
        }

        if (currentObjective != 106) {
            if (nitraPos[currentObjective] != -1) {
                memcpy(mollyStorage + nitraPos[currentObjective], current,
                       INVENTORY_CAPACITY);
                mollyStorage[nitraPos[currentObjective] + INVENTORY_CAPACITY] =
                    ' ';
            }
        } else {
            memcpy(mollyStorage + 112, current, INVENTORY_CAPACITY);
            mollyStorage[118] = ' ';
            memcpy(mollyStorage + 303, current, INVENTORY_CAPACITY);
            mollyStorage[309] = ' ';
            memcpy(mollyStorage + 318, current, INVENTORY_CAPACITY);
            mollyStorage[324] = ' ';
        }
        currentObjective++;

        current = terrain_scan(current, '\f');
        // skip '\f'
        current++;
    }
}