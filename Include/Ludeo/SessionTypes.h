// Copyright 2023 Ludeo. All rights reserved.

/**
 * @file
 * Types used by the Session interface
 */

#pragma once

#include "Ludeo/Common.h"
#include "Ludeo/RoomTypes.h"
#include "Ludeo/DataWriterTypes.h"
#include "Ludeo/DataReaderTypes.h"

#pragma pack(push, 8)

/** \addtogroup Session
 * @{
 */

/**
 * Session handle.
 * All Session functions take a session handle. It represents the session instance.
 * To create a session handle, use #ludeo_Session_Create
 */
EXTERN_C typedef struct LudeoSessionHandle* LudeoHSession;

/** \addtogroup ludeo_Session_Create
 * @{
 */

/** The most recent version of the #ludeo_Session_Create API */
#define LUDEO_SESSION_CREATE_API_LATEST 1

/**
 * Parameters for #ludeo_Session_Create
 */
LUDEO_STRUCT_START(LudeoSessionCreateParams)
	/** Set this to LUDEO_SESSION_CREATE_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoSessionCreateParams);

/**
 * @}
 *
 * \addtogroup ludeo_Session_Release
 * @{
 */

/** The most recent version of the #ludeo_Session_Release API */
#define LUDEO_SESSION_RELEASE_API_LATEST 1

/**
 * Parameters for #ludeo_Session_Release
 */
LUDEO_STRUCT_START(LudeoSessionReleaseParams)
	/** Set this to LUDEO_SESSION_RELEASE_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoSessionReleaseParams);

/** Parameters for the #ludeo_Session_Release callback */
LUDEO_STRUCT_START(LudeoSessionReleaseCallbackParams)
	/** Tells if the operation succeeded or failed. */
	LudeoResult resultCode;

	/** Context that was passed to the originating SDK call */
	void *clientData;
LUDEO_STRUCT_END(LudeoSessionReleaseCallbackParams);

/** Signature for the #ludeo_Session_Release callback */
LUDEO_DECLARE_CALLBACK(LudeoSessionReleaseCallback, const LudeoSessionReleaseCallbackParams* data);

/**
 * @}
 *
 * \addtogroup ludeo_Session_Activate
 * @{
 */

/**
 * User authentication type to use
 */
LUDEO_ENUM_START(LudeoAuthType)
	/**
	 * Explicit authentication done with Steam user details
	 */
	LUDEO_ENUM_VALUE(LudeoAuthType, Steam, 0)

	/**
	 * Explicit authentication done with Epic Games user details
	 *
	 * NOTE: Not yet implemented
	 */
	LUDEO_ENUM_VALUE(LudeoAuthType, Epic, 1)

	/**
	 * Authentication with GeForce NOW
	 * 
	 * This is used only internally. At the moment there is nothing for the client to specify explicitly. The SDK detects if
	 * running in the GeForceNow environment.
	 */
	LUDEO_ENUM_VALUE(LudeoAuthType, Nvidia, 2)

	/**
	 * Authentication with Cloud
	 *
	 * This is used only internally. At the moment there is nothing for the client to specify explicitly. The SDK detects if
	 * running in the Cloud environment.
	 */
	LUDEO_ENUM_VALUE_LAST(LudeoAuthType, GenericCloud, 3)

LUDEO_ENUM_END(LudeoAuthType);

/**
 * Structure with authentication details for ludeo_Session_Create when explicit authentication with Steam is wanted.
 */
LUDEO_STRUCT_START(LudeoSteamAuthDetails)
	/** Needs to be set to LudeoAuthType::Steam */
	LudeoAuthType authType;

	/**
	 * Steam user ID.
	 * You can retrieve this with Steamworks's SteamUser()->GetSteamID()
	 */
	const char* authId;

	 /**
	  * Steam beta branch.
	  * You can retrieve this with Steamworks's SteamApps()->GetCurrentBetaName()
	  * This is optional. If nulled or an empty string, then it's not a beta branch
	  */
	const char* currentBetaName;
LUDEO_STRUCT_END(LudeoSteamAuthDetails);

/** The most recent version of ludeo_Session_Activate API */
#define LUDEO_SESSION_ACTIVATE_API_LATEST 3

