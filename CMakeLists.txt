cmake_minimum_required(VERSION 3.20)
project(ChartCreator)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#[[ Linux ]]
set(CMAKE_PREFIX_PATH "/home/enrico/Qt/5.15.2/gcc_64/lib/cmake")
#[[ Windows
set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/mingw81_64/lib/cmake") ]]

set(RESOURCE_FILE
        Data/resources.qrc)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Charts
        Xml
        Network
        REQUIRED)

add_executable(ChartCreator "main.cpp" "src/Views/Source files/CurrentFilesView.cpp" "src/Views/Header files/CurrentFilesView.h" "src/Views/Source files/MainWindow.cpp" "src/Views/Header files/MainWindow.h" "src/Views/Source files/MainMenuBar.cpp" "src/Views/Header files/MainMenuBar.h"
        #[[ Windows only:  "Data/Resources.qrc" "Data/appicon.rc" ]] "src/Views/Source files/ChartView.cpp" "src/Views/Header files/ChartView.h" "src/Rows/Header files/DataRow.h" "src/Containers/Source files/RowContainer.cpp" "src/Containers/Header files/RowContainer.h" "src/Views/Source files/BarDataView.cpp" "src/Views/Header files/BarDataView.h" "src/Rows/Source files/DataRow.cpp" "src/Rows/Source files/AbstractRow.cpp" "src/Rows/Header files/AbstractRow.h" "src/Rows/Source files/SeriesRow.cpp" "src/Rows/Header files/SeriesRow.h" "src/Containers/Source files/AbstractRowContainer.cpp" "src/Containers/Header files/AbstractRowContainer.h" "src/Containers/Source files/SeriesContainer.cpp" "src/Containers/Header files/SeriesContainer.h" "src/Views/Source files/SeriesView.cpp" "src/Views/Header files/SeriesView.h" "src/Views/Source files/SingleFileView.cpp" "src/Views/Header files/SingleFileView.h" "src/Models/Source files/BarChartModel.cpp" "src/Models/Header files/BarChartModel.h" "src/Buttons/Source files/SelectColorButton.cpp" "src/Buttons/Header files/SelectColorButton.h" "src/Charts/Source files/PieChart.cpp" "src/Charts/Header files/PieChart.h" "src/Charts/Source files/AbstractChart.cpp" "src/Charts/Header files/AbstractChart.h" "src/Charts/Source files/BarChart.cpp" "src/Charts/Header files/BarChart.h" "src/Models/Source files/AbstractModel.cpp" "src/Models/Header files/AbstractModel.h" "src/Models/Source files/PieChartModel.cpp" "src/Models/Header files/PieChartModel.h" "src/Views/Source files/PieDataView.cpp" "src/Views/Header files/PieDataView.h" "src/Views/Source files/AbstractDataView.cpp" "src/Views/Header files/AbstractDataView.h" "src/Rows/Source files/SplineDataRow.cpp" "src/Rows/Header files/SplineDataRow.h" "src/Containers/Source files/SplineRowContainer.cpp" "src/Containers/Header files/SplineRowContainer.h" "src/Views/Source files/SplineDataView.cpp" "src/Views/Header files/SplineDataView.h" "src/Charts/Source files/SplineChart.cpp" "src/Charts/Header files/SplineChart.h" "src/Models/Source files/SplineChartModel.cpp" "src/Models/Header files/SplineChartModel.h" "src/HttpRequest/Source files/RiotApi.cpp" "src/HttpRequest/Header files/RiotApi.h" "../Data/resources.qrc" "src/HttpRequest/Header files/ApiKey.h")

target_link_libraries(ChartCreator
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Charts
        Qt5::Xml
        Qt5::Network
        )

if (WIN32)
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets Charts Xml)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
