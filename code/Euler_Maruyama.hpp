#ifndef EULER_MARUYAMA_HPP
#define EULER_MARUYAMA_HPP

#include <utility>
#include <vector>
#include <memory>
#include <stdio.h>
#include <random>
#include <math.h>

// Solve a stochastic DE using the Euler_Maruyama method
// The SDE is assumed to be of the form dX_t = a(X_t,t)dt + b(X_t,t)dW_t
// Here W_t is the Wiener process.
// Use a call back to evaluate the RHS using random numbers generated here

struct time_params
{
    double t_min;
    double t_max;
    unsigned samples;
};

class Euler_Maruyama
{
private:
    const time_params* params;
    double dt;

    std::vector<double> y;
    std::vector<double> W;
    std::vector<double> time;

    double y_0;

    void setup_arrays();
public:
    //Constructors and destructors
    Euler_Maruyama(const time_params*,const double);
    ~Euler_Maruyama();

    template <typename T>
    void run_simulation (T*);

    std::vector<double> get_y();
};

template <typename T>
void Euler_Maruyama::run_simulation(T* model)
{
    double dW;

    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<double> normal(0.0,sqrt(dt));

    for (size_t i = 0; i < y.size()-1; i++)
    {
        time[i+1] = time[i] + dt;
        W[i+1] = normal(gen);
        dW = W[i+1] - W[i];
        y[i+1] = y[i] + model->RHS(y[i],dW,dt);
    }

    printf("%f, %.15f, %.15f \n",time[params->samples-1],y[params->samples-1],y[10]);

}

#endif //EULER_MARUYAMA_HPP