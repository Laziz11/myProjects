#include "Queue.h"
#include "Ingridient.h"
#include "Operation.h"
#include "Recipe.h"

#include <cassert>
#include <iostream>

void IngridientCtorTest() {
    Ingridient defCtor;
    assert(defCtor.title() == "." &&
           defCtor.__unit() == Unit::UNIT_INDEF &&
           defCtor.quantity() == 0);

    Ingridient expCtor = Ingridient("Salt", "GRAMM", 10u);
    assert(expCtor.title() == "Salt" &&
           expCtor.__unit() == Unit::GRAMM &&
           expCtor.quantity() == 10u);

    Ingridient copyCtor = expCtor;
    assert(copyCtor == expCtor);

    Ingridient moveCtor = std::move(copyCtor);
    assert(moveCtor == expCtor);

    std::cerr << "Ingridient Constructor Test OK" << std::endl;
}

void IngridientAccessTest() {
    Ingridient access = Ingridient("Sugar", "GRAMM", 50u);
    assert(access.title() == "Sugar");
    assert(access.__unit() == Unit::GRAMM);
    assert(access.quantity() == 50);

    access.setTitle("Water");
    assert(access.title() == "Water");

    access.setUnit("MILLILITER");
    assert(access.__unit() == Unit::MILLILITER);

    access.setQuantity(500u);
    assert(access.quantity() == 500u);

    std::cerr << "Ingridient Access Test OK" << std::endl;
}

void OperationCtorTest() {
    Operation defCtor;
    assert(defCtor.__action() == Action::ACTION_INDEF &&
           defCtor.time() == 0);

    Operation expCtor = Operation("CUT", 0u);
    assert(expCtor.__action() == Action::CUT &&
           expCtor.time() == 0);

    Operation copyCtor = expCtor;
    assert(copyCtor == expCtor);

    Operation moveCtor = std::move(copyCtor);
    assert(moveCtor == expCtor);

    std::cerr << "Operation Constructor Test OK" << std::endl;
}

void OperationAccessTest() {
    Operation access = Operation("FRY", 600u);
    assert(access.__action() == Action::FRY);
    assert(access.time() == 600u);

    access.setAction("CUT");
    assert(access.__action() == Action::CUT);

    access.setTime(0u);
    assert(access.time() == 0u);

    std::cerr << "Operation Access Test OK" << std::endl;
}

void QueueTest() {
    Queue<int> queue;

    assert(queue.empty());

    queue.push(1);

    assert(queue.front() == 1);

    queue.push(2);
    queue.emplace(2);
    queue.emplace(3);


    assert(queue.back() == 3);
    assert(queue.size() == 4);

    queue.pop();

    assert(queue.size() == 3);
    assert(queue.front() == 2);

    queue.emplace(1);

    assert(queue.back() == 1);
    assert(queue.size() == 4);

    Queue<int> copyTest = queue;

    assert(copyTest.size() == queue.size());
    assert(copyTest.front() == queue.front());
    assert(copyTest.back() == queue.back());
    assert(copyTest == queue);

    queue.emplace(1);

    Queue<int> moveTest = std::move(queue);

    assert(moveTest.front() == 2);
    assert(moveTest.back() == 1);
    assert(moveTest.size() == 5);

    moveTest.clear();
    queue.clear();

    assert(moveTest.empty());

    assert(queue == moveTest);

    queue.emplace(1);
    moveTest.push(1);

    assert(queue == moveTest);

    queue.push(2);

    assert(queue != moveTest);

    std::cerr << "Queue Test is OK" << std::endl;
}

void RecipeTest() {
    Recipe recipe;

    auto e1 = std::make_shared<Ingridient>(Ingridient());
    auto e2 = std::make_shared<Operation>(Operation("CUT", 1));
    auto e3 = std::make_shared<Ingridient>(Ingridient("sugar", "GRAMM", 10));

    recipe.push(e1);
    recipe.push(e2);
    recipe.push(e3);

    assert(recipe.isValid());

    Recipe copy = recipe;

    assert(recipe == copy);

    copy.clear();

    recipe.print("file1.txt");

    copy.read("file1.txt");

    assert(recipe == copy);

    copy.pop();

    copy.push(std::make_shared<Ingridient>());

    assert(recipe != copy);

   std::cerr << "Recipe Test is OK" << std::endl;
}

void Test() {
    IngridientCtorTest();
    IngridientAccessTest();
    OperationCtorTest();
    OperationAccessTest();
    QueueTest();
    RecipeTest();
    std::cerr << "All Tests OK" << std::endl;
}
