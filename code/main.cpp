#include "Euler_Maruyama.hpp"
#include "Ornstein_Uhlenbeck.hpp"
#include "file_io.hpp"
#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iterator>

int main(int argc, char* argv[])
{
    // Initialize MPI

    int rank,size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double t1,t2;

    MPI_Barrier(MPI_COMM_WORLD);
    t1 = MPI_Wtime();

    double y_0 = 0.0;
    time_params params = {0.0,4.0,1000};
    parameters OU_params = {0.7,1.5,0.6};
    int N_total = 2;
    int N_time;
    int N_simulations;

    std::vector<double> results;

    MPI_File file_handle = NULL;
    MPI_File_delete("./example.bin",MPI_INFO_NULL);
    MPI_File_open(MPI_COMM_WORLD,"./example.bin",MPI_MODE_CREATE|MPI_MODE_WRONLY,MPI_INFO_NULL,&file_handle);
    int offset = 0;
    int buffsize = 0;

    MPI_Request request;

    if (rank == 0)
    {
    Ornstein_Uhlenbeck OU(OU_params);

    Euler_Maruyama test(&params,y_0);

    test.run_simulation(&OU);

    N_time = test.get_time().size();

    std::ofstream time_file("./time.txt");
    write_line(test.get_time(),time_file);

    N_simulations = N_total;

    results = std::vector<double> (1,0.0);

    if (size == 1)
    {
        std::ofstream output_file("./example.bin", std::ios::binary);
        //double t1 = omp_get_wtime();
        #pragma omp parallel shared(y_0,params,OU)
        {
            #pragma omp for
            for (size_t i = 0; i < N_simulations; i++)
            {
                Euler_Maruyama test_2(&params,y_0);

                test_2.run_simulation(&OU);

                #pragma omp critical
                {
                    write_line_bn(test_2.get_y_copy(),output_file);
                }
            }

        }
        //double t2 = omp_get_wtime();
        //printf("Time per iteration: %f \n",(t2-t1)/N_simulations);
    }

    if (size > 1)
    {
        int N_lower,N_remainder;
        N_lower = N_simulations/(size-1);
        N_remainder = N_simulations%(size-1);

        std::vector<int> N_sim = std::vector<int> (size, N_lower);
        N_sim[0] = 0;

        for (size_t i = 1; i < N_remainder+1; i++)
        {
            N_sim[i] += 1;
        }

        for (size_t i = 1; i < size; i++)
        {
            //printf("%d \n",N_sim[i]);
            MPI_Send(&N_sim[i],1,MPI_INT,i,0,MPI_COMM_WORLD);
            MPI_Send(&N_time,1,MPI_INT,i,0,MPI_COMM_WORLD);

            buffsize = N_time*N_sim[i]*sizeof(double);
            MPI_Send(&offset,1,MPI_INT,i,0,MPI_COMM_WORLD);
            MPI_Send(&buffsize,1,MPI_INT,i,0,MPI_COMM_WORLD);
            offset += buffsize;

            //Now these parameters are sent separately, but they could be grouped
        }

        buffsize = 0;
        printf("Hello!\n");
    }
    }

    else{
        MPI_Recv(&N_simulations,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(&N_time,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(&offset,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(&buffsize,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_File_set_size(file_handle,sizeof(double)*N_total*N_time);

    MPI_File_set_view(file_handle,offset,MPI_DOUBLE,MPI_DOUBLE,"native",MPI_INFO_NULL);

    if (rank>0)
    {
        results = std::vector<double> (N_simulations*N_time,0.0);
        std::vector<double> y_temp;

        printf("MPI process %d will do %d simulations\n", rank, N_simulations);

        Ornstein_Uhlenbeck OU_mpi(OU_params);

        for (size_t i = 0; i < N_simulations; i++)
        {
            Euler_Maruyama test_mpi(&params,y_0);

            test_mpi.run_simulation(&OU_mpi);
            y_temp = test_mpi.get_y();
            for (size_t j = 0; j < N_time; j++)
            {
                results[i*N_time + j] = y_temp[j];
            }

        }

        //MPI_File_iwrite(file_handle,&results[0],N_simulations*N_time,MPI_DOUBLE,&request);

        printf("MPI process %d has finished!\n", rank);
    }


    MPI_Barrier(MPI_COMM_WORLD);
    MPI_File_write_all(file_handle,&results[0],buffsize/sizeof(double),MPI_DOUBLE,MPI_STATUS_IGNORE);
    MPI_File_close(&file_handle);

    MPI_Barrier(MPI_COMM_WORLD);
    t2 = MPI_Wtime();
    if (rank == 0)
    {
        printf("Time per iteration: %f (s/iter)\n",(t2-t1)/N_total);
        printf("Total wall time: %f s\n",t2-t1);
    }

    // Finalize MPI
    MPI_Finalize();


    return 0;
}