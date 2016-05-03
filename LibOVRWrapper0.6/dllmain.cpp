// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

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
			int loglevel = 0;
			const char inifile[] = "LibOVRWrapper.ini";

			if (std::ifstream(inifile)) {
				boost::property_tree::ptree pt;
				try {
					boost::property_tree::ini_parser::read_ini(inifile, pt);

					loglevel = pt.get<int>("logging.loglevel", 0);
					if (loglevel > 0) {
						logging::add_file_log("LibOVRWrapper.log");
					}

				}
				catch (boost::property_tree::ini_parser_error) {

				}
			}			

			if (loglevel < 0)
				loglevel = 0;
			else if (loglevel > 3)
				loglevel = 3;
			
			switch (loglevel) {
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
		}		
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

