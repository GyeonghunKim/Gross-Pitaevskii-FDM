#pragma once
#include <complex>
#include <vector>
#include <functional>
#include <iostream>
#include <cmath>

#include "src/domain/rectangular_domain.h"
#include "src/initial_condition/initial_condition.h"
#include "src/serial_solver/base_serial_solver.h"
#include "src/serial_solver/forward_euler/forward_euler_rectangular_solver.h"
class CrankNicolsonRectangularSolver : BaseSolver
{
public:
    CrankNicolsonRectangularSolver() = default;
    CrankNicolsonRectangularSolver(
        InitialCondition initialCondition, 
        std::function<double(double, double)> potential, 
        double g, 
        RectangularDomain *rectangularDomain
    );
    void applyInitialCondition();
    void generateRectangularDomain();
    void solve_single_time(int k, double tolerance, int max_iter);
    void solve(double tolerance, int max_iter);

protected:
    InitialCondition initialCondition;
    RectangularDomain * domain;
    std::function<double(double, double)> potential_func;
    RectangularSpatialGrid *guess;
    ForwardEulerRectangularSolver *forward_euler_solver;
    double g;
    std::complex<double> temporal_equation(int i, int j, int k);
    std::complex<double> temporal_equation_from_guess(int i, int j);
    void initialize_guess_with_forward_euler(int k);
    void update_guess(int i, int j, int k);
    double calculate_error(int k);
};