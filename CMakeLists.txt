project(curl C CXX ASM)

set(CURL_STATICLIB ON)
set(BUILDING_LIBCURL ON)
set(HAVE_CONFIG_H ON)

add_library(curl OBJECT
	siglo/curl_nintendo_allocator.c
	siglo/curl_nintendo_allocator.h
	siglo/curl_nintendo_shim.c
	siglo/curl_nintendo_shim.h
)

target_compile_options(curl PRIVATE -fno-inline)
target_include_directories(curl PUBLIC include/)
