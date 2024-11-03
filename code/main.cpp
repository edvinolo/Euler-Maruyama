#include "Euler_Maruyama.hpp"
#include "Ornstein_Uhlenbeck.hpp"
#include "file_io.hpp"
#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <fstream>
#include <iterator>

int main(int argc, char* argv[])
{
    // Initialize MPI

    int rank,size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double y_0 = 0.0;
    time_params params = {0.0,4.0,1000};
    parameters OU_params = {0.7,1.5,0.6};

    if (rank == 0)
    {
    Ornstein_Uhlenbeck OU(OU_params);

    Euler_Maruyama test(&params,y_0);

    test.run_simulation(&OU);

    std::ofstream output_file("./example.txt");

    double t1 = omp_get_wtime();
    //#pragma omp parallel shared(y_0,params,OU)
    {
        //#pragma omp for
        for (size_t i = 0; i < 5; i++)
        {
            Euler_Maruyama test_2(&params,y_0);

            test_2.run_simulation(&OU);

            write_line(test_2.get_y(),output_file);

        }

    }
    double t2 = omp_get_wtime();
    printf("Time per iteration: %f \n",(t2-t1)/10);

    std::vector<double> test_y = test.get_y();
    }

    printf("\n");
    printf("Switching to MPI stuff ...\n"); 

    Ornstein_Uhlenbeck OU_mpi(OU_params);

    Euler_Maruyama test_mpi(&params,y_0);

    test_mpi.run_simulation(&OU_mpi);

    // Finalize MPI
    MPI_Finalize();


    return 0;
}