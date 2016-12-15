set(SOURCE_FILES
        OpenMP.cpp)

add_executable(project ${SOURCE_FILES})

set_target_properties(project PROPERTIES COMPILE_DEFINITIONS DEBUG)
