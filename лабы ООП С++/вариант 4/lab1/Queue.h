#pragma once

#include <memory>
#include <stdexcept>

template<typename T>
class Queue {
public:

    Queue() = default;

    Queue(const Queue &other_);

    Queue(Queue &&other_) noexcept = default;

    ~Queue() = default;

    Queue &operator=(const Queue &other_);

    Queue &operator=(Queue &&other_) noexcept = default;

    T &front();

    const T &front() const;

    T &back();

    const T &back() const;

    void pop();

    void push(const T &value_);

    void emplace(T &&value_);

    [[nodiscard]] size_t size() const;

    [[nodiscard]] bool empty() const;

    void clear();

    bool operator==(const Queue &rhs) const;

    bool operator!=(const Queue &rhs) const;

protected:
    class QueueElement {
    public:
        QueueElement() = delete;

        QueueElement(const T &value_, std::shared_ptr<QueueElement> next_) :
                value_{value_}, next_{next_} {}

        QueueElement(T &&value_, std::shared_ptr<QueueElement> next_) :
                value_{std::move(value_)}, next_{next_} {}

        T &value() {
            return value_;
        }

        std::shared_ptr<QueueElement> &next() {
            return next_;
        }

    private:
        T value_;
        std::shared_ptr<QueueElement> next_{nullptr};
    };

    std::shared_ptr<QueueElement> front_{nullptr};
    std::shared_ptr<QueueElement> back_{nullptr};
    size_t size_{0};
};

template<typename T>
Queue<T>::Queue(const Queue &other_) {
    auto tmp = other_.front_;
    while (tmp != nullptr) {
        push(tmp->value());
        tmp = tmp->next();
    }
}

template<typename T>
Queue<T> &Queue<T>::operator=(const Queue &other_) {
    auto tmp = other_.front_;
    while (tmp != nullptr) {
        push(tmp->value());
        tmp = tmp->next();
    }
    return *this;
}

template<typename T>
T &Queue<T>::front() {
    if (front_ == nullptr) {
        throw std::out_of_range("front() of empty queue");
    }
    return front_->value();
}

template<typename T>
const T &Queue<T>::front() const {
    if (front_ == nullptr) {
        throw std::out_of_range("front() of empty queue");
    }
    return front_->value();
}


template<typename T>
T &Queue<T>::back() {
    if (back_ == nullptr) {
        throw std::out_of_range("back() of empty queue");
    }
    return back_->value();
}

template<typename T>
const T &Queue<T>::back() const {
    if (back_ == nullptr) {
        throw std::out_of_range("back() of empty queue");
    }
    return back_->value();
}

template<typename T>
void Queue<T>::pop() {
    if (front_ != nullptr) {
        front_ = front_->next();
        --size_;
    }
}

template<typename T>
void Queue<T>::push(const T &value_) {
    if (back_ == nullptr) {
        back_ = front_ = std::make_shared<QueueElement>(value_, nullptr);
    } else {
        back_->next() = std::make_shared<QueueElement>(value_, nullptr);
        back_ = back_->next();
    }
    ++size_;
}

template<typename T>
void Queue<T>::emplace(T &&value_) {
    if (back_ == nullptr) {
        back_ = front_ = std::make_shared<QueueElement>(std::move(value_), nullptr);
    } else {
        back_->next() = std::make_shared<QueueElement>(std::move(value_), nullptr);
        back_ = back_->next();
    }
    ++size_;
}

template<typename T>
size_t Queue<T>::size() const {
    return size_;
}

template<typename T>
bool Queue<T>::empty() const {
    return front_ == nullptr;
}

template<typename T>
void Queue<T>::clear() {
    front_ = nullptr;
    back_ = nullptr;
    size_ = 0;
}

template<typename T>
bool Queue<T>::operator==(const Queue &rhs) const {
    if (size_ != rhs.size_) {
        return false;
    } else {
        auto tmp_l = front_;
        auto tmp_r = rhs.front_;
        while (tmp_l != nullptr) {
            if (tmp_l->value() != tmp_r->value()) {
                return false;
            }
            tmp_r = tmp_r->next();
            tmp_l = tmp_l->next();
        }
    }
    return true;
}

template<typename T>
bool Queue<T>::operator!=(const Queue &rhs) const {
    return !(*this == rhs);
}
