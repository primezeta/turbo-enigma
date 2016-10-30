#pragma once
#include "RenderResource.h"

//TODO See RenderResource.h for calls such as BeginInitResource

class FGridRenderResource : public FRenderResource
{
public:

    /** Default constructor. */
    FGridRenderResource() {}

    /**
    * Initializes the dynamic RHI resource and/or RHI render target used by this resource.
    * Called when the resource is initialized, or when reseting all RHI resources.
    * Resources that need to initialize after a D3D device reset must implement this function.
    * This is only called by the rendering thread.
    */
    virtual void InitDynamicRHI() override
    {
        FRenderResource::InitDynamicRHI();
    }

    /**
    * Releases the dynamic RHI resource and/or RHI render target resources used by this resource.
    * Called when the resource is released, or when reseting all RHI resources.
    * Resources that need to release before a D3D device reset must implement this function.
    * This is only called by the rendering thread.
    */
    virtual void ReleaseDynamicRHI() override
    {
        FRenderResource::ReleaseDynamicRHI();
    }

    /**
    * Initializes the RHI resources used by this resource.
    * Called when entering the state where both the resource and the RHI have been initialized.
    * This is only called by the rendering thread.
    */
    virtual void InitRHI() override
    {
        FRenderResource::InitRHI();
    }

    /**
    * Releases the RHI resources used by this resource.
    * Called when leaving the state where both the resource and the RHI have been initialized.
    * This is only called by the rendering thread.
    */
    virtual void ReleaseRHI() override
    {
        FRenderResource::ReleaseRHI();
    }

    /**
    * Initializes the resource.
    * This is only called by the rendering thread.
    */
    virtual void InitResource() override
    {
        FRenderResource::InitResource();
    }

    /**
    * Prepares the resource for deletion.
    * This is only called by the rendering thread.
    */
    virtual void ReleaseResource() override
    {
        FRenderResource::ReleaseResource();
    }
};
