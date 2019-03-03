#include <iostream>
#include <cmath>

//Newton-iterátort
/*'f' a függvény amelyiknek keresük a gyökét, 'der' f deriváltja,
'test' a megállási feltétel*/
template <typename T, typename f1, typename f2, typename f3>
T newton(f1 f, f2 der, f3 test, T x0)
{
    if(x0 == 0)
    {
        std::cout << "Nem megfelelő kezdőérték";
        exit(-1);
    }

    T x1;
    int n = 0;
    do
    {
        x1 = x0;
        x0 = x0 - f(x0) / der(x0);

        n++;
        if(n == 20) break; // maximum 20 lépés, ha az adott feltétel addig nem teljesül
    }while(test(x1,x0));

    return x0;
}

auto deriv = [](double x){ return 2.0 * x; };
auto test = [](double x1, double x0 ){ return std::abs(x1 - x0) > 1e-6; };

int main(int, char**)
{
    std::cout.precision(16);
    // az ellenőrzéshez használt érték 612^0.5 = 24.738633753705963298928
    double a = 24.738633753705963298928;

    double r = newton([](double x){ return x*x - 612.0; }, deriv, test, 1.); //abszolút eltérés figyelése
    //müködés ellenőrzése
    std::cout << "612 négyzetgyöke: ";
    std::cout << r << std::endl;

    std::cout << "Az egzakt értéktől való eltérés: "; // 6 tizedesjegyig vizsgalva, tol = 10^-6
    std::cout << std::abs(r - a);


    // egy szám négyzetgyökének meghatározása
    std::cout << std::endl << "Négyzetgyökvonás" << std::endl;
    std::cout << "Szám: ";
    double num, x0;
    std::cin >> num;
    std::cout << "Kezdőérték: ";
    std::cin >> x0;

    double r2 = newton([num](double x){ return x*x - num; }, deriv, test, x0);

    std::cout << r2;

    return 0;
}
