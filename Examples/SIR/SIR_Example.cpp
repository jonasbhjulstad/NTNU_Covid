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
#define SUCCESS 0
#define FAILURE 1
#include <NV_FilePaths.h>
#include <NV_CL_DefaultInstance.hpp>

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

int main(int argc, char *argv[])
{

    std::string DATA_PATH = NV_EXAMPLES_DATA_DIR;
    std::string cl_generator_dir = NV_CL_GENERATOR_DIR;
    std::string pwd = std::string(NV_OPENCL_DIR) + "/Examples/";
    std::string epidemiological_kernel_dir = std::string(NV_CL_KERNEL_DIR) + "/Epidemiological/";

    CL_Instance clInstance = clDefaultInitialize();

    size_t cacheSize = 0;
    clGetDeviceInfo(clInstance.device_ids[0], CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(size_t), &cacheSize, NULL);
    std::cout << "Device cache size:\t" << cacheSize << std::endl;

    size_t maxWorkGroupSize = 0;
    clGetDeviceInfo(clInstance.device_ids[0], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &maxWorkGroupSize, NULL);
    std::cout << "Device max work group size:\t" << maxWorkGroupSize << std::endl;

    int err = 0;
    clInstance.program = clLoadProgram((epidemiological_kernel_dir + "SIR_Compute_Stochastic.cl").c_str(), clInstance.context, err);

    assert(err == CL_SUCCESS);

    std::string build_options = "-I " + cl_generator_dir + " -I " + epidemiological_kernel_dir;
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

    /* Parameter/Buffer initialization */
    constexpr size_t N_trajectories = 10 * 1024;
    size_t N_SingleRun_Trajectories = (N_trajectories > maxWorkGroupSize) ? maxWorkGroupSize : N_trajectories;
    constexpr uint Nt = 100;

    size_t N_runs = std::ceil(N_trajectories / N_SingleRun_Trajectories);

    ulong seeds[N_SingleRun_Trajectories];

    float dt = 5.;

    typedef std::array<float, 3> state_t;
    float N_pop = 1e2;
    state_t x0;
    x0[1] = 10;
    x0[0] = N_pop - x0[1];
    x0[2] = 0;

    float R0 = 2.0;
    float alpha = .1 / 9;
    float beta = alpha * R0;

    std::array<float, 3> param;
    param[0] = alpha;
    param[1] = beta;
    param[2] = N_pop;

    size_t seedBufferSize = N_SingleRun_Trajectories * sizeof(ulong);
    size_t trajectorySize = 3 * (Nt + 1) * sizeof(float);
    size_t trajectoryBufferSize = trajectorySize * N_SingleRun_Trajectories;

    assert(trajectoryBufferSize < cacheSize);

    std::vector<float> x_traj(3 * (Nt + 1) * N_SingleRun_Trajectories);

    cl_mem seedBuffer = clCreateBuffer(clInstance.context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                       seedBufferSize, (void *)seeds, &err);

    cl_mem x0Buffer = clCreateBuffer(clInstance.context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                     sizeof(state_t), (void *)&x0, &err);
    cl_mem outputBuffer = clCreateBuffer(clInstance.context, CL_MEM_WRITE_ONLY,
                                         trajectoryBufferSize, NULL, &err);
    cl_mem paramBuffer = clCreateBuffer(clInstance.context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                        3 * sizeof(float), (void *)param.data(), &err);
    /*Step 8: Create kernel object */
    cl_kernel kernel = clCreateKernel(clInstance.program, "SIR_Compute_Stochastic", &err);
    assert(err == CL_SUCCESS);
    /*Step 9: Sets Kernel arguments.*/
    // status = clSetKernelArg(kernel, 0, sizeof(size_t), &num);
    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&seedBuffer);
    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&x0Buffer);
    status = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&outputBuffer);
    // status = clSetKernelArg(kernel, 3,  trajectorySize, NULL);
    status = clSetKernelArg(kernel, 3, sizeof(float), (void *)&dt);
    status = clSetKernelArg(kernel, 4, sizeof(uint), (void *)&Nt);
    status = clSetKernelArg(kernel, 5, sizeof(cl_mem), &paramBuffer);

    assert(status == CL_SUCCESS);

    std::ofstream outfile;
    outfile.open(DATA_PATH + "x_traj.csv");
    std::mt19937 rng;
    std::uniform_int_distribution<ulong> dist(0, UINT64_MAX);

    for (int i = 0; i < N_runs; i++)
    {
        for (int j = 0; j < N_SingleRun_Trajectories; j++)
        {
            seeds[j] = dist(rng);
        }
        clEnqueueWriteBuffer(clInstance.commandQueue, seedBuffer, CL_TRUE, 0, seedBufferSize, seeds, 0, NULL, NULL);
        std::cout << "Run " << i << std::endl;
        size_t global_work_size[1] = {N_SingleRun_Trajectories};
        status = clEnqueueNDRangeKernel(clInstance.commandQueue, kernel, 1, NULL,
                                        global_work_size, NULL, 0, NULL, NULL);

        clFinish(clInstance.commandQueue);

        status = clEnqueueReadBuffer(clInstance.commandQueue, outputBuffer, CL_TRUE, 0,
                                     trajectoryBufferSize, (void *)x_traj.data(), 0, NULL, NULL);

        assert(status == CL_SUCCESS);
        write_SIR_trajectories(outfile, x_traj.data(), Nt, N_SingleRun_Trajectories);
    }

    status = clReleaseKernel(kernel);              //Release kernel.
    status = clReleaseProgram(clInstance.program); //Release the program object.
    status = clReleaseMemObject(outputBuffer);
    status = clReleaseCommandQueue(clInstance.commandQueue); //Release  Command queue.
    status = clReleaseContext(clInstance.context);           //Release context.

    outfile.close();

    //write parameters to csv with names
    outfile.open(DATA_PATH + "param.csv");
    outfile << "alpha,beta,N_pop,N_trajectories,Nt,dt\n";
    outfile << alpha << "," << beta << "," << N_pop << "," << N_trajectories << "," << Nt << "," << dt << "\n";
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