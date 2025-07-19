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

#include "Argument.h"

namespace AbeArgs {

static size_t s_longest_long_name = 0;

Argument::Argument() = default;

Argument::Argument(ArgumentType p_arg_class,
                   int p_arg_ID,
                   const std::string& p_short_flag_name,
                   const std::string& p_long_flag_name,
                   const std::string& p_description,
                   ArgumentType p_value_type,
                   size_t p_num_params)
  : m_arg_ID(p_arg_ID)
  , m_class(p_arg_class)
  , m_short_flag_name(p_short_flag_name)
  , m_long_flag_name(p_long_flag_name)
  , m_description(p_description)
  , m_value_type(p_value_type)
  , m_num_params(p_num_params)
{
    initValueType();

    // By default, optional and required args have 1 param (flag = param).
    if (p_arg_class == OPTIONAL || p_arg_class == REQUIRED)
        if (m_num_params == DEFAULT_NUM_FLAG_PARAMS)
            m_num_params = 1;

    if (DEFAULT_LONG_FLAG_NAME != m_long_flag_name)
        if (m_long_flag_name.length() > s_longest_long_name)
            s_longest_long_name = m_long_flag_name.length();
}

std::string
Argument::getDefaultValueToString() const
{
    using namespace std;

    const size_t index = m_default_value.index();
    string result = {};
    if (index == 0)
        result = (get<bool>(m_default_value) ? "true" : "false");
    else if (index == 1)
        result = to_string(get<int>(m_default_value));
    else if (index == 2)
        result = to_string(get<float>(m_default_value));
    else if (index == 3)
        result = to_string(get<double>(m_default_value));
    else if (index == 4)
        result = get<string>(m_default_value);

    return result;
}

Argument&
Argument::setDefaultValue(VarValue_t p_value)
{
    m_default_value = p_value;
    m_has_default_value = true;
    return *this;
}

void
Argument::setFlagType(ArgumentType p_flag_type)
{
    if (p_flag_type == m_flag_type)
        return;

    m_flag_type = p_flag_type;

    if (m_flag_type == DASH_FLAG) {
        m_short_flag_chars = DEFAULT_SHORT_DASH_CHARS;
        m_long_flag_chars = DEFAULT_LONG_DASH_CHARS;
    } else if (m_flag_type == SLASH_FLAG) {
        m_short_flag_chars = DEFAULT_SHORT_SLASH_CHARS;
        m_long_flag_chars = DEFAULT_LONG_SLASH_CHARS;
    }
}

std::string
Argument::getShortFlag() const
{
    return m_short_flag_chars + m_short_flag_name;
}

std::string
Argument::getLongFlag() const
{
    return m_long_flag_chars + m_long_flag_name;
}

bool
Argument::matchesDefaultFlag(const std::string& p_value) const
{
    const bool arg_has_long_default = (m_long_flag_name == DEFAULT_LONG_FLAG_NAME);
    const bool arg_has_short_default = (m_short_flag_name == DEFAULT_SHORT_FLAG_NAME);
    const bool value_is_long_default = (p_value.compare(m_long_flag_chars + DEFAULT_LONG_FLAG_NAME) == 0);
    const bool value_is_short_default = (p_value.compare(m_short_flag_chars + DEFAULT_SHORT_FLAG_NAME) == 0);
    return (arg_has_long_default && value_is_long_default) ||
           (arg_has_short_default && value_is_short_default);
}

bool
Argument::matchesFlag(const std::string& p_value) const
{
    const std::string short_flag = m_short_flag_chars + m_short_flag_name;
    const std::string long_flag = m_long_flag_chars + m_long_flag_name;

    const bool matches_short = (p_value == short_flag);
    const bool matches_long = (p_value == long_flag);

    return matches_short || matches_long;
}

std::string
Argument::toString() const
{
    std::string long_flag_name = m_long_flag_name;
    std::string long_flag_chars = m_long_flag_chars;

    if (DEFAULT_LONG_FLAG_NAME == m_long_flag_name) {
        long_flag_name = {};
        long_flag_chars = "  ";
    }

    const std::string column_space = "   ";
    const size_t space_diff = s_longest_long_name - long_flag_name.length();

    std::string spaces = column_space;
    for (int i = 0; i < space_diff; ++i)
        spaces += " ";

    const std::string flags = m_short_flag_chars + m_short_flag_name + column_space +
                              long_flag_chars + long_flag_name + spaces;
    const size_t flags_len = flags.length();
    std::string flags_space{};
    for (int i = 0; i < flags_len; ++i)
        flags_space += " ";

    std::string str_result = '\t' + flags + m_description;

    if (isRequired())
        str_result += " (Required)";

    if (hasDefaultValue())
        str_result += "\n\t" + flags_space + "(default = " + getDefaultValueToString() + ")";

    return str_result;
}

void
Argument::initValueType()
{
    if (m_value_type != ArgumentType::DEFAULT_VALUE_TYPE)
        return;

    // If the value type wasn't set, set it now.
    if (ArgumentType::X_SWITCH == m_class)
        // Ensure the X_SWITCH class is of NO_VALUE type.
        m_value_type = ArgumentType::NO_VALUE_TYPE;
    else if (ArgumentType::SWITCH == m_class)
        // Ensure the SWITCH class is of BOOLEAN type.
        m_value_type = ArgumentType::BOOLEAN_TYPE;
    else if (ArgumentType::OPTIONAL == m_class ||
             ArgumentType::REQUIRED == m_class)
        // Set the real default to be a STRING type for OPTIONAL and REQUIRED.
        m_value_type = ArgumentType::STRING_TYPE;
}

} // namespace AbeArgs
