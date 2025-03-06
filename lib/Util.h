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
    typedef std::vector<std::string> StringList_t;

  public:
    Util() = delete;
    ~Util() = delete;

    /**
     * @brief Join two strings with a separator character in between.
     *
     * @param s1 The first string (to be appended to)
     * @param s2 The second string (to append to the first)
     * @param separator The char that separates s1 and s2
     * @return The joined string
     */
    static std::string join(const std::string& s1,
                            const std::string& s2,
                            char separator = ' ')
    {
        if (s1.empty())
            return s2;
        if (s2.empty())
            return s1;

        if (s1.back() == separator || s2.front() == separator)
            return s1 + s2;

        return s1 + separator + s2;
    }

    /// @brief Join the tokens with a separator char
    /// @param tokens The tokens to join
    /// @param separator with this separator char
    /// @return The joined string
    /// @date Feb 2025
    static std::string join(const StringList_t& tokens, char separator)
    {
        size_t result_len = 0;
        for (const auto& token : tokens)
            result_len += (token.length() + 1);

        std::string result = {};
        result.reserve(result_len);

        for (int i = 0, n = tokens.size(); i < n; ++i) {
            result += tokens[i];
            if (i < n - 1)
                result += separator;
        }

        return result;
    }

    /**
     * @brief Check to see if the vector contains the element (in any position)
     *
     * @param vector_
     * @param element
     * @return bool
     *
     * @author Abe Mishler
     * @date OCT-2022
     *
     */
    template<typename T>
    static bool contains(std::vector<T> vector_, const T& element)
    {
        // See if element is in the vector.
        return any_of(
          vector_.begin(), vector_.end(), [&element](auto const& value) {
              return element == value;
          });
    }

    /// @brief Determine if an array contains find
    /// @tparam T is the template class
    /// @param array The array to search
    /// @param array_len The length of the array to search
    /// @param find The char to find
    /// @param index The index position of the find char
    /// @return A boolean (yes/no) if the find char was found
    /// @date Feb 2025
    template<typename T>
    static bool arrayContains(const T* array, int array_len, T find, int& index)
    {
        for (int i = 0; i < array_len; ++i)
            if (find == array[i]) {
                index = i;
                return true;
            }

        index = -1;
        return false;
    }

    static int replaceAll(std::string& str, char find, char replace)
    {
        int count = 0;
        for (auto& c : str)
            if (c == find) {
                c = replace;
                ++count;
            }

        return count;
    }

    /// @brief Replace all instances of find with replace in str except those inside a block of pairs.
    /// @param str The string to search
    /// @param find The char to search for
    /// @param replace The char to overwrite with
    /// @param pairs The pairs that open and close a block of a string
    /// @return The number of replacements made
    /// @date Feb 2025
    static int replaceAll(std::string& str, char find, char replace, const char* pairs, int pairs_len)
    {
        if (pairs_len % 2 != 0)
            // Pairs must be even.
            return 0;

        const int num_pairs = pairs_len / 2;

        // Split the pairs into openers and closers.
        char openers[num_pairs] = {};
        char closers[num_pairs] = {};
        for (int i = 0, j = 0; i < num_pairs; ++i, j += 2) {
            openers[i] = pairs[j];
            closers[i] = pairs[j + 1];
        }

        int count = 0;
        bool open = false;
        int which_pair = -1;
        for (auto& c : str) {
            if (!open && arrayContains(openers, num_pairs, c, which_pair)) {
                open = true;
            } else if (open && which_pair > -1 && c == closers[which_pair]) {
                open = false;
                which_pair = -1;
            }

            if (!open && c == find) {
                c = replace;
                ++count;
            }
        }

        return count;
    }

    static bool endsWith(const std::string& str, const std::string& end)
    {
        const size_t str_len = str.size();
        const size_t end_len = end.size();
        const size_t n = str_len - end_len;

        return (str.rfind(end, n) == n);
    }

    static bool endsWith(const std::string& str, char c)
    {
        if (!str.empty()) {
            const int len = str.length();
            return (str[len - 1] == c);
        }
        // str is empty and doesn't end with char c.
        return false;
    }

    static bool startsWith(const std::string& str, const std::string& start)
    {
        return (str.rfind(start, 0) == 0);
    }

    static bool startsWith(const std::string& str, char c)
    {
        if (!str.empty())
            return (str[0] == c);

        // str is empty and doesn't begin with char c.
        return false;
    }

    /**
     * @brief Break up the sequence of words into pieces using the separator
     * character
     *
     * @param sequence of words
     * @param separator the character that separates each word
     * @return StringList_t
     *
     * @author Abe Mishler
     * @date OCT-2022
     */
    static StringList_t tokenize(const std::string& sequence, char separator)
    {
        StringList_t tokens;
        std::stringstream ss(sequence);
        std::string word;
        while (getline(ss, word, separator))
            if (!word.empty())
                tokens.push_back(word);

        return tokens;
    }

    static StringList_t joinDelimitedTokens(const StringList_t& tokens,
                                            char left_delim,
                                            char right_delim,
                                            char separator)
    {
        StringList_t result = {}, temp = {};
        bool do_join = false;
        for (const auto& token : tokens) {
            if (startsWith(token, left_delim) || do_join) {
                temp.push_back(token);
                do_join = true;
            } else
                result.push_back(token);

            if (endsWith(token, right_delim) && do_join) {
                const std::string joined = join(temp, separator);
                result.push_back(joined.substr(1, joined.length() - 2));
                temp.clear();
                do_join = false;
            }
        }

        if (!temp.empty())
            result.push_back(join(temp, separator));

        return result;
    }

    static std::string toLowerStr(const std::string& word_str)
    {
        std::string my_word_str = word_str;
        std::transform(my_word_str.begin(),
                       my_word_str.end(),
                       my_word_str.begin(),
                       ::tolower);
        return my_word_str;
    }

    static bool compareStr(const std::string& lhs,
                           const std::string& rhs,
                           bool ignore_case = true)
    {
        // Ensure lengths are the same, otherwise not equal.
        if (lhs.length() != rhs.length())
            return false;

        // Now compare the strings.
        if (ignore_case)
            return (strcasecmp(lhs.c_str(), rhs.c_str()) == 0);

        return (strcmp(lhs.c_str(), rhs.c_str()) == 0);
    }
};

} // namespace AbeArgs
