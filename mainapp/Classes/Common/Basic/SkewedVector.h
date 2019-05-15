//
//  SkewedVector.h -- N-base vector (w/ rather tedious forwarding things)
//  TodoSchool on Oct 21, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

// NB(xenosoz, 2016): Sometimes we need to call the first index 'N' to make people happy.
//   If we have to do it, it should be less stressful.

// NB(xenosoz, 2016): It is *not* designed to be compatible std::vector. Some operations
//   may work differently (in the future).


#pragma once

#include "TodoSchoolNS.h"
#include <vector>
#include <stdexcept>


BEGIN_NS_TODOSCHOOL

template <typename T, int BaseN>
class SkewedVector {
    std::vector<T> data_;

public:
    typedef typename std::vector<T>::value_type value_type;
    typedef typename std::vector<T>::size_type size_type;
    typedef typename std::vector<T>::difference_type difference_type;
    typedef typename std::vector<T>::reference reference;
    typedef typename std::vector<T>::const_reference const_reference;
    typedef typename std::vector<T>::pointer pointer;
    typedef typename std::vector<T>::const_pointer const_pointer;

    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;
    typedef typename std::vector<T>::reverse_iterator reverse_iterator;
    typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;

public:
#if false
    // XXX: More constructor/assign operator required.
    SkewedVector();

    SkewedVector& operator=(const SkewedVector& other);
    SkewedVector& operator=(SkewedVector&& other);
    SkewedVector& operator=(std::initializer_list<T> ilist);
#endif


    reference at(size_type pos) { return data_.at(etoi(pos)); }
    const_reference at(size_type pos) const { return data_.at(etoi(pos)); }

    reference operator[](size_type pos) { return data_[etoi(pos)]; }
    const_reference operator[](size_type pos) const { return data_[etoi(pos)]; }
    
    reference front() { return data_.front(); }
    const_reference front() const { return data_.front(); }
    
    reference back() { return data_.back(); }
    const_reference back() const { return data_.back(); }
    
    T* data() { return data_.data(); }
    const T* data() const { return data_.data(); }


    iterator begin() { return data_.begin(); }
    const_iterator begin() const { return data_.begin(); }
    const_iterator cbegin() const { return data_.cbegin(); }
    
    iterator end() { return data_.end(); }
    const_iterator end() const { return data_.end(); }
    const_iterator cend() const { return data_.cend(); }
    
    reverse_iterator rbegin() { return data_.rbegin(); }
    const_reverse_iterator rbegin() const { return data_.rbegin(); }
    const_reverse_iterator crbegin() const { return data_.crbegin(); }
    
    reverse_iterator rend() { return data_.rend(); }
    const_reverse_iterator rend() const { return data_.rend(); }
    const_reverse_iterator crend() const { return data_.crend(); }


    bool empty() const { return data_.empty(); }
    size_type size() const { return data_.size(); }
    size_type max_size() const { return data_.max_size(); }
    void reserve(size_type new_cap) { return data_.reserve(new_cap); }
    size_type capacity() const { return data_.capacity(); }
    void shrink_to_fit() { return data_.shrink_to_fit(); }


    void clear() { return data_.clear(); }
    iterator insert(iterator pos, const T& value) {
        return data_.insert(pos, value);
    }
    iterator insert(const_iterator pos, const T& value) {
        return data_.insert(pos, value);
    }
    iterator insert(const_iterator pos, T&& value) {
        return data_.insert(pos, std::forward<T>(value));
    }
    void insert(iterator pos, size_type count, const T& value) {
        return data_.insert(pos, count, value);
    }
    iterator insert(const_iterator pos, size_type count, const T& value) {
        return data_.insert(pos, count, value);
    }
    template <typename InputIt>
    void insert(iterator pos, InputIt first, InputIt last) {
        return data_.insert(pos, first, last);
    }
    template< class InputIt >
    iterator insert(const_iterator pos, InputIt first, InputIt last) {
        return data_.insert(pos, first, last);
    }
    iterator insert( const_iterator pos, std::initializer_list<T> ilist) {
        return data_.insert(pos, ilist);
    }
    template<typename... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        return data_.insert(pos, std::forward<Args>(args)...);
    }
    iterator erase(iterator pos) { return data_.erase(pos); }
    iterator erase(const_iterator pos) { return data_.erase(pos); }
    iterator erase(iterator first, iterator last) {
        return data_.erase(first, last);
    }
    iterator erase(const_iterator first, const_iterator last) {
        return data_.erase(first, last);
    }
    void push_back(const T& value) { return data_.push_back(value); }
    void push_back(T&& value) { return data_.push_back(std::forward<T>(value)); }
    template< class... Args >
    void emplace_back( Args&&... args ) { return data_.emplace_back(std::forward<Args>(args)...); }
    void pop_back() { return data_.pop_back(); }
    void resize(size_type count) { return data_.resize(count); }
    void resize(size_type count, const value_type& value) { return data_.resize(count, value); }
    void swap(SkewedVector& other) { return data_.swap(other.data_); }
    
public:
    // NB(xenosoz, 2016): External index to internal index.
    constexpr size_type etoi(size_type pos) const { return pos - BaseN; }

    // NB(xenosoz, 2016): Internal index to external index.
    constexpr size_type itoe(size_type pos) const { return pos + BaseN; }


    constexpr size_type begin_index() const { return itoe(0); }
    size_type end_index() const { return itoe(size()); }
    
    // NB(xenosoz, 2016): Resize it to have the given index.
    void resize_for_index(size_type pos)
    {
        // NB(xenosoz, 2016): Just return if we already meet this criteria.
        if (begin_index() <= pos && pos < end_index())
            return;

        // NB(xenosoz, 2016): There's no way to meet this criteria.
        if (pos < begin_index())
            throw std::range_error("SkewedVector could't make the valid range");
        
        resize(etoi(pos) + 1);
    }
};


END_NS_TODOSCHOOL