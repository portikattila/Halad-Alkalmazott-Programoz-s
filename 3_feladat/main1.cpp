#include <iostream>
#include <vector>
#include <array>
#include <numeric>
#include <algorithm>

template<typename T>
T square( T x){ return x*x; }

double average(std::vector<double> const& v)
{
    double n = static_cast<double>(v.size());
    return accumulate( v.begin(), v.end(), 0.0) / n;
}

double squared_average(std::vector<double> const& v)
{
     double n = static_cast<double>(v.size());
     return accumulate( v.begin(), v.end(), 0.0, [](double a, double b)->double{ return a + square(b);}) / n;
}

double f_product(std::vector<double> const& x, std::vector<double> const& y)
{
    double avx = average(x);
    double avy = average(y);

    return std::inner_product(x.begin(), x.end(), y.begin(), 0.0,
                              [](double a, double b)->double{ return a + b;},
                              [avx, avy](double a, double b)->double{ return (a - avx) * (b - avy); });

}


std::array<double, 3> fit(std::vector<double> const& x, std::vector<double> const& y)
{
    std::array<double, 3> r;
    r[0] = f_product(x, y) / f_product(x, x);
    r[1] = average(y) - r[0]  * average(x);

     double n = static_cast<double>(x.size());

    r[2] = square(std::inner_product(x.begin(), x.end(), y.begin(), 0.0) /  n - average(x) * average(y)) /
                                    ((squared_average(x) - square(average(x))) * (squared_average(y) - square(average(y)))) ;

    return r;
}




int main()
{

    std::vector<double> proba1(100);
    std::iota(proba1.begin(), proba1.end(), 0);
    std::vector<double> proba2(100);
    std::iota(proba2.begin(), proba2.end(), 0);

    std::array<double, 3> r =  fit(proba1, proba2);

    std::cout << "Teszt eset:" << std::endl;
    std::cout << " Egyenes ( f(x) = a + b*x ) illesztése az f(x) = x függvénynek megfelelő, \n természetes számokból alkotott adatsorokra." << std::endl;
    std::cout << "b = " << r[0] <<std::endl;
    std::cout << "a = " << r[1] <<std::endl;
    std::cout << "r^2 = " << r[2];
    return 0;
}


