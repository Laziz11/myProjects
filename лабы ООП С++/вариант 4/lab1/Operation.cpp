#include "Operation.h"

Operation::Operation(const std::string &action, int time) :
        action_(recogAction(action)),
        time_(time >= 0 ? time : 0) {}


Operation::Operation(Action action, unsigned time) :
        action_(action),
        time_(time) {}

Operation::Operation(Operation &&other) noexcept :
        action_(other.action_),
        time_(other.time_) {}

Operation &Operation::operator=(Operation &&other) noexcept {
    *this = std::move(other);
    return *this;
}

Action Operation::__action() const {
    return action_;
}

unsigned Operation::time() const {
    return time_;
}

void Operation::setAction(const std::string &new_action) {
    action_ = recogAction(new_action);
}

void Operation::setTime(int new_time) {
    time_ = new_time >= 0 ? new_time : 0;
}

Type Operation::type() {
    return Type::OPERATION;
}

Action Operation::recogAction(const std::string &action) {
    if (action == "CUT") {
        return Action::CUT;
    } else if (action == "PACK") {
        return Action::PACK;
    } else if (action == "FRY") {
        return Action::FRY;
    } else if (action == "COOK") {
        return Action::COOK;
    } else if (action == "PUT") {
        return Action::PUT;
    } else {
        return Action::ACTION_INDEF;
    }
}

bool Operation::operator==(const Operation &rhs) {
    return action_ == rhs.action_ && time_ == rhs.time_;
}

bool Operation::operator!=(const Operation &rhs) {
    return action_ != rhs.action_ || time_ != rhs.time_;
}

std::string Operation::action() const {
    std::string r_action;
    switch (action_) {
        case Action::CUT:
            r_action = "CUT";
            break;
        case Action::PACK:
            r_action = "PACK";
            break;
        case Action::FRY:
            r_action = "FRY";
            break;
        case Action::COOK:
            r_action = "COOK";
            break;
        case Action::PUT:
            r_action = "PUT";
            break;
        case Action::ACTION_INDEF:
            r_action = "ACTION_INDEF";
            break;
    }
    return r_action;
}

