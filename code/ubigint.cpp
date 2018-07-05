// $Id: ubigint.cpp,v 1.14 2016-06-23 17:21:26-07 - - $

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
    int digit1 = 0;
    int digit2 = 0;
    int small_size = that.ubig_value.size();
    //int carry = 0;
    if (ubig_value.size() < that.ubig_value.size()) {
        iter_size = that.ubig_value.size();
    //     small_size = ubig_value.size();
    //     for (int i = 0; i < iter_size - small_size; i++) {
    //         ubig_value.push_back(0);
    //     }
    } //else {
    //     iter_size = ubig_value.size();
    //     small_size = that.ubig_value.size();
    //     for (int i = 0; i < iter_size - small_size; i++) {
    //         that.ubig_value.push_back("0");
    //     }
    // }
    int partial_result = 0;
    for (int i = 0; i < iter_size; i++) {
        if (i > that.ubig_value.size()) {
            digit1 = 0;
        } else {
            digit1 = static_cast<int>(that.ubig_value[i]) - 48;
        }
        if (i > ubig_value.size()) {
            digit2 = 0;
        } else {
            digit2 = static_cast<int>(ubig_value[i]) - 48;
        }
        partial_result = digit1 + digit2;
        cout << digit1 << " + " << digit2 << " = " << partial_result << endl;
    }

    return ubigint (ubig_value[0] + that.ubig_value[0]);
}

ubigint ubigint::operator- (const ubigint& that) const {
    cout << "Called ubigint operater-";
    if (*this < that) throw domain_error ("ubigint::operator-(a<b)");
    return ubigint (ubig_value[0] - that.ubig_value[0]);
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
  return ubig_value[0] == that.ubig_value[0];
}

bool ubigint::operator< (const ubigint& that) const {
  return ubig_value[0] < that.ubig_value[0];
}

ostream& operator<< (ostream& out, const ubigint& that) { 
  return out << "ubigint(" << that.ubig_value[0] << ")";
}

