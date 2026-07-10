// GAME111.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <limits>

// Базовый класс для головоломок
class Puzzle {
public:
    virtual ~Puzzle() = default;
    virtual bool solve() = 0;
    virtual std::string getQuestion() const = 0;
};

// Конкретная головоломка: Математическая загадка
class MathPuzzle : public Puzzle {
public:
    bool solve() override {
        int answer = 0;
        std::cout << "Решите загадку: 2 + 2 * 2 = ?\n> ";
        std::cin >> answer;
        return answer == 6;
    }

    std::string getQuestion() const override {
        return "Математический замок. Введите ответ числом.";
    }
};

// Конкретная головоломка: Угадай число
class GuessPuzzle : public Puzzle {
public:
    bool solve() override {
        int answer = 0;
        std::cout << "Угадайте число от 0 до 9:\n> ";
        std::cin >> answer;
        return answer == 5; // Секретный ответ
    }

    std::string getQuestion() const override {
        return "Дверь заблокирована. Замок требует отгадать число.";
    }
};

// Локация
class Location {
public:
    int id;
    std::vector<std::string> art;
    std::map<std::string, int> connected_ids; // Направление -> ID локации
    bool is_open;
    std::shared_ptr<Puzzle> puzzle;

    Location(int id, std::vector<std::string> a, bool open, std::shared_ptr<Puzzle> p = nullptr)
        : id(id), art(a), is_open(open), puzzle(p) {
    }

    void draw() const {
        for (const auto& line : art) {
            std::cout << line << "\n";
        }
    }
};

// Персонаж
class Player {
public:
    int current_location_id;

    Player(int start_id) : current_location_id(start_id) {}
};

// Главный класс игры
class Game {
private:
    Player player;
    std::map<int, Location> locations;
std::map<int, Location> locations;
std::string player_name; // новое поле
public:
    Game() : player(1) {
        // Инициализация локаций
        // Локация 1 (Старт)
        locations.insert({ 1, Location(1, { "###################", "# ВЫХОД #", "# Тут СВЕТ #", "# [ ] #", "###################" }, true) });

        // Локация 2 (Закрытая, с математической загадкой)
        locations.insert({ 2, Location(2, {"*******************", "* ТЕМНЫЙ КОРИДОР *", "* === *", "* |___| *", "*******************" }, false, std::make_shared<MathPuzzle>()) });

        // Локация 3 (Победная, изначально закрыта другой загадкой)
        locations.insert({ 3, Location(3, { "===================", "= ПОБЕДА! =", "= Вы выбрались =", "= =", "===================" }, false, std::make_shared<GuessPuzzle>()) });

        // Настройка связей между локациями
        // Из 1 можно пойти на Север в 2
        locations[1].connected_ids["север"] = 2;

        // Из 2 можно вернуться на Юг в 1 или пойти на Восток в 3
        locations[2].connected_ids["юг"] = 1;
        locations[2].connected_ids["восток"] = 3;
    }

    void start() {
        std::cout << "Введите ваше имя: ";
std::getline(std::cin, player_name);
        std::cout << "Добро пожаловать в текстовый квест!\n";
        std::cout << "Ваша задача - выбраться из подземного лабиринта.\n\n";

        while (true) {
            Location& current_loc = locations.at(player.current_location_id);
            current_loc.draw();

            // Проверка на победу
            if (player.current_location_id == 3) {
                std::cout << "Поздравляем, вы прошли лабиринт!\n";
                break;
            }

            std::cout << "\nДоступные пути: ";
            for (const auto& pair : current_loc.connected_ids) {
                std::cout << "[" << pair.first << "] ";
            }

            std::cout << "\nВведите направление или 'выход': ";
            std::string command;
            std::cin >> command;

            if (command == "выход") {
                break;
            }

            if (current_loc.connected_ids.count(command) > 0) {
                int target_id = current_loc.connected_ids[command];
                Location& target_loc = locations.at(target_id);

                if (target_loc.is_open) {
                    player.current_location_id = target_id;


                    std::cout << "Вы успешно переместились.\n\n";
                }
                else {
                    std::cout << "\nДверь заперта. " << target_loc.puzzle->getQuestion() << "\n";
                    if (target_loc.puzzle->solve()) {
                        target_loc.is_open = true;
                        player.current_location_id = target_id;
                        std::cout << "Головоломка решена! Дверь открыта.\n\n";
                    }
                    else {
                        std::cout << "Неверный ответ! Дверь остается запертой.\n\n";
                    }
                }
            }
            else {
                std::cout << "Туда нельзя пройти. Попробуйте снова.\n\n";
            }

            // Очистка буфера после возможных ошибок ввода
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
};

int main() {
    Game game;
    game.start();
    return 0;
}
