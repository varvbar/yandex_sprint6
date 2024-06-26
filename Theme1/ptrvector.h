#pragma once

#include <vector>

template <typename T>
class PtrVector {
public:
    PtrVector() = default;

    // Создаёт вектор указателей на копии объектов из other
    PtrVector(const PtrVector& other) {
        // Резервируем место в vector-е для хранения нужного количества элементов
        // Благодаря этому при push_back не будет выбрасываться исключение
        items_.reserve(other.items_.size());

        try {
            for (auto p : other.items_) {
                // Копируем объект, если указатель на него ненулевой
                auto p_copy = p ? new T(*p) : nullptr;  // new может выбросить исключение

                // Не выбросит исключение, т. к. в vector память уже зарезервирована
                items_.push_back(p_copy);
            }
        } catch (...) {
            // удаляем элементы в векторе и перевыбрасываем пойманное исключение
            DeleteItems();
            throw;
        }
    }

    // Деструктор удаляет объекты в куче, на которые ссылаются указатели,
    // в векторе items_
    ~PtrVector() {
        DeleteItems();
    }

    // Возвращает ссылку на вектор указателей
    std::vector<T*>& GetItems() noexcept {
        return items_;
    }

    // Возвращает константную ссылку на вектор указателей
    std::vector<T*> const& GetItems() const noexcept {
        return items_;
    }


    PtrVector& operator=(const PtrVector& rhs) {
        // При присваивании PtrVector самому себе произойдёт удаление не только
        // своих элементов, но и элементов rhs.
        if (this != &rhs) {
            try {
                auto rhs_copy(rhs);
                swap(rhs_copy);
            }
            catch (...) {
                throw;
            }
        }
            return *this;
    }



private:
    void DeleteItems() noexcept {
        for (auto p : items_) {
            delete p;
        }
    }

    void swap(PtrVector& rhs) noexcept {
        items_.swap(rhs.items_);
    }

    std::vector<T*> items_;
};