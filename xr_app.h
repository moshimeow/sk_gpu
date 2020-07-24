#pragma once

// OpenXR platform defines
#ifdef __ANDROID__
#define XR_USE_PLATFORM_ANDROID
#define XR_USE_GRAPHICS_API_OPENGL_ES
#elif _WIN32
#define XR_USE_PLATFORM_WIN32
//#define XR_USE_GRAPHICS_API_D3D11
#define XR_USE_GRAPHICS_API_OPENGL
#endif

#if defined(XR_USE_GRAPHICS_API_D3D11)
#define XR_GFX_EXTENSION XR_KHR_D3D11_ENABLE_EXTENSION_NAME
#define XrSwapchainImage XrSwapchainImageD3D11KHR
#define XR_TYPE_SWAPCHAIN_IMAGE XR_TYPE_SWAPCHAIN_IMAGE_D3D11_KHR
#define XrGraphicsRequirements XrGraphicsRequirementsD3D11KHR
#define XR_TYPE_GRAPHICS_REQUIREMENTS XR_TYPE_GRAPHICS_REQUIREMENTS_D3D11_KHR
#define PFN_xrGetGraphicsRequirementsKHR PFN_xrGetD3D11GraphicsRequirementsKHR
#define NAME_xrGetGraphicsRequirementsKHR "xrGetD3D11GraphicsRequirementsKHR"
#define XrGraphicsBinding XrGraphicsBindingD3D11KHR
#define XR_TYPE_GRAPHICS_BINDING XR_TYPE_GRAPHICS_BINDING_D3D11_KHR

#elif defined(XR_USE_GRAPHICS_API_OPENGL)
#define XR_GFX_EXTENSION XR_KHR_OPENGL_ENABLE_EXTENSION_NAME
#define XrSwapchainImage XrSwapchainImageOpenGLKHR
#define XR_TYPE_SWAPCHAIN_IMAGE XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_KHR
#define XrGraphicsRequirements XrGraphicsRequirementsOpenGLKHR
#define XR_TYPE_GRAPHICS_REQUIREMENTS XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_KHR
#define PFN_xrGetGraphicsRequirementsKHR PFN_xrGetOpenGLGraphicsRequirementsKHR
#define NAME_xrGetGraphicsRequirementsKHR "xrGetOpenGLGraphicsRequirementsKHR"
#define XrGraphicsBinding XrGraphicsBindingOpenGLWin32KHR
#define XR_TYPE_GRAPHICS_BINDING XR_TYPE_GRAPHICS_BINDING_OPENGL_WIN32_KHR

#elif defined(XR_USE_GRAPHICS_API_VULKAN)
#define XR_GFX_EXTENSION XR_KHR_VULKAN_ENABLE_EXTENSION_NAME
#define XrSwapchainImage XrSwapchainImageVulkanKHR
#define XR_TYPE_SWAPCHAIN_IMAGE XR_TYPE_SWAPCHAIN_IMAGE_VULKAN_KHR
#define XrGraphicsRequirements XrGraphicsRequirementsVulkanKHR
#define XR_TYPE_GRAPHICS_REQUIREMENTS XR_TYPE_GRAPHICS_REQUIREMENTS_VULKAN_KHR
#define PFN_xrGetGraphicsRequirementsKHR PFN_xrGetVulkanGraphicsRequirementsKHR
#define NAME_xrGetGraphicsRequirementsKHR "xrGetVulkanGraphicsRequirementsKHR"
#define XrGraphicsBinding XrGraphicsBindingVulkanKHR
#define XR_TYPE_GRAPHICS_BINDING XR_TYPE_GRAPHICS_BINDING_VULKAN_KHR

