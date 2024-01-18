#include <iostream>
#include <unordered_set>
#include <vector>
#include <array>
#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <cmath>


static const size_t LINE_SIZE = 4;
static const size_t FIELD_SIZE = LINE_SIZE * LINE_SIZE;

using FieldArray = std::array<uint8_t, FIELD_SIZE>;

const FieldArray EMPTY_STATE = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
};

const FieldArray GOAL_STATE = {
    1, 2, 3, 4,
    5, 6, 7, 8,
    9, 10, 11, 12,
    13, 14, 15, 0
};

class FieldState {
public:
	FieldState(const FieldState& st)
        : state(st.state), zero_pos(st.zero_pos) {} 

    FieldState(const FieldArray& arr) {
        for (int i = 0; i < arr.size(); i++) {
            if (arr[i] == 0) {
                zero_pos = i;
            }
            state[i] = arr[i];
        }
    }

    ~FieldState() {}

    bool operator==(const FieldState& r) const {
        return state == r.state;
    }

    bool operator<(const FieldState& r) const {
        return state < r.state;
    }
	
    std::vector<std::pair<FieldState, char>> GetNextVertices() const {
        std::vector<std::pair<FieldState, char>> result;
        if (zero_pos >= LINE_SIZE) { // up
            FieldState st(*this);
            st.moveUp();
            result.push_back({st, 'D'});
        }
        if (zero_pos < FIELD_SIZE - LINE_SIZE) { // down
            FieldState st(*this);
            st.moveDown();
            result.push_back({st, 'U'});
        }
        if (zero_pos % LINE_SIZE != 0) { // left
            FieldState st(*this);
            st.moveLeft();
            result.push_back({st, 'R'});
        }
        if ((zero_pos + 1) % LINE_SIZE != 0) { // right
            FieldState st(*this);
            st.moveRight();
            result.push_back({st, 'L'});
        }
        return result;
        
    }

    void print() {
        for (int i = 0; i < state.size(); ++i) {
            std::cout << (int)state[i] << " ";
            if ((i + 1) % LINE_SIZE == 0) {
                std::cout << std::endl;
            }     
        }
        std::cout << "===============" << std::endl;
    }



    bool checkParity() {
        int k = (zero_pos / LINE_SIZE) + 1;
        //std::cout << k << " " << cntInversion() << std::endl;
        return (k + cntInversion()) % 2 == 0;
    }

    int getEvristic() {
        int sum = 0;
        for (int i = 0; i < state.size(); ++i) {
            if (i == zero_pos) { 
                sum += getManhetonLen(i, state.size() - 1);
            } else {
                sum += getManhetonLen(i, state[i]-1);
            }
        }
        return sum;
    }

 private:
    int getManhetonLen(int pos, int real_pos) {
        // x = 9 % 4 == 1 '' y = 9 div 4 == 2
        int x_pos = pos % LINE_SIZE;
        int y_pos = pos / LINE_SIZE;
        int x_real_pos = real_pos % LINE_SIZE;
        int y_real_pos = real_pos / LINE_SIZE;
        return std::abs(x_pos-x_real_pos) + std::abs(y_pos - y_real_pos);
    }

    void moveUp() {
        int new_zero_pos = zero_pos - LINE_SIZE;
        std::swap(state[new_zero_pos], state[zero_pos]);
        zero_pos = new_zero_pos;
    }
    void moveDown() {
        int new_zero_pos = zero_pos + LINE_SIZE;
        std::swap(state[new_zero_pos], state[zero_pos]);
        zero_pos = new_zero_pos;
    }
    void moveLeft() {
        int new_zero_pos = zero_pos - 1;
        std::swap(state[new_zero_pos], state[zero_pos]);
        zero_pos = new_zero_pos;
    }
    void moveRight() {
        int new_zero_pos = zero_pos + 1;
        std::swap(state[new_zero_pos], state[zero_pos]);
        zero_pos = new_zero_pos;
    }

    int cntInversion() { 
        int count = 0;
        for(int i = 0; i < state.size()-1; ++i) {
            if (i == zero_pos) {
                continue;
            }
            for (int j = i + 1; j < state.size(); ++j) {
                if (j == zero_pos) {
                    continue;
                }
                if (state[i] > state[j]) {
                    count++;
                }
            }
        }
        return count;
    }

    FieldArray state;
    int zero_pos;
};



bool findPath(FieldState start, std::vector<char>& path, int& len_path) {

    if (!start.checkParity()) {
        return false;
    }     
    FieldState empty(EMPTY_STATE);
    FieldState goal(GOAL_STATE);

    std::map< std::pair<FieldState, char>, std::pair<FieldState, char> > visited;
    visited.insert({{start, 'D'}, {EMPTY_STATE, 'D'}});
    std::set< std::pair<int, std::pair<FieldState, char> > > heap;

    heap.insert({0, {start, 'D'}});
    std::pair<FieldState, char> curr_vertex({EMPTY_STATE, 'D'});
    while (!heap.empty()) {
        curr_vertex = heap.begin()->second;
        heap.erase(heap.begin());

        if (curr_vertex.first == goal) {
            break;
        }

        for (auto& child : curr_vertex.first.GetNextVertices()) {
            if (visited.find(child) == visited.end()) {
                heap.insert({child.first.getEvristic(), child});
                visited.insert({child, curr_vertex});
            }
        }
    }
    while (!(curr_vertex.first == empty)) {
        path.push_back(curr_vertex.second);
        curr_vertex = visited.find(curr_vertex)->second;
        ++len_path;
    }
    return true;
}

void printSolution(const std::vector<char>& solution) {
    for (int i = solution.size() - 2; i > -1; --i) {
        std::cout << solution[i] << " ";
    }
    std::cout << std::endl;
}

void solution() {
    FieldArray array(EMPTY_STATE);
    int digit = 0;
    for (int i = 0; i < FIELD_SIZE; ++i) {
        std::cin >> digit;
        array[i] = digit;
    }
    FieldState state(array);
    std::vector<char> path;
    int len_path = 0;
    if (findPath(state, path, len_path)) {
        std::cout << len_path - 1 << std::endl;
        printSolution(path);
    } else {
        std::cout << -1 << std::endl;
    }
}

int main() {
    solution();
    return 0;

}