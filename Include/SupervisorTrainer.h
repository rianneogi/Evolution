#include "GenericTrainer.h"

class SupervisorTrainer
{
public:
	int mNumPopulation;
	int mNumSupervisors;
	int mNumSurvivors;
	int mNumSupervisorSurvivors;
	int mNumSubGenerations;

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
	GenericTrainer* mTrainers;
	SupervisedEnvironment* mEnvs;

	std::string mLoadPath;
	std::string mSavePath;

	SupervisorTrainer() {}
	~SupervisorTrainer() {}

	void init();
	void train();
	int testSupervisor(int num_gen, int sup_id, int sup_children);
};