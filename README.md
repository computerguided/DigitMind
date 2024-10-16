# DigitMind in C++

## Introduction

This repository contains the C++ implementation and design of _DigitMind_.

DigitMind is a game based on the well known MasterMind, but instead of colors one has to guess a code of 4 different digits.

The difficulty level of DigitMind can be set by specifying the number of digits:

* [1,2,3,4]
* [1,2,3,4,5]
* [1,2,3,4,5,6]
* [1,2,3,4,5,6.7]
* [1,2,3,4,5,6,7,8]
* [1,2,3,4,5,6,7,8,9]
* [1,2,3,4,5,6,7,8,9,0]

A score is defined as two numbers:

* _Correct position_<br>The number of digits that are part of the code and are at the correct position

* _Wrong position_<br>The number of digits that are part of the code, but placed at the wrong position.

Using the scores, a player has to guess the code. This will be quite a challenge at the higher levels!

An algorithm can be implemented that enables the computer to become a codebreaker and to give the score when a human plays the role of codebreaker.

**Click [here](https://htmlpreview.github.io/?https://raw.githubusercontent.com/computerguided/DigitMind/refs/heads/main/digitmind.html) to play a version in your browser.**

## Code breaking algorithm
To enable a computer to guess a code based on the score, the first step is to create a list with all possible combinations, given the difficulty level, i.e. the size of the set of digits.

For example: when the difficulty level is 10, then there are 10 x 9 x 8 x 7 = 5040 possible combinations.

At the start, simply one combination is chosen randomly from this list. When the score is supplied for this guess, all those combinations are removed that would not give the same score as these can never be the combination to guess.

This process is repeated resulting eventually in the code being 'guessed'.

## Data structures
To implement the described algorithms, a number of data structures are required:

* [_Combination_](#combination): an array of 4 digits.
* [_Combination list_](#combination-list): a set of (possible) combinations given the difficulty level.
* [_Score_](#score): the number of digits on the correct position and the number on the wrong position.
* [_Game mode_](#game-mode): the game can be played by the computer guessing a code chosen by a human player or by a human player guessing a secret code chosen by the computer.

### Combination

A 'combination' comprises 4 digits. For convenience this can be specified as a new type `DigitCombination`.
```c++
typedef std::array<int, 4> DigitCombination;
```
### Combination list
To hold all the possible combinations, a vector `CombinationList` can be defined.

```c++
typedef std::vector<DigitCombination> CombinationList;
```

Note that a `vector` is used which allows to add an arbitrary number of elements, depending on the difficulty level.

### Score

A 'score' comprises the number of digits on the right position and the number of digits on the wrong position. As such, to hold the score a struct `Score` can be defined with ``right_position`` and ``wrong_position`` attributes.

```c++
struct Score
{
    int right_position;
    int wrong_position;
};
```
When this struct is instantiated, the attributes must be initialized, which is done in a constructor.

```c++
Score() : right_position(0), wrong_position(0) {}
```

To be able to determine whether one score is equal to another, an equality operator is defined.

```c++
bool operator==(const Score &other) const
{
    return right_position == other.right_position
           && wrong_position == other.wrong_position;
}
```

### Game mode
To hold the selected game mode, the `GameMode` enumeration is defined.
```c++
enum GameMode
{
    Quit,
    ComputerGuesses,
    PlayerGuesses
};
```

## Functions
To implement the described algorithms, a number of functions are required:

* [_Calculate the score_](#calculate-the-score)<br>Given a 'guess' and the 'code' the score must be determined. This function can be used both to determine the score of a guess from the user or be used to filter the list of possible combinations as described in "[Code breaking algorithm](#code-breaking-algorithm)".


* [_Get the difficulty level_](#get-difficulty-level)<br> Ask the user to specify the difficulty level, i.e. the number of digit to choose from.


* [_Generate all combinations_](#generate-all-combinations)<br>Determine all the possible combinations given a difficulty level.


* [_Filter the combinations_](#filter-the-combinations)<br>Remove all combinations from the list that are no longer possible given the new score.


* [_Select a random combination_](#select-a-random-combination)<br>Randomly choose a combination from the list of possible combinations.


* [_Perform the computer move_](#perform-the-computer-move)<br>Have the computer show a new guess and let the user enter the score.


* [_Computer player_](#computer-player)<br>Run the game in the mode where the computer is the player guessing the code.


* [_Human player_](#human-player)<br>Run the game in the mode where the human is the player guessing the code.

### Calculate the score

Given a 'guess' and the 'code' the score can be determined by calling the `calculateScore()` function. This function can be used both to determine the score of a guess from the user or be used to filter the list of possible combinations as described in "[Code breaking algorithm](#code-breaking-algorithm)".

```c++
Score calculateScore(DigitCombination guess, 
                     DigitCombination code)
```
The score that will be returned will be stored in a local variable.                     
```c++
Score score;
```

To determine the score, an iteration is performed over the 4 elements of the guess and code. When the digits are the same, the `right_position` attribute of the score is incremented. When the digits are not the same, it is checked whether the digit is in the wrong location in which case the `wrong_posiiton` attribute is incremented.

```c++
for (int i = 0; i < 4; i++)
{
    if (guess[i] == code[i])
    {
        score.right_position++;
    }
    else
    {
        for (int j = 0; j < 4; j++)
        {
            if (guess[i] == code[j])
            {
                score.wrong_position++;
                break;
            }
        }
    }
}
```

Note that this implementation assumes that a combination contains 4 distinct digits.

### Get difficulty level
To ask the user for the difficulty level, the `getDifficultyLevel()` function is called.

```c++
int getDifficultyLevel()
```

The user is asked to enter the difficulty level.

```c++
int level = 0;
std::cout << "Please enter the difficulty level (from 4 to 10): ";
std::cin >> level;
```

If this was not done correctly, the request is repeated.

```c++
while(std::cin.fail() || level < 4 || level > 10)
{
    std::cin.clear();    // reset the error flags
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');    // ignore rest of the line
    std::cout << "Invalid input. Please enter a number between 4 and 10: ";
    std::cin >> level;
}
return level;
```

### Generate all combinations

When the difficulty level is known, a list cen be created with all possible combinations by calling the `generateAllCombinations()` function.
```c++
CombinationList generateAllCombinations(int level)
```

The combinations can be generated by 4 nested loops.

```c++
CombinationList allCombinations;
DigitCombination combination;

for (int i = 0; i < level; i++)
{
    for (int j = 0; j < level; j++)
    {
        for (int k = 0; k < level; k++)
        {
            for (int l = 0; l < level; l++)
            {
                if (i != j && i != k && i != l && j != k && j != l && k != l)
                {
                    allCombinations.push_back(DigitCombination{i, j, k, l});
                }
            }
        }
    }
}
return allCombinations;
```

### Filter the combinations
To remove all combinations from the list that are no longer possible given the score for a 'guess', the `filterCombinations` function is defined. 

```c++
void filterCombinations(CombinationList& allCombinations, 
                        const DigitCombination& guess, 
                        const Score& score)
```
To be able to iterate over the list, an iterator `it` is defined that points to the beginning of the list.

```c++
auto it = allCombinations.begin();
```
A while-loop can now be created that stops when the iterator `it` points to the end of the list.

```c++
while (it != allCombinations.end())
{
    // ...
}
```
Inside the while-loop it must be checked (using the overloaded `==` operator as described in "[Score](#score)") whether the combination of to which the iterator points would yield the same score. If this is the case, the combination is allowed to stay in the list.
```c++
if (calculateScore(guess, *it) == score)
{
    ++it;
    continue;
}
```

Note that in this context, the iterator `it` is being used to traverse through a container (allCombinations). When the iterator is incremented in the loop, it doesn't need to rely on the old value of the iterator; it simply needs to move to the next element. Therefore, the pre-increment (`++it`) is used. This prevents creating a temporary copy (as done in post-increment). Although for simple iterators (like those for vectors or arrays), the difference in performance might be negligible, using pre-increment is a good general practice.

When the score does not match, the combination must be removed from the list. This can be done by calling the vector's [`erase`](https://en.cppreference.com/w/cpp/container/vector/erase]) method which returns an iterator pointing to the new location of the element that followed the last element erased by the function call. This is the container end if the operation erased the last element in the sequence.
```c++
it = allCombinations.erase(it);
```

### Select a random combination

To randomly choose a combination from the list of possible combinations, the `selectRandomCombination()` function can be called.

```c++
DigitCombination selectRandomCombination(const CombinationList& combinations)
```

First, a random number generator must be initialized by calling the [`std::random_device`](https://en.cppreference.com/w/cpp/numeric/random/random_device).

```c++
std::random_device rd;
```

A Mersenne Twister random number generator of the type [`std::mt19937`](https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine) is initialized with the seed from the random device. This generator produces high-quality random numbers and is commonly used in simulations and other applications requiring randomization.

```c++
std::mt19937 gen(rd());
```

Next a uniform integer distribution `dis` is created ranging from `0` to `combinations.size() - 1`. The type `CombinationList::size_type` ensures that the distribution produces integers of the same type as the size of the combinations list. This is important for indexing into the list without type mismatches.

```c++
std::uniform_int_distribution<CombinationList::size_type> dis(0, combinations.size() - 1);
```

Finally, the function returns a randomly selected element from the combinations list. The `dis(gen)` call generates a random integer within the specified range, and this integer is used as the index to access an element in `combinations`.

```c++
return combinations[dis(gen)];
```

### Perform the computer move

To have the computer perform a move, the `performComputerMove()` function is defined. The function takes the list of possible combinations as its argument and returns whether the computer guessed the combination.

```c++
bool performComputerMove(CombinationList& combinations)
```

The first step is to choose a random combination from the list by calling the [`selectRandomCombination()`](#select-a-random-combination) function.

```c++
DigitCombination guess = selectRandomCombination(combinations);
```

This guess is then shown to the user.

```c++
std::cout << "Computer's guess: ";
for (int digit : guess)
{
    std::cout << digit;
}
std::cout << "\n";
```

The user must first supply the number of digits in the correct position. This number is them placed in the `right_position` attribute of the `score`.

```c++
Score score;
std::cout << "Enter number of digits in the correct position: ";
std::cin >> score.right_position;
```

Now if the number of digits in the right position is 4 then obviously the code was guessed and the function can return without asking for the number of digits in the wrong position.

```c++
if ( score.right_position == 4)
{
    return true;
}
```

When the code was not guessed yet, the users must supply the number of digits in the wrong position.

```c++
std::cout << "Enter number of correct digits in the wrong position: ";
std::cin >> score.wrong_position;
```

The supplied can now be used to filter combinations by calling the [`filterCombinations()`](#filter-the-combinations) function. After this is done, the function can return with `false` to indicate that the code was not guessed yet.

```c++
filterCombinations(combinations, guess, score);
return false;
```

### Computer player
To run the game in the mode where the computer is the player guessing the code, the `computerPlayer()` function is called.

```c++
void computerPlayer(CombinationList& combinations)
```

A while-loop is created that stops when the computer guessed the code.

```c++
bool codeGuessed;
do
{
    // ...
} while (!codeGuessed);
```
Inside this while-loop the first step is to perform a move by calling the [`performComputerMove()`](#perform-the-computer-move) function. Since this function returns whether the computer guessed the code, the result can be used to set the `codeGuessed` loop variable.

```c++
codeGuessed = performComputerMove(combinations);
```

A special situation occurs when the code was not guessed yet, but there are no more possible combinations left. This will happen when the user made an error when supplying the score. This is handled by printing the error message and exiting the function which will restart the game.

```c++
if (combinations.empty() && !codeGuessed)
{
    std::cout << "Input error detected, restarting game...\n";
    return;
}
```

When the while-loop end, the computer guessed the combination.
```c++
std::cout << "The computer has guessed your combination!\n";
```

### Human player
To run the game in the mode where the human is the player guessing the code, the `humanPlayer()` function is called.

```c++
void humanPlayer(const int level, CombinationList& combinations)
```

To be able to select a secret random combination, the first step is to get the difficulty level from the user by calling the [`getDifficultyLevel()`](#get-difficulty-level) function and use that to generate all possible combinations by calling the [`generateAllCombinations()`](#generate-all-combinations) function.

```c++
auto level = getDifficultyLevel();
auto combinations = generateAllCombinations(level);
```

The computer then selects a secret combination by calling the [`selectRandomCombination()`](#select-a-random-combination) function.

```c++
auto secretCode = selectRandomCombination(combinations);
```

When the while-loop ends, the user guessed the code.

```c++
Score score;
do
{
    // ...
} while (score.right_position < 4);  // repeat until all positions are correct

// The secret code has been found
std::cout << "Congratulations, you have guessed the combination!\n";
```

Inside the while-loop the player is asked to enter a new guess.
```c++
DigitCombination playerGuess;
std::cout << "Enter your guess of four distinct digits between 0 and " << level - 1 << " without spaces: ";
std::string input;
std::cin >> input;
```
It is assumed that the player enters the guess as a string of 4 digits. This string is then converted to integers and stored in `playerGuess`.

```c++
for(int i = 0; i < 4; ++i)
{
    playerGuess[i] = input[i] - '0';  // Using ASCII code to convert characters to integers
}
```
The score can now be calculated based on the player's guess and the secret combination.

```c++
score = calculateScore(playerGuess, secretCode);
```
The resulting score can be provided as feedback to the player.

```c++
std::cout << "Digits in the right position: " << score.right_position << "\n";
std::cout << "Correct digits in wrong position: " << score.wrong_position << "\n";
```


