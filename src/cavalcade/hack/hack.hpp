#ifndef HACK_HPP
#define HACK_HPP

#include "other/prediction.hpp"

namespace hack {
	struct impl {
		other::prediction m_prediction;
	};
} // namespace hack

inline hack::impl g_hack;

#endif // HACK_HPP