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

#include "ParserTests.h"

// Project includes
#include "../lib/abeargs.h"

// System includes
#include <iostream>

using namespace AbeArgs;
using namespace std;

void
ParserTests::testStrings1()
{
    const int STR_ID_1 = 1;

    Parser parser;
    parser.addArgument({ OPTIONAL, STR_ID_1, "1", "one", "Optional argument 1", STRING_t, 1 });

    ParsedArguments_t results;
    results = parser.exec("-1=This_is_a_long_string");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(STR_ID_1, results[0].first);
    CPPUNIT_ASSERT_EQUAL(string{ "This_is_a_long_string" }, get<string>(results[0].second));
}

void
ParserTests::testStrings2()
{
    const int STR_ID_1 = 1;

    Parser parser;
    parser.addArgument({ OPTIONAL, STR_ID_1, "1", "one", "Optional argument 1", STRING_t, 1 });

    ParsedArguments_t results;
    results = parser.exec("-1='This is =A= long string'");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(STR_ID_1, results[0].first);
    CPPUNIT_ASSERT_EQUAL(string{ "This is =A= long string" }, get<string>(results[0].second));
}

void
ParserTests::testStrings3()
{
    const int STR_ID_1 = 1;
    const int STR_ID_2 = 2;

    Parser parser;
    parser.addArgument({ OPTIONAL, STR_ID_1, "1", "one", "Optional argument 1", STRING_t, 1 });
    parser.addArgument({ OPTIONAL, STR_ID_2, "2", "two", "Optional argument 2", STRING_t, 1 });

    ParsedArguments_t results;
    results = parser.exec("-1='This is a long string' -2=Second");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(STR_ID_1, results[0].first);
    CPPUNIT_ASSERT_EQUAL(string{ "This is a long string" }, get<string>(results[0].second));
    CPPUNIT_ASSERT_EQUAL(string{ "Second" }, get<string>(results[1].second));
}

// Switches enable a feature (have a boolean effect with their presence).
void
ParserTests::testSwitch()
{
    const int ARG_ID_1 = 1;
    const int ARG_ID_2 = 2;

    Parser parser;
    parser.addArgument({ SWITCH, ARG_ID_1, "1", "one", "boolean flag one" });
    parser.addArgument({ SWITCH, ARG_ID_2, "2", "two", "boolean flag two" });

    ParsedArguments_t results;
    results = parser.exec("-1");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(ARG_ID_1, results[0].first);

    results = parser.exec("-2");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(ARG_ID_2, results[0].first);

    results = parser.exec("-1 -2");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(ARG_ID_1, results[0].first);
    CPPUNIT_ASSERT_EQUAL(true, get<bool>(results[0].second));
    CPPUNIT_ASSERT_EQUAL(ARG_ID_2, results[1].first);
    CPPUNIT_ASSERT_EQUAL(true, get<bool>(results[1].second));

    results = parser.exec("-3");
    CPPUNIT_ASSERT_EQUAL(true, parser.error());
    cout << __func__ << ": " << parser.getErrorMsg();
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(NO_ARG), results[0].first);
}

void
ParserTests::testXSwitch_short()
{
    const int ARG_ID = 1;

    Parser parser;
    parser.addArgument({ X_SWITCH, ARG_ID, "a", "about", "Show some info about the app" });

    ParsedArguments_t results;
    results = parser.exec("-a");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(ARG_ID, results[0].first);

    results = parser.exec("-i");
    CPPUNIT_ASSERT_EQUAL(true, parser.error());
    cout << __func__ << ": " << parser.getErrorMsg();
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(NO_ARG), results[0].first);
}

void
ParserTests::testXSwitch_long()
{
    const int ARG_ID = 1;

    Parser parser;
    parser.addArgument({ X_SWITCH, ARG_ID, "a", "about", "Show some info about the app" });

    ParsedArguments_t results;
    results = parser.exec("--about");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(ARG_ID, results[0].first);

    results = parser.exec("--info");
    CPPUNIT_ASSERT_EQUAL(true, parser.error());
    cout << __func__ << ": " << parser.getErrorMsg();
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(NO_ARG), results[0].first);
}

void
ParserTests::testSlashSwitch()
{
    const int ARG_ID = 1;

    Parser parser;
    parser.addArgument({ X_SWITCH, ARG_ID, "a", "about", "Show some info about the app" })->setFlagType(Argument_t::SLASH_FLAG);

    ParsedArguments_t results;
    results = parser.exec("/a");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(ARG_ID, results[0].first);

    results = parser.exec("/b");
    CPPUNIT_ASSERT_EQUAL(true, parser.error());
    cout << __func__ << ": " << parser.getErrorMsg();
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(NO_ARG), results[0].first);
}

