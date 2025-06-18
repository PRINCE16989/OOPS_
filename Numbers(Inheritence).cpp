/*
    Problem Statement : https://drive.google.com/file/d/1yA4EOHbJJfFHx1mJuZJaI_8lBzUPUBtM/view?usp=sharing
*/

#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#define P 1000000007
using namespace std;

class complex
{
    private:
        double real, imag;
    public:
        complex(double r = 0.0,double i = 0.0) : real(r), imag(i) {}
        virtual ~complex() {}

        virtual void operator+(complex& other) {
            this->real += other.real;
            this->imag += other.imag;
        }

        virtual void operator-(complex& other) {
            this->real -= other.real;
            this->imag -= other.imag;
        }

        virtual void operator*(complex& other) {
            double r = this->real*other.real - this->imag*other.imag;
            double i = this->real*other.imag + this->imag*other.real;
            this->real = r;  this->imag = i;
        }

        virtual void input(istream& stream) {
            stream >> this->real >> this->imag;
        }
        
        virtual void print(ostream& stream) {
            stream << fixed << setprecision(3) << this->real << " " << this->imag << endl;
        }

        virtual void reduce(){}
        virtual bool isprime(){ return false; }
        virtual void inverse(){}

        friend istream& operator>>(istream& s,complex&);
        friend ostream& operator<<(ostream&, complex&);
};

istream& operator>>(istream& s, complex& num) {
    num.input(s);
    return s;
}

ostream& operator<<(ostream& s, complex& num) {
    num.print(s);
    return s;
}

class rational : public complex
{
    private:
        double n, d;
    public:
        rational(double nume = 0, double deno = 1) : n(nume), d(deno) {}
        virtual ~rational() {}

        void operator+(complex& num) {
            rational& r = dynamic_cast<rational&>(num);
            this->n = (double)n/d + (double)r.n/r.d;
            this->d = 1;
        }

        void operator-(complex& num) {
            rational& r = dynamic_cast<rational&>(num);
            this->n = (double)n/d - (double)r.n/r.d;
            this->d = 1;
        }

        void operator*(complex& num) {
            rational& r = dynamic_cast<rational&>(num);
            this->n = (double)n/d * (double)r.n/r.d;;
            this->d = 1;
        }

        void reduce() {
            int x, y;
            if(abs(n) > abs(d)) { y = abs(n); x = abs(d); }
            else { x = abs(n); y = abs(d); }
            while( x > 0) {
                int temp = x;
                x = y % x;
                y = temp;
            }

            if(n*d>0)
            cout << (int)abs(n)/y << " " << (int)abs(d)/y << endl;
            else
            cout << -1*(int)(abs(n)/y) << " " << (int)(abs(d)/y) << endl;
        }

        void input(istream& stream) {
            stream >> n >> d;
        }

        void print(ostream& stream) {
            stream << fixed << setprecision(3) << n << endl;
        }
};

class natural : public rational 
{
    private:
        int n;
    public:
        natural(int x = 0) : n(x) {} 
        virtual ~natural() {}

        bool isprime() {
            if (n < 2) return false;
            if (n == 2 || n == 3) return true;
            if (n % 2 == 0 || n % 3 == 0) return false;
            
            for (int i = 5; i <= std::sqrt(n); i += 2) {
                if (n % i == 0) return false;
            }
            return true;
        }

        void inverse() {
            long long result = 1;
            long long base = n;
            long long exp = P-2;
            while (exp > 0) {
                if (exp % 2 == 1)  // If exp is odd, multiply base with result
                    result = (result * base) % P;
                base = (base * base) % P;  // Square the base
                exp /= 2;
            }
            cout << result << endl;
        }
        
        void operator+(complex& num) {
            natural& N = dynamic_cast<natural&>(num);
            n += N.n;
        }

        void operator-(complex& num) {
            natural& N = dynamic_cast<natural&>(num);
            n -= N.n;
        }

        void operator*(complex& num) {
            natural& N = dynamic_cast<natural&>(num);
            n *= N.n;
        }

        void input(istream& stream) {
            stream >> n;
        }

        void print(ostream& stream) {
            stream << n << endl;
        }
};

int main() {
    
    complex *num1, *num2;
    int n;
    cin >> n;
   
    string type, oper;
    while(n--) {
        
        cin >> type >> oper;
        
        if  (type == "complex")     { num1 = new complex();  num2 = new complex();  }
        else if(type == "rational") { num1 = new rational(); num2 = new rational(); }
        else if(type == "natural")  { num1 = new natural();  num2 = new natural();  }
        
        cin >> *num1;
        
        if(oper == "reduce") { num1->reduce(); }
        else if(oper == "isprime") { 
            if(num1->isprime()) cout << 1 << endl;
            else cout << 0 << endl;
        }
        else if(oper == "inverse") {
            num1->inverse();
        }
        else {
            cin >> *num2;
            if(oper == "add") { *num1 + *num2; }
            else if(oper == "sub") { *num1 - *num2; }
            else if(oper == "mult") { *num1 * *num2; }
            
            cout << *num1;
        }
    
        if(num1) delete num1;
        if(num2) delete num2;
    }   
    
}
