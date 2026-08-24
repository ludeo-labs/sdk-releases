// Copyright 2023 Ludeo. All rights reserved.

/**
 * @file
 * Functions not specific to any interface.
 * 
 * This includes things such as initializing and shutting down the SDK, logging, etc.
 */

#pragma once

#include "InitTypes.h"
#include "Common.h"

/** \addtogroup Root
 * @{
 *
 * Functions and types that don't belong to any specific interface.
 */

/** \addtogroup ludeo_Initialize
 * @{
 */

/**
 * Initializes the SDK.
 * 
 * Clients must call this function before calling any other SDK functions.
 * It should be called just once, and with a matching #ludeo_Shutdown call
 *
 * @param params Structure containing the parameters
 * @return A LudeoResult to indicate success or failure
 * 
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_Initialize(const LudeoInitializeParams* params);
LUDEO_DEFINE_FUNC_TRAITS_1(Initialize, INITIALIZE);

/**  ludeo_Initialize END
 * @}
 */

/** \addtogroup ludeo_Shutdown
 * @{
 */

/**
 * Shuts down the SDK
 *
 * After calling this, clients should not call any other SDK functions with the exception of ludeo_ResultToString.
 *
 * If there are any pending tasks (internal or externally triggered), the SDK will tick itself for a few seconds to attempt
 * gracefully finishing those tasks. If after that time window there are tasks pending, it will forcibly cancel them.
 * Therefore, any async calls the client started but haven't yet completed can have their callbacks called as a result of a 
 * ludeo_Shutdown call.
 *
 * The client should use the clientData context provided for those callbacks to determine if it needs to acts on those callbacks
 * or if it can just ignore them.
 *
 * @return A LudeoResult to indicate success or failure
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_Shutdown(void);

/** ludeo_Shutdown END
 * @}
 */

/**
 * Returns a string representation of a LudeoResult.
 *
 * The returned string is never null and should not be freed.
 */
LUDEO_DECLARE_FUNC(const char*) ludeo_ResultToString(LudeoResult result);

/**
 * Returns the SDK's tick timestamp as per the last #ludeo_Tick call.
 * The value is undefined if the SDK was not initialized or #ludeo_Tick was not yet called.
 * Internally the SDK uses a timestamp that is updated with each #ludeo_Tick call. This function returns that value.
 * One of the scenarios where the client can make use of this is with the #ludeo_DataWriter_EnterObjectLate function. 
 * Refer to that function's documentation for more information.
 * 
 * @warning Although at the moment this timestamp represents the Unix Epoch Time of the last #ludeo_Tick call, the client should not make that
 * assumption. Any use of this function should be purely to cache a timestamp that needs to eventually be passed back to the SDK.
 * 
 * @return The SDK's tick timestamp
 */
LUDEO_DECLARE_FUNC(uint64_t) ludeo_GetCurrentTimestamp(void);

/** \addtogroup Logging
 * @{
 */

/**
 * Sets the logging callback function.
 * Any logging done by the SDK will be forwarded to the specified function.
 * Avoid expensive operations from the callback, such as directly logging to a file.
 *
 * IMPORTANT: This callback can be called from any of the SDK's threads.
 *
 * @param callback Callback to use for logging. This replaces any previously set callback. If nulled, it will remove any
 *		previously set callback.
 * @return A LudeoResult to indicate success or failure
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_SetLoggingCallback(LudeoLogCallback callback);

/**
 * Enables or disables logging to a file.
 *
 * The SDK can automatically log to a file to simplify development, but that is disabled by default.
 * This function allows enabling or disabling that feature.
 *
 * If a file named LudeoSDK.log already exists, the SDK will attempt to rename that existing file by adding the last write
 * UTC time of that file to its name. If for any reason the rename fails (e.g: OS related permissions), LudeoResult::Unknown is 
 * returned.
 *
 * @param params Structure containing the parameters
 * @return A LudeoResult to indicate success or failure.
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_SetLoggingToFile(const LudeoSetLoggingToFileParams* params);
LUDEO_DEFINE_FUNC_TRAITS_1(SetLoggingToFile, SETLOGGINGTOFILE);

/**
 * Sets the maximum log level for the a log category
 * By default all categories will log at the LudeoLogLevel::Log level or bellow, which means Verbose
 * or VeryVerbose levels will not show up in the logs.
 * 
 * Although the client can filter logs in its logging callback, it is recommended to use this function instead.
 * This is because an unneeded log message at the callback level means wasted performance formatting the message, while
 * a log message filtered with this function will cause the message formatting to be skipped altogether.
 * 
 * @param category Log category to change . The special value LudeoLogCategory::All can be specified to set all categories
 * @param level Maximum log level to report
 * @return
 *		A LudeoResult to indicate success or failure. The only reason the function can fail is if the category value passed is
 *		an invalid enum value.
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_SetLoggingLevel(LudeoLogCategory category, LudeoLogLevel level);

/**
 * @}
 */

