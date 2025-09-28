#include "Tokenizer.h"
#include "../Executor/Executor.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <string>

// Simple test framework macros for this project
#define TEST(name) void test_##name()
#define ASSERT_EQ(expected, actual) \
    do { \
        if ((expected) \!= (actual)) { \
            std::cerr << "ASSERTION FAILED: " << #expected << " == " << #actual \
                      << " (expected: " << (expected) << ", actual: " << (actual) << ")" << std::endl; \
            exit(1); \
        } \
    } while(0)

#define ASSERT_TRUE(condition) \
    do { \
        if (\!(condition)) { \
            std::cerr << "ASSERTION FAILED: " << #condition << " should be true" << std::endl; \
            exit(1); \
        } \
    } while(0)

#define ASSERT_FALSE(condition) \
    do { \
        if (condition) { \
            std::cerr << "ASSERTION FAILED: " << #condition << " should be false" << std::endl; \
            exit(1); \
        } \
    } while(0)

#define RUN_TEST(name) \
    do { \
        std::cout << "Running " << #name << "..." << std::endl; \
        test_##name(); \
        std::cout << #name << " PASSED" << std::endl; \
    } while(0)

// Helper function to clear static state between tests
void clearTokenizerState() {
    Tokenizer::m_Strings.clear();
}

// Test ReadLines functionality

TEST(ReadLines_EmptyInput) {
    clearTokenizerState();
    std::vector<std::string> input = {};
    auto result = Tokenizer::ReadLines(input);
    ASSERT_TRUE(result.empty());
    ASSERT_TRUE(Tokenizer::m_Strings.empty());
}

TEST(ReadLines_SingleEmptyLine) {
    clearTokenizerState();
    std::vector<std::string> input = {""};
    auto result = Tokenizer::ReadLines(input);
    ASSERT_TRUE(result.empty());
    ASSERT_TRUE(Tokenizer::m_Strings.empty());
}

TEST(ReadLines_SingleWordPerLine) {
    clearTokenizerState();
    std::vector<std::string> input = {"PRINT", "END"};
    auto result = Tokenizer::ReadLines(input);
    ASSERT_EQ(2, result.size());
    ASSERT_EQ("PRINT", result[0]);
    ASSERT_EQ("END", result[1]);
}

TEST(ReadLines_MultipleWordsPerLine) {
    clearTokenizerState();
    std::vector<std::string> input = {"LET x 5", "PRINT END"};
    auto result = Tokenizer::ReadLines(input);
    ASSERT_EQ(5, result.size());
    ASSERT_EQ("LET", result[0]);
    ASSERT_EQ("x", result[1]);
    ASSERT_EQ("5", result[2]);
    ASSERT_EQ("PRINT", result[3]);
    ASSERT_EQ("END", result[4]);
}

TEST(ReadLines_StringLiteralBasic) {
    clearTokenizerState();
    std::vector<std::string> input = {"PRINT \"Hello World\""};
    auto result = Tokenizer::ReadLines(input);
    ASSERT_EQ(1, result.size());
    ASSERT_EQ("PRINT", result[0]);
    ASSERT_EQ(1, Tokenizer::m_Strings.size());
    ASSERT_EQ("Hello World", Tokenizer::m_Strings[0]);
}

TEST(ReadLines_StringLiteralEmpty) {
    clearTokenizerState();
    std::vector<std::string> input = {"PRINT \"\""};
    auto result = Tokenizer::ReadLines(input);
    ASSERT_EQ(1, result.size());
    ASSERT_EQ("PRINT", result[0]);
    ASSERT_EQ(1, Tokenizer::m_Strings.size());
    ASSERT_EQ("", Tokenizer::m_Strings[0]);
}

TEST(ReadLines_StringLiteralWithSpaces) {
    clearTokenizerState();
    std::vector<std::string> input = {"PRINT \"  Hello   World  \""};
    auto result = Tokenizer::ReadLines(input);
    ASSERT_EQ(1, result.size());
    ASSERT_EQ("PRINT", result[0]);
    ASSERT_EQ(1, Tokenizer::m_Strings.size());
    ASSERT_EQ("  Hello   World  ", Tokenizer::m_Strings[0]);
}

