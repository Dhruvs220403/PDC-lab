#include<bits/stdc++.h>
using namespace std;

void fun() {
	cout<<"Inside fun function : "<<endl;
	#pragma omp parallel
	{
		cout<<"Fun function parallel region"<<endl;
	}
}

int main() {
	cout<<"Main function called : "<<endl;
	
	#pragma omp parallel
	{
		cout<<"parallel region : "<<endl;
		cout<<"Fun function called from inside the main parallel region"<<endl;
		fun();
	}
	fun();
}
