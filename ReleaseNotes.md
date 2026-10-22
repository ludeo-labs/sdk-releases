# v4.2.10

## New features
* The overlay's in-game countdown now stops on the **Pause Timer** global trigger (`TIMER_PAUSE_KEY` / `TIMER_RESUME_KEY`) during local Ludeo playback. When the game pauses the timer, for example for a cutscene or dialogue, the countdown stops while the game and its events keep running, and it picks up again from the server's remaining time on **Resume Timer**. This needs the platform build that forwards the `pause-timer` / `resume-timer` events to the overlay.

## Fixes
* Fixed the countdown restarting too early when a game pause and a timer pause overlapped. It now restarts only after both have been resumed.

# v4.2.9

## New features
* Moments are now recorded in AV1 where the hardware supports it. On Windows the SDK tries NVENC AV1, then NVENC H.264, then the libx264 CPU encoder; on Android it tries MediaCodec AV1, then MediaCodec H.264. GPUs without AV1 support (pre-Ada NVIDIA, older Android devices) get H.264 as before. AMD, Intel and OpenGL games stay on libx264.
* Encoding can be controlled per game from the backend (Studio Lab), under `videoEncoderConfig`:
  * `hardwareEncoder` (default `true`): set `false` to keep a game on the libx264 CPU encoder.
  * `av1Config.enabled` (default `true`): set `false` to keep a game on H.264.
  * `av1Config` can also override `width`, `height`, `fps`, `bitrate`, `idrInterval`, `numEncodeBuffers` and `preset` (1-7) for AV1 only. Any field left out uses the top-level value. Older SDKs ignore `av1Config`.
* New debug overrides: the `video-codec` (`av1` / `h264`), `video-resolution` and `video-bitrate` commands, plus the `-ludeoVideoCodec` / `LUDEO_VIDEO_CODEC`, `-ludeoVideoSwEncoder` / `LUDEO_VIDEO_SWENCODER` and `-ludeoVideoLocalSave` launch options for games that can't call `ludeo_Command`.

## Improvements
* If an encoder stops mid-session, it is shut down and replaced by the next one in the list (up to 3 times per recording session), instead of video stopping for the rest of the session.
* Clips start closer to the marked moment: the 4x keyframe-interval multiplier on NVENC, AMF and MediaCodec was removed, since clips are cut on keyframes.
* When the game window and the encode size have different aspect ratios, the video is letterboxed instead of stretched.

## Fixes
* Fixed a GPU hang (`DXGI_ERROR_DEVICE_HUNG`, then a crash in SDL) when recording a D3D11 game whose device isn't multithread-protected (e.g. SDL2 without `SDL_HINT_RENDER_DIRECT3D_THREADSAFE`). Encoder init now fails cleanly and logs the reason.
* Fixed NVENC on D3D11 unmapping input textures on the download thread, which shares the game's immediate context.
* Fixed the render thread sleeping, or waiting forever, for a free encode buffer when NVENC or libx264 fell behind. The frame is now dropped, and the encoder is marked as stopped if its download thread exits.
* Fixed several NVENC and AMF shutdown and failure paths (double unmaps, buffers not returned to the pool, a single-producer queue used by two threads).

# v4.2.8

## Improvements
* The Windows SDK DLL now carries company, product, description and copyright fields in its file properties.
* Added Authenticode code signing to the Windows build pipeline.

# v4.2.7

## New features
* Automatic moment capture (backend-initiated moment capture) can now be turned on or off per game from the backend, via the Studio Lab "Auto Capture" toggle. When the game configuration carries a boolean `game.highlightConfig.autoCapture`, that value takes effect for the session and overrides whatever was set locally (the `LUDEO_BACKEND_INITIATED_MOMENT_CAPTURE` environment variable / command line, or the `backend-initiated-moment-capture` command). When the field is absent the locally configured value is kept unchanged.

