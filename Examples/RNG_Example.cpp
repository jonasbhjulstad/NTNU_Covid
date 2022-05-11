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
#include <NV_CL_DefaultInstance.hpp>

int main(int argc, char* argv[])
{

	std::string cl_generator_dir = NV_CL_GENERATOR_DIR;
	std::string pwd = std::string(NV_OPENCL_DIR) + "/Examples/";

	CL_Instance clInstance = clDefaultInitialize();

	int err = 0;
	clInstance.program = clLoadProgram(pwd + "kernels/HelloWorld_Kernel.cl", clInstance.context, err);


    assert(err == CL_SUCCESS);

    std::string  build_options = "-I "  + cl_generator_dir;
	/*Step 6: Build program. */
	int status = clBuildProgram(clInstance.program, 1, clInstance.device_ids.data(), build_options.c_str(), NULL, NULL);

    size_t count = 1024;
    uint seeds[count];
    std::default_random_engine rng;
    std::uniform_int_distribution<uint> dist(0, 10000);
    for (int i = 0; i < count; i++)
    {
        seeds[i] = dist(rng);
        std::cout << seeds[i] << std::endl;
    }
	cl_mem inputBuffer = clCreateBuffer(clInstance.context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
                             (count) * sizeof(uint), (void *) seeds, NULL);
	cl_mem outputBuffer = clCreateBuffer(clInstance.context, CL_MEM_WRITE_ONLY, 
                              count * sizeof(uint), NULL, NULL);

	/*Step 8: Create kernel object */
	cl_kernel kernel = clCreateKernel(clInstance.program, "HelloWorld", NULL);

	/*Step 9: Sets Kernel arguments.*/
	// status = clSetKernelArg(kernel, 0, sizeof(size_t), &num);
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&inputBuffer);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&outputBuffer);

	/*Step 10: Running the kernel.*/
	size_t global_work_size[1] = { count };
	status = clEnqueueNDRangeKernel(clInstance.commandQueue, kernel, 1, NULL, 
                                        global_work_size, NULL, 0, NULL, NULL);
    uint output[count];
    for (int i = 0; i < count; i++)
    {
        output[i] = 0;
    }


	/*Step 11: Read the cout put back to host memory.*/
	status = clEnqueueReadBuffer(clInstance.commandQueue, outputBuffer, CL_TRUE, 0, 
                 count * sizeof(uint), output, 0, NULL, NULL);



	/*Step 12: Clean the resources.*/
	status = clReleaseKernel(kernel); //Release kernel.
	status = clReleaseProgram(clInstance.program); //Release the program object.
	status = clReleaseMemObject(inputBuffer); //Release mem object.
	status = clReleaseMemObject(outputBuffer);
	status = clReleaseCommandQueue(clInstance.commandQueue); //Release  Command queue.
	status = clReleaseContext(clInstance.context); //Release context.

    std::cout << "RNG-numbers:\n";
    for (int i = 0; i < count; i++)
    {
        std::cout << output[i] << std::endl;
    }

	return SUCCESS;
}