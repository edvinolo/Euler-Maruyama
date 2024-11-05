#include "Euler_Maruyama.hpp"

Euler_Maruyama::Euler_Maruyama(const time_params* time_in,const double y_0_in)
{
    params = time_in;
    y_0 = y_0_in;

    setup_arrays();
}

Euler_Maruyama::~Euler_Maruyama()
{
}

void Euler_Maruyama::setup_arrays(){
    time = std::vector<double> (params->samples,0.0);
    y = std::vector<double> (params->samples,0.0);
    W = std::vector<double> (params->samples,0.0);

    dt = (params->t_max - params->t_min)/(params->samples-1);

    time[0] = params->t_min;
    y[0] = y_0;
}

const std::vector<double>& Euler_Maruyama::get_y()
{
    return this->y;
}

std::vector<double> Euler_Maruyama::get_y_copy()
{
    return y;
}

std::vector<double> Euler_Maruyama::get_time()
{
    return time;
}