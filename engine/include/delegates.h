#pragma once

template <typename... Args>
class Delegate
{
public:
    using Stub = void(*)(void*, Args...);

    Delegate() = default;

    // Bind a non-const member function
    template <typename T, void (T::* Method)(Args...)>
    static Delegate Bind(T* instance)
    {
        Delegate d;
        d.object_ = instance;
        d.stub_ = [](void* obj, Args... args)
            {
                (static_cast<T*>(obj)->*Method)(args...);
            };
        return d;
    }

    // Bind a const member function
    template <typename T, void (T::* Method)(Args...) const>
    static Delegate Bind(const T* instance)
    {
        Delegate d;
        d.object_ = const_cast<T*>(instance); // safe: we only call const Method
        d.stub_ = [](void* obj, Args... args)
            {
                (static_cast<const T*>(obj)->*Method)(args...);
            };
        return d;
    }

    // Bind a free/static function
    template <void (*Fn)(Args...)>
    static Delegate Bind()
    {
        Delegate d;
        d.object_ = nullptr;
        d.stub_ = [](void*, Args... args)
            {
                Fn(args...);
            };
        return d;
    }

    void operator()(Args... args) const
    {
        if (stub_) stub_(object_, args...);
    }

    explicit operator bool() const { return stub_ != nullptr; }

    bool equals(const Delegate& other) const
    {
        return object_ == other.object_ && stub_ == other.stub_;
    }

private:
    void* object_ = nullptr;
    Stub stub_ = nullptr;
};
