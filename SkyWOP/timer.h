#pragma once
#include <chrono>
#include <vector>
#include <optional>
#include <fstream>
#include <string>


class Timer {
public:
	/// <summary>
	/// Start the timer
	/// </summary>
	void start() {
		timestamp_start = std::chrono::high_resolution_clock::now();
	}

	/// <summary>
	/// Stop the timer
	/// </summary>
	void end() {
		timestamp_end = std::chrono::high_resolution_clock::now();
	}

	//std::chrono::duration<long double, std::micro> get_elapsed_us() const {
	//	const std::chrono::duration elapsed = duration_cast<std::chrono::microseconds>(timestamp_end - timestamp_start);
	//	return elapsed;
	//}
	//const std::chrono::duration<long double, std::milli> get_elapsed_ms() const {
	//	const std::chrono::duration elapsed = duration_cast<std::chrono::milliseconds>(timestamp_end - timestamp_start);
	//	return elapsed;
	//}
	//const std::chrono::duration<long double> get_elapsed_s() const {
	//	const std::chrono::duration elapsed = duration_cast<std::chrono::seconds>(timestamp_end - timestamp_start);
	//	return elapsed;
	//}

	/// <summary>
	/// Get the duration between start time and end time
	/// </summary>
	/// <typeparam name="DURATION">An std::chrono::duration type to be used as the return type. Default = std::chrono::microseconds</typeparam>
	/// <returns>An std::chrono::duration of type DURATION</returns>
	template <typename DURATION = std::chrono::duration<std::chrono::microseconds>> DURATION get_elapsed() {
		return duration_cast<DURATION>(timestamp_end - timestamp_start);
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> timestamp_start;
	std::chrono::time_point<std::chrono::high_resolution_clock> timestamp_end;

};

template<unsigned int DATAPOINT_COUNT = 1000>
class Timer_Avg : public Timer {
	typedef std::chrono::duration<long double, std::nano> HIGHREZ_DURATION;
public:
	/// <summary>
	/// Resets the timer, zeroing out datapoint count, setting timing started to false and filling the datapoints array with 0s
	/// </summary>
	void reset() {
		std::fill(m_durations, m_durations + DATAPOINT_COUNT, std::chrono::duration_cast<HIGHREZ_DURATION>(HIGHREZ_DURATION().zero()));
		m_current_datapoint_count = 0;
		m_datapoints_to_skip = 0;
		m_timing_started = false;
	}
	/// <summary>
	/// Starts the timer
	/// </summary>
	/// <param name="datapoints_to_skip">Number of datapoints that will be skipped before timing will start. Default  =  0 </param>
	void start(unsigned int datapoints_to_skip = 0) {
		m_datapoints_to_skip = datapoints_to_skip;
		Timer::start();

	}
	/// <summary>
	/// Stops one iteration of the timer and adds the elapsed time to the array of datapoints. Skips this if still skipping datapoints
	/// </summary>
	void end() {
		// Exit early if we're still skipping datapoints
		if (m_datapoints_to_skip != 0) {
			m_datapoints_to_skip--;
			// Restart the timer if we've just got to the end of our skipped datapoints
			if (m_datapoints_to_skip == 0) {
				start();
			}
			return;
		}
		// We're not skipping so end the timer, add it to the array of times and bump datapoint count
		Timer::end();
		m_durations[m_current_datapoint_count] = Timer::get_elapsed<HIGHREZ_DURATION>();
		m_current_datapoint_count++;
	}
	/// <summary>
	/// Checks whether all datapoints have been collected
	/// </summary>
	const bool are_all_times_collected() const {
		return m_current_datapoint_count >= DATAPOINT_COUNT;
	}

	const unsigned int get_datapoint_count() const {
		return DATAPOINT_COUNT;
	}
	/// <summary>
	/// Gets the average time between datapoints if all datapoints have been collected.
	/// </summary>
	/// <typeparam name="DURATION">An std::chrono::duration type to be used as the return type. Default = std::chrono::microseconds</typeparam>
	/// <returns>Duration of average time of type DURATION or nulloption if not all datapoints have been colleced.</returns>
	template<typename DURATION = std::chrono::duration<std::chrono::microseconds>>
	const std::optional<DURATION> get_avg_time() const {
		if (!are_all_times_collected()) return std::nullopt;
		HIGHREZ_DURATION total_duration(0);
		for (HIGHREZ_DURATION this_duration : m_durations) {
			total_duration += this_duration;
		}

		HIGHREZ_DURATION avg_in_nano = total_duration / DATAPOINT_COUNT;
		return duration_cast<DURATION>(avg_in_nano);
	}
	/// <summary>
	/// Gets the maximum time between datapoints if all datapoints have been collected.
	/// </summary>
	/// <typeparam name="DURATION">An std::chrono::duration type to be used as the return type. Default = std::chrono::microseconds</typeparam>
	/// <returns>Duration of maximum time of type DURATION or nulloption if not all datapoints have been collected.</returns>
	template<typename DURATION = std::chrono::duration<std::chrono::microseconds>>
	const std::optional<DURATION> get_max_time() const {
		if (!are_all_times_collected()) return std::nullopt;
		auto max = *std::max_element(m_durations, m_durations + DATAPOINT_COUNT);
		return max;
	}
	/// <summary>
	/// Gets the minimum time between datapoints if all datapoints have been collected.
	/// </summary>
	/// <typeparam name="DURATION">An std::chrono::duration type to be used as the return type. Default = std::chrono::microseconds</typeparam>
	/// <returns>Duration of minimum time of type DURATION or nulloption if not all datapoints have been collected.</returns>
	template<typename DURATION = std::chrono::duration<std::chrono::microseconds>>
	const std::optional<DURATION> get_min_time() const {
		if (!are_all_times_collected()) return std::nullopt;
		auto min = *std::min_element(m_durations, m_durations + DATAPOINT_COUNT);
		return min;
	}

protected:
	//std::array<std::chrono::duration<long double, std::nano>, DATAPOINT_COUNT> m_durations;
	/// Array of durations between timing points
	HIGHREZ_DURATION m_durations[DATAPOINT_COUNT];
	/// Number of timing points collected so far
	unsigned int m_current_datapoint_count = 0;
	/// Number of timing points left to skip
	unsigned int m_datapoints_to_skip = 0;
	/// Whether timing has started, accounting for skipped timing points
	bool m_timing_started = false;
};



template<unsigned int DATAPOINT_COUNTA = 1000>
class Timer_File : public Timer_Avg<DATAPOINT_COUNTA> {
public:
	/// <summary>
	/// Write the averages to a .csv file if all datapoints have been collected. Overwites existing files with the same name.
	/// </summary>
	/// <typeparam name="DURATION"></typeparam>
	/// <param name="file_name">Name of the file to be written to. Should not include file ype ending.</param>
	template<typename DURATION = std::chrono::duration<std::chrono::microseconds>>
	void write_to_file(const std::string& file_name = "profiling_times") const {
		// Exit early if not all times are collected yet
		if (!Timer_Avg<DATAPOINT_COUNTA>::are_all_times_collected()) { return; }

		std::ofstream timing_file;
		std::string file_path = file_name + ".csv";
		timing_file.open(file_path);

		// Write Header
		timing_file << "frame,time\n";

		// Write Timing Lines
		for (int i = 0; i < (Timer_Avg<DATAPOINT_COUNTA>::get_datapoint_count() - 1); i++) {
			timing_file << i << "," << duration_cast<DURATION>(Timer_Avg<DATAPOINT_COUNTA>::m_durations[i]).count() << "\n";
		}

		timing_file.close();
	}
};