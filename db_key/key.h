#ifndef KEY_H
#define KEY_H

#include <iostream>
#include <fstream>

// Класс ключа для базы данных
class key {
    int _meeting_id{};  // Идентификатор встречи

    friend class key_comparer;  // Дружественный класс для сравнения ключей
    friend std::ostream &operator<<(std::ostream &out, const key &_key_);

public:
    // Исключение, возникающее при ошибке создания ключа
    class create_exception final : public std::exception {
    private:
        std::string _message;

    public:
        explicit create_exception(std::string message)
                : _message(std::move(message)) {
        }

        [[nodiscard]] char const *what() const noexcept override {
            return _message.c_str();
        }
    };

    // Конструктор ключа из потока ввода или файла
    key(std::ifstream *input_stream, bool is_cin) {
        std::string token;

        if (is_cin) {
            std::cout << "Введите идентификатор встречи: >>";
            std::getline(std::cin, token);
        } else {
            std::getline((*input_stream), token);
        }

        if (token.empty()) {
            throw key::create_exception("Ошибка создания ключа: некорректный ввод идентификатора");
        }

        this->_meeting_id = std::stoi(token);
    }

    // Конструктор ключа по умолчанию
    key() = default;

    // Конструктор ключа с явно указанным идентификатором встречи
    key(int meeting_id)
            : _meeting_id(meeting_id) {
    }

    // Конструктор копирования ключа
    key(key const &obj)
            : key(obj._meeting_id) {
    }

    // Конструктор перемещения ключа
    key(key &&obj) noexcept
            : key(obj._meeting_id) {
        obj._meeting_id = 0;
    }

    // Оператор присваивания для копирования
    key &operator=(key const &obj) {
        if (this == &obj) {
            return *this;
        }

        _meeting_id = obj._meeting_id;

        return *this;
    }

    // Оператор присваивания для перемещения
    key &operator=(key &&obj) noexcept {
        if (this == &obj) {
            return *this;
        }

        _meeting_id = obj._meeting_id;
        obj._meeting_id = 0;

        return *this;
    }

    // Деструктор ключа по умолчанию
    ~key() = default;
};

// Оператор вывода ключа в поток
inline std::ostream &operator<<(std::ostream &out, const key &_key_) {
    out << _key_._meeting_id << std::endl;
    return out;
}

// Компаратор ключей для сравнения
class key_comparer {
public:
    int operator()(key x, key y) {
        return x._meeting_id - y._meeting_id;
    }
};

#endif //KEY_H