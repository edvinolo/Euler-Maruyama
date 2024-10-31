#ifndef ORNSTEIN_UHLENBECK_HPP
#define ORNSTEIN_UHLENBECK_HPP

struct parameters
{
    //Parameters that define an Ornstein-Uhlenbeck process
    double theta;
    double mu;
    double sigma;
};

class Ornstein_Uhlenbeck
{
private:
    parameters param;
public:
    Ornstein_Uhlenbeck(const parameters &);
    ~Ornstein_Uhlenbeck();

    const double RHS(const double, const double, const double);
};

#endif //ORNSTEIN_UHLENBECK_HPP