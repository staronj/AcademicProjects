#ifndef _PLANETS_H
#define _PLANETS_H

#include "IdType.h"
#include "Person.h"
#include <memory>

class PersonNotFound : public std::exception {
public:
    PersonNotFound() = default;
    const char* what() const noexcept override { return "person not found"; }
};

namespace detail {
template <class PersonType>
class CitizenStorage {
public:
    using person_ptr = std::unique_ptr<PersonType>;

    PersonType& insert(PersonType* ptr) {
        citizens_.emplace_back(ptr);
        return *citizens_.back();
    }

    PersonType& findById(IdType id) {
        for (const person_ptr& ptr : citizens_) {
            if (ptr->id() == id)
                return *ptr;
        }
        throw PersonNotFound();
    }

    PersonType& findByName(const std::string& name) {
        for (const person_ptr& ptr : citizens_) {
            if (ptr->name() == name)
                return *ptr;
        }
        throw PersonNotFound();
    }

private:
    std::vector<person_ptr> citizens_;
};
}  // detail

template <typename Creature>
class Planet {
public:
    Creature& registerCitizen(std::string name) {
        return storage_.insert(new Creature(std::move(name), getId()));
    }
    Creature& findCitizen(IdType id) { return storage_.findById(id); }

protected:
    IdType getId() { return IdType(next_free_id_++); }

    Planet() : next_free_id_(0){};
    int next_free_id_;
    detail::CitizenStorage<Creature> storage_;
};

class Earth : public Planet<Human> {
    friend Earth& earth();
};

class Qonos : public Planet<Klingon> {
    friend Qonos& qonos();
};

class Bynaus : public Planet<Binar> {
public:
    Binar& registerCitizen(std::string name) {
        return storage_.insert(new SingleBinar(std::move(name), getId()));
    }
    Binar& registerCitizen(Binar& first, Binar& second) {
        return storage_.insert(new DoubleBinar(first, second, getId()));
    }
};

Earth& earth();
Qonos& qonos();
Bynaus& bynaus();

#endif  // _PLANETS_H
