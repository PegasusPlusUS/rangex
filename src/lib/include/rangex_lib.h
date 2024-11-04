#ifndef __RANGEX_LIB_H__
#define __RANGEX_LIB_H__

// C++23, to support std::views::enumerate to implemente
// for(auto [index, v] : range) {}
#include <ranges>

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
template <typename T = int, bool IncludeIndex = false>
class rangex {
public:
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
        iterator(T value_, T step_)
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
            return value != other.value; // Check if the current value is not equal to end value
        }

    private:
        T value; // Current value
        T step;  // Step size
        std::conditional_t<IncludeIndex, std::size_t, std::monostate> _index; //std::size_t _index;
    };

    rangex(T start_, T end_, T step_ = 1, bool inclusive = false)
        : start(start_)
        //, _end(end)
        , step(step_) {
        // Calculate padding based on step direction
        T rangex_size = end_ - start;
        T num_steps = rangex_size / step;

        // Align `end` based on last multiple of `step` in rangex
        this->_end = start + (num_steps * step);
        
        // If inclusive, add one more `step` to include the endpoint
        if (inclusive) {
            this->_end += step;
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

private:
    T start, _end, step; // Start, end, and step size of the range
};

} // namespace ns_rangex

#endif // __RANGEX_LIB_H__
