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

#include "Parser.h"

// Project includes
#include "Argument.h"
#include "Math.h"
#include "Util.h"

// Standard includes
#include <iostream>
#include <memory>

using namespace std;

namespace AbeArgs {

static Argument s_no_arg;

Argument*
Parser::addArgument(const Argument& p_arg)
{
    m_args.push_back(std::move(p_arg));
    if (p_arg.isRequired())
        // Arguments start out missing (they haven't been parsed yet).
        m_required_args.insert({ p_arg.getID(), true });
    return &m_args.back();
}

Argument&
Parser::getArgument(int p_arg_ID)
{
    for (Argument& arg : m_args)
        if (arg.getID() == p_arg_ID)
            return arg;

    return s_no_arg;
}

const Argument&
Parser::getArgument(int p_arg_ID) const
{
    for (const Argument& arg : const_cast<ArgumentList_t&>(m_args))
        if (arg.getID() == p_arg_ID)
            return arg;

    return s_no_arg;
}

Argument&
Parser::getArgument(const std::string& p_flag)
{
    for (Argument& arg : m_args)
        if (arg.matchesFlag(p_flag))
            return arg;

    return s_no_arg;
}

const ArgumentList_t&
Parser::getArguments() const
{
    return m_args;
}

// Boolean testing can be done for internal look-aheads (quietly), or
// somewhat noisily when evaluating user input.
ValidBool_t
Parser::getBoolean(const string& p_value) const
{
    bool is_boolean = false;

    // Test for true params.
    is_boolean |= (0 == strcasecmp("t", p_value.c_str()));
    is_boolean |= (0 == strcasecmp("true", p_value.c_str()));
    is_boolean |= (0 == strcasecmp("y", p_value.c_str()));
    is_boolean |= (0 == strcasecmp("yes", p_value.c_str()));
    is_boolean |= (0 == strcasecmp("1", p_value.c_str()));
    is_boolean |= (0 == strcasecmp("on", p_value.c_str()));
    if (is_boolean)
        // Return a boolean that is true.
        return make_pair(true, true);

    // Test for false params.
    is_boolean |= (0 == strcasecmp("f", p_value.c_str()));
    is_boolean |= (0 == strcasecmp("false", p_value.c_str()));
    is_boolean |= (0 == strcasecmp("n", p_value.c_str()));
    is_boolean |= (0 == strcasecmp("no", p_value.c_str()));
    is_boolean |= (0 == strcasecmp("0", p_value.c_str()));
    is_boolean |= (0 == strcasecmp("off", p_value.c_str()));
    if (is_boolean)
        // Return a boolean that is false.
        return make_pair(true, false);

#ifdef DEBUG_BUILD
    std::cerr << "error[b]: Invalid boolean: " << p_value << '\n';
#endif

    // Don't return a boolean.
    return make_pair(false, false);
}

ValidInt_t
Parser::getInteger(const string& p_value) const
{
    try {
        const int int_value = stoi(p_value);
        const string str_value = to_string(int_value);
        if (str_value == p_value)
            return make_pair(true, int_value);
        else {
#ifdef DEBUG_BUILD
            std::cerr << "error[i0]: Invalid integer: " << p_value << '\n';
#endif
            return make_pair(false, 0);
        }
    } catch (const invalid_argument& inv_arg) {
#ifdef DEBUG_BUILD
        std::cerr << "error[i1]: Invalid integer: " << p_value << '\n';
#endif
    }
    // Don't return an integer.
    return make_pair(false, 1);
}

ValidFloat_t
Parser::getFloat(const string& p_value) const
{
    // Convert to a float value.
    const float f1_value = strtod(p_value.c_str(), nullptr);
    // Convert the float back to a string value.
    const string str_value = to_string(f1_value);
    // Convert to a float (again).
    const float f2_value = strtod(str_value.c_str(), nullptr);
    // Compare the strings to see if they are the same.
    if (Math::isEqual(f1_value, f2_value))
        // They are the same. It worked.
        return make_pair(true, f1_value);

#ifdef DEBUG_BUILD
    std::cerr << "error[f0]: Invalid float: " << p_value << '\n';
#endif

    // Don't return a float.
    return make_pair(false, 0);
}

ValidDouble_t
Parser::getDouble(const string& p_value) const
{
    // Convert to a double value.
    const double d1_value = strtod(p_value.c_str(), nullptr);
    // Convert the double back to a string value.
    const string str_value = to_string(d1_value);
    // Convert to a double value (again).
    const double d2_value = strtod(str_value.c_str(), nullptr);
    // Compare the converted values to see if they are the same.
    if (Math::isEqual(d1_value, d2_value))
        // They are the same. It worked.
        return make_pair(true, d1_value);

#ifdef DEBUG_BUILD
    std::cerr << "error[d0]: Invalid double: " << p_value << '\n';
#endif

    // Don't return a double.
    return make_pair(false, 0);
}

bool
Parser::fileExists(const char* p_file_path) const
{
    if (FILE* file = fopen(p_file_path, "r")) {
        fclose(file);
        return true;
    }
    return false;
}

ParsedArguments_t
Parser::exec(int p_argc, char* p_argv[])
{
    // Combine the exec line into one string for parsing by the parser.
    // Start at 1 to exclude the executable name (argv[0]).

    std::string argv_str{};
    for (int i = 1; i < p_argc; ++i) {
        argv_str += p_argv[i];
        if (i + 1 < p_argc)
            argv_str += " ";
    }

    return exec(argv_str);
}

ParsedArguments_t
Parser::exec(const string& p_argv)
{
    clearError();
    resetMissingArgs();

    Argument arg;
    ParsedArguments_t results;

    const char space_char = ' ';
    const char pairs[] = { '\'', '\'', '"', '"', '(', ')', '[', ']', '{', '}', '<', '>' };
    const int pairs_len = sizeof(pairs) / sizeof(char);

    // Make a copy of argv.
    string argv_str = p_argv;
    // Replace all '=' with ' ' except if within a block of pairs.
    Util::replaceAll(argv_str, '=', space_char, pairs, pairs_len);
    // Replace all ',' with ' ' except if within a block of pairs.
    Util::replaceAll(argv_str, ',', space_char, pairs, pairs_len);

    // Tokenize the arguments (separate with the space_char).
    m_argv_tokens = Util::tokenize(argv_str, space_char);
    // Join any tokens surrounded by string_pairs.
    for (size_t i = 0; i < pairs_len; i = i + 2)
        m_argv_tokens = Util::joinDelimitedTokens(m_argv_tokens, pairs[i], pairs[i + 1], space_char);
    // Get the new number of arguments.
    const int argc = m_argv_tokens.size();

    for (size_t i = 0, n = argc; i < n; ++i) {
        int next_i = i + 1;
        const bool has_next_i = (next_i < n);

        if (error())
            break;

        arg = getArgument(m_argv_tokens[i]);
        if (arg.isValidArg()) {

            // Arguments can be created with default long and short names.
            // These defaults signify an empty flag option.
            // If a user knows the default and tries to use the empty arg name, ignore it.
            if (arg.matchesDefaultFlag(m_argv_tokens[i])) {
                setErrorMsg("error: Unrecognized command-line option: " + m_argv_tokens[i]);
                break;
            }

            // Optional and Required flags specify how many params they need and their type.
            const size_t num_params = arg.getNumParams();
            const ArgumentType value_type = arg.getValueType();
            const bool arg_is_string_type = (value_type == STRING_TYPE);
            const bool arg_is_file_type = (value_type == FILE_TYPE);
            const bool arg_is_bool_type = (value_type == BOOLEAN_TYPE);
            const bool arg_is_int_type = (value_type == INTEGER_TYPE);
            const bool arg_is_float_type = (value_type == FLOAT_TYPE);
            const bool arg_is_double_type = (value_type == DOUBLE_TYPE);

            if (arg.isXSwitch()) {
                // Only handle the first exclusive switch, then return.
                results.clear();
                results.push_back(make_pair(arg.getID(), true));
                return results;
            } else if (arg.isSwitch()) {
                // Switch flags can have 0 or 1 params.
                // By default the presence of a switch turns something on (acts true).
                // When followed by a boolean, it takes the value.
                if (num_params == 0) {
                    // The presence of the switch makes it true.
                    results.push_back(make_pair(arg.getID(), true));
                    continue;
                } else if ((num_params == 1) && has_next_i) {
                    // The value of the switch is defined by the next parameter.
                    const auto result = getBoolean(m_argv_tokens[next_i]);
                    if (result.first)
                        // If a boolean was found, assign the value.
                        results.push_back(make_pair(arg.getID(), result.second));
                    else
                        setErrorMsg("error: Invalid boolean: " + m_argv_tokens[next_i]);
                    i = next_i;
                    continue;
                }
            } else if (arg.isOptional() || arg.isRequired()) {
                if ((num_params == 1) && has_next_i) {
                    const int results_before_size = results.size();

                    // Verify the type and add to the results.
                    if (arg_is_string_type) {
                        const string param_value = m_argv_tokens[next_i];
                        results.push_back(make_pair(arg.getID(), param_value));
                    } else if (arg_is_file_type) {
                        const char* param_cstr_value = m_argv_tokens[next_i].c_str();
                        if (fileExists(param_cstr_value))
                            results.push_back(make_pair(arg.getID(), param_cstr_value));
                        else
                            setErrorMsg("error: File not found: " + m_argv_tokens[next_i]);
                    } else if (arg_is_bool_type) {
                        const auto result = getBoolean(m_argv_tokens[next_i]);
                        if (result.first)
                            results.push_back(make_pair(arg.getID(), result.second));
                        else
                            setErrorMsg("error: Invalid boolean: " + m_argv_tokens[next_i]);
                    } else if (arg_is_int_type) {
                        const auto result = getInteger(m_argv_tokens[next_i]);
                        if (result.first)
                            results.push_back(make_pair(arg.getID(), result.second));
                        else
                            setErrorMsg("error: Invalid integer: " + m_argv_tokens[next_i]);
                    } else if (arg_is_float_type) {
                        const auto result = getFloat(m_argv_tokens[next_i]);
                        if (result.first)
                            results.push_back(make_pair(arg.getID(), result.second));
                        else
                            setErrorMsg("error: Invalid float: " + m_argv_tokens[next_i]);
                    } else if (arg_is_double_type) {
                        const auto result = getDouble(m_argv_tokens[next_i]);
                        if (result.first)
                            results.push_back(make_pair(arg.getID(), result.second));
                        else
                            setErrorMsg("error: Invalid double: " + m_argv_tokens[next_i]);
                    }

                    if (!error()) {
                        // Compare the before and after size of results.
                        const bool arg_was_added = (results.size() == results_before_size + 1);
                        if (arg.isRequired() && arg_was_added) {
                            // After seeing and adding the required arg, remove it from the list.
                            // Later we will know if all required args were used if this list is empty.
                            m_required_args[arg.getID()] = false;
                        }
                    }

                    i = next_i;
                    continue;
                } else if ((num_params > 1) && has_next_i) {
                    const int results_before_size = results.size();

                    Util::StringList str_results(num_params);
                    int idx = 0;
                    // Verify type but pack each into a space separated string.
                    for (size_t j = next_i, m = next_i + num_params; j < m; ++j, ++i, ++next_i) {
                        bool result_ok = false;
                        string value_type_str = {};

                        if (arg_is_bool_type) {
                            value_type_str = "boolean";
                            const auto result = getBoolean(m_argv_tokens[j]);
                            result_ok = result.first;
                        } else if (arg_is_int_type) {
                            value_type_str = "int";
                            const auto result = getInteger(m_argv_tokens[j]);
                            result_ok = result.first;
                        } else if (arg_is_float_type) {
                            value_type_str = "float";
                            const auto result = getFloat(m_argv_tokens[j]);
                            result_ok = result.first;
                        } else if (arg_is_double_type) {
                            value_type_str = "double";
                            const auto result = getDouble(m_argv_tokens[j]);
                            result_ok = result.first;
                        }

                        if (result_ok) {
                            str_results[idx++] = m_argv_tokens[j];
                        } else {
                            setErrorMsg("error: Invalid " + value_type_str + ": " + m_argv_tokens[j]);
                            break;
                        }
                    }

                    if (!error()) {
                        results.push_back(make_pair(arg.getID(), Util::join(str_results, ',')));

                        // Compare the before and after size of results.
                        const bool arg_was_added = (results.size() == results_before_size + 1);
                        if (arg.isRequired() && arg_was_added) {
                            // After seeing and adding the required arg, remove it from the list.
                            // Later we will know if all required args were used if this list is empty.
                            m_required_args[arg.getID()] = false;
                        }
                    }
                }
            }
        } else {
            setErrorMsg("error: Unrecognized command-line option: " + m_argv_tokens[i]);
            break;
        }
    }

    if (results.empty())
        // Return the default NO_ARG option.
        return { make_pair(NO_ARG, std::string(DEFAULT_STR)) };

    return results;
}

bool
Parser::error() const
{
    return !m_error_msg.empty();
}

std::string
Parser::getErrorMsg() const
{
    return m_error_msg;
}

void
Parser::resetMissingArgs()
{
    for (auto& [arg_id, arg_missing] : m_required_args)
        arg_missing = true;
}

void
Parser::clearError()
{
    m_error_msg.clear();
}

void
Parser::setErrorMsg(const std::string& p_value)
{
    m_error_msg = p_value;
}

bool
Parser::isMissingRequiredArgs() const
{
    bool missing = false;
    for (auto const& [arg_ID, arg_missing] : m_required_args)
        missing |= arg_missing;

    return missing;
}

bool
Parser::hasArgvToken(int p_arg_ID) const
{
    const Argument arg = getArgument(p_arg_ID);
    if (Util::contains(m_argv_tokens, arg.getShortFlag()))
        return true;

    if (Util::contains(m_argv_tokens, arg.getLongFlag()))
        return true;

    return false;
}

} // namespace AbeArgs
