template <typename T>
class Timer
{
private:
	T time;

public:
	Timer()
	{
		this->time = 0;
	}

	~Timer(){};

	T get_time()
	{
		return this->time++;
	};
};
