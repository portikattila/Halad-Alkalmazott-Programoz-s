#include <iostream>
#include <cmath>
#include <sstream>

template<typename T>
T sq( T x){ return x*x; }

template<typename T>
struct vector2d
{
    T x, y;

    vector2d<T>& operator=(vector2d<T> const& v)
    {
        x = v.x;
        y = v.y;
        return *this;
    }

    template<typename T2>
    vector2d<T>& operator+=(vector2d<T2> const& v)
    {
        x += (T2)v.x;
        y += (T2)v.y;

        return  *this;
    }
    
    template<typename T2>
    vector2d<T>& operator-=(vector2d<T2> const& v)
    {
        x -= (T2)v.x;
        y -= (T2)v.y;

        return  *this;
    }

    template<typename T2>
    vector2d<T>& operator*=(T2 const& c)
    {
        x *= (T2)c;
        y *= (T2)c;

        return  *this;
    }

    template<typename T2>
    vector2d<T>& operator/=(T2 const& c)
    {
        x /= (T2)c;
        y /= (T2)c;

        return  *this;
    }
    
};

template<typename T>
std::ostream& operator<<(std::ostream& o, vector2d<T> const& v)
{
    o << v.x << ',' << v.y;
    return o; 
}

template<typename T>
std::istream& operator>>(std::istream& i, vector2d<T>& v)
 {
    std::string tmp; 
    std::getline(i, tmp, ',');
    std::istringstream ss1(tmp);
    T num1;
    ss1 >> num1;
    v.x = num1;
    
    std::getline(i, tmp);
    std::istringstream ss2(tmp);
    T num2;
    ss2 >> num2;
    v.y = num2;
    return i;
 
}

template<typename T1, typename T2>
auto operator+(vector2d<T1> const& a, vector2d<T2> const& b)
{
    using T3 = decltype(a.x+b.x);
    return vector2d<T3>{ a.x + b.x, a.y +b.y};
}

template<typename T1, typename T2>
auto operator-(vector2d<T1> const& a, vector2d<T2> const& b)
{
    using T3 = decltype(a.x - b.x);
    return vector2d<T3>{ a.x - b.x, a.y  - b.y};
}

template<typename T1, typename T2>
auto operator*(vector2d<T1> const& a, T2 const& b)
{
    using T3 = decltype(b * a.x);
    return vector2d<T3>{a.x * b, a.y * b};
}

template<typename T1, typename T2>
auto operator*(T1 const& a, vector2d<T2> const& b)
{
    using T3 = decltype( a * b.x);
    return vector2d<T3>{a * b.x, a * b.y};
}

template<typename T1, typename T2>
auto operator/(vector2d<T1> const& a, T2 const& b)
{
    using T3 = decltype( a.x / b);
    return vector2d<T3> {a.x / b, a.y / b};
}


template<typename T1, typename T2>
auto dot(vector2d<T1> const& a, vector2d<T2> const& b)
{
    return a.x * b.x + a.y * b.y;
}

template<typename T>
auto sqlength(vector2d<T> const& v)
{
    if(v.x == 0 && v.y == 0)
    {
        std::cout << "nullvektor";
        exit(-1);
    }
    return sq(v.x) + sq(v.y);
}

template<typename T>
auto length(vector2d<T> const& v)
{
    return std::sqrt(sqlength(v));
}

template<typename T> 
auto normalize(vector2d<T> const& v)
{
    return v / length(v);
}

template<typename T> 
vector2d<T> abs(vector2d<T> const& v)
{
    return vector2d<T>{std::abs(v.x), std::abs(v.y)};
}

template<typename T1,typename T2>
auto rotation(vector2d<T1> const& v, T2 a)
{
    auto s = std::sin(a);
    auto c = std::cos(a);
    using  T3 = decltype(s * v.x);
    return vector2d<T3>{v.x * c - v.y * s, v.x * s + v.y * c};
}