# cpp-hometasks
# cpp-hometasks

# README: Park Management System

## 1. Как собрать и запустить
Для сборки требуется компилятор с поддержкой C++17 (из-за использования structured bindings, std::make_unique и т.д.).

g++ -std=c++17 second_hometask.cpp -o park_manager
./park_manager

## 2. Схема наследования
В проекте используется множественное и виртуальное наследование (ромбовидное).

* Thingy — базовый класс (виртуальный деструктор).
* Transport и Electro_thingy — промежуточные классы. Они наследуют Thingy виртуально (virtual public Thingy), чтобы избежать дублирования базового класса при создании Scooter и Ebike.
* Scooter и Ebike наследуют оба класса Transport и Electro_thingy (множественное наследование). 
* Bike наследует только Transport.
* Dock наследует только Electro_thingy.
* Helmet наследует напрямую от Thingy.

Схема:
Thingy (virtual base)
 ├── Transport (virtual public Thingy)
 │    ├── Bike (public Transport)
 │    ├── Scooter (public Electro_thingy, public Transport)
 │    └── Ebike (public Electro_thingy, public Transport)
 ├── Electro_thingy (virtual public Thingy)
 │    ├── Dock (public Electro_thingy)
 │    ├── Scooter (public Electro_thingy, public Transport)
 │    └── Ebike (public Electro_thingy, public Transport)
 └── Helmet (public Thingy)

## 3. Умные указатели (где и почему)
* std::shared_ptr: Park::things_. Главное хранилище. Владеет объектами и удаляет их, когда они больше никому не нужны.
* std::weak_ptr: Park::electro_things_, Park::transports_, Park::focus_. Списки-индексы для быстрого доступа (наблюдатели). Использование shared_ptr привело бы к утечкам (объект не удалялся бы из things_, пока на него ссылаются индексы). weak_ptr позволяет проверять объект через .lock().
* std::unique_ptr: 
  - Park::sup_ — эксклюзивное владение объектом техподдержки.
  - ParkManager::Parks — вектор парков. Класс Park некопируемый, а unique_ptr гарантирует, что при расширении std::vector адреса парков в памяти не изменятся (ссылки Park& останутся валидными).

## 4. Исключения (и ситуации)
* std::invalid_argument:
  - score вне диапазона [0, 10] (AddScooter, AddBike, AddEbike).
  - energy < 0 (AddScooter, AddEbike, AddDock).
  - Неверный индекс парка (ParkManager::SwitchPark).
  - Неизвестная команда/тип в main.
* std::runtime_error:
  - Объект не прошел проверку (MakeChecked — имя содержит "broken").
  - Попытка переноса TransferTo в парк, где уже есть такой id.
* std::out_of_range:
  - Попытка переноса несуществующего id (TransferTo).
  - Запрос несуществующего индекса парка (ParkManager::GetPark).

## 5. Шаблоны (Templates)
* template <typename T> bool Checker_function(const T&) — предикат проверки (ищет "broken").
* template <typename T> class Tech_support — обертка для функции проверки.
* template <typename Vehicle, typename... Args> std::shared_ptr<Vehicle> Park::MakeChecked(Args&&...) — ключевой шаблон. Используется во всех Add... Создает unique_ptr, проверяет через Tech_support и конвертирует в shared_ptr (устраняет дублирование кода).

## 6. Креативная часть
* Класс Tech_support: Проверка объектов на "сломанность" (наличие слова "broken") перед добавлением в парк.
* Класс ParkManager: Управление несколькими парками (вектор unique_ptr + активный индекс).
* Функция TransferTo: Перенос объекта между парками с использованием dynamic_pointer_cast. При переносе объект автоматически распределяется в нужные списки (electro_things_ или transports_) в зависимости от реального типа.

## 7. Использование ИИ
В ходе разработки использовался ИИ (DeepSeek).
* Сделано студентом: Каркас классов, реализация Tech_support, реализация ParkManager, таблица команд, черновики методов вывода, вывод ошибок через стандартные типы ошибок, отдельно заставил дипсика два раза переписывать readme, чтобы он не врал.
* Сделано ИИ: Парсинг команд в main, шаблон MakeChecked, функция TransferTo, исправление ошибок компиляции (забытые include, std::), добавление .lock() в методах вывода. Замена стандартных ошибок на кастомные. этот readme.
