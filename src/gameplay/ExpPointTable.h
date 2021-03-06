// Copyright (c) 2018-2021 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef VIGILANTE_EXP_POINT_TABLE_H_
#define VIGILANTE_EXP_POINT_TABLE_H_

#include <string>

namespace vigilante {

namespace exp_point_table {

void import(const std::string& tableFileName);
int getNextLevelExp(int currentLevel);

const int kLevelCap = 100;

}  // namespace exp_point_table

}  // namespace vigilante

#endif  // VIGILANTE_EXP_POINT_TABLE_H_
