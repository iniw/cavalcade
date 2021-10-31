#ifndef MD5_HPP
#define MD5_HPP

namespace hash {
	enum
	{
		MD5_DIGEST_LENGTH = 16,
		MD5_BIG_LENGHT    = MD5_DIGEST_LENGTH * sizeof( u8 )
	};

	struct md5_context {
		u32 buf[ 4 ];
		u32 bits[ 2 ];
		u8 in[ 64 ];
	};

	struct md5 {
		static u32 gen( u32 seed );

		static void init( md5_context* context );

		static void update( md5_context* context, u8 const* buf, u32 len );

		static void final( u8 digest[ MD5_DIGEST_LENGTH ], md5_context* context );

		static void transform( u32 buf[ 4 ], u32 const in[ 16 ] );
	};
} // namespace hash

#endif // MD5_HPP