#include "opt.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define max2(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define MaxDP 3300

int (*Dp)[MaxDP];

int editdistDP(char *str1, char *str2, int str1Len, int str2Len, int maxDist);
static int min3(int a, int b, int c);

inline int min3(int a, int b, int c) {
    int min = a < b ? a : b;
    return min < c ? min : c;
}

int editdistDP(char *str1, char *str2, int str1Len, int str2Len, int maxDist) {
    int i, j;
    int len1, len2;
    static int flag = 0;

    if (flag == 0) {
        Dp = (int(*)[MaxDP])malloc(MaxDP * MaxDP * sizeof(int));
        flag = 1;
    }

    len1 = str1Len + 1;
    len2 = str2Len + 1;

    for (j = 0; j <= len2; j++) {
        Dp[0][j] = j;
    }
    for (i = 0; i <= len1; i++) {
        Dp[i][0] = i;
    }
    for (i = 1; i <= len1; i++) {
        for (j = 1; j <= len2; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                Dp[i][j] = Dp[i - 1][j - 1];
            } else {
                int result =
                    1 + min3(Dp[i][j - 1], Dp[i - 1][j], Dp[i - 1][j - 1]);
                Dp[i][j] = result;
            }
        }

        int exceed = 1;
        for (j = 1; j <= len2; j++) {
            if (Dp[i][j] <= maxDist) {
                exceed = 0;
                break;
            }
        }

        if (exceed) {
            return INT_MAX;
        }
    }
    return Dp[len1][len2];
}

int wagner_fischer_O2(char *s1, char *s2, int maxDist) {
    int i, j, m, M, len1, len2;
    int Mx, mx, dia, top, left, *buf;
    char *p1;

    len1 = strlen(s1);
    len2 = strlen(s2);
    if (len1 < len2)
        return wagner_fischer_O2(s2, s1, maxDist);
    if (len2 == 0)
        return len1;
    if (len2 == 1)
        return len1 - (memchr(s1, s2[0], len1) != NULL);
    if (len2 == 2) {
        p1 = memchr(s1, s2[0], len1 - 1);
        if (p1) {
            return len1 - (index(p1 + 1, s2[1]) != NULL) - 1;
        } else {
            return len1 - (index(s1 + 1, s2[1]) != NULL);
        }
    }

    buf = malloc((len2 + 1) * sizeof(int));
    if (buf == NULL)
        return -1;

    Mx = (len2 - 1) / 2;
    mx = 1 - Mx - (len1 - len2);

    for (j = 0; j <= Mx; j++)
        buf[j] = j;

    for (i = 1; i <= len1; i++) {
        buf[0] = i - 1;

        m = MAX(mx, 1);
        M = MIN(Mx, len2);
        mx++;
        Mx++;

        dia = buf[m - 1];
        top = buf[m];

        if (s1[i - 1] != s2[m - 1])
            dia = MIN(dia, top) + 1;

        buf[m] = dia;
        left = dia;
        dia = top;

        for (j = m + 1; j <= M; j++) {
            top = buf[j];

            if (s1[i - 1] != s2[j - 1])
                dia = MIN(MIN(dia, top), left) + 1;
            buf[j] = dia;
            left = dia;
            dia = top;
        }

        int exceed = 1;
        for (j = m + 1; j <= M; j++) {
            if (buf[j] <= maxDist) {
                exceed = 0;
                break;
            }
        }

        if (exceed) {
            return INT_MAX;
        }

        if (len2 == M)
            continue;

        if (s1[i - 1] != s2[M])
            dia = MIN(dia, left) + 1;
        buf[M + 1] = dia;
    }
    dia = buf[len2];
    free(buf);
    return dia;
}