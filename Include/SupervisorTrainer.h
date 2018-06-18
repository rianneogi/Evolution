#include "Tests.h"

class SupervisorTrainer
{
public:
	int mNumPopulation;
	int mNumSupervisors;
	int mNumSurvivors;
	int mNumSupervisorSurvivors;

	Genotype* mPopulation;
	Genotype* mSupervisors;
	int mGeneration;
	int* mLastBest;
	int *mSupervisorLastBest;
	int *mScores;
	int *mSupervisorScores;
	std::vector<int> mBestID;
	std::vector<int> mSupervisorBestID;
	Execution mExe;
	AtariGame* mGame;

	std::string mLoadPath;
	std::string mSavePath;

	SupervisorTrainer() {}
	~SupervisorTrainer() {}

	void init();
	void train();
	int testSupervisor(int num_gen, int sup_id, int sup_children);
};