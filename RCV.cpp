#include"RCV.h"

void RCV::UltimateVotingSystem()
{
    results.clear();

    // compute Borda Count election
    computeBorda();
    // store results
    vector<pair<string, double>> bordaResultsTemp = results;
    vector<string> bordaResults;
    for (int i = 0; i < bordaResultsTemp.size(); i++)
        bordaResults.push_back(bordaResultsTemp[i].first);

    // compute Plurality election
    computePlurality();
    // store results
    vector<pair<string, double>> pluralityResultsTemp = results;
    vector<string> pluralityResults;
    for (int i = 0; i < pluralityResultsTemp.size(); i++)
        pluralityResults.push_back(pluralityResultsTemp[i].first);
        
    // compute Plurality with Runoff election
    computePluralityWithRunoff();
    // store results
    vector<pair<string, double>> pluralityRunoffResultsTemp = results;
    vector<string> pluralityRunoffResults;
    for (int i = 0; i < pluralityRunoffResultsTemp.size(); i++)
        pluralityRunoffResults.push_back(pluralityRunoffResultsTemp[i].first);

    // compute IRV election
    computeIRV();
    // store results
    vector<pair<string, double>> IRVResultsTemp = results;
    vector<string> IRVResults;
    for (int i = 0; i < IRVResultsTemp.size(); i++)
        IRVResults.push_back(IRVResultsTemp[i].first);

    // compute Copeland method election
    computeCopeland();
    // store results
    vector<pair<string, double>> copelandResultsTemp = results;
    vector<string> copelandResults;
    for (int i = 0; i < copelandResultsTemp.size(); i++)
        copelandResults.push_back(copelandResultsTemp[i].first);

    // compute Dowdall system election
    computeDowdall();
    // store results
    vector<pair<string, double>> dowdallResultsTemp = results;
    vector<string> dowdallResults;
    for (int i = 0; i < dowdallResultsTemp.size(); i++)
        dowdallResults.push_back(dowdallResultsTemp[i].first);

    // compute Minimax election
    computeMinimax();
    // store results
    vector<pair<string, double>> minimaxResultsTemp = results;
    vector<string> minimaxResults;
    for (int i = 0; i < minimaxResultsTemp.size(); i++) 
        minimaxResults.push_back(minimaxResultsTemp[i].first);

    // compute Baldwin method election
    computeBaldwin();
    // store results
    vector<pair<string, double>> baldwinResultsTemp = results;
    vector<string> baldwinResults;
    for (int i = 0; i < baldwinResultsTemp.size(); i++) 
        baldwinResults.push_back(baldwinResultsTemp[i].first);

    // compute Copeland-Borda hybrid election (pairwise comparison of each candidate with the excess tallied up)
    computeCopelandBorda();
    // store results
    vector<pair<string, double>> CopelandBordaResultsTemp = results;
    vector<string> CopelandBordaResults;
    for (int i = 0; i < CopelandBordaResultsTemp.size(); i++) 
        CopelandBordaResults.push_back(CopelandBordaResultsTemp[i].first);

    // eliminate the initial ballots
    ballots.clear();

    // treating all of the stored ranked results of the previous elections as ballots in and of
    // themselves, feed the ballots back into the election system.
    addBallot(bordaResults);
    addBallot(pluralityResults);
    addBallot(pluralityRunoffResults);
    addBallot(IRVResults);
    addBallot(copelandResults);
    addBallot(dowdallResults);
    addBallot(minimaxResults);
    addBallot(baldwinResults);
    addBallot(CopelandBordaResults);

    // using the ballots created from the ranked order produced by 9 different voting systems,
    // compute a borda count and print the results.
    computeBorda();
    printResults();

    // Recursively call this method to keep getting "better and better" results.
    UltimateVotingSystem();
}
//--
void RCV::computeMajority()
{
    if (!candidates.size())
    {
        printf("Can't do a vote with no candidates!\n");
    }

    int majorityCutoff = (ballots.size() / 2);
    results.resize(candidates.size());

    for (int i = 0; i < candidates.size(); i++)
    {
        pair <string, int> candidateResult;
        candidateResult.first = candidates[i]->name;

        if (candidates[i]->nthPlaceVotes[0] > majorityCutoff)
        {
            candidateResult.second = 1;
        }
        else
        {
            candidateResult.second = 0;
        }
        results[i] = candidateResult;
    }

    sort(results.begin(), results.end(), (*compare));
}
//--
void RCV::computeBorda()
{
    if (!candidates.size())
    {
        printf("Can't do a vote with no candidates!\n");
        return;
    }

    results.resize(candidates.size());

    for (int i = 0; i < candidates.size(); i++)
    {
        int x = candidates.size() - 1;
        int count = 0;
        for (int j = 0; j < candidates[i]->nthPlaceVotes.size() - 1; j++)
        {
            count += candidates[i]->nthPlaceVotes[j] * x;
            x--;
        }
        pair <string, int> candidateResult;
        candidateResult.first = candidates[i]->name;
        candidateResult.second = count;
        results[i] = candidateResult;
    }

    sort(results.begin(), results.end(), (*compare));
}
//--
void RCV::computePlurality()
{
    results.resize(candidates.size());

    for (int i = 0; i < candidates.size(); i++)
    {
        pair <string, int> candidateResult;
        candidateResult.first = candidates[i]->name;
        candidateResult.second = candidates[i]->nthPlaceVotes[0];
        results[i] = candidateResult;
    }

    sort(results.begin(), results.end(), (*compare));
}
//--
void RCV::computePluralityWithRunoff()
{
    if (candidates.size() < 2)
    {
        printf("Too few candidates to do a vote.\n");
    }

    results.resize(candidates.size());
    vector <pair<string, int>> pluralityOrder;

    for (int i = 0; i < candidates.size(); i++)
    {
        pair <string, int> candidateResult;
        candidateResult.first = candidates[i]->name;
        candidateResult.second = candidates[i]->nthPlaceVotes[0];
        pluralityOrder.push_back(candidateResult);
    }

    sort(pluralityOrder.begin(), pluralityOrder.end(), (*compare));
    pair <string, int> candidate1 = pluralityOrder[0];
    pair <string, int> candidate2 = pluralityOrder[1];
    int ar[2] = { 0, 0 };

    for (int i = 0; i < ballots.size(); i++)
    {
        for (int j = 0; j < ballots[i].size(); j++)
        {
            if (ballots[i][j] == candidate1.first)
            {
                ar[0]++;
                break;
            }
            else if (ballots[i][j] == candidate2.first)
            {
                ar[1]++;
                break;
            }
        }
    }

    pair <string, double> candidateResult;
    candidateResult.first = candidate1.first;
    candidateResult.second = ar[0];
    results[0] = candidateResult;

    pair <string, double> candidateResult2;
    candidateResult2.first = candidate2.first;
    candidateResult2.second = ar[1];
    results[1] = candidateResult2;

    for (int i = 2; i < candidates.size(); i++)
    {
        results[i] = pluralityOrder[i];
        results[i].second = 0;
    }

    sort(results.begin(), results.end(), (*compare));
}
//--
void RCV::computeIRV()
{
    results.clear(); 
    computeIRVHelper(candidates, ballots, 1);
    sort(results.begin(), results.end(), [](const pair<string, double>& a, const pair<string, double>& b)
    {
        return a.second > b.second;
    });
}
//--
void RCV::computeIRVHelper(vector<Candidate*>& remainingCandidates, vector<vector<string>>& remainingBallots, int rank)
{
    if (remainingCandidates.size() == 1)
    {
        results.push_back({ remainingCandidates[0]->name, static_cast<double>(rank) });
        return;
    }

    map<string, int> voteCount;
    for (auto& ballot : remainingBallots)
    {
        if (!ballot.empty())
        {
            string firstChoice = ballot[0];
            voteCount[firstChoice]++;
        }
    }

    string candidateToEliminate;
    int fewestVotes = INT_MAX;
    for (auto& candidate : remainingCandidates)
    {
        if (voteCount[candidate->name] < fewestVotes)
        {
            fewestVotes = voteCount[candidate->name];
            candidateToEliminate = candidate->name;
        }
    }

    vector<Candidate*> nextCandidates;
    for (auto& candidate : remainingCandidates)
    {
        if (candidate->name != candidateToEliminate)
        {
            nextCandidates.push_back(candidate);
        }
    }

    vector<vector<string>> nextBallots = remainingBallots;
    for (auto& ballot : nextBallots)
    {
        ballot.erase(remove(ballot.begin(), ballot.end(), candidateToEliminate), ballot.end());
    }

    computeIRVHelper(nextCandidates, nextBallots, rank + 1);
    results.push_back({ candidateToEliminate, static_cast<double>(rank) });
}
//--
void RCV::computeCopeland()
{
    if (candidates.size() < 2)
    {
        printf("Too few candidates to do a vote.\n");
        return;
    }

    map<string, double> copelandScores;

    for (auto& candidate : candidates)
    {
        copelandScores[candidate->name] = 0;
    }

    for (int i = 0; i < candidates.size(); i++)
    {
        for (int j = i + 1; j < candidates.size(); j++)
        {
            int winsI = 0, winsJ = 0;

            for (auto& ballot : ballots)
            {
                auto posI = find(ballot.begin(), ballot.end(), candidates[i]->name);
                auto posJ = find(ballot.begin(), ballot.end(), candidates[j]->name);
                if (posI < posJ)
                {
                    winsI++;
                }
                else if (posJ < posI)
                {
                    winsJ++;
                }
            }

            if (winsI > winsJ)
            {
                copelandScores[candidates[i]->name]++;
            }
            else if (winsJ > winsI)
            {
                copelandScores[candidates[j]->name]++;
            }
        }
    }

    results.clear();
    for (auto& score : copelandScores)
    {
        results.push_back(make_pair(score.first, score.second));
    }

    sort(results.begin(), results.end(), [](const pair<string, double>& a, const pair<string, double>& b)
    {
        return a.second > b.second;
    });
}
//--
void RCV::computeDowdall()
{
    if (candidates.size() < 2)
    {
        printf("Too few candidates to do a vote.\n");
        return;
    }
    
    results.resize(candidates.size());

    for (int i = 0; i < candidates.size(); i++)
    {
        pair <string, double> candidateResult;
        candidateResult.first = candidates[i]->name;
        candidateResult.second = 0;
        results[i] = candidateResult;
    }

    for (int i = 0; i < candidates.size(); i++)
    {
        for (int j = 0; j < candidates[i]->nthPlaceVotes.size(); j++)
        {
            results[i].second += static_cast<double>(candidates[i]->nthPlaceVotes[j]) / static_cast<double>(j + 1.0f);
        }
    }
    sort(results.begin(), results.end(), compare);
}
//--
void RCV::computeMinimax()
{
    if (candidates.size() < 2)
    {
        printf("Too few candidates to do a vote.\n");
        return;
    }

    results.clear();
    vector <vector<pair<int, int>>> comparisonTable;
    comparisonTable.resize(candidates.size());

    for (int i = 0; i < candidates.size(); i++)
    {
        comparisonTable[i].resize(candidates.size());
    }

    for (int i = 0; i < comparisonTable.size(); i++)
    {
        for (int j = 0; j < comparisonTable[i].size(); j++)
        {
            comparisonTable[i][j] = prefers(candidates[i], candidates[j]);
        }
    }

    for (int i = 0; i < comparisonTable.size(); i++)
    {
        int min = INT_MAX;
        for (int j = 0; j < comparisonTable[i].size(); j++)
        {
            if (i == j)
            {
                continue;
            }
            if (comparisonTable[i][j].first < min)
            {
                min = comparisonTable[i][j].first;
            }
        }
        pair <string, double> result;
        result.first = candidates[i]->name;
        result.second = static_cast<double>(min);
        results.push_back(result);
    }
    sort(results.begin(), results.end(), compare);
}
//--
void RCV::computeBaldwin()
{
    results.clear();
    computeBaldwinHelper(candidates, ballots, 1);
    sort(results.begin(), results.end(), compare);

}
//--
void RCV::computeBaldwinHelper(vector<Candidate*>& remainingCandidates, vector<vector<string>>& remainingBallots, int rank)
{
    if (remainingCandidates.size() == 1)
    {
        results.push_back( {remainingCandidates[0]->name, static_cast<double>(rank)});
        return;
    }

    // compute a borda count with the remaining candidates
    map <string, int> bordaCount;
    for (int i = 0; i < remainingBallots.size(); i++)
    {
        int x = candidates.size() - 1;
        int count = 0;
        for (int j = 0; j < remainingBallots[i].size(); j++)
        {
            bordaCount[remainingBallots[i][j]] += x;
            x--;
        }
    }

    // identify which candidate has the lowest borda count
    std::string candidateToEliminate;
    int smallestCount = INT_MAX;
    for (int i = 0; i < remainingCandidates.size(); i++)
    {
        if (bordaCount[remainingCandidates[i]->name] < smallestCount)
        {
            smallestCount = bordaCount[remainingCandidates[i]->name];
            candidateToEliminate = remainingCandidates[i]->name;
        }
    }

    // setup the candidate vector for the next recursion to use with 1 candidate eliminated
    vector <Candidate*> nextRecursionCandidates;
    for (int i = 0; i < remainingCandidates.size(); i++)
    {
        if (remainingCandidates[i]->name != candidateToEliminate)
        {
            nextRecursionCandidates.push_back(remainingCandidates[i]);
        }
    }

    // setup the ballots vector for the next recursion without any instances of the eliminated candidate.
    vector<vector<string>> nextBallots = remainingBallots;
    for (int i = 0; i < nextBallots.size(); i++)
    {
        nextBallots[i].erase(remove(nextBallots[i].begin(), nextBallots[i].end(), candidateToEliminate), nextBallots[i].end());
    }

    computeBaldwinHelper(nextRecursionCandidates, nextBallots, rank + 1);
    results.push_back({ candidateToEliminate, static_cast<double>(rank) });
}
//--
void RCV::computeCopelandBorda()
{
    if (candidates.size() < 2)
    {
        printf("Too few candidates to do a vote.\n");
        return;
    }

    results.clear();

    for (int i = 0; i < candidates.size(); i++)
    {
        pair<string, double> result;
        result.first = candidates[i]->name;
        result.second = 0;
        for (int j = 0; j < candidates.size(); j++)
        {
            if (i != j)
            {
                pair <int, int> comparison = prefers(candidates[i], candidates[j]);
                if (comparison.first - comparison.second > 0)
                {
                    result.second += comparison.first - comparison.second;
                }
            }
        }
        results.push_back(result);
    }

    sort(results.begin(), results.end(), compare);
}
//--
int RCV::addCandidate(string& name)
{
    // Skip special tokens
    if (ignoredCandidates.count(name)) {
        return -1;
    }

    if (getCandidate(name) != -1) {
        printf("Candidate already exists!\n");
        return -1;
    }

    Candidate* temp = new Candidate(name);
    temp->nthPlaceVotes.resize(candidates.size(), 0);  // initialize with N existing candidates

    candidates.push_back(temp);
    candidates_HT.insert(name);
    numCandidates++;

    // Add a new vote slot to each existing candidate for this new candidate's position
    for (auto& c : candidates) {
        c->nthPlaceVotes.push_back(0);
    }

    return 0;
}

