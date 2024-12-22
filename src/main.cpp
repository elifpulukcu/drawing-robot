#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include "parser.hpp"

struct LineSegment {
    sf::Vector2f start;
    sf::Vector2f end;
    sf::Color color;
    float thickness;
};

std::vector<LineSegment> computeSegments(const std::vector<Command>& commands)
{
    std::vector<LineSegment> segments;
    sf::Vector2f position(400.f, 300.f);
    float angle = 0.f;
    sf::Color currentColor = sf::Color::Black;
    float currentThickness = 1.f;

    constexpr float PI = 3.14159265358979f;

    for (auto &cmd : commands) {
        switch(cmd.type) {
            case CommandType::Move:
            {
                sf::Vector2f oldPos = position;
                position.x += cmd.value * std::cos(angle);
                position.y += cmd.value * std::sin(angle);

                LineSegment seg;
                seg.start = oldPos;
                seg.end   = position;
                seg.color = currentColor;
                seg.thickness = currentThickness;
                segments.push_back(seg);
                break;
            }
            case CommandType::Rotate:
                angle += cmd.value * (PI / 180.f);
                break;
            case CommandType::Color:
                currentColor = cmd.color;
                break;
            case CommandType::Pen:
                currentThickness = (cmd.value < 1) ? 1.f : static_cast<float>(cmd.value);
                break;
            case CommandType::Loop:
                break;
        }
    }

    return segments;
}

