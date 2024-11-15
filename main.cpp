#include <iostream>
#include <fstream>
#include <unordered_set>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

// Function to load the dictionary and create a set of valid words
std::unordered_set<std::string> loadDictionary(const std::string& filename) {
    std::unordered_set<std::string> dictionary;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot find dictionary file" << std::endl;
        exit(1);
    }

    std::string word;
    while (file >> word) {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower); // Convert to lowercase
        dictionary.insert(word);
    }

    file.close();
    return dictionary;
}

// function generates new word letter that differs by one letter each time
std::vector<std::string> getAdjacentWords(const std::string& word, const std::unordered_set<std::string>& dictionary) {
    std::vector<std::string> adjacentWords;
    for (size_t i = 0; i < word.length(); ++i) {
        std::string tempWord = word;
        for (char c = 'a'; c <= 'z'; ++c) {
            if (c == word[i]) continue;
            tempWord[i] = c;

            if (dictionary.find(tempWord) != dictionary.end()) {
                adjacentWords.push_back(tempWord);
            }
        }
    }
    return adjacentWords;
}

// Function that uses breadth-first search to finding shortest path between nodes
std::vector<std::string> findWordLadder(const std::string& start, const std::string& end, const std::unordered_set<std::string>& dictionary) {
    if (start.length() != end.length()) {
        return {}; // No ladder path can be traced
    }

    std::queue<std::vector<std::string>> bfsQueue;
    std::unordered_set<std::string> visited;
    bfsQueue.push({start});
    visited.insert(start);

    while (!bfsQueue.empty()) {
        std::vector<std::string> currentPath = bfsQueue.front();
        bfsQueue.pop();
        std::string currentWord = currentPath.back();

        if (currentWord == end) {
            return currentPath; // shortest path is found
        }

        for (const std::string& adjacentWord : getAdjacentWords(currentWord, dictionary)) {
            if (visited.find(adjacentWord) == visited.end()) {
                visited.insert(adjacentWord);
                std::vector<std::string> newPath = currentPath;
                newPath.push_back(adjacentWord);
                bfsQueue.push(newPath);
            }
        }
    }

    return {}; // no ladder found case
}


int main() {
    const std::string dictionaryFile = "dictionary.txt";
    std::unordered_set<std::string> dictionary = loadDictionary(dictionaryFile);

    std::cout << "Word-Ladder Problem Solver\n";
    std::cout << "Enter 'quit' to exit.\n";

    while (true) {
        std::string startWord, endWord;
        std::cout << "\nEnter the first word: ";
        std::cin >> startWord;
        if (startWord == "quit") break;

        std::cout << "Enter the second word: ";
        std::cin >> endWord;
        if (endWord == "quit") break;

        // upper to lowercase
        std::transform(startWord.begin(), startWord.end(), startWord.begin(), ::tolower);
        std::transform(endWord.begin(), endWord.end(), endWord.begin(), ::tolower);

        // timelenght tool
        clock_t startTime = clock();

        std::vector<std::string> wordLadder = findWordLadder(startWord, endWord, dictionary);

        if (!wordLadder.empty()) {
            std::cout << "Shortest word ladder: ";
            for (const std::string& word : wordLadder) {
                std::cout << word;
                if (&word != &wordLadder.back()) std::cout << " => ";
            }
            std::cout << "\n";
        } else {
            std::cout << "No wordladder is found between " << startWord << " and " << endWord << ".\n";
        }

        // Tracks the execution time
        clock_t endTime = clock();
        double duration = double(endTime - startTime) / CLOCKS_PER_SEC;
        std::cout << "Execution time: " << duration << " seconds\n";
    }

    return 0;
}
