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
#include "Defaults.h"

// Standard includes
#include <string>
#include <variant>

namespace AbeArgs {

typedef std::variant<bool, int, float, double, std::string> VarValue_t;

/// @brief Command line argument type info.
enum Argument_t : int
{
    /// @brief Default behavior
    NO_ARG = 0,
    /// @brief  A type of flag using dashes "-" and "--" (default).
    DASH_FLAG,
    /// @brief A type of flag using slashes "/".
    SLASH_FLAG,
    /// @brief A class of argument to enable/disable program behavior with a boolean type value.
    SWITCH,
    /// @brief A class of exclusive switch flag with no type value (its presence is a like a true boolean)
    ///        Exclusive means only the first one is handled.
    ///        app --version, or app --help.
    X_SWITCH,
    /// @brief A class of argument with a type & value (not for boolean - use a switch).
    OPTIONAL,
    /// @brief A required class of argument with a type & value.
    REQUIRED,
    /// @brief TODO: An argument that doesn't begin with a dash or slash, but
    ///        may further process other flags.
    // SUBCOMMAND,
    /// @brief A default value type.
    DEFAULT_VALUE_t,
    /// @brief No value for exclusive switch flags.
    NO_VALUE_t,
    /// @brief A string argument type.
    STRING_t,
    /// @brief A boolean argument type.
    BOOLEAN_t,
    /// @brief An integer argument type.
    INTEGER_t,
    /// @brief A float argument type.
    FLOAT_t,
    /// @brief A double argument type.
    DOUBLE_t,
    /// @brief A filename represented by a string type.
    FILE_t,
    /// @brief TODO: An IP address type.
    // IPADDR_TYPE,
};

class Argument
{
  public:
    Argument();
    Argument(Argument_t arg_class,
             int arg_ID,
             const std::string& short_flag_name = DEFAULT_SHORT_FLAG_NAME,
             const std::string& long_flag_name = DEFAULT_LONG_FLAG_NAME,
             const std::string& description = DEFAULT_FLAG_DESC,
             Argument_t value_type = Argument_t::DEFAULT_VALUE_t,
             size_t num_params = DEFAULT_NUM_FLAG_PARAMS);
    Argument(const Argument& rhs) = default; // Copy constructor
    Argument(Argument&& rhs) = default;      // Move constructor
    ~Argument() = default;

  public:
    Argument& operator=(const Argument& rhs) = default;

    int getID() const { return _arg_ID; }
    Argument_t getClass() const { return _class; }
    Argument_t getValueType() const { return _value_type; }

    VarValue_t getDefaultValue() const { return _default_value; }
    std::string getDefaultValueToString() const;
    Argument& setDefaultValue(VarValue_t value);
    bool hasDefaultValue() const { return _has_default_value; }

    bool isValidArg() const { return NO_ARG != _class; }
    bool isXSwitch() const { return X_SWITCH == _class; }
    bool isSwitch() const { return SWITCH == _class; }
    bool isOptional() const { return OPTIONAL == _class; }
    bool isRequired() const { return REQUIRED == _class; }

    size_t getNumParams() const { return _num_params; }

    void setFlagType(Argument_t flag_type);

    std::string getLongFlagChars() const { return _long_flag_chars; }
    std::string getShortFlagChars() const { return _short_flag_chars; }

    std::string getShortFlag() const;
    std::string getLongFlag() const;

    bool matchesDefaultFlag(const std::string& value) const;
    bool matchesFlag(const std::string& value) const;

    std::string toString() const;

  private:
    void initValueType();

  private:
    /// @brief The identifier for the argument.
    int _arg_ID = NO_ARG;

    /// @brief Whether this is a switch, optional, or required class of argument.
    Argument_t _class = NO_ARG;

    std::string _short_flag_name = DEFAULT_SHORT_FLAG_NAME;
    std::string _long_flag_name = DEFAULT_LONG_FLAG_NAME;
    std::string _description = DEFAULT_FLAG_DESC;

    /// @brief The argument's value type (boolean, integer, double, string, filename).
    Argument_t _value_type = DEFAULT_VALUE_t;

    bool _has_default_value = false;
    VarValue_t _default_value{ false };

    Argument_t _flag_type = DASH_FLAG;

    /// @brief The default way to signify a short flag is with a dash.
    std::string _short_flag_chars = DEFAULT_SHORT_DASH_CHARS;

    /// @brief The default way to signify a long flag is with dashes.
    std::string _long_flag_chars = DEFAULT_LONG_DASH_CHARS;

    /// @brief The number of params (0 or 1 for switch, 1 for optional and required).
    size_t _num_params = DEFAULT_NUM_FLAG_PARAMS;
};

} // namespace AbeArgs
