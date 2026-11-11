/**
 * Arbitrary-Precision Integer Arithmetic
 *
 * This file implements a small BigInt class using base-10 digits stored in
 * little-endian order. It supports:
 *   - construction from integers and decimal strings
 *   - comparison, addition, subtraction, multiplication
 *   - long division and remainder
 *   - unary negation, factorial, Fibonacci, and fast exponentiation
 *
 * Base 10 keeps the implementation readable and makes toString trivial. The
 * algorithms are the schoolbook O(n^2) versions, which are fast enough for
 * demonstrations and are the foundation for Karatsuba and FFT multiplication.
 *
 * Division by zero throws std::domain_error.
 */

#include <algorithm>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
using namespace std;

class BigInteger {
   private:
    bool negative;
    vector<int> digits;  // little-endian base-10 digits

    void trim() {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
        if (digits.empty()) {
            digits.push_back(0);
        }
        if (isZero()) {
            negative = false;
        }
    }

    static int compareAbsolute(const BigInteger &a, const BigInteger &b) {
        if (a.digits.size() != b.digits.size()) {
            return a.digits.size() < b.digits.size() ? -1 : 1;
        }

        for (int i = static_cast<int>(a.digits.size()) - 1; i >= 0; i--) {
            if (a.digits[i] != b.digits[i]) {
                return a.digits[i] < b.digits[i] ? -1 : 1;
            }
        }

        return 0;
    }

    static BigInteger addAbsolute(const BigInteger &a, const BigInteger &b) {
        BigInteger result;
        result.digits.clear();

        int carry = 0;
        int size = max(a.digits.size(), b.digits.size());

        for (int i = 0; i < size; i++) {
            int sum = carry;
            if (i < static_cast<int>(a.digits.size())) {
                sum += a.digits[i];
            }
            if (i < static_cast<int>(b.digits.size())) {
                sum += b.digits[i];
            }

            result.digits.push_back(sum % 10);
            carry = sum / 10;
        }

        if (carry > 0) {
            result.digits.push_back(carry);
        }

        result.trim();
        return result;
    }

    // Requires |a| >= |b|.
    static BigInteger subtractAbsolute(const BigInteger &a,
                                       const BigInteger &b) {
        BigInteger result;
        result.digits.clear();

        int borrow = 0;

        for (int i = 0; i < static_cast<int>(a.digits.size()); i++) {
            int difference = a.digits[i] - borrow;
            if (i < static_cast<int>(b.digits.size())) {
                difference -= b.digits[i];
            }

            if (difference < 0) {
                difference += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }

            result.digits.push_back(difference);
        }

        result.trim();
        return result;
    }

    static BigInteger multiplyAbsolute(const BigInteger &a,
                                       const BigInteger &b) {
        BigInteger result;
        result.digits.assign(a.digits.size() + b.digits.size(), 0);

        for (int i = 0; i < static_cast<int>(a.digits.size()); i++) {
            int carry = 0;

            for (int j = 0; j < static_cast<int>(b.digits.size()); j++) {
                int current =
                    result.digits[i + j] + a.digits[i] * b.digits[j] + carry;
                result.digits[i + j] = current % 10;
                carry = current / 10;
            }

            int position = i + static_cast<int>(b.digits.size());

            while (carry > 0) {
                int current = result.digits[position] + carry;
                result.digits[position] = current % 10;
                carry = current / 10;
                position++;
            }
        }

        result.trim();
        return result;
    }

    static pair<BigInteger, BigInteger> divideAbsolute(const BigInteger &a,
                                                       const BigInteger &b) {
        if (b.isZero()) {
            throw domain_error("BigInteger division by zero");
        }

        BigInteger quotient;
        quotient.digits.assign(a.digits.size(), 0);

        BigInteger remainder;

        for (int i = static_cast<int>(a.digits.size()) - 1; i >= 0; i--) {
            remainder = multiplyAbsolute(remainder, BigInteger(10));
            remainder = addAbsolute(remainder, BigInteger(a.digits[i]));

            int digit = 0;
            while (compareAbsolute(remainder, b) >= 0) {
                remainder = subtractAbsolute(remainder, b);
                digit++;
            }

            quotient.digits[i] = digit;
        }

        quotient.trim();
        remainder.trim();

        return {quotient, remainder};
    }

