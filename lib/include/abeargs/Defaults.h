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
#include <string>

namespace AbeArgs {

static const int DEFAULT_NUM_FLAG_PARAMS = 0;

static const std::string DEFAULT_STR = "default_str";
static const std::string DEFAULT_SHORT_DASH_CHARS = "-";
static const std::string DEFAULT_LONG_DASH_CHARS = "--";
static const std::string DEFAULT_SHORT_SLASH_CHARS = "/";
static const std::string DEFAULT_LONG_SLASH_CHARS = "/";
static const std::string DEFAULT_FLAG_DESC = "default_flag_desc";
static const std::string DEFAULT_LONG_FLAG_NAME = "default_long_flag";
static const std::string DEFAULT_SHORT_FLAG_NAME = "default_short_flag";

} // namespace AbeArgs
