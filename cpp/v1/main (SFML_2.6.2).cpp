#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;
using json = nlohmann::json; 

// Константы
constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;
constexpr int TILE_SIZE = 40;
constexpr int GRID_WIDTH = WIDTH / TILE_SIZE;
constexpr int GRID_HEIGHT = HEIGHT / TILE_SIZE;

enum class BlockType { Empty, Solid, Restart };
enum class GameState { Menu, Playing, Editing, LevelSelect };

struct Level {
    std::vector<std::vector<BlockType>> grid =
        std::vector(GRID_HEIGHT, std::vector<BlockType>(GRID_WIDTH, BlockType::Empty));
};

class Player {
public:
    sf::RectangleShape shape;
    float x, y;
    float vx, vy;
    bool isJumping;
    float speed = 300.0f;
    float jumpSpeed = -400.0f;
    float gravity = 900.0f;
    float startX, startY;

    Player(float startX, float startY) : x(startX), y(startY), vx(0), vy(0), isJumping(false), startX(startX), startY(startY) {
        shape.setSize(sf::Vector2f(20, 40));
        shape.setFillColor(sf::Color::Blue);
        shape.setPosition(x, y);
    }

    void update(float dt, const Level& level) {
        vx = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            vx = speed;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            vx = -speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isJumping) {
            vy = jumpSpeed;
            isJumping = true;
        }

        vy += gravity * dt;

        float dx = vx * dt;
        float dy = vy * dt;

        move(dx, dy, level);

        shape.setPosition(x, y);
    }

    void move(float dx, float dy, const Level& level) {

        x += dx;
        sf::FloatRect playerBounds = shape.getGlobalBounds();
        shape.setPosition(x, y);
        playerBounds = shape.getGlobalBounds();

        for (int gridY = 0; gridY < GRID_HEIGHT; ++gridY) {
            for (int gridX = 0; gridX < GRID_WIDTH; ++gridX) {
                if (level.grid[gridY][gridX] == BlockType::Solid) {
                    sf::FloatRect tileBounds(gridX * TILE_SIZE, gridY * TILE_SIZE, TILE_SIZE, TILE_SIZE);
                    if (playerBounds.intersects(tileBounds)) {
                        if (dx > 0) {
                            x = tileBounds.left - playerBounds.width;
                        } else if (dx < 0) {
                            x = tileBounds.left + TILE_SIZE;
                        }
                        vx = 0;
                        shape.setPosition(x, y);
                        playerBounds = shape.getGlobalBounds();
                    }
                }
            }
        }


        y += dy;
        shape.setPosition(x, y);
        playerBounds = shape.getGlobalBounds();
        bool grounded = false;

        for (int gridY = 0; gridY < GRID_HEIGHT; ++gridY) {
            for (int gridX = 0; gridX < GRID_WIDTH; ++gridX) {
                if (level.grid[gridY][gridX] == BlockType::Solid) {
                    sf::FloatRect tileBounds(gridX * TILE_SIZE, gridY * TILE_SIZE, TILE_SIZE, TILE_SIZE);

                    if (playerBounds.intersects(tileBounds)) {
                        if (dy > 0) {
                            y = tileBounds.top - playerBounds.height;
                            vy = 0;
                            grounded = true;
                        } else if (dy < 0) {
                            y = tileBounds.top + TILE_SIZE;
                            vy = 0;
                        }
                        shape.setPosition(x, y);
                        playerBounds = shape.getGlobalBounds();
                    }
                }
            }
        }

        isJumping = !grounded;
    }


    void Reset() {
        x = startX;
        y = startY;
        vx = 0;
        vy = 0;
        isJumping = false;
        shape.setPosition(x, y);
    }
};

class Game {
private:
    sf::RenderWindow window;
    GameState state = GameState::Menu;
    Level level;
    Player player;
    BlockType currentBlock = BlockType::Solid;
    std::vector<std::string> levelFiles;
    sf::Font font;
    int selectedLevel = 0;
    float restartTimer = 0.f;

    void DrawGrid() {
        sf::RectangleShape line(sf::Vector2f(WIDTH, 1));
        for (int y = 0; y <= GRID_HEIGHT; y++) {
            line.setPosition(0, y * TILE_SIZE);
            window.draw(line);
        }
        line.setSize({1, HEIGHT});
        for (int x = 0; x <= GRID_WIDTH; x++) {
            line.setPosition(x * TILE_SIZE, 0);
            window.draw(line);
        }
    }

    void SaveLevel(const std::string& filename) {
        json j;
        for (auto& row : level.grid) {
            std::vector<int> intRow;
            for (auto& b : row)
                intRow.push_back(static_cast<int>(b));
            j["grid"].push_back(intRow);
        }
        std::ofstream(fs::path("levels") / filename) << j.dump(4);
    }

    void LoadLevel(const std::string& filename) {
        std::ifstream file(fs::path("levels") / filename);
        json j = json::parse(file);

        for (size_t y = 0; y < j["grid"].size(); y++)
            for (size_t x = 0; x < j["grid"][y].size(); x++)
                level.grid[y][x] = static_cast<BlockType>(j["grid"][y][x]);
    }

    void HandleCollisions() {

    }
    
