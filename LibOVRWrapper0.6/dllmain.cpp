// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include "shimhelper.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{			
			const char inifile[] = "LibOVRWrapper.ini";

			auto settings = new WrapperSettings();

			boost::shared_ptr<sinks::synchronous_sink<sinks::text_file_backend>> sink = nullptr;

			if (std::ifstream(inifile)) {
				boost::property_tree::ptree pt;
				try {
					boost::property_tree::ini_parser::read_ini(inifile, pt);

					settings->loglevel = pt.get<int>("logging.loglevel", 0);
					if (settings->loglevel > 0) {
						sink = logging::add_file_log(
							keywords::file_name = "LibOVRWrapper.log", 
							keywords::auto_flush = true
						);
						
					}
					settings->srgbCorrectionEnabled = pt.get<bool>("graphics.srgbCorrectionEnabled", true);

				}
				catch (boost::property_tree::ini_parser_error) {

				}
			}			

			if (settings->loglevel < 0)
				settings->loglevel = 0;
			else if (settings->loglevel > 3)
				settings->loglevel = 3;
			
			switch (settings->loglevel) {
			case 0:
				logging::core::get()->set_logging_enabled(false);
				break;
			case 1:
				logging::core::get()->set_filter
				(
					logging::trivial::severity >= logging::trivial::warning
				);
				break;
			case 2:
				logging::core::get()->set_filter
				(
					logging::trivial::severity >= logging::trivial::info
				);
				break;
			case 3:
				logging::core::get()->set_filter
				(
					logging::trivial::severity >= logging::trivial::trace
				);
				break;			
			}

			BOOST_LOG_TRIVIAL(info) << "Initialized logging";			

			setWrapperSettings(settings);
		}		
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

