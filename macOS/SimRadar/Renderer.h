//
//  Renderer.h
//
//  Created by Boon Leng Cheong on 10/29/13.
//  Copyright (c) 2013 Boon Leng Cheong. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <OpenCL/OpenCL.h>
#import "GLText.h"
#import "GLOverlay.h"

#define RENDERER_NEAR_RANGE                 10.0f       // View range in m
#define RENDERER_FAR_RANGE                  100000.0f   // View range in m
#define RENDERER_TIC_COUNT                  10          //
#define RENDERER_MAX_DEBRIS_TYPES           8           // Actual plus one
#define RENDERER_MAX_CL_DEVICE              1
#define RENDERER_MAX_VBO_GROUPS             8
#define RENDERER_DEFAULT_BODY_COLOR_INDEX   1           // Default colormap index
#define RENDERER_FBO_COUNT                  5

#ifndef CLAMP
#define CLAMP(x, lo, hi)  MIN(MAX((x), (lo)), (hi));
#endif

enum hudConfig {
    hudConfigShowNothing          = 0,
    hudConfigShowAnchors          = 1,
    hudConfigShowGrid             = 1 << 1,
    hudConfigShowRadarView        = 1 << 2,
    hudConfigShowOverlay          = 1 << 3,
    hudConfigShowAll              = 0x0F,
    hudConfigLast                 = 15        // (1 << 4) - 1
};

typedef struct _draw_resource {
    GLchar         vShaderName[64];
    GLchar         fShaderName[64];
	GLuint         program;
	GLuint         vao;
	GLuint         vbo[5];            // positions, colors, tex_coord, wvp_mat, etc.
    
    GLint          mvUI;
	GLint          mvpUI;
    GLint          sizeUI;
	GLint          colorUI;
    GLint          textureUI;
    GLint          colormapUI;
    GLint          pingPongUI;
    GLint          lowZUI;
	
    GLuint         count;
	
    GLfloat        *colors;           // CPU side color
    GLfloat        *positions;        // CPU side position
    GLfloat        *textureCoord;     // CPU side texture coordinate
	GLuint         *indices;          // CPU side indexing for instancing

    GLuint         *segmentOrigins;
    GLuint         *segmentLengths;
    GLuint         segmentNextOrigin;
    GLuint         segmentMax;

    GLint          colorAI;
    GLint          positionAI;
	GLint          rotationAI;
    GLint          quaternionAI;
	GLint          translationAI;
    GLint          textureCoordAI;

    GLKTextureInfo *texture;
    GLuint         textureID;

    GLKTextureInfo *colormap;
    GLuint         colormapCount;
    GLuint         colormapIndex;
    GLfloat        colormapIndexNormalized;
    GLuint         colormapID;

    GLuint         sourceOffset;
    GLuint         instanceSize;
    GLenum         drawMode;

    GLKMatrix4     modelView;
    GLKMatrix4     modelViewProjection;
    GLKMatrix4     modelViewProjectionOffOne;
    GLKMatrix4     modelViewProjectionOffTwo;
} RenderResource;

typedef struct _draw_primitive {
    GLfloat vertices[64];
    GLuint indices[64];
    GLuint vertexSize;
    GLuint instanceSize;
    GLenum drawMode;
} RenderPrimitive;

enum RendererLineSegment {
    RendererLineSegmentBasicRectangle  = 0,
    RendererLineSegmentSimulationGrid  = 1,
    RendererLineSegmentAnchorLines     = 2
};

@protocol RendererDelegate <NSObject>

- (void)glContextVAOPrepared;
- (void)vbosAllocated:(GLuint [][8])vbos;
- (void)willDrawScatterBody;

@end

@interface Renderer : NSObject {
	
    NSString *titleString, *subtitleString, *overlayString;
    
	float GLSLVersion;
	
    GLfloat resetRange;
    GLKMatrix4 resetModelRotate;
    
	GLsizei width, height;
	GLfloat aspectRatio;
	GLfloat rotateX, rotateY, rotateZ, range;

	GLKMatrix4 modelView, camera, projection, modelViewProjection;
	GLKMatrix4 modelRotate;
    GLKMatrix4 hudProjection, hudModelViewProjection;
    GLKMatrix4 beamProjection, beamModelViewProjection;
	
    CGPoint hudOrigin;
    CGSize hudSize;
    
	GLint MVPUniformIndex, singleColorMVPUniformIndex;
	GLint ColorUniformIndex;

    GLfloat scattererScale;
	GLfloat pixelsPerUnit;
	GLfloat unitsPerPixel;
    GLfloat devicePixelRatio;