#elif defined(XR_USE_GRAPHICS_API_OPENGL_ES)
#define XR_GFX_EXTENSION XR_KHR_OPENGL_ES_ENABLE_EXTENSION_NAME
#define XrSwapchainImage XrSwapchainImageOpenGLESKHR
#define XR_TYPE_SWAPCHAIN_IMAGE XR_TYPE_SWAPCHAIN_IMAGE_OPENGLES_KHR
#define XrGraphicsRequirements XrGraphicsRequirementsOpenGLESKHR
#define XR_TYPE_GRAPHICS_REQUIREMENTS XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGLES_KHR
#define PFN_xrGetGraphicsRequirementsKHR PFN_xrGetOpenGLESGraphicsRequirementsKHR
#define NAME_xrGetGraphicsRequirementsKHR "xrGetOpenGLESGraphicsRequirementsKHR"
#define XrGraphicsBinding XrGraphicsBindingOpenGLESKHR
#define XR_TYPE_GRAPHICS_BINDING XR_TYPE_GRAPHICS_BINDING_OPENGLES_KHR

#endif

///////////////////////////////////////////

#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>

///////////////////////////////////////////

struct xr_callbacks_t {
	void *user_data;
	bool (*init_gfx)         (void *user_data, const XrGraphicsRequirements *requirements, XrGraphicsBinding *out_graphics);
	bool (*init_swapchain)   (void *user_data, int32_t view_count, int32_t surface_count, void **textures_view_surface, int32_t width, int32_t height, int64_t fmt);
	void (*draw)             (void *user_data, const XrCompositionLayerProjectionView *view, int32_t swapchain_view_id, int32_t swapchain_surface_id);
	void (*destroy_swapchain)(void *user_data);

	int64_t *pixel_formats;
	int32_t  pixel_format_count;
	int64_t *depth_formats;
	int32_t  depth_format_count;
};

///////////////////////////////////////////

bool openxr_init (const char *app_name, xr_callbacks_t *callbacks, void *hwnd);
bool openxr_step ();

///////////////////////////////////////////

#ifdef XR_APP_IMPL

#include <vector>

///////////////////////////////////////////

struct swapchain_t {
	XrSwapchain handle;
	int32_t     width;
	int32_t     height;
	std::vector<XrSwapchainImage> surface_images;
};

struct input_state_t {
	XrActionSet actionSet;
	XrAction    poseAction;
	XrAction    selectAction;
	XrPath      handSubactionPath[2];
	XrSpace     handSpace[2];
	XrPosef     handPose[2];
	XrBool32    renderHand[2];
	XrBool32    handSelect[2];
};

///////////////////////////////////////////

XrFormFactor            app_config_form = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
XrViewConfigurationType app_config_view = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;

const XrPosef          xr_pose_identity = { {0,0,0,1}, {0,0,0} };
XrInstance             xr_instance      = {};
XrSession              xr_session       = {};
XrSessionState         xr_session_state = XR_SESSION_STATE_UNKNOWN;
bool                   xr_running       = false;
XrSpace                xr_app_space     = {};
XrSystemId             xr_system_id     = XR_NULL_SYSTEM_ID;
input_state_t          xr_input         = { };
XrEnvironmentBlendMode xr_blend;
xr_callbacks_t         xr_callbacks = {};

std::vector<XrView>                  xr_views;
std::vector<XrViewConfigurationView> xr_config_views;
std::vector<swapchain_t>             xr_swapchains;

PFN_xrGetGraphicsRequirementsKHR ext_xrGetGraphicsRequirementsKHR;

///////////////////////////////////////////

void openxr_make_actions    ();
void openxr_shutdown        ();
void openxr_poll_events     (bool &exit);
void openxr_poll_actions    ();
void openxr_poll_predicted  (XrTime predicted_time);
void openxr_render_frame    ();
bool openxr_render_layer    (XrTime predictedTime, std::vector<XrCompositionLayerProjectionView> &projectionViews, XrCompositionLayerProjection &layer);
void openxr_preferred_format(const int64_t *pixel_fmt_options, int32_t pixel_fmt_count, const int64_t *depth_fmt_options, int32_t depth_fmt_count, int64_t &out_color, int64_t &out_depth);

