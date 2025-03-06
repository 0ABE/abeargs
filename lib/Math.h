/**
 *           d8888 888                     d8888
 *          d88888 888                    d88888
 *         d88P888 888                   d88P888
 *        d88P 888 88888b.   .d88b.     d88P 888 888d888 .d88b.  .d8888b
 *       d88P  888 888 "88b d8P  Y8b   d88P  888 888P"  d88P"88b 88K
 *      d88P   888 888  888 88888888  d88P   888 888    888  888 "Y8888b.
 *     d8888888888 888 d88P Y8b.     d8888888888 888    Y88b 888      X88
 *    d88P     888 88888P"   "Y8888 d88P     888 888     "Y88888  88888P'
 *                                                           888
 * ~$ Command Line Argument Processing Simplified       Y8b d88P
 *                                                       "Y88P"
 * Copyright (c) 2025, Abe Mishler
 * Licensed under the Universal Permissive License v 1.0
 * as shown at https://oss.oracle.com/licenses/upl/.
 */

#pragma once

namespace AbeArgs {

class Math
{

  public:
    Math() = delete;
    ~Math() = delete;

    /**
     * @brief Test if 2 numbers are equal within an epsilon amount.
     *
     * @param a The first number
     * @param b The second number
     * @param eps The difference allowed between them
     * @return bool
     */
    template<class T>
    static bool isEqual(T a, T b, T eps = 1e-6)
    {
        // c = fabs(a - b)
        T c = abs(a - b);
        return (c <= eps);
    }

    /**
     * @brief Absolute value of a T.
     *
     * @param c The template type value.
     * @return A positive template type value.
     */
    template<class T>
    static T
    abs(T c)
    {
        if (c < 0)
            c *= -1;

        return c;
    }
};

} // namespace AbeArgs