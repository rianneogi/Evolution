#pragma once

#include "Game.h"

class TicTacToe
{
public:
    int mTurn;
    int* mState;
    int mNumMoves;
    
    TicTacToe()
    {
        mTurn = 0;
        mState = new int[9];
        for(int i = 0;i<9;i++)
        {
            mState[i] = 2;
        }
        mNumMoves = 0;
    }
    
    ~TicTacToe()
    {
        delete[] mState;
    }
    
    bool is_legal(int pos) const
    {
        if(mState[pos]==2)
            return true;
        return false;
    }
    
    void make_move(int pos)
    {
        mState[pos] = mTurn;
        mTurn = (mTurn+1)%2;
        mNumMoves++;
    }
    
    int is_over() const;
    
    void restart();
    
    void print() const
    {
        for(int i = 0;i<9;i++)
        {
            printf("%d ", mState[i]);
            if((i+1)%3==0)
            {
                printf("\n");
            }
        }
    }
};

int play_tictactoe(Genotype* indi, Execution& exe, TicTacToe& game);
int duel_tictactoe(Genotype* p1, Genotype* p2, Execution& exe, TicTacToe& game);
void play_tictactoe_human(Genotype* indi, Execution& exe);
