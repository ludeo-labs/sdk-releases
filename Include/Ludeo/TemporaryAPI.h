// Copyright 2023 Ludeo. All rights reserved.

/**
 * Everything in this file is temporary or deprecated, and should not be used or show in the documentation
 * It will be removed as soon as possible
 */

#pragma once

// If running under Doxygen, don't process this file at all
#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "Ludeo/Common.h"
#include "Ludeo/SessionTypes.h"
#include "Ludeo/Utils.h"

// #RVF : We need to delete this file (And all the SDK code) related to these functions
// This is just to temporary to expose functions to the UE plugin

// Returns the auth token used to authenticate with the backend
LUDEO_DECLARE_FUNC(const char*) ludeo_Session_TEMP_GetAuthToken(LudeoHSession handle);

// Returns what is called by the backend as "gameSessionId". It's the Id given to the session when we log in
// Returns the string or an empty string on error
LUDEO_DECLARE_FUNC(const char*) ludeo_Session_TEMP_GetSessionId(LudeoHSession handle);

// Returns what the backend calls `gameId`.
// Returns the string, or empty string on error.
LUDEO_DECLARE_FUNC(const char*) ludeo_Session_TEMP_GetGameId(LudeoHSession handle);

// Returns what is called by the backend as "gameplayId"
// Returns the string or an empty string on error
LUDEO_DECLARE_FUNC(const char*) ludeo_Session_TEMP_GetGameplayId(LudeoHSession handle);

// Returns the URL of the last highlight or an empty string on error
LUDEO_DECLARE_FUNC(const char*) ludeo_Session_TEMP_GetLastHighlightUrl(LudeoHSession handle);

// Returns the Id of the last highlight or an empty string on error
LUDEO_DECLARE_FUNC(const char*) ludeo_Session_TEMP_GetLastHighlightId(LudeoHSession handle);

// Acknowledges that the video state change was done.
// contextId should be set to the string received in the videoState callback
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_Session_TEMP_AckVideoStateChange(LudeoHSession handle, const char* contextId);

//
// Temporary notification for highlight.
//

/** The most recent version of the ludeo_Session_AddNotifyResumeGameRequest API */
#define LUDEO_SESSION_ADDNOTIFYHIGHLIGHT_API_LATEST 1

/** The parameters to ludeo_Session_AddNotifyHighlight */
LUDEO_STRUCT_START(LudeoSessionAddNotifyHighlightParams)
	/** Set this to LUDEO_SESSION_ADDNOTIFYHIGHLIGHT_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoSessionAddNotifyHighlightParams);

/**
 * Structure with the parameters passed to the Highlight callback
 */
LUDEO_STRUCT_START(LudeoSessionHighlightCallbackParams)
	void *clientData;
	const char* highlightUrl;
	const char* highlightId;
LUDEO_STRUCT_END(LudeoSessionHighlightCallbackParams);

LUDEO_DECLARE_CALLBACK(LudeoSessionHighlightCallback, const LudeoSessionHighlightCallbackParams* data);

