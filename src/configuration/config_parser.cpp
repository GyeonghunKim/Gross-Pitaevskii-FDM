#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include "parameters.h"
#include "config_parser.h"

/**
 * @brief Parse configuration for the calculation
 * 
 * @param config_name configuration name
 * @param filename configuration file name
 * @return Parsed parameters 
 */
Parameters ConfigParser::parse(std::string config_name, std::string filename){
    auto parameters = Parameters();
    parameters.config_name = config_name;
    // -1 for outside, 0 for main,  1 for domain, 2 for initial condition, 3 for eqution, 4 for solver
    int mode = -1; 
    MainParameters main_parameters;
    DomainParameters domain_parameters;
    InitialConditionParameters init_parameters;
    EquationParameters equation_parameters;
    SolverParameters solver_parameters;
    bool filled_parallel = false;

    std::ifstream file(filename.c_str());
    if (file.is_open()) // If file is opened
    {
        std::string line;
        while (std::getline(file, line)) // while not EOF 
        {
            // remove all blanks in the line
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end()); 
            // replace all lines with lower case
            std::transform(line.begin(), line.end(), line.begin(),
                           [](unsigned char c)
                           { return std::tolower(c); });

            // skip the line when whole line is blank
            if (line == ""){
                continue;
            }
            // skip the line for the comment
            else if (line.rfind("#", 0) != std::string::npos)
            {
                continue;
            }
            else if (line.rfind("[mainconfiguration]", 0) != std::string::npos)
            {
                mode = 0;
            }
            else if (line.rfind("[domainconfiguration]", 0) != std::string::npos)
            {
                mode = 1;
            }
            else if (line.rfind("[initialconditionconfiguration]", 0) != std::string::npos)
            {
                mode = 2;
            }
            else if (line.rfind("[equationconfiguration]", 0) != std::string::npos)
            {
                mode = 3;
            }
            else if (line.rfind("[solverconfiguration]", 0) != std::string::npos)
            {
                mode = 4;
            }
            else{
                if (mode == -1){
                    std::cerr << "Unexpected Configuration File" << std::endl;
                }
                else if(mode == 0){
                    std::vector<std::string> dump;
                    std::string tmp;
                    std::stringstream string_stream(line);
                    if (line.rfind("type", 0) != std::string::npos)
                    {
                        while (std::getline(string_stream, tmp, '='))
                        {
                            dump.push_back(tmp);
                        }
                        if (dump.size() != 2)
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                        main_parameters.calculation_type = dump[1];
                    }
                    else{
                        while (std::getline(string_stream, tmp, '='))
                        {
                            dump.push_back(tmp);
                        }
                        if (dump.size() != 2)
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                        if (dump[0] == "sweep_start")
                        {
                            main_parameters.float_parameters[dump[0]] = (float)std::atof(dump[1].c_str());
                        }
                        else if (dump[0] == "sweep_end")
                        {
                            main_parameters.float_parameters[dump[0]] = (float)std::atof(dump[1].c_str());
                        }
                        else if (dump[0] == "sweep_count")
                        {
                            main_parameters.int_parameters[dump[0]] = std::atoi(dump[1].c_str());
                        }
                        else if (dump[0] == "end_point")
                        {
                            if (dump[1] == "true")
                            {
                                main_parameters.int_parameters[dump[0]] = (int)true;
                            }
                            else if (dump[1] == "false")
                            {
                                main_parameters.int_parameters[dump[0]] = (int)false;
                            }
                            else 
                            {
                                std::cerr << "Unexpected Configuration File" << std::endl;
                            }
                        }
                        else if (dump[0] == "mpi_use")
                        {
                            if (dump[1] == "true")
                            {
                                main_parameters.int_parameters[dump[0]] = (int)true;
                            }
                            else if (dump[1] == "false")
                            {
                                main_parameters.int_parameters[dump[0]] = (int)false;
                            }
                            else
                            {
                                std::cerr << "Unexpected Configuration File" << std::endl;
                            }
                        }
                        else if (dump[0] == "cuda_use")
                        {
                            if (dump[1] == "true")
                            {
                                main_parameters.int_parameters[dump[0]] = (int)true;
                            }
                            else if (dump[1] == "false")
                            {
                                main_parameters.int_parameters[dump[0]] = (int)false;
                            }
                            else
                            {
                                std::cerr << "Unexpected Configuration File" << std::endl;
                            }
                        }
                        else if (dump[0] == "gpu_count")
                        {
                            main_parameters.int_parameters[dump[0]] = std::atoi(dump[1].c_str());
                        }
                        else if (dump[0] == "calculation_per_gpu")
                        {
                            main_parameters.int_parameters[dump[0]] = std::atoi(dump[1].c_str());
                        }
                        else
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                    }
                }
                else if(mode == 1){
                    std::vector<std::string> dump;
                    std::string tmp;
                    std::stringstream string_stream(line);
                    if (line.rfind("type", 0) != std::string::npos)
                    {
                        while (std::getline(string_stream, tmp, '='))
                        {
                            dump.push_back(tmp);
                        }
                        if (dump.size() != 2){
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                        domain_parameters.domain_type = dump[1];
                    }
                    else{
                        while (std::getline(string_stream, tmp, '='))
                        {
                            dump.push_back(tmp);
                        }
                        if (dump.size() != 2)
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }

                        if (domain_parameters.domain_type != ""){
                            while (std::getline(string_stream, tmp, '='))
                            {
                                dump.push_back(tmp);
                            }
                            if (dump.size() != 2)
                            {
                                std::cerr << "Unexpected Configuration File" << std::endl;
                            }
                            
                            if (dump[0] == "n_x"){
                                domain_parameters.n_x = std::atoi(dump[1].c_str());
                            }
                            else if(dump[0] == "n_y"){
                                domain_parameters.n_y = std::atoi(dump[1].c_str());
                            }
                            else if(dump[0] == "n_time"){
                                domain_parameters.n_time = std::atoi(dump[1].c_str());
                            }
                            else if(dump[0] == "time_start"){
                                domain_parameters.time_start = (float)std::atof(dump[1].c_str());
                            }
                            else if(dump[0] == "time_end"){
                                domain_parameters.time_end = (float)std::atof(dump[1].c_str());
                            }
                            else{
                                domain_parameters.spatial_parameters[dump[0]] = (float)std::atof(dump[1].c_str());
                            }
                        }
                        else{
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                    }
                }
                else if(mode == 2){
                    std::vector<std::string> dump;
                    std::string tmp;
                    std::stringstream string_stream(line);
                    if (line.rfind("type", 0) != std::string::npos)
                    {
                        while (std::getline(string_stream, tmp, '='))
                        {
                            dump.push_back(tmp);
                        }
                        if (dump.size() != 2)
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                        init_parameters.init_cond_type = dump[1];
                    }
                    else
                    {
                        while (std::getline(string_stream, tmp, '='))
                        {
                            dump.push_back(tmp);
                        }
                        if (dump.size() != 2)
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }

                        if (init_parameters.init_cond_type != "")
                        {
                            init_parameters.init_cond_parameters[dump[0]] = (float)std::atof(dump[1].c_str());
                        }
                        else
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                    }
                }
                else if(mode == 3){
                    std::vector<std::string> dump;
                    std::string tmp;
                    std::stringstream string_stream(line);
                    if (line.rfind("type", 0) != std::string::npos)
                    {
                        while (std::getline(string_stream, tmp, '='))
                        {
                            dump.push_back(tmp);
                        }
                        if (dump.size() != 2)
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                        equation_parameters.potential_type = dump[1];
                    }
                    else if (line.rfind("g", 0) != std::string::npos)
                    {
                        while (std::getline(string_stream, tmp, '='))
                        {
                            dump.push_back(tmp);
                        }
                        if (dump.size() != 2)
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                        equation_parameters.g = std::atof(dump[1].c_str());
                    }
                    else
                    {
                        while (std::getline(string_stream, tmp, '='))
                        {
                            dump.push_back(tmp);
                        }
                        if (dump.size() != 2)
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }

                        if (equation_parameters.potential_type != "")
                        {
                            equation_parameters.potential_parameters[dump[0]] = std::atof(dump[1].c_str());
                        }
                        else
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                    }
                }
                else if (mode == 4){
                    std::vector<std::string> dump;
                    std::string tmp;
                    std::stringstream string_stream(line);

                    // Get Method
                    if (line.rfind("method", 0) != std::string::npos)
                    {
                        while (std::getline(string_stream, tmp, '='))
                        {
                            dump.push_back(tmp);
                        }
                        if (dump.size() != 2)
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                        solver_parameters.method = dump[1];
                    }
                    
                    // Get Parallel
                    else if (line.rfind("parallel", 0) != std::string::npos)
                    {
                        while (std::getline(string_stream, tmp, '='))
                        {
                            dump.push_back(tmp);
                        }
                        if (dump.size() != 2)
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                        
                        if (dump[1].rfind("true", 0) != std::string::npos)
                        {
                            solver_parameters.run_parallel = true;
                            filled_parallel = true;
                        }
                        else if (dump[1].rfind("false", 0) != std::string::npos)
                        {
                            solver_parameters.run_parallel = false;
                            filled_parallel = true;
                        }
                        else{
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                    }

                    // Get Print Info
                    else if (line.rfind("print_info", 0) != std::string::npos)
                    {
                        while (std::getline(string_stream, tmp, '='))
                        {
                            dump.push_back(tmp);
                        }
                        if (dump.size() != 2)
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                        if (dump[1].rfind("true", 0) != std::string::npos)
                        {
                            solver_parameters.print_info = true;
                        }
                        else if (dump[1].rfind("false", 0) != std::string::npos)
                        {
                            solver_parameters.print_info = false;
                        }
                        else
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                    }

                    // Get Save Data
                    else if (line.rfind("save_data", 0) != std::string::npos)
                    {
                        while (std::getline(string_stream, tmp, '='))
                        {
                            dump.push_back(tmp);
                        }
                        if (dump.size() != 2)
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                        if (dump[1].rfind("true", 0) != std::string::npos)
                        {
                            solver_parameters.save_data = true;
                        }
                        else if (dump[1].rfind("false", 0) != std::string::npos)
                        {
                            solver_parameters.save_data = false;
                        }
                        else
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                    }
                    
                    // Get Cuda Device
                    else if (line.rfind("cuda_device", 0) != std::string::npos)
                    {
                        while (std::getline(string_stream, tmp, '='))
                        {
                            dump.push_back(tmp);
                        }
                        if (dump.size() != 2)
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                        solver_parameters.int_parameters[dump[0]] = std::atoi(dump[1].c_str());
                    }

                    // Get max iter
                    else if (line.rfind("max_iter", 0) != std::string::npos)
                    {
                        while (std::getline(string_stream, tmp, '='))
                        {
                            dump.push_back(tmp);
                        }
                        if (dump.size() != 2)
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                        solver_parameters.int_parameters[dump[0]] = std::atoi(dump[1].c_str());
                    }

                    else
                    {
                        while (std::getline(string_stream, tmp, '='))
                        {
                            dump.push_back(tmp);
                        }
                        if (dump.size() != 2)
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                        if ( (solver_parameters.method != "") && filled_parallel)
                        {
                            solver_parameters.solver_parameters[dump[0]] = std::atof(dump[1].c_str());
                        }
                        else
                        {
                            std::cerr << "Unexpected Configuration File" << std::endl;
                        }
                    }

                }
                else{
                    std::cerr << "Unexpected Configuration File" << std::endl;
                }
            }
        }
    }
    parameters.main_parameters = main_parameters;
    parameters.domain_parameters = domain_parameters;
    parameters.init_cond_parameters = init_parameters;
    parameters.equation_parameters = equation_parameters;
    parameters.solver_parameters = solver_parameters;
    return parameters;
}

