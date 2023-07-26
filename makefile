# Define target platform: PLATFORM_DESKTOP, PLATFORM_RPI, PLATFORM_DRM, PLATFORM_ANDROID, PLATFORM_WEB
PLATFORM ?= PLATFORM_WEB

all:
	+$(MAKE) -C ./libs/raylib/src PLATFORM="$(PLATFORM)"
	+$(MAKE) -C ./src PLATFORM="$(PLATFORM)"

clean:
	+$(MAKE) clean -C ./libs/raylib/src
	+$(MAKE) clean -C ./src
