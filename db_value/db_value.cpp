#include "../chain_of_resp_and_command/handler.h"

// Getter для первого обработчика
handler* db_value::get_first_handler() {
    return _chain_of_resp;
}

// Getter для последнего обработчика
handler* db_value::get_last_handler() {
    return _last_handler;
}

// Добавление нового обработчика в цепочку
void db_value::add_new_handler(handler* handler_) {
    if (!_chain_of_resp) {
        _chain_of_resp = handler_;
    }

    if (_last_handler) {
        _last_handler->set_next(handler_);
    }
    _last_handler = handler_;
}

// Деструктор класса
db_value::~db_value() {
    // Освобождение строк из хранилища строк
    remove_string_from_string_holder(*_meeting_type);
    remove_string_from_string_holder(*_meeting_format);
    remove_string_from_string_holder(*_meeting_description);
    remove_string_from_string_holder(*_link_to_meeting);
    remove_string_from_string_holder(*_surname);
    remove_string_from_string_holder(*_name);
    remove_string_from_string_holder(*_patronymic);
    remove_string_from_string_holder(*_meeting_date);
    remove_string_from_string_holder(*_meeting_time);
    remove_string_from_string_holder(*_list_of_invitees);

    // Освобождение цепочки обработчиков
    if (_chain_of_resp) {
        _chain_of_resp->delete_chain_of_responsibility();
    }
}