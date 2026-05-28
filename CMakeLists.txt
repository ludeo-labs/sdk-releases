####################### Setup Ludeo SDK distribution #################
#
# Tidbits of information:
# https://stackoverflow.com/questions/46360959/how-do-i-specify-the-include-directory-for-an-imported-shared-library-using-cmak
#

add_library(LudeoSDK SHARED IMPORTED GLOBAL)

# Set the include path
target_include_directories(LudeoSDK INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")

#
# Useful information about setting up a SHARED IMPORTED library
# Below, I'm used IMPORTED_IMPLIB and IMPORTED_LOCATION, which specifies the libraries for all configurations.
# To specify different libs for Debug and Release, we can do use IMPORTED_IMPLIB_DEBUG/IMPORTED_LOCATION_DEBUG and
# IMPORTED_IMPLIB_RELEASE/IMPORTED_LOCATION_RELEASE
#
if (LUDEOSDK_DEBUG_USES_DEBUG)
	message(STATUS "LUDEOSDK: Debug builds will use the SDK's Debug build")
	set(debugConfig "Debug")
else()
	message(STATUS "LUDEOSDK: Debug builds will use the SDK's Development build")
	set(debugConfig "Development")
endif()


if (WIN32)
	set_target_properties(LudeoSDK PROPERTIES
		# Default is Release build
		IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/Bin/Win64/Release/LudeoSDK-Win64-Release.dll"
		IMPORTED_IMPLIB   "${CMAKE_CURRENT_LIST_DIR}/Lib/Win64/Release/LudeoSDK-Win64-Release.lib"

		IMPORTED_LOCATION_DEBUG "${CMAKE_CURRENT_LIST_DIR}/Bin/Win64/${debugConfig}/LudeoSDK-Win64-${debugConfig}.dll"
		IMPORTED_IMPLIB_DEBUG   "${CMAKE_CURRENT_LIST_DIR}/Lib/Win64/${debugConfig}/LudeoSDK-Win64-${debugConfig}.lib"

		IMPORTED_LOCATION_DEVELOPMENT "${CMAKE_CURRENT_LIST_DIR}/Bin/Win64/Development/LudeoSDK-Win64-Development.dll"
		IMPORTED_IMPLIB_DEVELOPMENT   "${CMAKE_CURRENT_LIST_DIR}/Lib/Win64/Development/LudeoSDK-Win64-Development.lib"
	)
elseif(ANDROID)
	set_target_properties(LudeoSDK PROPERTIES
		# Default is Release build
		IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/Bin/Android/Release/${ANDROID_ABI}/libLudeoSDK-Android-${ANDROID_ABI}-Release.so"
		IMPORTED_IMPLIB   "${CMAKE_CURRENT_LIST_DIR}/Lib/Android/Release/${ANDROID_ABI}/libLudeoSDK-Android-${ANDROID_ABI}-Release.so"

		IMPORTED_LOCATION_DEBUG "${CMAKE_CURRENT_LIST_DIR}/Bin/Android/${debugConfig}/${ANDROID_ABI}/libLudeoSDK-Android-${ANDROID_ABI}-${debugConfig}.so"
		IMPORTED_IMPLIB_DEBUG   "${CMAKE_CURRENT_LIST_DIR}/Lib/Android/${debugConfig}/${ANDROID_ABI}/libLudeoSDK-Android-${ANDROID_ABI}-${debugConfig}.so"

		IMPORTED_LOCATION_DEVELOPMENT "${CMAKE_CURRENT_LIST_DIR}/Bin/Android/Development/${ANDROID_ABI}/libLudeoSDK-Android-${ANDROID_ABI}-Development.so"
		IMPORTED_IMPLIB_DEVELOPMENT   "${CMAKE_CURRENT_LIST_DIR}/Lib/Android/Development/${ANDROID_ABI}/libLudeoSDK-Android-${ANDROID_ABI}-Development.so"
	)	
elseif(LINUX)
	message(FATAL_ERROR "Linux not supported yet")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	message(FATAL_ERROR "MacOSX not supported yet")
else()
	message(FATAL_ERROR "Unsupported platform")
endif()

#
# Setup Third Party dependencies
# To do this, we add custom properties to the target that we then can query later when setting up the POST_BUILD
#
if (WIN32)
	set(LUDEOSDK_THIRD_PARTY_DLLS_DEBUG       "${CMAKE_CURRENT_LIST_DIR}/ThirdParty/Win64/${debugConfig}/Bin")
	set(LUDEOSDK_THIRD_PARTY_DLLS_DEVELOPMENT "${CMAKE_CURRENT_LIST_DIR}/ThirdParty/Win64/Development/Bin")
	set(LUDEOSDK_THIRD_PARTY_DLLS_RELEASE     "${CMAKE_CURRENT_LIST_DIR}/ThirdParty/Win64/Release/Bin")
elseif(ANDROID)
	set(LUDEOSDK_THIRD_PARTY_DLLS_DEBUG       "${CMAKE_CURRENT_LIST_DIR}/ThirdParty/Android/Bin/${debugConfig}/${ANDROID_ABI}")
	set(LUDEOSDK_THIRD_PARTY_DLLS_DEVELOPMENT "${CMAKE_CURRENT_LIST_DIR}/ThirdParty/Android/Bin/Development/${ANDROID_ABI}")
	set(LUDEOSDK_THIRD_PARTY_DLLS_RELEASE     "${CMAKE_CURRENT_LIST_DIR}/ThirdParty/Android/Bin/Release/${ANDROID_ABI}")

elseif(LINUX)
	message(FATAL_ERROR "Linux not supported yet")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	message(FATAL_ERROR "MacOSX not supported yet")
else()
	message(FATAL_ERROR "Unsupported platform")
endif()

set_target_properties(LudeoSDK PROPERTIES
	_ludeo_THIRD_PARTY_DLLS_DEBUG "${LUDEOSDK_THIRD_PARTY_DLLS_DEBUG}"
	_ludeo_THIRD_PARTY_DLLS_DEVELOPMENT "${LUDEOSDK_THIRD_PARTY_DLLS_DEVELOPMENT}"
	_ludeo_THIRD_PARTY_DLLS_RELEASE "${LUDEOSDK_THIRD_PARTY_DLLS_RELEASE}"
)

#
# Given an executable target, it sets the POST_BUILD commands to copy the necessary LudeoSDK dependencies
#
function(ludeosdk_setupPostBuildCopy target)

	get_property(LUDEOSDK_THIRD_PARTY_DLLS_DEBUG TARGET LudeoSDK PROPERTY _ludeo_THIRD_PARTY_DLLS_DEBUG)
	get_property(LUDEOSDK_THIRD_PARTY_DLLS_DEVELOPMENT TARGET LudeoSDK PROPERTY _ludeo_THIRD_PARTY_DLLS_DEVELOPMENT)
	get_property(LUDEOSDK_THIRD_PARTY_DLLS_RELEASE TARGET LudeoSDK PROPERTY _ludeo_THIRD_PARTY_DLLS_RELEASE)

	#[===[
	message(STATUS "******* LUDEOSDK DEPENDENCIES START **************")
	message(STATUS "${LUDEOSDK_THIRD_PARTY_DLLS_DEBUG}")
	message(STATUS "***")
	message(STATUS "${LUDEOSDK_THIRD_PARTY_DLLS_DEVELOPMENT}")
	message(STATUS "***")
	message(STATUS "${LUDEOSDK_THIRD_PARTY_DLLS_RELEASE}")
	message(STATUS "******* LUDEOSDK DEPENDENCIES END **************")
	#]===]

	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
			$<$<CONFIG:Debug>:${LUDEOSDK_THIRD_PARTY_DLLS_DEBUG}>
			$<$<CONFIG:Development>:${LUDEOSDK_THIRD_PARTY_DLLS_DEVELOPMENT}>
			$<$<CONFIG:Release>:${LUDEOSDK_THIRD_PARTY_DLLS_RELEASE}>
			$<TARGET_FILE_DIR:${target}>
		COMMENT "Copying the LudeoSDK's third-party dlls to $<TARGET_FILE_DIR:${target}>"
	)

endfunction()

