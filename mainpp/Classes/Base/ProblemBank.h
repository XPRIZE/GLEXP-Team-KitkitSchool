/*
 * ProblemBank.h
 *
 *  Created on: Jan 16, 2015
 *      Author: jooddang
 */

#ifndef PROBLEMBANK_H_
#define PROBLEMBANK_H_


#include "3rdParty/json/json.h"

class ProblemBank {
public:
	virtual ~ProblemBank() {}

	//static ProblemBank& ProblemFactory(TodoGameScene::GameType type);
	virtual Json::Value generateParameters(int level) = 0;
};


#endif /* PROBLEMBANK_H_ */
