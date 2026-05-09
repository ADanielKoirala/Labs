#include <iostream>
#include <cctype>
#include <vector>
#include <memory>
#include "drink.h"
#include "customer.h"
#include "barista.h"

std::shared_ptr<Drink> buildDrinkInteractive() {
    char sizeChoice;
    std::cout << "Welcome to Coffee Shack, can I get you [l]arge, [m]edium, or [s]mall coffee? ";
    std::cin >> sizeChoice;
    sizeChoice = static_cast<char>(std::tolower(sizeChoice));

    std::shared_ptr<Drink> drink;
    switch (sizeChoice) {
        case 'l': drink = std::make_shared<LargeCoffee>(); break;
        case 'm': drink = std::make_shared<MediumCoffee>(); break;
        case 's': drink = std::make_shared<SmallCoffee>(); break;
        default:
            std::cout << "Unknown size, defaulting to small.\n";
            drink = std::make_shared<SmallCoffee>();
            break;
    }

    char ingChoice;
    while (true) {
        std::cout << "Would you like to add [s]prinkles, [c]aramel, milk [f]oam, [i]ce or [n]ot? ";
        std::cin >> ingChoice;
        ingChoice = static_cast<char>(std::tolower(ingChoice));

        if (ingChoice == 'n') break;

        switch (ingChoice) {
            case 's':
                drink = std::make_shared<Sprinkles>(drink);
                break;
            case 'c':
                drink = std::make_shared<Caramel>(drink);
                break;
            case 'f':
                drink = std::make_shared<MilkFoam>(drink);
                break;
            case 'i':
                drink = std::make_shared<Ice>(drink);
                break;
            default:
                std::cout << "Unknown ingredient, ignoring.\n";
                break;
        }
    }

    return drink;
}

int main() {
    Barista barista;
    std::vector<std::unique_ptr<Customer>> customers;

    char more = 'y';
    while (std::tolower(more) == 'y') {
        auto drink = buildDrinkInteractive();

        std::cout << "Can I get your name? ";
        std::string name;
        std::cin >> name;

        auto customer = std::make_unique<Customer>(name);
        customer->setDrink(drink);

        // Register customer and take order
        barista.registerCustomer(customer.get());
        barista.takeOrder(customer.get(), drink);

        customers.push_back(std::move(customer));

        // After taking an order, randomly either prepare a drink or just continue
        barista.maybePrepareRandomOrder();

        std::cout << "Is there another customer? (y/n) ";
        std::cin >> more;
    }

    // When no more new orders, finish making all remaining drinks
    barista.finishAllOrders();

    std::cout << "All drinks are ready. Have a nice day!\n";
    return 0;
}
