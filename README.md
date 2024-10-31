This repository contains a C++ implementation of the Euler-Maruyama method for solving stochastic differential equations (SDEs).

The solver can handle SDEs of the form

$$
\mathrm{d} X_t =  a(X_t,t)\mathrm{d}t + b(X_t,t)\mathrm{d} W_t
$$

where $W_t$ is a Wiener process.

In addition to the parameters for the time grid, the solver needs a pointer to a model object that defines the SDE and evaluates the RHS. The solver generates the Wiener process, which does not need to be implemented by the model itself.
The model RHS method should have the following signature:
```C++
double RHS(double X_t, double dW, double dt) 
```
where ```X_t``` is the current value of the random variable, ```dW``` is the increment of the Wiener process, and ```dt``` is the time step.

Currently implemented models:
- Ornstein-Uhlenbeck process
