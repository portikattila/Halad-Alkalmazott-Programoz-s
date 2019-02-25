#include <iostream>
#include <math.h>

double sqrt_newton(double num, double x0);
double step(double num, double x);

int main()
{
    double num, x0;

    std::cout.precision(16);
    std::cout << "Number: ";
    std::cin >> num;

    std::cout << "The initial value: ";
    std::cin >> x0;

    double root = sqrt_newton(num, x0);
    std::cout << "Square root: ";
    std::cout << root;

    return 0;
}

double step(double num, double x)
{
    double der = 2 * x;
    double fun = x * x -num;

    return x - fun / der;
}


double sqrt_newton(double num, double x0)
{

    for (int i = 0; i <= 100; i++)
    {
        x0 = step(num, x0);
    }

    return x0;
}
