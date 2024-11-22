#pragma once

template <typename T, typename Deleter>
class SharedPtr;

template <typename T, typename Deleter>
class WeakPtr {
public:
    WeakPtr() : controlBlock(nullptr) {}

    explicit WeakPtr(const SharedPtr<T, Deleter>& shared) : controlBlock(shared.controlBlock) {}

    WeakPtr(const WeakPtr& other) : controlBlock(other.controlBlock) {}

    // Конструктор перемещения
    WeakPtr(WeakPtr&& other) noexcept : controlBlock(other.controlBlock) {
        other.controlBlock = nullptr;
    }

    // Оператор присваивания копированием
    WeakPtr& operator=(const WeakPtr& other) {
        if (this != &other) {
            controlBlock = other.controlBlock;
        }
        return *this;
    }

    // Оператор присваивания перемещением
    WeakPtr& operator=(WeakPtr&& other) noexcept {
        if (this != &other) {
            controlBlock = other.controlBlock;
            other.controlBlock = nullptr;
        }
        return *this;
    }

    bool expired() const { return controlBlock == nullptr || controlBlock->count == 0; }

    explicit operator bool() const { return !expired(); }

    WeakPtr& operator=(std::nullptr_t) {
        controlBlock = nullptr;
        return *this;
    }

    std::size_t use_count() const {
        return controlBlock ? controlBlock->count : 0;
    }

    SharedPtr<T, Deleter> lock() const {
        if (!this->expired()) {
            return SharedPtr<T, Deleter>(*this, true);
        }
        return SharedPtr<T, Deleter>();
    }

    void swap(WeakPtr& other) noexcept {
        std::swap(controlBlock, other.controlBlock);
    }

    T* getPtr() const {
        if (controlBlock) {
            return controlBlock->ptr;
        } else {
            return nullptr;
        }
    }

private:
    friend class SharedPtr<T, Deleter>;
    typename SharedPtr<T, Deleter>::ControlBlock* controlBlock;
};