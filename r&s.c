// Afraid of the dark? No need, you got me!
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define ROCK_AND_STONE 18
#define WE_ARE_RICH 107
#define LET_THERE_BE_LIGHT 1024 * 1024

#define selectMission open
#define Mining_Expedition O_RDONLY
#define mission_t int
#define mine mmap
#define Mission_Selection_Terminal "codes.txt"
#define fighting printf
#define return_to_drop_pod return 0
#define deep_rock_galactic main
#define gg int
#define terrain_scan strchr

#define Mactera_Grabber "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s"
#define Cave_Leech "\n%s %s %s %s %s"
#define Glyphid_Septic_Spreader "\n%s %s "
#define Glyphid_Acid_Spitter "\n%s %s %s %s %s %s"
#define Glyphid_Exploder "\n%s %s "
#define Glyphid_Web_Spitter "\n%s %s %s "
#define Glyphid_Stingtail "\n%s %s "
#define Glyphid_Bulk_Detonator "\n%s %s "

void store(char *molly);

char minerals[WE_ARE_RICH][ROCK_AND_STONE] = {{0}};

gg deep_rock_galactic(void) {
    mission_t mission =
        selectMission(Mission_Selection_Terminal, Mining_Expedition);

    char *molly =
        mine(NULL, LET_THERE_BE_LIGHT, PROT_READ, MAP_PRIVATE, mission, 0);

    // Lighting up!
    store(molly);

    // i == 0

    fighting(Mactera_Grabber, minerals[0], minerals[8], minerals[11],
             minerals[31], minerals[32], minerals[52], minerals[54],
             minerals[59], minerals[62], minerals[73], minerals[78],
             minerals[84], minerals[85], minerals[86], minerals[95],
             minerals[97], minerals[106]);

    // i = 6
    fighting(Cave_Leech, minerals[6], minerals[61], minerals[69], minerals[76],
             minerals[88]);

    // i = 17
    fighting(Glyphid_Septic_Spreader, minerals[17], minerals[98]);
    // i = 22
    fighting(Glyphid_Acid_Spitter, minerals[22], minerals[57], minerals[71],
             minerals[94], minerals[102], minerals[104]);

    // i = 23
    fighting(Glyphid_Exploder, minerals[23], minerals[64]);

    // i = 34
    fighting(Glyphid_Web_Spitter, minerals[34], minerals[46], minerals[68]);

    // i = 36
    fighting(Glyphid_Stingtail, minerals[36], minerals[72]);

    // i = 39
    fighting(Glyphid_Stingtail, minerals[39], minerals[51]);

    // i = 55
    fighting(Glyphid_Stingtail, minerals[55], minerals[58]);
    // i = 79
    fighting(Glyphid_Stingtail, minerals[79], minerals[106]);

    // i = 90
    fighting(Glyphid_Bulk_Detonator, minerals[90], minerals[106]);

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
        char *currentBegin = current;

        current = terrain_scan(current, '\r');

        memcpy(minerals[currentObjective], currentBegin,
               (current - currentBegin));
        minerals[currentObjective][current - currentBegin] = '\0';
        currentObjective++;

        current = terrain_scan(current, '\f');
        // skip '\f'
        current++;
    }
}