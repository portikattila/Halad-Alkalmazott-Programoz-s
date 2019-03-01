#include <iostream>
#include <cmath>

double integrate(int n, double x0, double x1);
double fun(double x);

int main()
{
    std::cout.precision(16);

    double x0, x1;
    double a = 1.34638795680345037669816;

    std::cout << "A cos(x)exp(-x^2) integrálja: " << std::endl;
    std::cout << "Az egzakt értéktől való eltérés x0 = -1 és x1 = 3 esetén: " << std::endl;

    for(int i = 1; i < 10; i++)
    {
        int n = i * 100;
        double I = integrate(n, -1, 3);
        double delta = std::abs(I - a);

        std::cout << "n =  ";
        std::cout << n;

        std::cout << "  I = ";
        std::cout << I;
        std::cout <<"  Egzakt eredménytől való eltérés: ";
        std::cout << delta << std::endl;
    }

    std::cout << "x0 = ";
    std::cin >> x0;
    std::cout << "x1 = ";
    std::cin >> x1;

    if (x0 > x1)
    {
        double a = x0;
        x0 = x1;
        x1 = a;
    }
    std::cout << integrate(100000, x0, x1);



    return 0;
}

double fun(double x)
{
    return cos(x) * std::exp(-x * x);
}

double integrate(int n, double x0, double x1)
{
    double sum = fun(x1) + fun(x0);
    double h = (x1 - x0) / n;

    for(int i = 1; i <= n - 1; i++)
    {
        sum += 2 * fun(x0 + i * h);
    }

    return (h / 2) * sum;
}
