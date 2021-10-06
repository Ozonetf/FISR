#include <chrono>
#include <iostream>
#include <thread>  
#include <xmmintrin.h>
using namespace std;
using namespace std::chrono;

__forceinline float FISR(float n)
{
	const float threehalf = 1.5F;
	float y = n;

	long i = *(long*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;

	y = y * (threehalf - ((n * 0.5F) * y * y));
	return y;
}

__forceinline float SIMDsqrt(const float f)
{
	__m128 temp = _mm_set_ss(f);
	temp = _mm_rsqrt_ss(temp);
	return _mm_cvtss_f32(temp);
}

uint64_t countFISR(long long secondsToRun)
{
	const auto start = steady_clock::now();
	float value = 100;
	auto sum = 0.0;
	auto passes = 0UL;

	while (duration_cast<seconds>((steady_clock::now() - start)).count() < secondsToRun)
	{
		for (int i = 0; i < 1000; i++)
		{
			passes++;
			sum += FISR((float)value);
		}
	}
	//outputval = sum;
	return passes;
}


uint64_t countsqrtf(long long secondsToRun)
{
	const auto start = steady_clock::now();
	float value = 100;
	auto sum = 0.0;
	auto passes = 0UL;

	while (duration_cast<seconds>((steady_clock::now() - start)).count() < secondsToRun)
	{
		for (int i = 0; i < 1000; i++)
		{
			passes++;
			sum += 1 / sqrt(value);
		}
	}
	return passes;
}


uint64_t countSIMD(long long secondsToRun)
{
	const auto start = steady_clock::now();
	float value = 100;
	auto sum = 0.0;
	auto passes = 0UL;

	while (duration_cast<seconds>((steady_clock::now() - start)).count() < secondsToRun)
	{
		for (int i = 0; i < 1000; i++)
		{
			passes++;
			sum += SIMDsqrt(value);
		}
	}
	return passes;
}

int main()
{
	auto sqrtfcount = countsqrtf(1);
	auto SIMDcount = countSIMD(1);
	auto FISRtcount = countFISR(1);
	cout << "fast invers square root passes " << FISRtcount << endl;
	cout << "standard square root passes    " << sqrtfcount << endl;
	cout << "SIMD square root passes        " << SIMDcount << endl;
}