#ifndef _ID_TYPE_H
#define _ID_TYPE_H

#include <string>

class IdType {
public:
    IdType() = default;
    IdType(const std::string& id);
    IdType(const char* id);
    explicit IdType(int id);

    bool operator==(const IdType& other) const { return id_ == other.id_; }
    bool operator!=(const IdType& other) const { return !(*this == other); }
    bool operator<(const IdType& other) const { return id_ < other.id_; }
    bool operator>(const IdType& other) const { return !(*this < other); }
    bool operator<=(const IdType& other) const { return !(*this > other); }
    bool operator>=(const IdType& other) const { return !(*this < other); }

private:
    friend std::ostream& operator<<(std::ostream& os, const IdType&);
    std::string id_;
};

std::ostream& operator<<(std::ostream& os, const IdType&);

#endif  // _ID_TYPE_H