TEST(ReadLines_MultipleStringLiterals) {
    clearTokenizerState();
    std::vector<std::string> input = {"PRINT \"First\" \"Second\""};
    auto result = Tokenizer::ReadLines(input);
    ASSERT_EQ(1, result.size());
    ASSERT_EQ("PRINT", result[0]);
    ASSERT_EQ(2, Tokenizer::m_Strings.size());
    ASSERT_EQ("First", Tokenizer::m_Strings[0]);
    ASSERT_EQ("Second", Tokenizer::m_Strings[1]);
}

TEST(ReadLines_MixedTokensAndStrings) {
    clearTokenizerState();
    std::vector<std::string> input = {"LET name \"John\" age 25"};
    auto result = Tokenizer::ReadLines(input);
    ASSERT_EQ(4, result.size());
    ASSERT_EQ("LET", result[0]);
    ASSERT_EQ("name", result[1]);
    ASSERT_EQ("age", result[2]);
    ASSERT_EQ("25", result[3]);
    ASSERT_EQ(1, Tokenizer::m_Strings.size());
    ASSERT_EQ("John", Tokenizer::m_Strings[0]);
}

TEST(ReadLines_UnterminatedString) {
    clearTokenizerState();
    std::vector<std::string> input = {"PRINT \"Hello World"};
    auto result = Tokenizer::ReadLines(input);
    ASSERT_EQ(1, result.size());
    ASSERT_EQ("PRINT", result[0]);
    ASSERT_EQ(1, Tokenizer::m_Strings.size());
    ASSERT_EQ("Hello World", Tokenizer::m_Strings[0]);
}

TEST(ReadLines_StringAtEndOfLine) {
    clearTokenizerState();
    std::vector<std::string> input = {"PRINT \"End of line\""};
    auto result = Tokenizer::ReadLines(input);
    ASSERT_EQ(1, result.size());
    ASSERT_EQ("PRINT", result[0]);
    ASSERT_EQ(1, Tokenizer::m_Strings.size());
    ASSERT_EQ("End of line", Tokenizer::m_Strings[0]);
}

TEST(ReadLines_ConsecutiveSpaces) {
    clearTokenizerState();
    std::vector<std::string> input = {"PRINT    END     LET"};
    auto result = Tokenizer::ReadLines(input);
    ASSERT_EQ(3, result.size());
    ASSERT_EQ("PRINT", result[0]);
    ASSERT_EQ("END", result[1]);
    ASSERT_EQ("LET", result[2]);
}

TEST(ReadLines_LeadingAndTrailingSpaces) {
    clearTokenizerState();
    std::vector<std::string> input = {"   PRINT END   "};
    auto result = Tokenizer::ReadLines(input);
    ASSERT_EQ(2, result.size());
    ASSERT_EQ("PRINT", result[0]);
    ASSERT_EQ("END", result[1]);
}

TEST(ReadLines_ComplexMultilineInput) {
    clearTokenizerState();
    std::vector<std::string> input = {
        "LET x \"Hello\"",
        "LET y 42",
        "PRINT \"Result is\" x y",
        "END"
    };
    auto result = Tokenizer::ReadLines(input);
    ASSERT_EQ(9, result.size());
    ASSERT_EQ("LET", result[0]);
    ASSERT_EQ("x", result[1]);
    ASSERT_EQ("LET", result[2]);
    ASSERT_EQ("y", result[3]);
    ASSERT_EQ("42", result[4]);
    ASSERT_EQ("PRINT", result[5]);
    ASSERT_EQ("x", result[6]);
    ASSERT_EQ("y", result[7]);
    ASSERT_EQ("END", result[8]);
    ASSERT_EQ(2, Tokenizer::m_Strings.size());
    ASSERT_EQ("Hello", Tokenizer::m_Strings[0]);
    ASSERT_EQ("Result is", Tokenizer::m_Strings[1]);
}

// Test ReadTokens functionality

TEST(ReadTokens_EmptyTokens) {
    clearTokenizerState();
    std::vector<std::string> tokens = {};
    bool result = Tokenizer::ReadTokens(tokens);
    ASSERT_TRUE(result);
}

TEST(ReadTokens_PrintCommand) {
    clearTokenizerState();
    Tokenizer::m_Strings.push_back("Hello World");
    std::vector<std::string> tokens = {"PRINT"};
    bool result = Tokenizer::ReadTokens(tokens);
    ASSERT_TRUE(result);
}

