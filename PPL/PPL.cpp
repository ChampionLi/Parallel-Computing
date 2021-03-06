#include <ppl.h>
#include <vector>
#include <random>
#include <sstream>
#include <iostream>


struct my_data
{
	int num;
	std::string note;
};

// Returns the result of adding a value to itself.
template <typename T>
T twice(const T& t) 
{
	return t + t;
}

//Test Parallel_for algorithm
void test_Parallel_for()
{
	concurrency::parallel_for(1, 6, [](int value) {
		std::wstringstream ss;
		ss << value << L' ';
		std::wcout << ss.str();
	});
}

//Test Parallel_for_each algorithm: regular
void test_Parallel_for_each1()
{
	std::vector<std::string> my_str = { "hi", "world", "hello", "c", "language"};
	
	concurrency::parallel_for_each(begin(my_str), end(my_str), [](std::string value) {
		std::cout << value<<std::endl;
	});

}

//Test Parallel_for_each algorithm: wrong use, data should be independent
void test_Parallel_for_each2()
{
	std::vector<std::string> my_str = { "hi", "world", "hello", "c", "language" };
	int mark = 100;
	concurrency::parallel_for_each(begin(my_str), end(my_str), [&mark](std::string value) {
		std::cout << value <<"  "<<mark<< std::endl;
	});

}

//Test Parallel_for_each algorithm: test class data parallel
void test_Parallel_for_each3()
{
	my_data data1 = { 100,"hi" };
	my_data data2 = { 200,"world" };
	my_data data3 = { 300,"language" };
	std::vector<my_data> my_str = { data1,data2,data3 };
	concurrency::parallel_for_each(begin(my_str), end(my_str), [](my_data value) {
		std::cout << value.num << "  " <<value.note << std::endl;
	});

}

//Test Parallel_invoke algorithm
void test_Parallel_invoke()
{
	int n = 54;
	double d = 5.6;
	std::wstring s = L"Hello";

	concurrency::parallel_invoke(
		[&n] { n = twice(n);  },
		[&d] { d = twice(d); },
		[&s] { s = twice(s);}
	);
	std::wcout << n <<" "<< d<<" "<<s<<std::endl;
}

//Test Parallel_transform algorithm
void test_Parallel_transform()
{
	std::vector<int> values(1250000);
	std::generate(begin(values), end(values), std::mt19937(42));

	std::vector<int> results(values.size());
	std::vector<int> results2(values.size());

	// Negate each element in parallel.
	concurrency::parallel_transform(begin(values), end(values), begin(results), [](int n) {
		return -n;
	});

	// Alternatively, use the negate class to perform the operation.
	concurrency::parallel_transform(begin(values), end(values),
		begin(results), std::negate<int>());


	// Demonstrate use of parallel_transform together with a binary function.
	// This example uses a lambda expression.
	concurrency::parallel_transform(begin(values), end(values), begin(results),
		begin(results2), [](int n, int m) {
		return n - m;
	});

	// Alternatively, use the multiplies class:
	concurrency::parallel_transform(begin(values), end(values), begin(results),
		begin(results2), std::multiplies<int>());

}

//Test Parallel_reduce algorithm
void test_Parallel_reduce()
{
	std::vector<std::wstring> words;
	words.push_back(L"Hello ");
	words.push_back(L"i ");
	words.push_back(L"like ");
	words.push_back(L"c ");
	words.push_back(L"language, ");
	words.push_back(L"and ");
	words.push_back(L"parallel ");
	words.push_back(L"programming.");

	// Reduce the vector to one string in parallel.
	std::wcout << concurrency::parallel_reduce(begin(words), end(words), std::wstring()) << std::endl;

}

int main()
{
	// Alternatively, use one of following test function

	//test_Parallel_for();
	//test_Parallel_for_each1();
	//test_Parallel_for_each2();
	//test_Parallel_for_each3();
	//test_Parallel_invoke();
	//test_Parallel_transform();
	test_Parallel_reduce();

    return 0;
}

