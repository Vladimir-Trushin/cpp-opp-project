#include <iostream>
#include <string>
#include <vector>
#include <new>
#include <stdexcept>

//// Abstract class
//
class Animal
{
public:
    std::string name;
    double temp;
    double breath;
    double heart;

    ////---------------------------------------------------------------------------------
    // Constractor
    Animal(std::string _name, double _temp, double _breath, double _heart)
        : name(_name), temp(_temp), breath(_breath), heart(_heart)
    {
    }

    ////---------------------------------------------------------------------------------
    // Distractor
    virtual ~Animal()
    {
    }

    ////---------------------------------------------------------------------------------
    // Will be implemented in a child class
    virtual bool check_health() = 0;
};

////
//
class Dog : public Animal
{
public:
    bool is_large;

    ////---------------------------------------------------------------------------------
    // Constractor
    Dog(std::string _name, double _temp, double _breath, double _heart, bool _is_large)
        : Animal(_name, _temp, _breath, _heart), is_large(_is_large)
    {
    }

    ////---------------------------------------------------------------------------------
    // Distractor
    ~Dog()
    {
        std::cout << "distractor class Dog: " << name << std::endl;
    }

    ////---------------------------------------------------------------------------------
    // A virtual function that checks if dog is healthy
    bool check_health()
    {
        bool is_health = true;

        if (is_large) // large dog
        {
            if (!((38 <= temp && temp <= 39.2) &&
                  (10 <= breath && breath <= 35) &&
                  (60 <= heart && heart <= 100)))
            {
                is_health = false;
            }
        }
        else // small dog
        {
            if (!((38 <= temp && temp <= 39.2) &&
                  (10 <= breath && breath <= 35) &&
                  (100 <= heart && heart <= 140)))
            {
                is_health = false;
            }
        }
        return is_health;
    }
};

////
//
class Cat : public Animal
{
public:
    ////---------------------------------------------------------------------------------
    // Constractor
    Cat(std::string _name, double _temp, double _breath, double _heart)
        : Animal(_name, _temp, _breath, _heart)
    {
    }

    ////---------------------------------------------------------------------------------
    // Distractor
    ~Cat()
    {
        std::cout << "distractor class Cat: " << name << std::endl;
    }

    ////---------------------------------------------------------------------------------
    // A virtual function that checks if cat is healthy
    bool check_health()
    {
        bool is_health = true;

        if (!((38 <= temp && temp <= 39.2) &&
              (16 <= breath && breath <= 40) &&
              (120 <= heart && heart <= 140)))
        {
            is_health = false;
        }

        return is_health;
    }
};

////
//
class Cow : public Animal
{
public:
    double milk;

    ////---------------------------------------------------------------------------------
    // Constractor
    Cow(std::string _name, double _temp, double _breath, double _heart, double _litres)
        : Animal(_name, _temp, _breath, _heart), milk(_litres)
    {
    }

    ////---------------------------------------------------------------------------------
    // Distractor
    ~Cow()
    {
        std::cout << "distractor class Cow: " << name << std::endl;
    }

    ////---------------------------------------------------------------------------------
    // A virtual function that checks if cow is healthy
    bool check_health()
    {
        bool is_health = true;

        if (!((38.5 <= temp && temp <= 39.5) &&
              (26 <= breath && breath <= 50) &&
              (48 <= heart && heart <= 84) &&
              (milk >= 30)))
        {
            is_health = false;
        }

        return is_health;
    }
};

//// Wrapping pointer on class Animal
//
class Animal_ptr
{
private:
    mutable Animal *p_data;

    ////---------------------------------------------------------------------------------
    // Private assignment operator does not implemented
    Animal_ptr &operator=(const Animal_ptr &obj)
    {
        throw std::runtime_error("can't use assignment operator (class Animal_ptr)");
    }

public:
    ////---------------------------------------------------------------------------------
    // Constractor
    Animal_ptr(Animal *ptr) : p_data(ptr)
    {
    }

    ////---------------------------------------------------------------------------------
    // Distractor
    ~Animal_ptr()
    {
        delete p_data;
    }

    ////---------------------------------------------------------------------------------
    // Copy constractor move pointer
    Animal_ptr(const Animal_ptr &obj)
    {
        p_data = obj.p_data;
        obj.p_data = nullptr; // p_data mutable
    }

