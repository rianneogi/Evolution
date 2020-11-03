#include "NetTraining.h"

void setupNet(Board* nn)
{
	// Board nn;

	int BatchSize = 32;
	int num_actions = 4;
	float learning_rate = 0.005;

	Initializer* initializer = new RangeInitializer();

	Blob *inputBlob = nn->newBlob(make_shape(BatchSize, 4,110,80), "InputBlob");
	Blob *conv1Blob = nn->newBlob(make_shape(BatchSize, 4,110,80));
	Blob *act1Blob = nn->newBlob(make_shape(BatchSize, 4,110, 80));
	Blob *conv2Blob = nn->newBlob(make_shape(BatchSize, 4,110,80));
	Blob *act2Blob = nn->newBlob(make_shape(BatchSize, 4*110*80));
	Blob *fc3Blob = nn->newBlob(make_shape(BatchSize, 256));
	Blob *act3Blob = nn->newBlob(make_shape(BatchSize, 256));
	Blob *outputBlob = nn->newBlob(make_shape(BatchSize, num_actions), "OutputBlob");

	nn->setOptimizer(new AdamOptimizer(learning_rate));

	nn->addNeuron(new ConvNeuron(inputBlob,conv1Blob,8,8,0,0,4,4));
	nn->addNeuron(new LeakyReLUNeuron(conv1Blob,act1Blob));
	nn->addNeuron(new ConvNeuron(act1Blob, conv2Blob, 4, 4, 0, 0, 2, 2));
	nn->addNeuron(new LeakyReLUNeuron(conv2Blob, act2Blob));
	nn->addNeuron(new FullyConnectedNeuron(act2Blob, fc3Blob, initializer));
	nn->addNeuron(new LeakyReLUNeuron(fc3Blob, act3Blob));
	nn->addNeuron(new FullyConnectedNeuron(act3Blob, outputBlob, initializer));

	nn->addErrorFunction(new MeanSquaredError(outputBlob));

	// nn->addPlaceholder(&inputBlob->Data);
	// nn->addPlaceholder(&nn->mErrorFuncs[0]->mTarget);

	nn->setUp();
}

int transform_reward(int reward)
{
	if(reward > 0)
	{
		return 1;
	}
	else if(reward < 0)
	{
		return -1;
	}
	return reward;
}

// void addExperienceToBuffer(byte_t* buffer, byte_t* exp, int pos)
// {
// 	memcpy();
// }

// void grayscale(byte_t* input, byte_t* dest, int size)
// {
// 	auto ptr = dest;
// 	auto ptr2 = input;
// 	for (int i = 0; i < size; i++)
// 	{
// 		*ptr2 = *ptr + *(ptr + 1) + *(ptr + 2);
// 		ptr++;
// 		ptr2 += 3;
// 	}
// }

void downscale(std::vector<pixel_t>& src, byte_t* dest) //downsample by taking every alternate pixel
{
	byte_t *ptr = dest;
	for (int i = 0; i < src.size(); i += 2)
	{
		*ptr = src[i];
	}
}

void random_sample(byte_t* buffer, int buffer_size, Tensor frames, Tensor q_vals)
{
	int batch_size = frames.mShape[0];
	assert(batch_size == q_vals.mShape[0]);

	for (int i = 0; i < batch_size;i++)
	{
		int pos = rand() % buffer_size;
		//copy 4 consequtive frames to i-th batch
		// memcpy(&frames(i,0,0,0), &buffer[pos*110*80], 110*80*4);
		for (int j = 0; j < 110 * 80 * 4;j++)
		{
			frames(i * 110 * 80 * 4 + j) = buffer[pos * 110 * 80 + j] / 255.0f;
		}
	}
	frames.copyToGPU();
	q_vals.copyToGPU();
}

void train_dqn()
{
	int exp_buffer_size = 100000;
	int random_exploration_size = 1000;
	float epsilon = 1.0f;
	float gamma = 0.99f;
	int BatchSize = 32;
	int train_interval = 4;

	Board *player = new Board();
	Board *target = new Board();
	setupNet(player);
	setupNet(target);

	AtariGame game("ALE/roms/breakout.bin", 123, true);
	game.mALE->setBool("color_averaging", true);
	ActionVect act_vect = game.mALE->getMinimalActionSet();
	int size = act_vect.size();

	bool running = true;

	byte_t* expBufferFrames = new byte_t[exp_buffer_size*110*80];
	int* expBufferRewards = new int[exp_buffer_size];

	// byte_t* last4frames = new byte_t[4 * 110 * 80];
	Tensor last4framesTensor(make_shape(4, 110, 80));

	std::vector<pixel_t> lastFrame;
	byte_t *lastFrameDownscaled;

	int curr_buffer_pos = 0;
	bool is_buffer_full = false;
	int frame = 0;

	for (int gen = 0; running; gen++)
	{
		game.restart();
		int totalReward = 0;
		int last_reward = 0;
		while (!game.is_over() || game.mFrames >= 1000)
		{
			//Parse last frame and add it to experience buffer
			game.mALE->getScreenGrayscale(lastFrame);
			downscale(lastFrame, lastFrameDownscaled);
			memcpy(&expBufferFrames[110 * 80 * curr_buffer_pos], lastFrameDownscaled, 110 * 80 * sizeof(byte_t));
			expBufferRewards[curr_buffer_pos] = 

			curr_buffer_pos++;
			if(curr_buffer_pos>=exp_buffer_size)
			{
				is_buffer_full = true;
				curr_buffer_pos = 0;
			}

			int action = -1;
			if (rand_init(0, 1) < epsilon || frame<=random_exploration_size)
			{
				action = rand() % size;
			}
			else
			{
				//Copy last 4 frames to the input tensor
				int offset = std::max(0,curr_buffer_pos - 4) * 110 * 80;
				for (int i = 0; i < 4 * 80 * 110; i++)
				{
					last4framesTensor(i) = expBufferFrames[offset + i]/255.0f;
				}

				Tensor output = player->forward(last4framesTensor);

				int max_id = 0;
				float max = 0.0f;
				for (int i = 0; i < size; i++) //argmax
				{
					if(max < output(i))
					{
						max = output(i);
						max_id = i;
					}
				}
				action = max_id;
			}
			assert(action >= 0 && action < size);
			last_reward = game.do_action(action);
			totalReward += last_reward;

			//Training
			if(frame!=0 && frame%train_interval==0 && is_buffer_full)
			{
				random_sample(expBufferFrames, curr_buffer_pos, player->getBlob("InputBlob")->Data, target->mErrorFuncs[0]->mTarget);
				// player->train(ip, op, 1, BatchSize);
				std::vector<Tensor> empty_vec;
				player->backprop(empty_vec);
			}

			frame++;
		}
	}
}