# frequency_counter 📊

A C-based Command Line Interface (CLI) tool for text analysis and word frequency counting. It parses custom command-line options using K&R-style pointer manipulation and sorts word frequencies efficiently with a custom Quick Sort algorithm.

## 🚀 Key Features

- **Case-Insensitive Analysis (`-i`)**: Converts all text to lowercase to accurately count total word occurrences regardless of case.
- **Top-N Filtering (`-n`)**: Extracts and displays only the top N most frequent words.
- **Minimum Word Length (`-m`)**: Excludes short words (e.g., stop words or articles) below a specified length threshold.
- **Custom Quick Sort Implementation**: Sorts word counts in descending order with optimal runtime efficiency.

## 🛠 Build & Installation

You can compile the project using any GCC-compatible compiler.

```bash
# Clone the repository
git clone [https://github.com/your-username/frequency_counter.git](https://github.com/your-username/frequency_counter.git)
cd frequency_counter

# Build the executable
gcc -O2 main.c -o frequency_counter


📖 Usage
./frequency_counter [OPTIONS] [top_n] [min_len] <filename>

# Examples
./frequency_counter test.txt
./frequency_counter -i test.txt
./frequency_counter -m 4 test.txt
./frequency_counter -i -n 10 test.txt
./frequency_counter -i -n -m 5 3 test.txt

----------------------------------
RANK               WORD      COUNT
----------------------------------
  1                  the        6
  2                  abc        2
  3                 code        1
  4              program        1
  5                kinds        1
----------------------------------