TEST(ReadTokens_EndCommand) {
    clearTokenizerState();
    std::vector<std::string> tokens = {"END"};
    bool result = Tokenizer::ReadTokens(tokens);
    ASSERT_TRUE(result);
}

TEST(ReadTokens_LetCommandValid) {
    clearTokenizerState();
    std::vector<std::string> tokens = {"LET", "x", "5"};
    bool result = Tokenizer::ReadTokens(tokens);
    ASSERT_TRUE(result);
}

TEST(ReadTokens_LetCommandIncomplete) {
    clearTokenizerState();
    std::vector<std::string> tokens = {"LET", "x"};
    bool result = Tokenizer::ReadTokens(tokens);
    ASSERT_TRUE(result); // Should not crash, just skip incomplete command
}

TEST(ReadTokens_LetCommandMissingValue) {
    clearTokenizerState();
    std::vector<std::string> tokens = {"LET"};
    bool result = Tokenizer::ReadTokens(tokens);
    ASSERT_TRUE(result); // Should not crash, just skip incomplete command
}

TEST(ReadTokens_MultipleCommands) {
    clearTokenizerState();
    Tokenizer::m_Strings.push_back("Test message");
    std::vector<std::string> tokens = {"LET", "var", "value", "PRINT", "END"};
    bool result = Tokenizer::ReadTokens(tokens);
    ASSERT_TRUE(result);
}

TEST(ReadTokens_UnknownCommand) {
    clearTokenizerState();
    std::vector<std::string> tokens = {"UNKNOWN_COMMAND", "param"};
    bool result = Tokenizer::ReadTokens(tokens);
    ASSERT_TRUE(result); // Should handle gracefully
}

TEST(ReadTokens_MixedValidAndInvalidCommands) {
    clearTokenizerState();
    Tokenizer::m_Strings.push_back("Valid print");
    std::vector<std::string> tokens = {"PRINT", "INVALID", "LET", "x", "1", "END"};
    bool result = Tokenizer::ReadTokens(tokens);
    ASSERT_TRUE(result);
}

// Integration tests combining ReadLines and ReadTokens

TEST(Integration_SimpleProgram) {
    clearTokenizerState();
    std::vector<std::string> lines = {"LET x 42", "PRINT \"Value is\"", "END"};
    auto tokens = Tokenizer::ReadLines(lines);
    bool result = Tokenizer::ReadTokens(tokens);
    ASSERT_TRUE(result);
    ASSERT_EQ(1, Tokenizer::m_Strings.size());
    ASSERT_EQ("Value is", Tokenizer::m_Strings[0]);
}

TEST(Integration_ComplexProgram) {
    clearTokenizerState();
    std::vector<std::string> lines = {
        "LET name \"Alice\"",
        "LET age 30",
        "PRINT \"Hello\" name",
        "LET result \"Done\"",
        "END"
    };
    auto tokens = Tokenizer::ReadLines(lines);
    bool result = Tokenizer::ReadTokens(tokens);
    ASSERT_TRUE(result);
    ASSERT_EQ(3, Tokenizer::m_Strings.size());
    ASSERT_EQ("Alice", Tokenizer::m_Strings[0]);
    ASSERT_EQ("Hello", Tokenizer::m_Strings[1]);
    ASSERT_EQ("Done", Tokenizer::m_Strings[2]);
}

// Edge case tests

TEST(EdgeCase_OnlySpaces) {
    clearTokenizerState();
    std::vector<std::string> input = {"     "};
    auto result = Tokenizer::ReadLines(input);
    ASSERT_TRUE(result.empty());
}

TEST(EdgeCase_OnlyQuotes) {
    clearTokenizerState();
    std::vector<std::string> input = {"\"\""};
    auto result = Tokenizer::ReadLines(input);
    ASSERT_TRUE(result.empty());
    ASSERT_EQ(1, Tokenizer::m_Strings.size());
    ASSERT_EQ("", Tokenizer::m_Strings[0]);
}

TEST(EdgeCase_StringWithQuoteCharacters) {
    clearTokenizerState();
    std::vector<std::string> input = {"PRINT \"She said hello\""};
    auto result = Tokenizer::ReadLines(input);
    ASSERT_EQ(1, result.size());
    ASSERT_EQ("PRINT", result[0]);
    ASSERT_EQ(1, Tokenizer::m_Strings.size());
    ASSERT_EQ("She said hello", Tokenizer::m_Strings[0]);
}

