
/**
 * @file main.cpp
 * @brief Главный файл программы для управления зоопарком.
 * @details Этот файл содержит реализацию классов и функций для управления зоопарком,
 * включая работу с животными, вольерами, сотрудниками и ресурсами.
 */

#include <iostream>
#include <list>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <functional>


using namespace std;
/**
 * @brief Функция для получения целочисленного ввода от пользователя.
 * @param prompt Сообщение, выводимое пользователю перед запросом ввода.
 * @return Целочисленное значение, введенное пользователем.
 */
int getIntegerInput(const string& prompt);

class Zoo; // Предварительное объявление класса Zoo

// Функция для разделения строки на слова
vector<string> splitString(const string& str) {
    vector<string> words;
    string word;
    for (char c : str) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
        else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }
    return words;
}

// Функция для комбинирования видов
string combineSpecies(const string& species1, const string& species2) {
    // Разделяем виды на слова
    vector<string> words1 = splitString(species1);
    vector<string> words2 = splitString(species2);

    // Выбираем случайное слово из первого вида
    string part1 = words1[rand() % words1.size()];

    // Выбираем случайное слово из второго вида
    string part2 = words2[rand() % words2.size()];

    // Собираем новый вид
    return part1 + " " + part2;
}
/**
 * @brief Класс для представления животного.
 */
class Animal {
public:
    enum Type { LAND, AQUATIC }; // Типы животных: Земноводные и Водоплавающие

    string name;         ///< Имя животного
    string species;      ///< Вид животного
    int ageInDays;       ///< Возраст в днях
    int weight;          ///< Вес
    enum Climate { DESERT, FOREST, ARCTIC, OCEAN } climate; ///< Климат
    bool isCarnivore;    ///< Хищник или травоядное
    bool isInfected;     ///< Заражено ли животное
    char gender;         ///< Пол ('М' или 'Ж')
    pair<string, string> parents; ///< Имена родителей
    Type type;

    /**
     * @brief Конструктор для создания нового животного.
     * @param n Имя животного
     * @param s Вид животного
     * @param a Возраст в днях
     * @param w Вес
     * @param c Климат
     * @param carn Является ли хищником
     * @param g Пол ('M' или 'F')
     * @param t Тип животного ('Land' или 'Aquatic')
     * @param parent1 Имя первого родителя (по умолчанию пустая строка)
     * @param parent2 Имя второго родителя (по умолчанию пустая строка)
     */

     // Конструктор
    Animal(string n, string s, int a, int w, Climate c, bool carn, char g, Type t, string parent1 = "", string parent2 = "")
        : name(n), species(s), ageInDays(a), weight(w), climate(c), isCarnivore(carn), gender(g), type(t), parents({ parent1, parent2 }), isInfected(false) {}

    // Метод для проверки, является ли животное водоплавающим
    bool isAquatic() const {
        return type == AQUATIC;
    }
    /**
     * @brief Рассчитывает цену животного если оно водоплавающее.
     * @return Вес животного.
     */
    int calculateMaintenanceCost() const {
        if (isAquatic()) {
            return weight * 2; // Удвоенная стоимость содержания для водоплавающих
        }
        return weight;
    }
    /**
     * @brief Рассчитывает цену животного на основе его характеристик.
     * @return Цена животного в монетах.
     */
    int calculatePrice() const {
        int basePrice = 60;
        int price = basePrice + weight * 2 - ageInDays / 30 * 5;
        price += isCarnivore ? 100 : 0;
        price += static_cast<int>(climate) * 50;

        if (isAquatic()) {
            price += 200; // Например, дополнительная стоимость для водоплавающих
        }

        return max(price, 10);
    }
    /**
     * @brief Увеличивает возраст животного на один день.
     */
    void growOlder() {
        ageInDays++;
    }
    /**
     * @brief Вывод родителей животного.
     */
    void printParents() const {
        if (parents.first.empty() && parents.second.empty()) {
            cout << "Родители неизвестны";
        }
        else {
            cout << "Родители: " << parents.first << " и " << parents.second;
        }
    }
    /**
     * @brief Проверяет, умрет ли животное от старости.
     * @return true, если животное умирает от старости, иначе false.
     */
    bool diesOfOldAge() const {
        if (ageInDays > 60) { // Пример: максимальный возраст = 60 дней
            int deathChance = ageInDays - 60; // Шанс смерти = возраст - 60
            return rand() % 100 < deathChance;
        }
        return false;
    }

    /**
     * @brief Перегрузка оператора + для размножения животных.
     * @param other Второе животное для размножения.
     * @return Новое животное, полученное в результате размножения.
     * @throws runtime_error Если размножение невозможно.
     */
    Animal operator+(const Animal& other) {
        if (this->gender == other.gender) {
            throw runtime_error("Одинаковый пол! Размножение невозможно.");
        }
        if (this->species == other.species) {
            throw runtime_error("Животные одного вида не могут размножаться.");
        }

        // Генерация нового вида
        string newSpecies = combineSpecies(this->species, other.species);

        // Генерация случайного пола
        char newGender = rand() % 2 == 0 ? 'M' : 'F';

        // Определяем тип потомка
        Type newType = this->isAquatic() || other.isAquatic() ? AQUATIC : LAND;

        // Создаем новое животное
        return Animal(
            "",                          // Имя (пустое, так как пользователь задаст его позже)
            newSpecies,                  // Новый вид
            1,                           // Возраст (1 день)
            (this->weight + other.weight) / 2, // Средний вес
            this->climate,               // Климат
            this->isCarnivore || other.isCarnivore, // Тип питания
            newGender,                  // Пол
            newType                      // Тип животного
        );
    }
};
/**
 * @brief Класс для представления вольера.
 */
class Enclosure {
public:
    Animal::Climate climate; ///< Климат вольера
    int capacity;            ///< Вместимость вольера
    list<Animal> animals;    ///< Список животных в вольере
    int dailyCost;           ///< Ежедневные расходы на содержание вольера
    int level;               ///< Уровень вольера

