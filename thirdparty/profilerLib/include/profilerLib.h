#pragma once
#include <intrin.h>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

///////////////////////////////////////////
//https://github.com/meemknight/profilerLib
//do not remove this notice
//(c) Luta Vlad
// 
// 1.0.0
// 
///////////////////////////////////////////


//set this to true to remove the implementation
//usefull to quickly remove debug and profile cod and to port to
//other platforms
//the code that uses this library will still compile but won't do anything
#define PROFILER_LIB_REMOVE_IMPLEMENTATION 0

#define PROFILER_LIB_AVERAGE_PROFILER_MAX_TESTS 200


namespace PL 
{

	//todo port to linux

	struct ProfileRezults
	{
		float timeSeconds=0;
		unsigned int cpuClocks=0;
	};

#if !PROFILER_LIB_REMOVE_IMPLEMENTATION

	struct PerfFreqvency
	{
		PerfFreqvency()
		{
			QueryPerformanceFrequency(&perfFreq);
		}

		LARGE_INTEGER perfFreq;
	};
	const static PerfFreqvency freq;


	struct Profiler
	{

		LARGE_INTEGER startTime = {};
		__int64 cycleCount = {};

		void start()
		{
			QueryPerformanceCounter(&startTime);
			cycleCount = __rdtsc();
		}

		ProfileRezults end()
		{
			__int64 endCycleCount = __rdtsc();
			LARGE_INTEGER endTime;
			QueryPerformanceCounter(&endTime);

			cycleCount = endCycleCount - cycleCount;
			startTime.QuadPart = endTime.QuadPart - startTime.QuadPart;


			ProfileRezults r = {};

			r.timeSeconds = (float)startTime.QuadPart / (float)freq.perfFreq.QuadPart;
			r.cpuClocks = cycleCount;

			return r;
		}

	};


	struct AverageProfiler
	{
		ProfileRezults rezults[PROFILER_LIB_AVERAGE_PROFILER_MAX_TESTS];
		int index = 0;

		Profiler profiler;

		void start()
		{
			profiler.start();
		}

		ProfileRezults end()
		{
			auto r = profiler.end();
			
			if(index < PROFILER_LIB_AVERAGE_PROFILER_MAX_TESTS)
			{
				rezults[index] = r;
				index++;
			}

			return r;
		}

		ProfileRezults getAverageNoResetData()
		{
			if (index == 0)
			{
				return { 0,0 };
			}

			long double time = 0;
			unsigned long cpuTime = 0;

			for(int i=0;i<index;i++)
			{
				time += rezults[i].timeSeconds;
				cpuTime += rezults[i].cpuClocks;
			}


			return { (float)(time / index), cpuTime /index };
		}
		
		void resetData()
		{
			index = 0;
		}

		ProfileRezults getAverageAndResetData()
		{
			auto r = getAverageNoResetData();
			resetData();
			return r;
		}


	};

#else

	struct Profiler
	{
		
		void start()
		{
			
		}
	
		ProfileRezults end()
		{	
			return {};
		}
	
	};
	
	
	struct AverageProfiler
	{
		
		void start()
		{
		}
	
		ProfileRezults end()
		{
			return {};
		}
	
		ProfileRezults getAverageNoResetData()
		{
			return {};
		}
	
		void resetData()
		{
		}
	
		ProfileRezults getAverageAndResetData()
		{
			return {};
		}
	
	};



#endif

};