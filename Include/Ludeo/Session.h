// Copyright 2023 Ludeo. All rights reserved.

/**
 * @file
 * Session interface functions
 */

#pragma once

#include "Ludeo/Common.h"
#include "Ludeo/SessionTypes.h"

/** \addtogroup Session
 * @{
 *
 * The Session interface represents a connection to the backend.
 * It is the first interface a client will create after the SDK initialization. All the other interfaces are created from this
 * one, directly or indirectly.
 */

/**
 * Returns true or false depending if the handle is a valid handle.
 * This is useful for code that needs to quickly validate handles in a away that doesn't produce side effects. E.g: Debug code.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_Session_IsValid(LudeoHSession handle);

/** \defgroup ludeo_Session_Create ludeo_Session_Create
 * @{
 */

/**
 * Creates Ludeo session instance
 *
 * A Session instance represents a logged in user and their connection to the backend.
 * After calling this function the client should:
 *
 * 1. Register to receive any notifications with the ludeo_Session_AddNotifyXXX functions
 * 2. Call ludeo_Session_Activate to connect to the backend
 *
 * @param params Structure with the function parameters
 * @param outHandle Will contain the session handle on exit.
 * @return LudeoResult::Success if the session was created, in which case the output parameter handle will contain the handle
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_Session_Create(const LudeoSessionCreateParams* params, LudeoHSession* outHandle);
LUDEO_DEFINE_FUNC_TRAITS_2(Session, SESSION, Create, CREATE);

/**
 * @}
 *
 *\defgroup ludeo_Session_Release ludeo_Session_Release
 * @{
 */

/**
 * Releases a Ludeo session instance
 *
 * The client should call this for every Session before calling ludeo_Shutdown.
 * After calling this function, the session handle should not be used again. Doing so is undefined behavior.
 *
 * Session destruction is done asynchronously, and the completion callback is guaranteed to be the last callback for the Session.
 *
 * Ongoing async calls for the Session will be Canceled and their callbacks called with LudeoResult::Canceled.
 * Some of those ongoing async calls might have already finished and are waiting for the client to tick the SDK, therefore
 * those will not receive LudeoResult::Canceled in the callback.
 * The client should implement whatever strategies it needs to avoid triggering more calls to this Session handle when it receives
 * those callbacks.
 *
 * Consult the provided samples for possible patterns.
 * 
 * @param handle
 *		The interface handle
 *
 * @param params
 *		Parameters to the function.
 * 
 * @param clientData
 *		Context that will be passed to callback
 *
 * @param callback
 *		Callback to call once destruction is complete
 */
