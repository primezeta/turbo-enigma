#pragma once
#include "VoxelDatabaseCommon.h"
#include "GridTransformMaps.h"

#pragma warning(push)
#pragma warning(once:4211 4800 4503 4146)
#include <openvdb/openvdb.h>
#include <ostream>

typedef std::ostream OutputStreamType;

template<typename TransformMapType>
struct TTransformMapTypeTranslator;

template<> struct TTransformMapTypeTranslator<FAffineMap>
{
    typedef openvdb::math::AffineMap Type;
};

template<> struct TTransformMapTypeTranslator<FUnitaryMap>
{
    typedef openvdb::math::UnitaryMap Type;
};

template<> struct TTransformMapTypeTranslator<FScaleMap>
{
    typedef openvdb::math::ScaleMap Type;
};

template<> struct TTransformMapTypeTranslator<FUniformScaleMap>
{
    typedef openvdb::math::UniformScaleMap Type;
};

template<> struct TTransformMapTypeTranslator<FTranslationMap>
{
    typedef openvdb::math::TranslationMap Type;
};

template<> struct TTransformMapTypeTranslator<FScaleTranslationMap>
{
    typedef openvdb::math::ScaleTranslateMap Type;
};

template<> struct TTransformMapTypeTranslator<FUniformScaleTranslationMap>
{
    typedef openvdb::math::UniformScaleTranslateMap Type;
};

template<> struct TTransformMapTypeTranslator<FNonlinearFrustumMap>
{
    typedef openvdb::math::NonlinearFrustumMap Type;
};

template<> struct TTransformMapTypeTranslator<openvdb::math::AffineMap>
{
    typedef FAffineMap Type;
};

template<> struct TTransformMapTypeTranslator<openvdb::math::UnitaryMap>
{
    typedef FUnitaryMap Type;
};

template<> struct TTransformMapTypeTranslator<openvdb::math::ScaleMap>
{
    typedef FScaleMap Type;
};

template<> struct TTransformMapTypeTranslator<openvdb::math::UniformScaleMap>
{
    typedef FUniformScaleMap Type;
};

template<> struct TTransformMapTypeTranslator<openvdb::math::TranslationMap>
{
    typedef FTranslationMap Type;
};

template<> struct TTransformMapTypeTranslator<openvdb::math::ScaleTranslateMap>
{
    typedef FScaleTranslationMap Type;
};

template<> struct TTransformMapTypeTranslator<openvdb::math::UniformScaleTranslateMap>
{
    typedef FUniformScaleTranslationMap Type;
};

template<> struct TTransformMapTypeTranslator<openvdb::math::NonlinearFrustumMap>
{
    typedef FNonlinearFrustumMap Type;
};
