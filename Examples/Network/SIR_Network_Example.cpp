#include <CL/cl.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <cassert>
#include <array>
#include <random>
#include <cmath>
#include <igraph/igraph.h>
#include <igraph/igraph_games.h>
#define SUCCESS 0
#define FAILURE 1
#include <NV_FilePaths.h>
#include <NV_CL_DefaultInstance.hpp>

igraph_t generate_ER_Network(size_t N_nodes, float p)
{
    //Generate igraph ER network
    igraph_t g;
    igraph_erdos_renyi_game(&g, IGRAPH_ERDOS_RENYI_GNP, N_nodes, p, 1, 0);
    return g;
}

std::vector<float> get_flat_AdjMat(igraph_t g)
{
    size_t N_nodes = igraph_vcount(&g);
    // Get adjacency matrix of G
    igraph_matrix_t adjacency_matrix;
    igraph_matrix_init(&adjacency_matrix, N_nodes, N_nodes);
    igraph_get_adjacency(&g, &adjacency_matrix, IGRAPH_GET_ADJACENCY_BOTH, 0);
    //Copy adjacency matrix to 1 dimensional vector
    std::vector<float> adjacency_vector(igraph_matrix_nrow(&adjacency_matrix) * igraph_matrix_ncol(&adjacency_matrix));
    for (int i = 0; i < igraph_matrix_nrow(&adjacency_matrix); i++)
    {
        for (int j = 0; j < igraph_matrix_ncol(&adjacency_matrix); j++)
        {
            adjacency_vector[i * igraph_matrix_ncol(&adjacency_matrix) + j] = MATRIX(adjacency_matrix, i, j);
        }
    }
    return adjacency_vector;
}

//generate initial infection states for network
std::vector<float> generate_initial_infection_states(size_t N_nodes, float p_init_infected)
{
    std::vector<float> initial_infection_states(3 * N_nodes);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    for (int i = 0; i < N_nodes; i++)
    {
        if (dis(gen) < p_init_infected)
        {
            initial_infection_states[3 * i + 1] = 1;
        }
        else
        {
            initial_infection_states[i] = 1;
        }
    }
    return initial_infection_states;
}

void write_SIR_trajectories(std::ofstream &outfile, float *x_traj, size_t Nt, size_t N_SingleRun_Trajectories)
{
    //write all trajectories in x_traj to csv
    for (int i = 0; i < N_SingleRun_Trajectories; i++)
    {
        size_t offset = 3 * (Nt + 1) * i;
        for (int j = 0; j < Nt; j++)
        {
            outfile << x_traj[offset + 3 * j] << ", " << x_traj[offset + 3 * j + 1] << ", " << x_traj[offset + 3 * j + 2] << "\n";
        }
    }
}

//write network node states to file
void write_network_node_states(std::ofstream &outfile, float *x_traj, size_t N_nodes, size_t Nt, size_t N_SingleRun_Trajectories)
{
    //write all trajectories in x_traj to csv
    for (int i = 0; i < N_SingleRun_Trajectories; i++)
    {
        size_t offset = 3 * N_nodes * (Nt + 1) * i;
        for (int j = 0; j < N_nodes; j++)
        {
            for (int t = 0; t < Nt; j++)
            {
                outfile << j << ", " << x_traj[offset + 3 * t] << ", " << x_traj[offset + 3 * t + 1] << ", " << x_traj[offset + 3 * t + 2] << "\n";
            }
        }
    }
}

