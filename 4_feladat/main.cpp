#include "vector2d.h"

int main(int, char**) 
{
    // a referencia értékeket wolframalpha segítségével számoltam ki
    std::cout.precision(16);
    double tol = 1e-8;

    vector2d<int> a = { -5, 12};
    vector2d<double> b = {7.0, 11.2};

    double ref1 = 99.4;
    double r = dot(a, b);
    double delta1 = std::abs(r - ref1);
    if (delta1 > tol)
    {
        exit(-1);
    }
    std::cout << "Skalár szorzat: referenciaértéktől való eltérés: " << delta1 << std::endl;

    a = { 91, 2};

    double ref2 = 8285;
    double sql = sqlength(a);
    double delta2 = std::abs(sql - ref2);
    if (delta2 > tol)
    {
        exit(-1);
    }
    std::cout << "Hossz négyzete: referenciaértéktől való eltérés: " << delta2 << std::endl;


    a = { 91, 2};

    double ref3 = 91.021975368588876;
    double l = length(a);
    double delta3 = std::abs(l - ref3);
    if (delta3 > tol)
    {
        exit(-1);
    }
    std::cout << "Hossz: referenciaértéktől való eltérés: " << delta3 << std::endl;

    a = {11, 7};

    vector2d<double> ref4 = {0.8436614877321074, 0.5368754921931593};
    vector2d<double> n = normalize(a);
    vector2d<double> delta4 = abs(n - ref4);
    if (delta4.x > tol or delta4.y > tol )
    {
        exit(-1);
    }
    std::cout << "Normálás: referenciaértéktől való eltérés: " << delta4 << std::endl;

        b = {1.23, 7.1};
        double c = 3.33;

    vector2d<double> ref5 = {4.0959, 23.643};
    vector2d<double> delta5 = abs(b * c - ref5);
    vector2d<double> delta6 = abs(c * b - ref5);

    if ((delta4.x > tol or delta4.y > tol ) or (delta4.x > tol or delta4.y > tol))
    {
        exit(-1);
    }
    std::cout << "Skalárral való szorzás: referenciaértéktől való eltérés: "
              << delta5 << " és "<< delta6 << std::endl;

    
    vector2d<double> ref7 = { 0.3693693693693693, 2.1321321321321321};
    vector2d<double> delta7 = abs(b / c - ref7);

    if (delta7.x > tol or delta7.y > tol )
    {
        exit(-1);
    }
    std::cout << "Skalárral való osztás: referenciaértéktől való eltérés: "<< delta7 << std::endl;

    a = {22, 17};
    b = {31.0, 5.0};

    vector2d<double> ref8 = { 53.0, 22.0};
    vector2d<double> delta8 = abs(a + b - ref8);
    a += b;
    vector2d<double> delta9 = abs(a - ref8);
    
    if ((delta8.x > tol or delta8.y > tol ) or (delta9.x > tol or delta9.y > tol))
    {
        exit(-1);
    }
    std::cout << "Összeadás: referenciaértéktől való eltérés: "<< delta8 
              << " és " << delta9 << std::endl;

    a = {11, 6};
    b = {4.5, 17};

    vector2d<double> ref10 = { -6.5, 11};
    vector2d<double> delta10 = abs(b - a - ref10);
    b -= a;
    vector2d<double> delta11 = abs(b - ref10);
    
    if ((delta10.x > tol or delta10.y > tol ) or (delta11.x > tol or delta11.y > tol))
    {
        exit(-1);
    }
    std::cout << "Kivonás: referenciaértéktől való eltérés: "<< delta10 
              << " és " << delta11 << std::endl;
} 