#include <iostream>
#include <string>
#include <algorithm>
#include <cstdint>
#include <vector>
#include <stdexcept> // Needed for throw runtime_error to handle errors if present
using namespace std;

class BigInt
{
    string number;   // Stores the number as a string
    bool isNegative; // True if number is negative

    // Remove unnecessary leading zeros from the number string
    void removeLeadingZeros() {
        // TODO: Implement this function  (Done)
        while (number.length() > 1 && number[0] == '0')
        {
            number.erase(0, 1);
        }
        if (number == "0" || number.empty()) // added number.empty to make it to cover the case of empty input
        {
            number = "0";
            isNegative = false;
        }

    }

    // Compare absolute values of two BigInts (ignore signs)
    // Returns: 1 if |this| > |other|, 0 if equal, -1 if |this| < |other|
    int compareMagnitude(const BigInt &other) const
    {
        if (this->number.length() > other.number.length())
        {
            return 1;
        }

        if (this->number.length() < other.number.length())
        {
            return -1;
        }

        if (this->number > other.number)
        {
            return 1;
        }

        if (this->number < other.number)
        {
            return -1;
        }

        return 0;
    }

public:
    // Default constructor - initialize to zero
    BigInt() {
        // TODO: Implement this constructor (Done)
        number = "0";
        isNegative = false;
    }

    // Constructor from 64-bit integer
    BigInt(int64_t value)
    {
        // TODO: Implement this constructor (Done)
        if (value < 0)
        {
            isNegative = true;
            number = to_string(-value);

        }
        else
        {
            isNegative = false;
            number = to_string(value);
        }
        
    }

    // Constructor from string representation
    BigInt(const string &str)
    {
        // TODO: Implement this constructor
        if (str[0] == '-')
        {
            isNegative = true; //forgot to change sign
            number = str.substr(1);
        }
        else
        {
            isNegative = false;
            number = str;
        }
        removeLeadingZeros();
    }

    // Copy constructor
    BigInt(const BigInt &other)
    {
        // TODO: Implement this constructor
        number = other.number;
        isNegative = other.isNegative;
    }

    // Destructor
    ~BigInt() {
        // TODO: Implement if needed (Done)
    }

    // Assignment operator
    BigInt& operator=(const BigInt &other)
    {
        // TODO: Implement this operator
        if (this == &other) return *this;
        number = other.number;
        isNegative = other.isNegative;
        return *this;
    }

    // Unary negation operator (-x)
    BigInt operator-() const
    {
        BigInt result = *this;
        if (result.number != "0")
        {
            result.isNegative = !result.isNegative;
        }
        return result;
    }

    // Unary plus operator (+x)
    BigInt operator+() const
    {
        BigInt result = *this;
        return result;
    }

    // Addition assignment operator (x += y)
    BigInt& operator+=(const BigInt& other) {

        // Same signs -> add magnitudes
        if (isNegative == other.isNegative) {

            int i = number.size() - 1;
            int j = other.number.size() - 1;
            int carry = 0;

            string result;

            while (i >= 0 || j >= 0 || carry) {

                int sum = carry;

                if (i >= 0)
                    sum += number[i--] - '0';

                if (j >= 0)
                    sum += other.number[j--] - '0';

                result.push_back(char('0' + (sum % 10)));

                carry = sum / 10;
            }

            reverse(result.begin(), result.end());

            number = result;
        }

        // Different signs -> subtract magnitudes
        else {

            int cmp = compareMagnitude(other);

            // Equal magnitudes -> result is zero
            if (cmp == 0) {

                number = "0";
                isNegative = false;
                
                return *this;
            }

            // |this| > |other|
            if (cmp > 0) {

                int i = number.size() - 1;
                int j = other.number.size() - 1;
                int borrow = 0;

                string result;

                while (i >= 0) {

                    int digit = (number[i] - '0') - borrow;

                    if (j >= 0)
                        digit -= other.number[j--] - '0';

                    if (digit < 0) {
                        digit += 10;
                        borrow = 1;
                    }
                    else {
                        borrow = 0;
                    }

                    result.push_back(char('0' + digit));

                    i--;
                }

                reverse(result.begin(), result.end());

                number = result;
            }

            // |other| > |this|
            else {

                int i = other.number.size() - 1;
                int j = number.size() - 1;
                int borrow = 0;

                string result;

                while (i >= 0) {

                    int digit = (other.number[i] - '0') - borrow;

                    if (j >= 0)
                        digit -= number[j--] - '0';

                    if (digit < 0) {
                        digit += 10;
                        borrow = 1;
                    }
                    else {
                        borrow = 0;
                    }

                    result.push_back(char('0' + digit));

                    i--;
                }

                reverse(result.begin(), result.end());

                number = result;

                // Result takes the sign of the larger magnitude
                isNegative = other.isNegative;
            }
        }
        removeLeadingZeros();
        return *this;
    }

