#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <float.h>
#include <math.h>
#include <time.h>

#define wMin 0 						// minimum value for random weights initialization
#define wMax 675 					// maximum value for random weights initialization

#define top_x 5 					// map topology x
#define top_y 5 					// map topology y
#define epochs 100 					// number of learning cycles
#define gamma 0.02 					// learning parameter
#define radius 3.0 					// radius for neighborhood function

#define samples 60 					// number of training samples
#define inputs 9518 				// number of inputs
int train[samples][inputs] = {0};	// training set (needs to be loaded)

double w[top_x][top_y][inputs];		// weights
#define w(i,j,in) w[i][j][in]

// random weights initialization
void wInit() {
	time_t t;
	srand((unsigned) time(&t));
	for(int i=0; i<top_x; i++) for(int j=0; j<top_y; j++) for(int in=0; in<inputs; in++) w(i,j,in) = rand() / (RAND_MAX/(wMax-wMin)) + wMin;
}

// finds a winnder and adjusts weights
void changeWeights(int input[],double g ,double r) {
    // --- find winner
	int winner[2] = {0};
	double min = DBL_MAX;
	for(int i=0; i<top_x; i++) for(int j=0; j<top_y; j++) {
		double d = 0;
		for(int in=0; in<inputs; in++) {
			d = d + pow(w(i,j,in)-input[in],2);
		}
		if (d < min) {
        	min = d;
        	winner[0] = i;
        	winner[1] = j;
   		}
	}
	printf("%4d   %4d   %f\n", winner[0], winner[1], min);
	// --- change weights
	double distance;
    double neighborF;
    for(int i=0; i<top_x; i++) for(int j=0; j<top_y; j++) {
    	distance = pow(winner[0] - i, 2) + pow(winner[1] - j, 2);
    	neighborF = 1 * exp ( - distance / r );
    	for(int in=0; in<inputs; in++) {
			w(i,j,in) =  w(i,j,in) + g * neighborF * (input[in] - w(i,j,in));
		}
	}
}

// main
void main() {
	double g,r;
	wInit();
	printf("Weights Initialised.\n\n");
	printf("Sample      X      Y   Distance\n");

    // --- Training
	for (int e=0; e<epochs; e++) {
        g = gamma - (gamma/epochs) * e;
		r = radius - (radius/epochs) * e;
		printf("------------------------------------------- Epoch : %d (Gamma : %f, Radius : %f)\n", e+1, g, r);
		for (int t=0; t<samples; t++) {
            changeWeights(train[t], g, r);
		}
	}

    printf("\nFinished.");
}