void
ParserTests::testOptionalInteger1()
{
    const int OPT_ID_1 = 1;
    const int OPT_ID_2 = 2;

    Parser parser;
    parser.addArgument({ OPTIONAL, OPT_ID_1, "1", "one", "Optional argument 1", INTEGER_t, 1 });
    parser.addArgument({ OPTIONAL, OPT_ID_2, "2", "two", "Optional argument 2", INTEGER_t, 1 });

    ParsedArguments_t results;
    results = parser.exec("-1=1337");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(OPT_ID_1, results[0].first);
    CPPUNIT_ASSERT_EQUAL(1337, get<int>(results[0].second));

    results = parser.exec("-1=3.14");
    CPPUNIT_ASSERT_EQUAL(true, parser.error());
    cout << __func__ << ": " << parser.getErrorMsg();
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(NO_ARG), results[0].first);
}

void
ParserTests::testOptionalInteger2()
{
    const int OPT_ID_1 = 1;
    const int OPT_ID_2 = 2;
    const int OPT_ID_3 = 3;

    Parser parser;
    parser.addArgument({ OPTIONAL, OPT_ID_1, "1", "one", "Optional argument 1", INTEGER_t, 1 });
    parser.addArgument({ OPTIONAL, OPT_ID_2, "2", "two", "Optional argument 2", INTEGER_t, 2 });
    parser.addArgument({ OPTIONAL, OPT_ID_3, "3", "three", "Optional argument 3", INTEGER_t, 3 });

    ParsedArguments_t results;
    results = parser.exec("-1=1337");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(OPT_ID_1, results[0].first);
    CPPUNIT_ASSERT_EQUAL(1337, get<int>(results[0].second));

    results = parser.exec("-1=1337,1338");
    CPPUNIT_ASSERT_EQUAL(true, parser.error());
    cout << __func__ << ": " << parser.getErrorMsg();
    CPPUNIT_ASSERT_EQUAL(OPT_ID_1, results[0].first);
    CPPUNIT_ASSERT_EQUAL(1337, get<int>(results[0].second));

    results = parser.exec("-1=3.14");
    CPPUNIT_ASSERT_EQUAL(true, parser.error());
    cout << __func__ << ": " << parser.getErrorMsg();
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(NO_ARG), results[0].first);

    results = parser.exec("-2=1337,1338");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(OPT_ID_2, results[0].first);
    CPPUNIT_ASSERT_EQUAL(string("1337,1338"), get<string>(results[0].second));

    results = parser.exec("-2=1337,1338,1339");
    CPPUNIT_ASSERT_EQUAL(true, parser.error());
    cout << __func__ << ": " << parser.getErrorMsg();
    CPPUNIT_ASSERT_EQUAL(OPT_ID_2, results[0].first);
    CPPUNIT_ASSERT_EQUAL(string("1337,1338"), get<string>(results[0].second));

    results = parser.exec("-3=1337,1338,1339");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(OPT_ID_3, results[0].first);
    CPPUNIT_ASSERT_EQUAL(string("1337,1338,1339"), get<string>(results[0].second));

    results = parser.exec("-3=1337,13.38,1339");
    CPPUNIT_ASSERT_EQUAL(true, parser.error());
    cout << __func__ << ": " << parser.getErrorMsg();
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(NO_ARG), results[0].first);
}

