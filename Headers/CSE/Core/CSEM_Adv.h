#pragma once

#ifndef _CSE_MATH_ADVANCED
#define _CSE_MATH_ADVANCED

// These functions are used to solve some very difficaut problems.
// But its accuracy maybe very low.

#include <functional>
#include "CSE/Core/CSECore.h"
#include "CSE/Core/gltypes.h"

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_CSE_BEGIN

/****************************************************************************************\
*                                     XYZ -> Polar                                       *
\****************************************************************************************/

/// <summary>
/// Convert XY coordinate to polar coordinate, in format (r, Theta)
/// </summary>
_Check_return_ vec2 _cdecl XYToPolar(_In_ vec2 XY);

/// <summary>
/// Convert XYZ coordinate (Based on X-Z plane) to polar coordinate, in format (Lon, Lat, Dist)
/// </summary>
_Check_return_ vec3 _cdecl XYZToPolar(_In_ vec3 XYZ);

/****************************************************************************************\
*                                     Polar -> XYZ                                       *
\****************************************************************************************/

/// <summary>
/// Convert polar coordinate in format (r, Theta) to XY coordinate
/// </summary>
_Check_return_ vec2 _cdecl PolarToXY(_In_ vec2 Polar);

/// <summary>
/// Convert polar coordinate in format (Lon, Lat, Dist) to XYZ coordinate (Based on X-Z plane)
/// </summary>
_Check_return_ vec3 _cdecl PolarToXYZ(_In_ vec3 Polar);

/****************************************************************************************\
*                                       Functions                                        *
\****************************************************************************************/

// Derivate function generator based on std::function packager.

template <class _Ret = float64, class _Type = float64, uint64 _Pdx = 8>
class _1V_Derivate_Func_class : public _STD _Arg_types<_Type>
{
public:
	using result_type = _Ret;
	using _Ptrt = _STD _Func_base<_Ret, _Type>;

    union _Storage // storage for small objects (basic_string is small)
    {
        max_align_t _Dummy1; // for maximum alignment
        char _Dummy2[_STD _Space_size]; // to permit aliasing
        _Ptrt* _Ptrs[_STD _Small_object_num_ptrs]; // _Ptrs[_Small_object_num_ptrs - 1] is reserved
    }_Mystorage;

    bool _Local() const noexcept // test for locally stored copy of object
    {
        return _Getimpl() == static_cast<const void*>(&_Mystorage);
    }

    _Ptrt* _Getimpl() const noexcept // get pointer to object
    {
        return _Mystorage._Ptrs[_STD _Small_object_num_ptrs - 1];
    }

    void _Set(_Ptrt* _Ptr) noexcept // store pointer to object
    { 
        _Mystorage._Ptrs[_STD _Small_object_num_ptrs - 1] = _Ptr;
    }

    bool _Empty() const noexcept {return !_Getimpl();}

    _1V_Derivate_Func_class() noexcept {_Set(nullptr);}
    ~_1V_Derivate_Func_class() noexcept
    {
        if (!_Empty()) // destroy callable object and maybe delete it
        {
            _Getimpl()->_Delete_this(!_Local());
            _Set(nullptr);
        }
    }

    void _Reset_move(_1V_Derivate_Func_class&& _Right) noexcept
    { // move _Right's stored object
        if (!_Right._Empty())
        {
            if (_Right._Local())
            { // move and tidy
                _Set(_Right._Getimpl()->_Move(&_Mystorage));
                _Right._Tidy();
            }
            else
            { // steal from _Right
                _Set(_Right._Getimpl());
                _Right._Set(nullptr);
            }
        }
    }

    void _Swap(_1V_Derivate_Func_class& _Right) noexcept
    { // swap contents with contents of _Right
        if (!_Local() && !_Right._Local())
        { // just swap pointers
            _Ptrt* _Temp = _Getimpl();
            _Set(_Right._Getimpl());
            _Right._Set(_Temp);
        }
        else
        { // do three-way move
            _1V_Derivate_Func_class _Temp;
            _Temp._Reset_move(_STD move(*this));
            _Reset_move(_STD move(_Right));
            _Right._Reset_move(_STD move(_Temp));
        }
    }

    template <class _Fx>
    void _Reset(_Fx&& _Val)
    { // store copy of _Val
        if (!_STD _Test_callable(_Val))
        { // null member pointer/function pointer/std::function
            return; // already empty
        }

        using _Impl = _STD _Func_impl_no_alloc<_STD decay_t<_Fx>, _Ret, _Type>;
        if constexpr (_STD _Is_large<_Impl>) 
        {
            // dynamically allocate _Val
            _Set(_STD _Global_new<_Impl>(_STD forward<_Fx>(_Val)));
        }
        else
        {
            // store _Val in-situ
            _Set(::new (static_cast<void*>(&_Mystorage)) _Impl(_STD forward<_Fx>(_Val)));
        }
    }

    template <class _Fx, class _Function>
    using _Enable_if_callable_t = _STD enable_if_t<_STD conjunction_v<_STD negation<_STD is_same<_STD _Remove_cvref_t<_Fx>, _Function>>, _STD _Is_invocable_r<_Ret, _STD decay_t<_Fx>&, _Type>>,int>;

    _Ret operator()(_Type _Arg) const
    {
        if (_Empty()){_STD _Xbad_function_call();}
        const auto _Impl = _Getimpl();
        _Type _Dx = pow(10, -(int64)_Pdx);
        _Type _hDx = _Dx / 2.;
        return (_Impl->_Do_call(_STD forward<_Type>(_Arg) + _hDx) - _Impl->_Do_call(_STD forward<_Type>(_Arg) - _hDx)) / _Dx;
    }
};

template <class _Tx, uint64 _Pdx>
struct _Get_1V_original_function_impl
{
    static_assert(_Always_false<_Tx>, "Derivate function generator only accepts function types as template arguments.");
};

#define _GET_ORIGINAL_FUNCTION_IMPL(CALL_OPT, X1, X2, X3)     \
template <class _Ret, class _Types, uint64 _Pdx>              \
struct _Get_1V_original_function_impl<_Ret CALL_OPT(_Types), _Pdx>  \
{ /* determine type from argument list */                     \
    using type = _1V_Derivate_Func_class<_Ret, _Types, _Pdx>; \
};
_NON_MEMBER_CALL(_GET_ORIGINAL_FUNCTION_IMPL, X1, X2, X3)

_EXPORT_STD template <class _Fty = float64(float64), uint64 _Pdx = 8>
class DerivateFunction : public _Get_1V_original_function_impl<_Fty, _Pdx>::type
{
    using _Mybase = typename _Get_1V_original_function_impl<_Fty, _Pdx>::type;

public:
    DerivateFunction() noexcept {}
    DerivateFunction(nullptr_t) noexcept {}
    DerivateFunction(const DerivateFunction& _Right)
    {
        if (!_Right._Empty())
        {
            this->_Set(_Right._Getimpl()->_Copy(&this->_Mystorage));
        }
    }

    void swap(DerivateFunction& _Right) noexcept {this->_Swap(_Right);}

    template <class _Fx, typename _Mybase::template _Enable_if_callable_t<_Fx, DerivateFunction> = 0>
    DerivateFunction(_Fx&& _Func)
    {
        this->_Reset(_STD forward<_Fx>(_Func));
    }

    template <class _Fx, typename _Mybase::template _Enable_if_callable_t<_Fx, DerivateFunction> = 0>
    DerivateFunction& operator=(_Fx&& _Func)
    {
        DerivateFunction(_STD forward<_Fx>(_Func)).swap(*this);
        return *this;
    }
};

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif