#pragma once

#include "Item.h"

#include <string>

class Operation : public Item {
public:
    Operation() = default;

    explicit Operation(const std::string &action, int time);

    explicit Operation(Action action, unsigned time);

    Operation(const Operation &other) = default;

    Operation &operator=(const Operation &other) = default;

    Operation(Operation &&other) noexcept;

    Operation &operator=(Operation &&other) noexcept;

    ~Operation() override = default;

    [[nodiscard]] Action __action() const override;

    [[nodiscard]] std::string action() const override;

    [[nodiscard]] unsigned time() const override;

    void setAction(const std::string &new_action) override;

    void setTime(int new_time) override;

    Type type() override;

    bool operator==(const Operation &rhs);

    bool operator!=(const Operation &rhs);

private:
    static Action recogAction(const std::string &action);

    Action action_ = Action::ACTION_INDEF;
    unsigned time_ = 0;
};
