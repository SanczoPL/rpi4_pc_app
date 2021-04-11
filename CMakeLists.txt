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

macro(copy_json jsonname)
	configure_file(${jsonname} ${CMAKE_BINARY_DIR}/${jsonname})
	configure_file(${jsonname} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bin/${jsonname})
	configure_file(${jsonname} ${CMAKE_BINARY_DIR}/bin/${jsonname})
	configure_file(${jsonname} ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/${jsonname})
endmacro()
copy_json("config.json")

if (WIN32)
	set(QT5_PATH  "E:\\Qt\\5.15.2\\msvc2019_64")
	list(APPEND CMAKE_PREFIX_PATH "E:\\repo\\opencvBuild")
	list(APPEND CMAKE_PREFIX_PATH ${QT5_PATH})
	list(APPEND CMAKE_PREFIX_PATH "E:\\repo\\spdlog\\build\\install")
endif (WIN32)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Qt5Test REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

enable_testing()

macro(clone_build libname)
	configure_file(clone_build/${libname}.in ${libname}-download/CMakeLists.txt)
	execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${libname}-download")
	execute_process(COMMAND "${CMAKE_COMMAND}" --build .
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${libname}-download")
	add_subdirectory("${CMAKE_BINARY_DIR}/${libname}-src"
                 "${CMAKE_BINARY_DIR}/${libname}-build")
endmacro()

clone_build(googletest)
# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
clone_build(spdlog)
clone_build(IncludeSpdlog)
clone_build(ConfigReader)
clone_build(QtCommunicator)
clone_build(QtCommunicatorPlugin)
clone_build(Broadcaster)

file(GLOB_RECURSE SRC_TEST_GLOB QtCommunicator/src/*.cc )
file(GLOB_RECURSE HEADERS_TEST_GLOB QtCommunicator/include/*.h)

set(SRC_MAIN src/main.cc)

set(SRC src/mainwindow.cc src/server.cc )
set(HEADERS src/mainwindow.h src/server.h src/structures.h)


file(GLOB_RECURSE SRC_WIDGET_GLOB src/widgets/*.cc src/widgets/*.c src/widgets/*.cpp)
file(GLOB_RECURSE HEADERS_WIDGET_GLOB src/widgets/*.h src/widgets/*.hpp)

add_executable(g_test
config.json ${SRC} ${HEADERS} ${SRC_WIDGET_GLOB} ${HEADERS_WIDGET_GLOB} gtest/gtest.cc)
add_executable(q_test 
config.json ${SRC} ${HEADERS} ${SRC_WIDGET_GLOB} ${HEADERS_WIDGET_GLOB} qtest/qtest.cc qtest/qtest.h)

add_executable(${PROJECT_NAME}
	config.json
	${SRC_MAIN}
    ${SRC} ${HEADERS}
	${SRC_GLOB} ${HEADERS_GLOB}
	${SRC_WIDGET_GLOB} ${HEADERS_WIDGET_GLOB}
)

target_link_libraries(g_test Broadcaster QtCommunicator gtest gmock)
target_link_libraries(g_test ${QT_LIBRARIES} ${OpenCV_LIBS} Threads::Threads)
target_link_libraries(g_test Qt5::Test Qt5::Network Qt5::Widgets) 

target_link_libraries(q_test Broadcaster QtCommunicator )
target_link_libraries(q_test ${QT_LIBRARIES} ${OpenCV_LIBS} Threads::Threads)
target_link_libraries(q_test Qt5::Test Qt5::Network Qt5::Widgets) 

target_link_libraries(${PROJECT_NAME} Broadcaster)
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${OpenCV_LIBS} Threads::Threads)
target_link_libraries(${PROJECT_NAME} Qt5::Network Qt5::Widgets)

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
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Test${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Gui${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${QT5_PATH}/bin/Qt5Widgets${OPENCV_PREFIX_BUILD_TYPE}.dll   ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
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
