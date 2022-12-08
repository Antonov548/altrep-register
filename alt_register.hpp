#pragma once

#include <R.h>
#include <R_ext/Altrep.h>

#include <type_traits>

#define HAS_METHOD(name, reg) template <typename T> struct has_ ## name { template <class, class> class checker;\
    template <typename C> static std::true_type check(checker<C, decltype(&C::name)> *);\
    template <typename C> static std::false_type check(...);\
    typedef decltype(check<T>(nullptr)) type; static const bool value = std::is_same<std::true_type, decltype(check<T>(nullptr))>::value; };\
    template<typename Type>\
    void name ## _method(typename std::enable_if<has_ ## name<Type>::value, Type>::type* = 0) { reg ;}\
    template<typename Type>\
    void name ## _method(typename std::enable_if<!has_ ## name<Type>::value, Type>::type* = 0){}

HAS_METHOD(Length, R_set_altrep_Length_method(Type::class_t, Type::Length))
HAS_METHOD(Inspect, R_set_altrep_Inspect_method(Type::class_t, Type::Inspect))

template<typename Type>
void alt_register(DllInfo* dll, const char* class_name, const char* package_name) {
    Type::class_t = R_make_altreal_class(class_name, package_name, dll);

    Length_method<Type>();
    Inspect_method<Type>();

    // altvec
    R_set_altvec_Dataptr_method(Type::class_t, Type::Dataptr);
    R_set_altvec_Dataptr_or_null_method(Type::class_t, Type::Dataptr_or_null);

    // altreal
    R_set_altreal_Elt_method(Type::class_t, Type::real_Elt);
    R_set_altreal_Get_region_method(Type::class_t, Type::Get_region);
}