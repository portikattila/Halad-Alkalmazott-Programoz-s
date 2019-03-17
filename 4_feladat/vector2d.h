#include <iostream>
#include <cmath>

template<typename T>
T sq( T x){ return x*x; }

template<typename T>
struct vector2d
{
    T x, y;
    vector2d& operator+=(vector2d const& v)
    {
        x += v.x;
        y += v.y;

        return  *this;
    }
    
    vector2d& operator-=(vector2d const& v)
    {
        x -= v.x;
        y -= v.y;

        return  *this;
    }

    vector2d& operator*=(T const& c)
    {
        x *= c;
        y *= c;

        return  *this;
    }

    vector2d& operator/=(T const& c)
    {
        x /= c;
        y /= c;

        return  *this;
    }
    
};

template<typename T>
std::ostream& operator<<(std::ostream& o, vector2d<T> const& v)
{
    o << '[' << v.x << ',' << v.y << ']';
    return o; 
}

template<typename T>
std::istream& operator>>(std::istream& i, vector2d<T>& v)
{
    i >> v.x;
    i >> v.y;
    return i; 
}

template<typename T>
vector2d<T> operator+(vector2d<T> const& a, vector2d<T> const& b)
{
    return vector2d<T>{ a.x + b.x, a.y +b.y};
}

template<typename T>
vector2d<T> operator-(vector2d<T> const& a, vector2d<T> const& b)
{
    return vector2d<T>{ a.x - b.x, a.y  - b.y};
}

template<typename T1, typename T2>
vector2d<T1> operator*(vector2d<T1> const& a, T2 const& b)
{
    vector2d<T1> v = {a.x * b, a.y * b};
    return v;
}

template<typename T1, typename T2>
vector2d<T1> operator*(T2 const& a, vector2d<T1> const& b)
{
    vector2d<T1> v = {a* b.x, a * b.y};
    return v;
}

template<typename T1, typename T2>
vector2d<T1> operator/(vector2d<T1> const& a, T2 const& b)
{
    vector2d<T1> v = {a.x / b, a.y / b};
    return v;
}


template<typename T1, typename T2>
double dot(vector2d<T1> const& a, vector2d<T2> const& b)
{
    return a.x * b.x + a.y * b.y;
}

template<typename T>
double sqlength(vector2d<T> const& v)
{
    double sql = sq(v.x) + sq(v.y);
}

template<typename T>
double length(vector2d<T> const& v)
{
    double l = std::sqrt(sqlength(v));
}

template<typename T> 
vector2d<T> normalize(vector2d<T> const& v)
{
     return v / length(v);
}

