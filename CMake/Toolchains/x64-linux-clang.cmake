set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR AMD64)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

string(APPEND CMAKE_C_FLAGS " -mavx2 -mfma -mxsave -msse4.2")
string(APPEND CMAKE_CXX_FLAGS " -mavx2 -mfma -mxsave -msse4.2")

