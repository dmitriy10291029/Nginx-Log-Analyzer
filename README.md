# Особенности проекта

- Работа со строками в c-style (по ТЗ)

- В связи с ограниченем по ТЗ на использование внешних библиотек:

    - Написана своя небольшая утилита для тестирования, которая сама нумерует тесты, позволяет не придумывать разные названия тестам,
имеет цветной вывод, в случае ошибки выводит номер строки в коде тестов, а также тесты запускаются через ctest (см. ./tests)

    - Написана небольшая утилита для парсинга аргументов, которую можно использовать в других проектах

- Оптимизирована работа с оперативной памятью за счет хранения полиномиального хеша строки, вместо самой строки

- Оптимизирована работа со временем за счет использования хэш-таблицы

- Гарантируется верный результат, так как алгоритм исключает коллизии

- Используется буфферизированный ввод и вывод

- Программа устойчива к неправильному вводу (аргументы и содержимое файлов), в большинстве ошибочных случаев есть вывод 
соответствующей ошибки или предупреждения

# Сборка и запуск

### Способ 1.

Все команды запускаются из корня проекта:

1. `make init` - инициализация cmake

2. `make build` - сборка проекта

3. `make run args="[ARGS]"` - запуск программы

Тестирование:

1. `make test_init` - автоматическая установка файла для тестирования ([логи сервера NASA](https://drive.google.com/file/d/1jjzMocc0Rn9TqkK_51Oo93Fy78KYnm2i/view)). Необходимы утилиты **curl** и **gunzip**. Если команда завершилась с ошибкой, то скачайте архив самостоятельно и распакуйте его в папку ./res.

2. `make test_run` - запуск тестов

### Способ 2.

1. Инициализация cmake:
```sh
mkdir build
cd build
cmake ..
```

2. Сборка проекта (из папки build): `cmake --build .`

3. Запуск программы (из папки build/bin): `./AnalyzeLog ...`

Тестирование:

1. Скачайте архив ([логи сервера NASA](https://drive.google.com/file/d/1jjzMocc0Rn9TqkK_51Oo93Fy78KYnm2i/view)) и распакуйте его в папку ./res.

2. `ctest` из папки build - запуск тестов

# Аргументы
Одним из аргументов обязательно указывается файл, описание остальных:

| Короткий аргумент | Длинный аргумент  | Значение по умолчанию   | Описание |
|-------------------|-------------------|-------------------------|----------|
| `-o path`         | `--output=path`   |                         | Путь к файлу, в который будут записаны запросы с ошибками. Если файл не указан, анализ запросов с ошибками не выполняется. |
| `-p`              | `--print`         |                         | Продублировать вывод запросов с ошибками в `stdout` (стандартный поток вывода / терминал) |
| `-s n`            | `--stats=n`       | `10`                    | Вывести `n` самых частых запросов, завершившихся со статус кодом `5XX` в порядке их частоты. |
| `-w t`            | `--window=t`      | `0`                     | Найти и вывести промежуток (окно) времени длительностью t секунд, в которое количество запросов было максимально. Eсли t равно 0, расчет не производится. |
| `-f`              | `--from=time`     | Наименьшее время в логе | Время в формате [timestamp](https://www.unixtimestamp.com), начиная с которого происходит анализ данных. |
| `-е`              | `--to=time`       | Наибольшее время в логе | Время в формате [timestamp](https://www.unixtimestamp.com), до которого происходит анализ данных (включительно) |
