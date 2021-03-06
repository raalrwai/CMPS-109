// $Id: ubigint.cpp,v 1.14 2016-06-23 17:21:26-07 - - $
//#define DEBUG
// Connor Thurmond (cthurmon) Rami Alrwais (raalrwai)

#include <cctype>
#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <algorithm>
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
    int_partial = (static_cast<int>(digit1) - 48) + 
      (static_cast<int>(digit2) - 48) 
      + (static_cast<int>(carry) - 48);
    if (int_partial > 9) {
      carry = '1';
      corrected_result = int_partial % 10;
      result.ubig_value.push_back(corrected_result + '0');
    } else {
      result.ubig_value.push_back(int_partial + '0');
      carry = '0';
    }
    if (i == (iter_size - 1) and (carry == '1')) {
      result.ubig_value.push_back(carry);
    } 
  }
  while (result.ubig_value.size() > 1 and 
      ((static_cast<int>(result.ubig_value.back()) - 48) == 0)) {
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

  int int_partial;
  int corrected_result;
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
    int_partial = (static_cast<int>(digit1) - 48) - 
      (static_cast<int>(digit2) - 48) - 
      (static_cast<int>(borrow) - 48);
    if (digit1 < digit2) {
      corrected_result = int_partial + 10;
      borrow = '1';
      result.ubig_value.push_back(corrected_result + '0');
    } else {
      result.ubig_value.push_back(int_partial + '0');
       borrow = '0'; 
    }
  }
  while (result.ubig_value.size() > 1 and 
      ((static_cast<int>(result.ubig_value.back()) - 48) == 0)) {
    result.ubig_value.pop_back();
  } 
  return result;
}

ubigint ubigint::operator* (const ubigint& that) const {
  int size = ubig_value.size();
  int thatSize = that.ubig_value.size();
  int partial_product;
  ubigint product;
  for(int i = 0; i < size + thatSize; i++){
    product.ubig_value.push_back(static_cast<udigit_t>(0)+ '0');
  }

  udigit_t c_carry;
  int carry;

    for(int i = 0; i < size; i++){
        carry = 0;
        c_carry = '0';

        for(int j = 0; j < thatSize; j++){
            partial_product = 
              static_cast<int>(product.ubig_value.at(i + j) - 48) 
              + static_cast<int>(ubig_value.at(i) - 48) *
              static_cast<int>(that.ubig_value.at(j) - 48) +
              static_cast<int>(carry); 
            product.ubig_value[i + j] = partial_product%10 + '0';
            carry = floor(partial_product/10);
            c_carry = carry + '0';
        }
        product.ubig_value[i + thatSize] = c_carry;
    }
    while (product.ubig_value.size() > 1 and 
        ((static_cast<int>(product.ubig_value.back()) - 48) == 0)) {
            product.ubig_value.pop_back();             
    }  
    return product;
}


 

void ubigint::multiply_by_2() {
    int size = ubig_value.size();
    int partial_product;
    ubigint product;
    for(int i = 0; i < size + 1; i++){
        product.ubig_value.push_back(static_cast<udigit_t>(0)+ '0');
    }
    udigit_t c_carry;
    int carry;
    for (int i = 0; i < size; i++) {
        carry = 0;
        c_carry = '0';
        partial_product = 
          static_cast<int>(product.ubig_value.at(i + 1) - 48) 
          + static_cast<int>(ubig_value.at(i) - 48) *
          2 + static_cast<int>(carry); 
        product.ubig_value[i] = partial_product%10 + '0';
        carry = floor(partial_product/10);
        c_carry = carry + '0';
        product.ubig_value[i + 1] = c_carry;    
    }
    product.ubig_value[size + 1] = c_carry; 
    while (product.ubig_value.size() > 1 and 
        ((static_cast<int>(product.ubig_value.back()) - 48) == 0)) {
            product.ubig_value.pop_back();             
    }  
    ubig_value = product.ubig_value;
}

void ubigint::divide_by_2() {
    //ubig_value[0] /= 2;
    ubigint divide;
    udigit_t digit;
    udigit_t temp;
    int int_digit;
    int int_temp;

    for (int i = 0; i < static_cast<int>(ubig_value.size()); i++) {
        digit = ubig_value[i];
        int_digit = static_cast<int>(digit - 48);
        if ((int_digit % 2) != 0) {
            temp = ubig_value[i - 1];
            int_temp = static_cast<int>(temp - 48);
            int_temp += 5;
            if (i >= 1) {
                ubig_value.at(i - 1) = int_temp + '0';
            } 
        }
        int_digit = int_digit / 2;
        ubig_value.at(i) = int_digit + '0';
    }

    while (ubig_value.size() > 1 and 
        ((static_cast<int>(ubig_value.back()) - 48) == 0)) {
            ubig_value.pop_back();             
    }  
    
}


struct quo_rem { ubigint quotient; ubigint remainder; };
quo_rem udivide (const ubigint& dividend, ubigint divisor) {
  //Note: divisor is modified so pass by value (copy).
    ubigint zero {0};
    if (divisor == zero) throw domain_error ("udivide by zero");
    ubigint power_of_2 {1};
    ubigint quotient {0};
    ubigint remainder {dividend}; // left operand, dividend
    while (divisor < remainder) {
        divisor.multiply_by_2();
        power_of_2.multiply_by_2();
    }
    // while (power_of_2 > zero) {
    //     if (divisor <= remainder) {
    //         remainder = remainder - divisor;
    //         quotient = quotient + power_of_2;
    //     }
    //     divisor.divide_by_2();
    //     power_of_2.divide_by_2();
    // }
    return {.quotient = quotient, .remainder = remainder};
}

ubigint ubigint::operator/ (const ubigint& that) const {
  return udivide (*this, that).quotient;
}

ubigint ubigint::operator% (const ubigint& that) const {
  return udivide (*this, that).remainder;
}

bool ubigint::operator== (const ubigint& that) const {
  if (ubig_value.size() != that.ubig_value.size()) {
    return false;
  } else {
    for (int i = 0; 
        i < static_cast<int>(ubig_value.size() - 1); 
        i++) {
      if (ubig_value[i] != that.ubig_value[i]) {
        return false;
      }
    }
  }
  return true;
}

bool ubigint::operator< (const ubigint& that) const {
    if (ubig_value.size() > that.ubig_value.size()) {
        return false;
    } else if (ubig_value.size() == that.ubig_value.size()){
            for (int i = static_cast<int>(ubig_value.size() - 1); 
                i > 0; i--) {
                    if (ubig_value[i] > that.ubig_value[i]) { 
                        return false;
                    }
                }
    }        
    return true;
}

ostream& operator<< (ostream& out, const ubigint& that) {
    if(that.ubig_value.size() > 0){
        int end_pos = that.ubig_value.size() - 1;
        for(int i = end_pos; i >= 0; i--){
            if (((end_pos - i)%69 == 0) and (i != end_pos)){
                out << "\\" << endl;
            } 
            out << that.ubig_value.at(i);
        }
    }
    return out; 
}

