# rangex
Factory to produce clear iterator with step and optional inclusive stop, which is used in for(auto i:range) / for(auto [index, value]: range) 

Swift has a clear notation for range in loop:
```Swift
for i in 1...5 {
    //print(i)
}
```
It's the same range with the following C
```C
for (int i=1; i<=5; i++) {
    /* printf */
}
```
And Rust:
```Rust
for i in 1..=5 "
    // print!("{i}")
}
```
And C++20:
```C++20
#include <ranges>
for(auto value : std::views::iota(1, 6)) { // Generate values from 1 to 5, lazy computing
    // std::cout << value << std::endl; // #include <iostream>
}
```
Indexed range in C++20:
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
Indexed range in Rust
```Rust
    let elements = vec![10, 20, 30, 40, 50, 60, 70, 80, 90, 100];

    for (index, element) in elements.iter().enumerate() {
        println!("Index: {}, Element: {}", index, element);
    }
```
Indexed range in Swift
```Swift

let elements = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]

for (index, element) in elements.enumerated() {
    print("Index: \(index), Element: \(element)")
}

```

With rangex lib in C++20
Inclusive iterated range:
```C++20 rangex
for (auto i : rangex(1, 5, true)) {
    // std::printf()
}
```
Downward step
```C++20 rangex
for (auto i : rangex(5, 1, true, -1)) {
    // std::printf()
}
```
Indexed range
```C++20 rangex
for (auto [i, v] : rangex<true>(5, 1, -1, true)) {
    // std::printf("%u %u", i, v)
}
```
Templated range
```C++20 rangex
for (auto i : rangex<int8_t, false>(1, 6)) {
    // 1,2,3,4,5
}
```