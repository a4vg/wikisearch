cmake_minimum_required(VERSION 3.15)
project(cs3102_trabajofinal_covid_20)

set(CMAKE_CXX_STANDARD 14)

include_directories(DataProcessing)

add_executable(cs3102_trabajofinal_covid_20
        DataProcessing/src/B+Node.cpp
        DataProcessing/src/ZimManager.cpp
        DataProcessing/src/htmlParser.cpp
        DataProcessing/src/Preprocessor.cpp
        DataProcessing/src/SearchEngineManager.cpp
        DataProcessing/processing.cpp
        )

target_link_libraries(cs3102_trabajofinal_covid_20 zim myhtml)
