#include "Recipe.h"
#include <iostream>

bool Recipe::isValid() {
    if (size_ == 0) {
        return false;
    }
    if (front_->value()->type() != Type::INGRIDIENT) {
        return false;
    } else {
        auto tmp = front_;
        while (tmp->next() != nullptr) {
            if (tmp->value()->type() == Type::INGRIDIENT && tmp->next()->value()->type() == Type::INGRIDIENT) {
                return false;
            }
            tmp = tmp->next();
        }
    }
    return true;
}

bool Recipe::operator==(const Recipe &rhs) {
    if (size_ != rhs.size_) {
        return false;
    } else {
        auto tmp_l = front_;
        auto tmp_r = rhs.front_;
        while (tmp_l != nullptr) {
            if (tmp_l->value()->type() != tmp_r->value()->type()) {
                return false;
            }
            if (*tmp_l->value().get() != *tmp_r->value().get()) {
                return false;
            }
            tmp_l = tmp_l->next();
            tmp_r = tmp_r->next();
        }
    }
    return true;
}

bool Recipe::operator!=(const Recipe &rhs) {
    return !(*this == rhs);
}

void Recipe::print(const std::string &filename) const {
    std::ofstream out;
    out.open(filename);
    auto tmp = front_;
    while (tmp != nullptr) {
        if (tmp->value()->type() == Type::INGRIDIENT) {
            out << "INGRIDIENT " << tmp->value()->title() << " " << tmp->value()->unit() << " " << tmp->value()->quantity() << "\n";
        } else if (tmp->value()->type() == Type::OPERATION) {
            out << "OPERATION " << tmp->value()->action() << " " << tmp->value()->time() << "\n";
        } else {
            out << "ITEM\n";
        }
        tmp = tmp->next();
    }
    out.close();
}

void Recipe::read(const std::string &filename) {
    std::ifstream in;
    in.open(filename);
    if (!in.is_open()) {
        throw std::runtime_error("File not found.");
    }
    std::string type;
    while (in >> type) {
        if (type == "OPERATION") {
            std::string action;
            int time;
            in >> action >> time;
            push(std::make_shared<Operation>(action, time));
        } else {
            std::string title, unit;
            int quantity;
            in >> title >> unit >> quantity;
            push(std::make_shared<Ingridient>(title, unit, quantity));
        }
    }
    in.close();
}

void print(const Recipe &recipe) {
    auto tmp = recipe.front_;
    unsigned i = 0;
    while (tmp != nullptr) {
        std::cout << i++ << ": ";
        if (tmp->value()->type() == Type::INGRIDIENT) {
            std::cout << "INGRIDIENT " << tmp->value()->title() << " " << tmp->value()->unit() << " " << tmp->value()->quantity() << "\n";
        } else if (tmp->value()->type() == Type::OPERATION) {
            std::cout << "OPERATION " << tmp->value()->action() << " " << tmp->value()->time() << "\n";
        } else {
            std::cout << "ITEM\n";
        }
        tmp = tmp->next();
    }
}
