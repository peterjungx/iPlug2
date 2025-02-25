#define PLUG_NAME "MOXplore"
#define PLUG_MFR "PitchGrid"
#define PLUG_VERSION_HEX 0x00010000
#define PLUG_VERSION_STR "1.0.0"
#define PLUG_UNIQUE_ID 'ZxEZ'
#define PLUG_MFR_ID 'Acme'
#define PLUG_URL_STR "https://iplug2.github.io"
#define PLUG_EMAIL_STR "spam@me.com"
#define PLUG_COPYRIGHT_STR "Copyright 2020 Acme Inc"
#define PLUG_CLASS_NAME MOXplore

#define BUNDLE_NAME "MOXplore"
#define BUNDLE_MFR "PitchGrid"
#define BUNDLE_DOMAIN "com"

#define SHARED_RESOURCES_SUBPATH "MOXplore"

#ifdef APP_API
#define PLUG_CHANNEL_IO "1-2"
#else
#define PLUG_CHANNEL_IO "1-1 1-2 2-2"
#endif

#define PLUG_LATENCY 0
#define PLUG_TYPE 0
#define PLUG_DOES_MIDI_IN 1
#define PLUG_DOES_MIDI_OUT 1
#define PLUG_DOES_MPE 0
#define PLUG_DOES_STATE_CHUNKS 0
#define PLUG_HAS_UI 1
#define PLUG_WIDTH 980
#define PLUG_HEIGHT 760
#define PLUG_FPS 60
#define PLUG_SHARED_RESOURCES 0
#define PLUG_HOST_RESIZE 1

#define AUV2_ENTRY MOXplore_Entry
#define AUV2_ENTRY_STR "MOXplore_Entry"
#define AUV2_FACTORY MOXplore_Factory
#define AUV2_VIEW_CLASS MOXplore_View
#define AUV2_VIEW_CLASS_STR "MOXplore_View"

#define AAX_TYPE_IDS 'IPWV'
#define AAX_TYPE_IDS_AUDIOSUITE 'IPWA'
#define AAX_PLUG_MFR_STR "Acme"
#define AAX_PLUG_NAME_STR "MOXplore\nIPEF"
#define AAX_PLUG_CATEGORY_STR "Effect"
#define AAX_DOES_AUDIOSUITE 1

#define VST3_SUBCATEGORY "Fx"

#define APP_NUM_CHANNELS 2
#define APP_N_VECTOR_WAIT 0
#define APP_MULT 1
#define APP_COPY_AUV3 0
#define APP_SIGNAL_VECTOR_SIZE 64
