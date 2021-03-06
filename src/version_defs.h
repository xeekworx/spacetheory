#ifdef _WIN64
#define SPACETHEORY_ARCH        64
#define SPACETHEORY_ARCH_STR    "x64"
#else
#define SPACETHEORY_ARCH        32
#define SPACETHEORY_ARCH_STR    "x86"
#endif
#ifdef _DEBUG
#define SPACETHEORY_CONFIG "Debug"
#else
#define SPACETHEORY_CONFIG "Release"
#endif

#define SPACETHEORY_DEVSTAGE            "Alpha"
#define SPACETHEORY_BUILDDATE           1520711919
#define SPACETHEORY_PRODUCTVERSION      0,18,31,195
#define SPACETHEORY_STRPRODUCTVERSION   "0.18.31.195\0"
#define SPACETHEORY_FILEVERSION         SPACETHEORY_PRODUCTVERSION
#define SPACETHEORY_STRFILEVERSION      SPACETHEORY_STRPRODUCTVERSION
#define SPACETHEORY_FILEDESC            "Space Theory Game Engine"
#define SPACETHEORY_PRODUCTNAME         "Space Theory\0"
#define SPACETHEORY_AUTHOR              "John A. Tullos\0"
#define SPACETHEORY_EMAIL               "xeek@xeekworx.com\0"
#define SPACETHEORY_WWW                 "http://xeekworx.com/"
#define SPACETHEORY_GIT                 "https://github.com/xeekworx/spacetheory"
#define SPACETHEORY_COMPANY             "Xeekworx\0"
#define SPACETHEORY_COPYRIGHT           "Copyright � 2018 John Tullos. All Rights Reserved.\0"
#define SPACETHEORY_COPYRIGHT_SHORT     "Copyright � 2018 John Tullos.\0"

#define SPACETHEORY_LOGFILE             "engine.log"

// KEEP EXTRA SPACES BELOW THIS LINE, IT'S A BUG WITH RESOURCE FILES INCLUDING
// THIS HEADER CAUSING A "UNEXPECTED END OF FILE FOUND" ERROR
  