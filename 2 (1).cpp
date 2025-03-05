#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int main() {
	vector<pair<int , int>> randomNumber;
	
	int numThreads = 8;
	omp_set_num_threads(numThreads);
	#pragma omp parallel num_threads(numThreads) shared(randomNumber) 
	{
		int tid = omp_get_thread_num();
		#pragma omp barrier 
		{
			#pragma omp critical
			randomNumber.push_back({tid , rand() % int(1e3)});			
		}
	}
	
	
	int maxi = 0;
	for(auto i : randomNumber) {
		cout<<"Thread "<<i.first<<" generated "<<i.second<<endl;
		maxi = max(maxi , i.second);
	}
	cout<<"maximum generated number -> "<<maxi;
}
