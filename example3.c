#include <mpi.h>
#include <stdio.h>

struct Point {
    double x, y, z;
};

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Datatype dt_point;
    MPI_Type_contiguous(3, MPI_DOUBLE, &dt_point);
    MPI_Type_commit(&dt_point);

    int points_count = 10;
    struct Point data[points_count];
    if (rank == 0) {
        for (int i = 0; i < points_count; ++i) {
            data[i].x = (double) i;
            data[i].y = (double) -i;
            data[i].z = (double) i * i;
        }
        MPI_Send(data, points_count, dt_point, 1, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(data, points_count, dt_point, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; i < points_count; ++i) {
            printf("Point #%d : (%lf, %lf, %lf)\n", i, data[i].x, data[i].y, data[i].z);
        }
    }
    MPI_Type_free(&dt_point);
    MPI_Finalize();
    return 0;
}
