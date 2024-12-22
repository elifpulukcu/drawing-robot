#include <gtest/gtest.h>
#include "parser.hpp"

TEST(ParserTest, ValidSimpleCommands) {
    std::string input = "F 10 R 90 COLOR S";
    auto result = parseProgram(input);
    ASSERT_EQ(result.size(), 3);

    EXPECT_EQ(result[0].type, CommandType::Move);
    EXPECT_EQ(result[1].type, CommandType::Rotate);
    EXPECT_EQ(result[2].type, CommandType::Color);
    EXPECT_EQ(result[2].color, sf::Color::Black); 
}

TEST(ParserTest, NestedLoop) {
    std::string input = "L 2 [ F 5 R 45 ]";
    auto result = parseProgram(input);
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(result[0].type, CommandType::Move);
    EXPECT_EQ(result[1].type, CommandType::Rotate);
    EXPECT_EQ(result[2].type, CommandType::Move);
    EXPECT_EQ(result[3].type, CommandType::Rotate);
}

TEST(ParserTest, InvalidCommand) {
    std::string input = "F 10 Z 20";
    EXPECT_THROW(parseProgram(input), std::invalid_argument);
}
