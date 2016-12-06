#pragma once
#include "EngineMinimal.h"

template<typename ValueT>
class TVoxelValueSource
{
public:
    typedef ValueT ValueType;
};

class IVoxelBoolSource : public TVoxelValueSource<bool>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelUint8Source : public TVoxelValueSource<uint8>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelUint16Source : public TVoxelValueSource<uint16>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelUint32Source : public TVoxelValueSource<uint32>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelUint64Source : public TVoxelValueSource<uint64>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelInt8Source : public TVoxelValueSource<int8>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelInt16Source : public TVoxelValueSource<int16>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelInt32Source : public TVoxelValueSource<int32>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelInt64Source : public TVoxelValueSource<int64>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelColorSource : public TVoxelValueSource<FColor>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelFloatSource : public TVoxelValueSource<float>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelDoubleSource : public TVoxelValueSource<double>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelPackedNormalSource : public TVoxelValueSource<FPackedNormal>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelPackedRGB10A2NSource : public TVoxelValueSource<FPackedRGB10A2N>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelPackedRGBA16NSource : public TVoxelValueSource<FPackedRGBA16N>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelIntPointSource : public TVoxelValueSource<FIntPoint>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelIntVectorSource : public TVoxelValueSource<FIntVector>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelVectorSource : public TVoxelValueSource<FVector>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelVector4Source : public TVoxelValueSource<FVector4>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelVector2DSource : public TVoxelValueSource<FVector2D>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

class IVoxelLinearColorSource : public TVoxelValueSource<FLinearColor>
{
public:
    virtual ValueType GetValue(float x, float y, float z) = 0;
};

template<typename ValueT>
struct TVoxelValueSourceAdapter;

template<>
struct TVoxelValueSourceAdapter<bool>
{
    typedef IVoxelBoolSource Type;
};

template<>
struct TVoxelValueSourceAdapter<uint8>
{
    typedef IVoxelUint8Source Type;
};

template<>
struct TVoxelValueSourceAdapter<uint16>
{
    typedef IVoxelUint16Source Type;
};

template<>
struct TVoxelValueSourceAdapter<uint32>
{
    typedef IVoxelUint32Source Type;
};

template<>
struct TVoxelValueSourceAdapter<uint64>
{
    typedef IVoxelUint64Source Type;
};

template<>
struct TVoxelValueSourceAdapter<int8>
{
    typedef IVoxelInt8Source Type;
};

template<>
struct TVoxelValueSourceAdapter<int16>
{
    typedef IVoxelInt16Source Type;
};

template<>
struct TVoxelValueSourceAdapter<int32>
{
    typedef IVoxelInt32Source Type;
};

template<>
struct TVoxelValueSourceAdapter<int64>
{
    typedef IVoxelInt64Source Type;
};

template<>
struct TVoxelValueSourceAdapter<FColor>
{
    typedef IVoxelColorSource Type;
};

template<>
struct TVoxelValueSourceAdapter<float>
{
    typedef IVoxelFloatSource Type;
};

template<>
struct TVoxelValueSourceAdapter<double>
{
    typedef IVoxelDoubleSource Type;
};

template<>
struct TVoxelValueSourceAdapter<FPackedNormal>
{
    typedef IVoxelPackedNormalSource Type;
};

template<>
struct TVoxelValueSourceAdapter<FPackedRGB10A2N>
{
    typedef IVoxelPackedRGB10A2NSource Type;
};

template<>
struct TVoxelValueSourceAdapter<FPackedRGBA16N>
{
    typedef IVoxelPackedRGBA16NSource Type;
};

template<>
struct TVoxelValueSourceAdapter<FIntPoint>
{
    typedef IVoxelIntPointSource Type;
};

template<>
struct TVoxelValueSourceAdapter<FIntVector>
{
    typedef IVoxelIntVectorSource Type;
};

template<>
struct TVoxelValueSourceAdapter<FVector>
{
    typedef IVoxelVectorSource Type;
};

template<>
struct TVoxelValueSourceAdapter<FVector4>
{
    typedef IVoxelVector4Source Type;
};

template<>
struct TVoxelValueSourceAdapter<FVector2D>
{
    typedef IVoxelVector2DSource Type;
};

template<>
struct TVoxelValueSourceAdapter<FLinearColor>
{
    typedef IVoxelLinearColorSource Type;
};
