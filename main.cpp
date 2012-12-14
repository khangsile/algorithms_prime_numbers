/* 
 * File:   main.cpp
 * Author: KhangSiLe
 *
 * Created on September 24, 2012, 10:10 PM
 */

#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <sys/time.h>

using namespace std;

void subtract(vector<int> bin1, vector<int> bin2, vector<int>& difference);
void divide(vector<int>&, vector<int>&, vector<int>&, vector<int>&);
void multiply(vector<int>& num1, vector<int>& num2, vector<int>& product);
void modExp(vector<int> x, vector<int> y, vector<int> N, vector<int>& result);
void modAdd(vector<int> x, vector<int> y, vector<int> N, vector<int>& modSum);
void modMult(vector<int> x, vector<int> y, vector<int> N, vector<int>& modProd);
void fullAdd2(vector<int>, vector<int>, vector<int>&);
bool testPrimality(vector<int>&);

vector<int> bin2dec(vector<int>& v);
bool compare(vector<int> bin1, vector<int> bin2);
void genBinary(vector<int>& bin1, int n);
void twosComp (vector<int>& binary);
string lookAtArray(vector<int>& array);
void reduceZeros(vector<int>& binary);
/*
 * 
 */
int main(int argc, char** argv) {
    
    struct timeval start, end;
    
    srand (time(NULL));
    int n;
    cout << "Enter n-bits: ";
    while (cin >> n) {
        int counter = 0;
        
        gettimeofday(&start, NULL);
        vector<int> N, ndec;
        
        genBinary(N, n);
        while(!testPrimality(N)) {
            counter++;
            genBinary(N, n);
        }
        gettimeofday(&end, NULL);

        ndec = bin2dec(N);
        reduceZeros(ndec);
        
        cout << "N: " << lookAtArray(ndec) << endl;
        
        double tStart = static_cast<double>(start.tv_sec)*1000000+ (static_cast<double>(start.tv_usec));
        double tEnd = static_cast<double>(end.tv_sec)*1000000+ static_cast<double>((end.tv_usec));
        
        cout << "Elapsed Time: " << tEnd - tStart << "\n";
        cout << "Number of Tries: " << counter << endl;
                        
        cout << "Enter one: " ;
    }
    
    return 0;
}

bool testPrimality(vector<int>& num) {
    vector<int> three(2, 1); 
    vector<int> result, nMinus1;

    vector<int> one(num.size()-1, 0);
    one.push_back(1);
    
    subtract(num, one, nMinus1);
    
    modExp(three, nMinus1, num, result);
    
    reduceZeros(result);
    
    if(result.size()==1 and result[0]==1)
        return true;
    else
        return false;
    
}

void modAdd(vector<int> x, vector<int> y, vector<int> N, vector<int>& modSum) {
    fullAdd2(x, y, modSum);
    
    while(compare(modSum, N)) {
        subtract(modSum, N, modSum);
    }   
}

void modMult(vector<int> x, vector<int> y, vector<int> N, vector<int>& modProd) {
    vector<int> product, quotient;
    multiply(x, y, product);
    
    divide(product, N, quotient, modProd);
}

void modExp(vector<int> x, vector<int> y, vector<int> N, vector<int>& result) {
    result.push_back(1);
    
    for(int i = 0; i < y.size(); i++) {
        modMult(result, result, N, result);
        if (y[i] == 1) 
            modMult(x, result, N, result);
    }
}

void fullAdd2 (vector<int> bin1, vector<int> bin2, vector<int>& sum) {
    
    int larger, smaller;
    sum.clear();
    
    if (bin1.size() >= bin2.size()) {
        larger = bin1.size();
        smaller = bin2.size();
    } else {
        larger = bin2.size();
        smaller = bin1.size();
        vector<int> temp(bin2);
        bin2 = bin1;
        bin1 = temp;
    }
    
    int carryout = 0;
    int difference = larger-smaller;
    sum.resize(larger+1);
    
    for(int i = smaller-1; i >= 0; i--) {
        int added = bin1[i+difference] + bin2[i] + carryout;
        sum[i+difference+1] = added % 2;
        
        carryout = added / 2;
    }
    
    for(int i = difference-1; i >= 0; i--) {
        int added = bin1[i]+carryout;
        sum[i+1] = added % 2;
        
        carryout = added / 2;
    }
    
    sum[0] = carryout;
}

void twosComp (vector<int>& binary) {
    
    int firstOne = 0, index = binary.size()-1;
    while (firstOne == 0) {
        firstOne = binary[index];
        index--;
    }
    
    for(int i = index; i >= 0; i--) {
        int converter = binary[i]-1;
        binary[i] = abs(converter);
    }
}

void subtract(vector<int> bin1, vector<int> bin2, vector<int>& difference) {
    
    difference.clear();
    twosComp(bin2);
    fullAdd2(bin1, bin2, difference);
    
    int overflow = difference.size() - bin2.size();
    for (int i = 0; i < overflow; i++)
        difference.erase(difference.begin());
}

