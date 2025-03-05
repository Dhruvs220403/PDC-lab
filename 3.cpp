#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int main() {
	vector<int> a(8 , 0);
	
	#pragma omp parallel 
	{
		int id = omp_get_thread_num();
		for(int i = 0;i<10;i++) {
			a[id] += id;
		}
	}
	
	for(int i = 0;i<8;i++) {
		cout<<"Thread "<<i<<" -> "<<a[i]<<endl;
	}
}
