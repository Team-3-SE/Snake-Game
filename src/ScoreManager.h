#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

class ScoreManager
{
private:
    int score;

public:
    ScoreManager();

    void reset();
    void addFoodScore();

    int getScore() const;
};

#endif