#include "Ingridient.h"

#include <utility>

Ingridient::Ingridient(std::string title, const std::string &unit, int quantity) :
        title_(std::move(title)),
        unit_(recogUnit(unit)),
        quantity_(quantity >= 0 ? quantity : 0) {}

Ingridient::Ingridient(Ingridient &&other) noexcept :
        title_(std::move(other.title_)),
        unit_(other.unit_),
        quantity_(other.quantity_) {}

Ingridient &Ingridient::operator=(Ingridient &&other) noexcept {
    *this = std::move(other);
    return *this;
}

const std::string &Ingridient::title() const {
    return title_;
}

Unit Ingridient::__unit() const {
    return unit_;
}

unsigned Ingridient::quantity() const {
    return quantity_;
}

void Ingridient::setTitle(const std::string &new_title) {
    title_ = new_title;
}

void Ingridient::setUnit(const std::string &new_unit) {
    unit_ = recogUnit(new_unit);
}

void Ingridient::setQuantity(const int new_quantity) {
    quantity_ = new_quantity >= 0 ? new_quantity : 0;
}

Unit Ingridient::recogUnit(const std::string &unit) {
    if (unit == "GRAMM") {
        return Unit::GRAMM;
    } else if (unit == "MILLILITER") {
        return Unit::MILLILITER;
    } else if (unit == "PIECE") {
        return Unit::PIECE;
    } else {
        return Unit::UNIT_INDEF;
    }
}

enum Type Ingridient::type() {
    return Type::INGRIDIENT;
}

bool Ingridient::operator==(const Ingridient &rhs) {
    return quantity_ == rhs.quantity_ && unit_ == rhs.unit_ && title_ == rhs.title_;
}

bool Ingridient::operator!=(const Ingridient &rhs) {
    return quantity_ != rhs.quantity_ || unit_ != rhs.unit_ || title_ != rhs.title_;
}

std::string Ingridient::unit() const {
    std::string r_unit;
    switch (unit_) {
        case Unit::GRAMM:
            r_unit = "GRAMM";
            break;
        case Unit::MILLILITER:
            r_unit = "MILLILETER";
            break;
        case Unit::PIECE:
            r_unit = "PIECE";
            break;
        case Unit::UNIT_INDEF:
            r_unit = "UNIT_INDEF";
            break;
    }
    return r_unit;
}

