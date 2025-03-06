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

static int _longest_long_name = 0;

Argument::Argument() = default;

Argument::Argument(Argument_t arg_class,
                   int arg_ID,
                   const std::string& short_flag_name,
                   const std::string& long_flag_name,
                   const std::string& description,
                   Argument_t value_type,
                   size_t num_params)
  : _arg_ID(arg_ID)
  , _class(arg_class)
  , _short_flag_name(short_flag_name)
  , _long_flag_name(long_flag_name)
  , _description(description)
  , _value_type(value_type)
  , _num_params(num_params)
{
    initValueType();

    // By default, optional and required args have 1 param (flag = param).
    if (arg_class == OPTIONAL || arg_class == REQUIRED)
        if (_num_params == DEFAULT_NUM_FLAG_PARAMS)
            _num_params = 1;

    if (DEFAULT_LONG_FLAG_NAME != _long_flag_name)
        if (_long_flag_name.length() > _longest_long_name)
            _longest_long_name = _long_flag_name.length();
}

std::string
Argument::getDefaultValueToString() const
{
    using namespace std;

    const int index = _default_value.index();
    string result = {};
    if (index == 0)
        result = (get<bool>(_default_value) ? "true" : "false");
    else if (index == 1)
        result = to_string(get<int>(_default_value));
    else if (index == 2)
        result = to_string(get<float>(_default_value));
    else if (index == 3)
        result = to_string(get<double>(_default_value));
    else if (index == 4)
        result = get<string>(_default_value);

    return result;
}

Argument&
Argument::setDefaultValue(VarValue_t value)
{
    _default_value = value;
    _has_default_value = true;
    return *this;
}

void
Argument::setFlagType(Argument_t flag_type)
{
    if (flag_type == _flag_type)
        return;

    _flag_type = flag_type;

    if (_flag_type == DASH_FLAG) {
        _short_flag_chars = DEFAULT_SHORT_DASH_CHARS;
        _long_flag_chars = DEFAULT_LONG_DASH_CHARS;
    } else if (_flag_type == SLASH_FLAG) {
        _short_flag_chars = DEFAULT_SHORT_SLASH_CHARS;
        _long_flag_chars = DEFAULT_LONG_SLASH_CHARS;
    }
}

bool
Argument::matchesDefaultFlag(const std::string& value) const
{
    const bool arg_has_long_default = (_long_flag_name == DEFAULT_LONG_FLAG_NAME);
    const bool arg_has_short_default = (_short_flag_name == DEFAULT_SHORT_FLAG_NAME);
    const bool value_is_long_default = (value.compare(_long_flag_chars + DEFAULT_LONG_FLAG_NAME) == 0);
    const bool value_is_short_default = (value.compare(_short_flag_chars + DEFAULT_SHORT_FLAG_NAME) == 0);
    return (arg_has_long_default && value_is_long_default) ||
           (arg_has_short_default && value_is_short_default);
}

bool
Argument::matchesFlag(const std::string& value) const
{
    const std::string short_flag = _short_flag_chars + _short_flag_name;
    const std::string long_flag = _long_flag_chars + _long_flag_name;

    const bool matches_short = (value == short_flag);
    const bool matches_long = (value == long_flag);

    return matches_short || matches_long;
}

std::string
Argument::toString() const
{
    std::string long_flag_name = _long_flag_name;
    std::string long_flag_chars = _long_flag_chars;

    if (DEFAULT_LONG_FLAG_NAME == _long_flag_name) {
        long_flag_name = {};
        long_flag_chars = "  ";
    }

    const std::string column_space = "   ";
    const int space_diff = _longest_long_name - long_flag_name.length();

    std::string spaces = column_space;
    for (int i = 0; i < space_diff; ++i)
        spaces += " ";

    std::string first = _short_flag_chars + _short_flag_name + column_space +
                        long_flag_chars + long_flag_name + spaces;
    const int first_len = first.length();
    std::string first_space{};
    for (int i = 0; i < first_len; ++i)
        first_space += " ";

    std::string str_result = '\t' + first + _description;

    if (hasDefaultValue()) {
        std::string default_value = getDefaultValueToString();
        str_result += "\n\t" + first_space + "(default = " + getDefaultValueToString() + ")";
    }

    return str_result;
}

void
Argument::initValueType()
{
    if (_value_type != Argument_t::DEFAULT_VALUE_t)
        return;

    // If the value type wasn't set, set it now.
    if (Argument_t::X_SWITCH == _class)
        // Ensure the X_SWITCH class is of NO_VALUE type.
        _value_type = Argument_t::NO_VALUE_t;
    else if (Argument_t::SWITCH == _class)
        // Ensure the SWITCH class is of BOOLEAN type.
        _value_type = Argument_t::BOOLEAN_t;
    else if (Argument_t::OPTIONAL == _class ||
             Argument_t::REQUIRED == _class)
        // Set the real default to be a STRING type for OPTIONAL and REQUIRED.
        _value_type = Argument_t::STRING_t;
}

} // namespace AbeArgs
