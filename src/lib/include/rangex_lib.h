#pragma once

#include "cstdtype_helper.h"
using namespace ns_type_helper;

#include <variant>

namespace ns_rangex {

/// 2 use cases:
/// for(auto v : rangex<uint8_t>(start, end, step, inclusive)) {
///   std::cout << v << std::endl;
/// }
/// =>
/// for(auto it = iterator->begin(); i!=iterator->end(); i+=step;) {
///   std::cout << *it << endl;  
/// }
///
/// for(auto [i, v] : rangex<uint8_t, true>(start, end, step, inclusive)) {
///   std::cout << "index: " << i << " , value:" << v << std::endl;
/// }
///
///```
/// 
///```

template <typename T = int, bool IncludeIndex = false>
class rangex {
public:
using signed_step_type_t = make_signed_custom_t<T>;
    struct iterator {
    public:
        using value_type = std::conditional_t<IncludeIndex, std::pair<std::size_t, T>, T>;
        // Initialize index only if IncludeIndex is true
        auto initialize_index() const {
            if constexpr (IncludeIndex) {
                return std::size_t{0};
            } else {
                return std::monostate{};
            }
        }
        // Iterator constructor
        iterator(T value_, signed_step_type_t step_)
            : value(value_)
            , step(step_)
            , _index(initialize_index()) // , _index(0)
        {
        }
        // Dereference operator to return the current value
        value_type operator*() const {
            if constexpr (IncludeIndex) {
                return { _index, value };
            }
            else {
                return value;
            }
        }
        // Prefix increment operator to move to the next value
        iterator& operator++() {
            value += step; // Increment value by step
            if constexpr (IncludeIndex) {
                _index++;
            }
            return *this;
        }
        // Comparison operator to check if two iterators are not equal
        bool operator!=(const iterator& other) const {
            //std::printf("loop condition: %d %d\n", value, other.value);
            return value != other.value; // Check if the current value is not equal to end value
        }

    protected:
        T value; // Current value
        signed_step_type_t step;  // Step size
        std::conditional_t<IncludeIndex, std::size_t, std::monostate> _index; //std::size_t _index;
    };

    rangex(T start_, T end_, bool inclusive = false, signed_step_type_t step_ = 1)
        : start(start_)
        //, _end(end)
        , step(step_) {
        if ((start_ <= end_ && step_ < 0)
            || (start_ >= end_ && step_ > 0)
           ) {
            this->_end = start_; // will do nothing in loop
        }
        else if (0 == step_) {
            // throw exception or allow possible infinity loop call next() if start != end?
            this->_end = end_;
        }
        else {
            // Calculate padding based on step direction
            T rangex_size = end_ - start;
            T num_steps;
            bool exactly_on_step = std_div_exact(rangex_size, step, num_steps);

            // Align `end` based on last multiple of `step` in rangex
            this->_end = start + (num_steps * step);
            if (!exactly_on_step) {
                this->_end += step;
            }
            // If inclusive, add one more `step` to include the endpoint
            if (inclusive && exactly_on_step) {
                this->_end += step;
            }
        }
    };
    // Begin method for rangex-based for loop
    iterator begin() const {
        return iterator(start, step);
    }
    // End method for rangex-based for loop
    iterator end() const {
        return iterator(_end, step);
    }

protected:
    // Start, end, and step size of the range
    T start, _end;
    signed_step_type_t step; 
};

} // namespace ns_rangex