    /**
     * @brief Конструктор для создания нового вольера.
     * @param c Климат вольера
     * @param cap Вместимость вольера
     */
    Enclosure(Animal::Climate c, int cap)
        : climate(c), capacity(cap), level(1) {
        dailyCost = calculateDailyCost();
    }
    /**
     * @brief Проверяет, можно ли добавить животное в вольер.
     * @param animal Животное для добавления
     * @return true, если животное можно добавить, иначе false.
     */
    bool canAddAnimal(const Animal& animal) {
        if (animals.size() >= capacity) return false; // Проверка вместимости
        if (animal.climate != climate) return false;  // Проверка климата

        // Проверка типа животного
        if (climate == Animal::OCEAN && !animal.isAquatic()) {
            cout << "Только водоплавающие животные могут находиться в вольере с климатом 'Океан'!\n";
            return false;
        }
        if (climate != Animal::OCEAN && animal.isAquatic()) {
            cout << "Водоплавающие животные могут находиться только в вольерах с климатом 'Океан'!\n";
            return false;
        }

        // Проверка совместимости хищников и травоядных
        if (!animals.empty()) {
            bool hasCarnivore = animals.front().isCarnivore;
            if (hasCarnivore != animal.isCarnivore) {
                cout << "Нельзя смешивать хищников и травоядных в одном вольере!\n";
                return false;
            }
        }

        return true;
    }
    /**
     * @brief Добавляет животное в вольер, если это возможно.
     * @param animal Животное для добавления
     */
    void addAnimal(const Animal& animal) {
        if (canAddAnimal(animal)) {
            animals.push_back(animal); //push back добавляет новый эл animal в конец списка (animals - список)
        }
    }
    /**
     * @brief Размножает животных в вольере.
     * @param zoo Ссылка на объект зоопарка
     */
    void breedAnimals(Zoo& zoo) {
        if (animals.size() < 2) {
            cout << "Недостаточно животных для размножения!\n";
            return;
        }

        // Находим разнополую пару
        Animal* parent1 = nullptr;
        Animal* parent2 = nullptr;

        // Вывод списка животных в вольере
        cout << "Животные в вольере:\n";
        int index = 1;
        for (auto& animal : animals) {
            cout << index << ". " << animal.name
                << ", Пол: " << (animal.gender == 'M' ? "М" : "Ж")
                << ", Возраст: " << animal.ageInDays << " дней\n";
            index++;
        }

        // Запрос выбора первого животного
        int choice1 = getIntegerInput("Введите номер первого животного: ");
        if (choice1 <= 0 || choice1 > animals.size()) {
            cout << "Неверный номер первого животного!\n";
            return;
        }

        // Запрос выбора второго животного
        int choice2 = getIntegerInput("Введите номер второго животного: ");
        if (choice2 <= 0 || choice2 > animals.size() || choice1 == choice2) {
            cout << "Неверный номер второго животного или вы выбрали одно и то же животное!\n";
            return;
        }

        for (auto it1 = animals.begin(); it1 != animals.end(); ++it1) {
            for (auto it2 = next(it1); it2 != animals.end(); ++it2) {
                if (it1->gender != it2->gender && it1->ageInDays > 5 && it2->ageInDays > 5) {
                    parent1 = &(*it1);
                    parent2 = &(*it2);
                    break;
                }
            }
            if (parent1 && parent2) break;
        }

        if (!parent1 || !parent2) {
            cout << "Не удалось найти подходящую пару для размножения!\n";
            return;
        }

        // Выводим информацию о найденной паре
        cout << "Найдена пара для размножения:\n";
        cout << "1. " << parent1->name << ", Вид: " << parent1->species << "\n";
        cout << "2. " << parent2->name << ", Вид: " << parent2->species << "\n";

        // Запрос подтверждения у пользователя
        cout << "Хотите размножить этих животных?\n";
        cout << "1. Да\n2. Нет\n";
        int confirm = getIntegerInput("Ваш выбор: ");
        if (confirm != 1) {
            cout << "Размножение отменено.\n";
            return;
        }

        // Генерация потомков
        int offspringCount = rand() % 100 < 10 ? 2 : 1; // 10% шанс на двух потомков
        offspringCount = min(offspringCount, capacity - static_cast<int>(animals.size())); // Учитываем вместимость вольера

        if (offspringCount == 0) {
            cout << "Вольер переполнен! Размножение невозможно.\n";
            return;
        }

        for (int i = 0; i < offspringCount; ++i) {
            try {
                // Создаем новый вид как комбинацию видов родителей
                string newSpecies = combineSpecies(parent1->species, parent2->species);

                // Запрашиваем имя нового животного у пользователя
                cout << "Введите имя для нового животного (" << newSpecies << "): ";
                string newName;
                getline(cin, newName);

                Animal::Type newType = parent1->isAquatic() || parent2->isAquatic() ? Animal::AQUATIC : Animal::LAND;

                // Создаем новое животное
                char newGender = rand() % 2 == 0 ? 'M' : 'F';
                Animal offspring(
                    newName,                  // Имя
                    newSpecies,               // Новый вид
                    1,                        // Возраст (1 день)
                    (parent1->weight + parent2->weight) / 2, // Средний вес
                    parent1->climate,         // Климат
                    parent1->isCarnivore || parent2->isCarnivore, // Тип питания
                    newGender,               // Пол
                    newType,
                    parent1->name,            // Имя первого родителя
                    parent2->name             // Имя второго родителя
                );

                // Добавляем потомка в вольер
                animals.push_back(offspring);
                cout << "Рождено новое животное: " << offspring.name
                    << " (" << (offspring.gender == 'M' ? "М" : "Ж") << "), Вид: " << offspring.species << "\n";
            }
            catch (const runtime_error& e) {
                cout << e.what() << "\n";
            }
        }
    }
    /**
     * @brief Удаляет животное из вольера по имени.
     * @param name Имя животного для удаления
     */
    void removeAnimal(const string& name) {
        for (auto it = animals.begin(); it != animals.end(); ++it) { //Создаём итератор It который прохожит по всему списку
            if (it->name == name) {
                animals.erase(it);
                return;
            }
        }
    }
    /**
     * @brief Заражает случайное животное в вольере.
     */
    void infectRandomAnimal() {
        if (animals.empty()) return; // Если в вольере нет животных, ничего не делаем

        for (auto& animal : animals) {
            if (!animal.isInfected && rand() % 100 < 30) { // 30% шанс заражения
                animal.isInfected = true;
                cout << "Животное \"" << animal.name << "\" заразилось терановирусом!\n";
                return; // Заражаем только одно животное за раз
            }
        }
    }
    /**
    * @brief Распространяет вирус среди животных в вольере.
    */
    void spreadVirus() {
        int infectedCount = 0;
        for (auto& animal : animals) {
            if (animal.isInfected) infectedCount++;
        }

        if (infectedCount > animals.size() / 2) {
            // Если больше половины животных заражены, начинают умирать
            vector<string> deadAnimals;
            for (auto it = animals.begin(); it != animals.end() && infectedCount > animals.size() / 2;) {
                if (it->isInfected && rand() % 2 == 0) {
                    deadAnimals.push_back(it->name);
                    it = animals.erase(it);
                    infectedCount--;
                }
                else {
                    ++it;
                }
            }

            // Вывод уведомлений о смерти
            if (!deadAnimals.empty()) {
                cout << "\n--- Уведомления ---\n";
                for (const string& name : deadAnimals) {
                    cout << "Животное \"" << name << "\" умерло от терановируса.\n";
                }
            }
        }
        else {
            // Иначе каждое больное животное заражает ещё двух
            for (auto& animal : animals) {
                if (animal.isInfected) {
                    int infections = 0;
                    for (auto it = animals.begin(); it != animals.end() && infections < 2; ++it) {
                        if (!it->isInfected && rand() % 100 < 30) { // 30% шанс заражения
                            it->isInfected = true;
                            infections++;
                            cout << "Животное \"" << it->name << "\" заразилось терановирусом!\n";
                        }
                    }
                }
            }
        }
    }
    /**
    * @brief Улучшает вольер до следующего уровня.
    * @param baseUpgradeCost Базовая стоимость улучшения
    * @return true, если улучшение успешно, иначе false.
    */
    bool upgrade(int baseUpgradeCost) {
        if (level >= 3) { // Ограничение на максимальный уровень
            cout << "Достигнут максимальный уровень улучшения!\n";
            return false;
        }
        capacity *= 2; // Увеличиваем вместимость в два раза
        dailyCost += calculateDailyCost() / 2; // Увеличиваем ежедневные расходы
        level++; // Повышаем уровень
        return true;
    }
    /**
     * @brief Рассчитывает стоимость строительства вольера.
     * @return Стоимость вольера в монетах.
     */
    int calculateCost() const {
        int baseCost = 100; // Базовая стоимость
        int cost = baseCost;

        // Модификаторы стоимости:
        cost += capacity * 10; // Чем больше вместимость, тем дороже
        cost += static_cast<int>(climate) * 50; // Разные климаты влияют на стоимость

        return max(cost, 150); // Минимальная стоимость = 150
    }
    /**
     * @brief Рассчитывает ежедневные расходы на содержание вольера.
     * @return Ежедневные расходы в монетах.
     */
    int calculateDailyCost() const {
        int baseDailyCost = 10; // Базовые ежедневные расходы
        int dailyCost = baseDailyCost;

        // Модификаторы расходов:
        dailyCost += capacity / 10; // Большая вместимость увеличивает расходы
        dailyCost += static_cast<int>(climate) * 5; // Разные климаты влияют на расходы

        // Учет водоплавающих животных
        for (const auto& animal : animals) {
            if (animal.isAquatic()) {
                dailyCost += 10; // Дополнительные расходы за каждое водоплавающее животное
            }
        }

        return max(dailyCost, 10); // Минимальные расходы = 10
    }
};
/**
 * @brief Класс для представления сотрудника зоопарка.
 */
