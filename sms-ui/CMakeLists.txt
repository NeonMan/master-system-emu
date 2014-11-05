#Qt5 GUI
project(sms-ui)

# --- Qt5 ---
#Kit: MSVC 2013 kit
set(CMAKE_PREFIX_PATH "C:\\Qt\\5.3\\msvc2013")
#Kit: MSVC 2013 OpenGL
#set(CMAKE_PREFIX_PATH "C:\\Qt\\5.3\\msvc2013_opengl")
#Kit: Mingw 482
#set(CMAKE_PREFIX_PATH "C:\\Qt\\5.3\\mingw482_32")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Run automoc
set(CMAKE_AUTOMOC ON)

# Requred QT packages
find_package(Qt5Widgets)
find_package(Qt5Qml)
find_package(Qt5Quick)

# -----------
# Add resources
file (GLOB Resources *.qrc)
QT5_ADD_RESOURCES (ResourceSources ${Resources})

# Tell CMake to create the helloworld executable
add_executable(sms-ui main.cpp romwrapper.cpp ${ResourceSources})

# Use the Widgets module from Qt 5.
qt5_use_modules(sms-ui Widgets)
qt5_use_modules(sms-ui Quick)
qt5_use_modules(sms-ui Qml)