#include <complex>
#include <vector>

struct GridPoint
{
    double x, y;
    std::complex<double> wave_function;
};

class BaseSpatialGrid
{
    public:
        BaseSpatialGrid(int num_grid_1, int num_grid_2);
        GridPoint at(int index_1, int index_2);

    private:
        std::vector<std::vector<GridPoint>> spatial_data;
        double infinitesimal_distance_1, infinitesimal_distance_2;
        int num_grid_1, num_grid_2;
};

class BaseDomain
{
public:
    BaseDomain(int num_grid_1, int num_grid_2, double t_start, double t_end, int num_times);
    double get_t_start();
    double get_t_end();
    double get_dt();
    int get_num_times();
    
private:
    std::vector<BaseSpatialGrid> domain_data;
    std::vector<double> times;
    double t_start, t_end, dt;
    int num_times;
};