#pragma once

namespace sdk::auxiliary {
	struct net_channel_info {
		virtual const char* get_name( void ) const     = 0; // get channel name
		virtual const char* get_address( void ) const  = 0; // get channel IP address as string
		virtual float get_time( void ) const           = 0; // current net time
		virtual float get_time_connected( void ) const = 0; // get connection time in seconds
		virtual int get_buffer_size( void ) const      = 0; // netchannel packet history size
		virtual int get_data_rate( void ) const        = 0; // send data rate in byte/sec

		virtual bool is_loopback( void ) const   = 0; // true if loopback channel
		virtual bool is_timing_out( void ) const = 0; // true if timing out
		virtual bool is_playback( void ) const   = 0; // true if demo playback

		virtual float get_latency( int flow ) const                                        = 0; // current latency (RTT), more accurate but jittering
		virtual float get_avg_latency( int flow ) const                                    = 0; // average packet latency in seconds
		virtual float get_avg_loss( int flow ) const                                       = 0; // avg packet loss[0..1]
		virtual float get_avg_choke( int flow ) const                                      = 0; // avg packet choke[0..1]
		virtual float get_avg_data( int flow ) const                                       = 0; // data flow in bytes/sec
		virtual float get_avg_packets( int flow ) const                                    = 0; // avg packets/sec
		virtual int get_total_data( int flow ) const                                       = 0; // total flow in/out in bytes
		virtual int get_sequence_nr( int flow ) const                                      = 0; // last send seq number
		virtual bool is_valid_packet( int flow, int frame_number ) const                   = 0; // true if packet was not lost/dropped/chocked/flushed
		virtual float get_packet_time( int flow, int frame_number ) const                  = 0; // time when packet was send
		virtual int get_packet_bytes( int flow, int frame_number, int group ) const        = 0; // group size of this packet
		virtual bool get_stream_progress( int flow, int* received, int* total ) const      = 0; // TCP progress if transmitting
		virtual float get_time_since_last_received( void ) const                           = 0; // get time since last recieved packet in seconds
		virtual float get_command_interpolation_amount( int flow, int frame_number ) const = 0;
		virtual void get_packet_resposne_latency( int flow, int frame_number, int* pnLatencyMsecs, int* pnChoke ) const = 0;
		virtual void get_remote_framerate( float* pflFrameTime, float* pflFrameTimeStdDeviation ) const                 = 0;

		virtual float get_timeout_seconds( ) const = 0;
	};
} // namespace sdk::auxiliary