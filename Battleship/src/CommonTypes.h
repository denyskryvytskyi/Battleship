#ifndef _COMMON_TYPES_H
#define _COMMON_TYPES_H

#include <SFML/Graphics.hpp>

enum KeyCode
{
    KeyCode_MouseBtnPressed = sf::Event::MouseButtonPressed,
    KeyCode_LeftMouseBtn = sf::Mouse::Left,
    KeyCode_H = sf::Keyboard::H,
    KeyCode_WinClosed = sf::Event::Closed,
};

namespace CellColor
{
    static const sf::Color Background = sf::Color(260, 220, 180); // mint
    static const sf::Color Water = sf::Color(16, 16, 68); // dark blue
    static const sf::Color WaterBorder = sf::Color(92, 94, 154); // dark
    static const sf::Color Building = sf::Color(100, 100, 100); // gray
    static const sf::Color Ready = sf::Color(15, 160, 50); // green
    static const sf::Color Hited; // yellow
    static const sf::Color Killed; // red
    static const sf::Color Missed; // blue
};

#endif // !_COMMON_TYPES_H