///////////////////////////////////////////
// OpenXR code                           //
///////////////////////////////////////////

bool openxr_init(const char *app_name, xr_callbacks_t *callbacks, void *hwnd) {
	xr_callbacks = *callbacks;

	const char          *extensions[] = { XR_GFX_EXTENSION };
	XrInstanceCreateInfo createInfo   = { XR_TYPE_INSTANCE_CREATE_INFO };
	createInfo.enabledExtensionCount      = _countof(extensions);
	createInfo.enabledExtensionNames      = extensions;
	createInfo.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;
	strcpy_s(createInfo.applicationInfo.applicationName, 128, app_name);
	xrCreateInstance(&createInfo, &xr_instance);

	// Check if OpenXR is on this system, if this is null here, the user needs to install an
	// OpenXR runtime and ensure it's active!
	if (xr_instance == nullptr)
		return false;

	// Load extension methods that we'll need for this application! There's a
	// couple ways to do this, and this is a fairly manual one. Chek out this
	// file for another way to do it:
	// https://github.com/maluoi/StereoKit/blob/master/StereoKitC/systems/platform/openxr_extensions.h
	xrGetInstanceProcAddr(
		xr_instance,
		NAME_xrGetGraphicsRequirementsKHR,
		(PFN_xrVoidFunction *)(&ext_xrGetGraphicsRequirementsKHR));

	// Request a form factor from the device (HMD, Handheld, etc.)
	XrSystemGetInfo systemInfo = { XR_TYPE_SYSTEM_GET_INFO };
	systemInfo.formFactor = app_config_form;
	xrGetSystem(xr_instance, &systemInfo, &xr_system_id);

	// Check what blend mode is valid for this device (opaque vs transparent displays)
	// We'll just take the first one available!
	uint32_t blend_count = 0;
	xrEnumerateEnvironmentBlendModes(xr_instance, xr_system_id, app_config_view, 1, &blend_count, &xr_blend);

	// OpenXR wants to ensure apps are using the correct graphics card, so this MUST be called 
	// before xrCreateSession. This is crucial on devices that have multiple graphics cards, 
	// like laptops with integrated graphics chips in addition to dedicated graphics cards.
	XrGraphicsRequirements requirement = { XR_TYPE_GRAPHICS_REQUIREMENTS };
	ext_xrGetGraphicsRequirementsKHR(xr_instance, xr_system_id, &requirement);

	XrGraphicsBinding binding = { XR_TYPE_GRAPHICS_BINDING };
	if (!callbacks->init_gfx(callbacks->user_data, &requirement, &binding))
		return false;

	XrSessionCreateInfo sessionInfo = { XR_TYPE_SESSION_CREATE_INFO };
	sessionInfo.next     = &binding;
	sessionInfo.systemId = xr_system_id;
	xrCreateSession(xr_instance, &sessionInfo, &xr_session);

	// Unable to start a session, may not have an MR device attached or ready
	if (xr_session == nullptr)
		return false;

	int64_t pixel_fmt, depth_fmt;
	openxr_preferred_format(xr_callbacks.pixel_formats, xr_callbacks.pixel_format_count, xr_callbacks.depth_formats, xr_callbacks.depth_format_count, pixel_fmt, depth_fmt);

	// OpenXR uses a couple different types of reference frames for positioning content, we need to choose one for
	// displaying our content! STAGE would be relative to the center of your guardian system's bounds, and LOCAL
	// would be relative to your device's starting location. HoloLens doesn't have a STAGE, so we'll use LOCAL.
	XrReferenceSpaceCreateInfo ref_space = { XR_TYPE_REFERENCE_SPACE_CREATE_INFO };
	ref_space.poseInReferenceSpace = xr_pose_identity;
	ref_space.referenceSpaceType   = XR_REFERENCE_SPACE_TYPE_LOCAL;
	xrCreateReferenceSpace(xr_session, &ref_space, &xr_app_space);

	// Now we need to find all the viewpoints we need to take care of! For a stereo headset, this should be 2.
	// Similarly, for an AR phone, we'll need 1, and a VR cave could have 6, or even 12!
	uint32_t view_count = 0;
	uint32_t surface_count = 0;
	xrEnumerateViewConfigurationViews(xr_instance, xr_system_id, app_config_view, 0, &view_count, nullptr);
	xr_config_views.resize(view_count, { XR_TYPE_VIEW_CONFIGURATION_VIEW });
	xr_views       .resize(view_count, { XR_TYPE_VIEW });
	xrEnumerateViewConfigurationViews(xr_instance, xr_system_id, app_config_view, view_count, &view_count, xr_config_views.data());
	void **textures = nullptr;
	for (uint32_t i = 0; i < view_count; i++) {
		// Create a swapchain for this viewpoint! A swapchain is a set of texture buffers used for displaying to screen,
		// typically this is a backbuffer and a front buffer, one for rendering data to, and one for displaying on-screen.
		// A note about swapchain image format here! OpenXR doesn't create a concrete image format for the texture, like 
		// DXGI_FORMAT_R8G8B8A8_UNORM. Instead, it switches to the TYPELESS variant of the provided texture format, like 
		// DXGI_FORMAT_R8G8B8A8_TYPELESS. When creating an ID3D11RenderTargetView for the swapchain texture, we must specify
		// a concrete type like DXGI_FORMAT_R8G8B8A8_UNORM, as attempting to create a TYPELESS view will throw errors, so 
		// we do need to store the format separately and remember it later.
		XrViewConfigurationView &view           = xr_config_views[i];
		XrSwapchainCreateInfo    swapchain_info = { XR_TYPE_SWAPCHAIN_CREATE_INFO };
		XrSwapchain              handle;
		swapchain_info.arraySize   = 1;
		swapchain_info.mipCount    = 1;
		swapchain_info.faceCount   = 1;
		swapchain_info.format      = pixel_fmt;
		swapchain_info.width       = view.recommendedImageRectWidth;
		swapchain_info.height      = view.recommendedImageRectHeight;
		swapchain_info.sampleCount = view.recommendedSwapchainSampleCount;
		swapchain_info.usageFlags  = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT;
		xrCreateSwapchain(xr_session, &swapchain_info, &handle);

		// Find out how many textures were generated for the swapchain
		
		xrEnumerateSwapchainImages(handle, 0, &surface_count, nullptr);
		if (textures == nullptr)
			textures = (void**)malloc(sizeof(void *) * surface_count * view_count);

		// We'll want to track our own information about the swapchain, so we can draw stuff onto it! We'll also create
		// a depth buffer for each generated texture here as well with make_surfacedata.
		swapchain_t swapchain = {};
		swapchain.width  = swapchain_info.width;
		swapchain.height = swapchain_info.height;
		swapchain.handle = handle;
		swapchain.surface_images.resize(surface_count, XrSwapchainImage{ XR_TYPE_SWAPCHAIN_IMAGE } );
		xrEnumerateSwapchainImages(swapchain.handle, surface_count, &surface_count, (XrSwapchainImageBaseHeader*)swapchain.surface_images.data());
		
		for (uint32_t s = 0; s < surface_count; s++) {
#if defined(XR_USE_GRAPHICS_API_D3D11)
			void *tex = swapchain.surface_images[s].texture;
#elif defined(XR_USE_GRAPHICS_API_OPENGL_ES) || defined(XR_USE_GRAPHICS_API_OPENGL)
			void *tex = (void*)swapchain.surface_images[s].image;
#endif
			textures[i*surface_count + s] = tex;
		}
		xr_swapchains.push_back(swapchain);
	}
	callbacks->init_swapchain(callbacks->user_data, view_count, surface_count, textures, xr_swapchains[0].width, xr_swapchains[0].height, pixel_fmt);
	free(textures);

	return true;
}

