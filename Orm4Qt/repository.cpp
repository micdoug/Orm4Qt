#include "repository.h"

//Initializing static members
std::function<Orm4Qt::SqlProvider*()> Orm4Qt::Repository::sm_createProvider = [](){ return nullptr; };