## Improvements
* The overlay's auto-capture badge now reads its enabled state from the same single source of truth as the rest of the SDK (`gCmds.backendInitiatedMomentCapture`) instead of sampling the `LUDEO_BACKEND_INITIATED_MOMENT_CAPTURE` environment variable directly, so a game enabled through the backend configuration shows the badge too.

## Fixes
* Fixed the auto-capture badge ignoring the configured notification position. The badge was pinned to the top-left corner of the screen while the creator notification toast beside it already followed the per-environment position from Studio Lab (Advanced Settings --> Ludeo Overlays Position --> Notifications), so a game that moved its notifications was left with a badge in the opposite corner. Both now use the same position.

# v4.2.6

## New features
* Added a creator-flow upload-status toast to the overlay. While a moment is uploading, the overlay shows a persistent "Saving your moment / Uploading…" card with live batch progress ("Saving N moments" and an "X of Y saved" line), then a "Success!" / "Upload failed" result so the player knows when it's safe to exit the game.

## Improvements
* A marked moment now reports a single terminal outcome (`GlobalDelegates::onHighlightUploadCompleted`) from whichever stage ends it -- the mark request, the `capture_video_request` handshake, the ongoing-highlight watchdog, or the video upload. Previously only the video upload stage reported anything, so a moment that died earlier failed silently and UI layers had to fall back on their own timeouts.
* The creator screen owns upload batch counting internally, tracking active/saved/total uploads and per-batch failure state directly instead of relying on counts pushed through the upload event payloads.
* The overlay's upload safety timeout -- the fallback that flips a never-completing "Uploading..." card to the failure toast -- was raised from 60 seconds to 5 minutes and now re-arms on upload progress rather than on each new capture. Every known failure is reported the moment it happens, so the timeout only covers a broken reporting path; the old value could fail uploads the transport was still legitimately retrying.

## Fixes
* Fixed a moment's video upload retrying forever when the upload connection was accepted and then dropped mid-transfer. It now gives up after a bounded number of reconnects and reports the failure. Previously the upload stayed pending indefinitely, which also blocked `ludeo_Session_Release` from completing, since session destruction waits for in-flight uploads.
* Fixed the overlay stranding the player on a permanent "please keep the game open" message when an upload never completed; a stuck upload now falls back to the "Upload failed" result toast after the safety timeout.
* Fixed the upload-pending flag not being cleared when the creator screen closes.
* Fixed the in-process overlay showing the pre-play screen with an empty objective list. The objectives are now fetched over HTTP as soon as a Ludeo is selected, instead of waiting for the platform's `gameplays.gameplay-ready` event — which cannot arrive until the game has opened the room, so the list was always empty on first display.
* Fixed the pre-play "Play Moment" button being clickable before the room was ready. Pressing it in that window sent an event the SDK could not act on, gameplay never started, and the button stayed disabled for the rest of the screen. It is now drawn disabled and ignores input until the room is ready.

# v4.2.5

## New features
* The position of the in-game notification toast (capture hints and confirmations) can now be configured per environment from Studio Lab (Advanced Settings --> Ludeo Overlays Position --> Notifications), like the Mid-Play Screen. Games without a configured position keep the previous placement.

## Improvements
* The in-process (ImGui) overlay now connects to the Echo WebSocket in text mode as a `ui` client, replacing the legacy session WebSocket endpoint. The overlay connection starts only once a valid auth token is available and reuses the session's `principal_id` (connection id).
* Auth tokens passed on WebSocket connection URLs are now redacted from SDK logs in release builds (`token=<Redacted>`), matching the existing `userId` redaction.

# v4.2.4


## New features

## Improvements
* Cloud Token authentication now has highest preference (Cloud --> explicit --> implicit).
## Fixes
* Fixed a bug where the cursor would disappear in the Unity editor after SDK authentication.
# v4.2.3

## Improvements
* Removed the Gameface dependency and its leftovers (including OpenAL) from the SDK and third-party Deps, completing the migration to the ImGui-based overlay.

