#include <iostream>
#include <fstream>
#include <sstream>
#include <CL/cl.h>


	const int ARRAY_SIZE = 1000;

	//1. Select the OpenCL platform and create a context
	cl_context CreateContext()
	{
		cl_int errNum;
		cl_uint numPlatforms;
		cl_platform_id firstPlatformId;
		cl_context context = NULL;

		//Select the first of the available platforms
		errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
		if (errNum != CL_SUCCESS || numPlatforms <= 0)
		{
			std::cerr << "Failed to find any OpenCL platforms." << std::endl;
			return NULL;
		}

		//Create an OpenCL context
		cl_context_properties contextProperties[] =
		{
			CL_CONTEXT_PLATFORM,
			(cl_context_properties)firstPlatformId,
			0
		};
		context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_GPU,
			NULL, NULL, &errNum);

		return context;
	}


	//2. Create a device and create a command queue
	cl_command_queue CreateCommandQueue(cl_context context, cl_device_id *device)
	{
		cl_int errNum;
		cl_device_id *devices;
		cl_command_queue commandQueue = NULL;
		size_t deviceBufferSize = -1;

		// Get device buffer size
		errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &deviceBufferSize);

		if (deviceBufferSize <= 0)
		{
			std::cerr << "No devices available.";
			return NULL;
		}

		// Allocate buffer space for devices
		devices = new cl_device_id[deviceBufferSize / sizeof(cl_device_id)];
		errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceBufferSize, devices, NULL);

		//Pick the first of the available devices
		commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);

		*device = devices[0];
		delete[] devices;
		return commandQueue;
	}


	// 3.Create and build program
	cl_program CreateProgram(cl_context context, cl_device_id device, const char* fileName)
	{
		cl_int errNum;
		cl_program program;

		std::ifstream kernelFile(fileName, std::ios::in);
		if (!kernelFile.is_open())
		{
			std::cerr << "Failed to open file for reading: " << fileName << std::endl;
			return NULL;
		}

		std::ostringstream oss;
		oss << kernelFile.rdbuf();

		std::string srcStdStr = oss.str();
		const char *srcStr = srcStdStr.c_str();
		program = clCreateProgramWithSource(context, 1,
			(const char**)&srcStr,
			NULL, NULL);

		errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

		return program;
	}

	//Create and build objects
	bool CreateMemObjects(cl_context context, cl_mem memObjects[3],
		float *a, float *b)
	{
		memObjects[0] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			sizeof(float) * ARRAY_SIZE, a, NULL);
		memObjects[1] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			sizeof(float) * ARRAY_SIZE, b, NULL);
		memObjects[2] = clCreateBuffer(context, CL_MEM_READ_WRITE,
			sizeof(float) * ARRAY_SIZE, NULL, NULL);
		return true;
	}


	// Release OpenCL Resources
	void Cleanup(cl_context context, cl_command_queue commandQueue,
		cl_program program, cl_kernel kernel, cl_mem memObjects[3])
	{
		for (int i = 0; i < 3; i++)
		{
			if (memObjects[i] != 0)
				clReleaseMemObject(memObjects[i]);
		}
		if (commandQueue != 0)
			clReleaseCommandQueue(commandQueue);

		if (kernel != 0)
			clReleaseKernel(kernel);

		if (program != 0)
			clReleaseProgram(program);

		if (context != 0)
			clReleaseContext(context);
	}

	int main(int argc, char** argv)
	{
		cl_context context = 0;
		cl_command_queue commandQueue = 0;
		cl_program program = 0;
		cl_device_id device = 0;
		cl_kernel kernel = 0;
		cl_mem memObjects[3] = { 0, 0, 0 };
		cl_int errNum;

		// 1.Select the OpenCL platform and create a context
		context = CreateContext();

		// 2. Create a device and create a command queue
		commandQueue = CreateCommandQueue(context, &device);

		//Create and build program objects
		program = CreateProgram(context, device, "Add.cl");

		// 4.Create OpenCL kernel and allocate memory space
		kernel = clCreateKernel(program, "add_kernel", NULL);

		//Create data to process
		float result[ARRAY_SIZE];
		float a[ARRAY_SIZE];
		float b[ARRAY_SIZE];
		for (int i = 0; i < ARRAY_SIZE; i++)
		{
			a[i] = (float)i;
			b[i] = (float)(ARRAY_SIZE - i);
		}

		//Create a memory object
		if (!CreateMemObjects(context, memObjects, a, b))
		{
			Cleanup(context, commandQueue, program, kernel, memObjects);
			return 1;
		}

		// 5.Set kernel data and execute kernel
		errNum = clSetKernelArg(kernel, 0, sizeof(cl_mem), &memObjects[0]);
		errNum |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &memObjects[1]);
		errNum |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &memObjects[2]);

		size_t globalWorkSize[1] = { ARRAY_SIZE };
		size_t localWorkSize[1] = { 1 };

		errNum = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL,
			globalWorkSize, localWorkSize,
			0, NULL, NULL);

		//6.Read the execution result and release OpenCL resources
		errNum = clEnqueueReadBuffer(commandQueue, memObjects[2], CL_TRUE,
			0, ARRAY_SIZE * sizeof(float), result,
			0, NULL, NULL);

		for (int i = 0; i < ARRAY_SIZE; i++)
		{
			std::cout << result[i] << " ";
		}
		std::cout << std::endl;
		std::cout << "Executed program succesfully." << std::endl;
		getchar();
		Cleanup(context, commandQueue, program, kernel, memObjects);


    return 0;
}

