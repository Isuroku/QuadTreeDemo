#include "stdafx.h"
#include "PassInTree.h"

const SChildOrder SPassInTree::OctToChild[3][3]
{
	{ SChildOrder(1, 0, 5, 5, 5), SChildOrder(2, 0, 2, 5, 5), SChildOrder(1, 2, 5, 5, 5) },
	{ SChildOrder(2, 0, 1, 5, 5), SChildOrder(4, 0, 1, 2, 3), SChildOrder(2, 2, 3, 5, 5) },
	{ SChildOrder(1, 1, 5, 5, 5), SChildOrder(2, 1, 3, 5, 5), SChildOrder(1, 3, 5, 5, 5) },
};