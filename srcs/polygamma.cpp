#include <iostream>
#include <cmath>

/**
 * @brief Compute digamma, trigamma, tetragamma, and pentagamma functions.
 *
 * This collection of functions provides utilities for calculating various polygamma functions,
 * including digamma (ψ), trigamma (ψ'), tetragamma (ψ''), and pentagamma (ψ'''). These functions are 
 * useful in mathematical and statistical computations involving special functions and derivatives.
 *
 * @param x The input value for which the polygamma function is to be computed.
 *
 * @return The computed polygamma value corresponding to the specified order (n) and input value (x).
 *         For digamma (n=0), the function returns ψ(x). For trigamma (n=1), it returns ψ'(x), and so on.
 *         If the order (n) is not supported, an error message is displayed, and the function returns NAN.
 *
 * @note The digamma function is computed using a combination of Euler's constant and an asymptotic expansion.
 *       The trigamma, tetragamma, and pentagamma functions are derived using differences of digamma functions.
 */
double digamma(double x){
    // Euler's constant
    const double euler_gamma = 0.57721566490153286060;

    // Check for special cases
    if (x == 0.0 || std::isinf(x) || std::isnan(x))
        return NAN;

    // Use Euler-Mascheroni constant for x < 1
    if (x < 1.0)
        return euler_gamma - 1.0 / x;

    // Use recurrence relation for x >= 1
    double result = 0.0;
    while (x < 8.5) {
        result -= 1.0 / x;
        x += 1.0;
    }

    // Asymptotic expansion for x >= 8.5
    const double inv_x_squared = 1.0 / (x * x);
    result += std::log(x) - 0.5 / x - inv_x_squared / 12.0 + inv_x_squared / 120.0;

    return result;
}

double trigamma(double x){
    return digamma(x + 1.0) - digamma(x);
}

double tetragamma(double x) {
    return digamma(x) - 2 * digamma(x + 1.0) + digamma(x + 2.0);
}

double pentagamma(double x) {
    return digamma(x + 2.0) - 3 * digamma(x + 1.0) + 3 * digamma(x) - digamma(x - 1.0);
}

/**
 * @brief Compute polygamma function of arbitrary order.
 *
 * The polygamma function of order n is calculated based on the specified order and input value.
 * It uses the previously defined functions (digamma, trigamma, tetragamma, pentagamma) for orders 0 to 3.
 *
 * @param n The order of the polygamma function (0 for digamma, 1 for trigamma, 2 for tetragamma, 3 for pentagamma).
 * @param val The input value for which the polygamma function is to be computed.
 *
 * @return The computed polygamma value for the specified order (n) and input value (val).
 *         If the order (n) is not supported, an error message is displayed, and the function returns NAN.
 */
double polygamma(int n, double val) {
    switch (n) {
    case 0:
        return digamma(val);
    case 1:
        return trigamma(val);
    case 2:
        return tetragamma(val);
    case 3:
        return pentagamma(val);
    default:
        std::cerr << "Polygamma order not supported" << std::endl;
        return NAN;  // or any other suitable error value
    }
}


