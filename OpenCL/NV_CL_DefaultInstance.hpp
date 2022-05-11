#ifndef NV_CL_DEFAULT_INSTANCE_HPP
#define NV_CL_DEFAULT_INSTANCE_HPP
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <cassert>
#include <stdlib.h>
#include <CL/cl.h>
#include <NV_CL_Util.hpp>

struct CL_Instance
{
    cl_uint numPlatforms;
    cl_platform_id platform;
    cl_uint numDevices = 0;
    std::vector<cl_device_id> device_ids;
    cl_context context;
    cl_command_queue commandQueue;
    cl_program program;
};

CL_Instance clDefaultInitialize(uint device_type = CL_DEVICE_TYPE_GPU)
{
    CL_Instance clInstance;
    	/*Step1: Getting platforms and choose an available one.*/
	cl_uint numPlatforms; //the NO. of platforms
	cl_int status = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (status != CL_SUCCESS)
	{
		std::cout << "Error: Getting platforms!" << std::endl;
		return clInstance;
	}

	/*For clarity, choose the first available platform. */
	if (numPlatforms > 0)
	{
		cl_platform_id* platforms = 
                     (cl_platform_id*)malloc(numPlatforms * sizeof(cl_platform_id));
		status = clGetPlatformIDs(numPlatforms, platforms, NULL);
		clInstance.platform = platforms[0];
		free(platforms);
	}

	status = clGetDeviceIDs(clInstance.platform, device_type, 0, NULL, &clInstance.numDevices);
	if (clInstance.numDevices == 0) //no GPU available.
	{
		std::cout << "No GPU device available." << std::endl;
		std::cout << "Choose CPU as default device." << std::endl;
		status = clGetDeviceIDs(clInstance.platform, device_type, 0, NULL, &clInstance.numDevices);
		clInstance.device_ids.resize(clInstance.numDevices);
		status = clGetDeviceIDs(clInstance.platform, device_type, clInstance.numDevices, clInstance.device_ids.data(), NULL);
	}
	else
	{
		clInstance.device_ids.resize(clInstance.numDevices);
		status = clGetDeviceIDs(clInstance.platform, device_type, clInstance.numDevices, clInstance.device_ids.data(), NULL);
	}


	/*Step 3: Create context.*/
	clInstance.context = clCreateContext(NULL, 1, clInstance.device_ids.data(), NULL, NULL, NULL);

	/*Step 4: Creating command queue associate with the context.*/
	clInstance.commandQueue = clCreateCommandQueueWithProperties(clInstance.context, clInstance.device_ids[0], NULL, NULL);
    return clInstance;

}
cl_program clLoadProgram(std::string filename, cl_context& context, int& err)
{
	std::string sourceStr;
	int status = convertToString(filename.c_str(), sourceStr);
	const char *source = sourceStr.c_str();
	size_t sourceSize[] = { strlen(source) };
	return clCreateProgramWithSource(context, 1, &source, sourceSize, &err);
}

#endif