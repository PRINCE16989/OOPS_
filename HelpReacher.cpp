/*
    Problem Statement : https://docs.google.com/document/d/1U8EdP1NxRpfOLBsSo0HER5fb6XX8PX3HmLhscaMfUHs/edit?usp=sharing
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
using namespace std;

class CurrencySorter
{
    public:
        void operator()(vector<int> &arr) {
            int n = arr.size();
            quick_sort(arr, 0, n-1);
        }

        void quick_sort(vector<int> &arr, int low, int high) {
            if(low >= high) return;
            int pivot = partition(arr, low, high);

            quick_sort(arr, low, pivot-1);
            quick_sort(arr, pivot+1, high);
        }

        int partition(vector<int> &arr, int low, int high) {
            int pivot = arr[high];
            int i = low-1;
            for(int j=low; j<high; ++j) {
                if(arr[j] < pivot) {
                    ++i;
                    swap(arr[i],arr[j]);
                }
            }
            swap(arr[i+1],arr[high]);
            return i+1;
        }
};

class FibonacciGenerator
{
    private:
        long long mod = 1000000007;

        void fib(vector<long long> &mat, long long n) {
            if(n == 1) return;
            
            long long a = mat[0]; // a = f[k+1];
            long long b = mat[1]; // b = f[k];
            // f[2k] = f[k](2*f[k+1] - f[k])
            long long c =  ( b*((2*a - b + mod) % mod)) % mod;
            // f[2k+1] = f[k]^2 + f[k+1]^2;
            long long d = ((a*a) % mod + (b*b) % mod) % mod;

            mat[0] = d;
            mat[1] = c;
            fib(mat,n/2);
        
            if(n % 2 != 0)  {
                long long x = ((a*mat[1]) % mod + (b*(mat[0] - mat[1] + mod)) % mod) % mod;
                long long y = ((mat[0]*a) % mod + (b*mat[1]) % mod) % mod;
                mat[0] = y;
                mat[1] = x;
            }
        }
    public:
        FibonacciGenerator() {}
        long long operator()(long long i) {
            vector<long long> mat(2);
            mat[0] = 1;
            mat[1] = 1;
            fib(mat,i);
            return mat[1];
        }
};

class PrimeCalculator
{
    private:
        vector<long long> prime_numbers;
    public:
        PrimeCalculator() {
            vector<bool> prime(1000001,true);
            
            prime[0] = false;
            prime[1] = false;
            for(long long i=2; i<=1000; ++i) {
                if(prime[i]) {
                    long long j = i*i;
                    while(j <= 1000000) {
                        prime[j] = false;
                        j += i;
                    }
                }
            }
            for(long long i =2; i<1000001; ++i) {
                if(prime[i]) prime_numbers.push_back(i);
            }
        }
        void printPrimes(long long L, long long R) {
            vector<long long> primes = primes_between_LR(L,R);
            
            for(long long i=0; i<primes.size(); ++i) {
                cout << primes[i] << " ";
            }
            cout << endl;
        }

        void printPrimeSum(long long L, long long R) {
            long long sum = 0;
            vector<long long> primes = primes_between_LR(L,R);
            
            for(auto x : primes) sum += x;
            cout << sum << endl;
        }

        vector<long long> primes_between_LR(long long L,long long R) {
            vector<long long> primes;
            vector<bool> isPrime(R-L+1,true);

            if(L == 1) isPrime[0] = false;
            
            for(auto x : prime_numbers) {
                if(x*x > R) break;
                long long j = max(x*x,(L+x-1)/x *x);
                for(; j <= R; j += x) {
                    isPrime[j-L] = false;
                }
            }

            for(long long i=0; i< isPrime.size(); ++i) {
                if(isPrime[i]) primes.push_back(i+L);
            }
            return primes;
        }
};

class NumberAnalyzer
{
    private:
        vector<pair<int,int>> factorize;
        vector<long long> prime_numbers;

        void factorize_number(int X) {
            factorize.resize(0);
            for(auto x : prime_numbers) {
                if(x*x > X) break;
                long long power = 0;
                while(X % x == 0) { 
                    ++power;
                    X = X/x;
                }
                if(power != 0) factorize.push_back({x,power});
            }
            if(X == 1) return;
            factorize.push_back({X,1});
        }

    public:
        NumberAnalyzer() {
            vector<bool> prime(10001,true);
            
            prime[0] = false;
            prime[1] = false;
            for(long long i=2; i<=100; ++i) {
                if(prime[i]) {
                    long long j = i*i;
                    while(j <= 10000) {
                        prime[j] = false;
                        j += i;
                    }
                }
            }

            prime_numbers.resize(0);
            for(long long i =2; i<10001; ++i) {
                if(prime[i]) prime_numbers.push_back(i);
            }
        }
        
        bool isSquareFree(int X) {
            factorize_number(X);
            for(auto x : factorize) {
                if(x.second > 1) return false; 
            }
            return true;
        }

        int countDivisors(int X) {
            factorize_number(X);
            int ans = 1;
            for(auto x : factorize) {
                ans *= (x.second + 1);
            }
            return ans;
        }

        long long sumOfDivisors(int X) {
            factorize_number(X);
            long long sum = 1;
            for(auto x : factorize) {
                sum *= ((pow(x.first,x.second+1)-1)/(x.first-1));
            }
            return sum;
        }
};

int main()
{
    int type;
    cin >> type;
    if(type == 1) {
        int n;
        cin >> n;

        vector<vector<int>> input(n);
        
        for(int i=0; i<n; ++i) {
            int size;
            cin >> size;
            input[i].resize(size);
            for(int j=0; j<size; ++j) { 
                int p; cin >> p;
                input[i][j] = p;
            }
        }

        for_each(input.begin(),input.end(),CurrencySorter());
        for(auto v : input) {
            for(auto x : v) {
                cout << x << " ";
            }
            cout << endl;
        }
    }

    else if(type == 2) {
        long long t;
        cin >> t;
        
        vector<long long> arr(t);
        for(long long i=0; i<t; ++i) cin >> arr[i];
       
        transform(arr.begin(),arr.end(),arr.begin(),FibonacciGenerator());
        
        for(long long i=0; i<t; ++i) cout << arr[i] << endl;
    }

    else if(type == 3) {
        long long t,L,R;
        cin >> t;

        string s;
        PrimeCalculator P;
        for(long long i=0; i<t; ++i) {
            cin >> s >> L >> R;
            if(s == "printPrimes") {
                P.printPrimes(L,R);
            }
            else if(s == "printPrimeSum") {
                P.printPrimeSum(L,R);
            }
        }
    }

    else if(type == 4) {
        int t,X;
        cin >> t;

        string s;
        NumberAnalyzer A;
        for(int i=0; i<t; ++i) {
            cin >> s >> X;
            if(s == "isSquareFree") {
                cout << (A.isSquareFree(X) ? "yes" : "no") << endl;
            }
            else if (s == "countDivisors") {
                cout << A.countDivisors(X) << endl;
            }
            else if(s == "sumOfDivisors") {
                cout << A.sumOfDivisors(X) << endl;
            }
        }
    }
}