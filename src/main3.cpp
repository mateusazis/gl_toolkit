#include <cstdlib>
#include <ctime>
#include <iostream>
#include <CL/cl.h>
#include <cassert>
#include "utils.h"
#include <CL/cl_gl.h>

using namespace std;

const char* errorMessage(cl_int error)
{
	return "";
}

// shrRoundUp returns the smallest multiple of local_ws bigger than size
static size_t shrRoundUp(int m, int minimum){
	int i = 1;
	while (i * m < minimum)
		i++;
	return i * m;
}

int main4(){
	// ===================================== SETUP =====================================
	cl_int error = 0;   // Used to handle error codes
	cl_platform_id platform;
	cl_context context;
	cl_command_queue queue;
	cl_device_id device;
	
	// Platform
	cl_uint platformCount;
	error = clGetPlatformIDs(1, &platform, &platformCount);
	if (error != CL_SUCCESS) {
		cout << "Error getting platform id: " << errorMessage(error) << endl;
		exit(error);
	}
	// Device
	error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	if (error != CL_SUCCESS) {
		cout << "Error getting device ids: " << errorMessage(error) << endl;
		exit(error);
	}
	// Context
	context = clCreateContext(0, 1, &device, NULL, NULL, &error);
	if (error != CL_SUCCESS) {
		cout << "Error creating context: " << errorMessage(error) << endl;
		exit(error);
	}
	// Command-queue
	queue = clCreateCommandQueue(context, device, 0, &error);
	if (error != CL_SUCCESS) {
		cout << "Error creating command queue: " << errorMessage(error) << endl;
		exit(error);
	}
	typedef float num;

	// ===================================== ALLOCATION =====================================
	const int size = 123456;
	num* src_a_h = new num[size];
	num* src_b_h = new num[size];
	num* res_h = new num[size];
	srand(time(NULL));
	// Initialize both vectors
	for (int i = 0; i < size; i++) {
		src_a_h[i] = rand() % 1000;
		src_b_h[i] = rand() % 1000;
	}

	const int mem_size = sizeof(num)*size;
	// Allocates a buffer of size mem_size and copies mem_size bytes from src_a_h
	cl_mem src_a_d = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, mem_size, src_a_h, &error);
	cl_mem src_b_d = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, mem_size, src_b_h, &error);
	cl_mem res_d = clCreateBuffer(context, CL_MEM_WRITE_ONLY, mem_size, NULL, &error);

	// ===================================== PROGRAM CREATION =====================================

	// Creates the program
	// Uses NVIDIA helper functions to get the code string and it's size (in bytes)
	size_t src_size = 0;
	//const char* path = shrFindFilePath("vector_add_gpu.cl", NULL);
	//const char* source = oclLoadProgSource(path, "", &src_size);

	char src[2048] = "";
	readFile("vector_add_gpu.cl", src);
	const char *source = src;
	cl_program program = clCreateProgramWithSource(context, 1, &source, &src_size, &error);
	assert(error == CL_SUCCESS);

	// Builds the program
	error = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
	assert(error == CL_SUCCESS);

	// Shows the log
	char* build_log;
	size_t log_size;
	// First call to know the proper size
	clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
	build_log = new char[log_size + 1];
	// Second call to get the log
	clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, build_log, NULL);
	build_log[log_size] = '\0';
	cout << build_log << endl;
	delete[] build_log;

	// Extracting the kernel
	cl_kernel vector_add_k = clCreateKernel(program, "vector_add_gpu", &error);
	assert(error == CL_SUCCESS);

	// ===================================== LAUNCH PROGRAM =====================================

	// Enqueuing parameters
	// Note that we inform the size of the cl_mem object, not the size of the memory pointed by it
	//printf("Error: %x\n", error);
	error = clSetKernelArg(vector_add_k, 0, sizeof(cl_mem), &src_a_d);
	//printf("Error: %x\n", error);
	error |= clSetKernelArg(vector_add_k, 1, sizeof(cl_mem), &src_b_d);
	//printf("Error: %x\n", error);
	error |= clSetKernelArg(vector_add_k, 2, sizeof(cl_mem), &res_d);
	//printf("Error: %x, sizeof: %d\n", error, sizeof(size));
	error |= clSetKernelArg(vector_add_k, 3, sizeof(size), &size);
	//printf("Error: %x, 1: %d, 2: %d, 3: %d, 4: %d\n", error, error == CL_INVALID_KERNEL, error == CL_INVALID_ARG_INDEX, error == CL_INVALID_ARG_VALUE, error == CL_INVALID_ARG_SIZE);
	assert(error == CL_SUCCESS);

	// Launching kernel
	const size_t local_ws = 512;	// Number of work-items per work-group
	// shrRoundUp returns the smallest multiple of local_ws bigger than size
	//const size_t global_ws = 512;// shrRoundUp(local_ws, size);	// Total number of work-items
	const size_t global_ws = shrRoundUp(local_ws, size);	// Total number of work-items
	printf("Processing...\n");
	error = clEnqueueNDRangeKernel(queue, vector_add_k, 1, NULL, &global_ws, &local_ws, 0, NULL, NULL);
	printf("Processing done!\n");
	assert(error == CL_SUCCESS);

	// ===================================== READ BACK =====================================

	// Reading back
	num* check = new num[size];
	clEnqueueReadBuffer(queue, res_d, CL_TRUE, 0, mem_size, check, 0, NULL, NULL);

	// ===================================== PRINT RESULT =====================================

	for (int i = 0; i < size; i++) 
		printf("%4.0f + %4.0f = %4.0f\n", src_a_h[i], src_b_h[i], check[i]);

	// ===================================== CLEANUP =====================================

	// Cleaning up
	delete[] src_a_h;
	delete[] src_b_h;
	delete[] res_h;
	delete[] check;
	clReleaseKernel(vector_add_k);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	clReleaseMemObject(src_a_d);
	clReleaseMemObject(src_b_d);
	clReleaseMemObject(res_d);

	system("pause");
	return 0;
}