    // Subtraction assignment operator (x -= y)
    BigInt& operator-=(const BigInt& other) {

        // x - y = x + (-y)

        BigInt temp = other;

        if (temp.number != "0")
            temp.isNegative = !temp.isNegative;

        *this += temp;

        return *this;
    }

    // Multiplication assignment operator (x *= y)
    BigInt &operator*=(const BigInt &other)
    {
        if (this->number == "0" || other.number == "0")
        {
            this->number = "0";
            this->isNegative = false; // removed "" from false because it's a bool and not a string
            return *this;
        }

        this->isNegative = (this->isNegative != other.isNegative);
        int n1 = this->number.length();
        int n2 = other.number.length();

        vector<int> resultArray(n1 + n2, 0);

        for (int i = n1 - 1; i >= 0; i--)
        {
            for (int j = n2 - 1; j >= 0; j--)
            {
                int digit1 = this->number[i] - '0';
                int digit2 = other.number[j] - '0';
                int sum = (digit1 * digit2) + resultArray[i + j + 1];

                resultArray[i + j + 1] = sum % 10;
                resultArray[i + j] += sum / 10;
            }
        }

        string finalNumber = "";
        for (int i = 0; i < n1 + n2; i++)
        {
            if (!(finalNumber.empty() && resultArray[i] == 0))
            {
                finalNumber += to_string(resultArray[i]);
            }
        }

        this->number = finalNumber.empty() ? "0" : finalNumber;
        this->removeLeadingZeros();

        return *this;
    }

    // Division assignment operator (x /= y)
    BigInt &operator/=(const BigInt &other)
    {
        if (other.number == "0") {
            throw runtime_error("Division by zero");
        }

        // If |this| < |other|, the result is 0
        int cmp = this->compareMagnitude(other);
        if (cmp < 0) {
            this->number = "0";
            this->isNegative = false;
            return *this;
        }

        // XOR sign rule
        bool resultNegative = (this->isNegative != other.isNegative);

        // Digit-by-digit long division
        string quotient = "";
        BigInt current;
        current.number = "";
        current.isNegative = false;

        BigInt absOther = other;
        absOther.isNegative = false; // Only use positive magnitude for division loop

        for (size_t i = 0; i < this->number.length(); i++) {
            current.number += this->number[i];
            current.removeLeadingZeros();

            int count = 0;
            // Subtract absOther from current as many times as it fits
            while (current.compareMagnitude(absOther) >= 0) {
                current -= absOther;
                count++;
            }
            quotient += to_string(count);
        }

        this->number = quotient;
        this->isNegative = resultNegative;
        this->removeLeadingZeros();

        return *this;
    }

    // Modulus assignment operator (x %= y)
    BigInt &operator%=(const BigInt &other)
    {
        if (other.number == "0") {
            throw runtime_error("Division by zero");
        }

        // Section 5.5 formula: remainder = this - (this / other) * other
        BigInt quotient = *this / other;
        BigInt product = quotient * other;
        
        *this -= product;

        // Protect against negative zero
        if (this->number == "0") {
            this->isNegative = false;
        }

        return *this;
    }

    // Pre-increment operator (++x)
    BigInt &operator++()
    {
        // TODO: Implement this operator
        return *this;
    }

    // Post-increment operator (x++)
    BigInt operator++(int)
    {
        BigInt temp;
        // TODO: Implement this operator
        return temp;
    }

    // Pre-decrement operator (--x)
    BigInt &operator--()
    {
        // TODO: Implement this operator
        return *this;
    }

    // Post-decrement operator (x--)
    BigInt operator--(int)
    {
        BigInt temp;
        // TODO: Implement this operator
        return temp;
    }

    // Convert BigInt to string representation
    string toString() const
    {
        // TODO: Implement this function (Done)
        if (isNegative && number != "0")
        {
            return "-" + number;
        }
        return number;
    }

    // Output stream operator (for printing)
    friend ostream &operator<<(ostream &os, const BigInt &num)
    {
        // TODO: Implement this operator
        os << num.toString();
        return os;
    }

    // Input stream operator (for reading from input)
    friend istream &operator>>(istream &is, BigInt &num)
    {
        // TODO: Implement this operator
        string text;
        is >> text;
        num = BigInt(text);
        return is;
    }

    // Friend declarations for comparison operators
    friend bool operator==(const BigInt &lhs, const BigInt &rhs);
    friend bool operator<(const BigInt &lhs, const BigInt &rhs);
};

// Binary addition operator (x + y)
BigInt operator+(BigInt lhs, const BigInt& rhs) {

    lhs += rhs;

    return lhs;
}

// Binary subtraction operator (x - y)
BigInt operator-(BigInt lhs, const BigInt& rhs) {

    lhs -= rhs;

    return lhs;
}

// Binary multiplication operator (x * y)
BigInt operator*(BigInt lhs, const BigInt &rhs)
{
    BigInt result;

    result = lhs;
    result *= rhs;

    return result;
}

