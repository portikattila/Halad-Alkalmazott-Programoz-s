#include <iostream>
#include <math.h>
#include <fstream>
#include "miniwindow.h"
#include "header.h"
#include "vector2d.h"
#include "gnuplot.h"


struct win
{
    MainWindow wnd;

    int n, N; // mérőszintek száma, részecskék száma 
    double  t, tmax, temperature, mass, radius, boxsize, h, g;

    int w1, w2; //ablak méretek
    
    std::vector<double> m1; // mérőszintek
    std::vector<double> m2; // mérőszintek
    // impulzus váltuzás
    double p_avg;
    std::vector<double> pm;
    std::vector<double> pV;
    std::vector<double> pt, pb; 

    std::vector<double> Km;
    std::vector<double> KV;
    std::vector<double> Kt, Kb; 

    std::vector<int> Nm;
    std::vector<int> NV;
    std::vector<int> Nt, Nb; 


    //mérés ideje
    double dt0, dt; 
    std::vector<double> t_mV;
    std::vector<double> t_m;
    

    bool stop1 = true;
    bool save1 = false;
    bool stop2 = true;
    bool save2 = false;
    bool close = false;
    

    win()
	{
        n = 40;
        N = 1000;
        
		t = 0., tmax = 600., temperature = 600., mass = 2.7e-26, 
        radius = 0.001, boxsize = 1, h = 0.00001, g = -1000000,
		w1 = 800, w2 = 600;

        m1.resize(n);
        m2.resize(n - 2);
        pm.resize(n);
        pt.resize(n);
        pb.resize(n);
        pV.resize(n - 2);
        Km.resize(n);
        Kt.resize(n);
        Kb.resize(n);
        KV.resize(n - 2);
        Nm.resize(n);
        Nt.resize(n);
        Nb.resize(n);
        NV.resize(n - 2);
                



        
        double l = boxsize / (n - 1);
        double ll = 0;

        for(unsigned i{0}; i < m1.size(); i++)
        {
            m1[i] = ll;
            ll += l;
        }

        ll = l;
        for(unsigned j{0}; j < m2.size(); j++)
        {
            m2[j] = ll;
            ll += l;
        }
                
        dt0 = 0.;
        dt = 0.;
    }

