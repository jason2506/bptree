find_package(GTest REQUIRED)
include(${PROJECT_SOURCE_DIR}/cmake/PatchFindGTest.cmake)

set(${PROJECT_NAME}_TESTS
    static_vector_test
    static_assoc_test
)

enable_testing()
foreach(test ${${PROJECT_NAME}_TESTS})
    add_executable(${test} ${test}.cpp)

    target_link_libraries(${test}
        GTest::GTest
        GTest::Main
        ${PROJECT_NAME}
    )

    add_test(NAME ${test}
        COMMAND ${test} --gtest_color=yes
    )
endforeach(test ${test_SRCS})
