#ifndef DRINK_H
#define DRINK_H

#include <string>
#include <memory>

class Drink {
public:
    virtual ~Drink() = default;
    virtual std::string getDescription() const = 0;
    virtual double cost() const = 0;

    // For checking ingredients
    virtual bool hasIngredients() const { return false; }
    virtual bool hasMilkFoam() const { return false; }
};

// ----- Concrete base drinks -----

class SmallCoffee : public Drink {
public:
    std::string getDescription() const override {
        return "small coffee";
    }
    double cost() const override {
        return 1.00;
    }
};

class MediumCoffee : public Drink {
public:
    std::string getDescription() const override {
        return "medium coffee";
    }
    double cost() const override {
        return 2.00;
    }
};

class LargeCoffee : public Drink {
public:
    std::string getDescription() const override {
        return "large coffee";
    }
    double cost() const override {
        return 3.00;
    }
};

// ----- Decorator base class -----

class IngredientDecorator : public Drink {
protected:
    std::shared_ptr<Drink> drink;
public:
    explicit IngredientDecorator(std::shared_ptr<Drink> d) : drink(std::move(d)) {}

    bool hasIngredients() const override {
       // If we are a decorator, we have ingredients
        return true;
    }

    bool hasMilkFoam() const override {
        return drink->hasMilkFoam();
    }

    // Helper to append ingredient description
    std::string appendIngredient(const std::string& ing) const {
        std::string base = drink->getDescription();
        if (base.find(" with ") == std::string::npos) {
            return base + " with " + ing;
        } else {
            return base + ", " + ing;
        }
    }
};

//  ---- Concrete ingredient decorators -----

class Sprinkles : public IngredientDecorator {
public:
    using IngredientDecorator::IngredientDecorator;

    std::string getDescription() const override {
        return appendIngredient("sprinkles");
    }

    double cost() const override {
        return drink->cost() + 0.50;
    }
};

class Caramel : public IngredientDecorator {
public:
    using IngredientDecorator::IngredientDecorator;

    std::string getDescription() const override {
        return appendIngredient("caramel");
    }

    double cost() const override {
        return drink->cost() + 0.20;
    }
};

class MilkFoam : public IngredientDecorator {
public:
    using IngredientDecorator::IngredientDecorator;

    std::string getDescription() const override {
        return appendIngredient("foam");
    }

    double cost() const override {
        return drink->cost() + 0.40;
    }

    bool hasMilkFoam() const override {
        return true;
    }
};

class Ice : public IngredientDecorator {
public:
    using IngredientDecorator::IngredientDecorator;

    std::string getDescription() const override {
        return appendIngredient("ice");
    }

    double cost() const override {
        return drink->cost() + 0.10;
    }
};

#endif 
