#include <iostream>
#include <vector>


auto main() -> int
{
    std::vector<int> v;

    v.emplace_back(1);
    v.push_back(1);
    v.emplace_back(1);
    v.emplace_back(1);

    for (auto i : v) {
        std::cout << i << '\n';
    }


    return 0;
}
