cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(rpi-pc-app VERSION 0.0.1 DESCRIPTION "rpi-pc-app" LANGUAGES C CXX)

set(DEFAULT_BUILD_TYPE "Debug")
set(CMAKE_BUILD_TYPE "Debug")
set(TARGET_NAME "rpi-pc-app")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS_COMMON}")

set(CMAKE_AUTOMOC ON)

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

configure_file(config.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.json)
configure_file(config.json ${CMAKE_BINARY_DIR}/config.json)

if (WIN32)
	list(APPEND CMAKE_PREFIX_PATH "E:\\repo\\opencvBuild")
	set(QT5_PATH "E:\\Qt\\5.12.8\\msvc2017_64")
	list(APPEND CMAKE_PREFIX_PATH "E:\\Qt\\5.12.8\\msvc2017_64")
	list(APPEND CMAKE_PREFIX_PATH "E:\\repo\\spdlog\\build\\install")
	list(APPEND CMAKE_PREFIX_PATH "E:\\repo\\spdlog\\build\\install//lib//cmake//spdlog")
	include("E:\\repo\\spdlog\\cmake\\utils.cmake")
	list(APPEND CMAKE_PREFIX_PATH "E:\\repo\\IncludeSpdlog\\build\\install")
	list(APPEND CMAKE_PREFIX_PATH "E:\\repo\\ConfigReader\\build\\install")
	list(APPEND CMAKE_PREFIX_PATH "E:\\repo\\QtCommunicator\\build\\install")
	list(APPEND CMAKE_PREFIX_PATH "E:\\repo\\QtCommunicatorPlugin\\build\\install")
	list(APPEND CMAKE_PREFIX_PATH "E:\\repo\\Broadcaster\\build\\install")
endif (WIN32)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Qt5Test REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
find_package(spdlog REQUIRED)
find_package(IncludeSpdlog REQUIRED)
find_package(ConfigReader REQUIRED)
find_package(QtCommunicator REQUIRED)
find_package(QtCommunicatorPlugin REQUIRED)
find_package(Broadcaster REQUIRED)



