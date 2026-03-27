// Assignment: Homework 2 - Binary Representation, Floating-point Math and Bit Manipulation
// Author: Alexander Matos
// Date: March 27, 2026

#include <iostream>
#include <bitset>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <limits>

class FloatingPointAnalyzer {
private:
    float value;
    std::bitset<32> bits;
    
public:
    FloatingPointAnalyzer(float val);
    void displayIEEEFormat() const;
    std::bitset<32> getBits() const;
    float getValue() const;
    
    bool getSign() const;
    int getExponent() const;
    int getMantissa() const;
};

FloatingPointAnalyzer::FloatingPointAnalyzer(float val) : value(val) {
    union {
        float f;
        uint32_t i;
    } converter;
    converter.f = val;
    bits = std::bitset<32>(converter.i);
}

void FloatingPointAnalyzer::displayIEEEFormat() const {
    // Sign bit
    std::cout << bits[31] << " ";
    
    // Exponent bits
    for (int i = 30; i >= 23; i--) {
        std::cout << bits[i];
    }
    std::cout << " ";
    
    // Mantissa bits
    for (int i = 22; i >= 0; i--) {
        std::cout << bits[i];
    }
    std::cout << std::endl;
}

std::bitset<32> FloatingPointAnalyzer::getBits() const {
    return bits;
}

float FloatingPointAnalyzer::getValue() const {
    return value;
}

bool FloatingPointAnalyzer::getSign() const {
    return bits[31];
}

int FloatingPointAnalyzer::getExponent() const {
    int exp = 0;
    for (int i = 30; i >= 23; i--) {
        exp = (exp << 1) | bits[i];
    }
    return exp;
}

int FloatingPointAnalyzer::getMantissa() const {
    int mantissa = 0;
    for (int i = 22; i >= 0; i--) {
        mantissa = (mantissa << 1) | bits[i];
    }
    return mantissa;
}

bool validateArguments(int argc, char* argv[]);
float parseFloatingPoint(const std::string& str);
bool checkOverflow(float loopBound, float increment);
float calculateOverflowThreshold(float increment);
void displayResults(float loopBound, float increment);

int main(int argc, char* argv[]) {
    if (!validateArguments(argc, argv)) {
        return 1;
    }
    
    float loopBound, increment;
    try {
        loopBound = parseFloatingPoint(argv[1]);
        increment = parseFloatingPoint(argv[2]);
    } catch (const std::exception& e) {
        std::cerr << "Error: Invalid floating-point format in arguments." << std::endl;
        return 1;
    }
    
    FloatingPointAnalyzer boundAnalyzer(loopBound);
    FloatingPointAnalyzer incrementAnalyzer(increment);
    
    std::cout << "Loop bound:   ";
    boundAnalyzer.displayIEEEFormat();
    std::cout << "Loop counter: ";
    incrementAnalyzer.displayIEEEFormat();
    std::cout << std::endl;
    
    displayResults(loopBound, increment);
    
    return 0;
}

bool validateArguments(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "usage:" << std::endl;
        std::cerr << "    " << argv[0] << " loop_bound loop_counter" << std::endl;
        std::cerr << std::endl;
        std::cerr << "    loop_bound is a positive floating-point value" << std::endl;
        std::cerr << "    loop_counter is a positive floating-point value" << std::endl;
        return false;
    }
    
    if (argc > 3) {
        std::cerr << "usage:" << std::endl;
        std::cerr << "    " << argv[0] << " loop_bound loop_counter" << std::endl;
        std::cerr << std::endl;
        std::cerr << "    loop_bound is a positive floating-point value" << std::endl;
        std::cerr << "    loop_counter is a positive floating-point value" << std::endl;
        return false;
    }
    
    return true;
}

float parseFloatingPoint(const std::string& str) {
    char* endptr;
    float value = std::strtof(str.c_str(), &endptr);
    
    if (*endptr != '\0') {
        throw std::invalid_argument("Invalid floating-point format");
    }
    
    return value;
}

bool checkOverflow(float loopBound, float increment) {
    float threshold = calculateOverflowThreshold(increment);
    return loopBound >= threshold;
}

float calculateOverflowThreshold(float increment) {
    FloatingPointAnalyzer incrementAnalyzer(increment);
    int incrementExp = incrementAnalyzer.getExponent();
    
    if (increment <= 0.0f) {
        return std::numeric_limits<float>::infinity();
    }
    
    const int BIAS = 127;
    const int MANTISSA_BITS = 23;
    
    int actualIncrementExp = incrementExp - BIAS;
    int thresholdExpIEEE = actualIncrementExp + MANTISSA_BITS + BIAS;
    
    // Clamp to valid exponent range
    if (thresholdExpIEEE < 1) thresholdExpIEEE = 1;
    if (thresholdExpIEEE > 254) thresholdExpIEEE = 254;
    
    // Build IEEE 754 representation
    union {
        float f;
        uint32_t i;
    } converter;
    
    converter.i = (static_cast<uint32_t>(thresholdExpIEEE) << 23);
    return converter.f;
}

void displayResults(float loopBound, float increment) {
    bool willOverflow = checkOverflow(loopBound, increment);
    
    if (!willOverflow) {
        std::cout << "No overflow!" << std::endl;
    } else {
        std::cout << "Warning: Possible overflow!" << std::endl;
        std::cout << "Overflow threshold:" << std::endl;
        
        float threshold = calculateOverflowThreshold(increment);
        std::cout << "    " << std::scientific << threshold << std::endl;
        std::cout << "    ";
        
        FloatingPointAnalyzer thresholdAnalyzer(threshold);
        thresholdAnalyzer.displayIEEEFormat();
    }
}
