#include "parser.hpp"
#include <regex>
#include <stdexcept>
#include <iostream>

static std::vector<Command> parseCommands(std::sregex_iterator &it,
                                          const std::sregex_iterator &end,
                                          bool isInsideLoop);

std::vector<Command> parseProgram(const std::string &programText)
{
    std::regex tokenRegex(R"(([A-Za-z]+|\d+|\[|\]))");
    auto wordsBegin = std::sregex_iterator(programText.begin(), programText.end(), tokenRegex);
    auto wordsEnd   = std::sregex_iterator();

    std::sregex_iterator iter = wordsBegin;
    return parseCommands(iter, wordsEnd, false);
}

static std::vector<Command> parseCommands(std::sregex_iterator &it,
                                          const std::sregex_iterator &end,
                                          bool isInsideLoop)
{
    std::vector<Command> commands;

    while (it != end) {
        std::string token = it->str();
        ++it;

        if (token == "F") {
            if (it == end) throw std::invalid_argument("Missing value for 'F'");
            int val = std::stoi(it->str());
            ++it;
            commands.push_back({CommandType::Move, val});

        } else if (token == "R") {
            if (it == end) throw std::invalid_argument("Missing value for 'R'");
            int val = std::stoi(it->str());
            ++it;
            commands.push_back({CommandType::Rotate, val});

        } else if (token == "COLOR") {
            if (it == end) throw std::invalid_argument("Missing value for 'COLOR'");
            std::string colorCode = it->str();
            ++it;

            sf::Color chosenColor = sf::Color::Black;
            if      (colorCode == "K") chosenColor = sf::Color::Red;
            else if (colorCode == "Y") chosenColor = sf::Color::Green;
            else if (colorCode == "M") chosenColor = sf::Color::Blue;
            else if (colorCode == "S") chosenColor = sf::Color::Black;
            else {
                throw std::invalid_argument("Invalid color code: " + colorCode);
            }
            commands.push_back({CommandType::Color, 0, chosenColor});

        } else if (token == "P") {
            if (it == end) throw std::invalid_argument("Missing value for 'P'");
            int penThickness = std::stoi(it->str());
            ++it;
            commands.push_back({CommandType::Pen, penThickness});

        } else if (token == "L") {
            if (it == end) throw std::invalid_argument("Missing value for 'L'");
            int loopCount = std::stoi(it->str());
            ++it;

            if (it == end || it->str() != "[") {
                throw std::invalid_argument("Expected '[' after 'L'");
            }
            ++it; 

            std::vector<Command> loopCommands = parseCommands(it, end, true);

            for (int i = 0; i < loopCount; ++i) {
                commands.insert(commands.end(), loopCommands.begin(), loopCommands.end());
            }

        } else if (token == "]") {
            if (isInsideLoop) {
                return commands;
            } else {
                throw std::invalid_argument("']' without matching '['");
            }
        } else {
            throw std::invalid_argument("Unknown command: " + token);
        }
    }

    if (isInsideLoop) {
        throw std::invalid_argument("Missing closing ']' for loop");
    }

    return commands;
}
