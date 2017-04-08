### About
egg-parser is a small app designed to showcase how to parse an \*.egg file. This app was originally an experiment for standardizing an \*.egg graph string. One of the limitations encountered by [egg-checker](https://github.com/casey-c/egg-checker) was the fact that a tree structure did not have a unique graph string representation; the same basic tree structure could have a set number of permutations, all of which could construct valid trees. This made validity checking tricky. With standardization, any identical tree will produce the same graph string, regardless of the order in which that particular tree was constructed.

### Docs / Usage / Understanding the code
I'll try and add some better documentation here about how this program works, and hopefully try and make it clearer how to implement the \*.egg format into your own projects.

### License
egg-parser is released under the MIT license. Feel free to use any code here in your own projects.
