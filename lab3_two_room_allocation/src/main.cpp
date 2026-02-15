\
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

struct Item {
    std::string name;
    int width = 0;
    int value = 0;
};

static void usage(const char* prog) {
    std::cerr << "Usage: " << prog << " datafile\n";
}

int main(int argc, char** argv) {
    if (argc != 2) { usage(argv[0]); return 2; }
    std::ifstream in(argv[1]);
    if (!in) { std::cerr << "Error: cannot open " << argv[1] << "\n"; return 2; }

    int CA=0, CB=0, n=0;
    in >> CA >> CB >> n;
    if (!in || CA < 0 || CB < 0 || n < 0) { std::cerr << "Error: bad header\n"; return 2; }

    std::vector<Item> items(n);
    for (int i = 0; i < n; i++) {
        in >> items[i].name >> items[i].width >> items[i].value;
        if (!in) { std::cerr << "Error: bad item line " << i << "\n"; return 2; }
    }

    std::cout << "Lab 3 (two-room allocation / 2D knapsack)\n";
    std::cout << "CA=" << CA << " CB=" << CB << " n=" << n << "\nItems:\n";
    for (auto& it : items) {
        std::cout << "  " << std::setw(8) << it.name
                  << " width=" << std::setw(2) << it.width
                  << " value=" << std::setw(3) << it.value << "\n";
    }
    std::cout << "\n";

    // dp[a][b] for current prefix
    std::vector<std::vector<int>> dp(CA+1, std::vector<int>(CB+1, 0));

    // ============================
    // TODO: fill dp across items
    // Use next table to keep it simple:
    // next = dp; then consider placing item in A or B.
    // ============================

    std::vector<std::vector<std::vector<int>>> choices(n, std::vector<std::vector<int>>(CA + 1, std::vector<int>(CB + 1, 0)));

    for (int i = 0; i < n; i++) {
        // Create a copy of current DP
        std::vector<std::vector<int>> next_dp = dp;
        int w = items[i].width;
        int v = items[i].value;

        for (int a = 0; a <= CA; a++) {
            for (int b = 0; b <= CB; b++) {
                // Option 1: Skip (Already in next_dp[a][b] from the copy)
                choices[i][a][b] = 0; 

                // Option 2: Place in Room A
                if (a >= w) {
                    int val_A = dp[a - w][b] + v;
                    if (val_A > next_dp[a][b]) {
                        next_dp[a][b] = val_A;
                        choices[i][a][b] = 1;
                    }
                }

                // Option 3: Place in Room B
                if (b >= w) {
                    int val_B = dp[a][b - w] + v;
                    if (val_B > next_dp[a][b]) {
                        next_dp[a][b] = val_B;
                        choices[i][a][b] = 2;
                    }
                }
            }
        }
        dp = next_dp; // Move to the DP for next item
    }

    std::cout << "Optimal value: " << dp[CA][CB] << "\n\n";

    // Reconstruct assignments via backtracking
    std::vector<std::string> assignment(n, "not shown");
    int curr_a = CA;
    int curr_b = CB;

    for (int i = n - 1; i >= 0; i--) {
        int choice = choices[i][curr_a][curr_b];
        if (choice == 1) { // Room A
            assignment[i] = "Room A";
            curr_a -= items[i].width;
        } else if (choice == 2) { // Room B
            assignment[i] = "Room B";
            curr_b -= items[i].width;
        }
    }

    std::cout << "Assignments:\n";
    for (int i = 0; i < n; i++) {
        std::cout << "  " << std::setw(8) << items[i].name << ": " << assignment[i] << "\n";
    }
    std::cout << "TODO: compute optimal value dp[CA][CB] and reconstruct assignments.\n";

    // For small capacities, print dp grid (currently zeros)
    if (CA <= 25 && CB <= 25) {
        std::cout << "\nDP grid dp[a][b] (currently empty / zeros):\n";
        std::cout << "      b ->";
        for (int b = 0; b <= CB; b++) std::cout << std::setw(4) << b;
        std::cout << "\n";
        for (int a = 0; a <= CA; a++) {
            std::cout << "a=" << std::setw(2) << a << "  ";
            for (int b = 0; b <= CB; b++) std::cout << std::setw(4) << dp[a][b];
            std::cout << "\n";
        }
    }

    return 0;
}
