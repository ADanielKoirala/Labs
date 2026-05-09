#include "customer.h"
#include "drink.h"
#include <iostream>
#include <iomanip>
#include <cmath>

void Customer::notifyDrinkReady(std::shared_ptr<Drink> drink,
                                const std::string& baristaLevel) {
    if (!myDrink) return;

    if (drink == myDrink) {
        // Notify customer their drink is ready
        std::cout << std::fixed << std::setprecision(2);
        std::cout << name << ", your " << drink->getDescription()
                  << " is ready. It will be $" << drink->cost()
                  << ", please. (Prepared by " << baristaLevel << ")\n";
    }
}
