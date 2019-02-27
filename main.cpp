#include <iostream>
#include <cmath>


template <typename T, typename f1, typename f2, typename f3> 
double newton(f1 f, f2 der, f3 test, T x0)
{
    T x1;
    do
    {
        x1 = x0;
        x0 = x0 - f(x0) / der(x0);
    }while(test(x1,x0));
    
    return x0;
}

int main(int, char**)
{
    std::cout.precision(9);
    std::cout << newton([](double x){ return x*x - 612.0; }, [](double x){ return 2.0 * x; }, [](double x1, double x0 ){ return std::abs(x1 - x0) > 1e-8; }, 1.);
    return 0;
}
