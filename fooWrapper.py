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
