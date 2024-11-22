#pragma once

namespace cli_tests {
    void testCreateSequenceCommand();
    void testPrintSequenceCommand();
    void testAppendSequenceCommand();
    void testPrependSequenceCommand();
    void testInsertSequenceCommand();
    void testGetLengthCommand();
    void testGetSubsequenceCommand();
    void testGetSequenceElementCommand();
    void testTryGetSequenceElementCommand();
    void testConcatSequenceCommand();

    void testCreateSequenceWithSameName();
    void testAppendSequenceInvalidName();
    void testPrependSequenceInvalidName();
    void testInsertSequenceInvalidName();
    void testGetLengthInvalidName();
    void testGetSubsequenceInvalidName();
    void testGetSequenceElementInvalidName();
    void testTryGetSequenceElementInvalidName();
    void testConcatSequenceInvalidName();

    void testInsertSequenceInvalidIndex();
    void testGetSubsequenceInvalidIndex();
    void testGetSequenceElementInvalidIndex();
    void testTryGetSequenceElementInvalidIndex();
}