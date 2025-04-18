#ifndef Engine_Macro_h__
#define Engine_Macro_h__

namespace Engine
{
#define IDM_GAME_PLAY    20001
#define IDM_GAME_PAUSE    20002
#define IDM_GAME_STOP     20003

#define ID_BTN_PLAY   30001
#define ID_BTN_PAUSE  30002
#define ID_BTN_STOP   30003
#define ID_BTN_NEXTFRAME 30004

#define	VTXCNTX		129
#define VTXCNTZ		129
#define VTXITV		1

#ifndef			MSG_BOX
#define			MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#endif

#define			BEGIN(NAMESPACE)		namespace NAMESPACE {
#define			END						}

#define			USING(NAMESPACE)	using namespace NAMESPACE;

#ifdef	ENGINE_EXPORTS
#define ENGINE_DLL		_declspec(dllexport)
#else
#define ENGINE_DLL		_declspec(dllimport)
#endif

#define NO_COPY(CLASSNAME)								\
		private:										\
		CLASSNAME(const CLASSNAME&) = delete;			\
		CLASSNAME& operator = (const CLASSNAME&)= delete;		

#define SINGLETONCLASS(c) \
private: \
    c(); \
    c(const c& rhs) = delete; \
    c& operator=(const c& rhs) = delete; \
    ~c(); \
public: \
    static c& GetInstance() \
    { \
        static c instance; \
        return instance; \
    } \
private: \
    enum { __singleton_dummy = 0 }

	template <typename T>
	T clamp(T value, T min, T max)
	{
		return (value < min) ? min : (value > max ? max : value);
	}
}

#endif // Engine_Macro_h__