int main(int argc, char *argv[])
{

    //find and print all available opencl devices using clGetdeviceinfo
    cl_int err;
    cl_uint num_platforms;
    cl_platform_id *platforms;
    cl_uint num_devices;
    cl_device_id *devices;
    err = clGetPlatformIDs(0, NULL, &num_platforms);
    platforms = (cl_platform_id *)malloc(num_platforms * sizeof(cl_platform_id));
    err = clGetPlatformIDs(num_platforms, platforms, NULL);

    for (int i = 0; i < num_platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        char platform_name[100];
        err = clGetPlatformInfo(platform, CL_PLATFORM_NAME, 100, platform_name, NULL);
        std::cout << "Platform " << i << ": " << platform_name << std::endl;
        cl_uint num_devices;
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
        devices = (cl_device_id *)malloc(num_devices * sizeof(cl_device_id));
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, num_devices, devices, NULL);
        for (int j = 0; j < num_devices; j++)
        {
            cl_device_id device = devices[j];
            char device_name[100];
            err = clGetDeviceInfo(device, CL_DEVICE_NAME, 100, device_name, NULL);
            std::cout << "Device " << j << ": " << device_name << std::endl;
        }
    }

    
    std::string DATA_PATH = NV_EXAMPLES_DATA_DIR;
    std::string cl_generator_dir = NV_CL_GENERATOR_DIR;
    std::string pwd = std::string(NV_OPENCL_DIR) + "/Examples/";
    std::string epidemiological_kernel_dir = std::string(NV_CL_KERNEL_DIR) + "/Epidemiological/";

    CL_Instance clInstance = clDefaultInitialize(CL_DEVICE_TYPE_ALL);


    size_t cacheSize = 0;
    clGetDeviceInfo(clInstance.device_ids[0], CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(size_t), &cacheSize, NULL);
    std::cout << "Device cache size:\t" << cacheSize << std::endl;

    // get global memory size
    size_t global_mem_size;
    clGetDeviceInfo(clInstance.device_ids[0], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(size_t), &global_mem_size, NULL);
    std::cout << "Device global memory size:\t" << global_mem_size << std::endl;

    size_t maxWorkGroupSize = 0;
    clGetDeviceInfo(clInstance.device_ids[0], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &maxWorkGroupSize, NULL);
    std::cout << "Device max work group size:\t" << maxWorkGroupSize << std::endl;

    //print max work items
    // size_t maxWorkItems[3];
    // clGetDeviceInfo(clInstance.device_ids[0], CL_DEVICE_MAX_WORK_ITEM_SIZES, 3*sizeof(size_t), (void*) maxWorkItems, NULL);
    //print maxWorkItems 3 items
    // std::cout << "Device max work item dimensions:\t" << maxWorkItems[0] << " " << maxWorkItems[1] << " " << maxWorkItems[2] << std::endl;
    // int err = 0;
    clInstance.program = clLoadProgram((epidemiological_kernel_dir + "SIR_Compute_Network.cl").c_str(), clInstance.context, err);

    assert(err == CL_SUCCESS);




    /* Parameter/Buffer initialization */
    constexpr size_t N_trajectories = 8;
    size_t N_SingleRun_Trajectories = (N_trajectories > maxWorkGroupSize) ? maxWorkGroupSize : N_trajectories;
    constexpr uint Nt = 10;

     size_t N_runs = std::ceil(N_trajectories / N_SingleRun_Trajectories);

    ulong seeds[N_SingleRun_Trajectories];

    float dt = 5.;
    float p_init_infected = 0.1;
    size_t N_nodes = 100;
    igraph_t G = generate_ER_Network(N_nodes, .7);
    std::vector<float> adjacency_vector = get_flat_AdjMat(G);
    std::vector<float> x0 = generate_initial_infection_states(N_nodes, p_init_infected);

    size_t seedBufferSize = N_SingleRun_Trajectories * sizeof(ulong);
    size_t trajectorySize = 3 * (Nt + 1) * sizeof(float) * N_nodes;
    size_t trajectoryBufferSize = trajectorySize * N_SingleRun_Trajectories;

    assert(trajectoryBufferSize < global_mem_size);

    std::vector<float> x_traj(3 * (Nt + 1) * N_SingleRun_Trajectories * N_nodes);

    std::string program_directives = " -DN_NETWORK_NODES=" + std::to_string(N_nodes) + " -DN_TRAJECTORY_ITERATIONS=" + std::to_string(Nt);

    std::string build_options = "-I " + cl_generator_dir + " -I " + epidemiological_kernel_dir + program_directives;
    build_options += " -g";
    /*Step 6: Build program. */
    int status = clBuildProgram(clInstance.program, 1, clInstance.device_ids.data(), build_options.c_str(), NULL, NULL);
    if (status == CL_BUILD_PROGRAM_FAILURE)
    {
        // Determine the size of the log
        size_t log_size;
        clGetProgramBuildInfo(clInstance.program, clInstance.device_ids[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

        // Allocate memory for the log
        char *log = (char *)malloc(log_size);

        // Get the log
        clGetProgramBuildInfo(clInstance.program, clInstance.device_ids[0], CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

        // Print the log
        printf("%s\n", log);
    }

    cl_mem seedBuffer = clCreateBuffer(clInstance.context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                       seedBufferSize, (void *)seeds, &err);

    cl_mem x0Buffer = clCreateBuffer(clInstance.context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                     sizeof(float) * 3 * N_nodes, (void *)&x0, &err);
    cl_mem adjMatrixBuffer = clCreateBuffer(clInstance.context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                            sizeof(float) * N_nodes * N_nodes, (void *)&adjacency_vector[0], &err);
    cl_mem outputBuffer = clCreateBuffer(clInstance.context, CL_MEM_WRITE_ONLY,
                                         trajectoryBufferSize, NULL, &err);
    /*Step 8: Create kernel object */
    cl_kernel kernel = clCreateKernel(clInstance.program, "SIR_Compute_Network", &err);
    assert(err == CL_SUCCESS);

    float infection_p = .4;
    float recovery_p = .01;

    /*Step 9: Sets Kernel arguments.*/
    // status = clSetKernelArg(kernel, 0, sizeof(size_t), &num);
    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&seedBuffer);
    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&x0Buffer);
    status = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&adjMatrixBuffer);
    status = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&outputBuffer);
    status = clSetKernelArg(kernel, 4, sizeof(float), &infection_p);
    status = clSetKernelArg(kernel, 5, sizeof(float), &recovery_p);
    status = clSetKernelArg(kernel, 6, sizeof(uint), (void *)&Nt);
    status = clSetKernelArg(kernel, 7, sizeof(size_t), (void *)&N_nodes);
    assert(status == CL_SUCCESS);

    std::ofstream outfile;
    outfile.open(DATA_PATH + "x_traj.csv");
    std::mt19937_64 rng;
    std::uniform_int_distribution<ulong> dist(0, UINT64_MAX);

    for (int i = 0; i < N_runs; i++)
    {
        for (int i = 0; i < N_SingleRun_Trajectories; i++)
        {
            seeds[i] = dist(rng);
        }

        std::cout << "Run " << i << std::endl;
        size_t global_work_size[1] = {N_SingleRun_Trajectories};
        status = clEnqueueNDRangeKernel(clInstance.commandQueue, kernel, 1, NULL,
                                        global_work_size, NULL, 0, NULL, NULL);

        clFinish(clInstance.commandQueue);

        status = clEnqueueReadBuffer(clInstance.commandQueue, outputBuffer, CL_TRUE, 0,
                                     trajectoryBufferSize, (void *)x_traj.data(), 0, NULL, NULL);

        assert(status == CL_SUCCESS);
        write_network_node_states(outfile, x_traj.data(), N_nodes, Nt, N_SingleRun_Trajectories);
    }

    status = clReleaseKernel(kernel);              //Release kernel.
    status = clReleaseProgram(clInstance.program); //Release the program object.
    status = clReleaseMemObject(outputBuffer);
    status = clReleaseCommandQueue(clInstance.commandQueue); //Release  Command queue.
    status = clReleaseContext(clInstance.context);           //Release context.

    outfile.close();

    //write parameters to csv with names
    outfile.open(DATA_PATH + "param.csv");
    outfile << "N_nodes,N_trajectories,Nt\n";
    outfile <<  N_nodes << "," << N_SingleRun_Trajectories <<  "\n";
    outfile.close();

    float tvec[Nt + 1];
    for (int i = 0; i < Nt + 1; i++)
    {
        tvec[i] = i * dt;
    }

    //write time vector to csv
    outfile.open(DATA_PATH + "tvec.csv");
    for (int i = 0; i < Nt + 1; i++)
    {
        outfile << tvec[i] << "\n";
    }

    return SUCCESS;
}