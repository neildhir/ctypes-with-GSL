# Using C/C++ wrappers with GSL

So I spent about a day trying to figure out how to use any kind of C/C++ wrapper (`ctypes`, `cppyy`, `swig` etc) working
with the Gnu Scientific library. It was _not_ easy to find help on that point. So here is a MWE that hopefully others
find useful. 

First things first:

1. Make sure you have GSL installed; I [found this very helpful](https://askubuntu.com/questions/890027/installing-gsl-libraries-in-ubuntu-16-04-via-terminal). I followed the suggestions given in the first answer.
2. `ctypes` comes with standard python distro so you don't have to do anything there. 

### Preparing som C++ GSL functions

Here we're going to use create a C++ file which contains some mixed public methods.

```c
#include <iostream>
#include <gsl/gsl_sf_bessel.h>

class Foo
{
  public:
    void bar()
    {
        std::cout << "Hello world!" << std::endl;
    }

    // GSL function
    int bessel(void)
    {
        double x = 15.0;
        std::cout << std::fixed;
        double y = gsl_sf_bessel_J0(x);
        std::cout << y;
        return 0;
    }
};

extern "C"
{
    Foo *Foo_new() { return new Foo(); }

    void Foo_bar(Foo *foo) { foo->bar(); }

    void Foo_bessel(Foo *foo) { foo->bessel(); }
}

```


You can find the above stuff in `foo.cpp`. Now where did I get all of this from? I got it from
[here](https://stackoverflow.com/questions/145270/calling-c-c-from-python?answertab=votes#tab-top). Again, look at the
most popular answer concerning `ctypes`. I have extended this example with a `Bessel` function call from the `gsl`
library.

### The Python wrapper

Here is my wrapper which allows me to "see" these functions in Python:

```python

from ctypes import cdll
lib = cdll.LoadLibrary('./libfoo.so')


class Foo(object):
    def __init__(self):
        self.obj = lib.Foo_new()

    def bar(self):
        lib.Foo_bar(self.obj)

    def bessel(self):
        lib.Foo_bessel(self.obj)


if __name__ == '__main__':
    f = Foo()
    f.bar()
    f.bessel()

```

You can find this stuff in `fooWrapper.py`.

### Compiling

This is the part that I found the most frustrating, since I could not actually find an example which showed how to do
this step:

```bash
#!/usr/bin/env bash

g++ -c -fPIC foo.cpp -o foo.o
g++ -shared -Wl,-soname,libfoo.so -o libfoo.so  foo.o -lgsl -lgslcblas -lm
```

## Running

Provided that all went well, you can now simply do:

```
python fooWrapper.py
```
and you should see:

```
Hello new
-0.014224
```
