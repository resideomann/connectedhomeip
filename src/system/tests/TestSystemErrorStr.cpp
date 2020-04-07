/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2016-2017 Nest Labs, Inc.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 *    @file
 *      This file implements a process to effect a functional test for
 *      the CHIP System layer library error string support interfaces.
 *
 */

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#include <inttypes.h>
#include <stdint.h>
#include <string.h>

#include <inet/InetError.h>
#include <support/ErrorStr.h>

#include <nlunit-test.h>

using namespace chip;

// Test input data.

// clang-format off
static int32_t sContext[] =
{
    CHIP_SYSTEM_ERROR_NOT_IMPLEMENTED,
    CHIP_SYSTEM_ERROR_NOT_SUPPORTED,
    CHIP_SYSTEM_ERROR_BAD_ARGS,
    CHIP_SYSTEM_ERROR_UNEXPECTED_STATE,
    CHIP_SYSTEM_ERROR_UNEXPECTED_EVENT,
    CHIP_SYSTEM_ERROR_NO_MEMORY,
    CHIP_SYSTEM_ERROR_REAL_TIME_NOT_SYNCED,
    CHIP_SYSTEM_ERROR_ACCESS_DENIED
};
// clang-format on

static const size_t kTestElements = sizeof(sContext) / sizeof(sContext[0]);

static void CheckSystemErrorStr(nlTestSuite * inSuite, void * inContext)
{
    // Register the layer error formatter

    System::RegisterLayerErrorFormatter();

    // For each defined error...
    for (size_t i = 0; i < kTestElements; i++)
    {
        int32_t err         = sContext[i];
        const char * errStr = ErrorStr(err);
        char expectedText[9];

        // Assert that the error string contains the error number in hex.
        snprintf(expectedText, sizeof(expectedText), "%08" PRIX32, err);
        NL_TEST_ASSERT(inSuite, (strstr(errStr, expectedText) != NULL));

#if !CHIP_CONFIG_SHORT_ERROR_STR
        // Assert that the error string contains a description, which is signaled
        // by a presence of a colon proceeding the description.
        NL_TEST_ASSERT(inSuite, (strchr(errStr, ':') != NULL));
#endif // !CHIP_CONFIG_SHORT_ERROR_STR
    }
}

/**
 *   Test Suite. It lists all the test functions.
 */

// clang-format off
static const nlTest sTests[] =
{
    NL_TEST_DEF("SystemErrorStr", CheckSystemErrorStr),

    NL_TEST_SENTINEL()
};
// clang-format on

int main(void)
{
    // clang-format off
    nlTestSuite theSuite =
	{
        "System-Error-Strings",
        &sTests[0],
        NULL,
        NULL
    };
    // clang-format on

    // Generate machine-readable, comma-separated value (CSV) output.
    nl_test_set_output_style(OUTPUT_CSV);

    // Run test suit againt one context.
    nlTestRunner(&theSuite, &sContext);

    return nlTestRunnerStats(&theSuite);
}