class Employee {
public:
    string name;         ///< Имя сотрудника
    string position;     ///< Должность сотрудника
    int salary;          ///< Зарплата сотрудника
    int maxAnimals;      ///< Максимальное количество животных, за которыми может ухаживать сотрудник
    int currentAnimals;  ///< Текущее количество животных, за которыми ухаживает сотрудник
    /**
     * @brief Конструктор для создания нового сотрудника.
     * @param n Имя сотрудника
     * @param pos Должность
     * @param sal Зарплата
     * @param max Максимальное количество животных
     */
    Employee(string n, string pos, int sal, int max)
        : name(n), position(pos), salary(sal), maxAnimals(max), currentAnimals(0) {}
};
/**
 * @brief Генерирует случайное животное.
 * @return Случайное животное.
 */
Animal generateRandomAnimal(); // Предварительное объявление функции

/**
 * @brief Класс для представления зоопарка.
 */
class Zoo {
public:
    string name;                     ///< Название зоопарка
    int money, food, popularity;     ///< Деньги, количество еды и популярность зоопарка
    int day;                         ///< Текущий день
    int animalsBoughtToday;          ///< Счётчик купленных сегодня животных
    list<Enclosure> enclosures;      ///< Список вольеров 
    list<Employee> employees;        ///< Список сотрудников
    vector<Animal> animalMarket;     ///< Пул животных для покупки
    /**
     * @brief Конструктор для создания нового зоопарка.
     * @param n Название зоопарка
     * @param initialMoney Начальный капитал
     */
    Zoo(string n, int initialMoney)
        : name(n), money(initialMoney), food(0), popularity(50), day(1) {
        generateAnimalMarket(); // Инициализация пула животных
    }
    /**
     * @brief Генерирует пул животных для покупки.
     */
    void generateAnimalMarket() {
        const int MAX_ANIMALS_IN_MARKET = 10;
        animalMarket.clear();
        for (int i = 0; i < MAX_ANIMALS_IN_MARKET; ++i) {
            animalMarket.push_back(generateRandomAnimal());
        }
    }
    /**
     * @brief Сброс счётчика купленных животных за текущий день.
     */
    void resetDailyCounters() {
        animalsBoughtToday = 0; // Сбрасываем счетчик в начале дня
    }
    /**
     * @brief Список событий за текущий день.
     */
    vector<string> dailyEvents; // Список событий за текущий день
    /**
     * @brief Добавляем события.
     */
    void addEvent(const string& event) {
        dailyEvents.push_back(event);
    }
    /**
     * @brief Обновляет пул животных.
     * @param day Текущий день
     */
    void refreshAnimalMarket(int day) {
        if (day > 10 && animalMarket.size() >= 1) {
            cout << "После 10 дня можно обновить рынок только за плату!\n";
            int refreshCost = 150; // Стоимость обновления рынка
            if (money < refreshCost) {
                cout << "Недостаточно средств для обновления рынка!\n";
                return;
            }
            money -= refreshCost;
        }
        generateAnimalMarket();
        cout << "Рынок животных обновлен!\n";
    }
    /**
 * @brief Переходит к следующему дню в зоопарке.
 * @details Обрабатывает заражение животных, распространение вируса,
 * уменьшение популярности и расчет дохода, а также случайные события.
 */
    void nextDay() {
        cout << "\n--- День " << day << " ---\n";

        // Бюджет до дня
        cout << "Бюджет прошлого дня: " << money << " монет\n";

        dailyEvents.clear();

        resetDailyCounters();

        processRandomEvents();


        // Увеличение возраста животных и проверка смерти от старости
        for (auto& enc : enclosures) {
            for (auto it = enc.animals.begin(); it != enc.animals.end();) {
                it->growOlder(); // Увеличиваем возраст животного
                if (it->diesOfOldAge()) {
                    cout << "Животное \"" << it->name << "\" умерло от старости.\n";
                    it = enc.animals.erase(it); // Удаляем животное из списка
                }
                else {
                    ++it;
                }
            }
        }

        // Заражение случайного животного
        for (auto& enc : enclosures) {
            enc.infectRandomAnimal();
        }

        // Распространение вируса
        for (auto& enc : enclosures) {
            enc.spreadVirus();
        }

        // Уменьшение популярности из-за больных животных
        int infectedCount = 0;
        for (auto& enc : enclosures) {
            for (auto& animal : enc.animals) {
                if (animal.isInfected) infectedCount++;
            }
        }
        popularity -= infectedCount;
        popularity = max(popularity, 0);

        // Рассчет посетителей и дохода
        int visitors = 2 * popularity;
        int totalAnimals = getTotalAnimals();
        int income = visitors * totalAnimals;
        cout << "Посетители сегодня: " << visitors << "\n";
        cout << "Доход за день: +" << income << " монет\n";

        // Добавляем доход к бюджету
        money += income;

        // Зарплаты сотрудникам
        for (auto& emp : employees) {
            money -= emp.salary; // Вычитаем зарплату из всех денег
            emp.currentAnimals = 0; // Сброс счетчика
        }

        // Распределение животных между сотрудниками
        for (auto& enc : enclosures) { // Перебираем вольеры
            for (auto& emp : employees) { // Перебираем сотрудников
                if (emp.currentAnimals < emp.maxAnimals) {
                    int canTake = emp.maxAnimals - emp.currentAnimals;
                    int animalsInEnclosure = enc.animals.size(); // Определяем количество животных в вольере
                    int assignCount = min(canTake, animalsInEnclosure);
                    emp.currentAnimals += assignCount; // Обновляем счётчик
                    if (emp.currentAnimals >= emp.maxAnimals) break;
                }
            }
        }

        // Расходы на вольеры
        for (auto& enc : enclosures) {
            money -= enc.dailyCost;
        }

        // Питание животных
        int requiredFood = totalAnimals; // Количество еды, необходимое для всех животных
        vector<string> deadAnimals; // Список умерших животных
        if (food >= requiredFood) {
            food -= requiredFood;
            money -= requiredFood * 2; // Каждый кг еды стоит 2 монеты
        }
        else {
            int deficit = requiredFood - food; // Считаем сколько животных останутся голодными
            for (auto& enc : enclosures) { // Перебираем животных и со случайным шансом они умирают
                for (auto it = enc.animals.begin(); it != enc.animals.end() && deficit > 0;) {
                    if (rand() % 2 == 0) {
                        deadAnimals.push_back(it->name); // Сохраняем имя умершего животного
                        it = enc.animals.erase(it);
                        deficit--;
                    }
                    else {
                        ++it;
                    }
                }
            }
            food = 0;
        }

        // Колебания популярности
        int fluctuation = popularity * 0.1;
        int change = (rand() % (2 * fluctuation + 1)) - fluctuation;
        popularity += change;
        popularity = max(popularity, 0);

        // Бюджет после дня
        cout << "Бюджет текущего дня: " << money << " монет\n";

        // Уведомления о смерти животных
        if (!deadAnimals.empty()) {
            cout << "\n--- Уведомления ---\n";
            for (const string& name : deadAnimals) {
                cout << "Животное \"" << name << "\" умерло от голода.\n";
            }
        }

        // Проверка банкротства
        if (money < 0) {
            cout << "\nБАНКРОТСТВО! Вы проиграли.\n";
            exit(0); // Завершаем игру
        }

        // Увеличение дня
        day++; // Переход к следующему дню
    }

