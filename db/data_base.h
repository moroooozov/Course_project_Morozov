#ifndef DATA_BASE_H
#define DATA_BASE_H

#include <iostream>
#include <tuple>
#include <vector>

#include "../allocator/memory_holder.h"
#include "../allocator_from_global_heap/memory_from_global_heap.h"
#include "../allocator_with_sorted_list_deallocation/memory_with_sorted_list_deallocation.h"
#include "../allocator_with_boundary_tags_deallocation/memory_with_boundary_tags.h"
#include "../allocator_with_buddy_system/memory_with_buddy_system.h"

#include "../logger/logger_holder.h"

#include "../binary_tree/associative_container.h"
#include "../binary_tree/bs_tree.h"
#include "../avl_tree/avl_tree.h"
#include "../splay_tree/splay_tree.h"
#include "../rb_tree/rb_tree.h"
#include "../b_tree/b_tree.h"

#include "../chain_of_resp_and_command/handler.h"
#include "../db_key/key.h"

class data_base final :
        private memory_holder,
        private logger_holder
{
private:
private:
    // Внутренний класс string_comparer для сравнения строк
    class string_comparer
    {
    public:
        int operator()(std::string const & x, std::string const & y) {
            return x.compare(y);  // Метод сравнения строк
        }
    };

private:
    /* Иерархия структур:
     *     дерево пулов
     *          дерево схем
     *               дерево коллекций
     *                   коллекция
     */
    splay_tree<std::string,
            associative_container<std::string,
                    associative_container<std::string,
                            associative_container<key, db_value *> *
                    > *
            > *, string_comparer
    > * _database;  // Указатель на дерево пулов
    logger * _logger;
    memory* _allocator;
    std::map<std::string, memory *> _all_trees_allocators;  // Карта аллокаторов для всех деревьев

public:
    typedef enum trees_types {
        BST,
        AVL,
        SPLAY,
        RB,
        B,
        not_a_tree
    } trees_types_;

    typedef enum allocator_types {
        global,
        for_inner_use_sorted_list,
        sorted_list_best,
        sorted_list_worst,
        sorted_list_first,
        for_inner_use_descriptors,
        descriptors_best,
        descriptors_worst,
        descriptors_first,
        buddy_system,
        not_an_allocator
    } allocator_types_;

#pragma region exceptions
public:
    // Класс исключения db_insert_exception для ошибок вставки
    class db_insert_exception final : public std::exception {
    private:
        std::string _message;  // Сообщение об ошибке

    public:
        explicit db_insert_exception(std::string message)
                : _message(std::move(message)) {

        }

        [[nodiscard]] char const *what() const noexcept override {
            return _message.c_str();  // Метод для получения сообщения об ошибке
        }
    };

    // Класс исключения db_find_exception для ошибок поиска
    class db_find_exception final : public std::exception {
    private:
        std::string _message;  // Сообщение об ошибке

    public:
        explicit db_find_exception(std::string message)
                : _message(std::move(message)) {

        }

        [[nodiscard]] char const *what() const noexcept override {
            return _message.c_str();  // Метод для получения сообщения об ошибке
        }
    };

    // Класс исключения db_remove_exception для ошибок удаления
    class db_remove_exception final : public std::exception {
    private:
        std::string _message;  // Сообщение об ошибке

    public:
        explicit db_remove_exception(std::string message)
                : _message(std::move(message)) {

        }

        [[nodiscard]] char const *what() const noexcept override {
            return _message.c_str();  // Метод для получения сообщения об ошибке
        }
    };
#pragma endregion
#pragma region Find structure
private:
    // Метод для поиска пула данных по имени пула
    [[nodiscard]] associative_container<std::string, associative_container<std::string, associative_container<key, db_value *> *> *> *
    find_data_pool
            (std::string const & pool_name) const;

    // Метод для поиска схемы данных внутри пула по имени пула и схемы
    [[nodiscard]] associative_container<std::string, associative_container<key, db_value *> *> *
    find_data_scheme
            (std::string const & pool_name, std::string const & scheme_name) const;

    // Метод для поиска коллекции данных внутри схемы по имени пула, схемы и коллекции
    [[nodiscard]] associative_container<key, db_value *> *
    find_data_collection
            (std::string const & pool_name, std::string const & scheme_name, std::string const & collection_name) const;

#pragma endregion

#pragma region Collection-related functions
#pragma region Insertion in collection
public:
    // Метод для добавления значения в коллекцию
    void add_to_collection
            (std::string const & pool_name, std::string const & scheme_name, std::string const & collection_name,
             const key& _key, db_value * value) const;

#pragma endregion

#pragma region Updating a collection value
public:
    // Метод для обновления значения в коллекции
    void update_in_collection
            (std::string const & pool_name, std::string const & scheme_name, std::string const & collection_name,
             const key& _key, std::map<db_value_fields, unsigned char *> upd_dict) const;
#pragma endregion

#pragma region Finding among collection
public:
    // Метод для поиска значения в коллекции по ключу
    [[nodiscard]] db_value * find_among_collection
            (std::string const & pool_name, std::string const & scheme_name, std::string const & collection_name,
             const key& _key) const;

    // Метод для поиска значения в коллекции с учетом времени
    [[nodiscard]] db_value * find_with_time
            (std::string const & pool_name, std::string const & scheme_name, std::string const & collection_name,
             db_value * found_value, const key& _key, uint64_t time_parameter) const;

    // Метод для поиска значений в диапазоне ключей в коллекции
    [[nodiscard]] std::vector<db_value *> find_in_range
            (std::string const & pool_name, std::string const & scheme_name, std::string const & collection_name,
             key min_key, key max_key) const;
#pragma endregion

#pragma region Deletion from collection
public:
    // Метод для удаления значения из коллекции по ключу
    void delete_from_collection
            (std::string const & pool_name, std::string const & scheme_name, std::string const & collection_name,
             const key& key) const;
#pragma endregion
#pragma endregion

#pragma region Structure functions
#pragma region Inserting in structure of data base
    // Метод для получения нового аллокатора для внутренних деревьев
    // в соответствии с переданными параметрами
private:
    memory *
    get_new_allocator_for_inner_trees
            (std::string const & pool_name, std::string const & scheme_name, std::string const & collection_name,
             allocator_types_ allocator_type, size_t allocator_pool_size);

public:
    // Метод для добавления структуры в базу данных
    void add_to_structure
            (std::string const & pool_name, std::string const & scheme_name, std::string const & collection_name,
             trees_types_ tree_type, allocator_types_ allocator_type, size_t allocator_pool_size);

    // Перегруженный метод для добавления структуры в базу данных
    void add_to_structure
            (std::string const  & pool_name, std::string const & scheme_name,std::string const & collection_name,
             unsigned tree_parameter, allocator_types_ allocator_type, size_t allocator_pool_size);

#pragma endregion

#pragma region Deletion from structure of data base
private:
    // Метод для удаления коллекции из схемы
    void delete_collection
            (const std::string & collection_name, associative_container<std::string,
                    associative_container<key, db_value *> *> * parent_scheme);

    // Метод для удаления схемы из пула
    void delete_scheme
            (const std::string & scheme_name, associative_container<std::string,
                    associative_container<std::string, associative_container<key, db_value *> *> *> * parent_pool);

    // Метод для удаления пула из базы данных
    void delete_pool(const std::string & pool_name);

public:
    // Метод для удаления структуры из базы данных
    void delete_from_structure
            (std::string const & pool_name, std::string const & scheme_name, std::string const & collection_name);
#pragma endregion
#pragma endregion

#pragma region logger_holder and memory_holder contract
    // Метод для получения логгера
    [[nodiscard]] logger *get_logger() const noexcept override
    {
        return this->_logger;
    }

    // Метод для получения аллокатора
    [[nodiscard]] memory *get_memory() const noexcept override
    {
        return this->_allocator;
    }
#pragma endregion

#pragma region Constructor and Destructor
public:
    // Конструктор класса data_base
    explicit data_base(logger * this_db_logger = nullptr, memory * this_db_allocator = nullptr)
            : _logger(this_db_logger), _allocator(this_db_allocator)
    {
        // Создание дерева пулов с заданным логгером и аллокатором
        _database = new splay_tree<std::string,
                associative_container<std::string,
                        associative_container<std::string,
                                associative_container<key, db_value *> *> *> *
                , string_comparer>(this_db_logger, this_db_allocator);
    }

    // Деструктор класса data_base
    ~data_base() override;

    // Запрет копирования объектов класса
    data_base(data_base const &obj) = delete;

    // Запрет перемещения объектов класса
    data_base(data_base &&obj) noexcept = delete;

    // Запрет оператора копирования
    data_base &operator=(data_base const &obj) = delete;

    // Запрет оператора перемещения
    data_base &operator=(data_base &&obj) noexcept = delete;

#pragma endregion

    // Метод для поиска набора данных с учетом времени
    std::vector<db_value *>
    find_dataset_with_time(const std::string &pool_name, const std::string &scheme_name,
                           const std::string &collection_name,
                           key min_key, key max_key, uint64_t time_parameter);

};

#endif //DATA_BASE_H