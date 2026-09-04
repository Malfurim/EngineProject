#pragma once

#ifndef _SAFE_DELETE_
#define SAFE_DELETE(p) if(p) { delete (p); (p) = nullptr; }
#endif

#ifndef _SAFE_DELETE_ARRAY_
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] (p); (p) = nullptr; }
#endif

#ifndef _SAFE_RELEASE_
#define SAFE_RELEASE(p) if(p) { (p)->Release(); (p) = nullptr; }
#endif

#ifndef _SAFE_DELETE_VECTOR_
#define SAFE_DELETE_VECTOR(p) for (auto* resource : (p)) { delete resource;	} (p).clear();
#endif