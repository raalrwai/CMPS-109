// $Id: bigint.cpp,v 1.76 2016-06-14 16:34:24-07 - - $
// Connor Thurmond (cthurmon) Rami Alrwais (raalrwai)
#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "bigint.h"
#include "debug.h"
#include "relops.h"
//cp

bigint::bigint (long that): uvalue (that), is_negative (that < 0) {
   DEBUGF ('~', this << " -> " << uvalue)
}

bigint::bigint (const ubigint& uvalue, bool is_negative):
                uvalue(uvalue), is_negative(is_negative) {
}

bigint::bigint (const string& that) {
   is_negative = that.size() > 0 and that[0] == '_';
   uvalue = ubigint (that.substr (is_negative ? 1 : 0));
}

bigint bigint::operator+ () const {
   cout << "Unary +"; 
   return *this;
}

bigint bigint::operator- () const {
   return {uvalue, not is_negative};
}

bigint bigint::operator+ (const bigint& that) const {
    bigint result;
    if (that.is_negative == is_negative) {
        result = bigint(uvalue + that.uvalue, is_negative);
    } else {
        if (that.uvalue > uvalue) {
            result = bigint(that.uvalue - uvalue, that.is_negative);
        } else {
            if (uvalue == that.uvalue) {
                result = bigint(uvalue - that.uvalue, false);
            } else {
                result = bigint(uvalue - that.uvalue, that.is_negative);
            }
        }
    }
    return result;
}

bigint bigint::operator- (const bigint& that) const {
    bigint result;
    if (that.is_negative != is_negative) {
        if (uvalue < that.uvalue) { 
            result = bigint(that.uvalue + uvalue, that.is_negative);
        } else {
            result = bigint(that.uvalue + uvalue, is_negative);
        }
    } else {
        if (uvalue < that.uvalue) { 
            result = bigint(that.uvalue - uvalue, !that.is_negative);
        } else {
            if (uvalue == that.uvalue) {
                result = bigint(uvalue - that.uvalue, false);
            } else {
                result = bigint(uvalue - that.uvalue, that.is_negative);
            }
        }
    }
    return result;
}

bigint bigint::operator* (const bigint& that) const {
    bigint result = uvalue * that.uvalue;
    if (that.is_negative == is_negative) {
        result.is_negative = false;
    } else {
        result.is_negative = true;
    }
    return result;
}

bigint bigint::operator/ (const bigint& that) const {
   bigint result = uvalue / that.uvalue;
   return result;
}

bigint bigint::operator% (const bigint& that) const {
   bigint result = uvalue % that.uvalue;
   return result;
}

bool bigint::operator== (const bigint& that) const {
   return is_negative == that.is_negative and uvalue == that.uvalue;
}

bool bigint::operator< (const bigint& that) const {
   if (is_negative != that.is_negative) return is_negative;
   return is_negative ? uvalue > that.uvalue
                      : uvalue < that.uvalue;
}

ostream& operator<< (ostream& out, const bigint& that) {
    out << (that.is_negative ? "-" : "");
    out << that.uvalue;
    return out;
}