///////////////////////////////////////////

void openxr_preferred_format(const int64_t *pixel_fmt_options, int32_t pixel_fmt_count, const int64_t *depth_fmt_options, int32_t depth_fmt_count, int64_t &out_color, int64_t &out_depth) {
	// Get the list of formats OpenXR would like
	uint32_t count = 0;
	xrEnumerateSwapchainFormats(xr_session, 0, &count, nullptr);
	int64_t *formats = (int64_t *)malloc(sizeof(int64_t) * count);
	xrEnumerateSwapchainFormats(xr_session, count, &count, formats);

	// Check those against our formats
	out_color = 0;
	out_depth = 0;
	for (uint32_t i=0; i<count; i++) {
		for (int32_t f=0; out_color == 0 && f<pixel_fmt_count; f++) {
			if (formats[i] == pixel_fmt_options[f]) {
				out_color = pixel_fmt_options[f];
				break;
			}
		}
		for (int32_t f=0; out_depth == 0 && f<depth_fmt_count; f++) {
			if (formats[i] == depth_fmt_options[f]) {
				out_depth = depth_fmt_options[f];
				break;
			}
		}
	}

	// Release memory
	free(formats);
}

///////////////////////////////////////////

void openxr_make_actions() {
	XrActionSetCreateInfo actionset_info = { XR_TYPE_ACTION_SET_CREATE_INFO };
	strcpy_s(actionset_info.actionSetName,          "gameplay");
	strcpy_s(actionset_info.localizedActionSetName, "Gameplay");
	xrCreateActionSet(xr_instance, &actionset_info, &xr_input.actionSet);
	xrStringToPath(xr_instance, "/user/hand/left",  &xr_input.handSubactionPath[0]);
	xrStringToPath(xr_instance, "/user/hand/right", &xr_input.handSubactionPath[1]);

	// Create an action to track the position and orientation of the hands! This is
	// the controller location, or the center of the palms for actual hands.
	XrActionCreateInfo action_info = { XR_TYPE_ACTION_CREATE_INFO };
	action_info.countSubactionPaths = _countof(xr_input.handSubactionPath);
	action_info.subactionPaths      = xr_input.handSubactionPath;
	action_info.actionType          = XR_ACTION_TYPE_POSE_INPUT;
	strcpy_s(action_info.actionName,          "hand_pose");
	strcpy_s(action_info.localizedActionName, "Hand Pose");
	xrCreateAction(xr_input.actionSet, &action_info, &xr_input.poseAction);

	// Create an action for listening to the select action! This is primary trigger
	// on controllers, and an airtap on HoloLens
	action_info.actionType = XR_ACTION_TYPE_BOOLEAN_INPUT;
	strcpy_s(action_info.actionName,          "select");
	strcpy_s(action_info.localizedActionName, "Select");
	xrCreateAction(xr_input.actionSet, &action_info, &xr_input.selectAction);

	// Bind the actions we just created to specific locations on the Khronos simple_controller
	// definition! These are labeled as 'suggested' because they may be overridden by the runtime
	// preferences. For example, if the runtime allows you to remap buttons, or provides input
	// accessibility settings.
	XrPath profile_path;
	XrPath pose_path  [2];
	XrPath select_path[2];
	xrStringToPath(xr_instance, "/user/hand/left/input/grip/pose",     &pose_path[0]);
	xrStringToPath(xr_instance, "/user/hand/right/input/grip/pose",    &pose_path[1]);
	xrStringToPath(xr_instance, "/user/hand/left/input/select/click",  &select_path[0]);
	xrStringToPath(xr_instance, "/user/hand/right/input/select/click", &select_path[1]);
	xrStringToPath(xr_instance, "/interaction_profiles/khr/simple_controller", &profile_path);
	XrActionSuggestedBinding bindings[] = {
		{ xr_input.poseAction,   pose_path[0]   },
		{ xr_input.poseAction,   pose_path[1]   },
		{ xr_input.selectAction, select_path[0] },
		{ xr_input.selectAction, select_path[1] }, };
	XrInteractionProfileSuggestedBinding suggested_binds = { XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING };
	suggested_binds.interactionProfile     = profile_path;
	suggested_binds.suggestedBindings      = &bindings[0];
	suggested_binds.countSuggestedBindings = _countof(bindings);
	xrSuggestInteractionProfileBindings(xr_instance, &suggested_binds);

	// Create frames of reference for the pose actions
	for (int32_t i = 0; i < 2; i++) {
		XrActionSpaceCreateInfo action_space_info = { XR_TYPE_ACTION_SPACE_CREATE_INFO };
		action_space_info.action            = xr_input.poseAction;
		action_space_info.poseInActionSpace = xr_pose_identity;
		action_space_info.subactionPath     = xr_input.handSubactionPath[i];
		xrCreateActionSpace(xr_session, &action_space_info, &xr_input.handSpace[i]);
	}

	// Attach the action set we just made to the session
	XrSessionActionSetsAttachInfo attach_info = { XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO };
	attach_info.countActionSets = 1;
	attach_info.actionSets      = &xr_input.actionSet;
	xrAttachSessionActionSets(xr_session, &attach_info);
}

