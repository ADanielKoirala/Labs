#ifndef BARISTA_H
#define BARISTA_H

#include "drink.h"
#include "customer.h"
#include <vector>
#include <memory>
#include <random>
#include <iostream>

// ----- Chain of Responsibility for Barista levels -----

class AbstractBaristaHandler {
protected:
    AbstractBaristaHandler* next;
public:
    explicit AbstractBaristaHandler(AbstractBaristaHandler* n = nullptr)
        : next(n) {}

    virtual ~AbstractBaristaHandler() = default;

    AbstractBaristaHandler* findHandler(const Drink& drink) {
        if (canHandle(drink)) {
            return this;
        }
        if (next) {
            return next->findHandler(drink);
        }
        return nullptr;
    }

    virtual bool canHandle(const Drink& drink) const = 0;
    virtual std::string levelName() const = 0;
};

// Junior: only plain drinks
class JuniorBarista : public AbstractBaristaHandler {
public:
    using AbstractBaristaHandler::AbstractBaristaHandler;

    bool canHandle(const Drink& drink) const override {
        return !drink.hasIngredients();
    }

    std::string levelName() const override {
        return "JuniorBarista";
    }
};

// Senior: can handle drinks with ingredients, except those with milk foam
class SeniorBarista : public AbstractBaristaHandler {
public:
    using AbstractBaristaHandler::AbstractBaristaHandler;

    bool canHandle(const Drink& drink) const override {
        return drink.hasIngredients() && !drink.hasMilkFoam();
    }

    std::string levelName() const override {
        return "SeniorBarista";
    }
};

// Manager: can handle any drink
class Manager : public AbstractBaristaHandler {
public:
    using AbstractBaristaHandler::AbstractBaristaHandler;

    bool canHandle(const Drink&) const override {
        return true; 
    }

    std::string levelName() const override {
        return "Manager";
    }
};

// ----- Barista class managing orders and notifications -----

class Barista {
public:
    struct Order {
        std::shared_ptr<Drink> drink;
        Customer* customer;
        bool prepared = false;
    };

private:
    std::vector<ICustomerObserver*> observers;
    std::vector<Order> orders;

    JuniorBarista junior;
    SeniorBarista senior;
    Manager manager;
    AbstractBaristaHandler* head;

    std::mt19937 rng;

public:
    Barista()
        : junior(&senior), senior(&manager), manager(nullptr), head(&junior) {
        std::random_device rd;
        rng.seed(rd());
    }

    void registerCustomer(ICustomerObserver* c) {
        observers.push_back(c);
    }

    void takeOrder(Customer* customer, std::shared_ptr<Drink> drink) {
        orders.push_back(Order{drink, customer, false});
    }

   // Randomly decide to prepare a drink from pending orders
    void maybePrepareRandomOrder() {
        if (orders.empty()) return;

        std::uniform_int_distribution<int> coin(0, 1);
        int decision = coin(rng);

        if (decision == 0) {
            // Prepare a random pending order
            std::vector<int> pending;
            for (int i = 0; i < static_cast<int>(orders.size()); ++i) {
                if (!orders[i].prepared) pending.push_back(i);
            }
            if (pending.empty()) return;

            std::uniform_int_distribution<int> pick(0, pending.size() - 1);
            int idx = pending[pick(rng)];
            auto& ord = orders[idx];

            AbstractBaristaHandler* handler = head->findHandler(*ord.drink);
            std::string level = handler ? handler->levelName() : "Unknown";

            std::cout << level << " is preparing drink for "
                      << ord.customer->getName() << "...\n";

            ord.prepared = true;
            notifyAll(ord.drink, level);
        }
      
    }

    void finishAllOrders() {
        while (true) {
            bool anyPending = false;
            for (auto& o : orders) {
                if (!o.prepared) {
                    anyPending = true;
                    break;
                }
            }
            if (!anyPending) break;
            maybePrepareRandomOrder();
        }
    }

    void notifyAll(std::shared_ptr<Drink> drink, const std::string& baristaLevel) {
        for (auto* obs : observers) {
            obs->notifyDrinkReady(drink, baristaLevel);
        }
    }
};

#endif // BARISTA_H