// Forward declare this because it's only defined after SessionActivate and SessionActivate uses it
LUDEO_FORWARDDECLARE_STRUCT(LudeoSessionSetLocalizationParams);

/**
 * Parameters for #ludeo_Session_Activate
 */
LUDEO_STRUCT_START(LudeoSessionActivateParams)

	/** Set this to LUDEO_SESSION_ACTIVATE_API_LATEST */
	int32_t apiVersion;

	/**
	 * Ludeo platform's url to use. This can be for example https://services.ludeo.com
	 */
	const char* platformUrl;

	/**
	 * Pointer to one of the Ludeo<NAME>AuthDetails structs or nulled.
	 *
	 * If nulled, the SDK will try to automatically deduce authentication based on the current platform/environment.
	 * For example, on Platforms that support Steam, the SDK will check if the Steamworks dll is loaded and attempts to retrieve
	 * the user information. 
	 * Note that the SDK doesn't initialize Steamworks (or any other supported provider). That's the responsibility of the client.
	 * If the client didn't initialize any of the supported providers (e.g: Steamworks), then authentication will fail with
	 * LudeoResult::InvalidAuth.
	 * 
	 * Explicit authentication details allows working around any unforeseen problems with either the SDK or client design, if
	 * for some reason the SDK can't retrieve the required information.
	 * Also useful during development or for testing scenarios where e.g Steam might not be running.
	 */
	void* authDetails;

	/** Api key to access Ludeo services
	 * This is dependent on the url specified in platformUrl
	 */
	const char* apiKey;

	/**
	 * Backend app token to activate if available.
	 * It can be nulled.
	 * 
	 * Normally, this will cause the backend to signal the SDK that it should play a Ludeo, which will cause the ludeoSelected 
	 * field in the callback parameters struct will be set to true, and a LudeoSelected notification will occur.
	 */
	const char* appToken;

	/**
	 * The client's window handle (Native OS handle) that should be used for the SDK's overlays and video recording.
	 * It can be nulled. If this is null, overlay and video recording will be disabled  
	 */
	void* windowHandle;

	/**
	 * If true, it will reset the attributes and actions the backend has for the game.
	 * This should only be used during early development.
	 *
	 * This functionality is only available for some environment, to keep the client from accidentally reseting a live game.
	 *
	 * @warning Use this with care, as it will invalidate all existing ludeos and some other settings. If unsure, contact us first.
	 */
	LudeoBool reset;

	/**
	 * Localization settings to apply.
	 *
	 * This can be nulled, in which case the SDK will use default settings (english).
	 *
	 * This does exactly the same thing as calling #ludeo_Session_SetLocalization. The reason it can also be provided as part of
	 * the Session Activation is so the Overlay starts with these settings.
	 */
	 LudeoSessionSetLocalizationParams* localization;

	 /**
	  * The game version string.
	  * 
	  * Can not be nulled or an empty string if apiVersion is 3 or higher.
	  * 
	  * Used to identify for each ludeo what are the compatible game versions that can play it.
	  */
	 const char* gameVersion;

LUDEO_STRUCT_END(LudeoSessionActivateParams);

/** Parameters for the #ludeo_Session_Activate callback */
LUDEO_STRUCT_START(LudeoSessionActivateCallbackParams)
	/** Tells if the operation succeeded or failed. */
	LudeoResult resultCode;

	/** Context that was passed to the originating SDK call */
	void *clientData;

	/**
	 * If as part of the session activation the SDK detected that a ludeo should be played, this is set to true.
	 * When this is true, the SDK guarantees that soon after this callback, a LudeoSelected notification is received.
	 */
	LudeoBool ludeoSelected;

	/**
	 * Tells what authentication type the SDK used for the session activation.
	 *
	 * This has a valid value only if resultCode is LudeoResult::Success.
	 * If the client specified an explicit authentication, it will be set to what the client specified.
	 * If the client didn't specify an explicit authentication, the SDK will deduce an authentication type from the current
	 * environment and this will be set to that authentication type.
	 *
	 * The most common scenario in which this is useful is if the client needs to know if it is running in the cloud.
	 */
	LudeoAuthType authType;
LUDEO_STRUCT_END(LudeoSessionActivateCallbackParams);

/** Signature for the #ludeo_Session_Activate callback */
LUDEO_DECLARE_CALLBACK(LudeoSessionActivateCallback, const LudeoSessionActivateCallbackParams* data);

