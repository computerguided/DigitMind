#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <random> // for std::random_device and std::mt19937

typedef std::array<int, 4> DigitCombination;
typedef std::vector<DigitCombination> CombinationList;

struct Score
{
    int right_position;
    int wrong_position;

    Score() : right_position(0), wrong_position(0) {}

    // Overloading the inequality operator
    bool operator==(const Score &other) const
    {
        return right_position == other.right_position
               && wrong_position == other.wrong_position;
    }
};

/**
 * @brief Calculate the score for a guess against a secret code.
 *
 * @param guess The guessed digit combination.
 * @param code The secret digit combination.
 * @return Score The resulting score of the guess.
 *
 * The score is calculated based on the number of digits in the correct
 * position (right position) and the number of digits that are in the code
 * but in the wrong position (wrong position).
 *
 * The function iterates through each digit of the guess and checks if it
 * is in the correct position or the wrong position.
 *
 * If a digit is in the correct position, the `right_position` score is
 * incremented. If a digit is in the code but not in the correct position,
 * the `wrong_position` score is incremented and the digit is marked as counted.
 *
 * @note The function assumes that both `guess` and `code` are valid digit
 * combinations of length 4.
 *
 * @see Score
 * @see DigitCombination
 */
Score calculateScore(DigitCombination guess, DigitCombination code)
{
    Score score;

    for (int i = 0; i < 4; i++)
    {
        if (guess[i] == code[i])
        {
            // Correct digit at right position
            score.right_position++;
        }
        else
        {
            // Check if guess digit is in code
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

    return score;
}

/**
 * @brief Gets the difficulty level from the user.
 *
 * This function prompts the user to enter a difficulty level within the range
 * of 4 to 10. If the user enters an invalid value, they are prompted to enter
 * a valid value until it is received.
 *
 * @return The difficulty level selected by the user.
 */
int getDifficultyLevel()
{
    int level = 0;
    std::cout << "Please enter the difficulty level (from 4 to 10): ";
    std::cin >> level;

    // Check if the input value is in the correct range.
    while(std::cin.fail() || level < 4 || level > 10)
    {
        std::cin.clear();    // reset the error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');    // ignore rest of the line
        std::cout << "Invalid input. Please enter a number between 4 and 10: ";
        std::cin >> level;
    }
    return level;
}

/**
 * @brief Generates all possible combinations of digits from 0 to level-1.
 *
 * This function generates all possible combinations of digits from 0 to level-1
 * without repetition. Each combination is represented by a DigitCombination,
 * which is an array of 4 integers.
 *
 * @param level The maximum digit value (level-1) for generating combinations.
 * @return CombinationList A vector of DigitCombinations representing all the
 * combinations.
 */
CombinationList generateAllCombinations(int level)
{
    CombinationList allCombinations;
    DigitCombination combination;

    // Generate combinations from '0123' to 'level-1 level-1 level-1 level-1'
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
}

/**
 * @brief Filter combinations based on guess and score.
 *
 * This function filters a list of combinations based on a guess and score.
 * It removes combinations that don't produce the same score as the
 * guess and score provided.
 *
 * @param allCombinations The list of combinations to filter.
 * @param guess The guess combination.
 * @param score The score to compare against.
 */
void filterCombinations(CombinationList& allCombinations,
                        const DigitCombination& guess,
                        const Score& score)
{
    // Iterate over list of combinations and remove those that
    // don't produce the same score
    auto it = allCombinations.begin();
    while (it != allCombinations.end())
    {
        if (calculateScore(guess, *it) == score)
        {
            ++it;
            continue;
        }

        it = allCombinations.erase(it);  // erase returns the new iterator
    }
}

/**
 * @brief Selects a random combination from a list of combinations.
 *
 * @param combinations The list of combinations to select from.
 * @return The randomly selected combination.
 */
DigitCombination selectRandomCombination(const CombinationList& combinations)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<CombinationList::size_type> dis(0, combinations.size() - 1);
    return combinations[dis(gen)];
}

/**
 * @brief Performs the computer's move in the game.
 *
 * This function selects a random combination from the provided list of combinations,
 * displays it to the user, and then gets the user's feedback in terms of the number of digits
 * in the correct position and the number of correct digits in the wrong position.
 * The score is then used to filter the list of combinations, removing those combinations that do not
 * produce the same score as the guessed combination.
 *
 * @param combinations The list of combinations to choose from.
 * @return Whether the code was guessed
 */
bool performComputerMove(CombinationList& combinations)
{
    DigitCombination guess = selectRandomCombination(combinations);

    // Show guess to user
    std::cout << "Computer's guess: ";
    for (int digit : guess)
    {
        std::cout << digit;
    }
    std::cout << "\n";

    // Get user's feedback
    Score score;
    std::cout << "Enter number of digits in the correct position: ";
    std::cin >> score.right_position;

    if ( score.right_position == 4)
    {
        return true;
    }

    std::cout << "Enter number of correct digits in the wrong position: ";
    std::cin >> score.wrong_position;

    // Use score to filter combinations
    filterCombinations(combinations, guess, score);

    return false;
}

/**
 * @brief Plays the game with the computer as the guesser.
 *
 * This function allows the computer to play the game by guessing the user's combination and
 * receiving feedback on the correctness of the guess. The computer will continue to guess until
 * it correctly guesses the combination or the user input leads to an empty list of combinations.
 */
void computerPlayer(CombinationList& combinations)
{
    bool codeGuessed;
    do
    {
        // Perform a computer move and get the score
        codeGuessed = performComputerMove(combinations);

        // Check if combinations list is empty due to incorrect user input
        if (combinations.empty() && !codeGuessed)
        {
            std::cout << "Input error detected, restarting game...\n";
            return;
        }
    } while (!codeGuessed);  // repeat until the code is guessed

    std::cout << "The computer has guessed your combination!\n";
}

/**
 * @brief Computer selects a random combination and the human player guesses that combination.
 *
 * This function allows the human player to play the game by guessing the combination
 * that the computer generates. The player then receives feedback
 * on the correctness of the guess. The player will continue to guess until
 * the combination is guessed correctly.
 */
void humanPlayer(const int level, CombinationList& combinations)
{
    // Computer selects a secret combination
    auto secretCode = selectRandomCombination(combinations);

    Score score;
    do
    {
        // Prompt the player to enter a guess
        DigitCombination playerGuess;
        std::cout << "Enter your guess (4 distinct digits between 0 and " << level - 1 << "): ";
        std::string input;
        std::cin >> input;

        // Convert string characters to integers and store them in playerGuess
        for(int i = 0; i < 4; ++i)
        {
            playerGuess[i] = input[i] - '0';  // Using ASCII code to convert characters to integers
        }

        // Calculate the score based on the player's guess and the secret combination
        score = calculateScore(playerGuess, secretCode);

        // Provide feedback to the player
        std::cout << "Digits in the right position: " << score.right_position << "\n";
        std::cout << "Correct digits in wrong position: " << score.wrong_position << "\n";
    } while (score.right_position < 4);  // repeat until all positions are correct

    // The secret code has been found
    std::cout << "Congratulations, you have guessed the combination!\n";
}

/**
 * @brief Display a menu and prompt the user to choose a game mode.
 *
 * This function displays a menu with three options:
 * 0. Quit the game
 * 1. Computer guesses the user's combination
 * 2. User guesses the computer's combination
 *
 * The function prompts the user to enter the number of their chosen option.
 * If the user enters an invalid option (not 0, 1, or 2), the function will
 * continue to display the menu and prompt for a valid choice.
 *
 * @return The user's chosen option.
 */
int menu()
{
    int choice;

    do
    {
        std::cout << "\nChoose game mode:\n"
                  << "0. Quit\n"
                  << "1. Computer guesses your combination\n"
                  << "2. You guess the combination the computer has selected\n"
                  << "\n"
                  << "Enter the number of your chosen option: ";
        std::cin >> choice;
    } while (choice>2);

    return choice;
}

/**
 * @brief Main function to start the DigitMind game.
 *
 * This function displays a welcome message and allows the user to choose between two game modes:
 * 1. Computer guesses the user's combination
 * 2. User guesses the combination the computer has selected
 * After the user makes a choice, the corresponding game mode function is called until the user chooses to quit.
 */
int main()
{
    std::cout << "-- Welcome to DigitMind --\n";

    while (true)
    {
        int choice = menu();

        if (choice == 0)
        {
            exit(0);
        }

        // Get difficulty level and generate all possible combinations
        auto level = getDifficultyLevel();
        auto combinations = generateAllCombinations(level);

        if ( choice == 1)
        {
            computerPlayer(combinations);
        }
        else if (choice == 2)
        {
            humanPlayer(level, combinations);
        }
    }
}