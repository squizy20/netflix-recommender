/*
 * Netflix Movie Recommendation Assistant
 * LDCW6123 Group Project - Part 2 (Interactive Program)
 *
 * Inspired by Netflix, the disruptive innovation traced in Part 1.
 * Inputs : genre, mood, age group, time available
 * Outputs: best-match title, a runner-up, or a friendly "no match" message
 * Logic  : switch (genre names), if/else (age + time filters), scoring (mood)
 *
 * Build  : g++ -Wall -Wextra -o recommender main.cpp
 * Run    : ./recommender
 */
#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
using namespace std;

// One catalogue entry
struct Movie {
    string title;
    int genre;      // 1-7
    int mood;       // 1 fun, 2 thrilling, 3 thoughtful
    int minutes;    // approximate runtime
    int minAge;     // suggested minimum age
    string blurb;
};

const int MOVIE_COUNT = 14;
Movie catalogue[MOVIE_COUNT] = {
    {"Extraction", 1, 2, 116, 18, "A mercenary races to rescue a kidnapped boy through a hostile city."},
    {"The Gray Man", 1, 1, 129, 16, "A freelance agent is hunted across the globe by a rival assassin."},
    {"Murder Mystery", 2, 1, 97, 13, "A couple on holiday is framed for a billionaire's murder."},
    {"Don't Look Up", 2, 3, 138, 16, "Two scientists try to warn a distracted world about a comet."},
    {"Roma", 3, 3, 135, 16, "A year in the life of a domestic worker in 1970s Mexico City."},
    {"Marriage Story", 3, 3, 137, 16, "A couple navigates a painful and complicated divorce."},
    {"The Adam Project", 4, 1, 106, 13, "A time-travelling pilot teams up with his younger self."},
    {"I Am Mother", 4, 2, 113, 13, "A girl raised by a robot questions the world outside."},
    {"The Ritual", 5, 2, 94, 18, "Friends hiking in a Swedish forest meet something ancient."},
    {"Hush", 5, 2, 82, 18, "A deaf writer fights off a masked intruder."},
    {"My Octopus Teacher", 6, 3, 85, 0, "A filmmaker forms a bond with an octopus over a year."},
    {"The Social Dilemma", 6, 3, 94, 13, "Tech insiders discuss the effects of social media."},
    {"Klaus", 7, 1, 96, 0, "A postman and a toymaker bring cheer to a feuding village."},
    {"The Mitchells vs. the Machines", 7, 1, 114, 0, "A quirky family must stop a robot uprising."}
};

void printMovie(const Movie &m) {
    cout << "  " << m.title << " (" << m.minutes << " min, " << m.minAge << "+)\n"
         << "  " << m.blurb << "\n";
}

// switch: turn a genre number into its name
string genreName(int g) {
    switch (g) {
        case 1: return "Action";
        case 2: return "Comedy";
        case 3: return "Drama";
        case 4: return "Sci-Fi";
        case 5: return "Horror";
        case 6: return "Documentary";
        case 7: return "Animation";
        default: return "Unknown";
    }
}

// Read an int in [lo, hi], re-asking until the input is valid
int readChoice(const string &prompt, int lo, int hi) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= lo && value <= hi) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        if (cin.eof()) {            // input ended - stop cleanly
            cout << "\nNo more input. Goodbye!\n";
            exit(EXIT_SUCCESS);
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Please enter a number from " << lo << " to " << hi << ".\n";
    }
}

// if/else: convert the age-group choice into the oldest rating allowed
int maxAgeForGroup(int group) {
    if (group == 1) return 12;       // under 13
    else if (group == 2) return 13;  // 13-15
    else if (group == 3) return 16;  // 16-17
    else return 18;                  // 18 and above
}

// Score = genre match (3) + mood match (2). Returns 0 if the movie is filtered out.
int scoreMovie(const Movie &m, int genre, int mood, int maxAge, int timeLimit) {
    if (m.minAge > maxAge) return 0;      // too mature for this viewer
    if (m.minutes > timeLimit) return 0;  // too long for the time available
    int score = 0;
    if (m.genre == genre) score += 3;
    if (m.mood == mood) score += 2;
    return score;
}

void recommend() {
    cout << "\nGenre:  1 Action  2 Comedy  3 Drama  4 Sci-Fi  5 Horror  6 Documentary  7 Animation\n";
    int genre = readChoice("Pick a genre (1-7): ", 1, 7);

    cout << "\nMood:   1 Fun  2 Thrilling  3 Thoughtful\n";
    int mood = readChoice("Pick a mood (1-3): ", 1, 3);

    cout << "\nAge:    1 Under 13  2 13-15  3 16-17  4 18 and above\n";
    int ageGroup = readChoice("Pick your age group (1-4): ", 1, 4);

    int timeLimit = readChoice("\nHow many minutes do you have? (60-300): ", 60, 300);

    int maxAge = maxAgeForGroup(ageGroup);

    // Find the best and second-best scoring movies
    int bestIdx = -1, secondIdx = -1, bestScore = 0, secondScore = 0;
    for (int i = 0; i < MOVIE_COUNT; i++) {
        int s = scoreMovie(catalogue[i], genre, mood, maxAge, timeLimit);
        if (s > bestScore) {
            secondIdx = bestIdx;  secondScore = bestScore;
            bestIdx = i;          bestScore = s;
        } else if (s > secondScore) {
            secondIdx = i;        secondScore = s;
        }
    }

    cout << "\n----------------------------------------\n";
    if (bestIdx == -1) {
        cout << "Sorry, nothing in our catalogue fits those choices.\n"
             << "Try more time, a different mood, or another genre!\n";
    } else {
        if (catalogue[bestIdx].genre == genre)
            cout << "Your best match (" << genreName(genre) << "):\n";
        else
            cout << "No " << genreName(genre) << " title fits, but this is the closest match ("
                 << genreName(catalogue[bestIdx].genre) << "):\n";
        printMovie(catalogue[bestIdx]);

        if (secondIdx != -1) {
            cout << "\nRunner-up (" << genreName(catalogue[secondIdx].genre) << "):\n";
            printMovie(catalogue[secondIdx]);
        }
    }
    cout << "----------------------------------------\n";
}
int main() {
    cout << "=== Netflix Movie Recommendation Assistant ===\n";
    char again;
    do {
        recommend();
        while (true) {
            cout << "\nTry another recommendation? (y/n): ";
            if (!(cin >> again)) {          // input ended - stop cleanly
                cout << "\nGoodbye!\n";
                return 0;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (again == 'y' || again == 'Y' || again == 'n' || again == 'N')
                break;
            cout << "  Please enter y or n.\n";
        }
    } while (again == 'y' || again == 'Y');

    cout << "Enjoy the movie!\n";
    return 0;
}