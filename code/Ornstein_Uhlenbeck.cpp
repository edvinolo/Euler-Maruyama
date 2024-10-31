#include "Ornstein_Uhlenbeck.hpp"

Ornstein_Uhlenbeck::Ornstein_Uhlenbeck(const parameters &param_in)
{
    param = param_in;
}

Ornstein_Uhlenbeck::~Ornstein_Uhlenbeck()
{
}

const double Ornstein_Uhlenbeck::RHS(const double x, const double dW, const double dt)
{
    double dx;

    dx = param.theta*(param.mu-x)*dt + param.sigma*dW;

    return dx;
}