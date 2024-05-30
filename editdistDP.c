#include <stdlib.h>
#include <string.h>

#define max2(a, b) ((a) > (b) ? (a) : (b))

#define MaxDP 3300

int (*Dp)[MaxDP];

int editdistDP(char *str1, char *str2);
int min3(int a, int b, int c);

inline int min3(int a, int b, int c) {
    int min = a < b ? a : b;
    return min < c ? min : c;
}

int editdistDP(char *str1, char *str2) {
    int i, j;
    int len1, len2;
    static int flag = 0;

    if (flag == 0) {
        Dp = (int(*)[MaxDP])malloc(MaxDP * MaxDP * sizeof(int));
        flag = 1;
    }

    len1 = (int)strlen(str1) + 1;
    len2 = (int)strlen(str2) + 1;

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