    GLfloat beamAzimuth, beamElevation, beamNearZ;
    
    GLfloat cameraRoll, cameraPitch, cameraYaw;
    
	id<RendererDelegate> delegate;
    
    unsigned int hudConfigDecimal, hudConfigGray;
    
    int applyVFX;
    BOOL colorbarNeedsUpdate;
    BOOL fadeSmallScatterers;
    BOOL showDebrisAttributes;
	
	@private
	
    cl_uint clDeviceCount;
    
	cl_float4 modelCenter;
    
    cl_float4 domainOrigin;
    cl_float4 domainSize;

    BOOL fboNeedsUpdate;
    BOOL vbosNeedUpdate;
    BOOL viewParametersNeedUpdate;
    BOOL statusMessageNeedsUpdate;
    BOOL overlayNeedsUpdate;
    GLchar spinModel;
    
    RenderResource bodyRenderer[RENDERER_MAX_CL_DEVICE];
    RenderResource instancedGeometryRenderer;

    RenderResource lineRenderer;
    RenderResource anchorRenderer;
    RenderResource debrisRenderer[RENDERER_MAX_DEBRIS_TYPES];
    RenderResource meshRenderer;
    RenderResource frameRenderer;
    RenderResource blurRenderer;

    RenderPrimitive primitives[8];

    GLfloat backgroundOpacity;
    GLfloat phase;

    GLText *textRenderer;
    GLText *tTextRenderer;
    GLText *fwTextRenderer;
    
    GLOverlay *overlayRenderer;

    char statusMessage[16][256];

    char *colorbarTitle;
    char colorbarTickLabels[10][16];
    float *colorbarTickPositions;
    int colorbarTickCount;

    int itic, iframe;
    NSTimeInterval tics[RENDERER_TIC_COUNT];
    float fps;
    char fpsString[16];

    GLuint frameBuffers[5], frameBufferTextures[5];
    GLuint ifbo;
}

@property (nonatomic, copy) NSString *titleString, *subtitleString;
@property (nonatomic) GLfloat resetRange;
@property (nonatomic) GLKMatrix4 resetModelRotate;
@property (nonatomic, retain) id<RendererDelegate> delegate;
@property (nonatomic, readonly) GLsizei width, height;
@property (nonatomic) GLfloat beamAzimuth, beamElevation;
@property (nonatomic) BOOL showDebrisAttributes, fadeSmallScatterers, viewParametersNeedUpdate;
@property (nonatomic) GLfloat backgroundOpacity;
@property (nonatomic) GLfloat cameraRoll, cameraPitch, cameraYaw;

- (id)initWithDevicePixelRatio:(GLfloat)pixelRatio;

- (void)setSize:(CGSize)size;
- (void)setBodyCount:(GLuint)number forDevice:(GLuint)deviceId;
- (void)setPopulationTo:(GLuint)count forDebris:(GLuint)debrisId forDevice:(GLuint)deviceId;
- (void)setGridAtOrigin:(GLfloat *)origin size:(GLfloat *)size;
- (void)setAnchorPoints:(GLfloat *)points number:(GLuint)number;
- (void)setAnchorLines:(GLfloat *)lines number:(GLuint)number;
- (void)setBeamElevation:(GLfloat)elevation azimuth:(GLfloat)azimuth;
- (void)setCameraRoll:(GLfloat)roll pitch:(GLfloat)pitch yaw:(GLfloat)yaw;

- (void)allocateVAO:(GLuint)count;
- (void)updateBodyToDebrisMappings;

- (void)render;

- (void)panX:(GLfloat)x Y:(GLfloat)y dx:(GLfloat)dx dy:(GLfloat)dy;
- (void)magnify:(GLfloat)scale;
- (void)rotate:(GLfloat)angle;
- (void)resetViewParameters;
- (void)topView;

- (void)startSpinModel;
- (void)stopSpinModel;
- (void)toggleSpinModel;
- (void)toggleSpinModelReverse;
- (void)toggleHUDVisibility;
- (void)toggleVFX;
- (void)toggleBlurSmallScatterer;

- (void)increaseBackgroundOpacity;
- (void)decreaseBackgroundOpacity;

- (void)cycleForwardColormap;
- (void)cycleReverseColormap;
- (void)setColormapTitle:(char *)title tickLabels:(NSArray *)labels positions:(GLfloat *)positions;

- (void)cycleFBO;
- (void)cycleFBOReverse;

- (void)cycleForwardHUDConfig;
- (void)cycleReverseHUDConfig;
- (void)showAllHUD;

- (void)cycleVFX;

- (void)setOverlayText:(NSString *)bodyText withTitle:(NSString *)title;

@end
