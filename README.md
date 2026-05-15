# Assignment 4: String Matching Algorithms

#JANA FAWZI - 900241258

## Overview
This program implements and compares two string matching algorithms:
- **Boyer-Moore Algorithm**
- **Rabin-Karp Algorithm**

---

## How to Compile
```
g++ -o assignment4 Assignment4_StringMatching.cpp
```

## How to Run
```
./assignment4
```

---

## Menu Options

| Option | Description |
|--------|-------------|
| 1 | Load text from a `.txt` file |
| 2 | Enter text manually |
| 3 | Search using Boyer-Moore |
| 4 | Search using Rabin-Karp |
| 5 | Compare both algorithms |
| 6 | Exit |

---

## How to Use

**Step 1 — Load your text (choose option 1 or 2)**
```
Enter choice: 2
Enter text: MISSISSIPPI
```

**Step 2 — Search (choose option 3 or 4)**
```
Enter choice: 3
Enter number of patterns to search: 1
Enter pattern 1: ISSI
```

---

## Test Cases

| Text | Pattern | Expected Result |
|------|---------|-----------------|
| AAAAAAAAA | AAA | Multiple overlapping matches |
| DATA STRUCTURE | STRUCTURE | Match found at index 5 |
| HELLO WORLD | TEST | No match found |
| MISSISSIPPI | ISSI | Matches at index 1 and 4 |
| ALGORITHM | RITH | Match at index 4 |

---

## Sample Output
```
Text   : MISSISSIPPI
Pattern: ISSI
Result : M[ISSI]SS[ISSI]PPI
Match at index: 1
Match at index: 4

--- Statistics ---
Algorithm   : Boyer-Moore
Comparisons : 12
Matches     : 2
Time (ms)   : 0.003
------------------
```

---

## Algorithm Summary

### Boyer-Moore
- Compares pattern from **right to left**
- Skips sections of text using the **bad character rule**
- Fewer comparisons on large texts
- Best for: single pattern, long texts

### Rabin-Karp
- Converts pattern to a **hash number**
- Compares hashes before characters
- Uses **rolling hash** for efficiency
- Best for: multiple patterns

---

## Complexity

| Algorithm | Time Complexity | Space Complexity |
|-----------|----------------|-----------------|
| Boyer-Moore | O(n/m) best case | O(1) |
| Rabin-Karp | O(n+m) average | O(1) |

- n = length of text
- m = length of pattern

---

## File Structure
```
project/
│
├── Assignment4_StringMatching.cpp   # Main source code
├── input.txt                        # Sample input file (optional)
└── README.md                        # This file
```

---

## Input File Format (input.txt)
```
DATA STRUCTURE AND ALGORITHMS COURSE
THIS COURSE TEACHES DATA HANDLING AND TREE STRUCTURES
RABIN KARP AND BOYER MOORE ARE STRING MATCHING ALGORITHMS
DATA DATA DATA STRUCTURE STRUCTURE
AAAAAA TEST FOR OVERLAPPING AAA PATTERN
```
