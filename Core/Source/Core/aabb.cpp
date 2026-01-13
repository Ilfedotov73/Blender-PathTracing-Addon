#include "aabb.h"

namespace render_core {
	const aabb aabb::empty = aabb(interval::empty, interval::empty, interval::empty);
	const aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);
}