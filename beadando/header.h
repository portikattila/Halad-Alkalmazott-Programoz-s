#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <string>
//------------------------------------------------------------------------------------------------------
template<typename State, typename T, typename RHS> 
void rk4_step(State & y, T & t, T t1, T h, RHS f)
{
	if(t + h > t1){ h = t1 - t; }
	State k1 = f(t, y);
	State k2 = f(t + h * (T)0.5, y + (h * (T)0.5) * k1);
	State k3 = f(t + h * (T)0.5, y + (h * (T)0.5) * k2);
	State k4 = f(t + h,          y +  h           * k3);
	
    y = y + (k1 + k4 + (T)2 * (k2 + k3)) * (h / (T)6);
	t = t + h;
}
//------------------------------------------------------------------------------------------------------
struct  particle
{
    double x;
    double y;
    double vx;
    double vy;
};

particle operator+(particle const& a, particle const& b)
{
    particle temp;
    temp.x = a.x + b.x;
    temp.y = a.y + b.y;
    temp.vx = a.vx + b.vx;
    temp.vy = a.vy + b.vy;
    return temp;
}

particle operator-(particle const& a, particle const& b)
{
    particle temp;
    temp.x = a.x - b.x;
    temp.y = a.y - b.y;
    temp.vx = a.vx - b.vx;
    temp.vy = a.vy - b.vy;
    return temp;
}

template<typename T>
particle operator* (particle const& a, T const& b)
{
    particle temp;
    temp.x = b * a.x;
    temp.y = b * a.y;
    temp.vx = b * a.vx;
    temp.vy = b * a.vy;
    return temp;
}

template<typename T>
particle operator* (T const& b, particle const& a)
{
    particle temp;
    temp.x = b * a.x;
    temp.y = b * a.y;
    temp.vx = b * a.vx;
    temp.vy = b * a.vy;
    return temp;
}

std::vector<particle> operator+ (std::vector<particle> const& a, particle const& b)
{
    std::vector<particle> temp;
    int N = static_cast<int>(a.size()); 
    temp.resize(N);
    for(int i{0}; i < N; i++)
    {
        temp[i] = a[i] + b;
    }
    return temp;
}

std::vector<particle> operator+ (std::vector<particle> const& a, std::vector<particle> const& b)
{
    if(a.size() != b.size()){std::cout << "error in operator +"; exit(-1);}
    int N = static_cast<int>(a.size());
    std::vector<particle> temp;
    temp.resize(N);
    for (int i{0}; i < N; i++)
    {
        temp[i] = a[i] + b[i];
    }
    return temp;
}

template<typename T>
std::vector<particle> operator* (std::vector<particle> const& a, T const& b)
{
    std::vector<particle> temp;
    int N = static_cast<int>(a.size());
    temp.resize(N);
    for(int i{0}; i < N; i++)
    {
        temp[i] = a[i] * b;
    }
    return temp;
}

template<typename T>
std::vector<particle> operator* (T const& b, std::vector<particle> const& a)
{
    std::vector<particle> temp;
    int N = static_cast<int>(a.size());
    temp.resize(N);
    for(int i{0}; i < N; i++)
    {
        temp[i] = b * a[i];
    }
    return temp;
}
//-----------------------------------------------------------------------------------------------------
template<typename T>
void box(particle & a, T boxsize, T r)
{ 
    if((a.x + r) >= boxsize)
    {
        a.x = r;
    }else
    if((a.x - r) <= 0)
    {
        a.x = boxsize - r;
    }

    if((a.y - r) <= 0)
    {
        a.vy = -1 * a.vy;
        a.y = a.y + r;
    }
}

template<typename T>
void collision(particle & a, particle & b, T r)
{
    double dx = std::abs(a.x - b.x);
    double dy = std::abs(a.y - b.y);
    if( dx <= 2 * r && dy <= 2 * r )
    {   
        particle temp = a - b;
        double alfa = (temp.vx * temp.x + (temp).vy * (temp).y) / ((temp).x * (temp).x + (temp).y * (temp).y);
        double beta = ((-1 * temp).vx * (-1 * temp).x + (-1 * temp).vy * (-1 * temp).y) / ((-1 * temp).x * (-1 * temp).x + (-1 * temp).y * (-1 * temp).y);
        a.vx = a.vx - alfa * temp.x;
        a.vy = a.vy - alfa * temp.y;
        b.vx = b.vx + beta * temp.x;
        b.vy = b.vy + beta * temp.y;
  } 
}

template<typename T>
std::vector<particle> initialize(int N, double boxsize, T temperature, T mass)
{
    const double k = 1.380648527979 * 1e-23; //(J/K)

    std::random_device rd{};
    std::mt19937 gen(rd());   
    std::uniform_real_distribution<double> uniform(0, boxsize);
    std::normal_distribution<double> maxwell(0, std::sqrt(k * temperature / mass));
    
    std::vector<particle> p;
    p.resize(N); 
    for (int i{0}; i < N; i++)
    {
        p[i] = {uniform(gen), uniform(gen), maxwell(gen), maxwell(gen)};
    }
    return p;
}


//------------------------------------------------------------------------------------------------------
template<typename T, typename animation, typename measurement>
void kernel(std::vector<particle> & r, T & t, T tmax, T h, double radius, T boxsize, double g, double h0,  bool close, animation an, measurement mp)
{
        auto gravity = [&](double t, std::vector<particle> const& r)
        {
        unsigned n = r.size();
            std::vector<particle> dr;
            dr.resize(n);
            for(unsigned i{0}; i < n; i++)
            {
                dr[i].x = r[i].vx;
                dr[i].y = r[i].vy;
                dr[i].vx = 0;
                dr[i].vy = g * 1 / ((h0 + r[i].y) * (h0 + r[i].y));
            }
            return dr;
        };

        rk4_step(r, t, tmax, h, gravity);
        unsigned i{0};
        while (i < r.size())
        {
            mp(r[i]);
            box(r[i], boxsize, radius);

            if( r[i].y + radius >= boxsize)
            {
                if (close)
                {
                    r[i].vy = - r[i].vy;
                }
                else
                {
                    r.erase(r.begin() + i); 
                    i--;
                }

                if(r.empty())
                {
                    std::cout << "number of particles = 0" << '\n'<< "t = " << t;
                    
                    exit(-1);

                }
            }    
        i++;
        }

        for (unsigned k{0}; k < r.size(); k++)
        {
            for (unsigned j{k + 1}; j < r.size(); j++)
            {
                collision(r[k], r[j], radius);
            }
        } 
        an(t, r);
}



    void save(std::string data, std::vector<double> const& a, std::vector<double> const& b, 
              std::vector<double> const& m, unsigned j, unsigned n)
    {   
        std::ofstream ofile(data, std::ios::out | std::ios::app); 
        ofile << "x[m]" << ' ' << "p(x)[N/m]" << ' ' << "T(x)[K]" << '\n';
        for (unsigned i{j}; i < n; i++)
        {
            ofile << m[i] << ' ' << a[i] << ' ' << b[i] << '\n';
        }
        ofile << '\n';
    }