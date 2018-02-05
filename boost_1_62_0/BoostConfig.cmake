# disable Boost_NO_BOOST_CMAKE otherwise circulal dependency appears
SET(Boost_NO_BOOST_CMAKE ON)

if(NOT Boost_FOUND)	
	if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
		find_package(Boost 1.62.0 QUIET MODULE)
		set(Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIRS} ${BOOST_LIBRARYDIR})
		# Manually add libraries which are not found by findboost.
		find_library(BOOST_BZIP2 boost_bzip2 PATH ${Boost_LIBRARY_DIRS} NO_DEFAULT_PATH)
		find_library(BOOST_ZLIB boost_zlib PATH ${Boost_LIBRARY_DIRS} NO_DEFAULT_PATH)
		find_library(BOOST_FILESYSTEM boost_filesystem PATH ${Boost_LIBRARY_DIRS} NO_DEFAULT_PATH)
		find_library(BOOST_SYSTEM boost_system PATH ${Boost_LIBRARY_DIRS} NO_DEFAULT_PATH)		
		find_library(BOOST_THREAD boost_thread PATH ${Boost_LIBRARY_DIRS} NO_DEFAULT_PATH)
		find_library(BOOST_CHRONO boost_chrono PATH ${Boost_LIBRARY_DIRS} NO_DEFAULT_PATH)
		find_library(BOOST_TIMER boost_timer PATH ${Boost_LIBRARY_DIRS} NO_DEFAULT_PATH)
		find_library(BOOST_REGEX boost_regex PATH ${Boost_LIBRARY_DIRS} NO_DEFAULT_PATH)
		find_library(BOOST_LOG boost_log PATH ${Boost_LIBRARY_DIRS} NO_DEFAULT_PATH)
		find_library(BOOST_LOG_SETUP boost_log_setup PATH ${Boost_LIBRARY_DIRS} NO_DEFAULT_PATH)
		find_library(BOOST_PROGRAM_OPTIONS boost_program_options PATH ${Boost_LIBRARY_DIRS} NO_DEFAULT_PATH)
		find_library(BOOST_ATOMIC boost_atomic PATH ${Boost_LIBRARY_DIRS} NO_DEFAULT_PATH)
		find_library(BOOST_DATE_TIME boost_date_time PATH ${Boost_LIBRARY_DIRS} NO_DEFAULT_PATH)
		find_library(BOOST_IOSTREAM boost_iostreams PATH ${Boost_LIBRARY_DIRS} NO_DEFAULT_PATH)	
	
		set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_BZIP2})
		set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_ZLIB})
	    set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_FILESYSTEM})
		set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_SYSTEM})
		set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_THREAD})
		set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_CHRONO})
		set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_TIMER})
		set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_REGEX})
		set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LOG})
		set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LOG_SETUP})
		set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_PROGRAM_OPTIONS})
		set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_ATOMIC})
		set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_DATE_TIME})
		set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_IOSTREAM})
	
		# Boost requires to have some standard linux libraries so add them...
		set(Boost_LIBRARIES ${Boost_LIBRARIES} pthread nsl m dl rt)
	
	else()
		find_package(Boost 1.62.0 QUIET MODULE)
	endif()
endif()

if(MSVC) # CLEAR Boost_LIBRARIES and USE AUTOLINK FEATURE ON MSVC
	set(Boost_LIBRARIES "")
endif()