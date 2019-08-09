/**
 *******************************************************************************
 * @file       region.c
 * @version    V0.0.1   
 * @date       2019.5.27
 * @brief      This is a file for GUI region.	
 *******************************************************************************
 */ 

#include "../include/cogui.h"

static GuiRegionData guiRegionEmptydata = {0, 0};
static GuiRegionData guiBrokendata = {0, 0};

GuiRect guiEmptyRect = {0, 0, 0, 0};
GuiPoint guiEmptyPoint = {0, 0};

static StatusType RegionBreak(cogui_region_t *region)
{
    if(region->data && region->data->size){
        GuiFree(region->data);
    }

    region->extent = guiEmptyRect;
    region->data = &guiBrokendata;
    return GUI_REGION_STATUS_FAILURE;
}

StatusType RegionCopy(cogui_region_t *dst, cogui_region_t *src)
{
    if(dst==Co_NULL || src==Co_NULL){
        return GUI_REGION_STATUS_FAILURE;
    }

    if(dst==src){
        return GUI_REGION_STATUS_SUCCESS;
    }    
    
    dst->extent = src->extent;

    if(!src->data || !src->data->size){
        if(dst->data && dst->data->size){
            GuiFree(dst->data);
        }
    }
    if(!dst->data || (dst->data->size < src->data->numRects)){
        if(dst->data && dst->data->size){
            GuiFree(dst->data);
        }

        dst->data = GuiMalloc(sizeof(GuiRegionData)+(src->data->numRect)*sizeof(GuiRect));

        if(!dst->data){
            dst->data->extent = guiEmptyRect;
            dst->data         = guiBrokendata;

            return GUI_REGION_STATUS_FAILURE;
        }
        dst->data->size = src->data->numRects;
    }

    dst->data->numRects = src->data->numRects;
    MemMove((char *)((P_GuiRect)dst->data+1), (char *)((P_GuiRect)src->data+1), dst->data->numRects * sizeof(GuiRect));
    
    return GUI_REGION_STATUS_SUCCESS;
}

void RegionInitWithExtents(cogui_region_t *region, const cogui_rect_t *extents)
{
    region->extent = *extents;
    region->data = Co_NULL;
}

void RegionReset(cogui_region_t *region, cogui_rect_t *rect)
{
    if(region==Co_NULL){
        return;
    }

    if(region->data && region->data->size){
        GuiFree(region->data);
    }

    RegionInitWithExtents(region, rect);
}

void RectIntersect(cogui_rect_t *src, cogui_rect_t *dest)
{
    if (dest->x1 < src->x1) dest->x1 = src->x1;
    if (dest->y1 < src->y1) dest->y1 = src->y1;
    if (dest->x2 > src->x2) dest->x2 = src->x2;
    if (dest->y2 > src->y2) dest->y2 = src->y2;
}

StatusType RegionIntersectRect(cogui_region_t *newReg,cogui_region_t *reg1, cogui_rect_t *rect)
{
    cogui_region_t *region;

    region.data = RT_NULL;
    region.extent.x1 = rect->x1;
    region.extent.y1 = rect->y1;
    region.extent.x2 = rect->x2;
    region.extent.y2 = rect->y2;

    return RectIntersect(newReg, reg1, &region);
}

StatusType RegionUnion(cogui_region_t *newReg, cogui_region_t *reg1, cogui_region_t *reg2)
{
    if(newReg==Co_NULL || reg1==Co_NULL || reg2==Co_NULL){
        return GUI_REGION_STATUS_FAILURE;
    }

    /* Region 1 and 2 are the same */
    if (reg1 == reg2)
    {
        return RegionCopy(newReg, reg1);
    }

    /* Region 1 is empty */
    if (reg1->data && !(reg1->data->numRects))
    {
        if (reg1==&guiBrokendata){
            return RegionBreak(newReg)
        }
        if (newReg != reg2)
            return RegionCopy(newReg, reg2);
        return GUI_REGION_STATUS_SUCCESS;
    }

    /*  Region 2 is empty */
    if (reg2->data && !(reg2->data->numRects))
    {
        if (reg2==&guiBrokendata){
            return RegionBreak(newReg);
        if (newReg != reg1)
            return RegionCopy(newReg, reg1);
        return GUI_REGION_STATUS_SUCCESS;
    }

    /* Region 1 completely subsumes region 2 */
    if (!reg1->data && SUBSUMES(&reg1->extent, &reg2->extent))
    {
        if (newReg != reg1)
            return RegionCopy(newReg, reg1);
        return GUI_REGION_STATUS_SUCCESS;
    }

    /*  Region 2 completely subsumes region 1 */
    if (!reg2->data && SUBSUMES(&reg2->extent, &reg1->extent))
    {
        if (newReg != reg2)
            return RegionCopy(newReg, reg2);
        return GUI_REGION_STATUS_SUCCESS;
    }

    // ops

    newReg->extent.x1 = GUI_MIN(reg1->extent.x1, reg2->extent.x1);
    newReg->extent.y1 = GUI_MIN(reg1->extent.y1, reg2->extent.y1);
    newReg->extent.x2 = GUI_MAX(reg1->extent.x2, reg2->extent.x2);
    newReg->extent.y2 = GUI_MAX(reg1->extent.y2, reg2->extent.y2);

    if(newReg==Co_NULL){
        return GUI_REGION_STATUS_FAILURE;
    }

    return GUI_REGION_STATUS_SUCCESS;
}

StatusType RegionSubtract(cogui_region_t *regD, cogui_region_t *regM, cogui_region_t *regS)
{

}

StatusType RegionSubtractRect(cogui_region_t *regD, cogui_region_t *regM, cogui_rect_t *rect)
{

}

void RectMove(cogui_rect_t *rect, S32 x, S32 y)
{
    rect->x1 += x;
    rect->x2 += x;

    rect->y1 += y;
    rect->y2 += y;
}


void RectInflate(cogui_rect_t *rect, S32 d)
{
    rect->x1 -= d;
    rect->x2 += d;
    rect->y1 -= d;
    rect->y2 += d;
}