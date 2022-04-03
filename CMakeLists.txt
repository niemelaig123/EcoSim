cmake_minimum_required(VERSION 3.20)
project(Assignment4_GideonNiemelainen)

set(CMAKE_CXX_STANDARD 17)

add_executable(Assignment4_GideonNiemelainen main.cpp Organism.h Zombie.h Human.h City.h GameSpecs.h Human.cpp Zombie.cpp City.cpp Organism.cpp)
