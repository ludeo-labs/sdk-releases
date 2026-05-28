// Copyright 2023 Ludeo. All rights reserved.

/**
 * @file
 * Room interface functions
 */

#pragma once

#include "Ludeo/Common.h"
#include "Ludeo/RoomTypes.h"

/** \addtogroup Room
 * @{
 * The Room interface allows the client to track players in multiplayer games.
 * A room is still necessary for single player games, but only one player should be added to the room
 *
 * Rooms interfaces require a call to #ludeo_Room_Close to close the room and release any resources.
 */

/**
 * Returns true or false depending if the handle is a valid handle.
 * This is useful for code that needs to quickly validate handles in a away that doesn't produce side effects. E.g: Debug code.
 */
LUDEO_DECLARE_FUNC(LudeoBool) ludeo_Room_IsValid(LudeoHRoom handle);

/** \defgroup ludeo_Room_GetInfo ludeo_Room_GetInfo
 * @{
 */

/**
 * Returns the room's information.
 *
 * Check the #LudeoRoomInfo struct for what information is available.
 * 
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Parameters to the functions
 *
 * @param out
 *		If #LudeoResult::Success is returned, then on exit this will contain a pointer to the information.
 *
 * @return
 *		If LudeoResult::Success, then the "out" parameter contains the information
 *
 * @important
 *		Once done with the information, the client should call #ludeo_RoomInfo_Release to release the struct.
 */
LUDEO_DECLARE_FUNC(LudeoResult) ludeo_Room_GetInfo(LudeoHRoom handle, const LudeoRoomGetInfoParams* params, LudeoRoomInfo** out);
LUDEO_DEFINE_FUNC_TRAITS_2(Room, ROOM, GetInfo, GETINFO);

/**
 * Release any resources hold by the specified structure
 *
 * @param info structure to release. If nulled, the call is a no-op.
 */
LUDEO_DECLARE_FUNC(void) ludeo_RoomInfo_Release(LudeoRoomInfo* info);

/**
 * @}
 *
 *\defgroup ludeo_Room_Close ludeo_Room_Close
 * @{
 */

/**
 * Closes the specified room.
 *
 * This operation is asynchronous, and client should not attempt to open another room until this operation completes.
 * The room handle should not be used again after calling this. 
 *
 * @param handle
 *		The interface handle.
 * 
 * @param params
 *		Parameters to the function.

 * @param clientData
 *		Context that will be passed to callback

 * @param callback
 *		Callback to call once the operation completes
 */
LUDEO_DECLARE_FUNC(void) ludeo_Room_Close(LudeoHRoom handle, const LudeoRoomCloseParams* params, void* clientData, LudeoRoomCloseCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_2(Room, ROOM, Close, CLOSE);

/**
 * @}
 *
 *\defgroup ludeo_Room_AddPlayer ludeo_Room_AddPlayer
 * @{
 */

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// #LC3636 : There is an edge case here. Internally (at the time of writing), we need to add one player at a time to the backend.
//  If one fails, I might have others that succeeded and then need to be removed if I want to consider the entire operation as
// failed. The way this will be resolved will have to take into consideration the RoomReady callback. There is a problem with this
// entire thing of adding players that I haven't solved yet, because the SDK doesn't know when the developer is done adding players.
// Discussion at : https://ludeos-workspace.slack.com/archives/C06DGAFV85P/p1705671539449619
#endif

/**
 * Adds a player to a room.
 *
 * See the #LudeoRoomAddPlayerParams struct for more details.
 *
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Parameters to the function.
 *
 * @param clientData
 *		Context that will be passed to callback.
 *
 * @param callback
 *		Callback to call once the operation completes
 */
LUDEO_DECLARE_FUNC(void) ludeo_Room_AddPlayer(LudeoHRoom handle, const LudeoRoomAddPlayerParams* params, void* clientData, LudeoRoomAddPlayerCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_2(Room, ROOM, AddPlayer, ADDPLAYER);

/**
 * @}
 *
 *\defgroup ludeo_Room_RemovePlayer ludeo_Room_RemovePlayer
 * @{
 */

/**
 * Removes a player from a room
 *
 * Any existing GameplaySession handle associated with the players will be automatically invalidated.
 *
 * @param handle
 *		The interface handle.
 *
 * @param params
 *		Parameters to the function.
 *
 * @param clientData
 *		Context that will be passed to callback
 *
 * @param callback
 *		Callback to call once the operation completes
 *
 */
LUDEO_DECLARE_FUNC(void) ludeo_Room_RemovePlayer(LudeoHRoom handle, const LudeoRoomRemovePlayerParams* params, void *clientData, LudeoRoomRemovePlayerCallback callback);
LUDEO_DEFINE_FUNC_TRAITS_WITHCALLBACK_2(Room, ROOM, RemovePlayer, REMOVEPLAYER);

/**
 * @}
 */


/**
 * @}
 */


