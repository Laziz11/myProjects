#pragma once

#include <string>

enum class Type {
    OPERATION,
    INGRIDIENT,
    ITEM
};

enum class Action {
    CUT,
    PACK,
    FRY,
    COOK,
    PUT,
    ACTION_INDEF
};

enum class Unit {
    GRAMM,
    MILLILITER,
    PIECE,
    UNIT_INDEF
};

class Item {
public:
    Item() = default;

    virtual ~Item() = default;

    virtual Type type() {
        return Type::ITEM;
    }

    [[nodiscard]] virtual Action __action() const { return Action::ACTION_INDEF; }

    [[nodiscard]] virtual std::string action() const { return title_; }

    [[nodiscard]] virtual unsigned time() const { return 0; }

    virtual void setAction(const std::string &new_action) {}

    virtual void setTime(int time) {}

    [[nodiscard]] virtual const std::string &title() const { return title_; }

    [[nodiscard]] virtual Unit __unit() const { return Unit::UNIT_INDEF; }

    [[nodiscard]] virtual std::string unit() const { return title_; }

    [[nodiscard]] virtual unsigned quantity() const { return 0; }

    virtual void setTitle(const std::string &new_title) {}

    virtual void setUnit(const std::string &new_unit) {}

    virtual void setQuantity(int new_quantity) {}

    bool operator==(const Item &rhs) { return true; };

    bool operator!=(const Item &rhs) { return false; };

private:
    std::string title_{""};
};
