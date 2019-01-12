#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define NUM_COLORS 8

enum FORMAT{
    ARGB32,
    RGB24,
    RGB565,
    YUV444P,
    YUV444,
    YUV422P,
    YUV422
};

struct color_bar_map
{
    char format[10];
    uint8_t Bpp;
    uint64_t colors[NUM_COLORS];
};


struct color_bar_map color_list[] = {
    {   
        /*ARGB8888 BPP=4 */
        .format = "argb32",
        .Bpp = 4,
        .colors = {  
            0xFFFFFFFF, /* white */
            0xFFFF0000, /* red */
            0xFF00FF00, /* green */
            0xFF0000FF, /* blue */
            0xFFFFFF00, /* yellow */
            0xFF00FFFF, /* cyan */
            0xFFFF00FF, /* magenta */
            0xFF000000, /* black */
        },
    },
    {   
        /*RGB888 BPP =3 */
        .format = "rgb24",
        .Bpp = 3,
        .colors = {  
            0xFFFFFF, /* white */
            0xFF0000, /* red */
            0x00FF00, /* green */
            0x0000FF, /* blue */
            0xFFFF00, /* yellow */
            0x00FFFF, /* cyan */
            0xFF00FF, /* magenta */
            0x000000, /* black */
        },
    },
    {   
        /*RGB565 BPP =2 */
        .format = "rgb565",
        .Bpp = 2,
        .colors = {  
            0xffff, /* white */
            0xF800, /* red */
            0x07e0, /* green */
            0x001F, /* blue */
            0xFFE0, /* yellow */
            0x07ff, /* cyan */
            0xF81F, /* magenta */
            0x0000, /* black */
        },
    },
    {   
        /* YUV444P,BPP=3  packed no wastage*/
        .format = "yuv444p",
        .Bpp = 3,
        .colors = {  
            0x807fff, /* white */
            0xff544c, /* red */
            0x152b96, /* green */
            0x6bff1d, /* blue */
            0x9400e2, /* yellow */
            0x00abb3, /* cyan */
            0xead469, /* magenta */
            0x8080c0, /* silver */
        },
    },
    {   
        /* YUV444,BPP=4  packed no wastage*/
        .format = "yuv444",
        .Bpp = 4,
        .colors = {  
            0x807fff, /* white */
            0xff544c, /* red */
            0x152b96, /* green */
            0x6bff1d, /* blue */
            0x9400e2, /* yellow */
            0x00abb3, /* cyan */
            0xead469, /* magenta */
            0x8080c0, /* silver */
        },
    }
};

void gen_color_bar(uint32_t xres,uint32_t yres,uint64_t color_map[],uint8_t Bpp,uint8_t num_colors,void *src_fb)
{
	uint32_t i, j;
	uint8_t *pointer;
	uint8_t color_size = yres / num_colors;
	uint8_t color = 0;

	pointer = (uint8_t *)src_fb;

	for (i = 0; i < yres; i++) 
    {
		for (j = 0; j < xres; j++) 
        {
			color =i/color_size;
       
            uint64_t data = color_map[color];
            
            for(uint8_t k=0;k< Bpp;k++)
            {
                uint8_t val =  (data & (0xff << (8 *k))  ) >> (8*k) ;
                *pointer = val;
                 pointer++;
            }
		}
	}
    return;
}


int allocate_fb(uint32_t xres,uint32_t yres,uint8_t Bpp,void **buffer)
{

    uint32_t fb_size= (xres *  yres * Bpp /*Bytes per pixel*/) ;
    *buffer = calloc( 1, fb_size );

    if( buffer == NULL) {
        printf("Error allocating %d bytes frame buffer memory \n",fb_size);
        return -1;
    }
    printf("Frame buffer %d Bytes allocated at %p\n", fb_size, buffer);

    return 0;
}

int main(void)
{
    void *fb=NULL;

    uint8_t format=RGB565;
    uint32_t xres=640;
    uint32_t yres=480;

    allocate_fb(xres,yres,color_list[format].Bpp,&fb);
    gen_color_bar(xres,yres,color_list[format].colors,color_list[format].Bpp,8,fb);
    

    char name[10] ;
    sprintf(name,"frm.%s",color_list[format].format);
    FILE *fp = fopen(name,"wb");
    fwrite(fb,(xres *  yres * color_list[format].Bpp),1 ,fp);
    fclose(fp);
    printf("Created frame %s\n",name);

    return 0;
}
