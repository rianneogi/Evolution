#pragma once
#include "Tests.h"

#include <future>

void learn(int MAX_GEN);
void learn_sup();
void train_parallel(int num_threads, int mixing_step, int pop_per_thread, int survivors_per_thread);