    // Метод для обработки случайных событий
    void processRandomEvents() {
        const int EVENT_PROBABILITY = 20; // 20% вероятность события

        // Положительные события
        vector<pair<string, function<void()>>> positiveEvents = {
            {"Знаменитый посетитель", [this]() {
                popularity += 10;
                cout << "Знаменитый посетитель: Популярность увеличена на 10.\n";
                addEvent("Знаменитый посетитель: Популярность увеличена на 10.");
            }},
            {"Пожертвование от спонсора", [this]() {
                money += 500;
                cout << "Пожертвование от спонсора: Получено 500 монет.\n";
                addEvent("Пожертвование от спонсора: Получено 500 монет.");
            }},
            {"Редкий гость", [this]() {
                popularity += 5;
                cout << "Редкий гость: Популярность увеличена на 5.\n";
                addEvent("Редкий гость: Популярность увеличена на 5.");
            }},
            {"День защиты животных", [this]() {
                popularity += 15;
                cout << "День защиты животных: Популярность увеличена на 15.\n";
                addEvent("День защиты животных: Популярность увеличена на 15.");
            }},
            {"Благотворительный фонд", [this]() {
                money += 1000;
                cout << "Благотворительный фонд: Получено 1000 монет.\n";
                addEvent("Благотворительный фонд: Получено 1000 монет.");
            }}
        };

        // Отрицательные события
        vector<pair<string, function<void()>>> negativeEvents = {
        {"Побег животного", [this]() {
            popularity -= 10;
            cout << "Побег животного: Популярность уменьшена на 10.\n";
            addEvent("Побег животного: Популярность уменьшена на 10.");
        }},
        {"Протечка в системе водоснабжения", [this]() {
            money -= 300;
            cout << "Протечка в системе водоснабжения: Потеряно 300 монет.\n";
            addEvent("Протечка в системе водоснабжения: Потеряно 300 монет.");
        }},
        {"Конфликт сотрудников", [this]() {
            popularity -= 5;
            cout << "Конфликт сотрудников: Популярность уменьшена на 5.\n";
            addEvent("Конфликт сотрудников: Популярность уменьшена на 5.");
        }},
        {"Пожар в зоопарке", [this]() {
            popularity -= 15;
            money -= 500;
            cout << "Пожар в зоопарке: Популярность уменьшена на 15, потеряно 500 монет.\n";
            addEvent("Пожар в зоопарке: Популярность уменьшена на 15, потеряно 500 монет.");
        }},
        {"Штраф от экологов", [this]() {
            money -= 200;
            cout << "Штраф от экологов: Потеряно 200 монет.\n";
            addEvent("Штраф от экологов: Потеряно 200 монет.");
        }}
        };


        // Генерация случайных событий
        if (rand() % 100 < EVENT_PROBABILITY) {
            bool isPositive = rand() % 2 == 0; // 50% шанс на положительное или отрицательное событие
            auto& events = isPositive ? positiveEvents : negativeEvents;
            if (!events.empty()) {
                int eventIndex = rand() % events.size();
                auto& [description, effect] = events[eventIndex];
                cout << "Событие: " << description << "\n";
                effect(); // Выполняем эффект события
            }
        }
    }
    /**
     * @brief Лечит животное.
     * @param name Имя животного для лечения
     */
    void cureAnimal(const string& name) {
        for (auto& enc : enclosures) { // Перебираем все вольеры
            for (auto& animal : enc.animals) { // Перебираем всех животных в вольере
                if (animal.name == name) { // Находим животное по имени
                    if (!animal.isInfected) { // Проверяем, заражено ли оно
                        cout << "Животное \"" << animal.name << "\" не заражено.\n";
                        return;
                    }

                    // Стоимость лечения
                    const int CURE_COST = 30;

                    // Запрос подтверждения на лечение
                    cout << "Лечение животного \"" << animal.name << "\" стоит " << CURE_COST << " монет.\n";
                    cout << "Хотите продолжить?\n";
                    cout << "1. Да\n2. Нет\n";
                    int confirm = getIntegerInput("Ваш выбор: ");
                    if (confirm != 1) { // Если пользователь отказался
                        cout << "Лечение отменено.\n";
                        return;
                    }

                    // Проверка наличия средств
                    if (money < CURE_COST) {
                        cout << "Недостаточно средств для лечения!\n";
                        return;
                    }

                    // Лечение животного
                    animal.isInfected = false; // Лечим животное
                    money -= CURE_COST; // Вычитаем стоимость лечения из бюджета
                    cout << "Животное \"" << animal.name << "\" успешно вылечено!\n";
                    return;
                }
            }
        }

        // Если животное не найдено
        cout << "Животное с именем \"" << name << "\" не найдено.\n";
    }

    /**
     * @brief Подсчитывает общее количество животных в зоопарке.
     * @return Общее количество животных.
     */
    int getTotalAnimals() {
        int total = 0;
        for (auto& enc : enclosures) total += enc.animals.size();
        return total;
    }
};
/**
 * @brief Получает целочисленный ввод от пользователя.
 * @param prompt Сообщение пользователю перед запросом ввода.
 * @return Введенное пользователем число.
 */
int getIntegerInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Некорректный ввод. Попробуйте снова.\n";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

/**
 * @brief Управляет сотрудниками зоопарка.
 * @param zoo Ссылка на объект зоопарка
 */