sf::Texture drawSegmentsTexture(const std::vector<LineSegment>& segments, unsigned width, unsigned height)
{
    sf::RenderTexture rtex;
    rtex.create(width, height);
    rtex.clear(sf::Color::White);

    for (auto &seg : segments) {
        sf::Vector2f delta = seg.end - seg.start;
        float length = std::sqrt(delta.x*delta.x + delta.y*delta.y);

        sf::RectangleShape line(sf::Vector2f(length, seg.thickness));
        line.setFillColor(seg.color);
        line.setPosition(seg.start);

        float angleDeg = std::atan2(delta.y, delta.x) * 180.f / 3.14159265f;
        line.setRotation(angleDeg);
        rtex.draw(line);
    }

    rtex.display();
    return rtex.getTexture();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Drawing Robot - TGUI 1.6.1");
    window.setFramerateLimit(60);

    tgui::Gui gui(window);

    // ----------------------------
    // 1) panelMain
    // ----------------------------
    auto panelMain = tgui::Panel::create({"100%", "100%"});
    panelMain->getRenderer()->setBackgroundColor({220,220,220});
    panelMain->setWidgetName("panelMain");
    gui.add(panelMain);

    auto labelTitle = tgui::Label::create("WELCOME TO DRAWING ROBOT");
    labelTitle->setPosition("(parent.size - size) / 2", "10%");
    labelTitle->setTextSize(24);
    labelTitle->getRenderer()->setTextColor(sf::Color::Black);
    labelTitle->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    panelMain->add(labelTitle);

    auto btnStart = tgui::Button::create("Start");
    btnStart->setSize(100,40);
    btnStart->setPosition("(parent.size - size) / 2", "30%");
    panelMain->add(btnStart);

    auto btnHelp = tgui::Button::create("Help");
    btnHelp->setSize(100,40);
    btnHelp->setPosition("(parent.size - size) / 2", "40%");
    panelMain->add(btnHelp);

    auto btnQuit = tgui::Button::create("Quit");
    btnQuit->setSize(100,40);
    btnQuit->setPosition("(parent.size - size) / 2", "50%");
    panelMain->add(btnQuit);

    // ----------------------------
    // 2) Help PaneL
    // ----------------------------
    auto panelHelp = tgui::Panel::create({"100%", "100%"});
    panelHelp->getRenderer()->setBackgroundColor({240,240,255});
    panelHelp->setWidgetName("panelHelp");
    panelHelp->setVisible(false);
    gui.add(panelHelp);

    auto labelHelpTitle = tgui::Label::create("COMMANDS TABLE");
    labelHelpTitle->setPosition("(&.w - w) / 2", "5%");
    labelHelpTitle->setTextSize(20);
    labelHelpTitle->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    panelHelp->add(labelHelpTitle);

    std::vector<std::pair<std::string, std::string>> commandsTable = {
        {"F n",      "Move forward n px"},
        {"R n",      "Turn n degrees to the right"},
        {"L n [...]",  "Repeat the contents of the parantheses n times (loop)"},
        {"COLOR f",  "Line color (K: Red, Y: Green, M: Blue, S: Black)"},
        {"PEN n",      "Line thickness (1: thin, 2: medium, 3: thick)"}
    };

    float xLeft    = 80.f;   
    float xRight   = 320.f;  
    float startY   = 120.f;  
    float offsetY  = 40.f;   

    for (std::size_t i = 0; i < commandsTable.size(); ++i)
    {
        auto lblCommand = tgui::Label::create(commandsTable[i].first);
        lblCommand->setTextSize(16);
        lblCommand->setPosition(xLeft, startY + i * offsetY);
        panelHelp->add(lblCommand);

        auto lblDesc = tgui::Label::create(commandsTable[i].second);
        lblDesc->setTextSize(16);
        lblDesc->setPosition(xRight, startY + i * offsetY);
        panelHelp->add(lblDesc);
    }

    auto labelHelpNote = tgui::Label::create("Press BACK to return to the main menu.");
    labelHelpNote->setTextSize(16);
    labelHelpNote->setPosition(80.f, 350.f);
    panelHelp->add(labelHelpNote);

    auto btnHelpBack = tgui::Button::create("Back");
    btnHelpBack->setPosition(80.f, 400.f);
    btnHelpBack->setSize(100,40);
    panelHelp->add(btnHelpBack);

    // ----------------------------
    // 3) panelCommand
    // ----------------------------
    auto panelCommand = tgui::Panel::create({"100%", "100%"});
    panelCommand->getRenderer()->setBackgroundColor({255,255,240});
    panelCommand->setWidgetName("panelCommand");
    panelCommand->setVisible(false);
    gui.add(panelCommand);

    auto labelCmd = tgui::Label::create("Enter your commands:");
    labelCmd->setPosition(50, 50);
    labelCmd->setTextSize(18);
    panelCommand->add(labelCmd);

    auto editBoxCmd = tgui::TextArea::create();
    editBoxCmd->setPosition(50, 100);
    editBoxCmd->setSize(700, 300);
    editBoxCmd->setTextSize(16);
    editBoxCmd->setDefaultText("e.g. L 36 [ L 4 [ F 100 R 90 ] R 10 ]");
    panelCommand->add(editBoxCmd, "editBoxCommands");

    auto btnRun = tgui::Button::create("Run");
    btnRun->setPosition(50, 420);
    btnRun->setSize(100, 40);
    panelCommand->add(btnRun);

    auto btnCommandBack = tgui::Button::create("Back");
    btnCommandBack->setPosition(180, 420);
    btnCommandBack->setSize(100, 40);
    panelCommand->add(btnCommandBack);

    // ----------------------------
    // 4) Error Panel
    // ----------------------------
    auto panelError = tgui::Panel::create({"100%", "100%"});
    panelError->getRenderer()->setBackgroundColor({255,240,240});
    panelError->setWidgetName("panelError");
    panelError->setVisible(false);
    gui.add(panelError);

    auto labelError = tgui::Label::create("Error message here...");
    labelError->setPosition(50,50);
    labelError->setTextSize(18);
    labelError->getRenderer()->setTextColor(sf::Color::Red);
    panelError->add(labelError, "labelError");

    auto btnErrorBack = tgui::Button::create("Back");
    btnErrorBack->setPosition(50,150);
    btnErrorBack->setSize(100,40);
    panelError->add(btnErrorBack);

    // ----------------------------
    // 5) Draw Panel
    // ----------------------------
    auto panelDraw = tgui::Panel::create({"100%", "100%"});
    panelDraw->getRenderer()->setBackgroundColor(sf::Color::White);
    panelDraw->setWidgetName("panelDraw");
    panelDraw->setVisible(false);
    gui.add(panelDraw);

    auto picture = tgui::Picture::create();
    picture->setSize("100%", "100%");
    panelDraw->add(picture, "drawPicture");

    auto btnDrawBack = tgui::Button::create("Back to Menu");
    btnDrawBack->setPosition(50, 50);
    btnDrawBack->setSize(120, 40);
    panelDraw->add(btnDrawBack);

    // ---------------------------
    // Buton Events 
    // ---------------------------
    // Main menu
    btnStart->onPress([&](){
        panelMain->setVisible(false);
        panelCommand->setVisible(true);
    });
    btnHelp->onPress([&](){
        panelMain->setVisible(false);
        panelHelp->setVisible(true);
    });
    btnQuit->onPress([&](){
        window.close();
    });

    // Help back
    btnHelpBack->onPress([&](){
        panelHelp->setVisible(false);
        panelMain->setVisible(true);
    });

    // Command back
    btnCommandBack->onPress([&](){
        panelCommand->setVisible(false);
        panelMain->setVisible(true);
    });

    // Command run
    btnRun->onPress([&](){
        auto txt = editBoxCmd->getText().toStdString();
        try {
            auto commands = parseProgram(txt);
            auto segments = computeSegments(commands);

            auto sfTexture = drawSegmentsTexture(segments, 800, 600);

            picture->getRenderer()->setTexture(sfTexture);

            panelCommand->setVisible(false);
            panelDraw->setVisible(true);
        }
        catch(const std::exception &ex) {
            std::ofstream errFile("errorLog.txt", std::ios::app);
            errFile << "[ERROR] " << ex.what() << "\nCommands: " << txt << "\n\n";
            errFile.close();

            labelError->setText(ex.what());
            panelCommand->setVisible(false);
            panelError->setVisible(true);
        }
    });

    btnErrorBack->onPress([&](){
        panelError->setVisible(false);
        panelCommand->setVisible(true);
    });

    btnDrawBack->onPress([&](){
        panelDraw->setVisible(false);
        panelMain->setVisible(true);
    });

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            gui.handleEvent(event);
        }
        window.clear();
        gui.draw();
        window.display();
    }

    return 0;
}
