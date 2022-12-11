
//********************************************************************************************************
// Public defines
//********************************************************************************************************

//	structure retuned by mainopt_parse
	struct mainopt_struct
	{
		bool full_screen = false;
		int number_of_elements 	= 5;
		int envelopes_in_phase 	= 2;
		int element_freq_max	= 8;
		int base_resolution		= 1024;
		int window_width		= 1000;
		int window_height		= 1000;
		float envelope_speed	= 0.15;
		float amplitude			= 1.0;
		bool finished			= false;
		bool error				= false;
	};

//********************************************************************************************************
// Public variables
//********************************************************************************************************

//********************************************************************************************************
// Public prototypes
//********************************************************************************************************

//	Process command line options.
//	Small tasks, such as displaying help or version are handled internally.
	struct mainopt_struct mainopt_parse(int argc, char *argv[]);
