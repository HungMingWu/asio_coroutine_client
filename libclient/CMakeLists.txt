add_library(libclient 
	src/client.cpp
)
target_include_directories(libclient
PUBLIC include/
)
target_link_libraries(libclient PRIVATE asio)
target_compile_features(libclient PUBLIC cxx_std_20)