/**
 * @}
 *
 * \addtogroup ludeo_Session_GetLudeo
 * @{
 */

/** The most recent version of the #ludeo_Session_GetLudeo API */
#define LUDEO_SESSION_GETLUDEO_API_LATEST 1

/** Parameters to #ludeo_Session_GetLudeo */
LUDEO_STRUCT_START(LudeoSessionGetLudeoParams)
	/** Set this to LUDEO_SESSION_GETLUDEO_API_LATEST */
	int32_t apiVersion;

	/** String with the ludeo Id to retrieve */
	const char* ludeoId;
LUDEO_STRUCT_END(LudeoSessionGetLudeoParams);

/** Parameters to the #ludeo_Session_GetLudeo callback */
LUDEO_STRUCT_START(LudeoSessionGetLudeoCallbackParams)
	/**
	 * Tells if the operation succeeded or failed.
	 * If it succeeded, then dataReader contains the handle of the DataReader interface to use.
	 */
	LudeoResult resultCode;

	/** Context that was passed to the originating SDK call */
	void *clientData;

	/**
	 * If resultCode is Ludeo::Success, this contains the handle the client can use to read back the game state.
	 * The client can keep this handle for the entire lifetime of the owning Session instance, even between different 
	 * creator and player runs.
	 *
	 * The suggested approach is to keep the handle only while doing repeated ludeo player runs, and once switching to different ludeo
	 * or creator mode call ludeo_DataReader_Release to let the SDK know that ludeo data is no longer needed.
	 * The SDK is then free to release that memory in order to decrease memory usage.
	 */
	LudeoHDataReader dataReader;
LUDEO_STRUCT_END(LudeoSessionGetLudeoCallbackParams);

/** Signature to the #ludeo_Session_GetLudeo callback */
LUDEO_DECLARE_CALLBACK(LudeoSessionGetLudeoCallback, const LudeoSessionGetLudeoCallbackParams* data);

/**
 * @}
 *
 * \addtogroup ludeo_Session_OpenRoom
 * @{
 */

/** The most recent version of the ludeo_Session_OpenRoom API */
#define LUDEO_SESSION_OPENROOM_API_LATEST 2

/** Parameters to ludeo_Session_OpenRoom */
LUDEO_STRUCT_START(LudeoSessionOpenRoomParams)
	/** Set this to LUDEO_SESSION_OPENROOM_API_LATEST */
	int32_t apiVersion;

	/**
	 * A string that uniquely identifies the room.
	 * If specified, then this needs to be unique, such as a UUID.
	 * If not specified, then the SDK will generate a UUID. The client can then query the API to retrieve it.
	 */
	const char* roomId;

	/**
	 * If specified, it should be a valid Ludeo UUID
	 * If nulled, the room is opened to play the game normally (not a ludeo)
	 */
	const char* ludeoId;

	/**
	 * Set to LUDEO_TRUE to opt into a thread-safe DataWriter. Default: LUDEO_FALSE.
	 * Only read when apiVersion >= 2.
	 *
	 * When LUDEO_TRUE, the following functions are fully concurrent-safe and may be
	 * called simultaneously from multiple game threads:
	 * - #ludeo_DataWriter_EnterObject / #ludeo_DataWriter_LeaveObject
	 * - #ludeo_DataWriter_EnterComponent / #ludeo_DataWriter_LeaveComponent
	 * - All ludeo_DataWriter_SetXxx functions
	 * - #ludeo_DataWriter_DestroyObject
	 * - #ludeo_DataWriter_SendAction
	 *
	 * #ludeo_DataWriter_CreateObject is also thread-safe while by nature can not be called concurrently.
	 *
	 * The following functions are not designed for concurrent use and must only be called
	 * from the main thread (same restriction as in single-threaded mode):
	 * - #ludeo_DataWriter_SetCurrent
	 * - #ludeo_DataWriter_SetSendSettings
	 * - #ludeo_DataWriter_SaveStorage
	 *
	 * #ludeo_DataWriter_EnterObjectLate is not supported with a thread-safe DataWriter.
	 * Calling it will log an error and return LUDEO_FALSE.
	 */
	LudeoBool threadSafe;

	/**
	 * Maximum number of concurrent in-flight object writes allowed before a game thread blocks.
	 * Only meaningful when threadSafe == LUDEO_TRUE. Ignored in single-threaded mode.
	 * Only read when apiVersion >= 2.
	 *
	 * When 0, the SDK uses the default limit (1024), which is sufficient for most games.
	 *
	 * Each #ludeo_DataWriter_LeaveObject and #ludeo_DataWriter_DestroyObject call counts as
	 * one in-flight write until the SDK has finished processing it. If this limit is reached,
	 * the calling game thread will block until a slot becomes available. Increase this value
	 * if your game writes to many objects simultaneously from many threads in a single frame
	 * (e.g. 200+ objects across 8 threads at once).
	 */
	uint32_t maxConcurrentWrites;
	LUDEO_STRUCT_END(LudeoSessionOpenRoomParams);