///////////////////////////////////////////

void openxr_shutdown() {
	// We used a graphics API to initialize the swapchain data, so we'll
	// give it a chance to release anythig here!
	for (int32_t i = 0; i < xr_swapchains.size(); i++) {
		xrDestroySwapchain(xr_swapchains[i].handle);
	}
	xr_swapchains.clear();
	xr_callbacks.destroy_swapchain(xr_callbacks.user_data);

	// Release all the other OpenXR resources that we've created!
	// What gets allocated, must get deallocated!
	if (xr_input.actionSet != XR_NULL_HANDLE) {
		if (xr_input.handSpace[0] != XR_NULL_HANDLE) xrDestroySpace(xr_input.handSpace[0]);
		if (xr_input.handSpace[1] != XR_NULL_HANDLE) xrDestroySpace(xr_input.handSpace[1]);
		xrDestroyActionSet(xr_input.actionSet);
	}
	if (xr_app_space != XR_NULL_HANDLE) xrDestroySpace   (xr_app_space);
	if (xr_session   != XR_NULL_HANDLE) xrDestroySession (xr_session);
	if (xr_instance  != XR_NULL_HANDLE) xrDestroyInstance(xr_instance);
}

///////////////////////////////////////////

void openxr_poll_events(bool &exit) {
	exit = false;

	XrEventDataBuffer event_buffer = { XR_TYPE_EVENT_DATA_BUFFER };

	while (xrPollEvent(xr_instance, &event_buffer) == XR_SUCCESS) {
		switch (event_buffer.type) {
		case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED: {
			XrEventDataSessionStateChanged *changed = (XrEventDataSessionStateChanged*)&event_buffer;
			xr_session_state = changed->state;

			// Session state change is where we can begin and end sessions, as well as find quit messages!
			switch (xr_session_state) {
			case XR_SESSION_STATE_READY: {
				XrSessionBeginInfo begin_info = { XR_TYPE_SESSION_BEGIN_INFO };
				begin_info.primaryViewConfigurationType = app_config_view;
				xrBeginSession(xr_session, &begin_info);
				xr_running = true;
			} break;
			case XR_SESSION_STATE_STOPPING: {
				xr_running = false;
				xrEndSession(xr_session); 
			} break;
			case XR_SESSION_STATE_EXITING:      exit = true;              break;
			case XR_SESSION_STATE_LOSS_PENDING: exit = true;              break;
			}
		} break;
		case XR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING: exit = true; return;
		}
		event_buffer = { XR_TYPE_EVENT_DATA_BUFFER };
	}
}

