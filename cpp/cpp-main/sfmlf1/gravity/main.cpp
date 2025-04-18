#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

using namespace std;

const float G = 6.67430e-11; // Гравитационная постоянная
const float STAR_MASS = 1.0e10; // Масса звезды
const float PLANET_MASS = 1.0e5; // Масса планеты
const float ORBITAL_RADIUS = 150.0f; // Радиус начальной орбиты
const int TRAIL_LENGTH = 600; // Длина трейла (количество состояний)

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Planet Orbiting Star");

    // Вектор для хранения позиций планеты
    vector<sf::Vector2f> trail; // Используем вектор векторов для хранения позиций

    // Начальные параметры планеты
    float angle = 0.0f;
    float planetX = 400 + ORBITAL_RADIUS * cos(angle);
    float planetY = 300 + ORBITAL_RADIUS * sin(angle);
    float velocityX = 0.0f; // Начальная скорость по X
    float velocityY = sqrt(G * STAR_MASS / ORBITAL_RADIUS); // Начальная скорость по Y для орбитального движения

    // Создаем звезду как круг
    sf::CircleShape star(8); // Радиус звезды 5 пикселей
    star.setFillColor(sf::Color::Yellow);
    star.setPosition(400 - 5, 300 - 5); // Центрируем звезду в окне

    // Создаем планету как круг
    sf::CircleShape planet(5); // Радиус планеты 5 пикселей
    planet.setFillColor(sf::Color::Blue);
    planet.setPosition(planetX - 5, planetY - 5); // Центрируем планету

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Вычисляем расстояние между звездой и планетой
        float deltaX = 400 - planetX;
        float deltaY = 300 - planetY;
        float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

        // Вычисляем силу притяжения
        float force = G * (STAR_MASS * PLANET_MASS) / (distance * distance);
        float accelerationX = (force * deltaX / distance) / PLANET_MASS; // Ускорение по X
        float accelerationY = (force * deltaY / distance) / PLANET_MASS; // Ускорение по Y

        // Обновляем скорость планеты
        velocityX += accelerationX;
        velocityY += accelerationY;

        // Обновляем позицию планеты
        planetX += velocityX;
        planetY += velocityY;

        // Сохраняем позицию для трейла
        trail.push_back(sf::Vector2f(planetX, planetY));

        // Удаляем старые точки трейла, когда их количество превышает TRAIL_LENGTH
        if (trail.size() > TRAIL_LENGTH) {
            trail.erase(trail.begin()); // Удаляем первую позицию
        }

        // Очищаем окно
        window.clear(sf::Color::Black);

        // Рисуем трейл как круги
        for (const auto& pos : trail) {
            sf::CircleShape trailCircle(4); // Радиус кругов трейла
            trailCircle.setFillColor(sf::Color::Blue); // Цвет, соответствующий планете
            trailCircle.setPosition(pos.x - 5, pos.y - 5); // Центрируем круг
            window.draw(trailCircle); // Рисуем круг трейла
        }

        // Обновляем позицию планеты и рисуем её
        planet.setPosition(planetX - 5, planetY - 5); // Центрируем планету
        window.draw(planet);

        // Рисуем звезду
        window.draw(star);

        window.display();
    }

    return 0;
}
