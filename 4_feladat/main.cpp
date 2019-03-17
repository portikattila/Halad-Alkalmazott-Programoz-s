#include "vector2d.h"

int main(int, char**) 
{
    vector2d<double>  a{};
    std::cin >> a;
    vector2d<double>  b = {2.0, 3.0};
    vector2d<double>  c = {1.0, 1.0};
    a += b;
    b -= c;
    a *= 3.0;
    a /= 3.0;
    vector2d<double> v1 = a + b;
    vector2d<double> v2 = a - b;
    vector2d<double> v3 = 2 * a;
    vector2d<double> v4 = a * 2;
    vector2d<double> v5 = a / 2;
    double m =  dot(a,b);
    double sql = sqlength(a);
    double l = length(a);
    vector2d<double>n = normalize(a);


   std::cout << n;
}