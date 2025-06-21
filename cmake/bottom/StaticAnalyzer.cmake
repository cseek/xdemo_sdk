if(ENABLE_CPPCHECK)
    find_program(CPPCHECK cppcheck)
    if(CPPCHECK)
        file(GLOB_RECURSE CHECK_SRCS
            ${TOP}/src/*.c
            ${TOP}/src/*.cpp
            ${TOP}/example/*.c
            ${TOP}/example/*.cpp
            ${TOP}/include/*.h
        )
        execute_process(
            COMMAND cppcheck
            --inline-suppr
            --std=c++11
            --enable=all
            --quiet
            --force
            --inconclusive
            --language=c++
            --suppress=missingIncludeSystem
            --suppress=missingInclude
            ${CHECK_SRCS}
            --xml
            --xml-version=2
            --output-file=${CPPCHECK_REPORT_XML}
        )
        # set(CMAKE_CXX_CPPCHECK ${CPPCHECK} 
        #     --inline-suppr
        #     --std=c++11
        #     --enable=all
        #     --quiet
        #     --force
        #     --inconclusive
        #     --language=c++
        #     --suppress=missingIncludeSystem
        #     --xml
        #     --xml-version=2
        #     --output-file=${CPPCHECK_REPORT_XML}
        # )
        message("Cppcheck finished setting up.")
    else()
        message(SEND_ERROR "Cppcheck requested but executable not found.")
    endif()
endif()