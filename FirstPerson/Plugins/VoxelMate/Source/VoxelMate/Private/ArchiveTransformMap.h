#pragma once
#include "VoxelDatabaseTypeFactory.h"
#include "openvdb/openvdb.h"
#include <openvdb/math/Maps.h>

template<typename TransformMapType>
struct TTransformMapTypeTranslator;

struct FTransformMapFactory : public FVoxelDatabaseTypeFactory<openvdb::math::MapBase>
{
    template<typename MapType>
    static void Register()
    {
        try
        {
            TTransformMapTypeTranslator<MapType>::Type::registerMap();
            FTransformMapFactory::RegisteredTypeNames.Add(FName::NameToDisplayString(UTF8_TO_TCHAR(openvdb::typeNameAsString<MapType>()), false));
        }
        catch (const openvdb::KeyError& e)
        {
            (void)e; //TODO log error (map type already registered)
        }
    }

    template<typename MapType>
    static bool IsRegistered()
    {
        return TTransformMapTypeTranslator<MapType>::Type::isRegistered();
    }

    template<typename MapType>
    static ValueTypePtr Create()
    {
        ValueTypePtr TransformMapPtr = nullptr;
        try
        {
            TransformMapPtr = TTransformMapTypeTranslator<MapType>::Type::create();
        }
        catch (const openvdb::LookupError& e)
        {
            (void)e; //TODO log error (transform map type not registered)
        }
        check(TransformMapPtr != nullptr);
        return  TransformMapPtr;
    }
};

template<> struct TTransformMapTypeTranslator<FAffineCoordinateTransform>
{
    typedef openvdb::math::AffineMap Type;
};

template<> struct TTransformMapTypeTranslator<FUnitaryCoordinateTransform>
{
    typedef openvdb::math::UnitaryMap Type;
};

template<> struct TTransformMapTypeTranslator<FScaleCoordinateTransform>
{
    typedef openvdb::math::ScaleMap Type;
};

template<> struct TTransformMapTypeTranslator<FUniformScaleCoordinateTransform>
{
    typedef openvdb::math::UniformScaleMap Type;
};

template<> struct TTransformMapTypeTranslator<FTranslationCoordinateTransform>
{
    typedef openvdb::math::TranslationMap Type;
};

template<> struct TTransformMapTypeTranslator<FScaleTranslationCoordinateTransform>
{
    typedef openvdb::math::ScaleTranslateMap Type;
};

template<> struct TTransformMapTypeTranslator<FUniformScaleTranslationCoordinateTransform>
{
    typedef openvdb::math::UniformScaleTranslateMap Type;
};

template<> struct TTransformMapTypeTranslator<FNonlinearFrustumCoordinateTransform>
{
    typedef openvdb::math::NonlinearFrustumMap Type;
};

template<> struct TTransformMapTypeTranslator<openvdb::math::AffineMap>
{
    typedef FAffineCoordinateTransform Type;
};

template<> struct TTransformMapTypeTranslator<openvdb::math::UnitaryMap>
{
    typedef FUnitaryCoordinateTransform Type;
};

template<> struct TTransformMapTypeTranslator<openvdb::math::ScaleMap>
{
    typedef FScaleCoordinateTransform Type;
};

template<> struct TTransformMapTypeTranslator<openvdb::math::UniformScaleMap>
{
    typedef FUniformScaleCoordinateTransform Type;
};

template<> struct TTransformMapTypeTranslator<openvdb::math::TranslationMap>
{
    typedef FTranslationCoordinateTransform Type;
};

template<> struct TTransformMapTypeTranslator<openvdb::math::ScaleTranslateMap>
{
    typedef FScaleTranslationCoordinateTransform Type;
};

template<> struct TTransformMapTypeTranslator<openvdb::math::UniformScaleTranslateMap>
{
    typedef FUniformScaleTranslationCoordinateTransform Type;
};

template<> struct TTransformMapTypeTranslator<openvdb::math::NonlinearFrustumMap>
{
    typedef FNonlinearFrustumCoordinateTransform Type;
};