// Binary division operator (x / y)
BigInt operator/(BigInt lhs, const BigInt &rhs)
{
    lhs /= rhs;
    return lhs;
}

// Binary modulus operator (x % y)
BigInt operator%(BigInt lhs, const BigInt &rhs)
{
    lhs %= rhs;
    return lhs;
}

// Equality comparison operator (x == y)
bool operator==(const BigInt &lhs, const BigInt &rhs)
{
    // TODO: Implement this operator
    return false;
}

// Inequality comparison operator (x != y)
bool operator!=(const BigInt &lhs, const BigInt &rhs)
{
    // TODO: Implement this operator
    return false;
}

// Less-than comparison operator (x < y)
bool operator<(const BigInt &lhs, const BigInt &rhs)
{
    // TODO: Implement this operator
    return false;
}

// Less-than-or-equal comparison operator (x <= y)
bool operator<=(const BigInt &lhs, const BigInt &rhs)
{
    // TODO: Implement this operator
    return false;
}

// Greater-than comparison operator (x > y)
bool operator>(const BigInt &lhs, const BigInt &rhs)
{
    // TODO: Implement this operator
    return false;
}

// Greater-than-or-equal comparison operator (x >= y)
bool operator>=(const BigInt &lhs, const BigInt &rhs)
{
    // TODO: Implement this operator
    return false;
}

int main()
{
    cout << "=== BigInt Class Test Program ===" << endl
         << endl;
    cout << "NOTE: All functions are currently empty." << endl;
    cout << "Your task is to implement ALL the functions above." << endl;
    cout << "The tests below will work once you implement them correctly." << endl
         << endl;

    /*
    // Test 1: Constructors and basic output
    cout << "1. Constructors and output:" << endl;
    BigInt a(12345);              // Should create BigInt from integer
    BigInt b("-67890");           // Should create BigInt from string
    BigInt c("0");                // Should handle zero correctly
    BigInt d = a;                 // Should use copy constructor
    cout << "a (from int): " << a << endl;        // Should print "12345"
    cout << "b (from string): " << b << endl;     // Should print "-67890"
    cout << "c (zero): " << c << endl;            // Should print "0"
    cout << "d (copy of a): " << d << endl << endl; // Should print "12345"

    // Test 2: Arithmetic operations
    cout << "2. Arithmetic operations:" << endl;
    cout << "a + b = " << a + b << endl;          // Should calculate 12345 + (-67890)
    cout << "a - b = " << a - b << endl;          // Should calculate 12345 - (-67890)
    cout << "a * b = " << a * b << endl;          // Should calculate 12345 * (-67890)
    cout << "b / a = " << b / a << endl;          // Should calculate (-67890) / 12345
    cout << "a % 100 = " << a % BigInt(100) << endl << endl; // Should calculate 12345 % 100

    // Test 3: Relational operators
    cout << "3. Relational operators:" << endl;
    cout << "a == d: " << (a == d) << endl;       // Should be true (12345 == 12345)
    cout << "a != b: " << (a != b) << endl;       // Should be true (12345 != -67890)
    cout << "a < b: " << (a < b) << endl;         // Should be false (12345 < -67890)
    cout << "a > b: " << (a > b) << endl;         // Should be true (12345 > -67890)
    cout << "c == 0: " << (c == BigInt(0)) << endl << endl; // Should be true (0 == 0)

    // Test 4: Unary operators and increments
    cout << "4. Unary operators and increments:" << endl;
    cout << "-a: " << -a << endl;                 // Should print "-12345"
    cout << "++a: " << ++a << endl;               // Should increment and print "12346"
    cout << "a--: " << a-- << endl;               // Should print "12346" then decrement
    cout << "a after decrement: " << a << endl << endl; // Should print "12345"

    // Test 5: Large number operations
    cout << "5. Large number operations:" << endl;
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");
    cout << "Very large addition: " << num1 + num2 << endl;
    cout << "Very large multiplication: " << num1 * num2 << endl << endl;

    // Test 6: Edge cases and error handling
    cout << "6. Edge cases:" << endl;
    BigInt zero(0);
    BigInt one(1);
    try {
        BigInt result = one / zero;               // Should throw division by zero error
        cout << "Division by zero succeeded (unexpected)" << endl;
    } catch (const runtime_error& e) {
        cout << "Division by zero correctly threw error: " << e.what() << endl;
    }
    cout << "Multiplication by zero: " << one * zero << endl;        // Should be "0"
    cout << "Negative multiplication: " << BigInt(-5) * BigInt(3) << endl;  // Should be "-15"
    cout << "Negative division: " << BigInt(-10) / BigInt(3) << endl;       // Should be "-3"
    cout << "Negative modulus: " << BigInt(-10) % BigInt(3) << endl;        // Should be "-1"
    */

    return 0;
}
