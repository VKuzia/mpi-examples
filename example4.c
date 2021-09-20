#include <stdio.h>
#include <mpi.h>

struct Person {
    int age;
    double height;
    char name[10];
};

struct Person DEFAULT_PERSON = {
        19, 1.78, "Kuzia"
};

int main(int argc, char *argv[]) {
    int size;
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Datatype person_type;
    int lengths[3] = {1, 1, 10};
    MPI_Aint displacements[3] = {offsetof(struct Person, age),
                                 offsetof(struct Person, height),
                                 offsetof(struct Person, name)};

    MPI_Datatype types[3] = {MPI_INT, MPI_DOUBLE, MPI_CHAR};
    MPI_Type_create_struct(3, lengths, displacements, types, &person_type);
    MPI_Type_commit(&person_type);

    if (rank == 0) {
        struct Person buffer = DEFAULT_PERSON;
        printf("MPI process %d sends person:\n\t- age = %d\n\t- height = %f\n\t- name = %s\n", rank, buffer.age,
               buffer.height, buffer.name);
        MPI_Send(&buffer, 1, person_type, 1, 0, MPI_COMM_WORLD);
    } else {
        struct Person received;
        MPI_Recv(&received, 1, person_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("MPI process %d received person:\n\t- age = %d\n\t- height = %f\n\t- name = %s\n", rank, received.age,
               received.height, received.name);
    }
    MPI_Type_free(&person_type);
    MPI_Finalize();
    return 0;
}