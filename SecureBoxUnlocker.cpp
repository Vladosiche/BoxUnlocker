#include <iostream>
#include <vector>
#include <random>
#include <time.h>
/*
You are given a locked container represented as a two-dimensional grid of boolean values (true = locked, false = unlocked).
Your task is to write an algorithm that fully unlocks the box, i.e.,
transforms the entire matrix into all false.

Implement the function:
bool openBox(uint32_t y, uint32_t x);
This function should:
    - Use the SecureBox public API (toggle, isLocked, getState).
    - Strategically toggle cells to reach a state where all elements are false.
    - Return true if the box remains locked, false if successfully unlocked.
You are not allowed to path or modify the SecureBox class.

Evaluation Criteria:
    - Functional correctness
    - Computational efficiency
    - Code quality, structure, and comments
    - Algorithmic insight and clarity
*/

class SecureBox
{
private:
    std::vector<std::vector<bool>> box;

public:

    //================================================================================
    // Constructor: SecureBox
    // Description: Initializes the secure box with a given size and 
    //              shuffles its state using a pseudo-random number generator 
    //              seeded with current time.
    //================================================================================
    SecureBox(uint32_t y, uint32_t x) : ySize(y), xSize(x)
    {
        rng.seed(time(0));
        box.resize(y);
        for (auto& it : box)
            it.resize(x);
        shuffle();
    }

    //================================================================================
    // Method: toggle
    // Description: Toggles the state at position (x, y) and also all cells in the
    //              same row above and the same column to the left of it.
    //================================================================================
    void toggle(uint32_t y, uint32_t x)
    {
        box[y][x] = !box[y][x];
        for (uint32_t i = 0; i < xSize; i++)
            box[y][i] = !box[y][i];
        for (uint32_t i = 0; i < ySize; i++)
            box[i][x] = !box[i][x];
    }

    //================================================================================
    // Method: isLocked
    // Description: Returns true if any cell 
    //              in the box is true (locked); false otherwise.
    //================================================================================
    bool isLocked()
    {
        for (uint32_t x = 0; x < xSize; x++)
            for (uint32_t y = 0; y < ySize; y++)
                if (box[y][x])
                    return true;

        return false;
    }

    //================================================================================
    // Method: getState
    // Description: Returns a copy of the current state of the box.
    //================================================================================
    std::vector<std::vector<bool>> getState()
    {
        return box;
    }

private:
    std::mt19937_64 rng;
    uint32_t ySize, xSize;

    //================================================================================
    // Method: shuffle
    // Description: Randomly toggles cells in the box to 
    // create an initial locked state.
    //================================================================================
    void shuffle()
    {
        for (uint32_t t = rng() % 1000; t > 0; t--)
            toggle(rng() % ySize, rng() % xSize);
    }
};

//================================================================================
// Function: isAllEqualOne
// Description: Checks whether the box has or potentially can have the 'true' in all cells. 
//================================================================================
bool isAllEqualOne(const std::vector<std::vector<bool>>& state) 
{
    if (state.size() % 2 == state[0].size() % 2)  // If the box has both even or odd sides, 
    {
        return false;                             // it makes impossible to get a box fill of 'true'
    }
    else
    {
        for (auto row : state)
        {
            for (auto colum : row)
            {
                if (!colum) 
                {
                    return false;
                }
            }
        }
        return true;
    }

}

//================================================================================
// Function: toggleBoxByMask
// Description: receives the state of the box at the beginning and performs
//              a toggle operation on the 'true' in the received state. 
//================================================================================
auto toggleBoxByMask(std::vector<std::vector<bool>> state , SecureBox& box)
{
    for (decltype(state.size()) row = 0; row < state.size(); ++row)
    {
        for (decltype(state[row].size()) colum = 0; colum < state[row].size(); ++colum)
        {
            if (state[row][colum])
            {
                box.toggle(row, colum);
            }
        }
    }
    return box.getState();
}

//================================================================================
// Function: toggleByRowOrColum
// Description: executes the toggle method on a row or column
//              in a dependency or a paired row      
//================================================================================
void toggleByRowOrColum(uint32_t y , SecureBox& box , std::vector<std::vector<bool>> state)
{
    if (y % 2)                      // Checking string size for even or odd
    {
        for (decltype(state.size()) row = 0; row < state.size(); ++row)
        {
            box.toggle(row, 0);     // performing a toggle for all row values
        }
    }
    else
    {
        for (decltype(state.size()) colum = 0; colum < state.size(); ++colum)
        {
            box.toggle(0, colum);   // performing a toggle for all colum values
        }

    }

}
//================================================================================
// Function: openBox
// Description: Your task is to implement this function to unlock the SecureBox.
//              Use only the public methods of SecureBox (toggle, getState, isLocked).
//              You must determine the correct sequence of toggle operations to make
//              all values in the box 'false'. The function should return false if
//              the box is successfully unlocked, or true if any cell remains locked.
//================================================================================
bool openBox(uint32_t y, uint32_t x)
{
        SecureBox box(y, x);

          while (box.isLocked())
          {
              if (isAllEqualOne(toggleBoxByMask(box.getState(), box)))
              {
                  toggleByRowOrColum(y, box, box.getState());
              }
          }

    return box.isLocked();
}


int main(int argc, char* argv[])
{
    uint32_t y = std::atol(argv[1]); 
    uint32_t x = std::atol(argv[2]); 
    bool state = openBox(y, x);

    if (state)
        std::cout << "BOX: LOCKED!" << std::endl;
    else
        std::cout << "BOX: OPENED!" << std::endl;

    return state;
}

