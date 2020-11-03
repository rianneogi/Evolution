#include "SupervisedEnvironment.h"

class GenericTrainer 
{
public:
  	Genotype* mPopulation;
	int* mScores;
	int* mLastBest;
	std::vector<int> mBestID;
	int* mELO;

	int mNumPopulation;
	int mNumSurvivors;

	std::string mSavePath;
	std::string mLoadPath;
	int mPrintDelay;

	// AtariGame* mGame;
	// Execution mExe;
	Environment* mEnv;
	TicTacToe mTicTacToe;

	int mThreadID;

	GenericTrainer() {}
	~GenericTrainer();
	void init(int pop, int survivors, const std::string &load_path, const std::string &save_path, int print_delay);
	void train(int num_gen);
	void init_and_train(int num_gen, int pop, int survivors, const std::string &load_path, const std::string &save_path, int print_delay);
	void train_competitive(int num_gen);
	void cleanup();
};