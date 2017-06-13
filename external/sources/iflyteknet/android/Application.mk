APP_STL := gnustl_static

# Uncomment this line to compile to armeabi-v7a, your application will run faster but support less devices
APP_ABI := armeabi armeabi-v7a
#APP_ABI := armeabi

APP_CPPFLAGS := -frtti -DCC_ENABLE_BULLET_INTEGRATION=1 -std=c++11 -fsigned-char -fexceptions
APP_LDFLAGS := -latomic
#APP_PLATFORM := android-9

NDK_MODULE_PATH=../../../../;../../../../external/;../../../../external/sources/iflyteknet/src
USE_ARM_MODE := 1

ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif
