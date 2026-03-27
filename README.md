# Floating-Point Overflow Detection

## Overview

This program detects floating-point precision loss in C++ loop operations by analyzing IEEE 754 bit representations. Given a loop bound and increment value, it mathematically determines if precision loss will occur when repeatedly adding the increment to a counter variable.

## Problem Description

In floating-point arithmetic, repeatedly adding small increments to a counter can lead to precision loss. When the counter becomes large enough, adding the increment may have no effect due to limited mantissa precision in IEEE 754 representation. This can cause infinite loops in programs that rely on precise floating-point arithmetic.

## Features

- **IEEE 754 Analysis**: Displays bit-level representation of floating-point values
- **Mathematical Detection**: Uses bit manipulation to calculate overflow thresholds (no brute-force)
- **Command-line Interface**: Simple two-argument input validation
- **Precise Output**: Matches exact formatting requirements

## Build Instructions

### Prerequisites
- C++ compiler with C++20 support (clang++ recommended)
- Make utility

### Building

```bash
# Navigate to project directory
cd homework2

# Build the program
make

# Alternative: build with debug symbols
make debug

# Clean build artifacts
make clean
```

### Manual Build (Alternative)
```bash
clang++ -std=c++20 -Wall -Wextra -pedantic -o fp_overflow_checker main.cpp
```

## Usage

```bash
./fp_overflow_checker loop_bound loop_counter
```

### Parameters
- **loop_bound**: Target value that a loop counter is trying to reach
- **loop_counter**: Increment value added to counter each iteration

Both parameters must be positive floating-point values and support standard (10.5) and scientific notation (1.05e+01).

### Examples

```bash
# No overflow case
./fp_overflow_checker 1e+05 2.0

# Overflow case
./fp_overflow_checker 1e+08 1.0

# Scientific notation
./fp_overflow_checker 16777216.0 0.1
```

## Output Format

### Successful Analysis
```
Loop bound:   0 10001111 10000110101000000000000
Loop counter: 0 10000000 00000000000000000000000

No overflow!
```

### Overflow Detection
```
Loop bound:   0 10011001 01111101011110000100000
Loop counter: 0 01111111 00000000000000000000000

Warning: Possible overflow!
Overflow threshold:
    8.388608e+06
    0 10010110 00000000000000000000000
```

### Error Cases
```
usage:
    ./fp_overflow_checker loop_bound loop_counter

    loop_bound is a positive floating-point value
    loop_counter is a positive floating-point value
```

## Technical Implementation

### Core Algorithm
The program calculates the **Unit in Last Place (ULP)** threshold where floating-point precision is lost:

1. **Extract IEEE 754 components** using C++ `bitset<32>`
2. **Calculate exponent** where ULP equals the increment size
3. **Build threshold value** using bit manipulation
4. **Compare** loop bound against calculated threshold

### IEEE 754 Bit Display
- **Bit 31**: Sign bit (0 = positive, 1 = negative)
- **Bits 30-23**: Exponent (8 bits, biased by 127)
- **Bits 22-0**: Mantissa (23 bits + 1 implicit bit)

## Known Limitations

### 1. **32-bit Float Precision Only**
- Only supports `float` (32-bit IEEE 754)
- Does not handle `double` (64-bit) precision
- Limited to ~7 decimal digits of precision

### 2. **Positive Values Only**
- Algorithm assumes positive floating-point inputs
- Negative values may produce incorrect results
- Zero or negative increments return infinity threshold

### 3. **Normalized Numbers Only**
- Does not handle denormalized (subnormal) numbers
- Very small values near zero may be inaccurate
- Exponent range clamped to 1-254 (normalized range)

### 4. **Rounding Behavior**
- Uses default IEEE 754 rounding (round-to-nearest-even)
- Does not account for different rounding modes
- May have slight variations across different systems

### 5. **Scientific Notation Output**
- Threshold values displayed in scientific notation only
- May not match exact decimal representation expected
- Precision limited to 6 decimal places in some outputs


## Development Notes

### Code Structure
- **FloatingPointAnalyzer class**: Handles IEEE 754 bit manipulation
- **Modular functions**: Separate validation, parsing, and analysis logic
- **Clean error handling**: Proper exception handling and user feedback

