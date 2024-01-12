#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

using namespace std;
typedef vector<double> Vector;

void plotPreparation(const Vector& beat, const Vector& meet, const Vector& miss) {
    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    int size = beat.size();
    if (gnuplotPipe) {
        // Set title and labels
        fprintf(gnuplotPipe, "set title \"CAAR average for three groups\"\n");
        fprintf(gnuplotPipe, "set xlabel 'Announcement Date'\n");
        fprintf(gnuplotPipe, "set ylabel 'Avg CAAR'\n");
        

        // Set range for X and Y axis
        fprintf(gnuplotPipe, "set yrange [-0.20:0.04]\n");
        fprintf(gnuplotPipe, "set xrange [%d:%d]\n", -size/2, size/2);
        fprintf(gnuplotPipe, "set grid\n");

        // Add a vertical red line at x = 0
        fprintf(gnuplotPipe, "set arrow from 0,graph(0,0) to 0,graph(1,1) nohead lc rgb 'red'\n");

        // Set up the plot command with data
        fprintf(gnuplotPipe, "plot '-' with lines title 'Beat', '-' with lines title 'Meet', '-' with lines title 'Miss'\n");

        // Plot data for Beat, Meet, and Miss groups
        for (int i = -size/2; i < size/2; i++) {
            fprintf(gnuplotPipe, "%d %lf\n", i, beat[i + size/2]);
        }
        fprintf(gnuplotPipe, "e\n");

        for (int i = -size/2; i < size/2; i++) {
            fprintf(gnuplotPipe, "%d %lf\n", i, meet[i + size/2]);
        }
        fprintf(gnuplotPipe, "e\n");

        for (int i = -size/2; i < size/2; i++) {
            fprintf(gnuplotPipe, "%d %lf\n", i, miss[i + size/2]);
        }
        fprintf(gnuplotPipe, "e\n");

        // Flush and close the gnuplot pipe
        fflush(gnuplotPipe);
        fprintf(gnuplotPipe, "exit\n");
        pclose(gnuplotPipe);
    } else {
        std::cerr << "Could not open pipe to gnuplot.\n";
    }
   
}