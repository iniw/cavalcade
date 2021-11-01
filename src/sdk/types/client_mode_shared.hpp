#ifndef CLIENT_MODE_SHARED_HPP
#define CLIENT_MODE_SHARED_HPP

namespace sdk {
	struct client_mode_shared {
		PAD( 0x28 );
		math::v2i m_root_size;
	};

} // namespace sdk

#endif // CLIENT_MODE_SHARED_HPP