LUDEO_DECLARE_FUNC(void) ludeo_Session_Release(LudeoHSession handle, const LudeoSessionReleaseParams* params, void* clientData, LudeoSessionReleaseCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_2(Session, SESSION, Release, RELEASE);

/**
 * @}
 *
 *\defgroup ludeo_Session_Activate ludeo_Session_Activate
 * @{
 */

/**
 * Activates a session.
 * 
 * The client should call this after creating the session and registering to all the notifications.
 * While not technically required to register to notifications before calling this, some notifications can happen as the
 * result of calling this function and therefore might be missed by the client.
 * 
 * @param handle
 *		The interface handle.
 * 
 * @param params
 *		Parameters to the function.
 *
 * @param clientData
 *		Context that will be passed to a callback
 *
 * @param callback
 *		Callback to call once the operation completes
 * 
 * @warning
 * At the moment, if this call fails, the client should NOT attempt to call this a second time. If the client wishes to retry
 * the activation, it should destroy the session with #ludeo_Session_Release and attempt activating a second session.
 *
 */
LUDEO_DECLARE_FUNC(void) ludeo_Session_Activate(LudeoHSession handle, const LudeoSessionActivateParams* params, void* clientData, LudeoSessionActivateCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_2(Session, SESSION, Activate, ACTIVATE);

/**
 * @}
 *
 *\defgroup ludeo_Session_GetLudeo ludeo_Session_GetLudeo
 * @{
 */

/**
 * Asynchronously retrieves from the backend (or from the local cache if available) all the information needed to play a
 * specified Ludeo.
 *
 * A typical workflow for the client would be:
 * - Use ludeo_Session_AddNotifyLudeoSelected to register for ludeo selection notifications.
 *		- The user triggers a ludeo selection by some mechanism external to the game, which causes the LudeoSelected notification
 *		  to be called.
 * - Once the LudeoSelected notification callback is called, pass the supplied ludeoId to ludeo_Session_GetLudeo to retrieve that
 *   ludeo's data.
 * - The completion callback for ludeo_Session_GetLudeo provides the DataReader instance that can be used to setup the game state
 *   to play the ludeo.
 * 
 * Note that calling ludeo_Session_GetLudeo doesn't change the current state of the session.
 * The client is in control on when switch to ludeo player mode.
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
 *		Callback to call once the ludeo information is available. 
 */
LUDEO_DECLARE_FUNC(void) ludeo_Session_GetLudeo(LudeoHSession handle, const LudeoSessionGetLudeoParams* params, void *clientData, LudeoSessionGetLudeoCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_2(Session, SESSION, GetLudeo, GETLUDEO);

/**
 * @}
 *
 *\defgroup ludeo_Session_OpenRoom ludeo_Session_OpenRoom
 * @{
 */

/**
 * Opens a new room
 *
 * On completion, an handle to the new room is provided, which can be used with the ludeo_Room_XXX API.
 * To close a room a call to ludeo_Room_Close should be made.
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
 *		Callback to call once the operation completes.
 */
LUDEO_DECLARE_FUNC(void) ludeo_Session_OpenRoom(LudeoHSession handle, const LudeoSessionOpenRoomParams* params, void* clientData, LudeoSessionOpenRoomCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_2(Session, SESSION, OpenRoom, OPENROOM);


/**
 * @}
 *
 * \defgroup ludeo_Session_OpenGallery ludeo_Session_OpenGallery
 * @{
 */

/**
 * Opens the ludeo gallery.
 *
 * This allows the client to programmatically open the ludeo gallery, instead of relying on the overlay button
 *
 * @param handle
 *		The interface handle.
 * 
 * @param params
 *		Parameters to the function.
 *
 * @return
 *		LudeoResult::Success if the operation succeeded.
 */ 
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_Session_OpenGallery(LudeoHSession handle, const LudeoSessionOpenGalleryParams* params);
LUDEO_DEFINE_FUNC_TRAITS_2(Session, SESSION, OpenGallery, OPENGALLERY);

/**
 * @}
 *
 *\defgroup ludeo_Session_SetLocalization ludeo_Session_SetLocalization
 * @{
 */

/**
 * Changes the Overlay's localization.
 *
 * The change will only occur if the Overlay actually supports the requested language(s).
 *
 * Internally, this function is asynchronous (it sends the request to the backend), therefore the return value only tells
 * if the parameters and the SDK state allows the call. It doesn't tell if the actual change was applied or not.
 * 
 * If a change was made, the LocalizationChanged notification is raised. Note that even though it was the client requesting
 * the change, the notification is still raised. The client should take that into consideration and not end up in a loop
 * of calling #ludeo_Session_SetLocalization, receiving the notification, and then calling #ludeo_Session_SetLocalization again
 * and again.
 * 
 * This design is intentional to allow the developer to test if the client reacts to localization changes request coming from
 * the backend, which would be hard to test otherwise.
 *
 * A possible way the client can test localization changes at runtime is by (considering `en` and `fr`):
 * 
 * - Start the game in English
 * - Once the session is activated, call #ludeo_Session_SetLocalization to change to French (`fr`), BUT don't actually change the
 *   game's own language (leave it in English)
 * - The backend reacts th the #ludeo_Session_SetLocalization call and changes the Overlay to French.
 * - The LocalizationChanged notification is received by the game
 *		- The game looks at the notification details, sees it is a different language from the one it is currently using, and so
 *		  changes its language to French (to match the Overlay).
 *
 * In short, the client can use the notification as the point where it changes its own localization, instead of first changing the
 * language then asking the SDK to also change the Overlay.
 * 
 * @param handle
 *		The interface handle.
 * 
 * @param params
 *		Parameters to the function
 *
 * @return
 *		LudeoResult::Success if the call succeeded. By that, it means that all the required parameters were specified, and the
 *		information was forwarded to the backend/Overlay. It does **NOT** mean that localization was actually applied.
 *		The reason for this is because internally the operation is actually asynchronous (requires a call to the backend), but
 *		since there is nothing the client can do about a possible failure (e.g preferLang not supported by the Overlay), the API
 *		is presented as a synchronous operation.
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_Session_SetLocalization(LudeoHSession handle, const LudeoSessionSetLocalizationParams* params);
LUDEO_DEFINE_FUNC_TRAITS_2(Session, SESSION, SetLocalization, SETLOCALIZATION);

/**
 * @}
 */

//////////////////////////////////////////////////////////////////////////
//                    Session Notifications
//////////////////////////////////////////////////////////////////////////

/**
 *
 * \defgroup Notifications Notifications
 *
 * Notifications allows the client to react to things that are triggered by the backend and/or SDK.
 * To provide an optimal Ludeo experience a well behaved client should listen to all the notifications and react accordingly.
 * While some notifications can be safely ignored by the client, others should always be handled appropriately.
 * 
 * For all the **ludeo_Session_AddNotifyXXX** functions, a #LudeoNotificationId is returned.
 * The client should call #ludeo_RemoveNotification whenever it wants to stop receiving the notification.
 * The notification's lifetime is tied to the Session, and the SDK will stop sending the notification once the Session is destroyed.
 * If any of the parameters are invalid and the AddNotify call fails and returns LUDEO_INVALID_NOTIFICATIONID.
 *
 * Every AddNotify function takes a similar set of parameters
 * - The Session handle
 * - A `params` struct (which has any actual parameters the registration might need)
 * - A `clientData` and a `callback` parameter. This works the same as with the other async function calls. When the notification
 *   callback is called, it receives the `clientData` specified in the respective AddNotify call.
 * 
 * @{
 *
 * 
 */


/**
 * \defgroup LudeoSelected LudeoSelected
 * Notification received when a ludeo is selected externally to the client.
 *
 * @{
 */

/**
 * Registers to receive a notification when a Ludeo is selected externally to the client.
 * E.g: This allows the client to know when the player triggers a ludeo from outside the game.
 *
 * @param handle
 *		The interface handle.
 * 
 * @param params
 *		Parameters to the function.
 * 
 * @param clientData
 *		Context to be passed to the callback.
 *
 * @param callback
 *		Callback to call when a ludeo is selected
 *
 * @return
 * The notification Id. The client should call ludeo_RemoveNotification whenever it wants to stop receiving the notification.
 * The notification's lifetime is tied to the Session, the SDK will stop sending the notification once the Session is destroyed.
 * If any of the parameters are invalid and the call fails, it returns LUDEO_INVALID_NOTIFICATIONID
 *
 */
LUDEO_DECLARE_FUNC(LudeoNotificationId) ludeo_Session_AddNotifyLudeoSelected(LudeoHSession handle, const LudeoSessionAddNotifyLudeoSelectedParams* params, void* clientData, LudeoSessionLudeoSelectedCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_ADDNOTIFY(Session, SESSION, LudeoSelected, LUDEOSELECTED);

/**
 * @}
 *
 * \defgroup PauseGameRequest PauseGameRequest
 * Notification received when the backend or SDK requests the game to pause.
 * @{
 */

/**
 * Registers to receive a notification when the backend or SDK requests the game to pause.
 *
 * The client should respond by pausing gameplay (e.g., showing a pause menu or freezing the game state).
 *
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Parameters to the function.
 *
 * @param clientData
 *		Context to be passed to the callback.
 *
 * @param callback
 *		Callback to call when a pause request is received.
 *
 * @return
 * The notification Id. The client should call ludeo_RemoveNotification whenever it wants to stop receiving the notification.
 * The notification's lifetime is tied to the Session, the SDK will stop sending the notification once the Session is destroyed.
 * If any of the parameters are invalid and the call fails, it returns LUDEO_INVALID_NOTIFICATIONID
 */
LUDEO_DECLARE_FUNC(LudeoNotificationId) ludeo_Session_AddNotifyPauseGameRequest(LudeoHSession handle, const LudeoSessionAddNotifyPauseGameRequestParams* params, void* clientData, LudeoSessionPauseGameRequestCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_ADDNOTIFY(Session, SESSION, PauseGameRequest, PAUSEGAMEREQUEST);

/**
 * @}
 *
 * \defgroup ResumeGameRequest ResumeGameRequest
 * Notification received when the backend or SDK requests the game to resume after a pause.
 * @{
 */

/**
 * Registers to receive a notification when the backend or SDK requests the game to resume.
 *
 * This is typically sent after a previous pause request. The client should respond by resuming gameplay.
 *
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Parameters to the function.
 *
 * @param clientData
 *		Context to be passed to the callback.
 *
 * @param callback
 *		Callback to call when a resume request is received.
 *
 * @return
 * The notification Id. The client should call ludeo_RemoveNotification whenever it wants to stop receiving the notification.
 * The notification's lifetime is tied to the Session, the SDK will stop sending the notification once the Session is destroyed.
 * If any of the parameters are invalid and the call fails, it returns LUDEO_INVALID_NOTIFICATIONID
 */
LUDEO_DECLARE_FUNC(LudeoNotificationId) ludeo_Session_AddNotifyResumeGameRequest(LudeoHSession handle, const LudeoSessionAddNotifyResumeGameRequestParams* params, void* clientData, LudeoSessionResumeGameRequestCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_ADDNOTIFY(Session, SESSION, ResumeGameRequest, RESUMEGAMEREQUEST);

/**
 * @}
 *
 * \defgroup BackToMenuRequest BackToMenuRequest
 * Notification received when the backend or SDK requests the game to return to the main menu.
 * @{
 */

/**
 * Registers to receive a notification when the backend or SDK requests the game to return to the main menu.
 *
 * The client should respond by navigating back to its main menu or lobby screen.
 *
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Parameters to the function.
 *
 * @param clientData
 *		Context to be passed to the callback.
 *
 * @param callback
 *		Callback to call when a back-to-menu request is received.
 *
 * @return
 * The notification Id. The client should call ludeo_RemoveNotification whenever it wants to stop receiving the notification.
 * The notification's lifetime is tied to the Session, the SDK will stop sending the notification once the Session is destroyed.
 * If any of the parameters are invalid and the call fails, it returns LUDEO_INVALID_NOTIFICATIONID
 */
LUDEO_DECLARE_FUNC(LudeoNotificationId) ludeo_Session_AddNotifyBackToMenuRequest(LudeoHSession handle, const LudeoSessionAddNotifyBackToMenuRequestParams* params, void* clientData, LudeoSessionBackToMenuRequestCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_ADDNOTIFY(Session, SESSION, BackToMenuRequest, BACKTOMENUREQUEST);

/**
 * @}
 *
 * \defgroup RoomReady RoomReady
 * Notification received when a room is fully set up and ready for gameplay.
 * @{
 */

/**
 * Registers to receive a notification when a room is fully set up and ready for gameplay.
 *
 * The callback provides the handle to the room that became ready. Even if the client only has one room at a time,
 * it should validate that the notification refers to the expected room.
 *
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Parameters to the function.
 *
 * @param clientData
 *		Context to be passed to the callback.
 *
 * @param callback
 *		Callback to call when a room becomes ready.
 *
 * @return
 * The notification Id. The client should call ludeo_RemoveNotification whenever it wants to stop receiving the notification.
 * The notification's lifetime is tied to the Session, the SDK will stop sending the notification once the Session is destroyed.
 * If any of the parameters are invalid and the call fails, it returns LUDEO_INVALID_NOTIFICATIONID
 */
LUDEO_DECLARE_FUNC(LudeoNotificationId) ludeo_Session_AddNotifyRoomReady(LudeoHSession handle, const LudeoSessionAddNotifyRoomReadyParams* params, void* clientData, LudeoSessionRoomReadyCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_ADDNOTIFY(Session, SESSION, RoomReady, ROOMREADY);

/**
 * @}
 *
 * \defgroup ConsentUpdated ConsentUpdated
 * Notification received when the user's consent permissions for creating or playing ludeos are updated.
 * @{
 */

/**
 * Registers to receive a notification when the user's consent permissions are updated.
 *
 * The callback provides updated consent flags indicating whether the user is allowed to create and/or play ludeos.
 * The client should use this to enable or disable ludeo-related features accordingly.
 *
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Parameters to the function.
 *
 * @param clientData
 *		Context to be passed to the callback.
 *
 * @param callback
 *		Callback to call when consent permissions are updated.
 *
 * @return
 * The notification Id. The client should call ludeo_RemoveNotification whenever it wants to stop receiving the notification.
 * The notification's lifetime is tied to the Session, the SDK will stop sending the notification once the Session is destroyed.
 * If any of the parameters are invalid and the call fails, it returns LUDEO_INVALID_NOTIFICATIONID
 */
LUDEO_DECLARE_FUNC(LudeoNotificationId) ludeo_Session_AddNotifyConsentUpdated(LudeoHSession handle, const LudeoSessionAddNotifyConsentUpdatedParams* params, void* clientData, LudeoSessionConsentUpdatedCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_ADDNOTIFY(Session, SESSION, ConsentUpdated, CONSENTUPDATED);

/**
 * @}
 *
 * \defgroup LocalizationChanged LocalizationChanged
 * Notification to inform the client a localization change happened.
 * 
 * The client can use this to react to changes when running in a cloud environment.
 * 
 * @{
 */

/**
 * Registers to receive a notification when a localization change occurs.
 *
 * This is primarily relevant in cloud environments. The callback provides the new preferred language
 * (ISO 639-1 two-letter code). If the client supports the requested language, it should update its
 * localization accordingly.
 *
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Parameters to the function.
 *
 * @param clientData
 *		Context to be passed to the callback.
 *
 * @param callback
 *		Callback to call when the localization changes.
 *
 * @return
 * The notification Id. The client should call ludeo_RemoveNotification whenever it wants to stop receiving the notification.
 * The notification's lifetime is tied to the Session, the SDK will stop sending the notification once the Session is destroyed.
 * If any of the parameters are invalid and the call fails, it returns LUDEO_INVALID_NOTIFICATIONID
 */
LUDEO_DECLARE_FUNC(LudeoNotificationId) ludeo_Session_AddNotifyLocalizationChanged(LudeoHSession handle, const LudeoSessionAddNotifyLocalizationChangedParams* params, void* clientData, LudeoSessionLocalizationChangedCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_ADDNOTIFY(Session, SESSION, LocalizationChanged, LOCALIZATIONCHANGED);

/**
 * @}
* 
* \defgroup MuteRequest MuteRequest
* @{
*/

/**
 * Registers to receive a notification when Ludeo wants to start\\stop playing audio
 * E.g: This allows the client to know when the game sound will overlap with Ludeo audio
 *
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Parameters to the function.
 *
 * @param clientData
 *		Context to be passed to the callback.
 *
 * @param callback
 *		Callback to call when Ludeo audio stream is started\\stopped 
 *
 * @return
 * The notification Id. The client should call ludeo_RemoveNotification whenever it wants to stop receiving the notification.
 * The notification's lifetime is tied to the Session, the SDK will stop sending the notification once the Session is destroyed.
 * If any of the parameters are invalid and the call fails, it returns LUDEO_INVALID_NOTIFICATIONID
 *
 */
LUDEO_DECLARE_FUNC(LudeoNotificationId) ludeo_Session_AddNotifyMuteRequest(LudeoHSession handle, const LudeoSessionAddNotifyMuteRequestParams* params, void* clientData, LudeoSessionMuteRequestCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_ADDNOTIFY(Session, SESSION, MuteRequest, MUTEREQUEST);

/**
 * @}
 *
 * \defgroup SnapshotRequest SnapshotRequest
 * @{
 */

/**
 * Registers to receive a notification for snapshot request.
 *
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Parameters to the function.
 *
 * @param clientData
 *		Context to be passed to the callback.
 *
 * @param callback
 *		Callback to call when snapshot request is received
 *
 * @return
 * The notification Id. The client should call ludeo_RemoveNotification whenever it wants to stop receiving the notification.
 * The notification's lifetime is tied to the Session, the SDK will stop sending the notification once the Session is destroyed.
 * If any of the parameters are invalid and the call fails, it returns LUDEO_INVALID_NOTIFICATIONID
 *
 */
LUDEO_DECLARE_FUNC(LudeoNotificationId) ludeo_Session_AddNotifySnapshotRequest(LudeoHSession handle, const LudeoSessionAddNotifySnapshotRequestParams* params, void* clientData, LudeoSessionSnapshotRequestCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_ADDNOTIFY(Session, SESSION, SnapshotRequest, SNAPSHOTREQUEST);

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

