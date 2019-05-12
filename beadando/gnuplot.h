#include <iostream>
#include <string>

class gnuplot
{
    public:
        gnuplot();
        ~gnuplot();

        void operator ()(std::string const& command);
    protected:
            FILE *gnuplotpipe;
};

gnuplot::gnuplot()
{
    gnuplotpipe = popen("gnuplot -persist", "w");
    if (!gnuplotpipe)
    {
        std::cerr << ("Gnuplot not found.");
    }
}

gnuplot::~gnuplot()
{
    fprintf(gnuplotpipe, "exit\n");
}

void gnuplot::operator()(std::string const& command)
{
    fprintf(gnuplotpipe, "%s\n", command.c_str());
}

