#pragma once

template <class T>
class AverageFilter
{
public:
	explicit AverageFilter() = default;
	~AverageFilter() = default;

	T getAverage() const { 
		T sum = 0.0f;
		for (size_t i = 0; i < SAMPLES; i++) {
			sum += samples[i];
		}
		return sum / static_cast<T>(SAMPLES);
	};

	void newSample(const T sample) {
		samples[next_sample_idx] = sample;
		next_sample_idx++;
		next_sample_idx %= SAMPLES;
	};

private:
	static const size_t SAMPLES = 10;
    size_t next_sample_idx = 0;
	T samples[SAMPLES] = { static_cast<T>(0) };
};

