/*
Container class specific for PolyReductor system.
This class use vector and deliver specific interface for PolyReductor system.
*/

#ifndef MY_VECTOR
#define MY_VECTOR

#include <vector>

template<typename T>
class MyVector
{
public:
	MyVector()
	{
		//size = 0;
		//data = nullptr;
	}
	MyVector(size_t size)
	{
		//data = (T*)malloc(size*sizeof(T));

		data.resize(size);
		//this->size = size;
	}

	template<typename Iter>
	Iter begin()
	{
		return data.begin();
	}

	template<typename Iter>
	Iter end()
	{
		return data.end();;
	}

	size_t findElement(T element)
	{
		return data.size();
	}

	template<typename F>
	size_t findElement(F comparator)
	{
		int i{ 0 };
		bool found{ false };
		for (i; i < data.size(); ++i)
		{
			if (comparator(data[i]))
			{
				found = true;
				break;
			}
		}
		if (found == true)
		{
			return i;
		}
		else
		{
			return -1;
		}
	}

	void eraseElement(T element)
	{
		/*int i{ 0 };
		bool found{ false };
		for (i; i < size; ++i)
		{
			if (data[i] == element)
			{
				found = true;
				break;
			}
		}
		if (found == true)
		{
			T* tempData = (T*)malloc(sizeof(T)*(size - 1));
			for (int j = 0; j < i; ++i)
			{
				tempData[j] = data[j];
			}
			for (int j = i + 1; j < size; ++j)
			{
				tempData[j - 1] = data[j];
			}
			data = tempData;
			--size;
			if (size == 0)
			{
				if (data != nullptr)
				{
					free(data);
					data = nullptr;
				}
			}
		}*/
	}

	/*erase element if exist in contener. If non exist then nothing is done*/
	template<typename F>
	void eraseElement(F comparator)
	{
		int i{ 0 };
		bool found{ false };
		for (; i < data.size(); ++i)
		{
			if (comparator(data[i]))
			{
				found = true;
				break;
			}
		}
		if (found == true)
		{
			//T* tempData = (T*)malloc(sizeof(T)*(size - 1));
			std::vector<T> tempData;
			/*for (int j = 0; j < i; ++i)
			{
				tempData.push_back(data[j]);
			}
			for (int j = i + 1; j < data.size(); ++j)
			{
				tempData.push_back(data[j]);
			}*/

			for (int j = 0; j < data.size(); ++j)
			{
				if (i != j)
				{
					tempData.push_back(data[j]);
				}
			}

			data.clear();
			for (int i = 0; i < tempData.size(); ++i)
			{
				data.push_back(tempData[i]);
			}

			//data = tempData;
			/*--size;
			if (size == 0)
			{
				if (data != nullptr)
				{
					free(data);
					data = nullptr;
				}
			}*/
		}
	}

	T& operator[](size_t i)
	{
		return data[i];
	}

	void pushBack(T element)
	{
		/*T* tempData = (T*)malloc(sizeof(T)*(size + 1));
		int i{ 0 };
		for (; i < size; ++i)
		{
			tempData[i] = data[i];
		}
		tempData[i] = element;
		++size;
		data = tempData;*/

		data.push_back(element);
	}

	size_t getSize()
	{
		return data.size();
		//return size;
	}

	void clear()
	{
		data.clear();
		/*if (data != nullptr)
		{
			free(data);
			data = nullptr;
		}*/
	}

	~MyVector()
	{
		/*if (data != nullptr)
		{
			free(data);
		}*/
	}
protected:
private:
	std::vector<T> data;
	//size_t size;
};
#endif // !MY_VECTOR