LUDEO_DECLARE_FUNC(LudeoNotificationId) ludeo_Session_AddNotifyHighlight(
	LudeoHSession handle,
	const LudeoSessionAddNotifyHighlightParams* params,
	void* clientData,
	LudeoSessionHighlightCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_ADDNOTIFY(Session, SESSION, Highlight, HIGHLIGHT);

//
// Temporary notification for video enable/disable notifications
//

/** The most recent version of the ludeo_Session_AddNotifyVideoState API */
#define LUDEO_SESSION_ADDNOTIFYVIDEOSTATE_API_LATEST 1

/** The parameters to ludeo_Session_AddNotifyHighlight */
LUDEO_STRUCT_START(LudeoSessionAddNotifyVideoStateParams)
	/** Set this to LUDEO_SESSION_ADDNOTIFYVIDEOSTATE_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoSessionAddNotifyVideoStateParams);

/**
 * Structure with the parameters passed to the VideoState callback
 */
LUDEO_STRUCT_START(LudeoSessionVideoStateCallbackParams)
	void *clientData;

	// Set to 1 if video is enabled. 0 if disabled
	LudeoBool videoEnabled;

	// Maximum Highlight duration in seconds
	int32_t maxHighlightDuration;

	//
	const char* apiUpdateUploadUrl;

	// If this is not an empty string, the SDK will be waiting on a call to ludeo_Session_TEMP_VideoStateAck with this value
	const char* contextId;
LUDEO_STRUCT_END(LudeoSessionVideoStateCallbackParams);

LUDEO_DECLARE_CALLBACK(LudeoSessionVideoStateCallback, const LudeoSessionVideoStateCallbackParams* data);


/**
 * Registers to receive a notification when the user specifies if they want to allow video or not
 *
 * When the client gets this notification, it should make a call to ludeo_Session_TEMP_VideoStateAck and pass the "contextId"
 * after applying any required changes.
 */
LUDEO_DECLARE_FUNC(LudeoNotificationId) ludeo_Session_AddNotifyVideoState(
	LudeoHSession handle,
	const LudeoSessionAddNotifyVideoStateParams* params,
	void* clientData,
	LudeoSessionVideoStateCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_ADDNOTIFY(Session, SESSION, VideoState, VIDEOSTATE);

//////////////////////////////////////////////////////////////////////////
// MarkHighlight
//////////////////////////////////////////////////////////////////////////

/** The most recent version of the ludeo_Session_Markhighlight API */
#define LUDEO_SESSION_MARKHIGHLIGHT_API_LATEST 1

/** Structure with the parameters to ludeo_Session_MarkHighlight */
LUDEO_STRUCT_START(LudeoSessionMarkHighlightParams)
	/** Set this to LUDEO_SESSION_MARKHIGHLIGHT_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoSessionMarkHighlightParams);

/** Parameters to the ludeo_Session_MarkHighlight callback */
LUDEO_STRUCT_START(LudeoSessionMarkHighlightCallbackParams)
	/** Tells if the operation succeeded or failed. */
	LudeoResult resultCode;

	/** Context that was passed to the originating SDK call */
	void *clientData;
LUDEO_STRUCT_END(LudeoSessionMarkHighlightCallbackParams);

/** */
LUDEO_DECLARE_CALLBACK(LudeoSessionMarkHighlightCallback, const LudeoSessionMarkHighlightCallbackParams* data);

/**
 * Marks an highlight
 *
 * This is a valid call only when the session is in creator mode. If called when the Session is not in valid state to receive this
 * call, the callback will return a LudeoResult::WrongState error code.
 *
 * The client should not make another call ludeo_Session_MarkHighlight until the previous one completes.
 *
 * @param handle
 *		The interface handle.
 * 
 * @param params
 *		Parameters to the function.
 * 
 * @param clientData
 *		Context to be passed to the callback
 *
 * @param callback
 *		Callback to call once the operation completes
 */
LUDEO_DECLARE_FUNC(void) ludeo_Session_MarkHighlight(LudeoHSession handle, const LudeoSessionMarkHighlightParams* params, void* clientData, LudeoSessionMarkHighlightCallback callback );
LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_2(Session, SESSION, MarkHighlight, MARKHIGHLIGHT);


// #ROOMS : Remove all these inside doxygen REMOVE group
//////////////////////////////////////////////////////////////////////////
// Things that are not used at all at the moment, not even Legacy
//////////////////////////////////////////////////////////////////////////

/** \defgroup REMOVE
 * @{
 */

/**
 * The different modes a Session instance can be in
 */
 LUDEO_ENUM_START(LudeoSessionMode)

	// #LC3630 These values will probably have to change to be None, Idle, Running or something like that.

	/** The Session is currently just waiting */
	LUDEO_ENUM_VALUE(LudeoSessionMode, None, 0)

	/** The Session is in Creator mode */
	LUDEO_ENUM_VALUE(LudeoSessionMode, Creator, 1)

	/** The Session is in Player mode */
	LUDEO_ENUM_VALUE_LAST(LudeoSessionMode, Player, 2)

LUDEO_ENUM_END(LudeoSessionMode);


LUDEO_IMPL_ENUM_HELPERS(SessionMode)

//////////////////////////////////////////////////////////////////////////
// BeginCreator
//////////////////////////////////////////////////////////////////////////

/** The most recent version of the #ludeo_Session_BeginCreator API */
#define LUDEO_SESSION_BEGINCREATOR_API_LATEST 1

/** Structure with the parameters to ludeo_Session_BeginCreator */
LUDEO_STRUCT_START(LudeoSessionBeginCreatorParams)
	/** Set this to LUDEO_SESSION_BEGINCREATOR_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoSessionBeginCreatorParams);

/** The parameters to the ludeo_Session_BeginCreator callback */
LUDEO_STRUCT_START(LudeoSessionBeginCreatorCallbackParams)
	/** Tells if the operation succeeded or failed. */
	LudeoResult resultCode;

	/** Context that was passed to the originating SDK call */
	void *clientData;

	// #RVF : Add better documentation here, explaining the lifetime of the handle
	/**
	 * Handle to the DataWriter instance to use during the creator run.
	 * This handle is only valid during this specific creator run.
	 */
	LudeoHDataWriter dataWriter;
LUDEO_STRUCT_END(LudeoSessionBeginCreatorCallbackParams);

/** */
LUDEO_DECLARE_CALLBACK(LudeoSessionBeginCreatorCallback, const LudeoSessionBeginCreatorCallbackParams* data);

/**
 * Signals the SDK to start the session as a creator
 * Once the operation completes successfully, the client can then start feeding data to the SDK trough the supplied DataReader
 * instance.
 *
 * To stop the creator mode, a call to ludeo_Session_EndRun should be made.
 *
 * The client should avoid making calls to ludeo_Session_BeginCreator, ludeo_Session_BeginLudeo or ludeo_Session_EndRun while
 * any of those is still in progress.
 * Doing so will fail with a LudeoResult::WrongState because the SDK knows there is one in progresss, but because that call will
 * fail straight away, the callback will be called BEFORE the callback of the first call (which is still in progress). This can be
 * confusing for the client to handle.
 *
 * @param handle
 *		The interface handle
 *
 * @param params
 *		Parameters to the function.
 *
 * @param clientData
 *		Context to be passed to the callback 
 *
 * @param callback
 *		Completion callback. If the resultCode is LudeoResult::Success, then the client can start feeding data to the
 *        SDK.
 */
LUDEO_DECLARE_FUNC(void) ludeo_Session_BeginCreator(LudeoHSession handle, const LudeoSessionBeginCreatorParams* params, void *clientData, LudeoSessionBeginCreatorCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_2(Session, SESSION, BeginCreator, BEGINCREATOR);

//////////////////////////////////////////////////////////////////////////
// BeginLudeo
//////////////////////////////////////////////////////////////////////////

/** The most recent version of the ludeo_Session_BeginLudeo API */
#define LUDEO_SESSION_BEGINLUDEO_API_LATEST 1

/** Structure with the parameters to ludeo_Session_BeginLudeo */
LUDEO_STRUCT_START(LudeoSessionBeginLudeoParams)
	/** Set this to LUDEO_SESSION_BEGINLUDEO_API_LATEST */
	int32_t apiVersion;

	/** Ludeo Id to begin playing */
	const char* ludeoId;
LUDEO_STRUCT_END(LudeoSessionBeginLudeoParams);

/** Parameters to the ludeo_Session_BeginLudeo callback */
LUDEO_STRUCT_START(LudeoSessionBeginLudeoCallbackParams)
	/** Tells if the operation succeeded or failed. */
	LudeoResult resultCode;

	/** Context that was passed to the originating SDK call */
	void *clientData;

	/**
	 * Handle to the DataWriter instance to use during this ludeo run.
	 * This handle is only valid during this run.
	 */
	LudeoHDataWriter dataWriter;
LUDEO_STRUCT_END(LudeoSessionBeginLudeoCallbackParams);

/** */
LUDEO_DECLARE_CALLBACK(LudeoSessionBeginLudeoCallback, const LudeoSessionBeginLudeoCallbackParams* data);

/**
 * Signals the SDK to start the session as a ludeo player
 *
 * Prior to calling this, the client should set the game to the state required by this specific ludeo.
 * That can be done by using the function ludeo_Session_GetLudeo, which will give a DataReader instance that can be used to
 * retrieve the game state.
 *
 * Starting a Ludeo is a two step process (ludeo_Session_BeginLudeo and ludeo_Session_BeginGameplay)
 * If the call to this function succeeds, the client should then do the following:
 * - Use the provided DataWriter instance to write any initial state
 * - Call ludeo_Session_BeginGameplay and start the actual gameplay if that call succeeds
 * - Write any data to the DataWriter instance during gameplay
 * 
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Parameters to the function.
 *
 * @param clientData
 *		Context to be passed to the callback 
 *
 * @param callback
 *		Callback called once the SDK and backend are ready to play the ludeo
 */
LUDEO_DECLARE_FUNC(void) ludeo_Session_BeginLudeo(LudeoHSession handle, const LudeoSessionBeginLudeoParams* params, void *clientData, LudeoSessionBeginLudeoCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_2(Session, SESSION, BeginLudeo, BEGINLUDEO);


//////////////////////////////////////////////////////////////////////////
// BeginGameplay
//////////////////////////////////////////////////////////////////////////

/** The most recent version of the ludeo_Session_BeginGameplay API */
#define LUDEO_SESSION_BEGINGAMEPLAY_API_LATEST 1

/** Structure with the parameters to ludeo_Session_BeginGameplay */
LUDEO_STRUCT_START(LudeoSessionBeginGameplayParams)
	/** Set this to LUDEO_SESSION_BEGINGAMEPLAY_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoSessionBeginGameplayParams);

/** Parameters to the ludeo_Session_BeginGameplay callback */
LUDEO_STRUCT_START(LudeoSessionBeginGameplayCallbackParams)
	/** Tells if the operation succeeded or failed. */
	LudeoResult resultCode;

	/** Context that was passed to the originating SDK call */
	void *clientData;
LUDEO_STRUCT_END(LudeoSessionBeginGameplayCallbackParams);

LUDEO_DECLARE_CALLBACK(LudeoSessionBeginGameplayCallback, const LudeoSessionBeginGameplayCallbackParams* data);

/**
 * Signals the SDK to start actual gameplay
 * 
 * Starting a Creator or Ludeo Player run is a two step process.
 * The client calls either ludeo_Session_BeginCreator or ludeo_Sessoin_BeginLudeo, and if that call completes successfully, 
 * it should then call this function.
 *
 * @param handle Session handle
 * @param params Structure with the function's parameters
 * @param clientData Context to be passed to the callback 
 * @param callback Callback called once the SDK and backend are expecting the client to start actual gameplay.
 */
LUDEO_DECLARE_FUNC(void) ludeo_Session_BeginGameplay(LudeoHSession handle, const LudeoSessionBeginGameplayParams* params, void *clientData, LudeoSessionBeginGameplayCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_2(Session, SESSION, BeginGameplay, BEGINGAMEPLAY);

//////////////////////////////////////////////////////////////////////////
// EndRum
//////////////////////////////////////////////////////////////////////////

/** The most recent version of the ludeo_Session_EndRun API */
#define LUDEO_SESSION_ENDRUN_API_LATEST 1

/** Structure with the parameters to ludeo_Session_EndRun */
LUDEO_STRUCT_START(LudeoSessionEndRunParams)
	/** Set this to LUDEO_SESSION_ENDRUN_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoSessionEndRunParams);

/** Parameters to the ludeo_Session_EndRun callback */
LUDEO_STRUCT_START(LudeoSessionEndRunCallbackParams)
	/** Tells if the operation succeeded or failed. */
	LudeoResult resultCode;

	/** Context that was passed to the originating SDK call */
	void *clientData;
LUDEO_STRUCT_END(LudeoSessionEndRunCallbackParams);

/** */
LUDEO_DECLARE_CALLBACK(LudeoSessionEndRunCallback, const LudeoSessionEndRunCallbackParams* data);

/**
 * Ends a Creator or Ludeo player run
 *
 * This is a valid call only after a call to ludeo_Session_BeginCreator or ludeo_Session_BeginLudeo.
 * The error LudeoResult::WrongState is returned in the callback if called when in an invalid state.
 *
 * The client should avoid making calls to ludeo_Session_BeginCreator, ludeo_Session_BeginLudeo or ludeo_Session_EndRun while
 * any of those is still in progress.
 * Doing so will fail with a LudeoResult::WrongState because the SDK knows there is one in progress, but because that call will
 * fail straight away, the callback will be called BEFORE the callback of the first call (which is still in progress). This can be
 * confusing for the client to handle.
 *
 * @param handle Session handle
 * @param params Structure with the function's parameters
 * @param clientData Context to be passed to the callback
 * @param callback Callback to call once the operation completes
 */
LUDEO_DECLARE_FUNC(void) ludeo_Session_EndRun(LudeoHSession handle, const LudeoSessionEndRunParams* params, void* clientData, LudeoSessionEndRunCallback callback );
LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_2(Session, SESSION, EndRun, ENDRUN);

//////////////////////////////////////////////////////////////////////////
// GetMode
//////////////////////////////////////////////////////////////////////////

/**
 * Returns the mode the session is currently in.
 *
 * Note that calling this right after ludeo_Session_BeginCreator or ludeo_SessionBeginLudeo will return LudeoSession::None.
 * A session is only considered as switched to Creator or Player once the operation finishes successfully.
 *
 * The client should avoid making calls to ludeo_Session_BeginCreator, ludeo_Session_BeginLudeo or ludeo_Session_EndRun while
 * any of those is still in progress.
 * Doing so will fail with a LudeoResult::WrongState because the SDK knows there is one in progress, but because that call will
 * fail straight away, the callback will be called BEFORE the callback of the first call (which is still in progress). This can be
 * confusing for the client to handle.
 *
 * @param handle Session handle
 * @return What mode the session is in. If any error occurred (E.g: Invalid handle or SDK not initialized), it will return
 * LudeoSessionMode::None
 */
LUDEO_DECLARE_FUNC(LudeoSessionMode) ludeo_Session_GetMode(LudeoHSession handle);


/**
 * @}
 */


#endif // DOXYGEN_SHOULD_SKIP_THIS

