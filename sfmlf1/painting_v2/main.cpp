#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <sstream>  

using namespace sf;

int main() {

    VideoMode desktop = VideoMode::getDesktopMode();
    unsigned int screenWidth = desktop.width;
    unsigned int screenHeight = desktop.height;

    RenderWindow window(VideoMode(screenWidth, screenHeight), "Drawing Squares with Menu");
    window.setFramerateLimit(360);

    std::vector<RectangleShape> squares;
    bool isDrawing = false;
    bool isMenuOpen = false;
    bool showError = false;
    int squareSize = 20;  
    Color squareColor = Color::Black;  

    Font font;
    font.loadFromFile("Arial.ttf");

    // кнопки
    RectangleShape lengthBox(Vector2f(100, 30));
    lengthBox.setPosition(100, 50);
    lengthBox.setFillColor(Color(100, 100, 100));

    RectangleShape colorBox(Vector2f(100, 30));
    colorBox.setPosition(100, 100);
    colorBox.setFillColor(Color(100, 100, 100));

    RectangleShape clearButton(Vector2f(100, 30));
    clearButton.setPosition(100, 150);
    clearButton.setFillColor(Color(100, 100, 100));

    Text lengthText("Length", font, 15);
    lengthText.setPosition(20, 55);
    lengthText.setFillColor(Color::Black);

    Text colorText("Color", font, 15);
    colorText.setPosition(20, 105);
    colorText.setFillColor(Color::Black);

    Text clearText("Clear", font, 15);
    clearText.setPosition(120, 155);
    clearText.setFillColor(Color::Black);

    Text lengthInputText("", font, 15);
    lengthInputText.setPosition(105, 55);
    lengthInputText.setFillColor(Color::Black);

    Text colorInputText("", font, 15);
    colorInputText.setPosition(105, 105);
    colorInputText.setFillColor(Color::Black);

    // ошибка
    RectangleShape errorBox(Vector2f(200, 50));
    errorBox.setPosition(screenWidth / 2 - 100, screenHeight / 2 - 25);  
    errorBox.setFillColor(Color::Black);
    Text errorText("ERROR", font, 20);
    errorText.setFillColor(Color::Red);
    errorText.setPosition(screenWidth / 2 - 40, screenHeight / 2 - 15);  
    
    // ввод
    std::string lengthInput;
    std::string colorInput;
    bool isLengthActive = false;
    bool isColorActive = false;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::X) {
                    isMenuOpen = !isMenuOpen;
                }
            }

            if (isMenuOpen) {
                if (event.type == Event::MouseButtonPressed) {
                    Vector2f mousePos(Mouse::getPosition(window).x, Mouse::getPosition(window).y);

                    if (lengthBox.getGlobalBounds().contains(mousePos)) {
                        isLengthActive = true;
                        isColorActive = false;
                    } else if (colorBox.getGlobalBounds().contains(mousePos)) {
                        isColorActive = true;
                        isLengthActive = false;
                    } else if (clearButton.getGlobalBounds().contains(mousePos)) {
                        squares.clear();
                        clearButton.setFillColor(Color(60, 60, 60));
                    } else if (showError && errorBox.getGlobalBounds().contains(mousePos)) {
                        showError = false;  
                    } else {
                        isLengthActive = false;
                        isColorActive = false;
                    }
                }

                if (event.type == Event::MouseButtonReleased) {
                    clearButton.setFillColor(Color(100, 100, 100));
                }

                if (event.type == Event::TextEntered) {
                    if (isLengthActive) {
                        if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                            lengthInput += static_cast<char>(event.text.unicode);
                        } else if (event.text.unicode == 8 && !lengthInput.empty()) { 
                            lengthInput.pop_back();
                        }
                        lengthInputText.setString(lengthInput);  
                    } else if (isColorActive) {
                        if ((event.text.unicode >= '0' && event.text.unicode <= '9') || event.text.unicode == ' ') {
                            colorInput += static_cast<char>(event.text.unicode);
                        } else if (event.text.unicode == 8 && !colorInput.empty()) { 
                            colorInput.pop_back();
                        }
                        colorInputText.setString(colorInput);  
                    }
                }

                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                    if (isLengthActive) {
                        try {
                            squareSize = std::stoi(lengthInput);
                        } catch (...) {
                            showError = true;
                        }
                    } else if (isColorActive) {
                        int r, g, b;
                        std::istringstream colorStream(colorInput);
                        if (colorStream >> r >> g >> b && r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255) {
                            squareColor = Color(r, g, b);
                        } else {
                            showError = true;
                        }
                    }
                }
            } else {
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    isDrawing = true;
                }
                if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
                    isDrawing = false;
                }
            }
        }

        if (isDrawing && !isMenuOpen) {
            RectangleShape square(Vector2f(squareSize, squareSize));
            square.setPosition(Mouse::getPosition(window).x - squareSize / 2, Mouse::getPosition(window).y - squareSize / 2);
            square.setFillColor(squareColor);
            squares.push_back(square);
        }

        window.clear(Color::White);

        for (const auto &square : squares)
            window.draw(square);

        if (isMenuOpen) {
            RectangleShape overlay(Vector2f(screenWidth, screenHeight));
            overlay.setFillColor(Color(0, 0, 0, 150));
            window.draw(overlay);

            window.draw(lengthBox);
            window.draw(colorBox);
            window.draw(clearButton);
            window.draw(lengthText);
            window.draw(colorText);
            window.draw(clearText);
            window.draw(lengthInputText);  
            window.draw(colorInputText);   

            if (showError) {
                window.draw(errorBox);
                window.draw(errorText);
            }
        }

        window.display();
    }

    return 0;
}