#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include "matrix.h"

int main(int, char**) 
{
    std::random_device rd{};
    std::mt19937 gen(rd());   
    std::normal_distribution<double> distr(0, 10);

    for(int k{10}; k <= 500; k += 5)
    {
        int N = k;
        long long t{0};
        for(int j{0}; j < 50; j++)
        {
            std::vector<double> temp1;
            std::vector<double> temp2;
            for(int i{0}; i < N * N; i++)
            {
                temp1.push_back(distr(gen));
                temp2.push_back(distr(gen));
            }
            matrix<double> mat1{N, temp1};
            matrix<double> mat2{N, temp2};
            auto t1 = std::chrono::high_resolution_clock::now();
            mat1 * mat2;
            auto t2 = std::chrono::high_resolution_clock::now();
            long long delta = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
            if(t == 0){ t = delta;}
            else if (delta < t){t = delta;}
            
        }
        std::ofstream ofile("data.txt", std::ios::out | std::ios::app); 
        ofile << N << " " << t << '\n'; 
    }
    
}
