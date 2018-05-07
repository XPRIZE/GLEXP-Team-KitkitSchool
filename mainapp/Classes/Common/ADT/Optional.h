//
//  Optional.h on May 09, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#ifndef TODOSCHOOL_ADT_OPTIONAL_H
#define TODOSCHOOL_ADT_OPTIONAL_H

#include <array>
#include <cassert>


//
// NB(xenosoz, 2016): Represent owned, nullable object placeholder.
//   XXX: There's a known bug when use with Chain<T>!
//
template<typename T>
class Optional {
    bool valued;
    alignas(T) std::array<char, sizeof(T)> data;

public:
    typedef T value_type;

    Optional() : valued(false) {}
    ~Optional() { reset(); }

    Optional(const Optional& O) : valued(O.valued) {
        if (valued)
            new(&data) T(O.value());
    }
    Optional(const T& V) : valued(true) {
        new(&data) T(V);
    }

    Optional(Optional&& O) : valued(O.valued) {
        if (valued)
            new(&data) T(std::move(O.value()));
    }
    Optional(T&& V) : valued(true) {
        new(&data) T(std::forward<T>(V));  // XXX
    }

    void reset() {
        if (!valued)
            return;
        pointer()->~T();
        valued = false;
    }
    void reset(const T& V) {
        reset();
        new(&data) T(V);
        valued = true;
    }
    void reset(T&& V) {
        reset();
        new(&data) T(std::forward<T>(V));  // XXX
        valued = true;
    }

    Optional& operator=(const Optional& O) {
        reset();
        valued = O.valued;
        if (valued)
            new(&data) T(O.value());
        return *this;
    }
    Optional& operator=(const T& V) {
        reset(V);
        return *this;
    }

    Optional& operator=(Optional&& O) {
        reset();
        valued = O.valued;
        if (valued)
            new(&data) T(std::move(O.value()));
        return *this;
    }
    Optional& operator=(T&& V) {
        reset(std::forward<T>(V));  // XXX
        return *this;
    }

    explicit operator bool() const {
        return valued;
    }

    bool empty() const {
        return !valued;
    }

    bool operator==(const Optional& RHS) const {
        const Optional& LHS = *this;
        if (LHS.valued != RHS.valued)
            return false;
        return LHS.value() == RHS.value();
    }

    bool operator!=(const Optional& RHS) const {
        const Optional& LHS = *this;
        return !(LHS == RHS);
    }

    T& value() {
        assert(valued);
        return *reinterpret_cast<T*>(&data);
    }
    const T& value() const {
        assert(valued);
        return *reinterpret_cast<const T*>(&data);
    }
    const T& value(const T& Default) const {
        if (!valued)
            return Default;
        return value();
    }

    T* pointer() {
        if (!valued)
            return nullptr;
        return reinterpret_cast<T*>(&data);
    }
    const T* pointer() const {
        if (!valued)
            return nullptr;
        return reinterpret_cast<const T*>(&data);
    }

    T& operator*() {
        return value();
    }
    const T& operator*() const {
        return value();
    }

    T* operator->() {
        return pointer();
    }
    const T* operator->() const {
        return pointer();
    }
};



#endif // !defined(TODOSCHOOL_ADT_OPTIONAL_H)
