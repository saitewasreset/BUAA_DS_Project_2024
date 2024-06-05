#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define ROCK_INFINITY 10000
#define MAX_DP 3303

int *buf = NULL;

int rockEditDistance(char *s1, char *s2, int strLen1, int strLen2,
                     int maxDist) {
    // Rock and stone, forever!

    int i, j, dia, tmp, *buf;
    int len1, len2;

    len1 = strLen1;
    len2 = strLen2;

    if (len1 > len2) {
        return rockEditDistance(s2, s1, strLen2, strLen1, maxDist);
    }

    if (buf == NULL) {
        buf = malloc(MAX_DP * sizeof(int));
    }

    int Mx = maxDist;
    int mx = -maxDist;

    int resultDiag = len2 - len1;

    if (resultDiag > maxDist) {
        return ROCK_INFINITY;
    }

    if (buf == NULL)
        return -1;

    int M = MIN(Mx, len2);
    int m = MAX(mx, 0);
    int r = MIN(resultDiag, len2);

    for (j = m; j <= M; j++)
        buf[j] = j;
    buf[M + 1] = maxDist + 1;

    for (i = 1; i <= len1; i++) {
        Mx++;
        mx++;
        resultDiag++;

        M = MIN(Mx, len2);
        m = MAX(mx, 1);
        r = MIN(resultDiag, len2);

        if (m == 1) {
            dia = i - 1;
            buf[0] = i;
        } else {
            dia = buf[j - 1];
            buf[m - 1] = maxDist + 1;
        }

        for (j = m; j <= M; j++) {
            tmp = buf[j];

            if (s1[i - 1] == s2[j - 1]) {
                buf[j] = dia;
            } else {
                buf[j] = MIN(MIN(buf[j], buf[j - 1]), dia) + 1;
            }
            dia = tmp;
        }

        if (buf[r] > maxDist) {
            return ROCK_INFINITY;
        }

        buf[M + 1] = maxDist + 1;
    }

    return buf[len2];
}