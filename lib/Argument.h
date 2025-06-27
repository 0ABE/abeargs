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
enum ArgumentType : int
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
    DEFAULT_VALUE_TYPE,
    /// @brief No value for exclusive switch flags.
    NO_VALUE_TYPE,
    /// @brief A string argument type.
    STRING_TYPE,
    /// @brief A boolean argument type.
    BOOLEAN_TYPE,
    /// @brief An integer argument type.
    INTEGER_TYPE,
    /// @brief A float argument type.
    FLOAT_TYPE,
    /// @brief A double argument type.
    DOUBLE_TYPE,
    /// @brief A filename represented by a string type.
    FILE_TYPE,
    /// @brief TODO: An IP address type.
    // IPADDR_TYPE,
};

class Argument
{
  public:
    Argument();
    Argument(ArgumentType p_arg_class,
             int p_arg_ID,
             const std::string& p_short_flag_name = DEFAULT_SHORT_FLAG_NAME,
             const std::string& p_long_flag_name = DEFAULT_LONG_FLAG_NAME,
             const std::string& p_description = DEFAULT_FLAG_DESC,
             ArgumentType p_value_type = ArgumentType::DEFAULT_VALUE_TYPE,
             size_t p_num_params = DEFAULT_NUM_FLAG_PARAMS);
    Argument(const Argument& p_rhs) = default; // Copy constructor
    Argument(Argument&& p_rhs) = default;      // Move constructor
    ~Argument() = default;

  public:
    Argument& operator=(const Argument& p_rhs) = default;

    int getID() const { return m_arg_ID; }
    ArgumentType getClass() const { return m_class; }
    ArgumentType getValueType() const { return m_value_type; }

    VarValue_t getDefaultValue() const { return m_default_value; }
    std::string getDefaultValueToString() const;
    Argument& setDefaultValue(VarValue_t p_value);
    bool hasDefaultValue() const { return m_has_default_value; }

    bool isValidArg() const { return NO_ARG != m_class; }
    bool isXSwitch() const { return X_SWITCH == m_class; }
    bool isSwitch() const { return SWITCH == m_class; }
    bool isOptional() const { return OPTIONAL == m_class; }
    bool isRequired() const { return REQUIRED == m_class; }

    size_t getNumParams() const { return m_num_params; }

    void setFlagType(ArgumentType flag_type);

    std::string getLongFlagChars() const { return m_long_flag_chars; }
    std::string getShortFlagChars() const { return m_short_flag_chars; }

    std::string getShortFlag() const;
    std::string getLongFlag() const;

    bool matchesDefaultFlag(const std::string& p_value) const;
    bool matchesFlag(const std::string& p_value) const;

    std::string toString() const;

  private:
    void initValueType();

  private:
    /// @brief The identifier for the argument.
    int m_arg_ID = NO_ARG;

    /// @brief Whether this is a switch, optional, or required class of argument.
    ArgumentType m_class = NO_ARG;

    std::string m_short_flag_name = DEFAULT_SHORT_FLAG_NAME;
    std::string m_long_flag_name = DEFAULT_LONG_FLAG_NAME;
    std::string m_description = DEFAULT_FLAG_DESC;

    /// @brief The argument's value type (boolean, integer, double, string, filename).
    ArgumentType m_value_type = DEFAULT_VALUE_TYPE;

    bool m_has_default_value = false;
    VarValue_t m_default_value{ false };

    ArgumentType m_flag_type = DASH_FLAG;

    /// @brief The default way to signify a short flag is with a dash.
    std::string m_short_flag_chars = DEFAULT_SHORT_DASH_CHARS;

    /// @brief The default way to signify a long flag is with dashes.
    std::string m_long_flag_chars = DEFAULT_LONG_DASH_CHARS;

    /// @brief The number of params (0 or 1 for switch, 1 for optional and required).
    size_t m_num_params = DEFAULT_NUM_FLAG_PARAMS;
};

} // namespace AbeArgs
