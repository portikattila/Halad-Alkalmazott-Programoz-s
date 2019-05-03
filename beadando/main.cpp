#include <iostream>
#include <math.h>
#include <fstream>
#include "miniwindow.h"
#include "header.h"
#include "vector2d.h"


struct win
{
    MainWindow wnd;

    int n, N; // mérőszintek száma, részecskék száma 
    double  t, tmax, temperature, mass, radius, boxsize, h, g;

    int w1, w2; //ablak méretek
    
    std::vector<double> m; // mérőszintek
    std::vector<double> p; // impulzus váltuzás

    double dt0, dt; //mérés ideje
    bool stop = false;

    win()
	{
        n = 20;
        N = 100000;
        
		t = 0., tmax = 600., temperature = 300., mass = 2.7e-26, 
        radius = 0.0000005, boxsize = 2, h = 0.000001 , g = -100,
		w1 = 800, w2 = 600;
        
        p.resize(n);
        m.resize(n);
        for( int i{0}; i < n; i++)
        {
            m[i] = i * boxsize / (n - 1);
        }
        
        dt0 = 0.;
        dt = 1.0;
    }

    int start()
    {
        std::vector<particle> r = initialize(N, boxsize, temperature, mass);
        std::vector<vector2d<double>> data;
        wnd.window.eventDriven = false;
        wnd.mouseHandler([&](Mouse const& m)
		{   
            if(m.event == Mouse::Scroll    ){ g += m.dz; std::cout << "g = " << g << "\n";  }
			else if(m.event == Mouse::RightDown ){stop = true;}
            });

		wnd.resizeHandler([&](int /*w*/, int /*h*/, StateChange /*sc*/){});
        auto an = [&](double t, std::vector<particle> r)
        {
            data.clear();
            data.resize(r.size());
            for (unsigned i{0}; i < r.size(); i++)
            {
                vector2d<double> temp{r[i].x, r[i].y};
                data[i] = temp;
            }
        };

        auto mp = [&](particle const& a)
        {
            if(t < dt && !stop)
            {
                for (int i{0}; i < n; i++)
                {
                    if(std::abs(a.y - m[i]) <= radius)
                    {
                        p[i] = p[i] + std::abs(a.vy);
                        
                    }
                }
            }else
            {
                std::ofstream ofile("data.txt", std::ios::out | std::ios::app); 
                for (int i{0}; i < n; i++)
                {
                    ofile << p[i] << ' ' << m[i] << '\n';
                }
                exit(-1);  
            } 
        };

        wnd.idleHandler([&]
		{

            kernel(r, t, tmax, h, radius, boxsize, g, an, mp);
		});
        wnd.exitHandler([&]
        {
            std::ofstream ofile("data.txt", std::ios::out | std::ios::app); 
            for (int i{0}; i < n; i++)
            {
                ofile << p[i] << ' ' << m[i] << '\n';
            }
             
        });    
        wnd.renderHandler( [&](SoftwareRenderer& r)
		{
            auto black = [&](auto){return color(0, 0, 0);};
            r.forall_pixels([](auto, auto, auto){ return color(255, 255, 255); });
            r.line(0.5 * (w1 - w2), 0.5 * (w1 - w2), 0.5 * (w1 - w2), 0.5 * (w1 + w2), black);
            r.line(0.5 * (w1 + w2), 0.5 * (w1 + w2), 0.5 * (w1 + w2), 0.5 * (w1 - w2), black);
            r.line(0.5 * (w1 + w2), 0.5 * (w1 + w2), 0.5 * (w1 - w2), 0.5 * (w1 + w2), black);
            
            for (unsigned i{0}; i < data.size(); i++)
            {
                int x2 = 0.5 * (w1 - w2) + w2 / boxsize * data[i].x;
                int y2 = 0.5 * (w1 - w2) + w2 / boxsize * (boxsize - data[i].y);    
                r.ellipse(x2, y2, 1, 1, [&](auto){return color(0, 0, 205);});
            }
		});

        bool res = wnd.open(L"sym", {42, 42}, {w1, w1}, true, [&]{ return true; });
		return res ? 0 : -1;
	}


};


int main() 
{
    win a;
    a.start();
}
