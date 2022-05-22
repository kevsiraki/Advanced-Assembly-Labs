#include <iostream>
using namespace std;

int fib(int n)
{
	int temp=0;
	if (n <= 0)
		return 0;
	else if (n < 2)
		return 1;
	else {
		temp = fib(n-1);
		return temp+ fib(n-2);
	}
}

int main()
{
   int num;
   cout << "Which fibonacci number? ";
   cin >> num;
   cout << fib(num) << endl;
   return 0;
}
