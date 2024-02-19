#include <iostream>
#include <omp.h>
#include <cstdio>
#include <chrono>
#include <thread>


void sleep(int ms){
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void print_ten()
{
    #pragma omp parallel for 
    for (int i = 0; i < 10; i++)
    {
        printf("num: %i (th: %i)\n", i, omp_get_thread_num());
    }
}




// int main(){
//     // print_ten();
// }

int main (void)
{
    int b=0;
#pragma omp parallel set_num_threads(4)
    {
    for(int i=0;i<2;i++){
#pragma omp atomic
    b+=1;
        }
    }
    for(int i = i+1;i<8;i++){
        b+=1;
        }
        
#pragma omp parallel for
    for(int i=0; i<3; i++){
#pragma omp atomic
        b+=1;
        }
    std::cout <<"B: "<<b<<'\n';
}

// int main(int argc, char *argv[])
// {
//     float sum = 0.0;
//     int N = 1000;

//     #pragma omp parallel for private(sum)
//     for (int i = 0; i; < N; i++) {
//         sum += 1;
//     }
//     std::cout << sum << std::endl;
// }