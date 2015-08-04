# Boost
find_path(Boost_LOCAL NAMES boost/config.hpp PATHS "${CMAKE_CURRENT_SOURCE_DIR}/../include" NO_DEFAULT_PATH)
if(Boost_LOCAL)
	set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../include")
	find_package(Boost 1.49.0)
endif()
if(NOT Boost_FOUND)
	unset(BOOST_ROOT)
	message(STATUS "Did not find Boost locally; trying globally...")
	message(STATUS "If this doesn't work, run ./get-boost.sh and try again.")
	find_package(Boost 1.49.0 REQUIRED)
endif()
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

set(SHARED_HS
	../include/cycle.h
	../include/helpers.hpp
	)

ADD_EXECUTABLE(set
	set.cpp
	${SHARED_HS}
	../include/btree_set.h
	../include/safe_btree_set.h
	../include/interval_vector.hpp
	../include/sorted_deque.hpp
	../include/sorted_vector.hpp
	../include/tdc_trie.hpp
	../include/sti/sset.h
	)

ADD_EXECUTABLE(vector-realloc vector-realloc.cpp)

ADD_EXECUTABLE(dynamic-cast dynamic-cast.cpp ${SHARED_HS})

ADD_EXECUTABLE(string-to-double string-to-double.cpp ${SHARED_HS})

ADD_EXECUTABLE(string-to-int string-to-int.cpp ${SHARED_HS})

ADD_EXECUTABLE(string-compare string-compare.cpp ${SHARED_HS})

ADD_EXECUTABLE(read-whole-file read-whole-file.cpp ${SHARED_HS})
