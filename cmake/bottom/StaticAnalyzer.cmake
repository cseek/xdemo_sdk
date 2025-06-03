if(ENABLE_CLANG_TIDY)
    find_program(CLANGTIDY clang-tidy)
    if(CLANGTIDY)
        set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY} -extra-arg=-Wno-unknown-warning-option)
        message("Clang-Tidy finished setting up.")
    else()
        message(SEND_ERROR "Clang-Tidy requested but executable not found.")
    endif()
endif()

if(ENABLE_CPPCHECK)
    find_program(CPPCHECK cppcheck)
    if(CPPCHECK)
        set(CMAKE_CXX_CPPCHECK ${CPPCHECK} 
            --enable=all
            --inline-suppr
            --inconclusive
            --suppress=missingIncludeSystem
            --xml
            --xml-version=2
            --output-file=${CPPCHECK_REPORT_XML}
            -i${TOP}/deps/**
            -i${TOP}/build/**
        )
        message("Cppcheck finished setting up.")
    else()
        message(SEND_ERROR "Cppcheck requested but executable not found.")
    endif()
endif()
