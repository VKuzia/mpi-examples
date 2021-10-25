#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    // Инициализация MPI
    int32_t process_count, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Объявление основных данных
    size_t N = 100;  // размер матрицы
    double *A = NULL;  // вещественная матрица
    double *B = NULL;  // вещественный вектор
    double *C = NULL;  // вектор-результат AB
    double start;  // метка для измерения времени

    if (rank == 0) {  // master
        FILE *file = fopen("lab1_input", "r");  // открываем файл с А и В

        // Аллоцируем память и инициализируем А, В, С
        fscanf(file, "%zd", &N);
        A = malloc((N * N) * sizeof(double));
        B = malloc(N * sizeof(double));
        C = malloc(N * sizeof(double));
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < N; j++) {
                fscanf(file, "%lf", &A[i * N + j]);
            }
        }
        for (size_t i = 0; i < N; i++) {
            fscanf(file, "%lf", &B[i]);
        }
        fclose(file);
    }

    // Транслируем всем процессам полученное root значение N и аллоцируем В в дочерних процессах
    MPI_Bcast(&N, 1, MPI_UINT32_T, 0, MPI_COMM_WORLD);
    if (rank != 0) {
        B = malloc(N * sizeof(double));
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        start = MPI_Wtime();  // root замеряет время вычислений, включая пересылку параметров
    }
    MPI_Bcast(B, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // каждый процесс принимает свои part_lines_count строк матрицы
    int32_t part_lines_count = N / process_count;
    double buffer_a[N * part_lines_count];
    MPI_Scatter(A, N * part_lines_count, MPI_DOUBLE, buffer_a, N * part_lines_count, MPI_DOUBLE, 0,
                MPI_COMM_WORLD);

    // непосредственно вычисляем частичный результат.
    double buffer_c[N * part_lines_count];
    for (size_t i = 0; i < part_lines_count; i++) {
        buffer_c[i] = 0;
        for (size_t j = 0; j < N; j++) {
            buffer_c[i] += buffer_a[i * N + j] * B[j];
        }
    }

    // собираем все частичные ответы в вектор С
    MPI_Gather(buffer_c, part_lines_count, MPI_DOUBLE, C, part_lines_count, MPI_DOUBLE, 0,
               MPI_COMM_WORLD);

    // root процесс выводит результаты в файл с ответом и логирует время исполнения
    if (rank == 0) {  // master
        double finish = MPI_Wtime();
        FILE *output_file = fopen("lab1_output", "w");
        for (size_t i = 0; i < N; i++) {
            fprintf(output_file, "%lf\n", C[i]);
        }
        fclose(output_file);
        FILE *log_file = fopen("lab1_log", "a");
        fprintf(log_file, "%zd [%d]: %lf\n", N, process_count, finish - start);
        fclose(log_file);
        free(A);
        free(C);
    }
    free(B);
    MPI_Finalize();  // не забываем очищать выделенные ресурсы
    return 0;
}



