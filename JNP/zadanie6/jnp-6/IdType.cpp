#include "IdType.h"
#include <string>

IdType::IdType(const std::string& id) : id_(id) {}

IdType::IdType(const char* id) : id_(id) {}

IdType::IdType(int id) :id_(std::to_string(id)) { }

std::ostream& operator<<(std::ostream& os, const IdType& id) {
    os << id.id_;
    return os;
}
