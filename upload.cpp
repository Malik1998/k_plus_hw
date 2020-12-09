#include <algorithm>
#include <functional>
#include <iostream>
#include <utility>
#include <vector>

struct TeamPlayer {
    int index = 0;
    int64_t efficiency = 0;

    TeamPlayer(int ind = 0, int64_t eff = 0) : index(ind), efficiency(eff) {
    }
};

template <class Iterator, class Comparator>
class Heap {
public:
    typedef typename std::iterator_traits<Iterator>::value_type Type;
    Heap(Iterator& begin, Iterator& end, Comparator&& cmp)
        : begin_(begin) {
        size_ = end - begin;
        cmp_ = std::move(cmp);
        Heapify();
    }

    Type ExtractTop() {
        Type answer = *begin_;
        std::swap(*(begin_ + size_ - 1), *begin_);
        --size_;
        ShiftDown(0);
        return answer;
    }

    void TopToBack() {
        std::swap(*(begin_ + size_ - 1), *begin_);
        --size_;
        ShiftDown(0);
    }

private:
    void Heapify() {
        for (int64_t index = 0; index < size_; ++index) {
            ShiftUp(index);
        }
    }

    void ShiftDown(int64_t index) {
        auto left = 2 * index + 1;
        auto right = 2 * index + 2;

        while  (left < size_) {
            if (right >= size_) {
                if (!cmp_(*(begin_ + index), *(begin_ + left))) {
                    std::swap(*(begin_ + index), *(begin_ + left));
                }
                break;
            } else if (!cmp_(*(begin_ + index), *(begin_ + left)) ||
                       !cmp_(*(begin_ + index), *(begin_ + right))) {
                if (cmp_(*(begin_ + left), *(begin_ + right))) {
                    std::swap(*(begin_ + index), *(begin_ + left));
                    index = left;
                } else {
                    std::swap(*(begin_ + index), *(begin_ + right));
                    index = right;
                }
            } else {
                break;
            }
            left = 2 * index + 1;
            right = 2 * index + 2;
        }
    }

    void ShiftUp(int64_t index) {
        while (index != 0 && !cmp_(*(begin_ + (index - 1) / 2), *(begin_ + index))) {
            std::swap(*(begin_ + (index - 1) / 2), *(begin_ + index));
            index = (index - 1) / 2;
        }
    }

    int64_t size_;
    Iterator begin_;
    Comparator cmp_;
};

std::vector<TeamPlayer> ReadArray(std::istream& input = std::cin) {
    int64_t count;
    input >> count;

    std::vector<TeamPlayer> vector;
    for (int index = 1; index <= count; ++index) {
        int effic;
        input >> effic;
        vector.emplace_back(index, effic);
    }
    return vector;
}

struct CompareByEfficiency {
    bool operator()(const TeamPlayer &lhs, const TeamPlayer &rhs) const {
        return lhs.efficiency > rhs.efficiency;
    }
};

template <class Iterator, class Comp = std::less<Iterator>>
void Sort(Iterator begin, Iterator end, Comp cmp = Comp()) {
    Heap<Iterator, Comp> heap
            (begin, end, std::move(cmp));
    int index = 0;
    while (begin + index != end) {
        heap.TopToBack();
        ++index;
    }

    std::reverse(begin, end);
}

struct TeamEfficiency {
    std::vector<TeamPlayer>::const_iterator left;
    std::vector<TeamPlayer>::const_iterator right;
    int64_t efficiency = 0;
};

std::vector<TeamPlayer> GetMostEfficientSolidaryTeam(std::vector<TeamPlayer> sorted) {
    Sort(sorted.begin(), sorted.end(), CompareByEfficiency());
    TeamEfficiency current_efficiency = {sorted.begin(), sorted.begin(), sorted[0].efficiency};

    TeamEfficiency best_efficiency = current_efficiency;

    for (; current_efficiency.right + 1 < sorted.end(); ++current_efficiency.left) {
        while (current_efficiency.right + 1 < sorted.end() &&
               (*current_efficiency.right).efficiency +
               (*(current_efficiency.right + 1)).efficiency >=
               (*current_efficiency.left).efficiency) {
            current_efficiency.efficiency += (*(current_efficiency.right + 1)).efficiency;
            ++current_efficiency.right;
        }
        if (current_efficiency.efficiency > best_efficiency.efficiency) {
            best_efficiency = current_efficiency;
        }
        current_efficiency.efficiency -= (*best_efficiency.left).efficiency;
    }

    return { best_efficiency.left, best_efficiency.right + 1 };
}

int64_t GetSum(const std::vector<TeamPlayer>& team) {
    int64_t count = 0;
    for (const auto& player: team) {
        count += player.efficiency;
    }
    return count;
}

struct CompareByIndex {
public:
    bool operator()(const TeamPlayer &lhs, const TeamPlayer &rhs) const {
        return lhs.index < rhs.index;
    }
};

void PrintSumAndTeam(std::vector<TeamPlayer>& team, std::ostream& output = std::cout) {
    Sort(team.begin(), team.end(), CompareByIndex());
    output << GetSum(team) << "\n";
    for (auto& player: team) {
        output << player.index << " ";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    auto team_data = ReadArray();
    auto efficient_team = GetMostEfficientSolidaryTeam(team_data);
    PrintSumAndTeam(efficient_team);
}