/** Parameters to the #ludeo_Session_OpenRoom callback. */
LUDEO_STRUCT_START(LudeoSessionOpenRoomCallbackParams)

	/**
	 * Tells if the operation succeeded or failed.
	 * If this is LudeoResult::SDKDisabled, the player hasn't consented yet to the required SDK features, such as data capturing.
	 */
	LudeoResult resultCode;

	/** Context that was passed to the originating SDK call */
	void *clientData;

	/** 
	 * Room handle.
	 * This is used to call the ludeo_Room_XXX API.
	 */
	LudeoHRoom room;

	/**
	 * Handle to the DataWriter instance to use for the room.
	 * This handle is only valid while the room exists.
	 */
	LudeoHDataWriter dataWriter;
LUDEO_STRUCT_END(LudeoSessionOpenRoomCallbackParams);

/** Signature to the #ludeo_Session_OpenRoom callback */
LUDEO_DECLARE_CALLBACK(LudeoSessionOpenRoomCallback, const LudeoSessionOpenRoomCallbackParams* data);

/**
 * @}
 *
 * \addtogroup ludeo_Session_OpenGallery
 * @{
 */

/** The most recent version of the ludeo_Session_OpenGallery API */
#define LUDEO_SESSION_OPENGALLERY_API_LATEST 1

/** Structure with the parameters to ludeo_Session_OpenGallery */
LUDEO_STRUCT_START(LudeoSessionOpenGalleryParams)
	/** Set this to LUDEO_SESSION_OPENGALLERY_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoSessionOpenGalleryParams);

/**
 * @}
 *
 * \addtogroup ludeo_Session_SetLocalization
 * @{
 */

/** The most recent version of #ludeo_Session_SetLocalization API */
#define LUDEO_SESSION_SETLOCALIZATION_API_LATEST 1

/**
 * Structure with localization options.
 *
 * This is used by both the #ludeo_Session_Activate and #ludeo_Session_SetLocalization.
 *
 * Any language codes should be according to ISO 639-1 (two-letter codes).
 * There are some languages that can be specified as different regions, such as "es-419". The SDK intentionally doesn't validate
 * any language codes. It only checks if its nulled or an empty string. This is so that a game can request a language that is not
 * yet supported by the backend/Overlay, and have it automatically supported once the backend/Overlay are updated.
 */
LUDEO_STRUCT_START(LudeoSessionSetLocalizationParams)
	/** Set this to LUDEO_SESSION_SETLOCALIZATION_API_LATEST */
	int32_t apiVersion;

	/**
	 * Code for the client's preferred language.
	 * In practical terms this is what the player might have selected in any in-game menus.
	 * 
	 * This is required.
	 */
	const char* prefLang;

	/**
	 * Array of strings for all the languages the client supports.
	 *
	 * If prefLang is not available, then the backend will pick one from this list in the order they are specified.
	 *
	 * This can be nulled, in which case the backend will only take into consideration prefLang. If prefLang is also not
	 * available, it will default to "en".
	 */
	const char** supportedLangs;

	/**
	 * Number of entries in supportedLangs.
	 */
	uint32_t supportedLangsCount;
LUDEO_STRUCT_END(LudeoSessionSetLocalizationParams);

/**
 * @}
 */

//////////////////////////////////////////////////////////////////////////
//                     Notifications
//////////////////////////////////////////////////////////////////////////
/**
 *
 * \addtogroup Notifications
 * @{
 */


/**
 * \addtogroup LudeoSelected
 * @{
 */

