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

// CppUnit includes
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class ParserTests : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ParserTests);
    CPPUNIT_TEST(testStrings1);
    CPPUNIT_TEST(testStrings2);
    CPPUNIT_TEST(testStrings3);

    CPPUNIT_TEST(testSwitch);
    CPPUNIT_TEST(testXSwitch_short);
    CPPUNIT_TEST(testXSwitch_long);
    CPPUNIT_TEST(testSlashSwitch);
    CPPUNIT_TEST(testOptionalInteger1);
    CPPUNIT_TEST(testOptionalInteger2);
    CPPUNIT_TEST(testOptionalFloat1);
    CPPUNIT_TEST(testOptionalDouble1);
    CPPUNIT_TEST(testRequiredInteger1);
    CPPUNIT_TEST(testArgPrint);
    CPPUNIT_TEST(testMissingLongDashFlag);
    CPPUNIT_TEST(testMissingLongSlashFlag);
    CPPUNIT_TEST(testMissingShortDashFlag);
    CPPUNIT_TEST(testMissingShortSlashFlag);
    CPPUNIT_TEST(testDefaultValues1);
    CPPUNIT_TEST_SUITE_END();

  public:
    void testStrings1();
    void testStrings2();
    void testStrings3();
    void testSwitch();
    void testXSwitch_short();
    void testXSwitch_long();
    void testSlashSwitch();
    void testOptionalInteger1();
    void testOptionalInteger2();
    void testOptionalFloat1();
    void testOptionalDouble1();
    void testRequiredInteger1();
    void testArgPrint();
    void testMissingLongDashFlag();
    void testMissingLongSlashFlag();
    void testMissingShortDashFlag();
    void testMissingShortSlashFlag();
    void testDefaultValues1();
};

CPPUNIT_TEST_SUITE_REGISTRATION(ParserTests);
