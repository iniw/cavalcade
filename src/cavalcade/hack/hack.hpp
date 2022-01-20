#pragma once

#include "other/prediction.hpp"
#include "translator/translator.hpp"
#include "trainer/trainer.hpp"
#include "aimbot/aimbot.hpp"
#include "esp/esp.hpp"

namespace hack {
	struct impl {
		other::prediction m_prediction;
		translator m_translator;
		trainer m_trainer;
		aimbot m_aimbot;
		esp m_esp;
	};
} // namespace hack

inline hack::impl g_hack;