/** The most recent version of the #ludeo_Session_AddNotifyLudeoSelected API */
#define LUDEO_SESSION_ADDNOTIFYLUDEOSELECTED_API_LATEST 1

/** Parameters to #ludeo_Session_AddNotifyLudeoSelected */
LUDEO_STRUCT_START(LudeoSessionAddNotifyLudeoSelectedParams)
	/** Set this to LUDEO_SESSION_ADDNOTIFYLUDEOSELECTED_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoSessionAddNotifyLudeoSelectedParams);

/**
 * Parameters to the #ludeo_Session_AddNotifyLudeoSelected callback.
 */
LUDEO_STRUCT_START(LudeoSessionLudeoSelectedCallbackParams)

	/** Context that was passed to #ludeo_Session_AddNotifyLudeoSelected */
	void *clientData;

	/**
	 * Id of the selected ludeo.
	 * If the client wishes to play the ludeo, it should pass this to #ludeo_Session_GetLudeo to get the actual ludeo data.
	 */
	const char* ludeoId;
LUDEO_STRUCT_END(LudeoSessionLudeoSelectedCallbackParams);

/** Signature for the #ludeo_Session_AddNotifyLudeoSelected callback */
LUDEO_DECLARE_CALLBACK(LudeoSessionLudeoSelectedCallback, const LudeoSessionLudeoSelectedCallbackParams* data);

/**
 * @}
 *
 * \addtogroup PauseGameRequest
 * @{
 */

/** The most recent version of the ludeo_Session_AddNotifyPauseGameRequest API */
#define LUDEO_SESSION_ADDNOTIFYPAUSEGAMEREQUEST_API_LATEST 1

/** Parameters to #ludeo_Session_AddNotifyPauseGameRequest */
LUDEO_STRUCT_START(LudeoSessionAddNotifyPauseGameRequestParams)
	/** Set this to LUDEO_SESSION_ADDNOTIFYPAUSEGAMEREQUEST_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoSessionAddNotifyPauseGameRequestParams);

/**
 * Parameters to the #ludeo_Session_AddNotifyPauseGameRequest callback.
 */
LUDEO_STRUCT_START(LudeoSessionPauseGameRequestCallbackParams)
	/** Context that was passed to #ludeo_Session_AddNotifyPauseGameRequest */
	void *clientData;
LUDEO_STRUCT_END(LudeoSessionPauseGameRequestCallbackParams);

/** Signature for the #ludeo_Session_AddNotifyPauseGameRequest callback */
LUDEO_DECLARE_CALLBACK(LudeoSessionPauseGameRequestCallback, const LudeoSessionPauseGameRequestCallbackParams* data);

/**
 * @}
 *
 * \addtogroup ResumeGameRequest
 * @{
 */

/** The most recent version of the ludeo_Session_AddNotifyResumeGameRequest API */
#define LUDEO_SESSION_ADDNOTIFYRESUMEGAMEREQUEST_API_LATEST 1

/** Parameters to ludeo_Session_AddNotifyResumeGameRequest */
LUDEO_STRUCT_START(LudeoSessionAddNotifyResumeGameRequestParams)
	/** Set this to LUDEO_SESSION_ADDNOTIFYRESUMEGAMEREQUEST_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoSessionAddNotifyResumeGameRequestParams);

/**
 * Parameters to the #ludeo_Session_AddNotifyResumeGameRequest callback.
 */
LUDEO_STRUCT_START(LudeoSessionResumeGameRequestCallbackParams)
	/** Context that was passed to #ludeo_Session_AddNotifyResumeGameRequest */
	void *clientData;
LUDEO_STRUCT_END(LudeoSessionResumeGameRequestCallbackParams);

/** Signature to the #ludeo_Session_AddNotifyResumeGameRequest callback */
LUDEO_DECLARE_CALLBACK(LudeoSessionResumeGameRequestCallback, const LudeoSessionResumeGameRequestCallbackParams* data);

/**
 * @}
 *
 * \addtogroup BackToMenuRequest
 * @{
 */

/** The most recent version of the ludeo_Session_AddNotifyBackToMenuRequest API */
#define LUDEO_SESSION_ADDNOTIFYBACKTOMENUREQUEST_API_LATEST 1

