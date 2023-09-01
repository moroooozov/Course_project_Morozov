#ifndef DB_VALUE_H
#define DB_VALUE_H

#include "string_holder.h"

typedef enum fields {
    _meeting_type_,
    _meeting_format_,
    _meeting_description_,
    _link_to_meeting_,
    _surname_,
    _name_,
    _patronymic_,
    _meeting_date_,
    _meeting_time_,
    _duration_,
    _list_of_invitees_
} db_value_fields;

class handler;

class db_value {
public:
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

private:

    std::string * _meeting_type;
    std::string * _meeting_format;
    std::string * _meeting_description;
    std::string * _link_to_meeting;
    std::string * _surname, * _name, * _patronymic;
    std::string * _meeting_date;
    std::string * _meeting_time;
    int _duration;
    std::string * _list_of_invitees;

    uint64_t _timestamp;

    handler * _chain_of_resp;
    handler * _last_handler;

    friend class db_value_builder;
    friend class update_command;
    friend class add_command;
    friend class add_handler;

public:
    handler * get_first_handler();
    handler * get_last_handler();
    void add_new_handler(handler * handler_);

public:
    friend std::ostream &operator<<(std::ostream &out, const db_value &value);

    db_value * make_a_copy()
    {
        db_value * copy = new db_value((*(this->_meeting_type)), (*(this->_meeting_format)), (*(this->_meeting_description)),
                                       (*(this->_link_to_meeting)), (*(this->_surname)), (*(this->_name)),
                                       (*(this->_patronymic)), (*(this->_meeting_date)), (*(this->_meeting_time)), this->_duration,
                                       (*(this->_list_of_invitees)));
        copy->_timestamp = this->_timestamp;
        copy->_chain_of_resp = nullptr;
        copy->_last_handler = nullptr;
        return copy;
    }

private:
    static std::string * get_ptr_from_string_holder(std::string const & s)
    {
        return string_holder::get_instance()->get_string(s);
    }

    static void remove_string_from_string_holder(std::string const & s)
    {
        string_holder::get_instance()->remove_string(s);
    }

    db_value(std::string const & meeting_type, std::string const & meeting_format, std::string const & meeting_description,
             std::string const & link_to_meeting, std::string const & surname, std::string const & name,
             std::string const & patronymic, std::string const & meeting_date, std::string const & meeting_time,
             int duration, std::string const & list_of_invitees)
            : _duration(duration)
    {
        _meeting_type = get_ptr_from_string_holder(meeting_type);
        _meeting_format = get_ptr_from_string_holder(meeting_format);
        _meeting_description = get_ptr_from_string_holder(meeting_description);
        _link_to_meeting = get_ptr_from_string_holder(link_to_meeting);
        _surname = get_ptr_from_string_holder(surname);
        _name = get_ptr_from_string_holder(name);
        _patronymic = get_ptr_from_string_holder(patronymic);
        _meeting_date = get_ptr_from_string_holder(meeting_date);
        _meeting_time = get_ptr_from_string_holder(meeting_time);
        _list_of_invitees = get_ptr_from_string_holder(list_of_invitees);

        _timestamp = duration_cast<std::chrono::milliseconds>
                (std::chrono::system_clock::now().time_since_epoch()).count();
        _chain_of_resp = nullptr;
        _last_handler = nullptr;
    }

public:
    ~db_value();
};

inline std::ostream &operator<<(std::ostream &out, const db_value &value) {
    out << (*(value._meeting_type)) << std::endl;
    out << (*(value._meeting_format)) << std::endl;
    out << (*(value._meeting_description)) << std::endl;
    out << (*(value._link_to_meeting)) << std::endl;
    out << (*(value._surname)) << " " << (*(value._name)) << " " << (*(value._patronymic)) << std::endl;
    out << (*(value._meeting_date)) << std::endl;
    out << (*(value._meeting_time)) << std::endl;
    out << value._duration << std::endl;
    out << (*(value._list_of_invitees)) << std::endl;

    return out;
}

#endif //DB_VALUE_H
