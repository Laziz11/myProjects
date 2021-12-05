#pragma once

#include <algorithm>
#include <forward_list>
#include <vector>
#include <fstream>

template<class Type, class Hasher = std::hash<Type>>
class u_map {
    friend class MainWindow;
public:
    u_map() = default;

    explicit u_map(size_t num_buckets);

    explicit u_map(Hasher h_, size_t num_buckets_ = 1000);

    u_map(const u_map &other_) = default;

    u_map(u_map &&other_) noexcept = default;

    u_map &operator=(const u_map &other_) = default;

    u_map &operator=(u_map &&other) noexcept = default;

    ~u_map() = default;

    void clear();

    size_t size() const;

    bool empty() const;

    bool has(const Type &key) const;

    bool operator==(const u_map &rhs) const;

    bool operator!=(const u_map &rhs) const;

    u_map &operator<<(const Type &key);

    u_map &operator<<(Type &&key);

    size_t operator[](const Type &key) const;

    void erase(const Type &key);

    Type min() const;

    Type max() const;

    void print(const std::string &filename) const;

    void read(const std::string &filename);

private:
    size_t index(const Type &key) const;

    bool inc(size_t idx, const Type &key);

    template<class Func>
    Type extr(Func comp) const;

    Hasher hasher_{Hasher()};
    std::vector<std::forward_list<std::pair<Type, size_t>>> buckets_{1000};
    size_t size_{0};
};


template<class Type, class Hasher>
size_t u_map<Type, Hasher>::index(const Type &key) const {
    return hasher_(key) % buckets_.size();
}


template<class Type, class Hasher>
bool u_map<Type, Hasher>::inc(size_t idx, const Type &key) {
    for (auto &[k_, v_] : buckets_[idx]) {
        if (k_ == key) {
            ++v_;
            return true;
        }
    }
    return false;
}


template<class Type, class Hasher>
template<class Func>
Type u_map<Type, Hasher>::extr(Func comp) const {
    Type key;
    size_t value = comp(0, 1) ? -1 : 0;
    if (size_ == 0) {
        throw std::logic_error("Container are empty.");
    }
    for (const auto &bucket : buckets_) {
        if (!bucket.empty()) {
            for (const auto &[k_, v_] : bucket) {
                if (comp(v_, value)) {
                    value = v_;
                    key = k_;
                }
            }
        }
    }
    return key;
}


template<class Type, class Hasher>
u_map<Type, Hasher>::u_map(size_t num_buckets_) :
        buckets_(num_buckets_) {}


template<class Type, class Hasher>
u_map<Type, Hasher>::u_map(Hasher h_, size_t num_buckets_) :
        hasher_(h_), buckets_(num_buckets_) {}


template<class Type, class Hasher>
void u_map<Type, Hasher>::clear() {
    size_t num_buckets = buckets_.size();
    buckets_.clear();
    buckets_.resize(num_buckets);
    size_ = 0;
}


template<class Type, class Hasher>
size_t u_map<Type, Hasher>::size() const {
    return size_;
}


template<class Type, class Hasher>
bool u_map<Type, Hasher>::empty() const {
    return (size_ == 0);
}


template<class Type, class Hasher>
bool u_map<Type, Hasher>::has(const Type &key) const {
    size_t idx = index(key);
    return std::any_of(buckets_[idx].begin(), buckets_[idx].end(), [&key](const auto &x) {
        return x.first == key;
    });
}


template<class Type, class Hasher>
bool u_map<Type, Hasher>::operator==(const u_map &rhs) const {
    if (buckets_.size() != rhs.buckets_.size()) {
        return false;
    }
    for (auto it_l = buckets_.begin(), it_r = rhs.buckets_.begin();
         it_l != buckets_.end() && it_r != rhs.buckets_.end();
         ++it_l, ++it_r) {
        if (!std::is_permutation(it_l->begin(), it_l->end(), it_r->begin())) {
            return false;
        }
    }
    return true;
}


template<class Type, class Hasher>
bool u_map<Type, Hasher>::operator!=(const u_map &rhs) const {
    return !operator==(rhs);
}


template<class Type, class Hasher>
u_map<Type, Hasher> &u_map<Type, Hasher>::operator<<(const Type &key) {
    size_t idx = index(key);
    if (!inc(idx, key)) {
        buckets_[idx].emplace_front(std::make_pair(key, 1));
        ++size_;
    }
    return *this;
}


template<class Type, class Hasher>
u_map<Type, Hasher> &u_map<Type, Hasher>::operator<<(Type &&key) {
    size_t idx = index(key);
    if (!inc(idx, key)) {
        buckets_[idx].emplace_front(std::make_pair(std::move(key), 1));
        ++size_;
    }
    return *this;
}


template<class Type, class Hasher>
size_t u_map<Type, Hasher>::operator[](const Type &key) const {
    size_t idx = index(key);
    for (const auto &[k_, v_] : buckets_[idx]) {
        if (k_ == key) {
            return v_;
        }
    }
    throw std::invalid_argument("Key doesn't exists.");
}

template<class Type, class Hasher>
void u_map<Type, Hasher>::erase(const Type &key) {
    if (!has(key)) {
        return;
    }
    size_t idx = index(key);
    auto it = buckets_[idx].begin();
    if (it->first == key) {
        buckets_[idx].pop_front();
        --size_;
        return;
    } else {
        ++it;
        for (auto it_prev = buckets_[idx].begin(); it != buckets_[idx].end(); ++it, ++it_prev) {
            if (it->first == key) {
                buckets_[idx].erase_after(it_prev);
                --size_;
                return;
            }
        }
    }
}


template<class Type, class Hasher>
Type u_map<Type, Hasher>::min() const {
    return extr([](size_t lhs, size_t rhs) { return lhs < rhs; });
}


template<class Type, class Hasher>
Type u_map<Type, Hasher>::max() const {
    return extr([](size_t lhs, size_t rhs) { return lhs > rhs; });
}


template<class Type, class Hasher>
void u_map<Type, Hasher>::print(const std::string &filename) const {
    std::ofstream os;
    os.open(filename);
    for (const auto &bucket : buckets_) {
        for (const auto &[k_, v_] : bucket) {
            os << k_ << " == " << v_ << "\n";
        }
    }
    os.close();
}


template<class Type, class Hasher>
void u_map<Type, Hasher>::read(const std::string &filename) {
    clear();
    std::ifstream is;
    is.open(filename);
    if (!is.is_open()) {
        throw std::runtime_error("File doesn't exists.");
    }
    std::pair<Type, size_t> new_value;
    while (is >> new_value.first) {
        is.ignore(3);
        is >> new_value.second;
        for (unsigned i = 0; i < new_value.second; ++i) {
            operator<<(new_value.first);
        }
    }
    is.close();
}