///////////////////////////////////////////

void openxr_poll_actions() {
	if (xr_session_state != XR_SESSION_STATE_FOCUSED)
		return;

	// Update our action set with up-to-date input data!
	XrActiveActionSet action_set = { };
	action_set.actionSet     = xr_input.actionSet;
	action_set.subactionPath = XR_NULL_PATH;

	XrActionsSyncInfo sync_info = { XR_TYPE_ACTIONS_SYNC_INFO };
	sync_info.countActiveActionSets = 1;
	sync_info.activeActionSets      = &action_set;

	xrSyncActions(xr_session, &sync_info);

	// Now we'll get the current states of our actions, and store them for later use
	for (uint32_t hand = 0; hand < 2; hand++) {
		XrActionStateGetInfo get_info = { XR_TYPE_ACTION_STATE_GET_INFO };
		get_info.subactionPath = xr_input.handSubactionPath[hand];

		XrActionStatePose pose_state = { XR_TYPE_ACTION_STATE_POSE };
		get_info.action = xr_input.poseAction;
		xrGetActionStatePose(xr_session, &get_info, &pose_state);
		xr_input.renderHand[hand] = pose_state.isActive;

		// Events come with a timestamp
		XrActionStateBoolean select_state = { XR_TYPE_ACTION_STATE_BOOLEAN };
		get_info.action = xr_input.selectAction;
		xrGetActionStateBoolean(xr_session, &get_info, &select_state);
		xr_input.handSelect[hand] = select_state.currentState && select_state.changedSinceLastSync;

		// If we have a select event, update the hand pose to match the event's timestamp
		if (xr_input.handSelect[hand]) {
			XrSpaceLocation space_location = { XR_TYPE_SPACE_LOCATION };
			XrResult        res            = xrLocateSpace(xr_input.handSpace[hand], xr_app_space, select_state.lastChangeTime, &space_location);
			if (XR_UNQUALIFIED_SUCCESS(res) &&
				(space_location.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT   ) != 0 &&
				(space_location.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) != 0) {
				xr_input.handPose[hand] = space_location.pose;
			}
		}
	}
}

