#include <iostream>
#include <cmath>

double sqrt_newton(double num, double x0);
double step(double num, double x);

int main()
{
    double num, x0;

    std::cout.precision(9);
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
    double tol = 1e-8;
    double x1;
    do
    {
        x1 = x0;
        x0 = step(num, x0);
    }while(std::abs(x1 - x0) >= tol);

    return x0;
}
