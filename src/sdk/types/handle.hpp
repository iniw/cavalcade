#pragma once

namespace sdk {
	struct handle {
		enum : u32
		{
			MAX_EDICT_BITS            = 11,
			MAX_EDICTS                = 1 << MAX_EDICT_BITS,
			NUM_ENT_ENTRY_BITS        = MAX_EDICT_BITS + 2,
			NUM_ENT_ENTRIES           = 1 << NUM_ENT_ENTRY_BITS,
			INVALID_EHANDLE_INDEX     = 0xFFFFFFFF,
			NUM_SERIAL_NUM_BITS       = 32 - NUM_ENT_ENTRY_BITS,
			NUM_SERIAL_NUM_SHIFT_BITS = 32 - NUM_SERIAL_NUM_BITS,
			ENT_ENTRY_MASK            = ( 1 << NUM_SERIAL_NUM_BITS ) - 1,
		};

		bool validate( ) const {
			return m_idx != INVALID_EHANDLE_INDEX;
		}

		void invalidate( ) {
			m_idx = INVALID_EHANDLE_INDEX;
		}

		i32 get_index( ) const {
			if ( !validate( ) )
				return NUM_ENT_ENTRIES - 1;

			return m_idx & ENT_ENTRY_MASK;
		}

		template< typename T = base_entity* >
		T get( ) const;

	protected:

		u32 m_idx;
	};
} // namespace sdk