///////////////////////////////////////////

void openxr_poll_predicted(XrTime predicted_time) {
	if (xr_session_state != XR_SESSION_STATE_FOCUSED)
		return;

	// Update hand position based on the predicted time of when the frame will be rendered! This 
	// should result in a more accurate location, and reduce perceived lag.
	for (size_t i = 0; i < 2; i++) {
		if (!xr_input.renderHand[i])
			continue;
		XrSpaceLocation spaceRelation = { XR_TYPE_SPACE_LOCATION };
		XrResult        res           = xrLocateSpace(xr_input.handSpace[i], xr_app_space, predicted_time, &spaceRelation);
		if (XR_UNQUALIFIED_SUCCESS(res) &&
			(spaceRelation.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT   ) != 0 &&
			(spaceRelation.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) != 0) {
			xr_input.handPose[i] = spaceRelation.pose;
		}
	}
}

///////////////////////////////////////////

void openxr_render_frame() {
	// Block until the previous frame is finished displaying, and is ready for another one.
	// Also returns a prediction of when the next frame will be displayed, for use with predicting
	// locations of controllers, viewpoints, etc.
	XrFrameState frame_state = { XR_TYPE_FRAME_STATE };
	xrWaitFrame (xr_session, nullptr, &frame_state);
	// Must be called before any rendering is done! This can return some interesting flags, like 
	// XR_SESSION_VISIBILITY_UNAVAILABLE, which means we could skip rendering this frame and call
	// xrEndFrame right away.
	xrBeginFrame(xr_session, nullptr);

	// Execute any code that's dependant on the predicted time, such as updating the location of
	// controller models.
	openxr_poll_predicted(frame_state.predictedDisplayTime);

	// If the session is active, lets render our layer in the compositor!
	XrCompositionLayerBaseHeader            *layer      = nullptr;
	XrCompositionLayerProjection             layer_proj = { XR_TYPE_COMPOSITION_LAYER_PROJECTION };
	std::vector<XrCompositionLayerProjectionView> views;
	bool session_active = xr_session_state == XR_SESSION_STATE_VISIBLE || xr_session_state == XR_SESSION_STATE_FOCUSED;
	if (session_active && openxr_render_layer(frame_state.predictedDisplayTime, views, layer_proj)) {
		layer = (XrCompositionLayerBaseHeader*)&layer_proj;
	}

	// We're finished with rendering our layer, so send it off for display!
	XrFrameEndInfo end_info{ XR_TYPE_FRAME_END_INFO };
	end_info.displayTime          = frame_state.predictedDisplayTime;
	end_info.environmentBlendMode = xr_blend;
	end_info.layerCount           = layer == nullptr ? 0 : 1;
	end_info.layers               = &layer;
	xrEndFrame(xr_session, &end_info);
}

