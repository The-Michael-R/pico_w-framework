# Include each library part
add_subdirectory("lib/task1")
add_subdirectory("lib/global")
add_subdirectory("lib/wlan")


# Return the collected libraries to callee
return(PROPAGATE LIBRARIES)