//--
void RCV::addBallot(vector<string>& b)
{
    if (b.empty()) {
        printf("Ballots must contain at least 1 vote.\n");
        return;
    }

    // Cleanse and keep valid ballot
    for (auto& name : b) {
        name = cleanse(name);
    }

    ballots.push_back(b);
    numBallots++;

    for (int i = 0; i < b.size(); i++) {
        string& candidate = b[i];

        if (ignoredCandidates.count(candidate)) {
            continue; // don't count undervote/overvote toward tally
        }

        int idx = getCandidate(candidate);
        if (idx == -1) {
            addCandidate(candidate);
            idx = getCandidate(candidate);  // re-fetch after adding
        }

        if (idx != -1) {
            if (i < candidates[idx]->nthPlaceVotes.size()) {
                candidates[idx]->nthPlaceVotes[i]++;
            } else {
                // safety: grow vector if something misaligned
                candidates[idx]->nthPlaceVotes.resize(i + 1, 0);
                candidates[idx]->nthPlaceVotes[i]++;
            }
        }
    }
}
//--
int RCV::addBallotsFromCSV(string& CSVinput)
{
    stringstream ss(CSVinput);
    string line;
    bool isHeader = true;
    int count = 0;

    while (getline(ss, line)) {
        if (line.empty()) continue;

        vector<string> ballot;
        stringstream ls(line);
        string token;

        while (getline(ls, token, ',')) {
            // Trim whitespace
            token.erase(0, token.find_first_not_of(" \t\r\n"));
            token.erase(token.find_last_not_of(" \t\r\n") + 1);

            if (!token.empty()) {
                ballot.push_back(token);
            }
        }

        if (isHeader) {
            // Optional: add all candidate names from the header, or ignore them
            for (const auto& name : ballot) {
                if (!ignoredCandidates.count(name) && candidates_HT.count(name) == 0) {
                    addCandidate(const_cast<string&>(name));
                }
            }
            isHeader = false;
            continue;
        }

        if (!ballot.empty()) {
            addBallot(ballot);
            count++;
        }
    }

    return count;
}
//--
vector <string> RCV::getCandidateNames()
{
    vector <string> names;
    for (int i = 0; i < candidates.size(); i++)
    {
        names.push_back(candidates[i]->name);
    }
    return names;
}
//--
int RCV::getBallotCount()
{
    return ballots.size();
}
//--
int RCV::getCandidateCount()
{
    return candidates.size();
}
//--
vector <pair<string, double>> RCV::getResults()
{
    return results; 
}
//--
void RCV::printResults()
{
    bool noWinners = true;
    for (int i = 0; i < results.size(); i++)
    {
        if (results[i].second != 0)
        {
            noWinners = false;
        }
    }

    if (noWinners)
    {
        printf("NO CANDIDATE won the vote.\n");
    }
    else
    {
        printf("%s won the vote!\n", results[0].first.c_str());
    }

    for (int i = 0; i < results.size(); i++)
    {
        printf("#%d) %s, %f\n", i + 1, results[i].first.c_str(), results[i].second);
    }
}
//--
void RCV::printCandidates()
{
    for (int i = 0; i < candidates.size(); i++)
    {
        printf("%s\n", candidates[i]->name.c_str());
    }
}
//--
void RCV::printCandidateData()
{
    for (int i = 0; i < candidates.size(); i++)
    {
        printf("%s, ", candidates[i]->name.c_str());
        for (int j = 0; j < candidates[i]->nthPlaceVotes.size(); j++)
        {
            if (j == candidates[i]->nthPlaceVotes.size() - 1)
            {
                printf("%d\n", candidates[i]->nthPlaceVotes[j]);
            }
            else
            {
                printf("%d, ", candidates[i]->nthPlaceVotes[j]);
            }
        }
    }
    printf("\n");
}
//--
int RCV::getCandidate(string& candName)
{
    for (int i = 0; i < candidates.size(); i++)
    {
        if (candidates[i]->name == candName)
        {
            return i;
        }
    }
    return -1;
}
//--
bool RCV::compare(pair <string, double> x, pair<string, double> y)
{
    return x.second > y.second;
}
//--
void RCV::eliminateCandidate(Candidate* c)
{
    eliminated_HT.insert(c->name);

    for (int i = 0; i < eliminatedBallots.size(); i++)
    {
        for (int j = 0; j < eliminatedBallots[i].second.size(); j++)
        {
            if (eliminated_HT.count(eliminatedBallots[i].second[j]))
            {
                eliminatedBallots[i].second.erase(eliminatedBallots[i].second.begin() + j);
            }
        }
    }
}
//--
string RCV::cleanse(const string& word)
{
    string retString = "";
    int x;
    int count = 0;

    for (int i = 0; i < word.size(); i++)
    {
        x = int(word[i]);

        if ((x > 96) and (x < 123) or (x > 191) and (x < 256)) //if it's a lowercase letter or utf8 character, add it to the return string.
        {
            retString += char(x);
        }
        else if (x > 64 and x < 91) //if it's an uppercase letter, convert to it's lowercase equivalent, and add it to the return string.
        {
            retString += char(x);
        }
    }
    return retString;
}
//--
string RCV::getCondorcetWinner()
{
    
    for (int i = 0; i < candidates.size(); i++)
    {
        bool condorcetExists = true;
        for (int j = 0; j < candidates.size(); j++)
        {
            pair <int, int> comparison = prefers(candidates[i], candidates[j]);
            if (comparison.second >= comparison.first)
            {
                condorcetExists = false;
                break;
            }
        }
        if (condorcetExists)
        {
            return candidates[i]->name;
        }
    }
    return "No Condorcet Winner";
}
//--
pair<int, int> RCV::prefers(Candidate* a, Candidate* b)
{
    int tallya = 0;
    int tallyb = 0;
    
    for (int i = 0; i < ballots.size(); i++)
    {
        for (int j = 0; j < ballots[i].size(); j++)
        {
            if (ballots[i][j] == a->name)
            {
                tallya++;
                break;
            }
            else if (ballots[i][j] == b->name)
            {
                tallyb++;
                break;
            }
        }
    }

    pair <int, int> retPair;
    retPair.first = tallya;
    retPair.second = tallyb;
    return retPair;
}