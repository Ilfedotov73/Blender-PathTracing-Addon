#include "interval.h"

namespace render_core {
	const interval interval::empty = interval(+INF, -INF);
	const interval interval::universe = interval(-INF, +INF);
}