## Fixes
* Fixed the local Ludeo flow — the launcher-provided user id (e.g. the Steam id) is now sent as the `launcher_id` query parameter on the Echo WebSocket connections, captured at session activation.
* Fixed a deadlock during DX12 context shutdown (Steam overlay with `d3d11on12`) caused by the function-hook dispatch holding an exclusive lock across re-entrant calls into foreign code. Dispatch now takes a shared lock, and unhook drains in-flight dispatches before freeing the trampoline.
* Fixed false-positive `Error` logs during session-activation config parsing — the renamed `MarkHighlight` action, the deprecated `actionDataBufferSize` field, and the optional `PerformanceMonitor` endpoint no longer report spurious errors.
* Fixed a spurious `Json key 'url' not found` error logged on every mark-highlight against the Echo backend; the removed `url` field is no longer read from the response.
* Stopped the SDK from re-issuing the performance-monitor request every interval after the monitor endpoint returns a 404; monitoring is now disabled for the rest of the session, while transient failures still retry as before.

# v4.2.2

## Fixes
* Fixed the released version not being stamped into the binaries; the version is now stamped via the `LUDEOSDK_VERSION` override.

# v4.2.1

## Improvements
* ludeo_Tick log has now been moved to logging level Verbose.

## Fixes
* Fixed breaking bug in Echo - Ludeos using 4.2.0 couldn't run on the cloud. WS principal id is now 1:1 connection id.
* Fixed a bug where ImGui will appear alongside the platform's overlay on cloud.
* Fix for hardcoded ImGui overlay values.

# v4.2.0

## Breaking changes

* Migrated the realtime channel to the Echo WebSocket protocol — the session WebSocket is now split into separate binary and text channels with an enveloped wire format. Token and `principal_id` are passed as URL-encoded query parameters on the Echo endpoint. Game-event parsers (`NewLudeoSelected`, `CaptureVideoRequest`, etc.) were updated to the new Echo message shape.
* Replaced the `/highlights` HTTP endpoint with `/v1/moments`. `ludeo_DataWriter_MarkHighlight` now POSTs to `/v1/moments`, and backend-driven capture events use the matching `Capture_video_request` Echo event name.

## Improvements

* Replaced the in-process Game Face overlay with an ImGui-based implementation, removing the Game Face runtime dependency and reducing overlay startup cost and memory footprint.
* Hardened the new WebSocket client with reconnect-path coverage, frame-type enforcement, and a pong reply path in the Echo mock server used by tests.
* Added `MarkHighlightReason` plumbing so manual and backend-initiated highlights can be distinguished by call site rather than by a shared flag.

## Fixes

* Fixed a use-after-free during WebSocket reconnect when a pending IOCP read completed after the socket was torn down.
* Fixed `/api/v3` suffix handling in `SessionMarkHighlightTask` and removed the related magic number.
* Removed the dead FlatBuffer reply path on the session binary WebSocket and dropped the unused session lock that tolerated unexpected `connection_info` fields in `data.context`.

# v4.1.0

## New features

* **Thread-safe DataWriter** — added `threadSafe` and `maxConcurrentWrites` fields to `LudeoSessionOpenRoomParams` (API version 2). When `threadSafe` is `LUDEO_TRUE`, the following calls become fully concurrent-safe and may be called simultaneously from multiple game threads:
  - `ludeo_DataWriter_EnterObject` / `ludeo_DataWriter_LeaveObject`
  - `ludeo_DataWriter_EnterComponent` / `ludeo_DataWriter_LeaveComponent`
  - All `ludeo_DataWriter_SetXxx` functions
  - `ludeo_DataWriter_DestroyObject`
  - `ludeo_DataWriter_CreateObject`
  - `ludeo_DataWriter_SendAction`

  `maxConcurrentWrites` controls the maximum number of in-flight object writes before a game thread blocks (default: 1024). `ludeo_DataWriter_EnterObjectLate` is not supported in thread-safe mode.

## Fixes

* Fixed spurious SDK errors logged during session activation caused by a malformed log format string and stale endpoint configuration.

