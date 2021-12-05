#pragma once

#include "Item.h"

#include <string>

class Ingridient : public Item {
public:
    Ingridient() = default;

    explicit Ingridient(std::string title, const std::string &unit, int quantity);

    Ingridient(const Ingridient &other) = default;

    Ingridient &operator=(const Ingridient &other) = default;

    Ingridient(Ingridient &&other) noexcept;

    Ingridient &operator=(Ingridient &&other) noexcept;

    ~Ingridient() override = default;

    [[nodiscard]] const std::string &title() const override;

    [[nodiscard]] Unit __unit() const override;

    [[nodiscard]] std::string unit() const override;

    [[nodiscard]] unsigned quantity() const override;

    void setTitle(const std::string &new_title) override;

    void setUnit(const std::string &new_unit) override;

    void setQuantity(int new_quantity) override;

    Type type() override;

    bool operator==(const Ingridient &rhs);

    bool operator!=(const Ingridient &rhs);

private:
    static Unit recogUnit(const std::string &unit);

    std::string title_{"."};
    Unit unit_{Unit::UNIT_INDEF};
    unsigned quantity_{0};
};

