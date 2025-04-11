#ifndef Engine_Macro_h__
#define Engine_Macro_h__

namespace Engine
{
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

#define DECLARE_SINGLETON(CLASSNAME)					\
		NO_COPY(CLASSNAME)								\
		private:										\
		static CLASSNAME*	m_pInstance;				\
		public:											\
		static CLASSNAME*	GetInstance( void );		\
		static void DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)					\
		CLASSNAME*	CLASSNAME::m_pInstance = NULL;		\
		CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
			if(NULL == m_pInstance) {					\
				m_pInstance = new CLASSNAME;			\
			}											\
			return m_pInstance;							\
		}												\
		void CLASSNAME::DestroyInstance( void ) {		\
			if(NULL != m_pInstance)	{					\
				delete m_pInstance;						\
				m_pInstance = NULL;						\
			}											\
		}

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