file(GLOB_RECURSE SRC_TEST_GLOB QtCommunicator/src/*.cc )
file(GLOB_RECURSE HEADERS_TEST_GLOB QtCommunicator/include/*.h)

set(SRC_TEST test/test_broadcaster.cc )
set(HEADERS_TEST test/test_broadcaster.h)

#set(SRC_GTEST gtest/test.cc)
#set(HEADERS_GTEST gtest/test.h)

#set(SRC_MAIN_GTEST gtest/main.cc)
set(SRC_MAIN src/main.cc)
set(SRC_MAIN_TEST test/main.cc)

set(SRC src/mainwindow.cc src/server.cc )
set(HEADERS src/mainwindow.h src/server.h src/structures.h)


file(GLOB_RECURSE SRC_WIDGET_GLOB src/widgets/*.cc src/widgets/*.c src/widgets/*.cpp)
file(GLOB_RECURSE HEADERS_WIDGET_GLOB src/widgets/*.h src/widgets/*.hpp)



add_executable(${TARGET_NAME}
	${SRC_MAIN}
    ${SRC} ${HEADERS}
	${SRC_GLOB} ${HEADERS_GLOB}
	${SRC_WIDGET_GLOB} ${HEADERS_WIDGET_GLOB}
)

add_executable(testModules 
	${SRC} ${HEADERS} ${SRC_MAIN_TEST}
	${SRC_TEST} ${HEADERS_TEST}
	${SRC_GLOB} ${HEADERS_GLOB}
	${SRC_WIDGET_GLOB} ${HEADERS_WIDGET_GLOB}
)
	
#add_executable(gtestModules
#	${SRC} ${HEADERS}
#	${SRC_MAIN_GTEST}
#	#${SRC_GTEST} ${HEADERS_GTEST}
#	${SRC_GLOB} ${HEADERS_GLOB}
#	${SRC_WIDGET_GLOB} ${HEADERS_WIDGET_GLOB}
#)




target_link_libraries(testModules Broadcaster QtCommunicator )
target_link_libraries(testModules ${QT_LIBRARIES} ${OpenCV_LIBS} Threads::Threads)
target_link_libraries(testModules Qt5::Test Qt5::Network Qt5::Widgets) 
add_test(testModules testModules)

#target_link_libraries(gtestModules IncludeSpdlog ConfigReader Broadcaster QtCommunicator QtCommunicatorPlugin)
#target_link_libraries(gtestModules ${QT_LIBRARIES} ${OpenCV_LIBS} Threads::Threads)
#target_link_libraries(gtestModules Qt5::Network Qt5::Widgets)
#target_link_libraries(gtestModules gtest gtest_main)
#add_test( gtestModules gtestModules )

qt5_use_modules(${PROJECT_NAME} Network Widgets Test)


if (UNIX)
	target_link_libraries(${PROJECT_NAME} Broadcaster)
	target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${OpenCV_LIBS} Threads::Threads)
	target_link_libraries(${PROJECT_NAME} Qt5::Network Qt5::Widgets)
endif (UNIX)

if (WIN32)
	target_link_libraries(${PROJECT_NAME} Broadcaster)
	target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${OpenCV_LIBS} Threads::Threads)
	target_link_libraries(${PROJECT_NAME} Qt5::Network Qt5::Widgets)
endif (WIN32)



message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE} ")
if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND WIN32)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

endif()
if (CMAKE_BUILD_TYPE STREQUAL "Release" AND WIN32)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

endif()



if (WIN32)
    get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
    set(QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
       COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} --qmldir ${CMAKE_SOURCE_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
	add_custom_command(TARGET testModules POST_BUILD
       COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} --qmldir ${CMAKE_SOURCE_DIR} $<TARGET_FILE_DIR:testModules>)
endif(WIN32)


if (WIN32)
	macro(qt5_copy_dll APP DLL)
		# find the release *.dll file
		if(${CMAKE_BUILD_TYPE} STREQUAL  "Release")
			get_target_property(Qt5_${DLL}Location Qt5::${DLL} LOCATION)
		endif()
		# find the debug *d.dll file
		if(${CMAKE_BUILD_TYPE} STREQUAL  "Debug")
			get_target_property(Qt5_${DLL}LocationDebug Qt5::${DLL} IMPORTED_LOCATION_DEBUG)
		endif()

		add_custom_command(TARGET ${APP} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${Qt5_${DLL}LocationDebug}> $<$<NOT:$<CONFIG:Debug>>:${Qt5_${DLL}Location}> $<TARGET_FILE_DIR:${APP}>
			
			)
	endmacro()
	qt5_copy_dll( ${PROJECT_NAME} Core)
	qt5_copy_dll( testModules Core)

	message("CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}/Release ")


	set(OPENCV_PREFIX_BUILD_TYPE "")
	message("Copy OpenCV file:" )
	if(${CMAKE_BUILD_TYPE} STREQUAL  "Release")
		set(OPENCV_PREFIX_BUILD_TYPE "")
		message("Opencv prefix: Release" )
	endif()
	if(${CMAKE_BUILD_TYPE} STREQUAL  "Debug")
		set(OPENCV_PREFIX_BUILD_TYPE "d")
		message("Opencv prefix: Debug" )
	endif()

	message("Copy OpenCV file:" )
	message("qt:" ${QT5_PATH}/bin/Qt5Test${OPENCV_PREFIX_BUILD_TYPE}.dll )
		message("to:" ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		
		#COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Test${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
		#COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Gui${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
		#COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Widgets${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_stitching450${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_bgsegm450${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_tracking450${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_videoio450${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_video450${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_imgcodecs450${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_imgproc450${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_core450${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_calib3d450${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_features2d450${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_flann450${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_dnn450${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_highgui450${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_gapi450${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${OpenCV_DIR}/bin/${CMAKE_BUILD_TYPE}/opencv_videoio_ffmpeg450_64.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/

		)


endif(WIN32)
