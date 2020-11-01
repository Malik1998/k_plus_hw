#include <iostream>
#include <fstream>
#include <stdlib.h>

const int max_spaces_count = 2;
const int max_depth = 3;
const int max_element_count = 4;

std::string generate_string(int rest_depth, int element_count) {
    std::string generated = "(";

    for(int i = 0; i < element_count; i++) {
        const bool add_spaces = rand() >= RAND_MAX / 2; // с вероятностью 1/2
        if (add_spaces) {
            const int count = rand() % max_spaces_count + 1;
            for(int j = 0; j < count; j++) {
                generated += " ";
            }
        }

        const bool add_symbol = rand() >= RAND_MAX / 2; // с вероятностью 1/2
        if (i != 0) {
            generated += "+";
        }
        if (add_symbol || rest_depth == 0) {
            const char symbol = static_cast<char>(rand() % ('Z' - 'A') + 'A');
            generated += symbol;
        } else {
            generated += generate_string(rest_depth - 1, max_element_count);
        }
    }


    return generated + ")";
}

void generateAndWriteToFile(int count, const char *filename) {
    const int seed = 1234;
    srand(seed);

    std::ofstream generated_file(filename);
    for(int i = 0; i < count; i++) {
        const int cur_max_depth = rand() % max_depth + 1;
        const int cur_max_element_count = rand() % max_element_count + 1;
        generated_file << generate_string(cur_max_depth, cur_max_element_count) << std::endl;
    }
    generated_file.close();

}
int main(int argc, char** argv) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr );
    int count_of_elements_to_create = atoi(argv[1]);
    const char* filename = argv[2];

    generateAndWriteToFile(count_of_elements_to_create, filename);

}


