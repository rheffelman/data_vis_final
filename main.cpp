#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "RCV.h"

using namespace std;

// Function to run a method and collect results
void runMethodAndStoreResults(const string& methodName, void(RCV::*method)(), const string& ballots, ofstream& outFile) {
    RCV vote;

    int added = vote.addBallotsFromCSV(const_cast<string&>(ballots));
    if (added == 0) {
        cerr << "⚠ Failed to load ballots for method: " << methodName << "\n";
        return;
    }

    (vote.*method)();

    vector<pair<string, double>> results = vote.getResults();

    if (results.empty()) {
        cerr << "⚠ No results returned for method: " << methodName << "\n";
        return;
    }

    // Write header row
    outFile << methodName;
    for (const auto& [name, score] : results) {
        outFile << "," << name << "," << fixed << setprecision(2) << score;
    }
    outFile << "\n";
}


int main() {
    // Load ballots from file
    ifstream file("./data/converted_ballots.csv");
    string line, ballots;

    while (getline(file, line)) {
        ballots += line + "\n";
    }

    // Output CSV header
    ofstream outFile("./data/method_outcomes.csv");
    outFile << "Method,Candidate1,Score1,Candidate2,Score2,Candidate3,Score3,Candidate4,Score4\n";

    // List of methods to evaluate
    vector<pair<string, void(RCV::*)()>> methods = {
        {"Plurality", &RCV::computePlurality},
        {"Majority", &RCV::computeMajority},
        {"Borda", &RCV::computeBorda},
        {"IRV", &RCV::computeIRV},
        {"PluralityWithRunoff", &RCV::computePluralityWithRunoff},
        {"Copeland", &RCV::computeCopeland},
        {"Dowdall", &RCV::computeDowdall},
        {"Minimax", &RCV::computeMinimax},
        {"Baldwin", &RCV::computeBaldwin},
        {"CopelandBorda", &RCV::computeCopelandBorda}//,
        //{"UltimateVotingSystem", &RCV::UltimateVotingSystem}
    };

    for (const auto& [name, methodPtr] : methods) {
        runMethodAndStoreResults(name, methodPtr, ballots, outFile);
    }

    outFile.close();
    cout << "Voting method outcomes written to method_outcomes.csv\n";
    return 0;
}
