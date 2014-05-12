INCLUDE_DIRECTORIES(..)

ADD_EXECUTABLE(test-io test-io.cpp)

TARGET_LINK_LIBRARIES(test-io conviv)
TARGET_LINK_YOCTO(test-io json lingua)

GET_TARGET_PROPERTY(the_exe test-io LOCATION)

ADD_CUSTOM_COMMAND( TARGET test-io
	POST_BUILD
	COMMAND ${RCPACK} ${the_exe} ${CMAKE_CURRENT_SOURCE_DIR}/../embed )


ENABLE_TESTING()
ADD_TEST(CF3H  test-io ${CMAKE_CURRENT_SOURCE_DIR}/../../samples/CF3H/stdin)
ADD_TEST(C2H4O test-io ${CMAKE_CURRENT_SOURCE_DIR}/../../samples/C2H4O/stdin)
ADD_TEST(CH4   test-io ${CMAKE_CURRENT_SOURCE_DIR}/../../samples/CH4_DM_Hrot_eff/stdin)
ADD_TEST(Morse test-io ${CMAKE_CURRENT_SOURCE_DIR}/../../samples/Morse/stdin)
ADD_TEST(Morse test-io ${CMAKE_CURRENT_SOURCE_DIR}/../../samples/SO2/stdin)

ADD_CUSTOM_TARGET(check-io ALL
	COMMAND ${CMAKE_CTEST_COMMAND} -V 
	DEPENDS test-io)

