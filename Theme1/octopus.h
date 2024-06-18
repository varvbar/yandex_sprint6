#pragma once

// Тут можно подключить scopedptr.h и ptrvector.h, 
// если они вам понадобятся.
#include "scopedptr.h"
#include "ptrvector.h"

#include <new> // Для исключения bad_alloc
#include <vector>

// Щупальце
class Tentacle {
public:
    explicit Tentacle(int id) noexcept
        : id_(id) {
    }

    int GetId() const noexcept {
        return id_;
    }

    Tentacle* GetLinkedTentacle() const noexcept {
        return linked_tentacle_;
    }
    void LinkTo(Tentacle& tentacle) noexcept {
        linked_tentacle_ = &tentacle;
    }
    void Unlink() noexcept {
        linked_tentacle_ = nullptr;
    }

private:
    int id_ = 0;
    Tentacle* linked_tentacle_ = nullptr;
};

// Осьминог
class Octopus {
public:
    Octopus()
        : Octopus(8) {
    }

    explicit Octopus(int num_tentacles) {
        
        try {
            for (int i = 1; i <= num_tentacles; ++i) {
                ScopedPtr smart_t(new Tentacle(i));      // Может выбросить исключение bad_alloc
                tentacles_.GetItems().push_back(smart_t.GetRawPtr());  // Может выбросить исключение bad_alloc

                // Обнуляем указатель на щупальце, которое уже добавили в tentacles_,
                // чтобы не удалить его в обработчике catch повторно
                smart_t.Release();
            }
        } catch (const std::bad_alloc&) {
            // Удаляем щупальца, которые успели попасть в контейнер tentacles_
            //Cleanup();
            // Удаляем щупальце, которое создали, но не добавили в tentacles_
            //delete t;
            // Конструктор не смог создать осьминога с восемью щупальцами,
            // поэтому выбрасываем исключение, чтобы сообщить вызывающему коду об ошибке
            // throw без параметров внутри catch выполняет ПЕРЕВЫБРОС пойманного исключения
            throw;
        }
    }

    ~Octopus() {
        // Осьминог владеет объектами в динамической памяти (щупальца),
        // которые должны быть удалены при его разрушении.
        // Деструктор - лучшее место, чтобы прибраться за собой.
        //Cleanup();
    }

    // Добавляет новое щупальце с идентификатором,
    // равным (количество_щупалец + 1):
    // 1, 2, 3, ...
    // Возвращает ссылку на добавленное щупальце
    // Tentacle& AddTentacle() {
    //     Реализуйте добавление щупальца самостоятельно
    // }

    int GetTentacleCount() const noexcept {
        return static_cast<int>(tentacles_.GetItems().size());
    }

    const Tentacle& GetTentacle(size_t index) const {
        return *tentacles_.GetItems().at(index);
    }
    Tentacle& GetTentacle(size_t index) {
        return *tentacles_.GetItems().at(index);
    }

    Tentacle& AddTentacle() {
        ScopedPtr t = ScopedPtr(new Tentacle(GetTentacleCount() + 1));
        try {
            tentacles_.GetItems().push_back(t.GetRawPtr());
            t.Release();
        }
        catch(const std::bad_alloc&) {
            t.Release();
            throw;
        }
        return *tentacles_.GetItems().back();
    }


private:
  /*   void Cleanup() {
        // Удаляем щупальца осьминога из динамической памяти
        for (Tentacle* t : tentacles_) {
            delete t;
        }
        // Очищаем массив указателей на щупальца
        tentacles_.clear();
    } */

    // Вектор хранит указатели на щупальца. Сами объекты щупалец находятся в куче
    //std::vector<Tentacle*> tentacles_;
    PtrVector<Tentacle> tentacles_;
};