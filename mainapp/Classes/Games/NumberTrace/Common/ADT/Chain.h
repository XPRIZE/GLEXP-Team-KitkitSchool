//
//  Chain.h on May 09, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

// NB(xenosoz, 2016): Value sharing mechanism and hooks.

#ifndef TODOSCHOOL_ADT_CHAIN_H
#define TODOSCHOOL_ADT_CHAIN_H

#include <array>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <utility>

namespace todoschool {

template <typename T>
class Chain {
    typedef char* id_type;

    // NB(xenosoz, 2016): Invariant: id != nullptr && *id is valid.
    id_type ID;

    std::unordered_map<id_type, Chain*> Following;
    std::unordered_map<id_type, Chain*> Followers;

    // NB(xenosoz, 2016): Invariant: *re.._cast<T*>(&data) is always valid.
    alignas(T) std::array<char, sizeof(T)> Data;

    void clear() {
        clearRelations();
        clearInside();
    }

    void clearInside() {
        // NB(xenosoz, 2016): Do not expose clearInside to public. It'll break the invariant.
        pointer()->~T();
        delete ID;
    }

public:
    typedef T value_type;
    std::function<void(T&)> OnValueUpdate;

    template <typename... Args>
    Chain(Args&&... A)
        : ID(new std::pointer_traits<id_type>::element_type())
    {
        new(&Data) T(std::forward<Args>(A)...);
    }
    ~Chain() {
        clear();
    }

    Chain(const Chain& C)
        : ID(new std::pointer_traits<id_type>::element_type())
    {
        new(&Data) T(C.value());

        for (auto F: C.Following) {
            Chain* Target = F.second;
            Target->Followers[ID] = this;
        }
        Following = C.Following;

        for (auto F: C.Followers) {
            Chain* Target = F.second;
            Target->Following[ID] = this;
        }
        Followers = C.Followers;
    }

    Chain(Chain&& C)
        : ID(new std::pointer_traits<id_type>::element_type())
    {
        new(&Data) T(std::forward<T>(C.value()));

        for (auto F: C.Following) {
            Chain* Target = F.second;
            Target->Followers[ID] = this;
        }
        Following.swap(C.Following);

        for (auto F: C.Followers) {
            Chain* Target = F.second;
            Target->Following[ID] = this;
        }
        Followers.swap(C.Followers);
    }

    Chain& operator=(const Chain& C) {
        clear();

        ID = new std::pointer_traits<id_type>::element_type();
        new(&Data) T(C.value());

        for (auto F: C.Following) {
            Chain* Target = F.second;
            Target->Followers[ID] = this;
        }
        Following = C.Following;

        for (auto F: C.Followers) {
            Chain* Target = F.second;
            Target->Following[ID] = this;
        }
        Followers = C.Followers;
    }

    Chain& operator=(Chain&& C) {
        clear();

        ID = new std::pointer_traits<id_type>::element_type();
        new(&Data) T(std::forward<T>(C.value()));

        for (auto F: C.Following) {
            Chain* Target = F.second;
            Target->Followers[ID] = this;
        }
        Following.swap(C.Following);

        for (auto F: C.Followers) {
            Chain* Target = F.second;
            Target->Following[ID] = this;
        }
        Followers.swap(C.Followers);
    }

    void follow(Chain& Target, bool Update=true) {
        Following[Target.ID] = &Target;
        Target.Followers[ID] = this;

        if (Update)
            update(Target.value());
    }

    void unfollow(Chain& Target) {
        // NB(xenosoz, 2016): Watch out for the iterator invalidation!
        Following.erase(Target.ID);
        Target.Followers.erase(ID);
    }

    void clearRelations() {
        clearFollowing();
        clearFollowers();
    }

    void clearFollowing() {
        for (auto F: Following) {
            Chain& Target = *F.second;
            Target.Followers.erase(ID);
        }
        Following.clear();
    }

    void clearFollowers() {
        for (auto F: Followers) {
            Chain& Target = *F.second;
            Target.Following.erase(ID);
        }
        Followers.clear();
    }

    //
    // NB(xenosoz, 2016): Data as a (plain) value.
    //
    T& value() { return *pointer(); }
    const T& value() const { return *pointer(); }
    T& operator*() { return value(); }
    const T& operator*() const { return value(); }
    T& operator()() { return value(); }
    const T& operator()() const { return value(); }

    //
    // NB(xenosoz, 2016): Data as a pointer value.
    //
    T* pointer() { return reinterpret_cast<T*>(&Data); }
    const T* pointer() const { return reinterpret_cast<const T*>(&Data); }
    T* operator->() { return pointer(); }
    const T* operator->() const { return pointer(); }

    //
    // NB(xenosoz, 2016): Value propagation (+ optional update).
    //
    void update() {
        std::unordered_set<Chain*> Visited;
        updateUnvisited(Visited);
    }

    void update(const T& Value) {
        std::unordered_set<Chain*> Visited;
        updateUnvisited(Value, Visited);
    }

    void updateUnvisited(std::unordered_set<Chain*>& Visited) {
        if (Visited.find(this) != Visited.end()) { return; }
        Visited.insert(this);

        applyUpdateChain(Visited);

        if (OnValueUpdate)
            OnValueUpdate(value());
    }

    void updateUnvisited(const T& Value, std::unordered_set<Chain*>& Visited) {
        if (Visited.find(this) != Visited.end()) { return; }
        Visited.insert(this);

        value() = Value;
        applyUpdateChain(Value, Visited);

        if (OnValueUpdate)
            OnValueUpdate(value());
    }

    void applyUpdateChain(std::unordered_set<Chain*>& Visited) {
        // NB(xenosoz, 2016): Pay the cost of copying followers to avoid iterator invalidations.
        auto FollowersCopy = Followers;

        for (auto F: FollowersCopy) {
            Chain* Target = F.second;
            Target->updateUnvisited(Visited);
        }
    }

    void applyUpdateChain(const T& Value, std::unordered_set<Chain*>& Visited) {
        // NB(xenosoz, 2016): Pay the cost of copying followers to avoid iterator invalidations.
        auto FollowersCopy = Followers;

        for (auto F: FollowersCopy) {
            Chain* Target = F.second;
            Target->updateUnvisited(Value, Visited);
        }
    }
};

} // namesapce todoschool

#endif // !defined(TODOSCHOOL_ADT_CHAIN_H)
