#include <core/core_min.hpp>

#ifdef X_SYSTEM_LINUX

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

using namespace std;

struct CPUStats {
	unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
};

CPUStats getCPUStats() {
	ifstream statFile("/proc/stat");
	string   line;
	CPUStats stats = { 0, 0, 0, 0, 0, 0, 0, 0 };

	if (statFile.is_open()) {
		getline(statFile, line);
		istringstream ss(line);
		string        cpu;
		ss >> cpu >> stats.user >> stats.nice >> stats.system >> stats.idle >> stats.iowait >> stats.irq >> stats.softirq >> stats.steal;
		statFile.close();
	} else {
		cerr << "feiled to open stat file" << endl;
	}

	return stats;
}

double calculateCPUUsage(const CPUStats & prev, const CPUStats & curr) {
	unsigned long long prevIdle = prev.idle + prev.iowait;
	unsigned long long currIdle = curr.idle + curr.iowait;

	unsigned long long prevNonIdle = prev.user + prev.nice + prev.system + prev.irq + prev.softirq + prev.steal;
	unsigned long long currNonIdle = curr.user + curr.nice + curr.system + curr.irq + curr.softirq + curr.steal;

	unsigned long long prevTotal = prevIdle + prevNonIdle;
	unsigned long long currTotal = currIdle + currNonIdle;

	unsigned long long totalDiff = currTotal - prevTotal;
	unsigned long long idleDiff  = currIdle - prevIdle;

	return (double)(totalDiff - idleDiff) / totalDiff * 100.0;
}

int main() {
	while (true) {
		CPUStats prevStats = getCPUStats();
		this_thread::sleep_for(chrono::seconds(1));
		CPUStats currStats = getCPUStats();

		double cpuUsage = calculateCPUUsage(prevStats, currStats);
		cout << "CPU Usage: " << cpuUsage << "%" << endl;
	}

	return 0;
}

#elif defined(X_SYSTEM_DARWIN)

#include <sys/sysctl.h>
#include <sys/types.h>

#include <chrono>
#include <iostream>
#include <thread>

struct CPUStats {
	uint64_t userTime;
	uint64_t systemTime;
	uint64_t idleTime;
	uint64_t niceTime;
};

CPUStats getCPUStats() {
	CPUStats stats = { 0, 0, 0, 0 };
	size_t   size  = sizeof(stats.userTime);
	sysctlbyname("kern.cp_time", &stats, &size, NULL, 0);
	return stats;
}

double calculateCPUUsage(const CPUStats & prev, const CPUStats & curr) {
	uint64_t prevTotal = prev.userTime + prev.systemTime + prev.idleTime + prev.niceTime;
	uint64_t currTotal = curr.userTime + curr.systemTime + curr.idleTime + curr.niceTime;

	uint64_t totalDiff = currTotal - prevTotal;
	uint64_t idleDiff  = curr.idleTime - prev.idleTime;

	return (double)(totalDiff - idleDiff) / totalDiff * 100.0;
}

int main() {
	while (true) {
		CPUStats prevStats = getCPUStats();
		std::this_thread::sleep_for(std::chrono::minutes(1));
		CPUStats currStats = getCPUStats();

		double cpuUsage = calculateCPUUsage(prevStats, currStats);
		std::cout << "CPU Usage: " << cpuUsage << "%" << std::endl;
	}

	return 0;
}

#else

int main(int, char **) {
	xel::QuickExit();
	return 0;
}
#endif
