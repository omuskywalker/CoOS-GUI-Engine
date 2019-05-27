/**
 *******************************************************************************
 * @file       GuiRegion.h
 * @version    V0.0.1   
 * @date       2019.5.27
 * @brief      This is a file for GUI region.	
 *******************************************************************************
 */ 

#ifndef _GUI_REGION_H
#define _GUI_REGION_H

/*---------------------------- structure -------------------------------------*/
typedef struct regionData
{
    U32 size;
    U32 nubRects;

}GuiRegionData,*P_GuiRegionData;

typedef struct region
{
    GuiRect extent;

    P_GuiRegionData data;

}GuiRegion,*P_GuiRegion;

/* true iff Box r1 contains Box r2 */
#define SUBSUMES(r1,r2) \
      ( ((r1)->x1 <= (r2)->x1) && \
        ((r1)->x2 >= (r2)->x2) && \
        ((r1)->y1 <= (r2)->y1) && \
        ((r1)->y2 >= (r2)->y2) )

#define GUI_MIN(a,b) ((a) < (b) ? (a) : (b))
#define GUI_MAX(a,b) ((a) > (b) ? (a) : (b))

/*---------------------------- Function Define -------------------------------*/
static StatusType RegionBreak(P_GuiRegion *region);
StatusType RegionCopy(P_GuiRegion dest, P_GuiRegion source);

void RegionInitWithExtents(P_GuiRegion region, const P_GuiRect extents);

void RegionReset(rtgui_region_t *region, rtgui_rect_t *rect);

void RectIntersect(P_GuiRect src, P_GuiRect dest);
StatusType RegionIntersectRect(P_GuiRegion *newReg,P_GuiRegion *reg1, P_GuiRect *rect);

StatusType RegionUnion(P_GuiRegion newReg, P_GuiRegion reg1, P_GuiRegion reg2);

StatusType RegionSubtract(P_GuiRegion regD, P_GuiRegion regM, P_GuiRegion regS);
StatusType RegionSubtractRect(P_GuiRegion regD, P_GuiRegion regM, P_GuiRect rect);

void RectInflate(P_GuiRect rect, S32 d);

#endif /* _GUI_REGION_H */