Parameters ConfigParser::get_default()
{
    auto parameters = Parameters();
    parameters.config_name = "default";
    MainParameters main_parameters;
    DomainParameters domain_parameters;
    InitialConditionParameters init_parameters;
    EquationParameters equation_parameters;
    SolverParameters solver_parameters;

    main_parameters.calculation_type = "single";
    domain_parameters.domain_type = "rectangular";
    domain_parameters.n_x = 256;
    domain_parameters.n_y = 256;
    domain_parameters.n_time = 5;
    domain_parameters.time_start = 0.;
    domain_parameters.time_end = 0.01;
    domain_parameters.spatial_parameters["x_start"] = -10.;
    domain_parameters.spatial_parameters["x_end"] = 10.;
    domain_parameters.spatial_parameters["y_start"] = -10.;
    domain_parameters.spatial_parameters["y_end"] = 10.;

    init_parameters.init_cond_type = "singlegaussian";
    init_parameters.init_cond_parameters["sigma_x"] = 1.;
    init_parameters.init_cond_parameters["sigma_y"] = 1.;
    init_parameters.init_cond_parameters["center_x"] = 0.;
    init_parameters.init_cond_parameters["center_y"] = 0.;

    equation_parameters.potential_type = "harmonic";
    equation_parameters.g = -1;
    equation_parameters.potential_parameters["omega_x"] = 3.;
    equation_parameters.potential_parameters["omega_y"] = 5.;

    solver_parameters.method = "cranknicolson";
    solver_parameters.run_parallel = true;
    solver_parameters.print_info = false;
    solver_parameters.save_data = false;
    solver_parameters.solver_parameters["converge_crit"] = 1e-11;
    solver_parameters.int_parameters["max_iter"] = 1001;
    solver_parameters.int_parameters["cuda_device"] = 0;

    parameters.main_parameters = main_parameters;
    parameters.domain_parameters = domain_parameters;
    parameters.init_cond_parameters = init_parameters;
    parameters.equation_parameters = equation_parameters;
    parameters.solver_parameters = solver_parameters;
    return parameters;
}