void
ParserTests::testOptionalFloat1()
{
    const int OPT_ID_1 = 1;
    const int OPT_ID_2 = 2;
    const int OPT_ID_3 = 3;

    Parser parser;
    parser.addArgument({ OPTIONAL, OPT_ID_1, "1", "one", "Optional argument 1", FLOAT_t, 1 });
    parser.addArgument({ OPTIONAL, OPT_ID_2, "2", "two", "Optional argument 2", FLOAT_t, 2 });
    parser.addArgument({ OPTIONAL, OPT_ID_3, "3", "three", "Optional argument 3", FLOAT_t, 3 });

    ParsedArguments_t results;
    results = parser.exec("-1=13.37");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(OPT_ID_1, results[0].first);
    CPPUNIT_ASSERT_EQUAL((float)13.37, get<float>(results[0].second));

    results = parser.exec("-1=13.37,13.38");
    CPPUNIT_ASSERT_EQUAL(true, parser.error());
    cout << __func__ << ": " << parser.getErrorMsg();
    CPPUNIT_ASSERT_EQUAL(OPT_ID_1, results[0].first);
    CPPUNIT_ASSERT_EQUAL((float)13.37, get<float>(results[0].second));

    results = parser.exec("-1=1337");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(OPT_ID_1, results[0].first);
    CPPUNIT_ASSERT_EQUAL((float)1337, get<float>(results[0].second));

    results = parser.exec("-2=1337,1338");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(OPT_ID_2, results[0].first);
    CPPUNIT_ASSERT_EQUAL(string("1337,1338"), get<string>(results[0].second));

    results = parser.exec("-2=1337,1338,1339");
    CPPUNIT_ASSERT_EQUAL(true, parser.error());
    cout << __func__ << ": " << parser.getErrorMsg();
    CPPUNIT_ASSERT_EQUAL(OPT_ID_2, results[0].first);
    CPPUNIT_ASSERT_EQUAL(string("1337,1338"), get<string>(results[0].second));

    results = parser.exec("-3=1337,1338,1339");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(OPT_ID_3, results[0].first);
    CPPUNIT_ASSERT_EQUAL(string("1337,1338,1339"), get<string>(results[0].second));

    results = parser.exec("-3=1337,13.38,1339");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(OPT_ID_3, results[0].first);
    CPPUNIT_ASSERT_EQUAL(string("1337,13.38,1339"), get<string>(results[0].second));
}

void
ParserTests::testOptionalDouble1()
{
    const int OPT_ID_1 = 1;
    const int OPT_ID_2 = 2;
    const int OPT_ID_3 = 3;

    Parser parser;
    parser.addArgument({ OPTIONAL, OPT_ID_1, "1", "one", "Optional argument 1", DOUBLE_t, 1 });
    parser.addArgument({ OPTIONAL, OPT_ID_2, "2", "two", "Optional argument 2", DOUBLE_t, 2 });
    parser.addArgument({ OPTIONAL, OPT_ID_3, "3", "three", "Optional argument 3", DOUBLE_t, 3 });

    ParsedArguments_t results;
    results = parser.exec("-1=13.37");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(OPT_ID_1, results[0].first);
    CPPUNIT_ASSERT_EQUAL((double)13.37, get<double>(results[0].second));

    results = parser.exec("-1=13.37,13.38");
    CPPUNIT_ASSERT_EQUAL(true, parser.error());
    cout << __func__ << ": " << parser.getErrorMsg();
    CPPUNIT_ASSERT_EQUAL(OPT_ID_1, results[0].first);
    CPPUNIT_ASSERT_EQUAL((double)13.37, get<double>(results[0].second));

    results = parser.exec("-1=1337");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(OPT_ID_1, results[0].first);
    CPPUNIT_ASSERT_EQUAL((double)1337, get<double>(results[0].second));

    results = parser.exec("-2=1337,1338");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(OPT_ID_2, results[0].first);
    CPPUNIT_ASSERT_EQUAL(string("1337,1338"), get<string>(results[0].second));

    results = parser.exec("-2=1337,1338,1339");
    CPPUNIT_ASSERT_EQUAL(true, parser.error());
    cout << __func__ << ": " << parser.getErrorMsg();
    CPPUNIT_ASSERT_EQUAL(OPT_ID_2, results[0].first);
    CPPUNIT_ASSERT_EQUAL(string("1337,1338"), get<string>(results[0].second));

    results = parser.exec("-3=1337,1338,1339");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(OPT_ID_3, results[0].first);
    CPPUNIT_ASSERT_EQUAL(string("1337,1338,1339"), get<string>(results[0].second));

    results = parser.exec("-3=1337,13.38,1339");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(OPT_ID_3, results[0].first);
    CPPUNIT_ASSERT_EQUAL(string("1337,13.38,1339"), get<string>(results[0].second));
}

void
ParserTests::testRequiredInteger1()
{
    const int REQ_ID_1 = 1;
    const int OPT_ID_2 = 2;

    Parser parser;
    parser.addArgument({ REQUIRED, REQ_ID_1, "1", "one", "Required argument 1", INTEGER_t, 1 });
    parser.addArgument({ OPTIONAL, OPT_ID_2, "2", "two", "Optional argument 2", INTEGER_t, 2 });

    ParsedArguments_t results;
    results = parser.exec("-1=10");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(REQ_ID_1, results[0].first);
    CPPUNIT_ASSERT_EQUAL((int)10, get<int>(results[0].second));
    CPPUNIT_ASSERT_EQUAL(false, parser.isMissingRequiredArgs());

    results = parser.exec("--two=1337,1338");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(OPT_ID_2, results[0].first);
    CPPUNIT_ASSERT_EQUAL(string("1337,1338"), get<string>(results[0].second));
    CPPUNIT_ASSERT_EQUAL(true, parser.isMissingRequiredArgs());
    CPPUNIT_ASSERT_EQUAL(false, parser.hasArgvToken(REQ_ID_1));

    CPPUNIT_ASSERT_EQUAL(true, parser.hasArgvToken(OPT_ID_2));
}

