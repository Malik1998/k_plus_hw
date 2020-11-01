#include <iostream>
#include <set>
#include <vector>
#include <stack>
#include <map>
#include <fstream>

// Отвечает за скобку, т.е за то, что ограждено внутренней частью
struct sortedBound {
    // минимальный элемент в данной скобке
    char min_element;
    // элементы лежащие непосредственно в скобке
    std::multiset<char> all_set;
    // минимальные элементы из скобок внутри данной
    std::multimap<char, int> sorted_elements; // symbol to sort, index to know position
};

void writeFromSet(std::multiset<char>::iterator& begin,
        std::multiset<char>::iterator& end, std::string& answer, int& string_index,
        bool& isFirst, int element_to_compare = INT_MAX ) {

    while(*begin <= element_to_compare &&
            begin != end ) {
        if(!isFirst) {
            answer[string_index++] = '+';
        }
        answer[string_index++] = *begin;
        begin++;
        isFirst = false;
    }

}

void resolve(int cur_index,  const std::vector<sortedBound>& counter, std::string& answer,
        int& string_index) {
    answer[string_index++] = '(';
    auto cur_element = counter[cur_index].all_set.begin();
    auto last_element = counter[cur_index].all_set.end();
    int i = 0;
    bool isFirst = true; // не нужно выводить "+" вначале
    for(const auto& element_index: counter[cur_index].sorted_elements) {

        writeFromSet(cur_element, last_element, answer, string_index,
                     isFirst, element_index.first );

        if(!isFirst) {
            answer[string_index++] = '+';
        }
        if (element_index.second != cur_index) {
            resolve(element_index.second, counter, answer, string_index);
        }
        isFirst = false;
    }

    writeFromSet(cur_element, last_element, answer, string_index, isFirst );

    answer[string_index++] = ')';
}

std::string sortArray(std::vector<char>& unsorted, std::string& answer) {

    std::stack<int> index_of_not_ended;
    std::vector<sortedBound> counter;
    for(int i = 0; i < unsorted.size(); ++i) {
        if(unsorted[i] == '(') {
            index_of_not_ended.push(counter.size());
            counter.push_back({0, {}});
        } else if(unsorted[i] == ')') {
            int our_index = index_of_not_ended.top();
            counter[our_index].min_element = *counter[our_index].all_set.begin();
            if (!counter[our_index].sorted_elements.empty() ) {
                counter[our_index].min_element = std::min(counter[our_index].min_element,
                counter[our_index].sorted_elements.begin()->first);
            }

            index_of_not_ended.pop();
            if (!index_of_not_ended.empty()) {
                const int parent_index = index_of_not_ended.top();
                counter[parent_index].sorted_elements.insert({ counter[our_index].min_element,
                                                               our_index});
            }
        } else if (unsorted[i] != '+') {
            counter[index_of_not_ended.top()].all_set.insert(unsorted[i]);
        }
    }

    answer = std::string(unsorted.size(), '0');
    int string_index = 0;
    int cur_index = 0;
    resolve(cur_index, counter, answer, string_index);

    return answer;
}

void tokenizer(const std::string& question_string, std::vector<char>& question) {
    for(int i = 0; i < question_string.length(); i++) {
        if (question_string[i] != ' ') {
            question.push_back(question_string[i]);
        }
    }
}

int main(int argc, char** argv) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    {
        std::ifstream generated_file(argv[1]);
        std::ofstream generated_answer(std::string(argv[1]) + "_sorted");
        std::string cur_line;
        while (std::getline(generated_file, cur_line)) {
            std::vector<char> question;
            tokenizer(cur_line, question);
            std::string sorted_answer;
            sortArray(question, sorted_answer);
            generated_answer << sorted_answer << std::endl;
        }
        generated_file.close();
        generated_answer.close();
    }

    return 0;
}