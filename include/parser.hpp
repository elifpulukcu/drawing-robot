#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

enum class CommandType {
    Move,
    Rotate,
    Loop,
    Color,
    Pen
};

struct Command {
    CommandType type;
    int value;
    sf::Color color = sf::Color::Black; 
};

std::vector<Command> parseProgram(const std::string &programText);

#endif
