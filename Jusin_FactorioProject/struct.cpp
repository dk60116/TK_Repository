#include "struct.h"

const vector2Int vector2::toVector2Int() const
{
	return vector2Int((int)round(x), (int)round(y));
}