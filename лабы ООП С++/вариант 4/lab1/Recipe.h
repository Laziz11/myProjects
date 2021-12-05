#pragma once

#include "Queue.h"
#include "Item.h"
#include "Operation.h"
#include "Ingridient.h"
#include "mainwindow.h"
#include <fstream>

class Recipe : public Queue<std::shared_ptr<Item>> {
    friend class MainWindow;
    friend void print(const Recipe &recipe);
public:
    [[nodiscard]] bool isValid();

    bool operator==(const Recipe &rhs);

    bool operator!=(const Recipe &rhs);

    void print(const std::string &filename) const;

    void read(const std::string &filename);
};

void print(const Recipe &recipe);
