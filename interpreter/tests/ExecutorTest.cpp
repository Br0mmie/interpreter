#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/Executor.h"
#include <sstream>
#include <iostream>
#include <string>
#include <unordered_map>

// Test fixture for Executor tests
class ExecutorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Capture cout and printf output for testing
        original_cout_buffer = std::cout.rdbuf();
        original_stdout = stdout;
        
        // Redirect cout to string stream
        std::cout.rdbuf(cout_buffer.rdbuf());
        
        // Redirect stdout to a file for printf capture
        stdout = freopen("test_output.txt", "w", stdout);
    }
    
    void TearDown() override {
        // Restore original streams
        std::cout.rdbuf(original_cout_buffer);
        if (stdout \!= original_stdout) {
            fclose(stdout);
            stdout = original_stdout;
        }
        
        // Clean up test output file
        remove("test_output.txt");
    }
    
    std::string getCoutOutput() {
        std::cout.flush();
        return cout_buffer.str();
    }
    
    std::string getPrintfOutput() {
        fflush(stdout);
        std::ifstream file("test_output.txt");
        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        return content;
    }
    
    void clearOutput() {
        cout_buffer.str("");
        cout_buffer.clear();
        if (stdout) {
            fclose(stdout);
            stdout = freopen("test_output.txt", "w", stdout);
        }
    }
    
private:
    std::stringstream cout_buffer;
    std::streambuf* original_cout_buffer;
    FILE* original_stdout;
};

// Tests for print() function
TEST_F(ExecutorTest, PrintEmptyString) {
    Executor executor;
    
    bool result = executor.print("");
    
    EXPECT_TRUE(result);
    std::string output = getPrintfOutput();
    EXPECT_EQ(output, "\n");
}

TEST_F(ExecutorTest, PrintSimpleString) {
    Executor executor;
    
    bool result = executor.print("Hello World");
    
    EXPECT_TRUE(result);
    std::string output = getPrintfOutput();
    EXPECT_EQ(output, "Hello World\n");
}

TEST_F(ExecutorTest, PrintStringWithSpecialCharacters) {
    Executor executor;
    
    bool result = executor.print("Hello\tWorld\n");
    
    EXPECT_TRUE(result);
    std::string output = getPrintfOutput();
    EXPECT_EQ(output, "Hello\tWorld\n\n");
}

TEST_F(ExecutorTest, PrintStringWithNumbers) {
    Executor executor;
    
    bool result = executor.print("Count: 123");
    
    EXPECT_TRUE(result);
    std::string output = getPrintfOutput();
    EXPECT_EQ(output, "Count: 123\n");
}

TEST_F(ExecutorTest, PrintStringWithFormatSpecifiers) {
    Executor executor;
    
    bool result = executor.print("Value: %d %s");
    
    EXPECT_TRUE(result);
    std::string output = getPrintfOutput();
    EXPECT_EQ(output, "Value: %d %s\n");
}

TEST_F(ExecutorTest, PrintVeryLongString) {
    Executor executor;
    std::string longString(1000, 'A');
    
    bool result = executor.print(longString);
    
    EXPECT_TRUE(result);
    std::string output = getPrintfOutput();
    EXPECT_EQ(output, longString + "\n");
}

TEST_F(ExecutorTest, PrintMultipleMessages) {
    Executor executor;
    
    bool result1 = executor.print("First message");
    bool result2 = executor.print("Second message");
    bool result3 = executor.print("Third message");
    
    EXPECT_TRUE(result1);
    EXPECT_TRUE(result2);
    EXPECT_TRUE(result3);
    std::string output = getPrintfOutput();
    EXPECT_EQ(output, "First message\nSecond message\nThird message\n");
}

TEST_F(ExecutorTest, PrintStringWithUnicodeCharacters) {
    Executor executor;
    
    bool result = executor.print("Hello 世界");
    
    EXPECT_TRUE(result);
    std::string output = getPrintfOutput();
    EXPECT_EQ(output, "Hello 世界\n");
}

// Tests for end() function
TEST_F(ExecutorTest, EndFunction) {
    Executor executor;
    
    bool result = executor.end();
    
    EXPECT_TRUE(result);
    std::string output = getPrintfOutput();
    EXPECT_EQ(output, "\n\nEND OF EXECUTION.\nRETURNED 0\n");
}

TEST_F(ExecutorTest, EndFunctionMultipleCalls) {
    Executor executor;
    
    bool result1 = executor.end();
    bool result2 = executor.end();
    
    EXPECT_TRUE(result1);
    EXPECT_TRUE(result2);
    std::string output = getPrintfOutput();
    EXPECT_EQ(output, "\n\nEND OF EXECUTION.\nRETURNED 0\n\n\nEND OF EXECUTION.\nRETURNED 0\n");
}

