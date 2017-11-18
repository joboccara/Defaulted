#ifndef defaulted_h
#define defaulted_h

#include "optional.hpp"

namespace fluent
{

struct DefaultValue{};
static const DefaultValue defaultValue;

template<typename T>
using IsNotReference = typename std::enable_if<!std::is_reference<T>::value, void>::type;

    template<typename T, T... DefaultedParameter>
class Defaulted
{
public:
    Defaulted(T const& t) : value_(t){}
    template<typename T_ = T, typename = IsNotReference<T_>>
    Defaulted(T&& t) : value_(std::move(t)){}
    Defaulted(DefaultValue) : value_(DefaultedParameter...) {}
    T const& get_or_default() const { return value_; }
    T & get_or_default() { return value_; }
private:
    T value_;
};

template<typename T, typename GetDefaultValue>
class DefaultedF
{
public:
    DefaultedF(T const& t) : value_(t){}
    template<typename T_ = T, typename = IsNotReference<T_>>
    DefaultedF(T&& t) : value_(std::move(t)){}
    DefaultedF(DefaultValue) : value_(fluent::nullopt) {}
    
    template<typename... Args>
    T get_or_default(Args&&... args)
    {
        if (value_)
        {
            return *value_;
        }
        else
        {
            return GetDefaultValue::get(std::forward<Args>(args)...);
        }
    }
private:
    fluent::optional<T> value_;
};

}


#endif /* defaulted_h */