    int start()
    {

        double a = 0.5 * (w1 - w2);
        double c = w2 / boxsize;
        std::vector<vector2d<double>> data;
        int b = radius * w2 / boxsize;
        if (b < 1){b = 2;}


        auto an = [&](double t, std::vector<particle> & r)
        {
            data.clear();
            data.resize(r.size());
            for (unsigned i{0}; i < r.size(); i++)
            {
                vector2d<double> temp{r[i].x, r[i].y};
                data[i] = temp;
            }
        };
        int sd1 = 0, sd2 = 0, sd3 = 0, sd4 = 0;
        auto mp = [&](particle const& a)
        {
            const double k_b = 1.380648527979 * 1e-23;
            
            if((a.x + radius) >= boxsize)
            {
                p_avg = p_avg + std::abs(a.vx);
            }else
            if((a.x - radius) <= 0)
            {
                p_avg = p_avg + std::abs(a.vx);
            }


            if (t > dt0 && t > dt)
            {
                for (int i{0}; i < n; i++)
                {
                    if(std::abs(a.y - m1[i]) <= radius && a.vy > 0)
                    {
                        pm[i] = pm[i] + std::abs(a.vy);
                        Km[i] = Km[i] + (a.vx * a.vx + a.vy * a.vy);
                        Nm[i] = Nm[i] + 1;               
                    }
                }   
            }else
            if( t > dt && dt != dt0)
            {
                dt = dt0;
                save("data/data_1.dat", pm, Km, m1, 1, n);      
                sd1 = 1;    
            }

            if(!stop1)
            {
                save1 = true;
                for (int i{0}; i < n; i++)
                {
                    if(std::abs(a.y - m1[i]) <= radius && a.vy > 0)
                    {
                        pb[i] = pb[i] + std::abs(a.vy);
                        Kb[i] = Kb[i] + (a.vx * a.vx + a.vy * a.vy);
                        Nb[i] = Nb[i] + 1;  
                    }
                    
                    if(std::abs(a.y - m1[i]) <= radius && a.vy < 0)
                    {
                        pt[i] = pt[i] + std::abs(a.vy);
                        Kt[i] = Kt[i] + (a.vx * a.vx + a.vy * a.vy);
                        Nt[i] = Nt[i] + 1;
                    }
                }
            }else
            if (save1)
            {
                for (int i{0}; i < n - 1; i++)
                {
                    pb[i + 1] = mass / boxsize / (t_m[1] - t_m[0]) * pb[i + 1];
                    pt[i] = mass / boxsize / (t_m[1] - t_m[0]) * pt[i];

                    Kb[i + 1] = mass / 2 / k_b / Nb[i + 1] * Kb[i + 1];                
                    Kt[i] = mass / 2 / k_b / Nt[i] * Kt[i];                

                }
                

                save("data/data_2.dat", pb, Kb, m1, 1, n);
                save("data/data_3.dat", pt, Kt, m1, 0, n - 1);
                save1 = false;
                sd2 = 1, sd3 = 1;
            } 

            int m = n - 2;
            double b =  0.5 * boxsize;
            double d = boxsize /5/ m;
            double dx = 10 * boxsize /5/ m;
            if(!stop2)
            {
                save2 = true;
                for (int i{0}; i < m; i++)
                {
                   if((a.x <= b + dx) && (a.x >= b - dx))
                    {
                        if((std::abs(a.y - m2[i] - d) <= radius) && (a.vy > 0))
                        {
                            pV[i] = pV[i] + a.vy;
                            KV[i] = KV[i] + (a.vx * a.vx + a.vy * a.vy);
                            NV[i] = NV[i] + 1;
                        }

                        if((std::abs(a.y - m2[i] + d) <= radius) && (a.vy < 0))
                        {
                            pV[i] = pV[i] + std::abs(a.vy);
                            KV[i] = KV[i] + (a.vx * a.vx + a.vy * a.vy);
                            NV[i] = NV[i] + 1;
                        }
                    }
                    
                    if((a.y <= m2[i] + d) && (a.y >= m2[i] - d))
                    {
                        if((std::abs(a.x - b + dx)<= radius) && (a.vx > 0))
                        {
                            pV[i] = pV[i] + a.vx;
                            KV[i] = KV[i] + (a.vx * a.vx + a.vy * a.vy);
                            NV[i] = NV[i] + 1;
                        }

                        if((std::abs(a.x - b - dx) <= radius) && (a.vx < 0))
                        {
                            pV[i] = pV[i] + std::abs(a.vx);
                            KV[i] = KV[i] + (a.vx * a.vx + a.vy * a.vy);
                            NV[i] = NV[i] + 1;
                        }
                    }
                }
            }else
            if (save2)
            {
                for (int i{1}; i < n - 1; i++)
                {
                    pV[i] = mass / (2 * (dx + d)) / (t_mV[1] - t_mV[0]) * pV[i];                
                    KV[i] = mass / 2 / k_b / NV[i] * KV[i];                

                }
                save("data/data_4.dat", pV, KV, m2, 1, n - 2);
                sd4 = 1;

                save2 = false;
            }
        };

        std::vector<particle> r = initialize(N, boxsize, temperature, mass);
        wnd.window.eventDriven = false;
        wnd.mouseHandler([&](Mouse const& m)
		{   
            if(m.event == Mouse::Scroll    ){ g += 10 * m.dz; std::cout << "g = " << g << "\n";  }
			else if(m.event == Mouse::RightDown ){stop1 = !stop1; t_m.push_back(t);}
            else if(m.event == Mouse::LeftDown ){stop2 = !stop2; t_mV.push_back(t);}
            else if(m.event == Mouse::MiddleDown){close = !close; }
			});

		wnd.resizeHandler([&](int /*w*/, int /*h*/, StateChange /*sc*/){});

        wnd.idleHandler([&]
		{
            kernel(r, t, tmax, h, radius, boxsize, g, 1.0, close, an, mp);
		});
        
        wnd.renderHandler( [&](SoftwareRenderer& r)
		{
            auto black = [&](auto){return color(0, 0, 0);};
            r.forall_pixels([](auto, auto, auto){ return color(255, 255, 255); });
            if(close) 
            {
                r.line(0.5 * (w1 - w2), 0.5 * (w1 - w2), 0.5 * (w1 + w2), 0.5 * (w1 - w2), black);
            }
            r.line(0.5 * (w1 - w2), 0.5 * (w1 - w2), 0.5 * (w1 - w2), 0.5 * (w1 + w2), black);
            r.line(0.5 * (w1 + w2), 0.5 * (w1 + w2), 0.5 * (w1 + w2), 0.5 * (w1 - w2), black);
            r.line(0.5 * (w1 + w2), 0.5 * (w1 + w2), 0.5 * (w1 - w2), 0.5 * (w1 + w2), black);
            

            if (!stop1)
            {
                for(unsigned i{0}; i < m1.size(); i++)
                {
                    r.line(0.5 * (1.7 * w1 - w2), a + c * (boxsize - m1[i]), 
                           0.5 * (1.8 * w1 - w2),  a + c * (boxsize - m1[i]), 
                           [&](auto){return color(255, 0, 0);});

                    r.line(0.5 * (1 * w1 - w2), a + c * (boxsize - m1[i]), 
                           0.5 * (1.2 * w1 - w2),  a + c * (boxsize - m1[i]), 
                           [&](auto){return color(255, 0, 0);});
                    
                    r.line(0.5 * (2.3 * w1 - w2), a + c * (boxsize - m1[i]), 
                           0.5 * (2.5 * w1 - w2),  a + c * (boxsize - m1[i]), 
                           [&](auto){return color(255, 0, 0);});
                }
            }
            if (!stop2)
            {
                double d = boxsize /5 / (n - 2);
                double cx1 = 0.5 * boxsize + 10 * d;
                double cx2 = 0.5 * boxsize - 10 * d;

                for(unsigned i{0}; i < m2.size(); i++)
                {
      
                    r.line(a + c * cx2, a + c * (boxsize - (m2[i] + d)), 
                           a + c * cx1, a + c * (boxsize - (m2[i] + d)),
                           [&](auto){return color(255, 0, 0);});
                    r.line(a + c * cx1, a + c * (boxsize - (m2[i] + d)), 
                           a + c * cx1, a + c * (boxsize - (m2[i] - d)),
                           [&](auto){return color(255, 0, 0);});
                    r.line(a + c * cx1, a + c * (boxsize - (m2[i] - d)), 
                           a + c * cx2, a + c * (boxsize - (m2[i] - d)),
                           [&](auto){return color(255, 0, 0);});
                    r.line(a + c * cx2, a + c * (boxsize - (m2[i] - d)), 
                           a + c * cx2, a + c * (boxsize - (m2[i] + d)),
                           [&](auto){return color(255, 0, 0);});
                
                }
            }

            for (unsigned i{0}; i < data.size(); i++)
            {   
                int x2 = a + c * data[i].x;
                int y2 = a + c * (boxsize - data[i].y);    
                r.ellipse(x2, y2, b, b, [&](auto){return color(0, 0, 205);});
            }
		});

        wnd.exitHandler([&]
        {
            gnuplot GNUPLOT;
            GNUPLOT("cd \"data\" ");
            GNUPLOT("set term png");
            GNUPLOT("set key autotitle columnheader");
            GNUPLOT("set grid");
            
            if (sd1 == 1)
            {
                GNUPLOT("set output \"pre1.png\" ");
                GNUPLOT("plot \"data_1.dat\" u 1:2 with linespoints ");
                 GNUPLOT("set output \"temp1.png\" ");
                GNUPLOT("plot \"data_1.dat\" u 1:3 with linespoints ");
            
            }
            if (sd2 == 1)
            {
                GNUPLOT("set output \"pre2.png\" ");
                GNUPLOT("plot \"data_2.dat\" u 1:2 with linespoints ");
                 GNUPLOT("set output \"temp2.png\" ");
                GNUPLOT("plot \"data_2.dat\" u 1:3 with linespoints ");
            }
            if (sd3 == 1)
            {
                GNUPLOT("set output \"pre3.png\" ");
                GNUPLOT("plot \"data_3.dat\" u 1:2 with linespoints ");
                GNUPLOT("set output \"temp3.png\" ");
                GNUPLOT("plot \"data_3.dat\" u 1:3 with linespoints ");
            }
            if (sd4)
            {
                GNUPLOT("set output \"pre4.png\" ");
                GNUPLOT("plot \"data_4.dat\" u 1:2 with linespoints ");
                GNUPLOT("set output \"temp4.png\" ");
                GNUPLOT("plot \"data_4.dat\" u 1:3 with linespoints ");
            }

            std::ofstream ofile("data/parameters.dat", std::ios::out | std::ios::app); 
            ofile << "number of particles: N = " << N << "\n";
            ofile << "mass of particle: m = " << mass << " kg" << "\n";
            ofile << "radius of particles: r = " << radius << " m" << "\n";
            ofile << "initial temparature:  T = " << temperature << " K" << "\n";
            ofile << "size of simulation box: l = " << boxsize << 'm'<< "\n";
            ofile << "gravitational coefficient: g = " << g << "m*s^2" <<"\n";
            ofile << "time of simulation: t = " << t << "s" <<"\n";
            ofile << "average pressure: p = " << mass / boxsize / t * p_avg << "N/m" <<"\n";
            ofile << "pressure from ideal gas equation: p = " 
                  << 8.3144598 / (6.0221e23) * N * temperature / (boxsize * boxsize) << "N/m" <<"\n";
            ofile << "pressure from van der waals equation: p = " 
                  << 8.3144598 / (6.0221e23) * N * temperature /((boxsize * boxsize) - N * radius *radius) << "N/m" <<"\n";


        });    

        bool res = wnd.open(L"sym", {0, 0}, {w1, w1}, true, [&]{ return true; });
		return res ? 0 : -1;
	}


};
int main() 
{
    win a; 
    a.start();
}
