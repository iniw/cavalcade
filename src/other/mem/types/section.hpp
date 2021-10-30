#ifndef SECTION_HPP
#define SECTION_HPP

namespace mem {
	struct section {
		u32 m_start;
		u32 m_size;

		section( ) = default;

		section( u32 start, u32 size ) : m_start{ start }, m_size{ size } { }

		u32 end( ) { return m_start + m_size; }
	};
} // namespace mem

#endif // SECTION_HPP