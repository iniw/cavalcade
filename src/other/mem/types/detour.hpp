#ifndef DETOUR_HPP
#define DETOUR_HPP

#include "address.hpp"

namespace mem {
	struct detour : address {
		u64 m_og;

	private:

		struct {
			PLH::CapstoneDisassembler* capstone;
			PLH::x86Detour* x86_detour;

		} m_plh;

	public:

		constexpr detour( ) = default;

		detour( address addr ) : m_og{ 0u } {
			m_address = addr;

			m_plh.capstone   = nullptr;
			m_plh.x86_detour = nullptr;
		}

		bool hook( unk target ) {
			m_plh.capstone   = new PLH::CapstoneDisassembler( PLH::Mode::x86 );
			m_plh.x86_detour = new PLH::x86Detour( m_address, reinterpret_cast< u64 >( target ), &m_og, *m_plh.capstone );

			return m_plh.x86_detour->hook( );
		}

		void unhook( ) {
			// @polyhook devs: whoever decided it was a good idea to unhook on the destructor, i
			// wish you a very fuck you
			if ( m_plh.x86_detour && m_plh.capstone ) {
				delete m_plh.x86_detour;
				delete m_plh.capstone;
			}
		}
	};
} // namespace mem

#endif // DETOUR_HPP
