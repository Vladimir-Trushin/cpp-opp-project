#include <iostream>
#include <stdexcept>
#include <new>
#include <cmath>

////
//
class Equation
{
    friend std::ostream &operator<<(std::ostream &out, const Equation &obj);
    friend Equation operator+(double _c, const Equation &obj);

private:
    double a;
    double b;
    double c;
    double *solutions;
    size_t solutions_size;

    ////----------------------------------------------------------------
    // A function that updates solution and solution_size
    void update_solutions()
    {
        double discriminant = std::pow(b, 2) - (4 * a * c);
        double square = std::sqrt((discriminant >= 0) ? discriminant : 0);

        if (discriminant > 0) // two solutions
        {
            solutions_size = 2;
            solutions[0] = (-b + square) / (2 * a);
            solutions[1] = (-b - square) / (2 * a);
        }
        else if (discriminant == 0) // one solution
        {
            solutions_size = 1;
            solutions[0] = (-b) / (2 * a);
            solutions[1] = solutions[0];
        }
        else // zero solutions
        {
            solutions_size = 0;
            solutions[0] = 0;
            solutions[1] = 0;
        }
    }

public:
    ////----------------------------------------------------------------
    // Constractor
    Equation(double _a, double _b, double _c) : a(_a), b(_b), c(_c), solutions(nullptr), solutions_size(0)
    {
        if (a == 0) // A: can't be zero
            throw std::invalid_argument("A: can't be zero");

        try
        {
            solutions = new double[2];
        }
        catch (std::bad_alloc &e)
        {
            std::cout << "error: bad allocetion: " << e.what() << std::endl;
            exit(1);
        }

        update_solutions();
    }

    ////----------------------------------------------------------------
    // Distractor
    ~Equation()
    {
        delete[] solutions;
    }

    ////----------------------------------------------------------------
    // Copy constractor
    Equation(const Equation &obj) : a(obj.a), b(obj.b), c(obj.c), solutions(nullptr), solutions_size(obj.solutions_size)
    {
        try
        {
            solutions = new double[2];
        }
        catch (std::bad_alloc &e)
        {
            std::cout << "error: bad allocetion: " << e.what() << std::endl;
            exit(1);
        }

        solutions[0] = obj.solutions[0];
        solutions[1] = obj.solutions[1];
    }

    ////----------------------------------------------------------------
    // Assignment operator
    Equation &operator=(const Equation &obj)
    {
        if (this == &obj)
            return *this;

        a = obj.a; // can't be an object with A: zero
        b = obj.b;
        c = obj.c;
        solutions_size = obj.solutions_size;

        delete[] solutions;

        try
        {
            solutions = new double[2];
        }
        catch (std::bad_alloc &e)
        {
            std::cout << "error: bad allocetion: " << e.what() << std::endl;
            exit(1);
        }

        solutions[0] = obj.solutions[0];
        solutions[1] = obj.solutions[1];

        return *this;
    }

    ////----------------------------------------------------------------
    // Operator + (Equation + Equation)
    Equation operator+(const Equation &obj) const
    {
        return Equation(a + obj.a, b + obj.b, c + obj.c);
    }

    ////----------------------------------------------------------------
    // Operator + (Equation + double)
    Equation operator+(double _c) const
    {
        return Equation(a, b, c + _c);
    }

    ////----------------------------------------------------------------
    // Get A
    double get_a()
    {
        return a;
    }

    ////----------------------------------------------------------------
    // Set A
    void set_a(double _a)
    {
        if (_a == 0) // A: can't be zero
            throw std::invalid_argument("A: can't be zero");

        a = _a;
        update_solutions();
    }

    ////----------------------------------------------------------------
    // Get B
    double get_b()
    {
        return b;
    }

    ////----------------------------------------------------------------
    // Set B
    void set_b(double _b)
    {
        b = _b;
        update_solutions();
    }

    ////----------------------------------------------------------------
    // Get C
    double get_c()
    {
        return c;
    }

    ////----------------------------------------------------------------
    // Set C
    void set_c(double _c)
    {
        c = _c;
        update_solutions();
    }

    ////----------------------------------------------------------------
    // A function that returns solutions size
    size_t get_solutions_size()
    {
        return solutions_size;
    }

    ////----------------------------------------------------------------
    // A function that returns solutions
    double const *const get_solutions()
    {
        return solutions;
    }
};

////----------------------------------------------------------------
// A friend function that prints an object Equation
std::ostream &operator<<(std::ostream &out, const Equation &obj)
{
    printf("%.1lfX^2", obj.a);

    if (obj.b >= 0) // print sign of a number B
        printf("+");
    else
        printf("-");
    printf("%.1lfX", ((obj.b >= 0) ? obj.b : -obj.b));

    if (obj.c >= 0) // print sign of a number C
        printf("+");
    else
        printf("-");
    printf("%.1lf", ((obj.c >= 0) ? obj.c : -obj.c));

    return out;
}

////----------------------------------------------------------------
// Friend operator + (double + Equation)
Equation operator+(double _c, const Equation &obj)
{
    return Equation(obj.a, obj.b, obj.c + _c);
}

////////////////////////////////////////////////////////////////////

int main()
{
    using namespace std;

    //// test from project
    //
    cout << "test from project:" << endl;

    Equation eq(10, 20, 30);
    cout << eq << endl;
    cout << eq.get_solutions_size() << endl;
    cout << -20 + eq << endl;
    eq = -20 + eq;
    cout << eq.get_solutions_size() << endl;
    cout << eq.get_solutions()[0] << endl;
    cout << Equation(1, 5, 3) + Equation(2, 6, 4) << endl;
    eq = Equation(1, 3, -4);
    cout << eq << endl;
    cout << eq.get_solutions_size() << endl;
    cout << eq.get_solutions()[0] << endl;
    cout << eq.get_solutions()[1] << endl;
    cout << endl;
    //
    ////

    //// my test
    //
    cout << "my test:" << endl;

    Equation x1(-1, -2, -3);
    Equation x2(x1);

    cout << x2 << endl;
    cout << x2 + 12.55 << endl;
    cout << 12.55 + x2 << endl;
    x1 = x1 + x2;
    cout << x1 << endl;
    cout << x1.get_solutions_size() << endl;
    x2 = Equation(2, -4, 2);
    cout << x2 << endl;
    cout << x2.get_solutions_size() << endl;
    cout << x2.get_solutions()[0] << endl;

    try
    {
        x2.set_a(0);
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << e.what() << '\n';
    }

    try
    {
        x1 = Equation(0, 10, -20);
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << e.what() << '\n';
    }
    //
    ////

    return 0;
}