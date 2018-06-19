#include "Execution.h"
class Game 
{
public:
	Game() {};
	virtual ~Game() {};

	virtual bool is_over() = 0;
	virtual int do_action(int action) = 0;
	virtual byte_t* getState() = 0;
	virtual int getStateSize() = 0;
	virtual void restart() = 0;
};