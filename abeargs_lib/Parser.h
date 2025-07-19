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
#include "Argument.h"
#include "Util.h"

// Standard includes
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace AbeArgs {

typedef std::vector<std::pair<int, VarValue_t>> ParsedArguments_t;
typedef std::vector<Argument> ArgumentList_t;
typedef std::pair<bool, bool> ValidBool_t;
typedef std::pair<bool, int> ValidInt_t;
typedef std::pair<bool, float> ValidFloat_t;
typedef std::pair<bool, double> ValidDouble_t;

class Parser
{
  public:
    Parser() = default;
    ~Parser() = default;

    Argument* addArgument(const Argument& p_arg);
    Argument& getArgument(int p_arg_ID);
    const Argument& getArgument(int p_arg_ID) const;
    Argument& getArgument(const std::string& p_flag);
    const ArgumentList_t& getArguments() const;

    ParsedArguments_t exec(int p_argc, char* p_argv[]);
    ParsedArguments_t exec(const std::string& p_argv);
    bool error() const;
    std::string getErrorMsg() const;
    bool isMissingRequiredArgs() const;

    bool hasArgvToken(int p_arg_ID) const;

  private:
    ValidBool_t getBoolean(const std::string& p_value) const;
    ValidInt_t getInteger(const std::string& p_value) const;
    ValidFloat_t getFloat(const std::string& p_value) const;
    ValidDouble_t getDouble(const std::string& p_value) const;

    bool fileExists(const char* p_file_path) const;

    void resetMissingArgs();

    void clearError();
    void setErrorMsg(const std::string& p_value);

  private:
    ArgumentList_t m_args;
    std::map<int, bool> m_required_args;
    Util::StringList m_argv_tokens;
    std::string m_error_msg;
};

} // namespace AbeArgs
