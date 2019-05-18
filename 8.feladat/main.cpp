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

    std::ofstream ofile("data.txt", std::ios::out | std::ios::app);
    ofile << "\"Mátrix mérete\"" << " " << "Szekvenciális[ns]" << " " << "Párhuzamos[ns]" <<  '\n'; 
    for(int k{40}; k <= 60; k += 1)
    {
        int N = k;
        long long min1{0};
        long long min2{0};
        for(int j{0}; j < 1000; j++)
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
            auto m1 = mat1 * mat2;
            auto t2 = std::chrono::high_resolution_clock::now();
            auto m2 = multiplication(mat1, mat2);
            auto t3 = std::chrono::high_resolution_clock::now();

            long long delta1 = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
            long long delta2 = std::chrono::duration_cast<std::chrono::nanoseconds>(t3 - t2).count();
            
            if(min1 == 0){min1 = delta1;}
            else if (delta1 < min1){min1 = delta1;}

            if(min2 == 0){min2 = delta2;}
            else if (delta2 < min2){min2 = delta2;}
        }
        ofile << N << " " << min1 << " " << min2 <<  '\n'; 
    }
}

