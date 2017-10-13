#include "StaticMultiSet.h"
#include <iostream>
#include <vector> 
using namespace std;

int main()
{
	StatisticMultiset<float> StSet;
	for(float i = 0;i < 1000;i++)
	{
		StSet.AddNum(i);
	}
	cout << StSet.GetCountAbove(500) << endl;
	cout << StSet.GetCountUnder(1) << endl;
}