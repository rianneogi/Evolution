#include "Tests.h"

class SupervisorTrainer
{
public:

	int mNumPopulation;
	int mNumSupervisors;
	int mNumSurvivors;

	Genotype* mPopulation;
	Genotype* mSupervisors;
	int mGeneration;
	int* mLastBest;
	int* mScores;
	std::vector<int> mBestID;
	Execution mExe;
	AtariGame* mGame;

	std::string mLoadPath;
	std::string mSavePath;

	SupervisorTrainer() {}
	~SupervisorTrainer() {}

	void init();
	void train();
	void testSupervisor(int num_gen, int sup_id, int sup_children);
};