   public:
    BigInteger() : negative(false), digits{0} {}

    BigInteger(long long value) : negative(false) {
        unsigned long long magnitude;

        if (value < 0) {
            magnitude = static_cast<unsigned long long>(-(value + 1)) + 1;
            negative = true;
        } else {
            magnitude = static_cast<unsigned long long>(value);
        }

        if (magnitude == 0) {
            digits.push_back(0);
            return;
        }

        while (magnitude > 0) {
            digits.push_back(static_cast<int>(magnitude % 10));
            magnitude /= 10;
        }
    }

    explicit BigInteger(const string &value) : negative(false) {
        if (value.empty()) {
            throw invalid_argument("empty decimal string");
        }
        int start = 0;

        if (!value.empty() && (value[0] == '-' || value[0] == '+')) {
            negative = (value[0] == '-');
            start = 1;
        }

        if (start == static_cast<int>(value.size())) {
            throw invalid_argument(
                "decimal string needs digits after its sign");
        }
        for (int i = start; i < static_cast<int>(value.size()); i++) {
            if (value[i] < '0' || value[i] > '9') {
                throw invalid_argument("invalid decimal digit");
            }
        }

        while (start < static_cast<int>(value.size()) && value[start] == '0') {
            start++;
        }

        if (start == static_cast<int>(value.size())) {
            digits.push_back(0);
            negative = false;
            return;
        }

        for (int i = static_cast<int>(value.size()) - 1; i >= start; i--) {
            digits.push_back(value[i] - '0');
        }

        trim();
    }

    bool isZero() const {
        return digits.size() == 1 && digits[0] == 0;
    }

    string toString() const {
        if (isZero()) {
            return "0";
        }

        string result;

        if (negative) {
            result.push_back('-');
        }

        for (int i = static_cast<int>(digits.size()) - 1; i >= 0; i--) {
            result.push_back(static_cast<char>('0' + digits[i]));
        }

        return result;
    }

    BigInteger operator-() const {
        BigInteger result = *this;
        if (!result.isZero()) {
            result.negative = !result.negative;
        }
        return result;
    }

    BigInteger operator+(const BigInteger &other) const {
        BigInteger result;

        if (negative == other.negative) {
            result = addAbsolute(*this, other);
            result.negative = negative;
        } else {
            int comparison = compareAbsolute(*this, other);

            if (comparison == 0) {
                return BigInteger(0);
            }

            if (comparison > 0) {
                result = subtractAbsolute(*this, other);
                result.negative = negative;
            } else {
                result = subtractAbsolute(other, *this);
                result.negative = other.negative;
            }
        }

        result.trim();
        return result;
    }

    BigInteger operator-(const BigInteger &other) const {
        return *this + (-other);
    }

    BigInteger operator*(const BigInteger &other) const {
        if (isZero() || other.isZero()) {
            return BigInteger(0);
        }

        BigInteger result = multiplyAbsolute(*this, other);
        result.negative = (negative != other.negative);
        result.trim();
        return result;
    }

    BigInteger operator/(const BigInteger &other) const {
        pair<BigInteger, BigInteger> result = divideAbsolute(*this, other);

        if (!result.first.isZero()) {
            result.first.negative = (negative != other.negative);
        }

        return result.first;
    }

    BigInteger operator%(const BigInteger &other) const {
        pair<BigInteger, BigInteger> result = divideAbsolute(*this, other);

        if (!result.second.isZero()) {
            result.second.negative = negative;
        }

        return result.second;
    }

    bool operator==(const BigInteger &other) const {
        return negative == other.negative && digits == other.digits;
    }

    bool operator!=(const BigInteger &other) const {
        return !(*this == other);
    }

    bool operator<(const BigInteger &other) const {
        if (negative != other.negative) {
            return negative;
        }

        int comparison = compareAbsolute(*this, other);
        return negative ? comparison > 0 : comparison < 0;
    }

    bool operator>(const BigInteger &other) const {
        return other < *this;
    }

    bool operator<=(const BigInteger &other) const {
        return !(other < *this);
    }

