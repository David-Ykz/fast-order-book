#include <iostream>
#include <vector>
#include <cmath>
#include <random>

using namespace std;

struct StockParameters {
    double initialPrice = 1.0;
    double drift = 0.0001; // expected return (mu)
    double volatility = 0.01; // volatility (sigma)
    double dt = 1.0; // time step
};

class GBMGenerator {
    mt19937 gen;
    normal_distribution<double> dist;
    StockParameters params;
    double currentPrice;

public:
    GBMGenerator(StockParameters p) : gen(random_device{}()), dist(0.0, 1.0), params(p) {
        currentPrice = params.initialPrice;
    }

    double computeNextPrice() {
        // get random value
        double x = dist(gen);
        double exponent = (params.drift - 0.5 * params.volatility * params.volatility) * params.dt 
                        + (params.volatility * sqrt(params.dt) * x);
        
        return currentPrice * exp(exponent);
    }

    uint32_t generatePrice() {
        currentPrice = computeNextPrice();
        return static_cast<uint32_t>(round(currentPrice * 100));
    }

};