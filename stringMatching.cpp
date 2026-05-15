#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <climits>

using namespace std;
using namespace chrono;

string globalText = "";


struct Stats {
    int comparisons = 0;
    int matches     = 0;
    double timeMs   = 0.0;
    string algorithm;
    vector<int> matchPositions;
};


void highlightMatch(const string& text, const string& pattern, const vector<int>& positions) {
    if (positions.empty()) {
        cout << text << "\n";
        return;
    }

    int patLen = pattern.size();
    int textLen = text.size();
    string result = "";
    int i = 0;

    vector<int> sorted = positions;
    sort(sorted.begin(), sorted.end());

    int posIdx = 0;
    while (i < textLen) {
        if (posIdx < (int)sorted.size() && i == sorted[posIdx]) {
            result += "[";
            result += text.substr(i, patLen);
            result += "]";
            i += patLen;
            posIdx++;
        } else {
            result += text[i];
            i++;
        }
    }
    cout << result << "\n";
}


void printStatistics(const Stats& s) {
    cout << "\nStatistics\n";
    cout << "Algorithm   : " << s.algorithm << "\n";
    cout << "Comparisons : " << s.comparisons << "\n";
    cout << "Matches     : " << s.matches << "\n";
    cout << "Time (ms)   : " << s.timeMs << "\n";
    if (!s.matchPositions.empty()) {
        cout << "Match positions: ";
        for (int p : s.matchPositions) cout << p << " ";
        cout << "\n";
    }
    cout << "...................\n";
}


void buildBadChar(const string& pattern, int badChar[256]) {
    int m = pattern.size();
    for (int i = 0; i < 256; i++) badChar[i] = -1;
    for (int i = 0; i < m; i++)
        badChar[(unsigned char)pattern[i]] = i;
}

Stats boyerMoore(const string& text, const string& pattern) {
    Stats s;
    s.algorithm = "Boyer-Moore";

    int n = text.size();
    int m = pattern.size();

    if (m == 0 || n == 0 || m > n) {
        return s;
    }

    int badChar[256];
    buildBadChar(pattern, badChar);

    int shift = 0;
    while (shift <= n - m) {
        int j = m - 1;

        while (j >= 0) {
            s.comparisons++;
            if (pattern[j] == text[shift + j])
                j--;
            else
                break;
        }

        if (j < 0) {
            // Full match found
            s.matches++;
            s.matchPositions.push_back(shift);
            // Shift by 1 to find overlapping matches
            shift += 1;
        } else {
            int bc = badChar[(unsigned char)text[shift + j]];
            int skip = j - bc;
            shift += (skip > 1) ? skip : 1;
        }
    }
    return s;
}


Stats rabinKarp(const string& text, const string& pattern) {
    Stats s;
    s.algorithm = "Rabin-Karp";

    int n = text.size();
    int m = pattern.size();

    if (m == 0 || n == 0 || m > n) {
        return s;
    }

    const int BASE  = 256;
    const int MOD   = 101;

    long long patHash  = 0;
    long long textHash = 0;
    long long h = 1;

    for (int i = 0; i < m - 1; i++)
        h = (h * BASE) % MOD;

    // Compute initial hashes
    for (int i = 0; i < m; i++) {
        patHash  = (BASE * patHash  + pattern[i]) % MOD;
        textHash = (BASE * textHash + text[i])    % MOD;
    }

    for (int i = 0; i <= n - m; i++) {
        s.comparisons++; // hash comparison

        if (patHash == textHash) {
            // Verify character by character
            bool match = true;
            for (int j = 0; j < m; j++) {
                s.comparisons++;
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                s.matches++;
                s.matchPositions.push_back(i);
            }
        }

        // Rolling hash (not for last window)
        if (i < n - m) {
            textHash = (BASE * (textHash - (unsigned char)text[i] * h) + (unsigned char)text[i + m]) % MOD;
            if (textHash < 0) textHash += MOD;
        }
    }
    return s;
}


