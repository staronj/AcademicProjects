
#ifndef _PERSON_H
#define _PERSON_H

#include "IdType.h"
#include <exception>
#include <memory>
#include <string>
#include <vector>

class Person {
public:
    Person(IdType id) : id_(id) {}
    virtual std::string name() const = 0;
    IdType id() const { return id_; };
    virtual ~Person() = default;

private:
    IdType id_;
};

class Human : public Person {
public:
    Human(std::string name, IdType id) : Person(id), name_(name) {}

    std::string name() const { return name_; };
private:
    std::string name_;
};

class Klingon : public Person {
public:
    Klingon(std::string name, IdType id) : Person(id), name_(name) {}

    std::string name() const override { return name_; };

private:
    std::string name_;
};

class Binar : public Person {
public:
    Binar(IdType id) : Person(id) {}
};

class SingleBinar : public Binar {
public:
    SingleBinar(std::string name, IdType id) : Binar(id), name_(name) {}

    std::string name() const override { return name_; };

private:
    std::string name_;
};

class DoubleBinar : public Binar {
public:
    DoubleBinar(Binar& first, Binar& second, IdType id)
        : Binar(id), first_(&first), second_(&second) {}

    std::string name() const override { return first_->name() + '&' + second_->name(); };

private:
    Binar *first_, *second_;
};

#endif  // _PERSON_H
