//
//  rcs.h
//  Radar Simulation Framework
//
//  Created by Boon Leng Cheong on 3/16/15.
//  Copyright (c) 2015 Boon Leng Cheong. All rights reserved.
//

#ifndef _radarsim_rcs_h
#define _radarsim_rcs_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define RCSConfigSquarePlate       "square_plate"
#define RCSConfigRoofTile          "roof_tile"

typedef void* RCSHandle;
typedef char* RCSConfig;

typedef struct rcs_grid {
    uint32_t  rev;            // Revision number, perhaps?
    uint32_t  na;             // Number of cells in alpha direction
    uint32_t  nb;             // Number of cells in beta direction
    float     *a;             // Alpha values
    float     *b;             // Beta values
} RCSGrid;

typedef struct _rcs_data {
    float *a;
    float *b;
    float *hh;
    float *vv;
    float *hv;
} RCSData;

typedef struct _rcs_table {
    uint32_t  nt;             // Number of debris types
    uint32_t  na;             // Number of cells in alpha direction
    uint32_t  nb;             // Number of cells in beta direction
    uint32_t  nn;             // Number of cells in all directions combined
    float     *a;
    float     *b;
    float     *hh;
    float     *vv;
    float     *hv;
    char      **name;
} RCSTable;

RCSHandle *RCS_init_with_config_path(const RCSConfig config, const char *path);
RCSHandle *RCS_init(void);
void RCS_free(RCSHandle *);

RCSTable *RCS_get_frame(const RCSHandle *);

void RCS_show_table_summary(const RCSTable *);

#endif