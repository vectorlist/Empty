#include <Core/PCH.h>
#include <Core/Profiler.h>

std::unordered_map<std::string, ProfilerSample> Profiler::gSamples;

void Profiler::Start(const std::string& name)
{
	//Try To Find Sample
	try 
	{
		ProfilerSample* sample = &gSamples.at(name);
		if (sample->IsStarted) {
			//TODO : warnnings Override start time
			ASSERT_MSG(0, (std::string(name) + " sample is already started").c_str());
		}
		sample->StartTime = Clock::now();
		sample->IsStarted = true;
	}
	catch(std::out_of_range& e)
	{
		//Create New Sample When Except
		ProfilerSample sample;
		sample.StartTime = Clock::now();
		sample.IsStarted = true;
		gSamples[name] = sample;
	}
}

void Profiler::End(const std::string & name)
{
	try
	{
		ProfilerSample* sample = &gSamples.at(name);
		if (sample->IsStarted) {
			sample->IsStarted = false;
			
			auto elapse = std::chrono::duration_cast<std::chrono::duration<double>>
				(Clock::now() - sample->StartTime);
			sample->ElapseTime = elapse.count();
		}
		else {
			//TODO : EXIT
			ASSERT_MSG(0, (std::string(name) + " sample is must be start").c_str());
		}
	}
	catch (std::out_of_range& e)
	{
		exit(-1);
	}
}

double Profiler::GetMS(const std::string& name)
{
	try {
		ProfilerSample* sample = &gSamples.at(name);
		if (!sample->IsStarted) {
			return sample->ElapseTime;
		}
		else {
			return 0.0;
		}
	}
	catch (std::out_of_range& e) {
		return 0.0;
	}
}
