# DigitMind in C++

## Data structures

### Combination

A 'combination' comprises 4 digits. For convenience this can be specified as a new type `DigitCombination`.
```c++
typedef std::array<int, 4> DigitCombination;
```

To hold all the possible combinations, a vector `CombinationList` can be defined.

```c++
typedef std::vector<DigitCombination> CombinationList;
```

Note that a `vector` is used which allows to add an arbitrary number of elements, depending on the difficulty level.

### Score

A 'score' comprises the number of digits on the right position and the number of digits on the wrong position. As such, to hold the score a struct `Score` can be defined with ``right_position`` and ``wrong_position`` attributes.

```c++
struct Score {
    int right_position;
    int wrong_position;
};
```
When this struct is instantiated, the attributes must be initialized, which is done in a constructor.

```c++
Score() : right_position(0), wrong_position(0) {}
```

To be able to determine whether one score differs from another, an inequality operator is defined.

```c++
bool operator!=(const Score &other) const {
    return right_position != other.right_position
           || wrong_position != other.wrong_position;
}
```
