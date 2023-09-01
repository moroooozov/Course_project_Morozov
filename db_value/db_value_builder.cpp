#include "db_value_builder.h"

db_value_builder *db_value_builder::with_meeting_type(std::string &&meeting_type) {
    _meeting_type = std::move(meeting_type);
    return this;
}

db_value_builder *db_value_builder::with_meeting_format(std::string &&meeting_format) {
    _meeting_format = std::move(meeting_format);
    return this;
}

db_value_builder *db_value_builder::with_meeting_description(std::string &&meeting_description) {
    _meeting_description = std::move(meeting_description);
    return this;
}

db_value_builder *db_value_builder::with_link_to_meeting(std::string &&link_to_meeting) {
    _link_to_meeting = std::move(link_to_meeting);
    return this;
}

db_value_builder *db_value_builder::with_name(std::string &&name) {
    _name = std::move(name);
    return this;
}

db_value_builder *db_value_builder::with_surname(std::string &&surname) {
    _surname = std::move(surname);
    return this;
}

db_value_builder *db_value_builder::with_patronymic(std::string &&patronymic) {
    _patronymic = std::move(patronymic);
    return this;
}

db_value_builder *db_value_builder::with_meeting_date(std::string &&meeting_date) {
    _meeting_date = std::move(meeting_date);
    return this;
}

db_value_builder *db_value_builder::with_meeting_time(std::string &&meeting_time) {
    _meeting_time = std::move(meeting_time);
    return this;
}

db_value_builder *db_value_builder::with_duration(int duration) {
    _duration = duration;
    return this;
}

db_value_builder *db_value_builder::with_list_of_invitees(std::string &&list_of_invitees) {
    _list_of_invitees = std::move(list_of_invitees);
    return this;
}


// todo: check if it works
db_value* db_value_builder::build() const {
    return new db_value(_meeting_type,
    _meeting_format,
    _meeting_description,
    _link_to_meeting,
    _surname, _name, _patronymic,
    _meeting_date,
    _meeting_time,
    _duration,
    _list_of_invitees);
}

db_value* db_value_builder::build_from_stream(std::ifstream *input_stream, bool is_cin) {
    std::string token, delimiter = " ";
    size_t pos;
    unsigned delimiter_length = delimiter.length();

    if (is_cin) {
        std::cout << "Enter value:" << std::endl;
        std::cout << "Meeting type:                >>";
        std::getline(std::cin, token);
    } else {
        std::getline((*input_stream), token);
    }
    check_if_string_of_value_is_empty(token);
    this->with_meeting_type(std::move(token));

    if (is_cin) {
        std::cout << "Meeting format:                >>";
        std::getline(std::cin, token);
    } else {
        std::getline((*input_stream), token);
    }
    check_if_string_of_value_is_empty(token);
    this->with_meeting_format(std::move(token));

    if (is_cin) {
        std::cout << "Meeting description:                >>";
        std::getline(std::cin, token);
    } else {
        std::getline((*input_stream), token);
    }
    check_if_string_of_value_is_empty(token);
    this->with_meeting_description(std::move(token));

    if (is_cin) {
        std::cout << "Link to meeting:                >>";
        std::getline(std::cin, token);
    } else {
        std::getline((*input_stream), token);
    }
    check_if_string_of_value_is_empty(token);
    this->with_link_to_meeting(std::move(token));

    if (is_cin) {
        std::cout << "Surname Name Patronymic:              >>";
        std::getline(std::cin, token);
    } else {
        std::getline((*input_stream), token);
    }

    if ((pos = token.find(delimiter)) != std::string::npos) {
        this->with_surname(std::move(token.substr(0, pos)));
        token.erase(0, pos + delimiter_length);
    } else {
        throw db_value::create_exception("db_value_builder::build_from_stream:: incorrect input while building a value");
    }

    if ((pos = token.find(delimiter)) != std::string::npos) {
        this->with_name(std::move(token.substr(0, pos)));
        token.erase(0, pos + delimiter_length);
    } else {
        throw db_value::create_exception("db_value_builder::build_from_stream:: incorrect input while building a value");
    }

    if (token.empty()) {
        throw db_value::create_exception("db_value_builder::build_from_stream:: incorrect input while building a value");
    }
    this->with_patronymic(std::move(token));

    if (is_cin) {
        std::cout << "Meeting date <dd/mm/yyyy>:                >>";
        std::getline(std::cin, token);
    } else {
        std::getline((*input_stream), token);
    }
    check_if_string_of_value_is_empty(token);
    this->with_meeting_date(std::move(token));

    if (is_cin) {
        std::cout << "Meeting time <hh/ii>:                >>";
        std::getline(std::cin, token);
    } else {
        std::getline((*input_stream), token);
    }
    check_if_string_of_value_is_empty(token);
    this->with_meeting_time(std::move(token));

    if (is_cin) {
        std::cout << "Meeting duration <ii>:                >>";
        std::getline(std::cin, token);
    } else {
        std::getline((*input_stream), token);
    }
    check_if_string_of_value_is_empty(token);
    this->with_duration(stoi(token));

    if (is_cin) {
        std::cout << "List of invitees:                >>";
        std::getline(std::cin, token);
    } else {
        std::getline((*input_stream), token);
    }
    check_if_string_of_value_is_empty(token);
    this->with_list_of_invitees(std::move(token));

    return (this->build());
}
