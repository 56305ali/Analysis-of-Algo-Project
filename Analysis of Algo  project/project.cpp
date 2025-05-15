#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

// Define the problem dimension
const int DIM = 30;   // Problem dimension
const int POP_SIZE = 50;  // Population size
const int MAX_ITER = 1000;  // Maximum number of iterations
const double LIMIT = 100;  // Limit of unsuccessful attempts for a bee

// Function to calculate the objective value (e.g., Sphere function)
double objectiveFunction(const vector<double>& solution) {
    double sum = 0.0;
    for (int i = 0; i < DIM; i++) {
        sum += solution[i] * solution[i];
    }
    return sum;  // Sphere function: f(x) = sum(x^2)
}

// Random number generation function
double randomInRange(double minVal, double maxVal) {
    return minVal + (maxVal - minVal) * (rand() / double(RAND_MAX));
}

// Bee class to represent a solution in the population
class Bee {
public:
    vector<double> position;  // Solution vector
    double fitness;  // Fitness value
    int trials;  // Number of unsuccessful attempts
    
    Bee() {
        position.resize(DIM);
        trials = 0;
    }
    
    void initialize(double minVal, double maxVal) {
        for (int i = 0; i < DIM; i++) {
            position[i] = randomInRange(minVal, maxVal);
        }
        fitness = objectiveFunction(position);
    }
};

// Artificial Bee Colony Algorithm class
class ABC {
public:
    vector<Bee> population;  // Population of employed and onlooker bees
    vector<Bee> trialSolutions;  // Trial solutions for the scout bees
    
    double minVal, maxVal;  // Range of the search space
    
    ABC(double minValue, double maxValue) {
        minVal = minValue;
        maxVal = maxValue;
    }
    
    // Initialize the population
    void initialize() {
        population.resize(POP_SIZE);
        trialSolutions.resize(POP_SIZE);
        
        // Initialize the employed bees
        for (int i = 0; i < POP_SIZE; i++) {
            population[i].initialize(minVal, maxVal);
        }
    }
    
    // Update the position of a bee using the neighborhood search
    void updatePosition(Bee& bee, Bee& bestBee) {
        int randIndex = rand() % DIM;
        double phi = randomInRange(-1.0, 1.0);
        
        // Generate a new solution
        vector<double> newPosition = bee.position;
        newPosition[randIndex] = bee.position[randIndex] + phi * (bee.position[randIndex] - bestBee.position[randIndex]);
        
        // Ensure the new solution is within bounds
        newPosition[randIndex] = std::min(std::max(newPosition[randIndex], minVal), maxVal);
        
        // Calculate the fitness of the new solution
        double newFitness = objectiveFunction(newPosition);
        
        // If the new solution is better, update the bee's position and fitness
        if (newFitness < bee.fitness) {
            bee.position = newPosition;
            bee.fitness = newFitness;
            bee.trials = 0;
        } else {
            bee.trials++;
        }
    }
    
    // Perform the artificial bee colony algorithm
    void run() {
        int iteration = 0;
        Bee bestBee;
        bestBee.fitness = INFINITY;
        
        // Main loop of the algorithm
        while (iteration < MAX_ITER) {
            // Employed Bees Phase
            for (int i = 0; i < POP_SIZE; i++) {
                updatePosition(population[i], bestBee);
                if (population[i].fitness < bestBee.fitness) {
                    bestBee = population[i];
                }
            }
            
            // Onlooker Bees Phase
            for (int i = 0; i < POP_SIZE; i++) {
                int randIndex = rand() % POP_SIZE;
                updatePosition(population[i], bestBee);
                if (population[i].fitness < bestBee.fitness) {
                    bestBee = population[i];
                }
            }
            
            // Scout Bees Phase
            for (int i = 0; i < POP_SIZE; i++) {
                if (population[i].trials > LIMIT) {
                    population[i].initialize(minVal, maxVal);  // Reinitialize the scout bee
                    population[i].trials = 0;
                }
            }
            
            // Print the best solution so far
            cout << "Iteration " << iteration << " Best Fitness: " << bestBee.fitness << endl;
            
            iteration++;
        }
        
        cout << "Final Best Fitness: " << bestBee.fitness << endl;
    }
};

int main() {
    srand(time(0));
    
    double minVal = -5.12;
    double maxVal = 5.12;
    
    ABC abc(minVal, maxVal);
    abc.initialize();
    abc.run();
    
    return 0;
}