/**
 * Allows the SDK to do work.
 * This needs to be called periodically, ideally once per game tick.
 * 
 * Any callbacks are called only from this function. This allows the client to control when it can receive callbacks, providing
 * a well defined threading model.
 *
 * If the SDK has not been initialized, this does nothing.
 */
LUDEO_DECLARE_FUNC(void) ludeo_Tick(void);

/**
 * Removes a notification that the client added with any of the ludeo_AddNotifyXXXXX methods and ludeo_<INTERFACE>_AddNotifyXXXXX
 * methods; If the notification id has a value of LUDEO_INVALID_NOTIFICATIONID, then the call is a no-op.
 */
LUDEO_DECLARE_FUNC(void) ludeo_RemoveNotification(LudeoNotificationId notificationId);

//////////////////////////////////////////////////////////////////////////
//                    Global Notifications
//////////////////////////////////////////////////////////////////////////

/** \addtogroup ludeo_AddNotifyOOBInfo
 * @{
 */

/**
 * Registers to receive a notification when an out-of-band error occurs.
 * For example: For HTTP requests that are triggered as part of the public API,
 * the game knows the result through the completion callback, but the SDK has other internal HTTP requests (and websocket
 * messages) that can occur at any time. If these fail, progression cannot continue.
 *
 * Possible return values:
 *		NetworkError: This is coming when HTTP or Websocket communication fails. This notification only gets triggered once between ludeo_Initialize and ludeo_Shutdown, even if multiple events would trigger it
 * 
 * @param params
 *		Parameters to the function.
 *
 * @param clientData
 *		Context to be passed to the callback.
 *
 * @param callback
 *		Callback to call when out of band error happens
 *
 * @return
 * The notification ID. The client should call ludeo_RemoveNotification whenever it wants to stop receiving the notification.
 * If any of the parameters are invalid and the call fails, it returns LUDEO_INVALID_NOTIFICATIONID.
 *
 */
 LUDEO_DECLARE_FUNC(LudeoNotificationId) ludeo_AddNotifyOOBInfo(const LudeoAddNotifyOOBInfoParams* params, void* clientData, LudeoOOBInfoCallback callback); 
 LUDEO_DEFINE_FUNC_TRAITS_ADDNOTIFY_NO_INTERFACE(OOBInfo, OOBINFO);

/**
 * @}
 */