    void CheckRestartCollision() {
        sf::FloatRect playerBounds = player.shape.getGlobalBounds();

        for (int gridY = 0; gridY < GRID_HEIGHT; ++gridY) {
            for (int gridX = 0; gridX < GRID_WIDTH; ++gridX) {
                if (level.grid[gridY][gridX] == BlockType::Restart) {
                    sf::FloatRect tileBounds(gridX * TILE_SIZE, gridY * TILE_SIZE, TILE_SIZE, TILE_SIZE);

                    if (playerBounds.intersects(tileBounds)) {
                        restartTimer = 2.0f;
                        player.Reset();
                        return;
                    }
                }
            }
        }
    }

public:
    Game() : window({WIDTH, HEIGHT}, "Platformer"), player(100, HEIGHT / 2 - 20) {
        window.setFramerateLimit(60);
        fs::create_directory("levels");

        if (!font.loadFromFile("arial.ttf")) {
            throw std::runtime_error("Failed to load font");
        }
    }

    void Run() {
        while (window.isOpen()) {
            sf::Event e;
            while (window.pollEvent(e)) {
                if (e.type == sf::Event::Closed)
                    window.close();

                if (state == GameState::Editing && e.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int gridX = mousePos.x / TILE_SIZE;
                    int gridY = mousePos.y / TILE_SIZE;

                    if (e.mouseButton.button == sf::Mouse::Left) {
                        level.grid[gridY][gridX] = currentBlock;
                    } else if (e.mouseButton.button == sf::Mouse::Right) {
                        currentBlock = static_cast<BlockType>((static_cast<int>(currentBlock) + 1) % 3);
                    }
                }
                if (e.type == sf::Event::KeyPressed) {
                    if (e.key.code == sf::Keyboard::R) {
                        restartTimer = 2.f;
                        player.Reset();
                    }
                }
            }

            window.clear();

            switch (state) {
                case GameState::Menu:
                    DrawMenu();
                    break;
                case GameState::Playing:
                    UpdateGame();
                    break;
                case GameState::Editing:
                    DrawEditor();
                    break;
                case GameState::LevelSelect:
                    DrawLevelSelect();
                    break;
            }

            window.display();
        }
    }

private:
    void DrawMenu() {
        sf::Text playText("Play", font, 50);
        sf::Text createText("Create", font, 50);
        playText.setPosition(500, 300);
        createText.setPosition(500, 400);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (playText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                state = GameState::LevelSelect;
                levelFiles.clear();
                for (const auto& entry : fs::directory_iterator("levels"))
                    levelFiles.push_back(entry.path().filename().string());
            } else if (createText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                state = GameState::Editing;
                level = Level();
            }
        }

        window.draw(playText);
        window.draw(createText);
    }

    void UpdateGame() {
        if (restartTimer > 0) {
            restartTimer -= 1.f / 60;
            window.clear(sf::Color::White);
            if (restartTimer <= 0) {
                player.Reset();
            }
            return;
        }

        sf::Clock clock;
        float dt = 1.f / 60;
        player.update(dt, level);
        CheckRestartCollision();

        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                if (level.grid[y][x] != BlockType::Empty) {
                    sf::RectangleShape tile({TILE_SIZE, TILE_SIZE});
                    tile.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                    tile.setFillColor(level.grid[y][x] == BlockType::Solid ? sf::Color::Green : sf::Color::Red);
                    window.draw(tile);
                }
            }
        }
        window.draw(player.shape);
    }

    void DrawEditor() {
        sf::Text saveText("Save (S)", font, 30);
        saveText.setPosition(10, 10);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            SaveLevel("level_" + std::to_string(time(nullptr)) + ".json");
            state = GameState::Menu;
        }

        DrawGrid();
        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                if (level.grid[y][x] != BlockType::Empty) {
                    sf::RectangleShape tile({TILE_SIZE, TILE_SIZE});
                    tile.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                    tile.setFillColor(level.grid[y][x] == BlockType::Solid ? sf::Color::Green : sf::Color::Red);
                    window.draw(tile);
                }
            }
        }
        sf::RectangleShape cursor({TILE_SIZE, TILE_SIZE});
        cursor.setPosition(sf::Mouse::getPosition(window).x / TILE_SIZE * TILE_SIZE,
                          sf::Mouse::getPosition(window).y / TILE_SIZE * TILE_SIZE);
        cursor.setFillColor(sf::Color(255, 255, 255, 100));
        window.draw(cursor);
        window.draw(saveText);
    }

    void DrawLevelSelect() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) selectedLevel--;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) selectedLevel++;
        selectedLevel = std::clamp(selectedLevel, 0, (int)levelFiles.size() - 1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            LoadLevel(levelFiles[selectedLevel]);
            state = GameState::Playing;
        }

        sf::Text text("Select level:", font, 40);
        text.setPosition(500, 100);
        window.draw(text);

        for (size_t i = 0; i < levelFiles.size(); i++) {
            sf::Text levelText(levelFiles[i], font, 30);
            levelText.setPosition(500, 200 + i * 50);
            if (i == selectedLevel) levelText.setFillColor(sf::Color::Red);
            window.draw(levelText);
        }
    }
};

int main() {
    Game game;
    game.Run();
    return 0;
}