    ////---------------------------------------------------------------------------------
    // Operator dereferencing
    Animal &operator*()
    {
        return *p_data;
    }

    ////---------------------------------------------------------------------------------
    // Member access operator
    Animal *operator->()
    {
        return p_data;
    }
};

//// Singleton class
//
class Vet
{
private:
    std::vector<Animal_ptr> animals; // vector for wrapping pointer on class Animal

    ////---------------------------------------------------------------------------------
    // Private constractor
    Vet()
    {
    }

    ////---------------------------------------------------------------------------------
    // Private copy constractor does not implemented
    Vet(const Vet &obj)
    {
        throw std::runtime_error("can't use constructor copy (class Vet)");
    }

    ////---------------------------------------------------------------------------------
    // Private assignment operator does not implemented
    Vet &operator=(const Vet &obj)
    {
        throw std::runtime_error("can't use assignment operator (class Vet)");
    }

public:
    ////---------------------------------------------------------------------------------
    // A function that creates a singleton instance of the class Vet
    static Vet &get_instance()
    {
        static Vet ret;

        return ret;
    }

    ////---------------------------------------------------------------------------------
    // A functiom that add a new wrapping pointer on class Animal
    void add_animal(const Animal_ptr &value)
    {
        animals.push_back(value);
    }

    ////---------------------------------------------------------------------------------
    // A function that prints a sick animals
    void show_sick()
    {
        for (size_t i = 0; i < animals.size(); i++)
            if (!animals[i]->check_health())
                std::cout << animals[i]->name << " ";

        std::cout << std::endl;
    }

    ////---------------------------------------------------------------------------------
    // A function that prints a sick dogs
    void show_sick_dogs()
    {
        for (size_t i = 0; i < animals.size(); i++)
            if (!animals[i]->check_health() && dynamic_cast<Dog *>(&(*animals[i]))) // check if animal sick and check if it is dog
                std::cout << animals[i]->name << " ";

        std::cout << std::endl;
    }
};

///////////////////////////////////////////////////////////////////

int main()
{
    using namespace std;

    //// test from project
    //
    cout << "program start" << endl;
    Vet &vet = Vet::get_instance();

    vet.add_animal(Animal_ptr(new Dog("dog1", 38.5, 20, 80, true)));
    vet.add_animal(Animal_ptr(new Dog("dog2", 40, 20, 80, true)));    // sick dog
    vet.add_animal(Animal_ptr(new Dog("dog3", 38.5, 20, 80, false))); // sick dog

    vet.add_animal(Animal_ptr(new Cat("cat1", 38.7, 30, 130)));
    vet.add_animal(Animal_ptr(new Cat("cat2", 38.7, 42, 130))); // sick cat

    vet.add_animal(Animal_ptr(new Cow("cow1", 39, 37, 70, 32)));
    vet.add_animal(Animal_ptr(new Cow("cow2", 39, 37, 70, 20))); // sick caw

    vet.show_sick();      // dog2 dog3 cat2 cow2
    vet.show_sick_dogs(); // dog2 dog3
    cout << endl;
    //
    ////

    //// my test
    //
    Vet &vet2 = Vet::get_instance();
    cout << &vet << endl;
    cout << &vet2 << endl;

    vet2.add_animal(Animal_ptr(new Dog("dog4", 20, 19.5, 100, true)));   // sick dog
    vet2.add_animal(Animal_ptr(new Dog("dog5", 35.5, 5.5, 100, false))); // sick dog

    vet2.add_animal(Animal_ptr(new Cat("cat3", 39, 30.5, 105))); // sick cat
    vet2.add_animal(Animal_ptr(new Cat("cat4", 39, 45, 130)));   // sick cat

    vet2.add_animal(Animal_ptr(new Cow("cow3", 39, 55, 49, 35))); // sick cow
    vet2.add_animal(Animal_ptr(new Cow("cow4", 40, 30, 84, 35))); // sick cow

    vet2.show_sick();      // dog2 dog3 cat2 cow2 dog4 dog5 cat3 cat4 cow3 cow4
    vet2.show_sick_dogs(); // dog2 dog3 dog4 dog5

    cout << endl;
    //
    ////

    return 0;
}