#pragma once
#include "VoxelDatabaseCommon.h"
#include "GridCoordinateTransforms.h"

#pragma warning(push)
#pragma warning(once:4211 4800 4503 4146)
#include <openvdb/openvdb.h>
#include <ostream>

typedef std::ostream OutputStreamType;

template<typename TransformMapType>
struct TTransformMapTypeTranslator;

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