/** Parameters to #ludeo_Session_AddNotifyBackToMenuRequest */
LUDEO_STRUCT_START(LudeoSessionAddNotifyBackToMenuRequestParams)
	/** Set this to LUDEO_SESSION_ADDNOTIFYBACKTOMENUREQUEST_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoSessionAddNotifyBackToMenuRequestParams)

/**
 * Parameters to the #ludeo_Session_AddNotifyBackToMenuRequest callback.
 */
LUDEO_STRUCT_START(LudeoSessionBackToMenuRequestCallbackParams)
	/** Context that was passed to #ludeo_Session_AddNotifyBackToMenuRequest */
	void *clientData;
LUDEO_STRUCT_END(LudeoSessionBackToMenuRequestCallbackParams);

/** Signature for the #ludeo_Session_AddNotifyBackToMenuRequest callback */
LUDEO_DECLARE_CALLBACK(LudeoSessionBackToMenuRequestCallback, const LudeoSessionBackToMenuRequestCallbackParams* data);


/**
 * @}
 *
 * \addtogroup RoomReady
 * @{
 */

/** The most recent version of the #ludeo_Session_AddNotifyRoomReady API */
#define LUDEO_SESSION_ADDNOTIFYROOMREADY_API_LATEST 1

/** Parameters to #ludeo_Session_AddNotifyRoomReady */
LUDEO_STRUCT_START(LudeoSessionAddNotifyRoomReadyParams)
	/** Set this to LUDEO_SESSION_ADDNOTIFYROOMREADY_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoSessionAddNotifyRoomReadyParams);

/**
 * Parameters to the #ludeo_Session_AddNotifyRoomReady callback.
 */
LUDEO_STRUCT_START(LudeoSessionRoomReadyCallbackParams)

	/** Context that was passed to #ludeo_Session_AddNotifyRoomReady */
	void *clientData;

	/**
	 * Handle of the room this notification refers to.
	 * Note that this is NOT a new handle. It's the handle to an existing room the client created.
	 *
	 * Even if the client only has 1 room at a time, it should still validate if the notification is for the room it expects.
	 * This can avoid bugs related to a callback being in the queue for execution but when the client handles the callback that
	 * room is no longer relevant.
	 * If the client needs to query other info about the room, it can use the #ludeo_Room_GetInfo function.
	 */
	LudeoHRoom room;
LUDEO_STRUCT_END(LudeoSessionRoomReadyCallbackParams);

/** Signature for the #ludeo_Session_AddNotifyRoomReady callback */
LUDEO_DECLARE_CALLBACK(LudeoSessionRoomReadyCallback, const LudeoSessionRoomReadyCallbackParams* data);

/**
 * @}
 *
 * \addtogroup ConsentUpdated
 * @{
 */

//
// ludeo_Session_AddNotifyConsentUpdated related types
//

/** The most recent version of the #ludeo_Session_AddNotifyConsentUpdated API */
#define LUDEO_SESSION_ADDNOTIFYCONSENTUPDATED_API_LATEST 1

/** Parameters to #ludeo_Session_AddNotifyConsentUpdated */
LUDEO_STRUCT_START(LudeoSessionAddNotifyConsentUpdatedParams)
	/** Set this to LUDEO_SESSION_ADDNOTIFYCONSENTUPDATED_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoSessionAddNotifyConsentUpdatedParams);

/**
 * Parameters passed to #ludeo_Session_AddNotifyConsentUpdated callback
 */
LUDEO_STRUCT_START(LudeoSessionConsentUpdatedCallbackParams)

	/** Context that was passed to #ludeo_Session_AddNotifyConsentUpdated */
	void *clientData;

	/** If true, the user is allowed to create ludeos */
	LudeoBool canCreateLudeo;

	/** If true, the user is allowed to play ludeos */
	LudeoBool canPlayLudeo;
LUDEO_STRUCT_END(LudeoSessionConsentUpdatedCallbackParams);

/** Signature for the #ludeo_Session_AddNotifyConsentUpdated callback*/
LUDEO_DECLARE_CALLBACK(LudeoSessionConsentUpdatedCallback, const LudeoSessionConsentUpdatedCallbackParams* data);

/**
 * @}
 *
 * \addtogroup LocalizationChanged
 * @{
 */

//
// ludeo_Session_AddNotifyLocalizationChanged related types
//

