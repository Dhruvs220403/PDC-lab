#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int N = 1000;

void initializeVectors(vector<int> &a , vector<int> &b) {
	random_device r;
	for(int i = 0;i<N;i++) {
		a[i] = r() % 100 + 1;
		b[i] = r() % 100 + 1;
	}
}

int dotProductSingle(vector<int> &a , vector<int> &b) {
	int ans = 0;
	for(int i = 0;i<N;i++) {
		ans += a[i] * b[i];
	}
	return ans;
}

int main() {
	vector<int> a(N) , b(N);
	int dpparallelAns = 0;
	
	initializeVectors(a , b);
	
	int dpSingle = dotProductSingle(a , b);
	
	#pragma omp parallel reduction(+:dpparallelAns)
	{
		int num_threads = omp_get_num_threads();
		int threadId = omp_get_thread_num();
		int chunkSize = N / num_threads;
		int start = chunkSize * threadId;
		int end = (threadId == num_threads - 1) ? N : start + chunkSize;
		
		int dpParallel = 0;
		
		#pragma omp parallel for reduction(+:dpParallel)
		for(int i = start;i<end;i++) {
			dpParallel += a[i] * b[i];
		}
		
			dpparallelAns += dpParallel;
		
	}
	
	cout<<"Single thread dot product -> "<<dpSingle<<endl;
	cout<<"Multi thread dot product -> "<<dpparallelAns<<endl;
}
