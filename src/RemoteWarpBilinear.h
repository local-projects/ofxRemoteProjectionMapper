//
//  RemoteWarpBilinear.hpp
//  RemoteProjectionMapper
//
//  Created by Michael Allison on 5/1/18.
//

#pragma once

#include "RemoteWarpBase.h"

class RemoteWarpBilinear : public RemoteWarpBase {
public:
    RemoteWarpBilinear(const std::string& name, const WarpSettings& settings);
    virtual ~RemoteWarpBilinear();
    
    virtual void serialize(nlohmann::json & json) override;
    virtual void deserialize(const nlohmann::json & json) override;
        
    //! set whether the mesh is linear (or curved)
    void setLinear(bool linear);
    //! return whether the mesh is linear (or curved)
    bool getLinear() const;
    
    //! set whether the mesh resolution is adaptive to the window size
    void setAdaptive(bool adaptive);
    //! return whether the mesh resolution is adaptive to the window size
    bool getAdaptive() const;
    
    //! increase the mesh resolution
    void increaseResolution();
    //! decrease the mesh resolution
    void decreaseResolution();
    //! return the mesh resolution
    int getResolution() const;
    
    //! reset control points to undistorted image
    virtual void reset(const glm::vec2 & scale = glm::vec2(1.0f), const glm::vec2 & offset = glm::vec2(0.0f)) override;
    
    //! set the number of horizontal control points for this warp
    void setNumControlsX(int n);
    //! set the number of vertical control points for this warp
    void setNumControlsY(int n);
    
    void setCorners(float left, float top, float right, float bottom);
    
    virtual void rotateClockwise() override;
    virtual void rotateCounterclockwise() override;
    
    virtual void flipHorizontal() override;
    virtual void flipVertical() override;
    
protected:
    //! draw a specific area of a warped texture to a specific region
    virtual void drawTexture(const ofTexture & texture, const ofRectangle & srcBounds, const ofRectangle & dstBounds) override;
    //! draw the warp's controls interface
    virtual void drawControls() override;
    
    //! set up the frame buffer
    void setupFbo();
    //! set up the shader and vertex buffer
    void setupVbo();
    //! set up the vbo mesh
    void setupMesh(int resolutionX = 36, int resolutionY = 36);
    //! update the vbo mesh based on the control points
    void updateMesh();
    //!    return the specified control point, values for col and row are clamped to prevent errors.
    glm::vec2 getPoint(int col, int row) const;
    //! perform fast Catmull-Rom interpolation, and return the interpolated value at t
    glm::vec2 cubicInterpolate(const std::vector<glm::vec2> & knots, float t) const;
    //!
    ofRectangle getMeshBounds() const;
    
protected:

    ofVbo vbo;
    ofShader shader;
    
    //! linear or curved interpolation
    bool linear;
    
    bool adaptive;
    
    //! texture coordinates of corners
    glm::vec4 corners;
    
    //! detail of the generated mesh (multiples of 5 seem to work best)
    int resolution;
    
    //! number of horizontal quads
    int resolutionX;
    //! number of vertical quads
    int resolutionY;
    
    //remoteUI
    int remoteNumControlsX{2};
    int remoteNumControlsY{2};
    
    bool remoteIncRes{false};
    bool remoteDecRes{false};
    
    bool remoteFlipH{false};
    bool remoteFlipV{false};

protected:
        
    virtual void handleRemoteUpdate(RemoteUIServerCallBackArg & arg)override;

    //! greatest common divisor using Euclidian algorithm (from: http://en.wikipedia.org/wiki/Greatest_common_divisor)
    inline int gcd(int a, int b) const
    {
        if (b == 0)
        {
            return a;
        }
        else
        {
            return gcd(b, a % b);
        }
    };
};