void
ParserTests::testArgPrint()
{
    const int ARG_ID_1 = 1;
    const int ARG_ID_2 = 2;
    const int ARG_ID_3 = 3;

    Parser parser;
    parser.addArgument({ X_SWITCH, ARG_ID_1, "a", DEFAULT_LONG_FLAG_NAME, "Show some info about the app" });
    parser.addArgument({ X_SWITCH, ARG_ID_2, "h", "help", "Show this info" });
    parser.addArgument({ X_SWITCH, ARG_ID_3, "v", "version", "Show version info" });

    cout << "\n-----\n";
    ArgumentList_t list = parser.getArguments();
    for (const auto& arg : list)
        cout << arg.toString() << '\n';

    cout << "-----\n";
}

void
ParserTests::testMissingLongDashFlag()
{
    const int ARG_ID_1 = 1;

    Parser parser;
    parser.addArgument({ X_SWITCH, ARG_ID_1, "a", DEFAULT_LONG_FLAG_NAME, "Show some info about the app" });

    ParsedArguments_t results;
    results = parser.exec("-a");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(ARG_ID_1, results[0].first);

    results = parser.exec("--default_long_flag");
    CPPUNIT_ASSERT_EQUAL(true, parser.error());
    cout << __func__ << ": " << parser.getErrorMsg();
}

void
ParserTests::testMissingLongSlashFlag()
{
    const int ARG_ID_1 = 1;

    Parser parser;
    parser.addArgument({ X_SWITCH, ARG_ID_1, "a", DEFAULT_LONG_FLAG_NAME, "Show some info about the app" })->setFlagType(SLASH_FLAG);

    ParsedArguments_t results;
    results = parser.exec("/a");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(ARG_ID_1, results[0].first);

    results = parser.exec("/default_long_flag");
    CPPUNIT_ASSERT_EQUAL(true, parser.error());
    cout << __func__ << ": " << parser.getErrorMsg();
}

void
ParserTests::testMissingShortDashFlag()
{
    const int ARG_ID_1 = 1;

    Parser parser;
    parser.addArgument({ X_SWITCH, ARG_ID_1, DEFAULT_SHORT_FLAG_NAME, "about", "Show some info about the app" });

    ParsedArguments_t results;
    results = parser.exec("--about");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(ARG_ID_1, results[0].first);

    results = parser.exec("--default_short_flag");
    CPPUNIT_ASSERT_EQUAL(true, parser.error());
    cout << __func__ << ": " << parser.getErrorMsg();
}

void
ParserTests::testMissingShortSlashFlag()
{
    const int ARG_ID_1 = 1;

    Parser parser;
    parser.addArgument({ X_SWITCH, ARG_ID_1, DEFAULT_SHORT_FLAG_NAME, "about", "Show some info about the app" })->setFlagType(SLASH_FLAG);

    ParsedArguments_t results;
    results = parser.exec("/about");
    CPPUNIT_ASSERT_EQUAL(false, parser.error());
    CPPUNIT_ASSERT_EQUAL(ARG_ID_1, results[0].first);

    results = parser.exec("/default_short_flag");
    CPPUNIT_ASSERT_EQUAL(true, parser.error());
    cout << __func__ << ": " << parser.getErrorMsg();
}

void
ParserTests::testDefaultValues1()
{
    const int REQ_ID_1 = 1;
    const int OPT_ID_2 = 2;

    Parser parser;
    parser.addArgument({ REQUIRED, REQ_ID_1, "1", "one", "Required argument 1", INTEGER_t, 1 })->setDefaultValue(1);
    parser.addArgument({ OPTIONAL, OPT_ID_2, "2", "two", "Optional argument 2", INTEGER_t, 2 });

    CPPUNIT_ASSERT_EQUAL(1, get<int>(parser.getArgument(REQ_ID_1).getDefaultValue()));

    // parser.getArgument(OPT_ID_2).setDefaultValue("42,43");
    // CPPUNIT_ASSERT_EQUAL(string("42,43"), get<string>(parser.getArgument(OPT_ID_2).getDefaultValue()));

    cout << "\n-----\n";
    ArgumentList_t list = parser.getArguments();
    for (int i = 0, n = list.size(); i < n; ++i) {
        cout << list[i].toString() << '\n';
        if (i < n - 1)
            cout << '\n';
    }
    cout << "-----\n";
}
