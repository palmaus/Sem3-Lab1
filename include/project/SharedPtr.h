#pragma once

#include <memory>
#include <type_traits>
#include <utility>

template <typename T, typename Deleter = std::default_delete<T>> class WeakPtr;

template <typename T, typename Deleter = std::default_delete<T>>
class SharedPtr {
public:
    struct ControlBlock {
        size_t count;
        T* ptr;
        Deleter deleter;

        ControlBlock(size_t cnt, T* p, Deleter d) : count(cnt), ptr(p), deleter(d) {}
    };

    // Конструктор по умолчанию
    SharedPtr() : ptr(nullptr), controlBlock(nullptr) {}

    // Конструктор от указателя
    template <typename U = T, typename = std::enable_if_t<!std::is_array_v<U>>>
    explicit SharedPtr(T* ptr = nullptr, Deleter d = Deleter()) : ptr(ptr), controlBlock(ptr ? new ControlBlock{1, ptr, d} : nullptr) {}

    // Конструктор копирования
    SharedPtr(const SharedPtr& other) : ptr(other.ptr), controlBlock(other.controlBlock) {
        increment();
    }

    // Конструктор перемещения
    SharedPtr(SharedPtr&& other) noexcept : ptr(other.ptr), controlBlock(other.controlBlock) {
        other.ptr = nullptr;
        other.controlBlock = nullptr;
    }

    // Оператор присваивания копированием
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            reset();
            ptr = other.ptr;
            controlBlock = other.controlBlock;
            increment();
        }
        return *this;
    }

    template<typename U, typename D>
    bool operator!=(const SharedPtr<U, D>& other) const {
        return !(*this == other);
    }

    // Оператор присваивания перемещением
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            reset();
            ptr = other.ptr;
            controlBlock = other.controlBlock;
            other.ptr = nullptr;
            other.controlBlock = nullptr;
        }
        return *this;
    }

    SharedPtr& operator=(std::nullptr_t) {
        reset();
        return *this;
    }

    // Деструктор
    ~SharedPtr() { reset(); }

    // Оператор разыменования
    T& operator*() const { return *ptr; }
    // Оператор стрелка
    T* operator->() const { return ptr; }

    // Получение указателя
    T* get() const { return ptr; }

    bool operator==(std::nullptr_t) const { return ptr == nullptr; }
    bool operator!=(std::nullptr_t) const { return ptr != nullptr; }

    template<typename U, typename D>
    bool operator==(const SharedPtr<U, D>& other) const {
        return ptr == other.ptr;
    }

    // Получение количества ссылок
    size_t use_count() const { return controlBlock ? controlBlock->count : 0; }

    // Проверка на пустоту
    explicit operator bool() const { return ptr != nullptr; }

    // Сброс указателя
    void reset() {
        if (controlBlock) {
            decrement();
        }
    }

    // Сброс с новым указателем
    template <typename U = T, typename = std::enable_if_t<!std::is_array_v<U>>>
    void reset(T* def_ptr, Deleter d = Deleter()) {
        reset();
        ptr = def_ptr;
        controlBlock = def_ptr ? new ControlBlock{1, ptr, d} : nullptr;
    }

private:
    T* ptr;
    ControlBlock* controlBlock;

    explicit SharedPtr(const WeakPtr<T, Deleter>& weak, bool) : ptr(weak.getPtr()), controlBlock(weak.controlBlock) {
        increment();
    }


    void increment() {
        if (controlBlock) {
            ++controlBlock->count;
        }
    }

    void decrement() {
        if (controlBlock && --controlBlock->count == 0) {
            controlBlock->deleter(ptr);
            controlBlock = nullptr;
            ptr = nullptr;
        }
    }

    friend class WeakPtr<T, Deleter>;

    template <typename U, typename D, typename... Args>
    friend SharedPtr<U, D> MakeShared(Args&&... args);

    template <typename U, typename D, typename... Args>
    friend SharedPtr<U, D> MakeSharedForArray(std::size_t size, Args&&... args);
};

template <typename T, typename Deleter = std::default_delete<T>, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...), Deleter{});
}

template <typename T, typename Deleter = std::default_delete<T>, typename... Args>
SharedPtr<T> MakeSharedForArray(std::size_t size, Args&&... args) {
    return SharedPtr<T>(new T[size](std::forward<Args>(args)...), Deleter{});
}