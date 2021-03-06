#include<omp.h>
#include<iostream>

//Test parallel directive
void test_openmp_parallel1()
{
#pragma omp parallel
	{
		std::cout << omp_get_thread_num();
	}
}

//Test parallel directive with thread number
void test_openmp_parallel2()
{
#pragma omp parallel num_threads(3)
	{
		std::cout << omp_get_thread_num();
	}
}

//Test for directive: one format
void test_openmp_parallel_for1()
{
	int data[1000];
	#pragma omp parallel
		{
			#pragma omp for
				for (int i = 0; i < 1000; ++i)
					data[i] = i;
		 }
}

////Test parallel directive: the othe format
void test_openmp_parallel_for2()
{
	int data[1000];
	#pragma omp parallel for
		for (int i = 0; i < 1000; ++i)
		 data[i] = i;
}

////Test section directive
void test_openmp_parallel_section()
{
#pragma omp parallel sections
	{
		#pragma omp section
		std::cout << omp_get_thread_num();
		#pragma omp section
		std::cout << omp_get_thread_num();
	}
}

//Test critical directive
void test_openmp_parallel_critical()
{
#pragma omp parallel num_threads(4)
	{
		#pragma omp critical
			std::cout << omp_get_thread_num() << omp_get_thread_num();
	}

}

int main()
{
	//Alternatively, use one of following test function

	//test_openmp_parallel1();
	//test_openmp_parallel2();
	//test_openmp_parallel_for1();
	//test_openmp_parallel_for2();
	//test_openmp_parallel_section();
	test_openmp_parallel_critical();
    return 0;
}

