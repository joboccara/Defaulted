#ifndef defaulted_h
#define defaulted_h

namespace fluent
{
    struct DefaultValue{};
    static const DefaultValue defaultValue;
    
    template<typename T, T... DefaultedParameters>
    class Defaulted
    {
    public:
        Defaulted(T t) : value_(std::move(t)){}
        Defaulted(DefaultValue) : value_(DefaultedParameters...) {}
        T const& get() const { return value_; }
        T & get() { return value_; }
    private:
        T value_;
    };
    
    template<typename T, typename GetValue>
    class DefaultedF
    {
    public:
        DefaultedF(T t) : value_(std::move(t)){}
        DefaultedF(DefaultValue) : value_(GetValue::get()) {}
        T const& get() const { return value_; }
        T & get() { return value_; }
    private:
        T value_;
    };

}


#endif /* defaulted_h */
