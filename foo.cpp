#include <iostream>
#include <gsl/gsl_sf_bessel.h>

class Foo
{
  public:
    // non-GSL stuff
    void bar()
    {
        std::cout << "Hello new" << std::endl;
    }

    // GSL function
    int bessel(void)
    {
        double x = 15.0;
        std::cout << std::fixed; // We want fixed precision.
        double y = gsl_sf_bessel_J0(x);
        std::cout << y;
        return 0;
    }
};


// This is the wrapper part, which actually exposes the C++ functions to python.
extern "C"
{
    Foo *Foo_new() { return new Foo(); }

    void Foo_bar(Foo *foo) { foo->bar(); }

    void Foo_bessel(Foo *foo) { foo->bessel(); }
}