TEST_F(ExecutorTest, EndAfterPrint) {
    Executor executor;
    
    bool printResult = executor.print("Starting execution");
    bool endResult = executor.end();
    
    EXPECT_TRUE(printResult);
    EXPECT_TRUE(endResult);
    std::string output = getPrintfOutput();
    EXPECT_EQ(output, "Starting execution\n\n\nEND OF EXECUTION.\nRETURNED 0\n");
}

// Tests for setVariable() function
TEST_F(ExecutorTest, SetVariableSimple) {
    Executor executor;
    
    executor.setVariable("testVar", "testValue");
    
    std::string output = getCoutOutput();
    EXPECT_EQ(output, "VARIABLE SET: testVar VALUE: testValue\n");
}

TEST_F(ExecutorTest, SetVariableEmptyName) {
    Executor executor;
    
    executor.setVariable("", "testValue");
    
    std::string output = getCoutOutput();
    EXPECT_EQ(output, "VARIABLE SET:  VALUE: testValue\n");
}

TEST_F(ExecutorTest, SetVariableEmptyValue) {
    Executor executor;
    
    executor.setVariable("testVar", "");
    
    std::string output = getCoutOutput();
    EXPECT_EQ(output, "VARIABLE SET: testVar VALUE: \n");
}

TEST_F(ExecutorTest, SetVariableBothEmpty) {
    Executor executor;
    
    executor.setVariable("", "");
    
    std::string output = getCoutOutput();
    EXPECT_EQ(output, "VARIABLE SET:  VALUE: \n");
}

TEST_F(ExecutorTest, SetVariableOverwrite) {
    Executor executor;
    
    executor.setVariable("testVar", "initialValue");
    clearOutput();
    executor.setVariable("testVar", "newValue");
    
    std::string output = getCoutOutput();
    EXPECT_EQ(output, "VARIABLE SET: testVar VALUE: newValue\n");
}

TEST_F(ExecutorTest, SetVariableSpecialCharacters) {
    Executor executor;
    
    executor.setVariable("var_with_underscore", "value with spaces");
    
    std::string output = getCoutOutput();
    EXPECT_EQ(output, "VARIABLE SET: var_with_underscore VALUE: value with spaces\n");
}

TEST_F(ExecutorTest, SetVariableNumbers) {
    Executor executor;
    
    executor.setVariable("count", "42");
    
    std::string output = getCoutOutput();
    EXPECT_EQ(output, "VARIABLE SET: count VALUE: 42\n");
}

TEST_F(ExecutorTest, SetVariableLongStrings) {
    Executor executor;
    std::string longName(100, 'N');
    std::string longValue(200, 'V');
    
    executor.setVariable(longName, longValue);
    
    std::string output = getCoutOutput();
    EXPECT_EQ(output, "VARIABLE SET: " + longName + " VALUE: " + longValue + "\n");
}

TEST_F(ExecutorTest, SetMultipleVariables) {
    Executor executor;
    
    executor.setVariable("var1", "value1");
    executor.setVariable("var2", "value2");
    executor.setVariable("var3", "value3");
    
    std::string output = getCoutOutput();
    EXPECT_EQ(output, "VARIABLE SET: var1 VALUE: value1\n"
                     "VARIABLE SET: var2 VALUE: value2\n"
                     "VARIABLE SET: var3 VALUE: value3\n");
}

TEST_F(ExecutorTest, SetVariableWithNewlines) {
    Executor executor;
    
    executor.setVariable("testVar", "line1\nline2");
    
    std::string output = getCoutOutput();
    EXPECT_EQ(output, "VARIABLE SET: testVar VALUE: line1\nline2\n");
}

// Integration tests
TEST_F(ExecutorTest, MixedOperations) {
    Executor executor;
    
    executor.setVariable("greeting", "Hello");
    bool printResult = executor.print("Starting program");
    executor.setVariable("count", "5");
    bool endResult = executor.end();
    
    EXPECT_TRUE(printResult);
    EXPECT_TRUE(endResult);
    
    std::string coutOutput = getCoutOutput();
    std::string printfOutput = getPrintfOutput();
    
    EXPECT_EQ(coutOutput, "VARIABLE SET: greeting VALUE: Hello\n"
                         "VARIABLE SET: count VALUE: 5\n");
    EXPECT_EQ(printfOutput, "Starting program\n\n\nEND OF EXECUTION.\nRETURNED 0\n");
}

