cmake_minimum_required(VERSION 2.6)

if(NOT GLUON_BUILD_ALL)
    find_package(GluonGraphics REQUIRED)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${GLUON_GRAPHICS_INCLUDES})

set(particlecollidercomponent_SRCS
    particlecollidercomponent.cpp
)

qt4_automoc(${particlecollidercomponent_SRCS})
add_library(gluon_component_particlecollider SHARED ${particlecollidercomponent_SRCS})

if(GLUON_BUILD_ALL)
    add_dependencies(gluon_component_particlecollider GluonEngine GluonGraphics)
endif()

set_target_properties(gluon_component_particlecollider PROPERTIES VERSION ${GLUON_VERSION_STRING} SOVERSION ${GLUON_VERSION_STRING} DEFINE_SYMBOL MAKE_GLUON_COMPONENT_PARTICLECOLLIDER_LIB)
target_link_libraries(gluon_component_particlecollider GluonEngine ${GLUON_GRAPHICS_LIBS})

install(
    TARGETS
    gluon_component_particlecollider
    DESTINATION
    ${LIB_INSTALL_DIR}/gluon
)
