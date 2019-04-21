#include <cmath>
#include <vector>
#include <algorithm>
#include <numeric>
#include <initializer_list>
#include <string>
#include <iostream>
#include <sstream> 
#include <iterator>
#include "matrix.h"

int main(int,char**)
{
    auto err = [](auto str){ std::cout << "Matrix.h error in: " << str << "\n";};// std::exit(-1); };

	{
		matrix<double> u;
		if(u.size() != 0){ err("default contructor test [size]");}
        if(u.N() != 0){ err("default contructor test [dim]");}
		if(u.begin() != u.end()){ err("default contructor test [begin == end]");}
		if(u.cbegin() != u.cend()){ err("default contructor test [cbegin == cend]"); }
	}
    
   
    {
		matrix<double> a = {2, {3.1, 5.2, 9.3, 7.5}};
		matrix<double> b{a};
		if(a(0, 0) != 3.1 || a(0, 1) != 5.2 || a(1, 0) != 9.3 || a(1,1) != 7.5)
        { 
           err("copy constructor test [src elements]");
        }
      	if(! check(a, b)){ err("copy constructor test [elements]");}
    }

    {
    	matrix<double> a = {2, {3.1, 5.2, 9.3, 7.5}};
		matrix<double> b{ std::move(a)};
		if(a.size() != 0){ err("move constructor test [src size]");}  
        if(!a.N()){ err("move constructor test [src dim]");}         
		if(a.begin() != a.end()){ err("move constructor test [src begin == src end]"); }
		if(b.size() != 4 || b.N() != 2 ){ err("move constructor test [size]");}
        if(b.N() != 2 || b.N() != 2 ){ err("move constructor test [dim]");}
		if(b(0, 0) != 3.1 || b(0, 1)!= 5.2 || b(1, 0) != 9.3 || b(1, 1) != 7.5)
        { 
            err("move constructor test [elements]");       
        }
	}
    	
    {
		matrix<int> a{ [](int i, int j){ return i + j;}, 2};
		if(a.size() != 4){ err("constructor from indexible test [size]");}
        if(a.N() != 2){ err("constructor from indexible test [dim]");}
		if(a(0, 0) != 0 || a(0,1) != 1 || a(1, 0) != 1 || a(1,1) != 2)
        {
             err("constructor from indexible test [elements]"); 
        }
	}

    {
		matrix<double> a = {2, {3.1, 5.2, 9.3, 7.5}};
		matrix<double> b;
		b = a;
		if(b.size() != 4 || b.N() != 2){ err("assignment test [size]");}
		if(b(0,0)!= 3.1 || b(0,1) != 5.2 || b(1, 0) != 9.3 || b(1, 1) != 7.5)
        { 
            err("assignment test [elements]");
        }
		if(a.size() != 4){ err("assignment test [src size]");}
		if(a.N() != 2){ err("assignment test [src dim]");}
		if(a(0, 0) != 3.1 || a(0,1) != 5.2 || a(1, 0) != 9.3 || a(1, 1) != 7.5)
        { 
            err("assignment test [src elements]"); 
        }
	}

	{
		matrix<double> c = {2, {3.1, 5.2, 9.3, 7.5}};
		c = c;
		if(c.size() != 4 || c.N() != 2){ err("self assignment test [size]");}
		if(c(0,0) != 3.1 || c(0,1) != 5.2 || c(1,0) != 9.3 || c(1,1) != 7.5)
        { 
            err("self assignment test [elements]"); 
        }
	}

	{
		matrix<double> a = {2, {3.1, 5.2, 9.3, 7.5}};
		matrix<double> b;
		b = std::move(a);
		if(a.begin() != a.end()){ err("assignment test [src begin == src end]"); }
		if(a.size() != 0 ){ err("assignment test [src size]");}
		if(!a.N()){ err("assignment test [src dim]");}
		if(b.size() != 4 || b.N() != 2){ err("assignment test [size]");}
		if(b(0,0) != 3.1 || b(0,1) != 5.2 || b(1,0) != 9.3 || b(1,1) != 7.5)
        { 
            err("assignment test [elements]");             
        }
	}

    
    {
        matrix<double> A0 = {2, {3.1, 5.2, 9.3, 7.5}};
        matrix<double> A1 = {2, {3.1, 5.2, 9.3, 7.5}};
        matrix<double> A2 = {2, {3.1, 5.2, 9.3, 7.5}};
        matrix<double> B0 = {2, {7.1, 15.2, 11.3, 13.4}};
        matrix<double> B1 = {2, {7.1, 15.2, 11.3, 13.4}};
        matrix<double> B2 = {2, {7.1, 15.2, 11.3, 13.4}};
        matrix<double> C{2, {10.2, 20.4, 20.6, 20.9}}; // WolframAlpha - val számolva

        {
            matrix<double> R{A0 + B0};
            if(!check(R,C)){ err("addition test [const& - const&]"); }
        }

        {
            matrix<double> R{A0 + std::move(B1)};
            if(!check(R,C)){ err("addition test [const& - &&]"); }
	    if(B1.size() != 0){ err("addition test [const& - &&]");}
        }

        {
            matrix<double> R{std::move(A1) + B0};
            if(!check(R,C)){ err("addition test [&& - const&]"); }
            if(A1.size() != 0){ err("addition test [&& - const&]");} 
        }

        {
            matrix<double> R{std::move(A2) + std::move(B2)};
            if(!check(R,C)){ err("addition test [&& - &&]"); }
            if(A2.size() != 0){ err("addition test [&& - &&]");}
        }
        
    }

    {
        matrix<double> A0 = {2, {3.1, 5.2, 13.3, 7.5}};
        matrix<double> A1 = {2, {3.1, 5.2, 13.3, 7.5}};
        matrix<double> A2 = {2, {3.1, 5.2, 13.3, 7.5}};
        matrix<double> B0 = {2,{7.1, 15.2, 11.3, 13.4}};
        matrix<double> B1 = {2,{7.1, 15.2, 11.3, 13.4}};
        matrix<double> B2 = {2,{7.1, 15.2, 11.3, 13.4}};
        matrix<double> C{2,{-4, -10, 2, -5.9}}; // WolframAlpha - val számolva

        {
            matrix<double> R{A0 - B0}; 
            if(!check(R,C)){ err("subtraction test [const& - const&]"); }
        }

        {
            matrix<double> R{A0 - std::move(B1)};
            if(!check(R,C)){ err("subtraction test [const& - &&]"); }
            if(B1.size() != 0){ err("subtraction test [const& - &&]");}
        }

        {
            matrix<double> R{std::move(A1) - B0};
            if(!check(R,C)){ err("subtraction test [&& - const&]"); }
            if(A1.size() != 0){ err("subtraction test [&& - const&]");} 
        }

        {
            matrix<double> R{std::move(A2) - std::move(B2)};
            if(!check(R,C)){ err("subtraction test [&& - &&]"); }
            if(A2.size() != 0){ err("subtraction test [&& - &&]");}
           }
        
    }

    {
        matrix<double> A0 = {2,{3.1, 5.2, 13.3, 7.5}};
        matrix<double> A1 = {2,{3.1, 5.2, 13.3, 7.5}};
        matrix<double> A2 = {2,{3.1, 5.2, 13.3, 7.5}};
        matrix<double> B0 = {2,{7.1, 15.2, 11.3, 13.4}};
        matrix<double> B1 = {2,{7.1, 15.2, 11.3, 13.4}};
        matrix<double> B2 = {2,{7.1, 15.2, 11.3, 13.4}};
        matrix<double> C = {2,{80.77, 116.8, 179.18, 302.66}}; // WolframAlpha - val számolva

        {
            matrix<double> R{A0 * B0}; 
            if(!check(R,C)){ err("multiplication by matrix test [const& - const&]"); }
        }

        {
            matrix<double> R{A0 * std::move(B0)};
            if(!check(R,C)){ err("multiplication by matrix test [const& - &&]"); }
            if(B0.size() != 0){ err("multiplication by matrix test [const& - &&]");}
        }

        {
            matrix<double> R{std::move(A1) * B1};
            if(!check(R,C)){ err("multiplication by matrix test [&& - const&]"); }
            if(A1.size() != 0){ err("multiplication by matrix test [&& - const&]");}
        }

        {
            matrix<double> R = std::move(A2) * std::move(B2);
            if(!check(R,C)){ err("multiplication by matrix test [&& - &&]"); }
            if(A2.size() != 0){ err("multiplication by matrix test [&& - &&]");}
        }
        
    }

    {
        matrix<double> A0 = {2,{3.1, 5.2, 13.3, 7.5}};
        matrix<double> A1 = {2,{3.1, 5.2, 13.3, 7.5}};
        double a{1.7};
        matrix<double> C{2,{5.27, 8.84, 22.61, 12.75}}; // WolframAlpha - val számolva

        {
            matrix<double> R{A0 * a}; 
            if(!check(R,C)){ err("multiplication by scalar test"); }
        }

        {
            matrix<double> R{a * A0};
            if(!check(R,C)){ err("multiplication by scalar test"); }
        }


        {
            matrix<double> R{std::move(A0) * a};
            if(!check(R,C)){ err("multiplication by scalar test"); }
            if(A0.size() != 0){ err("multiplication by scalar test");}
        }

        {
            matrix<double> R{a * std::move(A1)};
            if(!check(R,C)){ err("multiplication by scalar test"); }
            if(A1.size() != 0){ err("multiplication by scalar test");}
        }
        
    }

    {
        matrix<double> A = {2, {3.8, 5.5, 13.4, 7.5}};
        double a{2};
        matrix<double> C{2,{1.9, 2.75, 6.7, 3.75}}; // WolframAlpha - val számolva
       
        {
            matrix<double> R{A / a};
            if(!check(R,C)){ err("division  by scalar test"); }
        }

        {
            matrix<double> R{std::move(A) / a};
            if(!check(R,C)){ err("division  by scalar test"); }
            if(A.size() != 0){ err("division by scalar test");}
        }
        
    }

    {
        matrix<double> A{2,{3.1, 5.2, 9.3, 7.5}};
        matrix<double> B{2,{7.1, 15.2, 11.3, 13.4}};
        matrix<double> C0{2,{10.2, 20.4, 20.6, 20.9}}; // WolframAlpha - val számolva
        matrix<double> C1{2,{-4, -10, -2, -5.9}};
        matrix<double> C2{2,{1.55, 2.6, 4.65, 3.75}};
        matrix<double> C3{2,{6.2, 10.4, 18.6, 15}};
        matrix<double> C4{2,{80.77, 116.8, 179.18, 302.66}};
        double a{2};
        {
            matrix<double> A0{A};
            A0 += B;
            if(!check(A0, C0)){ err("operator+= test  test"); }
        }

        {
            matrix<double> A1{A};
            A1 -= B;
            if(!check(A1, C1)){ err("operator-= test  test"); }
        }

        {
            matrix<double> A2{A};
            A2 /= a;
            if(!check(A2,C2)){ err("operator/= test test"); }
        }

        {
            matrix<double> A3{A};
            A3 *= a;
            if(!check(A3,C3)){ err("operator*= test [scalar]"); }
        }
        
        {
            matrix<double> A4{A};
            A4 *= B;
            if(!check(A4,C4)){ err("operator*= test [matrix]"); }
        }
        
    }

    {
        matrix<double> A{2,{3.1, 5.2, 9.3, 7.5}};
        std::string A_str = "3.1 5.2\n9.3 7.5\n";
        std::stringstream ss;
        ss << A;
        if(!(ss.str() == A_str) ){ err("operator << test "); }
    }

    {
        matrix<double> A{2,{3.1, 5.2, 9.3, 7.5}};
        std::string A_str = "3.1 5.2\n9.3 7.5\n";
        std::stringstream ss(A_str);
        matrix<double> B;
        ss >> B;
        if(!(check(A, B))){ err("operator >> test "); }
    }
    return 0;
}
