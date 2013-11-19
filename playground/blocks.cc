#include <iostream>
#include <numeric>
#include <fstream>
using namespace std;

const unsigned tc_number = 10;
const float strength_factor = 1.9;

struct Block
{
    Block() : count(0), strength(1.0), head(NULL), tail(NULL) {}

    template<typename AdvanceFunc>
    void advance(AdvanceFunc advance_func)
    {
        strength *= strength_factor;
        head = advance_func(head);
        *head = true;
        *tail = false;
        tail = advance_func(tail);
    }

    void swallow(Block other)
    {
        count += other.count;
        strength += other.count;
        head = other.head;
    }

    unsigned count;
    float strength;
    bool* head;
    bool* tail;
};

template<typename AdvanceFunc>
Block get_block(bool* begin, bool* end, AdvanceFunc advance_func)
{
    Block result;

    while (*begin)
    {
        ++result.count;
        if (result.tail == NULL) result.tail = begin;
        if (begin == end) break;
        begin = advance_func(begin);
    }

    if (result.tail)
        result.head = advance_func(result.tail, result.count - 1);

    return result;
}

struct advance_by_row
{
    explicit advance_by_row(size_t size) : size(size) { }
    bool* operator()(bool* ptr) { return ptr + size; }
    bool* operator()(bool* ptr, size_t count) { return ptr + (count * size); }

    size_t size;
};

struct reverse_advance_by_row
{
    explicit reverse_advance_by_row(size_t size) : size(size) { }
    bool* operator()(bool* ptr) { return ptr - size; }
    bool* operator()(bool* ptr, size_t count) { return ptr - (count * size); }
    
    size_t size;
};

struct advance_by_col
{
    explicit advance_by_col() : size(1) { }
    bool* operator()(bool* ptr) { return ptr + size; }
    bool* operator()(bool* ptr, size_t count) { return ptr + (count * size); }

    size_t size;
};

struct reverse_advance_by_col
{
    explicit reverse_advance_by_col() : size(1) { }
    bool* operator()(bool* ptr) { return ptr - size; }
    bool* operator()(bool* ptr, size_t count) { return ptr - (count * size); }
    
    size_t size;
};

template<typename AdvanceFunc>
void fall(bool* begin, bool* end, AdvanceFunc advance_func)
{
    Block block = get_block(begin, end, advance_func);
    if (block.count == 0)
        return;

    bool* current = begin;
    bool* next = NULL;
    do {
        next = advance_func(current);
        if (*next)
        {
            Block enemy = get_block(next, end, advance_func);
            if (enemy.count >= block.strength)
            {
                break;
            }
            else
            {
                block.swallow(enemy);
            }
        }
        else
        {
            block.advance(advance_func);
        }

        current = next;

        if (current == end) break;
    } while (true);
}

bool array[501][501];

fstream f("sample_input3.txt");

int main()
{
    for (unsigned tc = 0; tc < tc_number; ++tc)
    {
        size_t size;
        f >> size;
        for(size_t i = 0; i < size; ++i)
            for(size_t j = 0; j < size; ++j)
                f >> array[i][j];

        // top-down
        for (size_t i = 0; i < size; ++i)
        {
            fall(&array[0][i], &array[size-1][i], advance_by_row(501));
        }

        // left-right
        for (size_t i = 0; i < size; ++i)
        {
            fall(&array[i][0], &array[i][size-1], advance_by_col());
        }
    
        // down-top
        for (size_t i = 0; i < size; ++i)
        {
            fall(&array[size-1][i], &array[0][i], reverse_advance_by_row(501));
        }
        
        // right-left
        for (size_t i = 0; i < size; ++i)
        {
            fall(&array[i][size-1], &array[i][0], reverse_advance_by_col());
        }

        cout << "#" << tc + 1 << " ";
        unsigned sum = 0;
        for (size_t i = 0; i < size; ++i)
        {
            sum = 0;
            for (size_t j = 0; j < size; ++j)
                sum += array[j][i];
            cout << sum << " ";
        }
        cout << endl;
     }
}
