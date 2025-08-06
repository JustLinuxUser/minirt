#pragma once

#include "rt_parser.h"

enum RT_NODE_TYPE effective_type(enum RT_NODE_TYPE target,
                                 enum RT_NODE_TYPE got);
bool rt_type_cmp(enum RT_NODE_TYPE target, enum RT_NODE_TYPE got);
char* type_to_str(enum RT_NODE_TYPE t);
