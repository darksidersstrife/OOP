#ifndef _StaticMultiSet_h_
#define _StaticMultiSet_h_


#include <set>
#include <deque>
#include <fstream>

class EmptySetException : public std::exception
{
	EmptySetException() : exception("Set is empty") {};
};

template<class T>
class StatisticMultiset
{
public:
	StatisticMultiset(unsigned int cacheSize = 5);
	void SetCacheSize(unsigned int cacheSize);
	void AddNum(const T& number);
	template<class Container>
	void AddNums(const Container& numbers);
	void AddNumsFromFile(const char* filename);
	T GetMax() const;
	T GetMin() const;
	float GetAvg() const;
	unsigned int GetCountUnder(float value) const;
	unsigned int GetCountAbove(float value) const;
private:
	typename std::multiset<T>::iterator begin() const
	{
		return Data.begin();
	}
	typename std::multiset<T>::iterator end() const
	{
		return Data.end();
	}

	std::multiset<T> Data;

	unsigned int CacheSize;

	mutable bool MaxCached;
	mutable T MaxCachedValue;

	mutable bool MinCached;
	mutable T MinCachedValue;

	mutable float AvgCachedValue;
	mutable bool AvgCached;

	mutable bool CountUnderCached;
	mutable std::deque<std::pair<float, unsigned int>> CountUnderCachedValue;

	mutable bool CountAboveCached;
	mutable std::deque<std::pair<float, unsigned int>> CountAboveCachedValue;
};


template<class T>
StatisticMultiset<T>::StatisticMultiset(unsigned int cacheSize = 5)
{
	CacheSize = cacheSize;
}

template<class T>
void StatisticMultiset<T>::SetCacheSize(unsigned int cacheSize)
{
	CacheSize = cacheSize;
	if (CacheSize < CountUnderCachedValue.size())
	{
		CountUnderCachedValue.resize(CacheSize);
	}
}

template<class T>
void StatisticMultiset<T>::AddNum(const T& number)
{
	Data.insert(number);
	MaxCached = false;
	MinCached = false;
	AvgCached = false;
	CountUnderCached = false;
	CountAboveCached = false;
}

template<class T>
template<class Container>
void StatisticMultiset<T>::AddNums(const Container& numbers)
{
	for (auto it : numbers)
	{
		Data.insert(it);
	}
}

template<class T>
void StatisticMultiset<T>::AddNumsFromFile(const char* fileName)
{
	ifstream file(fileName);
	while (file.good())
	{
		T x;
		file >> x;
		AddNum(x);
	}
}

template<class T>
T StatisticMultiset<T>::GetMax() const
{
	if (Data.size() == 0)
		throw EmptySetException();

	if (!MaxCached)
	{
		MaxCachedValue = *(Data.begin());
		MaxCached = true;
	}
	return MaxCachedValue;
}

template<class T>
T StatisticMultiset<T>::GetMin() const
{
	if (Data.size() == 0)
		throw EmptySetException();

	if (!MinCached)
	{

		MinCachedValue = *(Data.rbegin());
		MinCached = true;

	}
	return MinCachedValue;
}


template<class T>
float StatisticMultiset<T>::GetAvg() const
{
	if (Data.size() == 0)
		throw EmptySetException();

	if (!AvgCached)
	{
		AvgCachedValue = 0;
		for (auto it : Data)
		{
			AvgCachedValue += it;
		}
		MinCachedValue = AvgCachedValue / Data.size();
		MinCached = true;
	}
	return MinCachedValue;
}

template<class T>
unsigned int StatisticMultiset<T>::GetCountUnder(float value) const
{
	if (Data.size() == 0)
		throw EmptySetException();

	unsigned int ans;
	if (!CountUnderCached)
	{
		CountUnderCachedValue.clear();
		ans = std::distance(Data.begin(), Data.lower_bound(value));
		CountUnderCachedValue.push_front(std::make_pair(value, ans));
		CountUnderCached = true;
		return ans;
	}

	for (auto it = CountUnderCachedValue.begin(); it != CountUnderCachedValue.end(); it++)
	{
		if (it->first == value)
		{
			ans = it->second;
			CountUnderCachedValue.erase(it);
			CountUnderCachedValue.push_front(std::make_pair(value, ans));
			return ans;
		}
	}

	ans = std::distance(Data.begin(), Data.lower_bound(value));
	if (CountUnderCachedValue.size() >= CacheSize)
	{
		CountUnderCachedValue.pop_back();
	}
	CountUnderCachedValue.push_front(std::make_pair(value, ans));
	return ans;
}

template<class T>
unsigned int StatisticMultiset<T>::GetCountAbove(float value) const
{
	if (Data.size() == 0)
		throw EmptySetException();

	unsigned int ans;
	if (!CountAboveCached)
	{
		CountAboveCachedValue.clear();
		ans = std::distance(Data.upper_bound(value), Data.end());
		CountAboveCachedValue.push_front(std::make_pair(value, ans));
		CountAboveCached = true;
		return ans;
	}

	for (auto it = CountAboveCachedValue.begin(); it != CountAboveCachedValue.end(); it++)
	{
		if (it->first == value)
		{
			ans = it->second;
			CountAboveCachedValue.erase(it);
			CountAboveCachedValue.push_front(std::make_pair(value, ans));
			return ans;
		}
	}
	ans = std::distance(Data.begin(), Data.upper_bound(value));
	if (CountAboveCachedValue.size() >= CacheSize)
	{
		CountAboveCachedValue.pop_back();
	}
	CountAboveCachedValue.push_front(std::make_pair(value, ans));
	return ans;
}
#endif