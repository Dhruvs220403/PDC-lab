#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

void singleThread(vector<int> &a , vector<int> &b) {
	double ans = 0;
	int num_threads = 8;
	
	omp_set_num_threads(num_threads);
	double start_time, end_time;

    start_time = omp_get_wtime();
	
	for(int i = 0;i<1000000;i++) {
		ans += a[i] * (double)(b[i]);
	}
	
	end_time = omp_get_wtime();
	
	cout<<"Dot Poduct single thread -> "<<ans<<" time taken -> "<<end_time - start_time<<endl;
}

void multiThread(vector<int> &a , vector<int> &b) {
	double ans = 0;
	int num_threads = 8;
	
	omp_set_num_threads(num_threads);
	double start_time, end_time;

    start_time = omp_get_wtime();
	
	#pragma omp parallel for reduction(+:ans)
	for(int i = 0;i<1000000;i++) {
		ans += a[i] * (double)(b[i]);
	}
	
	end_time = omp_get_wtime();
	
	cout<<"Dot Poduct multi thread -> "<<ans<<" time taken -> "<<end_time - start_time<<endl;
}

int main() {
	vector<int> a(1000000) , b(1000000);
	
	for(int i = 0;i<1000000;i++) {
		a[i] = 1 + (rand() % 100);
		b[i] = 1 + (rand() % 100);
	}
	singleThread(a , b);
	multiThread(a , b);
}
