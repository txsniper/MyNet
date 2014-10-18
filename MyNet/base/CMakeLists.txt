set(base_SRCS
  Condition.cc
  CountDownLatch.cc
  Thread.cc
  Date.cc
  FileUtil.cc
  LogStream.cc
  Timestamp.cc
  TimeZone.cc
  ThreadPool.cc
  ProcessInfo.cc
  LogFile.cc
  )

add_library(mynet_base ${base_SRCS})
target_link_libraries(mynet_base pthread rt)

add_library(mynet_base_cpp11 ${base_SRCS})
target_link_libraries(mynet_base_cpp11 pthread rt)
set_target_properties(mynet_base_cpp11 PROPERTIES COMPILE_FLAGS "-std=c++0x")

install(TARGETS mynet_base DESTINATION lib)
install(TARGETS mynet_base_cpp11 DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/MyNet/base)

add_subdirectory(tests)
