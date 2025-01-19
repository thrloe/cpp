#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cstdlib>

using namespace std;

void launchApplication(const string& appPath) {
    // Используем system для запуска приложения
    if (system(appPath.c_str()) == -1) {
        cerr << "Failed to launch application: " << appPath << endl;
    }
}

int main() {
    sf::Color a1 (100,100,100);
    sf::Color a2 (60,60,60);

    // Создаем окно без использования SFML
    sf::RenderWindow window(sf::VideoMode(400, 300), "Application Launcher");
    
    // Кнопки
    sf::RectangleShape gravityButton(sf::Vector2f(180, 50));
    gravityButton.setPosition(110, 50);
    gravityButton.setFillColor(a1);
    
    sf::RectangleShape paintingButton(sf::Vector2f(180, 50));
    paintingButton.setPosition(110, 150);
    paintingButton.setFillColor(a2);
    
    // Текст
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) { // Убедитесь, что шрифт доступен
        return -1;
    }

    sf::Text gravityText("Gravity", font, 24);
    gravityText.setPosition(160, 60);
    gravityText.setFillColor(sf::Color::White);

    sf::Text paintingText("Painting V2", font, 24);
    paintingText.setPosition(130, 160);
    paintingText.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (gravityButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    launchApplication("/Users/petr/sfmlf1/gravity/gravity"); // Путь к gravity
                }
                if (paintingButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    launchApplication("/Users/petr/sfmlf1/painting_v2/painting_v2"); // Путь к painting_v2
                }
            }
        }

        window.clear();
        window.draw(gravityButton);
        window.draw(paintingButton);
        window.draw(gravityText);
        window.draw(paintingText);
        window.display();
    }

    return 0;
}