/** The most recent version of the #ludeo_Session_AddNotifyLocalizationChanged API */
#define LUDEO_SESSION_ADDNOTIFYLOCALIZATIONCHANGED_API_LATEST 1

/** Parameters to #ludeo_Session_AddNotifyLocalizationChanged */
LUDEO_STRUCT_START(LudeoSessionAddNotifyLocalizationChangedParams)
	/** Set this to LUDEO_SESSION_ADDNOTIFYLOCALIZATIONCHANGED_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoSessionAddNotifyLocalizationChangedParams);

/**
 * Parameters passed to #ludeo_Session_AddNotifyLocalizationChanged callback
 */
LUDEO_STRUCT_START(LudeoSessionLocalizationChangedCallbackParams)

	/** Context that was passed to #ludeo_Session_AddNotifyLocalizationChanged */
	void *clientData;

	/**
	 * The language the Overlay changed to (according to ISO 639-1, two-letter codes)
	 *
	 * If the client supports this language, it should change its localization to this language.
	 */
	const char* prefLang;

LUDEO_STRUCT_END(LudeoSessionLocalizationChangedCallbackParams);

/** Signature for the #ludeo_Session_AddNotifyLocalizationChanged callback */
LUDEO_DECLARE_CALLBACK(LudeoSessionLocalizationChangedCallback, const LudeoSessionLocalizationChangedCallbackParams* data);


/**
 * @}
 * \addtogroup MuteRequest
 * @{
 */
//
// ludeo_Session_AddNotifyMuteGameRequest related types
//

/** The most recent version of the #ludeo_Session_AddNotifyMuteRequest API */
#define LUDEO_SESSION_ADDNOTIFYMUTEREQUEST_API_LATEST 1

/** The parameters to #ludeo_Session_AddNotifyMuteRequest */
LUDEO_STRUCT_START(LudeoSessionAddNotifyMuteRequestParams)
	/** Set this to LUDEO_SESSION_ADDNOTIFYMUTEREQUEST_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoSessionAddNotifyMuteRequestParams);

/**
 * Structure with the parameters passed to the MuteRequest callback
 * See #ludeo_Session_AddNotifyMuteRequest for more details.
 */
LUDEO_STRUCT_START(LudeoSessionMuteRequestCallbackParams)
	/** Context that was passed to #ludeo_Session_AddNotifyMuteRequest */
	void* clientData; 
	/** If true, the game should be muted. If false, the game should be unmuted. */
	LudeoBool mute;
LUDEO_STRUCT_END(LudeoSessionMuteRequestCallbackParams);

/** Signature to the #ludeo_Session_AddNotifyMuteRequest callback */
LUDEO_DECLARE_CALLBACK(LudeoSessionMuteRequestCallback, const LudeoSessionMuteRequestCallbackParams* data);


// ludeo_Session_AddNotifySnapshotRequest related types
//

/** The most recent version of the #ludeo_Session_AddNotifySnapshotRequest API */
#define LUDEO_SESSION_ADDNOTIFYSNAPSHOTREQUEST_API_LATEST 1

/** The parameters to #ludeo_Session_AddNotifySnapshotRequest */
LUDEO_STRUCT_START(LudeoSessionAddNotifySnapshotRequestParams)
	/** Set this to LUDEO_SESSION_ADDNOTIFYSNAPSHOTREQUEST_API_LATEST */
	int32_t apiVersion;
LUDEO_STRUCT_END(LudeoSessionAddNotifySnapshotRequestParams);

/**
 * Structure with the parameters passed to the SnapshotRequest callback
 * See #ludeo_Session_AddNotifySnapshotRequest for more details.
 */
LUDEO_STRUCT_START(LudeoSessionSnapshotRequestCallbackParams)
	/** Context that was passed to #ludeo_Session_AddNotifySnapshotRequest */
	void* clientData;
LUDEO_STRUCT_END(LudeoSessionSnapshotRequestCallbackParams);

/** Signature to the #ludeo_Session_AddNotifySnapshotRequest callback */
LUDEO_DECLARE_CALLBACK(LudeoSessionSnapshotRequestCallback, const LudeoSessionSnapshotRequestCallbackParams* data);

/**
 * @}
 */


// Notifications - END
/**
 * @}
 */

// Session - END
/**
 * @}
 */


#pragma pack(pop)