TEST(EdgeCase_VeryLongLine) {
    clearTokenizerState();
    std::string longString(1000, 'a');
    std::vector<std::string> input = {"PRINT \"" + longString + "\""};
    auto result = Tokenizer::ReadLines(input);
    ASSERT_EQ(1, result.size());
    ASSERT_EQ("PRINT", result[0]);
    ASSERT_EQ(1, Tokenizer::m_Strings.size());
    ASSERT_EQ(longString, Tokenizer::m_Strings[0]);
}

TEST(EdgeCase_ManyTokensOnOneLine) {
    clearTokenizerState();
    std::vector<std::string> input = {"A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"};
    auto result = Tokenizer::ReadLines(input);
    ASSERT_EQ(26, result.size());
    for (int i = 0; i < 26; ++i) {
        ASSERT_EQ(std::string(1, 'A' + i), result[i]);
    }
}

// Performance and stress tests

TEST(Stress_ManyLines) {
    clearTokenizerState();
    std::vector<std::string> input;
    for (int i = 0; i < 100; ++i) {
        input.push_back("LET var" + std::to_string(i) + " " + std::to_string(i));
    }
    auto result = Tokenizer::ReadLines(input);
    ASSERT_EQ(300, result.size()); // 3 tokens per line * 100 lines
}

TEST(Stress_ManyStrings) {
    clearTokenizerState();
    std::vector<std::string> input;
    for (int i = 0; i < 50; ++i) {
        input.push_back("PRINT \"String " + std::to_string(i) + "\"");
    }
    auto result = Tokenizer::ReadLines(input);
    ASSERT_EQ(50, result.size()); // 1 PRINT token per line
    ASSERT_EQ(50, Tokenizer::m_Strings.size());
    for (int i = 0; i < 50; ++i) {
        ASSERT_EQ("String " + std::to_string(i), Tokenizer::m_Strings[i]);
    }
}

// Main test runner
int main() {
    std::cout << "Running Tokenizer Tests..." << std::endl;
    
    // ReadLines tests
    RUN_TEST(ReadLines_EmptyInput);
    RUN_TEST(ReadLines_SingleEmptyLine);
    RUN_TEST(ReadLines_SingleWordPerLine);
    RUN_TEST(ReadLines_MultipleWordsPerLine);
    RUN_TEST(ReadLines_StringLiteralBasic);
    RUN_TEST(ReadLines_StringLiteralEmpty);
    RUN_TEST(ReadLines_StringLiteralWithSpaces);
    RUN_TEST(ReadLines_MultipleStringLiterals);
    RUN_TEST(ReadLines_MixedTokensAndStrings);
    RUN_TEST(ReadLines_UnterminatedString);
    RUN_TEST(ReadLines_StringAtEndOfLine);
    RUN_TEST(ReadLines_ConsecutiveSpaces);
    RUN_TEST(ReadLines_LeadingAndTrailingSpaces);
    RUN_TEST(ReadLines_ComplexMultilineInput);
    
    // ReadTokens tests
    RUN_TEST(ReadTokens_EmptyTokens);
    RUN_TEST(ReadTokens_PrintCommand);
    RUN_TEST(ReadTokens_EndCommand);
    RUN_TEST(ReadTokens_LetCommandValid);
    RUN_TEST(ReadTokens_LetCommandIncomplete);
    RUN_TEST(ReadTokens_LetCommandMissingValue);
    RUN_TEST(ReadTokens_MultipleCommands);
    RUN_TEST(ReadTokens_UnknownCommand);
    RUN_TEST(ReadTokens_MixedValidAndInvalidCommands);
    
    // Integration tests
    RUN_TEST(Integration_SimpleProgram);
    RUN_TEST(Integration_ComplexProgram);
    
    // Edge case tests
    RUN_TEST(EdgeCase_OnlySpaces);
    RUN_TEST(EdgeCase_OnlyQuotes);
    RUN_TEST(EdgeCase_StringWithQuoteCharacters);
    RUN_TEST(EdgeCase_VeryLongLine);
    RUN_TEST(EdgeCase_ManyTokensOnOneLine);
    
    // Stress tests
    RUN_TEST(Stress_ManyLines);
    RUN_TEST(Stress_ManyStrings);
    
    std::cout << "\nAll tests passed successfully\!" << std::endl;
    return 0;
}