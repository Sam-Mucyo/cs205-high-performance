#include <iostream>
#include <omp.h>

static long n_steps = 100000;

int main(){
    int i; double x, pi, sum = 0.0;
    int num_threads = omp_get_num_threads();
    long local_steps = steps / num_threads;
    double sums[num_threads];

    double start = omp_get_wtime();

    #pragma omp parallel
    {
        int th = omp_get_num_thread();
        double step = 1.0 / (double) n_steps;

        for (i = 0; i <= local_steps; ++i){
            x = step * (i + 0.5);
            sum = sum + 4.0 / (1.0 + x * x);
        }
    }

    double end = omp_get_wtime();

    pi = step * sum;

    printf("PI: %f (took %f sec)\n", pi, end - start);
}