void manageEmployees(Zoo& zoo) {
    cout << "\n--- Управление работниками ---\n";
    cout << "1. Нанять сотрудника\n";
    cout << "2. Уволить сотрудника\n";
    cout << "3. Просмотреть список\n";
    cout << "0. Назад\n";

    int choice = getIntegerInput("Выберите действие: ");
    switch (choice) {
    case 1: {
        cout << "\nНаем сотрудника:\n";
        string name;
        cout << "Введите имя: ";
        getline(cin, name);

        cout << "1. Уборщик\n2. Ветеринар\n3. Кормилец\n";
        int posChoice = getIntegerInput("Выберите должность: ");
        string position;
        int salary, maxAnimals;

        switch (posChoice) {
        case 1:
            position = "Уборщик";
            salary = 80;
            maxAnimals = 20;
            break;
        case 2:
            position = "Ветеринар";
            salary = 150;
            maxAnimals = 10;
            break;
        case 3:
            position = "Кормилец";
            salary = 100;
            maxAnimals = 30;
            break;
        default:
            cout << "Неверный выбор!\n";
            return;
        }

        if (zoo.money >= salary) {
            zoo.employees.emplace_back(name, position, salary, maxAnimals);
            zoo.money -= salary;
            cout << "Сотрудник нанят!\n";
        }
        else {
            cout << "Недостаточно средств!\n";
        }
        break;
    }
    case 2: {
        cout << "\nУвольнение сотрудника:\n";
        int index = 1;
        for (auto& emp : zoo.employees) {
            if (emp.position != "Директор") { // Исключаем директора
                cout << index << ". " << emp.name << " (" << emp.position << ")\n";
                index++;
            }
        }
        int choice = getIntegerInput("Введите номер сотрудника: ");
        if (choice <= 0 || choice >= index) {
            cout << "Неверный номер!\n";
            break;
        }

        auto it = zoo.employees.begin();
        while (it != zoo.employees.end()) {
            if (it->position == "Директор") {
                ++it; // Пропускаем директора
                continue;
            }
            if (choice == 1) {
                zoo.employees.erase(it);
                cout << "Сотрудник уволен!\n";
                break;
            }
            --choice;
            ++it;
        }
        break;
    }
    case 3: {
        cout << "\nСписок сотрудников:\n";
        for (auto& emp : zoo.employees) {
            cout << "- " << emp.name << " (" << emp.position << ") Зарплата: "
                << emp.salary << ", Обслуживает: " << emp.currentAnimals << "/"
                << emp.maxAnimals << " животных\n";
        }
        break;
    }
    case 0:
        return;
    }
}
/**
 * @brief Управляет вольерами зоопарка.
 * @param zoo Ссылка на объект зоопарка
 */
void manageEnclosures(Zoo& zoo) {
    cout << "\n--- Управление вольерами ---\n";
    cout << "1. Построить вольер\n";
    cout << "2. Улучшить вольер\n";
    cout << "3. Просмотреть вольеры\n";
    cout << "0. Назад\n";

    int choice = getIntegerInput("Выберите действие: ");
    switch (choice) {
    case 1: {
        cout << "\n--- Создание нового вольера ---\n";
        cout << "Выберите климат для вольера:\n";
        cout << "0. Пустыня (Множитель цены: 1.2)\n";
        cout << "1. Лес (Множитель цены: 1.0)\n";
        cout << "2. Арктика (Множитель цены: 1.5)\n";
        cout << "3. Океан (Множитель цены: 1.8)\n";
        Animal::Climate climate = static_cast<Animal::Climate>(getIntegerInput("Ваш выбор: "));

        int capacity = getIntegerInput("Вместимость (Одно место = 50 монет): ");

        // Создаем временный вольер для расчета стоимости
        Enclosure newEnclosure(climate, capacity);
        int cost = newEnclosure.calculateCost();

        cout << "Стоимость вольера: " << cost << " монет\n";
        cout << "Хотите построить этот вольер?\n";
        cout << "1. Да\n2. Нет\n";
        int confirm = getIntegerInput("Ваш выбор: ");

        if (confirm != 1) {
            cout << "Строительство отменено.\n";
            break;
        }

        if (zoo.money < cost) {
            cout << "Недостаточно средств для строительства!\n";
            break;
        }

        zoo.enclosures.emplace_back(climate, capacity); //emplace back создаёт объект непосредственно в контейнере, избегая лишних копирований
        zoo.money -= cost;
        cout << "Вольер успешно построен!\n";
        break;
    }
    case 2: {
        cout << "\nУлучшение вольера:\n";
        int index = 1;
        for (auto& enc : zoo.enclosures) {
            string climate;
            switch (enc.climate) {
            case Animal::DESERT: climate = "Пустыня"; break;
            case Animal::FOREST: climate = "Лес"; break;
            case Animal::ARCTIC: climate = "Арктика"; break;
            case Animal::OCEAN: climate = "Океан"; break;
            }
            cout << index << ". Климат: " << climate
                << ", Уровень: " << enc.level
                << ", Животных: " << enc.animals.size() << "/" << enc.capacity
                << ", Расходы в день: " << enc.dailyCost << "\n";
            index++;
        }

        int choice = getIntegerInput("Введите номер вольера для улучшения: ");
        if (choice <= 0 || choice > zoo.enclosures.size()) {
            cout << "Неверный номер!\n";
            break;
        }

        auto it = zoo.enclosures.begin();
        advance(it, choice - 1); // Перемещаем итератор

        // Рассчитываем стоимость улучшения
        int upgradeCost = it->capacity * 5 * (it->level + 1);
        cout << "Стоимость улучшения: " << upgradeCost << " монет\n";
        cout << "Хотите улучшить этот вольер?\n";
        cout << "1. Да\n2. Нет\n";
        int confirm = getIntegerInput("Ваш выбор: ");

        if (confirm != 1) {
            cout << "Улучшение отменено.\n";
            break;
        }

        if (zoo.money < upgradeCost) {
            cout << "Недостаточно средств для улучшения!\n";
            break;
        }

        if (it->upgrade(upgradeCost)) {
            zoo.money -= upgradeCost;
            cout << "Вольер успешно улучшен до уровня " << it->level << "!\n";
        }
        break;
    }
    case 3: {
        cout << "\nСписок вольеров:\n";
        int index = 1;
        for (auto& enc : zoo.enclosures) {
            string climate;
            switch (enc.climate) {
            case Animal::DESERT: climate = "Пустыня"; break;
            case Animal::FOREST: climate = "Лес"; break;
            case Animal::ARCTIC: climate = "Арктика"; break;
            case Animal::OCEAN: climate = "Океан"; break;
            }
            cout << index << ". Климат: " << climate
                << ", Уровень: " << enc.level
                << ", Животных: " << enc.animals.size() << "/" << enc.capacity
                << ", Расходы в день: " << enc.dailyCost << "\n";
            index++;
        }
        break;
    }
    case 0:
        return;
    }
}

/**
 * @brief Список видов животных для климата пустыни.
 */
const string DESERT_SPECIES[] = { "Песчаный дракон", "Каменный скорпион", "Солнечный ящер", "Пустынный волк", "Гигантский скорпион" };
/**
 * @brief Список видов животных для климата леса.
 */
const string FOREST_SPECIES[] = { "Лесной феникс", "Теневой олень", "Кристальный медведь", "Искрящийся лис", "Механический единорог" };
/**
 * @brief Список видов животных для климата арктики.
 */
const string ARCTIC_SPECIES[] = { "Ледяной медведь", "Снежный дракон", "Арктический волк", "Хрустальная рыба", "Ледяной орёл" };
/**
 * @brief Список видов животных для климата океана.
 */
