#include <CL/cl.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <cassert>
#include <random>
#define SUCCESS 0
#define FAILURE 1
#include <NV_CL_Util.hpp>
#include <NV_FilePaths.h>


int main(int argc, char* argv[])
{

	std::string cl_generator_dir = NV_CL_GENERATOR_DIR;
	std::string pwd = std::string(NV_OPENCL_DIR) + "/Examples/";

	/*Step1: Getting platforms and choose an available one.*/
	cl_uint numPlatforms; //the NO. of platforms
	cl_platform_id platform = NULL; //the chosen platform
	cl_int status = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (status != CL_SUCCESS)
	{
		std::cout << "Error: Getting platforms!" << std::endl;
		return FAILURE;
	}

	/*For clarity, choose the first available platform. */
	if (numPlatforms > 0)
	{
		cl_platform_id* platforms = 
                     (cl_platform_id*)malloc(numPlatforms * sizeof(cl_platform_id));
		status = clGetPlatformIDs(numPlatforms, platforms, NULL);
		platform = platforms[0];
		free(platforms);
	}

	/*Step 2:Query the platform and choose the first GPU device if has one.Otherwise use the CPU as device.*/
	cl_uint numDevices = 0;
	cl_device_id        *devices;
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
	if (numDevices == 0) //no GPU available.
	{
		std::cout << "No GPU device available." << std::endl;
		std::cout << "Choose CPU as default device." << std::endl;
		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 0, NULL, &numDevices);
		devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, numDevices, devices, NULL);
	}
	else
	{
		devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, numDevices, devices, NULL);
	}


	/*Step 3: Create context.*/
	cl_context context = clCreateContext(NULL, 1, devices, NULL, NULL, NULL);

	/*Step 4: Creating command queue associate with the context.*/
	cl_command_queue commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);

	/*Step 5: Create program object */
	std::string filename = pwd + "kernels/HelloWorld_Kernel.cl";
    std::cout << filename << std::endl;
	std::string sourceStr;
	status = convertToString(filename.c_str(), sourceStr);
	const char *source = sourceStr.c_str();
	size_t sourceSize[] = { strlen(source) };
    int err = 0;
	cl_program program = clCreateProgramWithSource(context, 1, &source, sourceSize, &err);

    assert(err == CL_SUCCESS);

    std::string  build_options = "-I "  + cl_generator_dir;
	/*Step 6: Build program. */
	status = clBuildProgram(program, 1, devices, build_options.c_str(), NULL, NULL);

    size_t count = 1024;
    uint seeds[count];
    std::default_random_engine rng;
    std::uniform_int_distribution<uint> dist(0, 10000);
    for (int i = 0; i < count; i++)
    {
        seeds[i] = dist(rng);
        std::cout << seeds[i] << std::endl;
    }
	cl_mem inputBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
                             (count) * sizeof(uint), (void *) seeds, NULL);
	cl_mem outputBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
                              count * sizeof(uint), NULL, NULL);

	/*Step 8: Create kernel object */
	cl_kernel kernel = clCreateKernel(program, "HelloWorld", NULL);

	/*Step 9: Sets Kernel arguments.*/
	// status = clSetKernelArg(kernel, 0, sizeof(size_t), &num);
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&inputBuffer);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&outputBuffer);

	/*Step 10: Running the kernel.*/
	size_t global_work_size[1] = { count };
	status = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, 
                                        global_work_size, NULL, 0, NULL, NULL);
    uint output[count];
    for (int i = 0; i < count; i++)
    {
        output[i] = 0;
    }


	/*Step 11: Read the cout put back to host memory.*/
	status = clEnqueueReadBuffer(commandQueue, outputBuffer, CL_TRUE, 0, 
                 count * sizeof(uint), output, 0, NULL, NULL);



	/*Step 12: Clean the resources.*/
	status = clReleaseKernel(kernel); //Release kernel.
	status = clReleaseProgram(program); //Release the program object.
	status = clReleaseMemObject(inputBuffer); //Release mem object.
	status = clReleaseMemObject(outputBuffer);
	status = clReleaseCommandQueue(commandQueue); //Release  Command queue.
	status = clReleaseContext(context); //Release context.

    std::cout << "RNG-numbers:\n";
    for (int i = 0; i < count; i++)
    {
        std::cout << output[i] << std::endl;
    }

	if (devices != NULL)
	{
		free(devices);
		devices = NULL;
	}

	return SUCCESS;
}