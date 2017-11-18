# Defaulted

The purpose of `Defaulted` is to work around two constraints in C++ related to default parameters:
- The first one is that all the default parameters have to be at the end of the arguments list of a function. This can make an interface less natural, because arguments are no longer grouped in a logical order. Instead, they are grouped in a technical order: the non-default parameters first, then the default ones, which can be confusing at call site.
- The second constraint is their interdependence: if there are several default parameters , and a call site wants to pass a value for only one of them, it also has to provide a value for all the other default parameters preceding it in the parameters list of the function. This again makes for awkward call sites.


_The components shown in this document are located int the `fluent` namespace._

## Basic usage

### Placing default parameters between other parameters

Consider the following prototype:

```cpp
void f(int x, int y, int z);
```

To add a default value to a parameter in the middle of a function prototype we normally have to put it at the end:

```cpp
void f(int x, int z, int y = 42);
```

`Defaulted` allows to place it anywhere:

```cpp
void f(int x, Defaulted<int, 42> y, int y)
```

This can be called this way:

```cpp
f(0, defaultValue, 1); // equivalent to f(0, 42, 1);
```

Or with a specific value:

```cpp
f(0, 35, 1);
```

And the implementation of `f` can retrieve the value inside the `Defaulted` by calling its `.get()` method. 

### Specifying the value of only one default parameter

In the case of several default parameters, we normally have to specify the values for the default parameters preceding it:

```cpp
void f(int x, int y = 42, int z = 43);

f(0, 42, 1); // we have to write the default value 42 at call site to be able to pass 1 to z
```

With Defaulted we don't have to:

```cpp
void f(int x, Defaulted<int, 42> y, Defaulted<int, 43> z);

f(0, defaultValue, 1);
```

## Values that don't fit into a template

`int`s like 42 fit into a template such as `Defaulted<int, 42>`, but most other types don't.
For other types, a variation of `Defaulted` called `DefaultedF` accept the default value wrapped into a function, itself wrapped into a type:

```cpp
struct GetDefaultAmount{ static double get(){ return 45.6; } };

void g(int x, DefaultedF<double, GetDefaultAmount> y, int z)
```

##Default parameters that depend on other parameters

Since C++ does not allow default values to depend on other parameters:
```cpp
void f(double x, double y, double z = x + y) // imaginary C++
{
std::cout << "x = " << x << '\n'
<< "y = " << y << '\n'
<< "z = " << z << '\n';
}
```

`DefaultedF` allows default functions to accepts parameters, to which a function can pass the other parameters:

```cpp
struct GetDefaultAmount{ static double get(double x, double y){ return x + y; } };

void f(double x, double y, DefaultedF<double, GetDefaultAmount> z)
{
std::cout << "x = " << x << '\n'
<< "y = " << y << '\n'
<< "z = " << z.get_or_default(x, y) << '\n';
}
```

## The particular case of the default default value

A pretty common case for default parameters is when they take the value resulting from a call to the default constructor of their type: `T()`.

To make this easier to express in an interface, we adopt the convention that if no value is passed in the Defaulted template, then it falls back to calling the default constructor of its underlying type, for a default value:

```cpp
void f(int x, Defaulted<std::string> y, int z); // equivalent to pseudo-C++ void f(int x, std::string y = std::string(), int z);
```

## Passing default parameters by const reference

To emulate a default parameter on a const reference, we use a `Defaulted` on a const reference:

```cpp
void h(int x, Defaulted<std::string const&> y, int z)
```
which does not make a copy of the parameter passed, when there is one.
