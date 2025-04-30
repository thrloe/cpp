#include <iostream>
#include <algorithm> 
#include <vector>
#include <utility>

using namespace std;

class Operator {
public:
virtual double evala () = 0;

};



class SumOP :public Operator {

    private:

    Operator* left;
    Operator* right;

    public:

    SumOP (Operator* l, Operator* r){
        left = l;
        right = r;
    }
    double evala() override{
        return left->evala() + right->evala();
    }
};
class SubOP :public Operator {

    private:

    Operator* left;
    Operator* right;

    public:

    SubOP (Operator* l, Operator* r){
        left = l;
        right = r;
    }
    double evala() override{
        return left->evala() - right->evala();
    }
};
class MulOP :public Operator {

    private:

    Operator* left;
    Operator* right;

    public:

    MulOP (Operator* l, Operator* r){
        left = l;
        right = r;
    }
    double evala() override{
        return left->evala() * right->evala();
    }
};
class DevOP :public Operator {

    private:

    Operator* left;
    Operator* right;

    public:

    DevOP (Operator* l, Operator* r){
        left = l;
        right = r;
    }
    double evala() override{
        return left->evala() / right->evala();
    }
};
class NumOP :public Operator {

    private:

    double num;

    public:

    NumOP (double n){
        num = n;
    }
    double evala() override{
        return num;
    }
};


int main(){

NumOP* a = new NumOP(4);
NumOP* b = new NumOP(7);
NumOP* c = new NumOP(31);
NumOP* d = new NumOP(5);
NumOP* e = new NumOP(75);
NumOP* f = new NumOP(3);

SumOP* sum = new SumOP(a, b);
SubOP* sub = new SubOP(c, sum);
MulOP* mul = new MulOP(d, sub);
DevOP* dev1 = new DevOP(e,f);
DevOP* dev2 = new DevOP(mul,dev1);
double result = dev2->evala();

NumOP* j = new NumOP(4);
NumOP* i = new NumOP(7);

SumOP* sum1 = new SumOP(a, b);

double result1 = sum1->evala();


cout << result;
}