///////////////////////////////////////////

bool openxr_render_layer(XrTime predictedTime, std::vector<XrCompositionLayerProjectionView> &views, XrCompositionLayerProjection &layer) {

	// Find the state and location of each viewpoint at the predicted time
	uint32_t         view_count  = 0;
	XrViewState      view_state  = { XR_TYPE_VIEW_STATE };
	XrViewLocateInfo locate_info = { XR_TYPE_VIEW_LOCATE_INFO };
	locate_info.viewConfigurationType = app_config_view;
	locate_info.displayTime           = predictedTime;
	locate_info.space                 = xr_app_space;
	xrLocateViews(xr_session, &locate_info, &view_state, (uint32_t)xr_views.size(), &view_count, xr_views.data());
	views.resize(view_count);

	// And now we'll iterate through each viewpoint, and render it!
	for (uint32_t i = 0; i < view_count; i++) {

		// We need to ask which swapchain image to use for rendering! Which one will we get?
		// Who knows! It's up to the runtime to decide.
		uint32_t                    img_id = 0;
		XrSwapchainImageAcquireInfo acquire_info = { XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO };
		xrAcquireSwapchainImage(xr_swapchains[i].handle, &acquire_info, &img_id);

		// Wait until the image is available to render to. The compositor could still be
		// reading from it.
		XrSwapchainImageWaitInfo wait_info = { XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO };
		wait_info.timeout = XR_INFINITE_DURATION;
		xrWaitSwapchainImage(xr_swapchains[i].handle, &wait_info);

		// Set up our rendering information for the viewpoint we're using right now!
		views[i] = { XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW };
		views[i].pose = xr_views[i].pose;
		views[i].fov  = xr_views[i].fov;
		views[i].subImage.swapchain        = xr_swapchains[i].handle;
		views[i].subImage.imageRect.offset = { 0, 0 };
		views[i].subImage.imageRect.extent = { xr_swapchains[i].width, xr_swapchains[i].height };

		// Call the rendering callback with our view and swapchain info
		xr_callbacks.draw(xr_callbacks.user_data, &views[i], i, img_id);

		// And tell OpenXR we're done with rendering to this one!
		XrSwapchainImageReleaseInfo release_info = { XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO };
		xrReleaseSwapchainImage(xr_swapchains[i].handle, &release_info);
	}

	layer.space     = xr_app_space;
	layer.viewCount = (uint32_t)views.size();
	layer.views     = views.data();
	return true;
}

///////////////////////////////////////////

bool openxr_step() {
	bool quit = false;
	openxr_poll_events(quit);

	if (xr_running) {
		openxr_poll_actions();
		openxr_render_frame();
	}
	return !quit;
}

#endif