# v4.0.21

## New features

* Added `ludeo_Session_AddNotifySnapshotRequest` — a new notification API that triggers a callback when the SDK requests a snapshot of the game's initial state.
* Added `ludeo_DataWriter_SaveStorage` — saves an arbitrary binary payload to SDK-managed storage associated with the current run, allowing games to persist precomputed data alongside the recorded gameplay.
* Added `ludeo_DataReader_LoadStorage` and `ludeo_DataReader_GetStorageSize` — retrieve binary payloads previously saved via `SaveStorage` during playback, with `GetStorageSize` allowing callers to query the payload size upfront for pre-allocation.

## Improvements

* Added memory profiling with Tracy
* Added a dictionary for using the SDK with games that use OpenGL/AL
* Refactored CI/CD with Android builds and an on-demand test machine
* Enhanced WebSocket stability using reconnect strategy
* Updated Tracy to v0.12.2

## Fixes

* Removed redundant session activation related errors from SDK logs
* Fixed a bug when using Vulkan
* Fixed a bug when using with a game running OpenAL

# v4.0.20

## Improvements

* DirectX12 performance optimization

# v4.0.19

## Improvements

* Window handle is no longer required when calling the session activation API, to support headless game servers.
* For games that already depend on Gameface, the SDK can now use those Gameface DLLs.
* Video encoder now supports OpenGL
* Implementation-independent DirectX12 hooking (no need to rely on memory scanning)

## Fixes

* Fixed video encoder reset being triggered in a scenario where it should not have been
* Some improvements to shutdown code

# v4.0.18

## Improvements

* When the `video-localsave` ludeo command is set to `1`, it now saves a `LudeoVideoMessage-<TIMESTAMP>.txt` alongside the `h264` file, containing information to help debug video problems.
* Added an experimental internal data structure that considerably improves SDK performance for certain scenarios.
* Attribute registration is now split into multiple HTTP requests to reduce the load on the backend. The threshold for splitting can be controlled with the `data-attributes_per_request` command.
* `ludeo_DataWriter_EnterObject` can now be nested. This is allowed just for convenience — it does not make the objects related in any way.

## Fixes

* Fixed a heap corruption in DataWriter/DataReader
* Video encoder: the encoder no longer resets when a game is minimized unless the resolution or format changed.
* Video encoder: the video ring buffer now grows in capacity during an ongoing highlight, to help minimize cases where the video is slightly shorter than requested.
* The SDK now sends extra information to the backend (first frame information) to help solve video vs data sync issues.

# v4.0.17

## Fixes

* Failed `ludeo_Session_Activate` calls now automatically perform resource cleanup instead of leaving everything for the `ludeo_Session_Release` call. The client still needs to call `ludeo_Session_Release` for every session created, regardless of the `ludeo_Session_Activate` result.
* The callback for `ludeo_Session_Release` is now the last callback called for the session, as documented.
* Fixed some WebSocket related memory leaks.
* Fixed some asserts and WebSocket issues related to calling `ludeo_Session_Release` before `ludeo_Session_Activate` completes.

# v4.0.16

## Improvements

* Improved network timeout detection. Timeouts are now based on lack of traffic rather than total operation duration, preventing false timeouts on slow connections.
* `ludeo_DataWriter_EnterObject` and `ludeo_DataReader_EnterObject` can now be nested. See the headers for more information.
* If videos are being uploaded when `ludeo_Session_Release` is called, the call now completes only once all uploads finish.
* `ludeo_Tick` now also logs its own self-time, making it easier to identify if it is a bottleneck.
* Other logging improvements.

## Fixes

* Temporary workaround for a divide-by-zero exception caused by Coherent's Gameface.

# v4.0.15

## Fixes

* Fixed a crash in the Gallery.
* Fixed a bug in video recording where the SDK was not using the start and end times as requested by the backend.
* Fixed a bug in video recording where the SDK was hardcoding the video framerate to 30fps.

# v4.0.14

## Breaking changes

