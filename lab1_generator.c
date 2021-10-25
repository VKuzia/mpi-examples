#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int32_t MAX = 100;
const int32_t MIN = 5;

double GetRandomDouble() {
    int32_t a = rand() % (MAX - MIN) + MIN;
    int32_t b = rand() % (MAX - MIN) + MIN;
    return (double) a / b;
}

int main() {
    srand(time(NULL));
    size_t n;
    scanf("%zd", &n);
    FILE* file = fopen("lab1_input", "w");
    fprintf(file, "%zd\n", n);
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            fprintf(file, "%lf ", GetRandomDouble());
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n");
    for (size_t i = 0; i < n; i++) {
        fprintf(file, "%lf ", GetRandomDouble());
    }
    fprintf(file, "\n");
    return 0;
}