TEST_F(ExecutorTest, ComplexWorkflow) {
    Executor executor;
    
    // Simulate a complex execution workflow
    executor.setVariable("program", "test_script");
    executor.print("Initializing program");
    executor.setVariable("status", "running");
    executor.print("Program status: running");
    executor.setVariable("result", "success");
    executor.print("Execution completed successfully");
    executor.end();
    
    std::string coutOutput = getCoutOutput();
    std::string printfOutput = getPrintfOutput();
    
    EXPECT_THAT(coutOutput, ::testing::HasSubstr("VARIABLE SET: program VALUE: test_script"));
    EXPECT_THAT(coutOutput, ::testing::HasSubstr("VARIABLE SET: status VALUE: running"));
    EXPECT_THAT(coutOutput, ::testing::HasSubstr("VARIABLE SET: result VALUE: success"));
    
    EXPECT_THAT(printfOutput, ::testing::HasSubstr("Initializing program"));
    EXPECT_THAT(printfOutput, ::testing::HasSubstr("Program status: running"));
    EXPECT_THAT(printfOutput, ::testing::HasSubstr("Execution completed successfully"));
    EXPECT_THAT(printfOutput, ::testing::HasSubstr("END OF EXECUTION.\nRETURNED 0"));
}

// Edge case and stress tests
TEST_F(ExecutorTest, StressTestMultiplePrints) {
    Executor executor;
    
    // Test with many print operations
    for (int i = 0; i < 100; ++i) {
        bool result = executor.print("Message " + std::to_string(i));
        EXPECT_TRUE(result);
    }
    
    std::string output = getPrintfOutput();
    EXPECT_THAT(output, ::testing::HasSubstr("Message 0\n"));
    EXPECT_THAT(output, ::testing::HasSubstr("Message 50\n"));
    EXPECT_THAT(output, ::testing::HasSubstr("Message 99\n"));
}

TEST_F(ExecutorTest, StressTestMultipleVariables) {
    Executor executor;
    
    // Test with many variable operations
    for (int i = 0; i < 50; ++i) {
        executor.setVariable("var" + std::to_string(i), "value" + std::to_string(i));
    }
    
    std::string output = getCoutOutput();
    EXPECT_THAT(output, ::testing::HasSubstr("VARIABLE SET: var0 VALUE: value0"));
    EXPECT_THAT(output, ::testing::HasSubstr("VARIABLE SET: var25 VALUE: value25"));
    EXPECT_THAT(output, ::testing::HasSubstr("VARIABLE SET: var49 VALUE: value49"));
}

TEST_F(ExecutorTest, NullCharacterHandling) {
    Executor executor;
    
    std::string messageWithNull = "Hello";
    messageWithNull += '\0';
    messageWithNull += "World";
    
    bool result = executor.print(messageWithNull);
    
    EXPECT_TRUE(result);
    // Note: printf will stop at the null character
    std::string output = getPrintfOutput();
    EXPECT_EQ(output, "Hello\n");
}

// Performance-related tests
TEST_F(ExecutorTest, LargeMessagePerformance) {
    Executor executor;
    
    // Test with very large message
    std::string largeMessage(10000, 'X');
    
    auto start = std::chrono::high_resolution_clock::now();
    bool result = executor.print(largeMessage);
    auto end = std::chrono::high_resolution_clock::now();
    
    EXPECT_TRUE(result);
    
    // Verify the operation completes in reasonable time (less than 1 second)
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    EXPECT_LT(duration.count(), 1000);
}

// Test const correctness and parameter passing
TEST_F(ExecutorTest, ConstStringParameter) {
    Executor executor;
    const std::string constMessage = "Constant message";
    
    bool result = executor.print(constMessage);
    
    EXPECT_TRUE(result);
    std::string output = getPrintfOutput();
    EXPECT_EQ(output, "Constant message\n");
}

TEST_F(ExecutorTest, StringLiteralParameter) {
    Executor executor;
    
    bool result = executor.print("String literal");
    
    EXPECT_TRUE(result);
    std::string output = getPrintfOutput();
    EXPECT_EQ(output, "String literal\n");
}

// Test static variable behavior
TEST_F(ExecutorTest, StaticVariablesPersistAcrossInstances) {
    {
        Executor executor1;
        executor1.setVariable("shared", "value1");
    }
    
    clearOutput();
    
    {
        Executor executor2;
        executor2.setVariable("shared", "value2");
        
        std::string output = getCoutOutput();
        EXPECT_EQ(output, "VARIABLE SET: shared VALUE: value2\n");
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}