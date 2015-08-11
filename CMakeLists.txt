#specify version
cmake_minimum_required(VERSION 3.0)
#name of project
project(QBF_CPP)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -std=c++11 -Wall -pedantic")
set(SOURCE_FILES main.cpp)
include_directories(depqbf formula)
link_directories(depqbf)
add_executable(QBF_CPP ${SOURCE_FILES} parser.cpp parser.h Formula/clause.cpp Formula/clause.h Formula/formula.cpp Formula/formula.h FNFException.cpp FNFException.h ParseException.cpp ParseException.h Formula/quantgroup.cpp Formula/quantgroup.h  preprocessing.cpp preprocessing.h)
target_link_libraries(QBF_CPP qdpll)