const string OCEAN_SPECIES[] = { "Глубинный кракен", "Электрическая акула", "Морской дракон", "Водяной дух", "Океанический гигант" };

/**
 * @brief Получает список видов животных для указанного климата.
 * @param climate Климат, для которого нужно получить виды.
 * @return Вектор строк, содержащий названия видов для указанного климата.
 */
vector<string> getSpeciesByClimate(Animal::Climate climate) {
    switch (climate) {
    case Animal::DESERT:
        return vector<string>(begin(DESERT_SPECIES), end(DESERT_SPECIES));
    case Animal::FOREST:
        return vector<string>(begin(FOREST_SPECIES), end(FOREST_SPECIES));
    case Animal::ARCTIC:
        return vector<string>(begin(ARCTIC_SPECIES), end(ARCTIC_SPECIES));
    case Animal::OCEAN:
        return vector<string>(begin(OCEAN_SPECIES), end(OCEAN_SPECIES));
    default:
        return {};
    }
}

/**
 * @brief Получает случайный вид животного для указанного климата.
 * @param climate Климат, для которого нужно получить случайный вид.
 * @return Строка с названием случайного вида.
 */
string getRandomSpecies(Animal::Climate climate) {
    switch (climate) {
    case Animal::DESERT:
        return DESERT_SPECIES[rand() % 5];
    case Animal::FOREST:
        return FOREST_SPECIES[rand() % 5];
    case Animal::ARCTIC:
        return ARCTIC_SPECIES[rand() % 5];
    case Animal::OCEAN:
        return OCEAN_SPECIES[rand() % 5];
    default:
        return "Неизвестный вид";
    }
}
/**
 * @brief Генерирует случайное животное.
 * @details Создает животное со случайными характеристиками, включая возраст, вес, климат, тип питания, пол и вид.
 * @return Новое случайно сгенерированное животное.
 */
Animal generateRandomAnimal() {
    Animal::Climate climates[] = { Animal::DESERT, Animal::FOREST, Animal::ARCTIC, Animal::OCEAN };

    int randomAge = rand() % 20 + 1;       // Возраст от 1 до 20
    int randomWeight = rand() % 96 + 5;  // Вес от 5 lj 100
    Animal::Climate randomClimate = climates[rand() % 4]; // Случайный климат
    bool isCarnivore = rand() % 2 == 0;    // Хищник или травоядное
    char randomGender = rand() % 2 == 0 ? 'M' : 'F'; // Случайный пол

    string randomSpecies = getRandomSpecies(randomClimate);

    Animal::Type randomType = (randomClimate == Animal::OCEAN) ? Animal::AQUATIC : Animal::LAND;

    return Animal("", randomSpecies, randomAge, randomWeight, randomClimate, isCarnivore, randomGender, randomType);
}
/**
 * @brief Изменяет имя животного в выбранном вольере.
 * @details Позволяет пользователю выбрать вольер, затем выбрать животное в этом вольере и изменить его имя.
 * @param zoo Ссылка на объект зоопарка.
 */
void renameAnimal(Zoo& zoo) {
    if (zoo.enclosures.empty()) {
        cout << "У вас нет вольеров!\n";
        return;
    }

    // Вывод списка вольеров
    int index = 1;
    for (auto& enc : zoo.enclosures) {
        string climateName;
        switch (enc.climate) {
        case Animal::DESERT: climateName = "Пустыня"; break;
        case Animal::FOREST: climateName = "Лес"; break;
        case Animal::ARCTIC: climateName = "Арктика"; break;
        case Animal::OCEAN: climateName = "Океан"; break;
        }
        cout << index << ". Климат: " << climateName
            << ", Животных: " << enc.animals.size() << "/" << enc.capacity << "\n";
        index++;
    }

    // Выбор вольера
    int enclosureChoice = getIntegerInput("Введите номер вольера: ");
    if (enclosureChoice <= 0 || enclosureChoice > zoo.enclosures.size()) {
        cout << "Неверный номер вольера!\n";
        return;
    }

    auto encIt = zoo.enclosures.begin();
    advance(encIt, enclosureChoice - 1);

    // Проверка, есть ли животные в вольере
    if (encIt->animals.empty()) {
        cout << "В этом вольере нет животных!\n";
        return;
    }

    // Вывод списка животных в выбранном вольере
    cout << "Животные в вольере:\n";
    index = 1;
    for (auto& animal : encIt->animals) {
        cout << index << ". " << animal.name << ", Вид: " << animal.species
            << ", Возраст: " << animal.ageInDays << " дней\n";
        index++;
    }

    // Выбор животного
    int animalChoice = getIntegerInput("Введите номер животного для изменения имени: ");
    if (animalChoice <= 0 || animalChoice > encIt->animals.size()) {
        cout << "Неверный номер животного!\n";
        return;
    }

    auto animalIt = encIt->animals.begin();
    advance(animalIt, animalChoice - 1);

    // Запрос нового имени
    cout << "Текущее имя: " << animalIt->name << "\n";
    cout << "Введите новое имя: ";
    string newName;
    getline(cin, newName);

    // Изменение имени
    animalIt->name = newName;
    cout << "Имя успешно изменено на \"" << newName << "\".\n";
}

/**
 * @brief Управляет животными зоопарка.
 * @param zoo Ссылка на объект зоопарка
 */
