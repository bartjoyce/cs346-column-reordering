add_executable(visualise
  colors.hpp
  colors.cpp
  visualisation.hpp
  visualisation.cpp
  BitmapImage.hpp
  BitmapImage.cpp
  main.cpp
)

target_link_libraries(visualise shared)
