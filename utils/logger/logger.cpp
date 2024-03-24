//
// Created by ne1mnn on 2/27/2024.
//

#include "logger.h"

const std::unordered_map<scl::logger::message_type, std::string> scl::logger::_message_type_str = {
    {message_type::INFO, "+"}, {message_type::WARNING, "?"}, {message_type::ERR, "-"}};

scl::logger::logger() : logger("LOGGER") {}

scl::logger::logger(std::string name) : _name(std::move(name)) {}

const std::string& scl::logger::get_name() const { return _name; }

void scl::logger::set_name(std::string name) { _name = std::move(name); }