void manageAnimals(Zoo& zoo) {
    cout << "\n--- Управление животными ---\n";
    cout << "1. Купить готовое животное\n";
    cout << "2. Продать животное\n";
    cout << "3. Просмотреть животных\n";
    cout << "4. Лечение жвиотных\n";
    cout << "5. Обновить список животных (цена 150 монет)\n";
    cout << "6. Размножить животных\n";
    cout << "7. Изменить имя животного\n";
    cout << "0. Назад\n";

    int choice = getIntegerInput("Выберите действие: ");
    switch (choice) {
    case 1: { // Покупка готового животного
        cout << "\n--- Покупка готового животного ---\n";

        if (zoo.animalMarket.empty()) {
            cout << "На рынке нет доступных животных!\n";
            break;
        }

        // Ограничение на покупку после 10 - го дня
        if (zoo.day > 10) {
            cout << "После 10-го дня можно купить только одно животное в день!\n";
            if (zoo.animalsBoughtToday >= 1) {
                cout << "Вы уже купили животное сегодня.\n";
                break;
            }
        }

        // Выводим список животных
        cout << "Доступные животные:\n";
        for (int i = 0; i < zoo.animalMarket.size(); ++i) {
            Animal& animal = zoo.animalMarket[i];
            string climateName;
            switch (animal.climate) {
            case Animal::DESERT: climateName = "Пустыня"; break;
            case Animal::FOREST: climateName = "Лес"; break;
            case Animal::ARCTIC: climateName = "Арктика"; break;
            case Animal::OCEAN: climateName = "Океан"; break;
            }
            cout << i + 1 << ". Вид: " << animal.species // Используем поле species
                << ", Климат: " << climateName
                << ", Возраст: " << animal.ageInDays << " дней"
                << ", Вес: " << animal.weight << " кг"
                << ", Пол: " << (animal.gender == 'M' ? "М" : "Ж")
                << ", Тип: " << (animal.isCarnivore ? "Хищник" : "Травоядное")
                << ", Тип животного: " << (animal.isAquatic() ? "Вода" : "Земля")
                << ", Цена: " << animal.calculatePrice() << "\n";
        }
        // Выбор животного
        int choice = getIntegerInput("Введите номер животного для покупки: ");
        if (choice <= 0 || choice > zoo.animalMarket.size()) {
            cout << "Неверный номер!\n";
            break;
        }

        Animal selectedAnimal = zoo.animalMarket[choice - 1];
        int price = selectedAnimal.calculatePrice();

        cout << "Итоговая цена животного: " << price << " монет\n";
        cout << "Хотите купить это животное?\n";
        cout << "1. Да\n2. Нет\n";
        int confirm = getIntegerInput("Ваш выбор: ");

        if (confirm != 1) {
            cout << "Покупка отменена.\n";
            break;
        }

        if (zoo.money < price) {
            cout << "Недостаточно средств для покупки!\n";
            break;
        }

        // Запрос имени для животного
        string name;
        cout << "Введите имя для животного: ";
        getline(cin, name);
        selectedAnimal.name = name;

        // Фильтрация вольеров по климату
        vector<Enclosure*> suitableEnclosures;
        for (auto& enc : zoo.enclosures) {
            if (enc.climate == selectedAnimal.climate && enc.canAddAnimal(selectedAnimal)) {
                suitableEnclosures.push_back(&enc);
            }
        }

        if (suitableEnclosures.empty()) {
            cout << "Ошибка: Нет подходящего вольера!\n";
            break;
        }

        // Вывод подходящих вольеров
        cout << "\nВыберите вольер для размещения животного:\n";
        for (int i = 0; i < suitableEnclosures.size(); ++i) {
            Enclosure* enc = suitableEnclosures[i];
            cout << i + 1 << ". Климат: " << (enc->climate == Animal::DESERT ? "Пустыня" :
                enc->climate == Animal::FOREST ? "Лес" :
                enc->climate == Animal::ARCTIC ? "Арктика" : "Океан")
                << ", Животных: " << enc->animals.size() << "/" << enc->capacity << "\n";
        }

        int enclosureChoice = getIntegerInput("Введите номер вольера: ");
        if (enclosureChoice <= 0 || enclosureChoice > suitableEnclosures.size()) {
            cout << "Неверный номер вольера!\n";
            break;
        }

        Enclosure* selectedEnclosure = suitableEnclosures[enclosureChoice - 1];
        selectedEnclosure->addAnimal(selectedAnimal);
        zoo.money -= price;

        cout << "Животное \"" << selectedAnimal.name << "\" успешно добавлено в вольер!\n";

        // Удаляем купленное животное из пула
        zoo.animalMarket.erase(zoo.animalMarket.begin() + choice - 1);

        break;
    }
    case 2: { // Продажа животного
        cout << "\n--- Продажа животных ---\n";
        if (zoo.enclosures.empty()) {
            cout << "У вас нет вольеров!\n";
            break;
        }

        // Вывод списка вольеров
        int index = 1;
        for (auto& enc : zoo.enclosures) {
            string climateName;
            switch (enc.climate) {
            case Animal::DESERT: climateName = "Пустыня"; break;
            case Animal::FOREST: climateName = "Лес"; break;
            case Animal::ARCTIC: climateName = "Арктика"; break;
            case Animal::OCEAN: climateName = "Океан"; break;
            }
            cout << index << ". Климат: " << climateName
                << ", Животных: " << enc.animals.size() << "/" << enc.capacity << "\n";
            index++;
        }

        // Выбор вольера
        int enclosureChoice = getIntegerInput("Введите номер вольера: ");
        if (enclosureChoice <= 0 || enclosureChoice > zoo.enclosures.size()) {
            cout << "Неверный номер вольера!\n";
            break;
        }

        auto encIt = zoo.enclosures.begin();
        advance(encIt, enclosureChoice - 1);

        // Проверка, есть ли животные в вольере
        if (encIt->animals.empty()) {
            cout << "В этом вольере нет животных!\n";
            break;
        }

        // Вывод списка животных в выбранном вольере
        cout << "\nЖивотные в вольере:\n";
        index = 1;
        for (auto& animal : encIt->animals) {
            cout << index << ". " << animal.name << ", Возраст: " << animal.ageInDays
                << ", Вес: " << animal.weight << ", Цена: " << animal.calculatePrice() << "\n";
            index++;
        }

        // Выбор животного
        int animalChoice = getIntegerInput("Введите номер животного для продажи: ");
        if (animalChoice <= 0 || animalChoice > encIt->animals.size()) {
            cout << "Неверный номер животного!\n";
            break;
        }

        auto animalIt = encIt->animals.begin();
        advance(animalIt, animalChoice - 1);

        // Расчет цены продажи
        int price = animalIt->calculatePrice();
        int sellPrice = price * 0.8; // 80% от цены

        // Вывод информации о продаже
        cout << "Животное \"" << animalIt->name << "\" можно продать за " << sellPrice << " монет.\n";
        cout << "Вы уверены, что хотите продать это животное?\n";
        cout << "1. Да\n2. Нет\n";
        int confirm = getIntegerInput("Ваш выбор: ");

        // Подтверждение продажи
        if (confirm != 1) {
            cout << "Продажа отменена.\n";
            break;
        }

        // Сохраняем имя животного перед удалением
        string animalName = animalIt->name;

        // Удаление животного и добавление денег
        zoo.money += sellPrice;
        encIt->animals.erase(animalIt);

        // Вывод сообщения об успешной продаже
        cout << "Животное \"" << animalName << "\" продано за " << sellPrice << " монет.\n";
        break;
    }
    case 3: { // Просмотреть животных
        cout << "Список животных:\n";
        for (auto& enc : zoo.enclosures) {
            for (auto& animal : enc.animals) {
                string climateName;
                switch (animal.climate) {
                case Animal::DESERT: climateName = "Пустыня"; break;
                case Animal::FOREST: climateName = "Лес"; break;
                case Animal::ARCTIC: climateName = "Арктика"; break;
                case Animal::OCEAN: climateName = "Океан"; break;
                }
                cout << "- " << animal.name << ", "
                    << "Вид: " << animal.species << ", "
                    << animal.ageInDays << " дней, "
                    << animal.weight << " кг, "
                    << (animal.isCarnivore ? "Хищник" : "Травоядное") << ", "
                    << (animal.isAquatic() ? "Водоплавающее" : "Земноводное") << ", "
                    << "Климат: " << climateName << ", "
                    << "Пол: " << (animal.gender == 'M' ? "М" : "Ж") << ", ";
                animal.printParents();
                cout << "\n";
            }
        }
        break;
    }
    case 4: { // Лечение животного
        cout << "\n--- Лечение животных ---\n";
        if (zoo.enclosures.empty()) {
            cout << "У вас нет вольеров!\n";
            break;
        }

        // Вывод списка вольеров
        int index = 1;
        for (auto& enc : zoo.enclosures) {
            string climateName;
            switch (enc.climate) {
            case Animal::DESERT: climateName = "Пустыня"; break;
            case Animal::FOREST: climateName = "Лес"; break;
            case Animal::ARCTIC: climateName = "Арктика"; break;
            case Animal::OCEAN: climateName = "Океан"; break;
            }
            cout << index << ". Климат: " << climateName
                << ", Животных: " << enc.animals.size() << "/" << enc.capacity << "\n";
            index++;
        }

        // Выбор вольера
        int enclosureChoice = getIntegerInput("Введите номер вольера: ");
        if (enclosureChoice <= 0 || enclosureChoice > zoo.enclosures.size()) {
            cout << "Неверный номер вольера!\n";
            break;
        }

        auto encIt = zoo.enclosures.begin();
        advance(encIt, enclosureChoice - 1);

        // Проверка, есть ли больные животные в вольере
        bool hasInfected = false;
        for (auto& animal : encIt->animals) {
            if (animal.isInfected) {
                hasInfected = true;
                break;
            }
        }
        if (!hasInfected) {
            cout << "В этом вольере нет больных животных!\n";
            break;
        }

        // Вывод списка больных животных
        cout << "\nБольные животные в вольере:\n";
        index = 1;
        for (auto& animal : encIt->animals) {
            if (animal.isInfected) {
                cout << index << ". " << animal.name << ", Возраст: " << animal.ageInDays
                    << ", Вес: " << animal.weight << "\n";
                index++;
            }
        }

        // Выбор животного для лечения
        int animalChoice = getIntegerInput("Введите номер животного для лечения: ");
        if (animalChoice <= 0 || animalChoice > index - 1) {
            cout << "Неверный номер животного!\n";
            break;
        }

        int count = 0;
        for (auto& animal : encIt->animals) {
            if (animal.isInfected) {
                count++;
                if (count == animalChoice) {
                    zoo.cureAnimal(animal.name); // Вызов метода лечения
                    break;
                }
            }
        }
        break;
    }
    case 5: { // Обновление рынка
        zoo.refreshAnimalMarket(zoo.day);
        break;
    }
    case 6: { // Размножение животных
        cout << "\n--- Размножение животных ---\n";
        if (zoo.enclosures.empty()) {
            cout << "У вас нет вольеров!\n";
            break;
        }

        // Вывод списка вольеров
        int index = 1;
        for (auto& enc : zoo.enclosures) {
            string climateName;
            switch (enc.climate) {
            case Animal::DESERT: climateName = "Пустыня"; break;
            case Animal::FOREST: climateName = "Лес"; break;
            case Animal::ARCTIC: climateName = "Арктика"; break;
            case Animal::OCEAN: climateName = "Океан"; break;
            }
            cout << index << ". Климат: " << climateName
                << ", Животных: " << enc.animals.size() << "/" << enc.capacity << "\n";
            index++;
        }

        // Выбор вольера
        int enclosureChoice = getIntegerInput("Введите номер вольера: ");
        if (enclosureChoice <= 0 || enclosureChoice > zoo.enclosures.size()) {
            cout << "Неверный номер вольера!\n";
            break;
        }

        auto encIt = zoo.enclosures.begin();
        advance(encIt, enclosureChoice - 1);

        // Вызов метода размножения
        encIt->breedAnimals(zoo);
        break;
    }
    case 7: { // Изменение имени животного
        cout << "--- Изменение имени животного ---\n";
        renameAnimal(zoo);
        break;
    }
    case 0:
        return;
    }
}
/**
 * @brief Управляет ресурсами зоопарка.
 * @param zoo Ссылка на объект зоопарка
 */
