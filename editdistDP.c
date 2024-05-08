#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max2(a, b) ((a) > (b) ? (a) : (b))
int **Dp, MaxDP = 3300; // for dynamic programming
int min3(int a, int b, int c) {
    int min = a < b ? a : b;
    return min < c ? min : c;
}
int error2(char *s) {
    fprintf(stderr, "%s\n", s);
    exit(-1);
}
int initDP() {
    int i;
    Dp = (int **)malloc(MaxDP * sizeof(int *));
    for (i = 0; i < MaxDP; i++)
        Dp[i] = (int *)malloc(MaxDP * sizeof(int));
    return 0;
}
int editdistDP(char *str1, char *str2) {
    int i, j;
    int len1, len2;
    static int flag = 0;

    (flag++) ? 1 : initDP();
    len1 = strlen(str1) + 1;
    len2 = strlen(str2) + 1;
    (max2(len1, len2) >= MaxDP) ? error2("DP memory error!") : len1;
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
    }
    return Dp[len1][len2];
}
