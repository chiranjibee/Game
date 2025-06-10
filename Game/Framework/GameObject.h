#ifndef GE_GAMEOBJECT_H
#define GE_GAMEOBJECT_H

#include <string>
#include <typeinfo>
#include <cassert>

#define DECLARE_CLASS(Type, ParentType)                                                                                                 \
    typedef Type ThisClass;                                                                                                             \
    typedef ParentType Super;                                                                                                           \
public:                                                                                                                                 \
    static std::string GetTypeName() { return std::string(#Type); }                                                                     \
    static const std::type_info& GetTypeInfo() { return typeid(Type); }                                                                 \
    static const bool IsA(const std::type_info& InInfo) { return (ThisClass::GetTypeInfo() == InInfo) ? true : Super::IsA(InInfo); }    \

//////////////////////////////////////////////////////////////////////////
// Casts - TODO: @CS, move to different file?

/*template<typename To, typename From>
To* Cast(From* Src);

template<typename To, typename From>
To* CastChecked(From* Src);

/*template<typename From, typename To>
struct TCastImpl
{
	__forceinline static To* DoCast(CBase* Src)
	{
		return Src && Src->IsA(To::GetTypeInfo()) ? (To*)Src : nullptr;
	}
};#1#

template<typename To, typename From>
__forceinline To* Cast(From* Src)
{
	//return TCastImpl<From, To>::DoCast(Src);
	//return Src && Src->IsA(To::GetTypeInfo()) ? (To*)Src : nullptr;
	return Src && Src->IsA(To::GetTypeInfo()) ? static_cast<To*>(Src) : nullptr;
}

template<typename To, typename From>
__forceinline To* CastChecked(From* Src)
{
	assert(Src != nullptr);
	To* Result = Cast<To>(Src);
	assert(!Result);
	return Result;
}*/

class CGameObject
{
private:
    typedef CGameObject ThisClass;
    
public:
    CGameObject();
    virtual ~CGameObject();

    // Static functions
    static std::string GetTypeName() { return std::string("CGameObject"); }
    static const std::type_info& GetTypeInfo() { return typeid(CGameObject); }
    static const bool IsA(const std::type_info& InInfo) { return (ThisClass::GetTypeInfo() == InInfo) ? true : false; }

    // Game loop functions
    virtual void Init() = 0;
    virtual void Update(float DeltaTime) = 0;
    virtual void Render() = 0;
    virtual void Shutdown() = 0;
};

#endif