/**
 * 
 * This function should **NOT** be used for shipping.
 * It allows tweaking some of the SDK internals to help debug issues.
 * A client should not rely on any of these command to be available with newer versions of the SDK.
 * Also, the SDK makes no effort to make sure commands are compatible with eachother or that the client passed values in the
 * expected range. This is intentional, so it doesn't limit whatever experimentation the client wants to do.
 * 
 * Available commands:
 * 
 * `overlay-enabled`
 *	* **Description**: Allows the client to keep the overlay from initializing. Disabling the overlay means you can't have an actual ludeo
 *		experience, but it can be useful to diagnose problems.
 *		This should be called before ludeo_Initialize. Once the overlay is enabled it can't be disabled.<br>
 *	* **Value**:
 *		* `"0"` - Overlay will be disabled.
 *		* `"1"` (default) - Overlay will be enabled.
 *	* **Example:**
 *		* `ludeo_Command("overlay-enabled", "0");`
 * 
 * `monitor-enabled`
 *	* **Description**:
 *		The SDK monitors some performance related stats such as FPS and frametime variance and sends them to the backend at
 *		regular intervals.
 *		This command allows disabling monitor. This can be useful during development to disable any unnecessary outgoing
 *		http requests and to further reduce logging noise. It should be called before ludeo_Initialize.
 *	* **Value**:
 *		* `"0"` - Disable monitoring.
 *		* `"1"` - (default) Doesn't do anything, since monitoring is only enabled if the backend tells the SDK to enable it.
 *
 * `backendlogs-enabled`
 *	* **Description**:
 *		Depending on how the game was configured on the platform, logs might be being sent to the backend. When configured that
 *		way, it means that it will be overriding the verbosity the client tries to set.
 *		This command allows disabling that upload feature to make development easier, since during development you most likely
 *		don't want to be uploading logs.
 *		This should be called BEFORE activating a session.
 *	* **Value**:
 *		* `"0"` - Disable log uploading.
 *		* `"1"` - (default) Doesn't do anything. As-in, if the backend wants logs, it will send them, but it doesn't enable
 *		          uploading by itself
 *
 * `video-enabled`
 *	* **Description**:
 *		Allows the client to disable recording. This can be used during debug. This should be called before ludeo_Initialize.
 *	* **Value**:
 *		* `"0"` - Video recording will be disabled
 *		* `"1"` (default) - Video recording will be enabled
 *	* **Example**:
 *		* `ludeo_Command("video-enabled", "0");`
 *
 * `backend-initiated-moment-capture`
 *	* **Description**:
 *		Opts the SDK into AI mode. When enabled, `capture_moment` events
 *		received over the session WebSocket will trigger a highlight capture
 *		via the standard markHighlight flow (equivalent to a manual F9 press).
 *		Can be called at any time.
 *
 *		AI mode can also be enabled without source-level changes by setting
 *		the `LUDEO_BACKEND_INITIATED_MOMENT_CAPTURE` environment variable
 *		(or passing `-ludeoBackendInitiatedMomentCapture` on the command line)
 *		to `"1"` before launching the game. The SDK reads it at session
 *		activation. A subsequent `ludeo_Command` call overrides it (last
 *		writer wins).
 *
 *		In addition, the backend can control AI mode per game: when the game
 *		configuration (fetched during session activation) carries a boolean
 *		`game.highlightConfig.autoCapture` field, that value takes effect
 *		for the session, overriding whatever was set locally. When the field
 *		is absent, the locally configured value (command / environment
 *		variable / command line) is kept unchanged.
 *	* **Value**:
 *		* `"0"` (default) - SDK ignores `capture_moment` events.
 *		* `"1"` - SDK acts on `capture_moment` events.
 *	* **Example:**
 *		* `ludeo_Command("backend-initiated-moment-capture", "1");`
 *		* `set LUDEO_BACKEND_INITIATED_MOMENT_CAPTURE=1` (operator-side, before launch)
 * 
 *	`video-upload-enabled`
 *	* **Description**:
 *		Allows the client to disable video uploading. This can be used during debugging/development if you wish to skip
 *		the upload for whatever reason (e.g: internet speed reasons)
 *	* **Value**:
 *		* `"0"` - Video uploading will be disabled.
 *		* `"1"` (default) - Video uploading is enabled
 *
 *	`video-localsave`
 *	* **Description**:
 *		If set to true, it will cause videos to be saved locally as a raw h264 file before uploading. The videos will be saved
 *		as `LudeoVideoMessage-<UTC TIMESTAMP>.h264` files in the current working directory.
 *		Note that this should ONLY be used for debugging.
 *		This can be called at any time, and the change will affect any videos created after the command.
 *	* **Value**:
 *		* `"0"` - (default) Videos are not saved to files
 *		* `"1"` - Videos are saved to files before uploading
 *
 *	`video-swencoder`
 *	* **Description**:
 *		Typically, the SDK tries to use any supported hardware encoders, and the software encoder is the fallback encoder.
 *		Setting this to true forces the use of the software encoder.
 *		It should be called before ludeo_Initialize
 *	* **Value**:
 *		* `"0"` - (default) Will use an hardware encoder if available
 *		* `"1"` - Will use the software encoder.
 *
 * `session-forcebetacode`
 *	* **Description**:
 *		Forces any Session activation to use this specific beta code.
 *		Can be used at any moment. Any subsequent ludeo_Session_Activate will then use it.
 *	* **Value**:
 *		* A string with the Beta name.
 * 
 * `activation-ludeoid`
 *	* **Description**:
 *		Allows to set a Ludeo ID to be used right after session activation, by invoking the LudeoSelected notification with the given ID.
 *		This can be useful for debugging the ludeo notification on activation flow.
 *		This command can be called at any time, and any subsequent ludeo_Session_Activate will then use it.
 *	* **Value**:
 *		* A string with the Ludeo ID.
 *
 * `start-heartbeatdetector`
 *	* **Description**:
 *		* **WARNING**: Only implemented on Windows.
 *		* Start an heartbeat detector in a separate thread, which regularly checks if ludeo_Tick is being called. As soon as it
 *			detects that ludeo_Tick hasn't been called for N seconds, it creates a full dump file of the process.
 *			Once enabled, it can't be disabled. Also, it only triggers the dump once (on the first timeout)
 *	* **Value**:
 *		* A positive integer number. 0 will leave it disabled (default)
 *
 * `profiler-sendlogs`
 *	* **Description**:
 *		Allows controlling if logs are sent to the profiler when using an instrumented build.
 *	* **Value**:
 *		* `"0"` - Logs are not sent to the profiler.
 *		* `"1"` - (default) Logs are sent to the profiler.
 *
 * `data-attributes_per_request`
 *	* **Description**:
 *		Sets the maximum number of new attributes that can be registered with a single request to the backend.
 *		Internally, when the client requires to register more than this, it will split it into several requests.
 *	* **Value**: 0 to N.
 *		Altough it it doesn't make practical sense to use very small values (we recommend values higher than 500), the SDK doesn't
 *		enforce a minimum value, since `ludeo_Command` is mostly a tool for experimentation/development. Therefore, even a value
 *		of `0` is allowed.
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_Command(const char* name, const char* value);


// General - END 
/**
 * @}
 */