    bool operator>=(const BigInteger &other) const {
        return !(*this < other);
    }
};

BigInteger power(BigInteger base, int exponent) {
    if (exponent < 0) {
        throw invalid_argument("negative exponent");
    }

    BigInteger result(1);

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = result * base;
        }
        base = base * base;
        exponent /= 2;
    }

    return result;
}

BigInteger factorial(int n) {
    if (n < 0) {
        throw invalid_argument("negative factorial argument");
    }
    BigInteger result(1);

    for (int value = 2; value <= n; value++) {
        result = result * BigInteger(value);
    }

    return result;
}

BigInteger fibonacci(int n) {
    if (n < 0) {
        throw invalid_argument("negative Fibonacci index");
    }
    BigInteger a(0);
    BigInteger b(1);

    for (int i = 0; i < n; i++) {
        BigInteger next = a + b;
        a = b;
        b = next;
    }

    return a;
}

void check(const string &label, const string &actual, const string &expected) {
    cout << label << ": " << actual << "  "
         << (actual == expected ? "ok" : "MISMATCH") << endl;

    if (actual != expected) {
        cout << "  expected: " << expected << endl;
    }
}

int main() {
    BigInteger a("12345678901234567890");
    BigInteger b("98765432109876543210");

    check("a + b", (a + b).toString(), "111111111011111111100");
    check("b - a", (b - a).toString(), "86419753208641975320");
    check("a * b", (a * b).toString(),
          "1219326311370217952237463801111263526900");
    check("b / a", (b / a).toString(), "8");
    check("b % a", (b % a).toString(), "900000000090");

    check("negative addition",
          (BigInteger("-50") + BigInteger("20")).toString(), "-30");
    check("negative multiplication",
          (BigInteger("-50") * BigInteger("20")).toString(), "-1000");
    check("negative division", (BigInteger("-50") / BigInteger("7")).toString(),
          "-7");
    check("negative remainder",
          (BigInteger("-50") % BigInteger("7")).toString(), "-1");
    check("subtract to zero", (a - a).toString(), "0");
    check("unary minus", (-BigInteger("42")).toString(), "-42");
    check("leading zeros", BigInteger("00000123").toString(), "123");
    check("string zero", BigInteger("-0000").toString(), "0");

    check("factorial(30)", factorial(30).toString(),
          "265252859812191058636308480000000");
    check("factorial(50)", factorial(50).toString(),
          "30414093201713378043612608166064768844377641568960512000000000000");
    check("fibonacci(100)", fibonacci(100).toString(), "354224848179261915075");
    check("2^100", power(BigInteger(2), 100).toString(),
          "1267650600228229401496703205376");

    cout << "comparisons: " << (BigInteger("10") < BigInteger("20")) << " "
         << (BigInteger("-10") < BigInteger("0")) << " "
         << (BigInteger("10") == BigInteger("010")) << endl;

    try {
        BigInteger unused = a / BigInteger(0);
        (void)unused;
        cout << "division by zero: not thrown" << endl;
    } catch (const domain_error &error) {
        cout << "division by zero: " << error.what() << endl;
    }

    return 0;
}

// Expected output:
// a + b: 111111111011111111100  ok
// b - a: 86419753208641975320  ok
// a * b: 1219326311370217952237463801111263526900  ok
// b / a: 8  ok
// b % a: 900000000090  ok
// negative addition: -30  ok
// negative multiplication: -1000  ok
// negative division: -7  ok
// negative remainder: -1  ok
// subtract to zero: 0  ok
// unary minus: -42  ok
// leading zeros: 123  ok
// string zero: 0  ok
// factorial(30): 265252859812191058636308480000000  ok
// factorial(50):
// 30414093201713378043612608166064768844377641568960512000000000000  ok
// fibonacci(100): 354224848179261915075  ok
// 2^100: 1267650600228229401496703205376  ok
// comparisons: 1 1 1
// division by zero: BigInteger division by zero
//
// Special cases: zero, negative values, leading zeros, subtraction to zero,
// negative division and remainder (truncation toward zero), large factorials,
// Fibonacci numbers, and exponentiation by squaring.
