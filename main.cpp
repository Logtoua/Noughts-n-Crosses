#include <array>
#include <cctype>
#include <iostream>
#include <limits>

const char EmptyCell = ' ';
const char HumanMarker = 'X';
const char ComputerMarker = 'O';

using Board = std::array<char, 9>;

Board createBoard();
bool isMoveValid(const Board& board, int position);
bool makeMove(Board& board, int position, char marker);
char winner(const Board& board);
bool isBoardFull(const Board& board);
bool isGameOver(const Board& board);
int findWinningMove(const Board& board, char marker);
int chooseComputerMove(const Board& board);
char displayCell(const Board& board, int index);
void printBoard(const Board& board);
int readHumanMove(const Board& board);

#ifndef NOUGHTS_AND_CROSSES_TEST
int main()
{
    Board board = createBoard();

    std::cout << "Noughts and Crosses\n";
    std::cout << "You are X. The computer is O.\n";

    while (!isGameOver(board)) {
        printBoard(board);
        makeMove(board, readHumanMove(board), HumanMarker);

        if (isGameOver(board)) {
            break;
        }

        const int computerMove = chooseComputerMove(board);
        makeMove(board, computerMove, ComputerMarker);
        std::cout << "Computer has choseen square " << computerMove << ".\n";
    }

    printBoard(board);

    const char result = winner(board);
    if (result == HumanMarker) {
        std::cout << "You win!\n";
    } else if (result == ComputerMarker) {
        std::cout << "Computer wins!\n";
    } else {
        std::cout << "It's a draw!\n";
    }

    std::cout << "WOuld you like to play again? (yes or no).";
    string choice = "";
    std::cin >> choice;

    return 0;
}
#endif

Board createBoard()
{
    return {EmptyCell, EmptyCell, EmptyCell,
            EmptyCell, EmptyCell, EmptyCell,
            EmptyCell, EmptyCell, EmptyCell};
}

bool isMoveValid(const Board& board, int position)
{
    return position >= 1 && position <= 9 && board[position - 1] == EmptyCell;
}

bool makeMove(Board& board, int position, char marker)
{
    if (!isMoveValid(board, position)) {
        return false;
    }

    board[position - 1] = marker;
    return true;
}

char winner(const Board& board)
{
    const int winningLines[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };

    for (const auto& line : winningLines) {
        const char first = board[line[0]];
        if (first != EmptyCell && first == board[line[1]] && first == board[line[2]]) {
            return first;
        }
    }

    return EmptyCell;
}

bool isBoardFull(const Board& board)
{
    for (char cell : board) {
        if (cell == EmptyCell) {
            return false;
        }
    }

    return true;
}

bool isGameOver(const Board& board)
{
    return winner(board) != EmptyCell || isBoardFull(board);
}

int findWinningMove(const Board& board, char marker)
{
    for (int position = 1; position <= 9; ++position) {
        Board trialBoard = board;
        if (makeMove(trialBoard, position, marker) && winner(trialBoard) == marker) {
            return position;
        }
    }

    return 0;
}

int chooseComputerMove(const Board& board)
{
    const int winningMove = findWinningMove(board, ComputerMarker);
    if (winningMove != 0) {
        return winningMove;
    }

    const int blockingMove = findWinningMove(board, HumanMarker);
    if (blockingMove != 0) {
        return blockingMove;
    }

    if (isMoveValid(board, 5)) {
        return 5;
    }

    for (int position = 1; position <= 9; ++position) {
        if (isMoveValid(board, position)) {
            return position;
        }
    }

    return 0;
}

char displayCell(const Board& board, int index)
{
    return board[index] == EmptyCell ? static_cast<char>('1' + index) : board[index];
}

void printBoard(const Board& board)
{
    std::cout << "\n";
    for (int row = 0; row < 3; ++row) {
        const int start = row * 3;
        std::cout << " " << displayCell(board, start)
                  << " | " << displayCell(board, start + 1)
                  << " | " << displayCell(board, start + 2) << "\n";

        if (row < 2) {
            std::cout << "---+---+---\n";
        }
    }
    std::cout << "\n";
}

int readHumanMove(const Board& board)
{
    int position = 0;

    while (true) {
        std::cout << "\nChoose a square (1-9): ";
        if (std::cin >> position && isMoveValid(board, position)) {
            return position;
        }

        std::cout << "That move is not available. Try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}
