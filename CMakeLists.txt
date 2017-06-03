cmake_minimum_required(VERSION 3.1)
project(Tubac)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(CMAKE_CXX_STANDARD 14)
find_package(Boost 1.64.0 COMPONENTS serialization filesystem system REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    #    include_directories(/usr/include/allegro5/)
    add_executable(genedriver
	GeneDriver/AllegroProxy.cpp
        GeneDriver/BoundingBox.cpp
	GeneDriver/Dimension2d.cpp
        GeneDriver/Painter.cpp
	GeneDriver/World.cpp
        GeneDriver/main.cpp
	GeneDriver/Neural/Network.cpp
    )
    target_link_libraries(genedriver ${Boost_LIBRARIES} /usr/lib/x86_64-linux-gnu/liballegro.so.5.0.11 /usr/lib/x86_64-linux-gnu/liballegro_font.so.5.0.11 /usr/lib/x86_64-linux-gnu/liballegro_ttf.so.5.0.11 /usr/lib/x86_64-linux-gnu/liballegro_main.so.5.0.11 /usr/lib/x86_64-linux-gnu/liballegro_primitives.so.5.0.11 /usr/lib/x86_64-linux-gnu/liballegro_image.so.5.0.11)
endif()
