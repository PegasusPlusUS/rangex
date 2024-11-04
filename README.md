# rangex
Factory to produce clear iterator with step and optional inclusive stop, which is used in for(auto i:range) / for(auto [index, value]: range) 

Swift has very clear notations for range in loop:
```Swift
//inclusive 5
for i in 1...5 {
    //print(i)
}
//downward inclusive 1, use 'to: 1' for exclusive
for i in stride(from: 5, through: 1, by: -1) {
    //print(i)
}
```
In C
```C
/* inclusive 5 */
for (int i=1; i<=5; i++) {
    /* printf("%d ", i); */
}
/* downward inclusive 1 */
for (int i=5; i>=1; i--) {
    /* printf("%d ", i); */
}
```
And Rust:
```Rust
// inclusive 5
for i in 1..=5 {
    // print!("{i} ")
}
// downward inclusive 5 (and 1)
for i in (1..=5).rev() {
    // print!("{} ", i);
}
```
And C++20:
```C++20
#include <ranges>
// exclusive 6
for(auto value : std::views::iota(1, 6)) { // Generate values from 1 to 5, lazy computing
    // std::cout << value << std::endl; // #include <iostream>
}
// downward exclusive 6
for (int i : std::views::iota(1, 6) | std::views::reverse) {
    // std::cout << i << std::endl;
}
```
With rangex lib in C++20
```C++20 rangex
#include "rangex_lib.h"
using namespace ns_rangex;

//Inclusive iterated range:
for (auto i : rangex(1, 5, true)) {
    // std::printf()
}
//Inclusive downward step -1
for (auto i : rangex(5, 1, true, -1)) {
    // std::printf()
}
//Templated typename
for (auto i : rangex<int>(1, 6)) {
    // 1,2,3,4,5
}
```
An indexed range will provide index for iteration start from 0
Indexed range in Swift
```Swift

let elements = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]
for (index, element) in elements.enumerated() {
    //print("Index: \(index), Element: \(element)")
}
```
Indexed range in Rust
```Rust
    let elements = vec![10, 20, 30, 40, 50, 60, 70, 80, 90, 100];
    for (index, element) in elements.iter().enumerate() {
        //println!("Index: {}, Element: {}", index, element);
    }
```
Indexed range in std C++20:
```C++20
    // Create a vector of elements
    std::vector<int> elements = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    // Create an indexed range using iota and a custom zip view
    auto indexed_range = std::views::iota(0, static_cast<int>(elements.size()))
                       | std::views::transform([&](int index) {
                             return std::pair{index, elements[index]};
                         });

    // Iterate over the indexed range and print the index and corresponding element
    for (const auto& [index, element] : indexed_range) {
        std::cout << "Index: " << index << ", Element: " << element << std::endl;
    }
```
Indexed range in rangex C++20
```C++20 rangex
#include "rangex_lib.h"
using namespace ns_rangex;

for (auto [i, v] : rangex<int, true>(5, 1, true, -1)) {
    // std::printf("%u %u", i, v)
}
```
In Swift, by in (from, to, by) must be signed  say Int, Float, Double
from, to, through must be of same type, by must be of the signed type with them.
If from <= to/through and step < 0, or from >= to/through and step > 0, for loop will do nothing.
If step is 0, Swift will panic
