#include "DisjointSet.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>
#include <algorithm>

void convertBImgToBools(const std::string& filePath, std::vector<std::vector<bool>>& image) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::vector<bool> row;
        for (char pixel : line) {
            row.push_back(pixel == '+');
        }
        image.push_back(row);
    }
}

DisjointSet getcomponents(std::vector<std::vector<bool>>& grid, int* empty) {
    if (grid.empty() || grid[0].empty()) return 0;
    int rows = grid.size();
    int cols = grid[0].size();
    *empty = -1;
    DisjointSet ds(rows * cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j]) { // every + gets joined to its neighbors
                // Check right
                if (j < cols - 1 && grid[i][j + 1]) {
                    ds.union_sets(i * cols + j, i * cols + (j + 1));
                }
                // Check down
                if (i < rows - 1 && grid[i + 1][j]) {
                    ds.union_sets(i * cols + j, (i + 1) * cols + j);
                }
                // Check diagonal down-right
                if (i < rows - 1 && j < cols - 1 && grid[i + 1][j + 1]) {
                    ds.union_sets(i * cols + j, (i + 1) * cols + (j + 1));
                }
                // Check diagonal down-left
                if (i < rows - 1 && j > 0 && grid[i + 1][j - 1]) {
                    ds.union_sets(i * cols + j, (i + 1) * cols + (j - 1));
                }
                // Additional: Check up-right (for completeness in diagonal checks)
                if (i > 0 && j < cols - 1 && grid[i - 1][j + 1]) {
                    ds.union_sets(i * cols + j, (i - 1) * cols + (j + 1));
                }
            } else {
                if (*empty == -1) *empty = i * cols + j;
                ds.union_sets(i * cols + j, *empty);
            }
        }
    }
    ds.final_sets();
    return ds;
}

void thresholdPrint(const std::vector<std::vector<bool>>& grid, int threshold) {
    for (const auto& row : grid) {
        for (bool pixel : row) {
            std::cout << (pixel ? '+' : ' ');
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_image_file>" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    std::vector<std::vector<bool>> binaryImage;
    convertBImgToBools(filePath, binaryImage);

    // 1. Print the input image
    std::cout << "Input image: \n";
    for (const auto& row : binaryImage) {
        for (bool pixel : row) {
            std::cout << (pixel ? '+' : ' ');
        }
        std::cout << std::endl;
    }

    int empty = -1;
    DisjointSet components = getcomponents(binaryImage, &empty);
    empty = components.find_set(empty);
    // this code is pretty convoluted but I couldn't think of a better way in the moment

    std::cout << "Number of components: " << components.final_sets() - 1 << std::endl;

    int height = binaryImage.size();
    int width = binaryImage[0].size();

    const char alpha[] = "abcdefghijklmnopqrstuvwxyz";

    std::unordered_map<int, char> componentMap;
    std::unordered_map<int, int> componentSizes;

    // 2. The connected component image where each component is labelled with a unique character
    for (int i = 0; i < height * width; i++) {
        int find = components.find_set(i);
        if (find == empty) {
            std::cout << " ";
        } else {
            // if the component is not in the map, add it
            if (componentMap.find(find) == componentMap.end()) {
                componentMap[find] = alpha[componentMap.size()];
                componentSizes[find] = 1;
            } else {
                componentSizes[find]++;
            }
            std::cout << componentMap[find];
        }
        if ((i + 1) % width == 0) {
            std::cout << std::endl;
        }
    }

    // we do a little sorting
    std::vector<std::pair<int, int>> sortedcomponentSizes(componentSizes.begin(), componentSizes.end());

    // Sort based on the value (second element of the pair)
    std::sort(sortedcomponentSizes.begin(), sortedcomponentSizes.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });

    // 3. a list sorted by component size, each line contains the size and the label of the component
    std::cout << "component sizes: \n";
    for (const auto& [component, size] : sortedcomponentSizes) {
        std::cout << componentMap[component] << " " << componentSizes[component] << std::endl;
    }

    // 4. same as 2, but only connected components whose size is greater than 2 are printed
    std::cout << "Connected components with size greater than 2: \n";
    for (int i = 0; i < height * width; i++) {
        int find = components.find_set(i);
        if (find == empty || componentSizes[find] <= 2) {
            std::cout << " ";
        } else {
            std::cout << componentMap[find];
        }
        if ((i + 1) % width == 0) {
            std::cout << std::endl;
        }
    }

    // 5. same as 2, but only connected components whose size is greater than 11 are printed
    std::cout << "Connected components with size greater than 11: \n";
    for (int i = 0; i < height * width; i++) {
        int find = components.find_set(i);
        if (find == empty || componentSizes[find] <= 11) {
            std::cout << " ";
        } else {
            std::cout << componentMap[find];
        }
        if ((i + 1) % width == 0) {
            std::cout << std::endl;
        }
    }

    return 0;
}
