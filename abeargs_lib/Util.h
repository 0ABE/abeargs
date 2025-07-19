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

// Project includes
#ifdef _MSC_VER
#include "MSVC.h"
#endif

// Standard includes
#include <algorithm>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

namespace AbeArgs {

class Util
{
  public:
    typedef std::vector<std::string> StringList;

  public:
    Util() = delete;
    ~Util() = delete;

    /**
     * @brief Join two strings with a separator character in between.
     *
     * @param p_s1 The first string (to be appended to)
     * @param p_s2 The second string (to append to the first)
     * @param p_separator The char that separates s1 and s2
     * @return The joined string
     */
    static std::string join(const std::string& p_s1,
                            const std::string& p_s2,
                            char p_separator = ' ')
    {
        if (p_s1.empty())
            return p_s2;
        if (p_s2.empty())
            return p_s1;

        if (p_s1.back() == p_separator || p_s2.front() == p_separator)
            return p_s1 + p_s2;

        return p_s1 + p_separator + p_s2;
    }

    /// @brief Join the tokens with a separator char
    /// @param p_tokens The tokens to join
    /// @param p_separator with this separator char
    /// @return The joined string
    /// @date Feb 2025
    static std::string join(const StringList& p_tokens, char p_separator)
    {
        size_t result_len = 0;
        for (const auto& token : p_tokens)
            result_len += (token.length() + 1);

        std::string result = {};
        result.reserve(result_len);

        for (size_t i = 0, n = p_tokens.size(); i < n; ++i) {
            result += p_tokens[i];
            if (i < n - 1)
                result += p_separator;
        }

        return result;
    }

    /**
     * @brief Check to see if the vector contains the element (in any position)
     * @param p_vector
     * @param p_element
     * @return bool
     * @author Abe Mishler
     * @date OCT-2022
     *
     */
    template<typename T>
    static bool contains(std::vector<T> p_vector, const T& p_element)
    {
        // See if element is in the vector.
        return any_of(
          p_vector.begin(), p_vector.end(), [&p_element](auto const& value) {
              return p_element == value;
          });
    }

    /// @brief Determine if an array contains find
    /// @tparam T is the template class
    /// @param p_array The array to search
    /// @param p_array_len The length of the array to search
    /// @param p_find The char to find
    /// @param p_index The index position of the find char
    /// @return A boolean (yes/no) if the find char was found
    /// @date Feb 2025
    template<typename T>
    static bool arrayContains(const T* p_array, int p_array_len, T p_find, int& p_index)
    {
        for (int i = 0; i < p_array_len; ++i)
            if (p_find == p_array[i]) {
                p_index = i;
                return true;
            }

        p_index = -1;
        return false;
    }

    static int replaceAll(std::string& p_str, char p_find, char p_replace)
    {
        int count = 0;
        for (auto& c : p_str)
            if (c == p_find) {
                c = p_replace;
                ++count;
            }

        return count;
    }

    /// @brief Replace all instances of find with replace in str except those inside a block of pairs.
    /// @param p_str The string to search
    /// @param p_find The char to search for
    /// @param p_replace The char to overwrite with
    /// @param p_pairs The pairs that open and close a block of a string
    /// @return The number of replacements made
    /// @date Feb 2025
    static int replaceAll(std::string& p_str, char p_find, char p_replace, const char* p_pairs, int p_pairs_len)
    {
        if (p_pairs_len % 2 != 0)
            // Pairs must be even.
            return 0;

        const int num_pairs = p_pairs_len / 2;

        // Split the pairs into openers and closers.
        std::vector<char> openers_vec(num_pairs);
        std::vector<char> closers_vec(num_pairs);
        for (int i = 0, j = 0; i < num_pairs; ++i, j += 2) {
            openers_vec[i] = p_pairs[j];
            closers_vec[i] = p_pairs[j + 1];
        }

        int count = 0;
        bool open = false;
        int which_pair = -1;
        for (auto& c : p_str) {
            if (!open && arrayContains(openers_vec.data(), num_pairs, c, which_pair)) {
                open = true;
            } else if (open && which_pair > -1 && c == closers_vec[which_pair]) {
                open = false;
                which_pair = -1;
            }

            if (!open && c == p_find) {
                c = p_replace;
                ++count;
            }
        }

        return count;
    }

    static bool endsWith(const std::string& p_str, const std::string& p_end)
    {
        const size_t str_len = p_str.size();
        const size_t end_len = p_end.size();
        const size_t n = str_len - end_len;

        return (p_str.rfind(p_end, n) == n);
    }

    static bool endsWith(const std::string& p_str, char p_c)
    {
        if (!p_str.empty()) {
            const size_t len = p_str.length();
            return (p_str[len - 1] == p_c);
        }
        // str is empty and doesn't end with char c.
        return false;
    }

    static bool startsWith(const std::string& p_str, const std::string& p_start)
    {
        return (p_str.rfind(p_start, 0) == 0);
    }

    static bool startsWith(const std::string& p_str, char p_c)
    {
        if (!p_str.empty())
            return (p_str[0] == p_c);

        // str is empty and doesn't begin with char c.
        return false;
    }

    /**
     * @brief Break up the sequence of words into pieces using the separator
     * character
     *
     * @param p_sequence of words
     * @param p_separator the character that separates each word
     * @return StringList
     *
     * @author Abe Mishler
     * @date OCT-2022
     */
    static StringList tokenize(const std::string& p_sequence, char p_separator)
    {
        StringList tokens;
        std::stringstream ss(p_sequence);
        std::string word;
        while (getline(ss, word, p_separator))
            if (!word.empty())
                tokens.push_back(word);

        return tokens;
    }

    static StringList joinDelimitedTokens(const StringList& p_tokens,
                                          char p_left_delim,
                                          char p_right_delim,
                                          char p_separator)
    {
        StringList result = {}, temp = {};
        bool do_join = false;
        for (const auto& token : p_tokens) {
            if (startsWith(token, p_left_delim) || do_join) {
                temp.push_back(token);
                do_join = true;
            } else
                result.push_back(token);

            if (endsWith(token, p_right_delim) && do_join) {
                const std::string joined = join(temp, p_separator);
                result.push_back(joined.substr(1, joined.length() - 2));
                temp.clear();
                do_join = false;
            }
        }

        if (!temp.empty())
            result.push_back(join(temp, p_separator));

        return result;
    }

    static std::string toLowerStr(const std::string& p_word_str)
    {
        std::string my_word_str = p_word_str;
        std::transform(my_word_str.begin(),
                       my_word_str.end(),
                       my_word_str.begin(),
                       ::tolower);
        return my_word_str;
    }

    static bool compareStr(const std::string& p_lhs,
                           const std::string& p_rhs,
                           bool p_ignore_case = true)
    {
        // Ensure lengths are the same, otherwise not equal.
        if (p_lhs.length() != p_rhs.length())
            return false;

        // Now compare the strings.
        if (p_ignore_case)
            return (strcasecmp(p_lhs.c_str(), p_rhs.c_str()) == 0);

        return (strcmp(p_lhs.c_str(), p_rhs.c_str()) == 0);
    }
};

} // namespace AbeArgs