* `LudeoSessionActivateParams` now requires a `gameVersion` field (non-null, non-empty string) when `apiVersion` is 3 or higher. This field is used to identify which game versions are compatible with a given ludeo.

## New features

* Added `gameVersion` parameter to `LudeoSessionActivateParams`, used to identify compatible game versions for each recorded ludeo.

## Improvements

* Added the `activation-ludeoid` ludeo command, which allows specifying a ludeo ID to use on the next `ludeo_Session_Activate` call. Useful for testing a specific ludeo during development.

## Fixes

* Fixed a crash when using `ludeo_Shutdown`.

# v4.0.13.1

## Fixes

* Fixed PS5 controller input handling.

# v4.0.13

## New features

* Tracy profiler is now integrated and can be used to analyze SDK performance.

# v4.0.12

## New features

* The SDK can now upload logs to the backend. If this feature is enabled in the platform, calls to `ludeo_SetLoggingLevel` are ignored since the backend controls verbosity.
* WebSocket connections are now retried a few times before considering a failure.
* Added support for DirectInput, allowing the overlay to be navigated with a gamepad.

# v4.0.11

## Fixes

* Fixed blob attribute size being wrongly calculated.

# v4.0.10

## New features

* Added a `fullName` field to `LudeoDataReaderVisitAttributesCallbackParams`, representing the attribute's full name.
* Added `ludeo_DataWriter_EnterObjectLate` — allows sending data with an older timestamp.
* Added `ludeo_GetCurrentTimestamp` — returns the current SDK tick time.

## Improvements

* Blob attributes larger than 100KB are now automatically split into smaller chunks, transparently to the client.
* `ludeo_DataWriter_SetBlob` now accepts a null pointer when size is 0.

# v4.0.9

## New features

* The authentication type (cloud, Steam, etc.) used by the SDK for session activation is now included in `LudeoSessionActivateCallbackParams`.

## Improvements

* Improved video capture FPS stability for low-FPS gameplay.

## Fixes

* Fixed inputs polled from XInput not being properly forwarded to the foreground window.

# v4.0.8

## New features

* Added a notification API for requests to mute/unmute the game.

## Fixes

* Fixed overlay not being reset when session activation fails.
* Fixed a potential crash when the process exits immediately after the SDK library is loaded.

# v4.0.7

## Fixes

* Fixed a race condition which can happen during overlay initialization.
* Fixed hwinfo dependencies not properly restoring locale state after changing it.

# v4.0.6

## Fixes

* Replaced a DirectX12 call that is not supported in Proton.
* Fixed a memory leak in the blob API.

# v4.0.5

## New features

* Introduced a new Blob data type for storing arbitrary binary data within recorded gameplay.
  * `ludeo_DataWriter_SetBlob` — records a binary blob attribute. Accepts any arbitrary data; note that if even a single byte changes compared to the previous value, the entire blob is re-sent, so blobs should be used judiciously.
  * `ludeo_DataReader_GetBlob` — reads back a blob attribute. Call `ludeo_DataReader_GetSize` first to get the blob size and allocate a sufficiently large buffer.

## Fixes

* Fixed a multi-threaded crash in overlay input handling.

# 4.0.4

## Improvements

* Update Coherent library version to 1.59.0.1 for the overlay
* Stability improvements for gamepad navigation in overlay.
* Error and Fatal log messages are now logged to stderr if logging was not set up.

## Fixes

* Fixed DirectX12 hooking crash.

# 4.0.3

## Improvements

* Overlay localization support API.
* Gamepad navigation in overlay for XInput supported devices.

# 4.0.2

## Improvements

* Tutorial videos (accessible through the Gallery) now have audio.
* Further improvements to the C headers documentation.
* Some minor improvements to what gets logged.

## Fixes

* Temporarily disabling NvEnc (Nvidia) based video encoder while we chase down some bugs. Machines with an NVidia card will use the software encoder for now.

# 4.0.1

## Improvements

* New video encoding solution

# 4.0.0

First Core SDK release
