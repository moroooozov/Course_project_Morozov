#ifndef DB_VALUE_BUILDER_H
#define DB_VALUE_BUILDER_H

#include "db_value.h"

class db_value_builder {
    std::string _meeting_type;
    std::string _meeting_format;
    std::string _meeting_description;
    std::string _link_to_meeting;
    std::string _surname, _name, _patronymic;
    std::string _meeting_date;
    std::string _meeting_time;
    int _duration;
    std::string _list_of_invitees;

public:
    db_value_builder * with_meeting_type(std::string && meeting_type);
    db_value_builder * with_meeting_format(std::string && meeting_format);
    db_value_builder * with_meeting_description(std::string && meeting_description);
    db_value_builder * with_link_to_meeting(std::string && link_to_meeting);
    db_value_builder * with_name(std::string && name);
    db_value_builder * with_surname(std::string && surname);
    db_value_builder * with_patronymic(std::string && patronymic);
    db_value_builder * with_meeting_date(std::string && meeting_date);
    db_value_builder * with_meeting_time(std::string && meeting_time);
    db_value_builder * with_duration(int duration);
    db_value_builder * with_list_of_invitees(std::string && list_of_invitees);

    [[nodiscard]] db_value *build() const;

private:
    static void check_if_string_of_value_is_empty(std::string const& s)
    {
        if (s.empty()) {
            throw db_value::create_exception("db_user_communication:: incorrect input while building a value");
        }
    }

public:
    db_value *build_from_stream(std::ifstream *input_stream, bool is_cin);

};

#endif //DB_VALUE_BUILDER_H
