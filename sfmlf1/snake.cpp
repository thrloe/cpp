#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 800;
const int CELL_SIZE = 20;
const float INITIAL_SPEED = 0.25f;

enum class Direction { UP, DOWN, LEFT, RIGHT };

struct SnakeSegment {
    int x, y;
};

class Game {
private:
    sf::RenderWindow window;
    Direction dir;
    std::vector<SnakeSegment> snake;
    sf::RectangleShape food;
    sf::Clock gameClock;
    float timer;
    int score;
    sf::Font font;
    sf::Text scoreText;
    bool gameOver;

    void generateFood() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distX(0, (WINDOW_WIDTH/CELL_SIZE)-1);
        std::uniform_int_distribution<> distY(0, (WINDOW_HEIGHT/CELL_SIZE)-1);
        
        food.setPosition(distX(gen)*CELL_SIZE, distY(gen)*CELL_SIZE);
    }

    void checkCollisions() {
        // колизии
        if (snake.front().x < 0 || snake.front().x >= WINDOW_WIDTH/CELL_SIZE ||
            snake.front().y < 0 || snake.front().y >= WINDOW_HEIGHT/CELL_SIZE) {
            gameOver = true;
        }

        // колизии сам с собой
        for (size_t i = 1; i < snake.size(); ++i) {
            if (snake.front().x == snake[i].x && snake.front().y == snake[i].y) {
                gameOver = true;
                break;
            }
        }

        // колизии еды
        if (snake.front().x == food.getPosition().x/CELL_SIZE &&
            snake.front().y == food.getPosition().y/CELL_SIZE) {
            score += 10;
            scoreText.setString("Score: " + std::to_string(score));
            growSnake();
            generateFood();
        }
    }

    void growSnake() {
        SnakeSegment newSegment = snake.back();
        snake.push_back(newSegment);
    }

public:
    Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake Game"),
             dir(Direction::RIGHT),
             timer(0),
             score(0),
             gameOver(false) {
        window.setFramerateLimit(60);

        // змея
        snake.push_back({5, 5});
        snake.push_back({4, 5});
        snake.push_back({3, 5});

        // еда
        food.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        food.setFillColor(sf::Color::Red);
        generateFood();

        // текст
        if (!font.loadFromFile("arial.ttf")) {
        }
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10, 10);
    }

    void run() {
        while (window.isOpen()) {
            handleInput();
            update();
            render();
        }
    }

    void handleInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::W:
                        if (dir != Direction::DOWN) dir = Direction::UP;
                        break;
                    case sf::Keyboard::S:
                        if (dir != Direction::UP) dir = Direction::DOWN;
                        break;
                    case sf::Keyboard::A:
                        if (dir != Direction::RIGHT) dir = Direction::LEFT;
                        break;
                    case sf::Keyboard::D:
                        if (dir != Direction::LEFT) dir = Direction::RIGHT;
                        break;
                }
            }
        }
    }

    void update() {
        if (gameOver) return;

        float deltaTime = gameClock.restart().asSeconds();
        timer += deltaTime;

        if (timer > INITIAL_SPEED) {
            timer = 0;

            // движение тела
            for (size_t i = snake.size()-1; i > 0; --i) {
                snake[i].x = snake[i-1].x;
                snake[i].y = snake[i-1].y;
            }

            // движение
            switch (dir) {
                case Direction::UP:    snake[0].y--; break;
                case Direction::DOWN:  snake[0].y++; break;
                case Direction::LEFT:  snake[0].x--; break;
                case Direction::RIGHT: snake[0].x++; break;
            }

            checkCollisions();
        }
    }

    void render() {
        window.clear(sf::Color::Black);

        // змея
        for (auto& segment : snake) {
            sf::RectangleShape rect(sf::Vector2f(CELL_SIZE-1, CELL_SIZE-1));
            rect.setPosition(segment.x*CELL_SIZE, segment.y*CELL_SIZE);
            rect.setFillColor(sf::Color::Green);
            window.draw(rect);
        }

        // еда
        window.draw(food);

        // скор
        window.draw(scoreText);

        if (gameOver) {
            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("Game Over! Final Score: " + std::to_string(score));
            gameOverText.setCharacterSize(40);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 - 50);
            window.draw(gameOverText);
        }

        window.display();
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}