void manageResources(Zoo& zoo) {
    cout << "\n--- Управление ресурсами ---\n";
    cout << "1. Купить еду\n";
    cout << "2. Заказать рекламу\n";
    cout << "0. Назад\n";

    int choice = getIntegerInput("Выберите действие: ");
    switch (choice) {
    case 1: {
        cout << "\nПокупка еды:\n";
        int amount = getIntegerInput("Сколько кг еды хотите купить? ");
        if (amount <= 0) {
            cout << "Неверное количество!\n";
            break;
        }

        int cost = amount * 2; // Цена еды: 2 монеты за 1 кг
        if (zoo.money < cost) {
            cout << "Недостаточно средств для покупки!\n";
            break;
        }

        zoo.food += amount;
        zoo.money -= cost;
        cout << "Куплено " << amount << " кг еды за " << cost << " монет.\n";
        break;
    }
    case 2: {
        const int COST_PER_POPULARITY = 20; // Стоимость одной единицы популярности
        cout << "Стоимость одной единицы популярности: " << COST_PER_POPULARITY << " монет\n";

        int cost = getIntegerInput("Введите сумму для рекламной кампании: ");
        if (cost <= 0) {
            cout << "Неверная сумма!\n";
            break;
        }

        if (zoo.money >= cost) {
            int popularityIncrease = cost / COST_PER_POPULARITY; // Рассчитываем прирост популярности
            zoo.money -= cost;
            zoo.popularity += popularityIncrease;

            cout << "Популярность увеличена на " << popularityIncrease << "!\n";
        }
        else {
            cout << "Недостаточно средств!\n";
        }
        break;
    }
    case 0:
        return;
    }
}
/**
 * @brief Главная функция программы.
 * @return Код завершения программы.
 */
int main() {
    srand(time(0));
    system("chcp 1251 > nul");
    setlocale(LC_ALL, "Russian");

    string zooName;
    cout << "Введите название зоопарка: ";

    // Очистка буфера ввода
    cin.clear(); // Сбрасываем флаги ошибок
    cin.sync();  // Синхронизируем поток ввода
    getline(cin, zooName); // Теперь корректно считываем строку

    int initialMoney = getIntegerInput("Введите начальный капитал: ");
    while (initialMoney < 0) {
        cout << "Недопустимое значение. ";
        initialMoney = getIntegerInput("Введите начальный капитал: ");
    }

    Zoo zoo(zooName, initialMoney);
    zoo.employees.emplace_back("Егор Потрошила", "Директор", 50, 50);

    while (true) {
        cout << "\n\n=== " << zoo.name << " ===\n";
        cout << "День: " << zoo.day << "\n";
        cout << "Деньги: " << zoo.money << " монет\n";
        cout << "Еда: " << zoo.food << " кг\n";
        cout << "Популярность: " << zoo.popularity << "\n";
        cout << "Животных: " << zoo.getTotalAnimals() << "\n";
        cout << "Вольеров: " << zoo.enclosures.size() << "\n";
        cout << "Работников: " << zoo.employees.size() << "\n";
        cout << "Посетители сегодня: " << 2 * zoo.popularity << "\n";

        cout << "\n[1] Животные\n";
        cout << "[2] Работники\n";
        cout << "[3] Вольеры\n";
        cout << "[4] Ресурсы\n";
        cout << "[0] Следующий день\n";

        int choice = getIntegerInput("Ваш выбор: ");
        if (choice == 0) {
            zoo.nextDay();
            if (zoo.money < 0) {
                cout << "\nБАНКРОТСТВО! Вы проиграли.\n";
                break;
            }
            if (zoo.day > 30) {
                cout << "\nПоздравляем! Вы успешно управляли зоопарком 30 дней!\n";
                break;
            }
        }
        else if (choice == 1) {
            manageAnimals(zoo);
        }
        else if (choice == 2) {
            manageEmployees(zoo);
        }
        else if (choice == 3) {
            manageEnclosures(zoo);
        }
        else if (choice == 4) {
            manageResources(zoo);
        }
    }

    return 0;
}