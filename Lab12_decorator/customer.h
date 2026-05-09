#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <memory>
#include <iostream>
#include <iomanip>

class Drink;

// Observer interface for Customer
class ICustomerObserver {
public:
    virtual ~ICustomerObserver() = default;
    virtual void notifyDrinkReady(std::shared_ptr<Drink> drink,
                                  const std::string& baristaLevel) = 0;
};

// Customer class implementing the observer interface
class Customer : public ICustomerObserver {
    std::string name;
    std::shared_ptr<Drink> myDrink;

public:
    explicit Customer(std::string n) : name(std::move(n)) {}

    const std::string& getName() const { return name; }

    void setDrink(std::shared_ptr<Drink> d) {
        myDrink = std::move(d);
    }

    void notifyDrinkReady(std::shared_ptr<Drink> drink,
                          const std::string& baristaLevel) override;
};

#endif // CUSTOMER_H
