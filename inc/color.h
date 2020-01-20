/**
 *******************************************************************************
 * @file       color.h
 * @version    V0.0.3
 * @date       2020.01.20
 * @brief      Color define header file.
 *******************************************************************************
 */ 

#ifndef __COGUI_COLOR_H__
#define __COGUI_COLOR_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef co_int64_t cogui_color_t;   /**< Color type belong to long integer */

/** Determine which color style used */
//#define COGUI_USING_RGB565
//#define COGUI_USING_BGR565
#define COGUI_USING_RGB888
//#define COGUI_USING_ARGB8888
//#define COGUI_USING_RGBA8888
//#define COGUI_USING_ABGR8888

/* RGB into integer by different style */
#define COGUI_RGB565(r,g,b)     ((cogui_color_t)((((r)>>3)<<11)|(((g)>>2)<<5)|((b)>>3)))
#define COGUI_BGR565(b,g,r)     ((cogui_color_t)((((b)>>3)<<11)|(((g)>>2)<<5)|((r)>>3)))
#define COGUI_RGB888(r,g,b)     ((cogui_color_t)(((r)<<16)|((g)<<8)|(b)))
#define COGUI_ARGB8888(a,r,g,b) ((cogui_color_t)(((a)<<24)|((r)<<16)|((g)<<8)|(b)))
#define COGUI_RGBA8888(r,g,b,a) ((cogui_color_t)(((r)<<24)|((g)<<16)|((b)<<8)|(a)))
#define COGUI_ABGR8888(a,b,g,r) ((cogui_color_t)(((a)<<24)|((b)<<16)|((g)<<8)|(r)))

#ifdef COGUI_USING_RGB565
#define COGUI_RGB(r,g,b) COGUI_RGB565((r),(g),(b))
#endif

#ifdef COGUI_USING_BGR565
#define COGUI_RGB(r,g,b) COGUI_BGR565((b),(g),(r))
#endif

#ifdef COGUI_USING_RGB888
#define COGUI_RGB(r,g,b) COGUI_RGB565((r),(g),(b))
#endif

#ifdef COGUI_USING_ARGB8888
#define COGUI_RGB(r,g,b) COGUI_ARGB8888(255,(r),(g),(b))
#endif

#ifdef COGUI_USING_RGBA8888
#define COGUI_RGB(r,g,b) COGUI_RGB565((r),(g),(b), 255)
#endif

#ifdef COGUI_USING_ABGR8888
#define COGUI_RGB(r,g,b) COGUI_ABGR8888(255,(b),(g),(r))
#endif

/* some default color scheme */
#define COGUI_RED               COGUI_RGB(0xff, 0x00, 0x00)         /**< Default red color          */ 
#define COGUI_GREEN             COGUI_RGB(0x00, 0xff, 0x00)         /**< Default green color        */ 
#define COGUI_BLUE              COGUI_RGB(0x00, 0x00, 0xff)         /**< Default blue color         */ 
#define COGUI_YELLOW            COGUI_RGB(0xff, 0xff, 0x00)         /**< Default yellow color       */ 
#define COGUI_PURPLE            COGUI_RGB(0xff, 0x00, 0xff)         /**< Default purple color       */
#define COGUI_CYAN              COGUI_RGB(0x00, 0xff, 0xff)         /**< Default cyan color         */ 
#define COGUI_HIGH_LIGHT        COGUI_RGB(0xf5, 0xf5, 0xf5)         /**< Default high light color   */ 
#define COGUI_LIGHT_GRAY        COGUI_RGB(0x9e, 0x9e, 0x9e)         /**< Default light gray color   */ 
#define COGUI_DARK_GRAY         COGUI_RGB(0x42, 0x42, 0x42)         /**< Default dark gray color    */ 
#define COGUI_WHITE             COGUI_RGB(0xff, 0xff, 0xff)         /**< Default white color        */ 
#define COGUI_BLACK             COGUI_RGB(0x00, 0x00, 0x00)         /**< Default black color        */ 

#ifdef __cplusplus
}
#endif

#endif /* __COGUI_COLOR_H__ */