void multiply(vector<int>& num1, vector<int>& num2, vector<int>& product) {
    
    /*Multiplies two numbers. Traverses the vector, num2. If num2[i] is odd, adds num1 to the product. After each
     traversal of the for loop num1 is multiplied by 2 and num2 is divided by 2.*/
    //O(n^2)
    product.clear();
    
    for(int i = num2.size()-1; i >= 0; i--) {
        
        if (num2[i] == 1) {
            fullAdd2(product, num1, product);
        }
        reduceZeros(num1);
        num1.push_back(0);
    }
}

void divide(vector<int>& numer, vector<int>& denom, vector<int>& quotient, vector<int>& remainder) {
    vector<int> increment(1,1);
    remainder.clear();
    
    for(int i = 0; i < numer.size(); i++) {
        quotient.push_back(0);
        remainder.push_back(0);
        
        if (numer[i] == 1) {
            fullAdd2(remainder, increment, remainder);
        }
        
        bool greaterthan = compare(remainder, denom);
        if (greaterthan) {
            subtract(remainder, denom, remainder);
            fullAdd2(quotient, increment, quotient);
        }
           
    }
}

string lookAtArray(vector<int>& array) {
    string nthVal =  "";
    for (int i = 0; i < array.size(); i++) {
            stringstream sstream;
            sstream << array[i];
            nthVal += sstream.str();
    }
    return nthVal;
}

void genBinary(vector<int>& bin1, int n) {
       
    bin1.clear();

    bin1.push_back(1);
    //O(n)
    for (int i=1; i < n-1; i++) {
        bin1.push_back(rand() % 2);
    }
    bin1.push_back(1);
}


bool compare(vector<int> bin1, vector<int> bin2) {
    if (bin1.size() >= bin2.size()) 
    {
        int equalizer = bin1.size()-bin2.size();
        
        for (int i = 0; i < equalizer; i++) {
            bin2.push_back(0);
            for (int i = bin2.size()-1; i > 0; i--) {
                int temp = bin2[i];
                bin2[i] = bin2[i-1];
                bin2[i-1] = temp;
            }
        }
    }
    else
    {
        int equalizer = bin2.size()-bin1.size();
        
        for (int i = 0; i < equalizer; i++) {
            bin1.push_back(0);
            for (int i = bin1.size()-1; i > 0; i--) {
                int temp = bin1[i];
                bin1[i] = bin1[i-1];
                bin1[i-1] = temp;
            }
        }
    }
    
    for(int i = 0; i < bin1.size(); i++) {
        if (bin1[i] > bin2[i]) 
            return true; 
        if (bin1[i] < bin2[i])
            return false;         
    }
    return true;
}

void deciAdder(vector<int> num1, vector<int> num2, vector<int>& sum) {
    
    int largerSize;
    sum.clear();
    
    if (num1.size() >= num2.size()) 
    {
        largerSize = num1.size();
        int equalizer = num1.size()-num2.size();
        
        for (int i = 0; i < equalizer; i++) {
            num2.push_back(0);
            for (int i = num2.size()-1; i > 0; i--) {
                int temp = num2[i];
                num2[i] = num2[i-1];
                num2[i-1] = temp;
            }
        }
    }
    else
    {
        largerSize = num2.size();
        int equalizer = num2.size()-num1.size();
        
        for (int i = 0; i < equalizer; i++) {
            num1.push_back(0);
            for (int i = num1.size()-1; i > 0; i--) {
                int temp = num1[i];
                num1[i] = num1[i-1];
                num1[i-1] = temp;
            }
        }
    }
    
    /* Acts just like a full adder by using a carryout, which represents the carry over in addition, and added
     is the sum of the two digits of num1 and num2 and the carryout. */
    int carryOut = 0;
    sum.resize(largerSize+1);
    for(int i = largerSize-1; i >= 0; i--)
    {
        int added = num1[i]+num2[i]+carryOut;
        sum[i+1] = added % 10;
        
        carryOut = added / 10;
    }
        sum[0] = carryOut;
 
}

vector<int> twoExp(vector<int>& z) {
    vector<int> result;
    result.resize(z.size()+1);
    
    int carryout = 0;
    for (int i = z.size()-1; i >= 0; i--) {
        int product = z[i] * 2 + carryout;
        result[i+1] = product % 10;
        carryout = product / 10;
    }
    if (carryout > 0)
        result[0] = carryout;
    return result;
}

vector<int> bin2dec(vector<int>& v) {
    vector<int> decimal, powerOfTwo(1,1);
    
    for (int i = v.size()-1; i >= 0; i--) {
        if (v[i]==1) {
            deciAdder(decimal, powerOfTwo, decimal);
        }
        powerOfTwo = twoExp(powerOfTwo);
    }
    return decimal;
}

void reduceZeros(vector<int>& binary) {
    
    for (int i = 0; i < binary.size(); i++) {
        if (binary[i] >= 1) 
            return;
        binary.erase(binary.begin());
        i--;
    }
    
    if(binary.empty())
        binary.push_back(0);
    
}

