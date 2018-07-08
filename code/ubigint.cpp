// $Id: ubigint.cpp,v 1.14 2016-06-23 17:21:26-07 - - $
//#define DEBUG

#include <cctype>
#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
#include <sstream>
#include <cmath>
using namespace std;

#include "ubigint.h"
#include "debug.h"

ubigint::ubigint (unsigned long that)  {

  stringstream ss;
  string uvalue;

  ss << that;
  ss >> uvalue;
  for(auto it = uvalue.rbegin(); it != uvalue.rend(); it++){
    ubig_value.push_back(*it);
    DEBUGF('-', *it);
  }
}

ubigint::ubigint (const string& that)  {
  for (auto it = that.rbegin(); it != that.rend(); it++){
    ubig_value.push_back(*it);
    DEBUGF('~', *it);
  }
}

ubigint ubigint::operator+ (const ubigint& that) const {
    ubigint result;
    int iter_size = ubig_value.size();
    unsigned char digit1 = '0';
    unsigned char digit2 = '0';
    unsigned char carry = '0';
    if (ubig_value.size() < that.ubig_value.size()) {
        iter_size = that.ubig_value.size();
    } 
 
    //unsigned char partial_result;
    int int_partial;
    int corrected_result;
    for (int i = 0; i < iter_size; i++) {
        if (i > static_cast<int>(that.ubig_value.size() - 1)) {
            digit1 = '0';
        } else {
            digit1 = that.ubig_value[i];
        }
        if (i > static_cast<int>(ubig_value.size() - 1)) {
            digit2 = '0';
        } else {
            digit2 = ubig_value[i];
        }
        int_partial = (static_cast<int>(digit1) - 48) + (static_cast<int>(digit2) - 48) + (static_cast<int>(carry) - 48);
        //cout << "(Digit1: " << digit1 << ") + (Digit2: "  << digit2 << ") + (Carry: " << carry << ") = " << int_partial << endl; 
        //cout << "Int partial: " << int_partial << endl;
        if (int_partial > 9) {
            carry = '1';
            corrected_result = int_partial % 10;
            //cout << "Corrected Result: " << corrected_result << endl;
            result.ubig_value.push_back(corrected_result + '0');
        } else {
            result.ubig_value.push_back(int_partial + '0');
            carry = '0';
        }
        if (i == (iter_size - 1) and (carry == '1')) {
            result.ubig_value.push_back(carry);
        } 
    }
    while (result.ubig_value.size() > 1 and ((static_cast<int>(result.ubig_value.back()) - 48) == 0)) {
        result.ubig_value.pop_back();
    } 
    return result;
}

ubigint ubigint::operator- (const ubigint& that) const {
    ubigint result;
    int iter_size = ubig_value.size();
    unsigned char digit1 = '0';
    unsigned char digit2 = '0';
    unsigned char borrow = '0';
    if (ubig_value.size() < that.ubig_value.size()) {
        iter_size = that.ubig_value.size();
    } 

    //unsigned char partial_result;
    int int_partial;
    int corrected_result;
    //int negative_one = -1;
    for (int i = 0; i < iter_size; i++) {
        if (i > static_cast<int>(ubig_value.size() - 1)) {
            digit1 = '0';
        } else {
            digit1 = ubig_value[i];
        }
        if (i > static_cast<int>(that.ubig_value.size() - 1)) {
            digit2 = '0';
        } else {
            digit2 = that.ubig_value[i];
        }
        //cout << "Digit1: " << digit1 << endl;
        //cout << "Digit2: " << digit2 << endl;
        int_partial = (static_cast<int>(digit1) - 48) - (static_cast<int>(digit2) - 48) - (static_cast<int>(borrow) - 48);
        //cout << digit1 << " - " << digit2 << " - " << borrow << " = " << int_partial << endl;
        if (digit1 < digit2) {
            //int_partial = (static_cast<int>(digit1) - 38) - (static_cast<int>(digit2) - 48) - (static_cast<int>(borrow) - 48);
            //borrow = '1';
            //cout << "Borrow: " << borrow << endl;
            //cout << "int partial " << int_partial << endl;
            corrected_result = int_partial + 10;
            //cout << "Corrected Result: " << corrected_result << endl;
            //cout << "Pushing " << corrected_result << endl;
            result.ubig_value.push_back(corrected_result + '0');
        } else {
            //int_partial = (static_cast<int>(digit1) - 48) - (static_cast<int>(digit2) - 48) - (static_cast<int>(borrow) - 48);
            //cout << "Pushing " << int_partial << endl;
            result.ubig_value.push_back(int_partial + '0');
            borrow = '0';
        }

        //cout << "Digit 1/Left: " << (char) digit1 << "Digit 2/Right: " << char (digit2) << endl;
        
        //partial_result = (digit1-'0') - (digit2-'0');
        //cout << (char) (digit1) << " - " << (char) (digit2) << " = " 
        //<< (char) (partial_result + '0') << endl;
        //result.ubig_value.push_back(int_partial + '0');   
    }
    while (result.ubig_value.size() > 1 and ((static_cast<int>(result.ubig_value.back()) - 48) == 0)) {
        result.ubig_value.pop_back();
    } 
    return result;
}

