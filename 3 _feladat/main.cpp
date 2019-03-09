#include <iostream>
#include <vector>
#include <array>
#include <numeric>
#include <algorithm>

double average(std::vector<double> const& v)
{
    int n = static_cast<int>(v.size());
    return accumulate( v.begin(), v.end(), 0.0) / n;
}

std::vector<double> subtract(std::vector<double> const& v, auto a)
{
    std::vector<double> r = v;
    std::transform(r.begin(), r.end(), r.begin(), std::bind2nd(std::plus<double>(), a));
    return r;
}

double product (std::vector<double> const& x, std::vector<double> const& y)
    {
        return std::inner_product(x.begin(), x.end(), y.begin(), 0.0);
    }

std::vector<double> square_vec(std::vector<double> const& v)
{
    std::vector<double> r = v;
    std::transform(r.begin(), r.end(), r.begin(), [](double x)->double{ return x*x; });
    return r;
}
auto square(auto x)
{
    return x * x;
}

std::array<double, 3> fit(std::vector<double> const& x, std::vector<double> const& y)
{
    double ya = average(y);
    double xa = average(x);
    std::vector<double> ys = subtract(y, ya);
    std::vector<double> xs = subtract(x, xa);

    std::array<double, 3> r;
    r[0] = product(xs, ys) / product(xs, xs);
    r[1] = ya - r[0]  * xa;
    int n = static_cast<int>(x.size());
    r[2] = square((product(x, y)) / n - average(x) * average(y)) /
     ((average(square_vec(x)) - square(average(x))) * (average(square_vec(y)) - square(average(y))));



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


