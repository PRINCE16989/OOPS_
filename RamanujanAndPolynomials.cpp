/*
    Problem statement : https://docs.google.com/document/d/1NIfS8xc44mo-8NiqtA_ZBt7MP-sc5YH9ycfBrneDGRY/edit?usp=sharing
*/

#include <iostream>
#include <vector>
#include <stack>
#include <iomanip>
#include <cmath>
#define integer long long int
#define float long double 
using namespace std;
 
class complex
{
    private:
        long long int real, imag;
    public:
        complex(long long int r=0, long long int i=0) : real(r), imag(i) {}
 
        complex operator*(complex &other) {
            long long int r = this->real * other.real - this->imag * other.imag;
            long long int i = this->imag * other.real + this->real * other.imag;
 
            return complex(r,i);
        }

        complex operator+(complex &other) {
            long long int r = this->real + other.real;
            long long int i = this->imag + other.imag;

            return complex(r,i);
        }
        
        complex operator+=(complex & other) {
            this->real = this->real + other.real;
            this->imag = this->imag + other.imag;

            return *this;
        }

        bool operator==(complex &other) {
            return (real == other.real && imag == other.imag);
        }

        integer getReal() { return real; }
        integer getImag() { return imag; }
    friend istream& operator>>(istream& s, complex &b);
    friend ostream& operator<<(ostream& s, complex &b);
};
 
istream& operator>>(istream& s, complex &b) {
    s >> b.real >> b.imag;
    return s;
}
 
ostream& operator<<(ostream& s, complex &b) {
    s << b.real << " " << b.imag;
    return s;
}

template<typename T>
class Polynomial    // class for multiplication
{
    private:
        vector<T> coeff;
    public:
        Polynomial(int x) { coeff.resize(x); for(auto &x : coeff) { cin >> x; } }
        Polynomial(vector<T> a) { coeff = a; }
        
        vector<T> karatsuba(vector<T>& a, vector<T>& b) {
            int size = a.size();
            if (size == 1) return vector<T>({a[0] * b[0]});

            int mid = size/2;

            vector<T> a1(a.begin(), a.begin() + mid);
            vector<T> a2(a.begin() + mid, a.end());
            vector<T> b1(b.begin(), b.begin() + mid);
            vector<T> b2(b.begin() + mid, b.end());

            vector<T> x1 = karatsuba(a1, b1);
            vector<T> x2 = karatsuba(a2, b2);
            vector<T> x3 = karatsuba(a1, b2);
            vector<T> x4 = karatsuba(a2, b1);

            vector<T> result(2*size - 1, 0);

            for (int i = 0; i < x1.size(); i++) {
                result[i] += x1[i];
                result[i + mid] += x4[i];
            }
            for (int i = 0; i < x2.size(); i++) {
                result[i + size] += x2[i];
                result[i + mid] += x3[i];
            }

            return result;
        }
 
        Polynomial<T> multiply(Polynomial<T> b) {
            int n = 1;
            //cout << (n << 1 << 1)  << endl;
            while(n < coeff.size() || n < b.coeff.size()) { n *= 2; }
            coeff.resize(n,0);  b.coeff.resize(n,0);
            
            return  Polynomial<T>(karatsuba(this->coeff,b.coeff));
        }
 
        Polynomial<T> operator*(Polynomial<T> &other_poly) { 
            //cout << "inside operator* " << endl;
            return multiply(other_poly);
        }
 
 
        void print_poly(int x) { 
            for(int i=0; i<x; ++i) cout << fixed << setprecision(6) << coeff[i] << " ";
            cout << endl;
        }
};
 
string operator*(string &a, integer x) {
    string ans;
    for(int i=0; i<x; ++i) {
        ans += a;
    }
    return ans;
} 

template<typename T>
class Poly_eval         // class for evaluation
{
    private:
        vector<T> data;
        T result;
    public:
        Poly_eval(int deg) { data.resize(deg); for(auto &x : data) cin >> x; }
 
        void Eval(int x) {
            result = data[0];
            for(int i=1; i<data.size(); ++i) {
                integer p = pow(x,i);
                result = data[i]*p + result;
            }
            cout << fixed << setprecision(6) << result << endl;
        }
        
        void print_result() { cout << fixed << setprecision(6) << result <<endl; }
};
 
template<typename T>
class Poly_diff         // class for differentiation
{
    private:
        vector<T> data;
    public:
        Poly_diff(int x) { 
            data.resize(x); 
            for(auto &y : data) cin >> y; 
        }
 
        void diff() {
            vector<T> result;
            for(int i=0; i<data.size()-1; ++i) {
                result.push_back(data[i+1]*(i+1));
            }
            
            for(auto x : result) cout << fixed << setprecision(6) << x << " ";
            cout << endl;
        }
};
 
int main()
{
    int q,op;
    cin >> q;
 
    while(q--) {
        cin >> op;
        if(op == 1) {
            string type;
            cin >> type;
 
            int deg1, deg2;
            if(type == "integer") {
                cin >> deg1;
                Polynomial<integer> P1(deg1);
                cin >> deg2;
                Polynomial<integer> P2(deg2);
 
                Polynomial<integer> P = P1*P2;
                P.print_poly(deg1+deg2-1);
            }
            else if(type == "float") {
                cin >> deg1;
                Polynomial<float> P1(deg1);
                cin >> deg2;
                Polynomial<float> P2(deg2);
 
                P1 = P1*P2;
                P1.print_poly(deg1+deg2-1);
            }
            else if(type == "complex") {
                cin >> deg1;
                Polynomial<complex> P1(deg1);
                cin >> deg2;
                Polynomial<complex> P2(deg2);
 
                P1 = P1*P2;
                P1.print_poly(deg1+deg2-1);
            }
        }
        else if(op == 2) {
            string type;
            cin >> type;
 
            int deg;
            cin >> deg;
 
            if(type == "integer") {
                Poly_eval<integer> P(deg);
                int x;  cin >> x;
                P.Eval(x);
            }
            else if(type == "float") {
                Poly_eval<float> P(deg);
                int x;  cin >> x;
                P.Eval(x);
            }
            else if(type == "string") {
                Poly_eval<string> P(deg);
                int x;  cin >> x;
                P.Eval(x);
            }
        }
        else if(op == 3) {
            string type;
            cin >> type;
            int deg;
            cin >> deg;
 
            if(type == "integer") {
                Poly_diff<integer> P(deg);
                P.diff();
            }
            else if(type == "float") {
                Poly_diff<float> P(deg);
                P.diff(); 
            }
        }
    }
}