void runSearch(bool useBoyer) {
    if (globalText.empty()) {
        cout << "No text loaded. Please load a file or enter text first.\n";
        return;
    }

    cout << "Enter number of patterns to search: ";
    int numPatterns;
    cin >> numPatterns;
    cin.ignore();

    for (int p = 0; p < numPatterns; p++) {
        cout << "Enter pattern " << (p + 1) << ": ";
        string pattern;
        getline(cin, pattern);

        if (pattern.empty()) {
            cout << "Pattern cannot be empty.\n";
            continue;
        }

        Stats s;
        auto start = high_resolution_clock::now();

        if (useBoyer)
            s = boyerMoore(globalText, pattern);
        else
            s = rabinKarp(globalText, pattern);

        auto end = high_resolution_clock::now();
        s.timeMs = duration<double, milli>(end - start).count();

        cout << "\nText   : " << globalText << "\n";
        cout << "Pattern: " << pattern << "\n";

        if (s.matches == 0) {
            cout << "Result : No match found.\n";
        } else {
            cout << "Result : ";
            highlightMatch(globalText, pattern, s.matchPositions);
            for (int pos : s.matchPositions)
                cout << "Match at index: " << pos << "\n";
        }

        printStatistics(s);
    }
}


void loadFile() {
    cout << "Enter file name: ";
    string filename;
    cin.ignore();
    getline(cin, filename);

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file '" << filename << "'.\n";
        return;
    }

    globalText = "";
    string line;
    bool first = true;
    while (getline(file, line)) {
        if (!first) globalText += " ";
        globalText += line;
        first = false;
    }
    file.close();

    cout << "File loaded successfully. Text length: " << globalText.size() << " characters.\n";
    cout << "Text: " << globalText << "\n";
}

void manualInput() {
    cout << "Enter text: ";
    cin.ignore();
    getline(cin, globalText);
    cout << "Text set. Length: " << globalText.size() << " characters.\n";
}

void boyerMooreSearch() {
    runSearch(true);
}

void rabinKarpSearch() {
    runSearch(false);
}

void compareAlgorithms() {
    if (globalText.empty()) {
        cout << "No text loaded. Please load a file or enter text first.\n";
        return;
    }

    cout << "Enter pattern to compare: ";
    cin.ignore();
    string pattern;
    getline(cin, pattern);

    if (pattern.empty()) {
        cout << "Pattern cannot be empty.\n";
        return;
    }

    // Boyer-Moore
    auto start1 = high_resolution_clock::now();
    Stats bm = boyerMoore(globalText, pattern);
    auto end1   = high_resolution_clock::now();
    bm.timeMs   = duration<double, milli>(end1 - start1).count();

    // Rabin-Karp
    auto start2 = high_resolution_clock::now();
    Stats rk = rabinKarp(globalText, pattern);
    auto end2   = high_resolution_clock::now();
    rk.timeMs   = duration<double, milli>(end2 - start2).count();

    cout << "\nComparison:\n";
    cout << "Text: " << globalText << "\n";
    cout << "Pattern: " << pattern << "\n\n";

    cout << "[Boyer-Moore]\n";
    cout << "Comparisons: " << bm.comparisons << "\n";
    cout << "Matches: " << bm.matches     << "\n";
    cout << "Time (ms): " << bm.timeMs      << "\n";

    cout << "\n[Rabin-Karp]\n";
    cout << "Comparisons: " << rk.comparisons << "\n";
    cout << "Matches: " << rk.matches     << "\n";
    cout << "Time (ms): " << rk.timeMs      << "\n";

    cout << "\n[Winner]\n";
    if (bm.comparisons < rk.comparisons)
        cout << "Fewer comparisons: Boyer-Moore (" << bm.comparisons << " vs " << rk.comparisons << ")\n";
    else if (rk.comparisons < bm.comparisons)
        cout << "Fewer comparisons: Rabin-Karp (" << rk.comparisons << " vs " << bm.comparisons << ")\n";
    else
        cout << "Equal comparisons: " << bm.comparisons << "\n";

    if (bm.timeMs < rk.timeMs)
        cout << "Faster: Boyer-Moore\n";
    else if (rk.timeMs < bm.timeMs)
        cout << "Faster: Rabin-Karp\n";
    else
        cout <<"Same speed\n";

}


int main() {


    int choice = 0;
    while (true) {
        cout << "\nMenu\n";
        cout << "1.Load Text File\n";
        cout << "2.Enter Text Manually\n";
        cout << "3.Search Using Boyer-Moore\n";
        cout << "4.Search Using Rabin-Karp\n";
        cout << "5.Compare Algorithms\n";
        cout << "6.Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "Invalid input. Please enter a number 1-6.\n";
            continue;
        }

        switch (choice) {
            case 1: loadFile();           break;
            case 2: manualInput();        break;
            case 3: boyerMooreSearch();   break;
            case 4: rabinKarpSearch();    break;
            case 5: compareAlgorithms();  break;
            case 6:
                return 0;
            default:
                cout << "Invalid choice. Enter 1-6.\n";
        }
    }
}