ubigint ubigint::operator* (const ubigint& that) const {
  return ubigint (ubig_value[0] * that.ubig_value[0]);
}

void ubigint::multiply_by_2() {
  ubig_value[0] *= 2;
}

void ubigint::divide_by_2() {
  ubig_value[0] /= 2;
}


struct quo_rem { ubigint quotient; ubigint remainder; };
quo_rem udivide (const ubigint& dividend, ubigint divisor) {
  // Note: divisor is modified so pass by value (copy).
  ubigint zero {0};
  if (divisor == zero) throw domain_error ("udivide by zero");
  ubigint power_of_2 {1};
  ubigint quotient {0};
  ubigint remainder {dividend}; // left operand, dividend
  while (divisor < remainder) {
    divisor.multiply_by_2();
    power_of_2.multiply_by_2();
  }
  while (power_of_2 > zero) {
    if (divisor <= remainder) {
      remainder = remainder - divisor;
      quotient = quotient + power_of_2;
    }
    divisor.divide_by_2();
    power_of_2.divide_by_2();
  }
  return {.quotient = quotient, .remainder = remainder};
}

ubigint ubigint::operator/ (const ubigint& that) const {
  return udivide (*this, that).quotient;
}

ubigint ubigint::operator% (const ubigint& that) const {
  return udivide (*this, that).remainder;
}

bool ubigint::operator== (const ubigint& that) const {
    //return ubig_value[0] == that.ubig_value[0];
    if (ubig_value.size() != that.ubig_value.size()) {
        return false;
    } else {
        for (int i = 0; i < static_cast<int>(ubig_value.size() - 1); i++) {
            if (ubig_value[i] != that.ubig_value[i]) {
                return false;
            }
        }
    }
    return true;
}

bool ubigint::operator< (const ubigint& that) const {
  //return ubig_value[0] < that.ubig_value[0];
    if (ubig_value.size() > that.ubig_value.size()) {
        return false;
    } else if (ubig_value.size() == that.ubig_value.size()){
        for (int i = 0; i < static_cast<int>(ubig_value.size() - 1); i++) {
            if (ubig_value[i] > that.ubig_value[i]) {
                //cout << "First non matching numbers " << ubig_value[i] << that.ubig_value[i] << endl; 
                return false;
            }
        }
    }
    return true;
}

ostream& operator<< (ostream& out, const ubigint& that) { 
  // if(that.ubig_value.size() > 0){
  //   for(int i = 0; i <= that.ubig_value.size()-1; i++){
  //     if (i%69 == 0 and i != 0){
  //       out << "\\" << endl;
  //     }
  //     out << (static_cast<char>(that.ubig_value.at(i)));
  //   }
  // }
    if(that.ubig_value.size() > 0){
        for(int i = that.ubig_value.size()-1; i >= 0; i--){
            if ((that.ubig_value.size() - i)%70 == 0 and i != 0){ // was 69
                out << "\\" << endl;
            }
            out << (static_cast<char>(that.ubig_value.at(i)));
        }
    }
    return out; 
}

