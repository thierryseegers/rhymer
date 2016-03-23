add_library(librhymer INTERFACE)

target_sources(librhymer
        INTERFACE rhymer.h)

add_custom_target(rhymer
        SOURCES rhymer.h)
