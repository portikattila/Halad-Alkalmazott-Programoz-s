#include <iostream>
#include <math.h>

double integrate(int n, double x0, double x1);
double fun(double x);

int main()
{
    std::cout.precision(16);

    double x0, x1;\
    std::cout << "x0 = ";
    std::cin >> x0;
    std::cout << "x1 = ";
    std::cin >> x1;

    for(int i = 1; i <= 10; i++)
    {
        int n = i * 100000;
        double I = integrate(n, x0, x1);

        std::cout << "n =  ";
        std::cout << n;

        std::cout << "  I = ";
        std::cout << I << std::endl;
    }

    return 0;
}

double fun(double x)
{
    return std::cos(x) * std::exp(-x * x);
}

double integrate(int n, double x0, double x1)
{
    double sum = fun(x1) + fun(x0);

    for(int i = 1; i <= n - 1; i++ )
    {
        sum += 2 * fun(x0 + i *((x1 - x0) / n));
    }

    return ((x1 - x0) / (2 * n)) * sum;
}
