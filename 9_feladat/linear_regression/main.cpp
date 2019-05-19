#include <iostream>
#include <vector>
#include <array>
#include <numeric>
#include <algorithm>
/*
auto add = [](auto x, auto y){ return x + y;};
auto id = [](auto x){ return x;};
*/

double average(std::vector<double> const& v)
{
    int n = static_cast<double>(v.size());
    if(n == 0){ std::cerr <<"Üres adatvektor";};
    auto sum = accumulate( v.begin(), v.end(), 0.0);
    /*auto policy = std::execution::par;
    auto  sum = std::transform_reduce(policy, v.begin(), v.end(), 0.0, add, id);*/
    return sum / n;
}


std::array<double, 2> fit(std::vector<double> const& x, std::vector<double> const& y)
{
    double ya = average(y);
    double xa = average(x);

    std::array<double, 2> r;
    double temp1 = std::inner_product(x.begin(), x.end(), y.begin(), 0.0,
                             [](double a, double b){return a + b;},
                             [xa, ya](double a, double b){return (a - xa)*(b - ya);});

    double temp2 = std::inner_product(x.begin(), x.end(), x.begin(), 0.0,
                                     [](double a, double b){return a + b;},
                                     [xa](double a, double b){return (a - xa)*(b - xa);});

    r[0] = temp1 / temp2;
    r[1] = ya - r[0]  * xa;


    return r;
}

int main()
{
    return 0;
}
