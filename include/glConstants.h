#pragma once

#define OPENGLREVMAJOR	4
#define OPENGLREVMINOR	1

#ifdef __APPLE__

#if (OPENGLREVMAJOR > 2)
#define COREPROFILE	true
#else
#define COREPROFILE	false
#endif

#else // Not __APPLE__

#define COREPROFILE	false

#endif // if __APPLE__


#define FOV_DEFAULT     60.0
#define NEAR_DEFAULT	0.5
#define FAR_DEFAULT	1024.0
