#include "repository.h"

//Initializing static members
std::function<SqlProvider*()> Repository::sm_createProvider = [](){ return nullptr; };
