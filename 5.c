#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <omp.h>

#define SIZE 5

char *words[SIZE] = {"lorem", "ipsum", "dolo", "sit", "amet"};
int counts[SIZE] = {0};

void lower(char *s) {
    for (int i = 0; s[i]; i++)
        s[i] = tolower((unsigned char)s[i]);
}

int getCount(char *fileName, char *key) {
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) return -1;

    char word[100];
    int count = 0;
    char search[100];

    strcpy(search, key);
    lower(search);

    while (fscanf(fp, "%99s", word) == 1) {
        lower(word);

        if (strcmp(word, search) == 0)
            count++;
    }

    fclose(fp);
    return count;
}

int main() {
    char fileName[100];
    int threads;

    printf("Enter file name: ");
    scanf("%99s", fileName);

    printf("Enter number of threads: ");
    scanf("%d", &threads);

    omp_set_num_threads(threads);

    double t = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < SIZE; i++)
        counts[i] = getCount(fileName, words[i]);

    t = omp_get_wtime() - t;

    printf("\nWord frequencies:\n");
    for (int i = 0; i < SIZE; i++)
        printf("%s : %d\n", words[i], counts[i]);

    printf("Time taken: %f seconds\n", t);

    return 0;
}
