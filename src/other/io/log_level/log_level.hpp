#pragma once

#include "../../utils/cx/cx.hpp"

namespace io {
	enum class log_level
	{
		NORMAL = 0,
		WARNING,
		DISASTER,
	};

	namespace detail {
		struct log_option {
		private:

			std::string m_prefix;
			fmt::text_style m_style;

		public:

			log_option( fmt::text_style style, std::string_view prefix ) {
				m_prefix = prefix;
				m_style  = style;
			}

			auto get_style( ) const {
				return m_style;
			}

			auto get_prefix( ) const {
				return m_prefix.data( );
			}
		};

		struct level_handler {
		private:

			static inline std::array< log_option, 3 > m_options = {
				// NORMAL
				log_option( fmt::text_style( fmt::fg( fmt::color::white ) ), "" ),
				// WARNING
				log_option( fmt::text_style( fmt::fg( fmt::color::aquamarine ) ), "WARNING - " ),
				// DISASTER
				log_option( fmt::text_style( fmt::fg( fmt::color::red ) ), "ERROR! DISASTER! - " )
			};

		public:

			log_option& operator[]( log_level idx ) {
				return m_options[ static_cast< size_t >( idx ) ];
			}
		};

	} // namespace detail

	inline detail::level_handler level_handler;
} // namespace io