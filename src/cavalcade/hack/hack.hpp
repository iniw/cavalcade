#pragma once

#include "other/prediction.hpp"
#include "translator/translator.hpp"
#include "trainer/trainer.hpp"
#include "aimbot/aimbot.hpp"
#include "esp/esp.hpp"
#include "graph/graph.hpp"
#include "indicators/screen.hpp"
#include "nightmode/nightmode.hpp"
#include "movement/movement.hpp"
#include "hitmarker/hitmarker.hpp"
#include "fog/fog.hpp"
#include "sunset/sunset.hpp"
#include "scaleform/scaleform.hpp"
#include "backtrack/backtrack.hpp"
#include "weather/weather.hpp"

namespace hack {
	struct impl {
		other::prediction m_prediction;
		translator m_translator;
		trainer m_trainer;
		aimbot m_aimbot;
		esp m_esp;
		graph m_velgraph;
		indicators::screen m_indscreen;
		nightmode m_nightmode;
		movement m_movement;
		hitmarker m_hitmarker;
		fog m_fog;
		sunset m_sunset;
		scaleform m_scaleform;
		backtrack m_backtrack;
		weather m_weather;
	};
} // namespace hack

inline hack::impl g_hack;