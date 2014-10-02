//
//  SimPoint.h
//  SimRadar
//
//  Created by Boon Leng Cheong on 11/11/13.
//  Copyright (c) 2013 Boon Leng Cheong. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "rs.h"

@protocol SimPointDelegate <NSObject>

- (void)timeAdvanced:(id)sender;

@end


@interface SimPoint : NSObject {
	
	BOOL busy;
	id<SimPointDelegate> delegate;

	float az_deg;
	
@private
	
	RSHandle *S;

	LESHandle *L;
	
	int table_id;
}

@property (nonatomic, readonly) BOOL busy;
@property (nonatomic, assign) id<SimPointDelegate> delegate;
@property (nonatomic, readonly) NSInteger numberOfPoints;
@property (nonatomic, readonly, getter=azimuthInDegrees) float az_deg;

- (void)shareVBOsWithGL:(GLuint *)vbos;
- (void)upload;
- (void)populate;
- (void)explode;
- (void)advanceTime;
- (void)advanceBeamPosition;
- (void)advanceTimeAndBeamPosition;
- (void)randomBeamPosition;
- (void)run;

- (cl_float4 *)points;
- (NSInteger)pointCount;

- (cl_float4 *)anchors;
- (NSInteger)anchorCount;

- (cl_float4 *)anchorLines;
- (NSInteger)anchorLineCount;

- (RSVolume)simulationDomain;


@end