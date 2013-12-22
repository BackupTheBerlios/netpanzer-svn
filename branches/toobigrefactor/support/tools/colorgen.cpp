
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdint.h>

#include "physfs/physfs.h"

using namespace std;

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t nothing;
} Color;

Color palette[256];
string folder;

void generateFilters();

int main ( int argc, char** argv )
{
    printf("Colorgen for NetPanzer V 0.9\n");
    if ( argc < 3 )
    {
        printf("use: %s <palette_file> <out_folder>\n", argv[0]);
        return 1;
    }

    string filename  = argv[1];
    folder = argv[2];

    if ( ! PHYSFS_init(argv[0]) )
    {
        printf("Error initializing filesystem: %s\n", PHYSFS_getLastError());
        return 1;
    }

    PHYSFS_addToSearchPath(PHYSFS_getBaseDir(), 1);
    PHYSFS_setWriteDir(PHYSFS_getBaseDir());

    if ( ! PHYSFS_exists(filename.c_str()) )
    {
        printf("You cheater! The file '%s' doesn't exists\n", filename.c_str());
        PHYSFS_deinit();
        return 1;
    }

    if ( ! PHYSFS_isDirectory(folder.c_str()) )
    {
        PHYSFS_mkdir( folder.c_str() );
        if ( ! PHYSFS_isDirectory(folder.c_str()) )
        {
            printf("Output folder '%s' doesn't exists and cannot be created\n", folder.c_str());
        }
    }

    PHYSFS_File *f = PHYSFS_openRead( filename.c_str() );
    if ( ! f )
    {
        printf("Ooops, I can't open the file because %s\n", PHYSFS_getLastError());
        PHYSFS_deinit();
        return 1;
    }

    for ( int n = 0; n < 256; n++ )
    {
        PHYSFS_read(f, &palette[n], 3, 1);
    }

    PHYSFS_close(f);

    generateFilters();

    PHYSFS_deinit();
    printf("Bye\n");
    return 0;
}

uint8_t findNearestColor(int r, int g, int b, const bool ignoreIndexZero = false)
{
    int   bestDist = 10000000;
    int   best     = 0;
    int   start    = ignoreIndexZero ? 1 : 0;

    for (int i = start; i < 256; i++)
    {
        int dr = palette[i].r-r;
        int dg = palette[i].g-g;
        int db = palette[i].b-b;
        int dist = (dr * dr) + (dg * dg) + (db * db);

        if (dist < bestDist) {
            bestDist = dist;
            best = i;
        }
    }

    return best;
}

void saveFile(const string& name, const uint8_t* data, const int len)
{
    string dest_file(folder);
    dest_file += '/';
    dest_file += name;

    PHYSFS_File *f = PHYSFS_openWrite( dest_file.c_str() );
    if ( ! f )
    {
        printf("Ooops, I can't open the file '%s' to save data because %s\n", dest_file.c_str(), PHYSFS_getLastError());
        return;
    }

    if ( PHYSFS_write(f, data, len, 1) == -1 )
    {
        printf("Ooops, something like this happened when writting the file '%s': %s\n", dest_file.c_str(), PHYSFS_getLastError());
    }
    else
    {
        printf("Saved '%s'\n", dest_file.c_str());
    }
    
    PHYSFS_close(f);
}

void generate_blend( uint8_t* buffer, const int color1Percent, const int color2Percent )
{
    unsigned curOffset = 0;

    float color1 = float(color1Percent) / 100.0f;
    float color2 = float(color2Percent) / 100.0f;

    uint8_t r,g,b;

    for ( unsigned index = 0; index < 256; index++ )
    {
        const Color * col = &palette[index];

        for ( unsigned indexPic = 0; indexPic < 256; indexPic++ )
        {
            const Color * colPic = &palette[indexPic];

            curOffset = (int(index) << 8) + indexPic;

            r = (uint8_t) (color1 * col->r + color2 * colPic->r);
            g = (uint8_t) (color1 * col->g + color2 * colPic->g);
            b = (uint8_t) (color1 * col->b + color2 * colPic->b);

            // Makes the color table use color 0 as transparent.
            if (indexPic == 0)
            {
                buffer[curOffset] = index;
            }
            else
            {
                buffer[curOffset] = findNearestColor(r, g, b);
            }
        }
    }
}

void generate_brighten( uint8_t* buffer, const int brightness )
{
    int   curOffset;
    int   curRed;
    int   curGreen;
    int   curBlue;
    float nb;        // The new brightness color.

    float fBrightness = float(brightness) / 256.0f;

    for (int y = 0; y < 256; y++)
    {
        for (int x = 0; x < 256; x++)
        {
            nb = float(x) * fBrightness;
            curOffset = (y * 256) + x;

            // !SOMDEDAY! Try holding a threshold when any value gets to 255.
            curRed   = (int) (nb + palette[y].r);
            curGreen = (int) (nb + palette[y].g);
            curBlue  = (int) (nb + palette[y].b);

            if (curRed   > 255) curRed   = 255;
            if (curGreen > 255) curGreen = 255;
            if (curBlue  > 255) curBlue  = 255;
            //curColor = Palette::color[y];

            buffer[curOffset] = findNearestColor(curRed, curGreen, curBlue);
        }
    }
}

void generate_darken( uint8_t* buffer, const float fudgeValue )
{
    float    curPercent;
    int      curOffset;
    uint8_t  r, g, b;
    const float percent = fudgeValue;

    for (int y = 0; y < 256; y++)
    {
        for (int x = 0; x < 256; x++)
        {
            curPercent = (float(255 - x) / 255.0f) * percent + 1.0f - percent;
            curOffset  = (y * 256) + x;

            r = (uint8_t) (curPercent * float(palette[y].r));
            g = (uint8_t) (curPercent * float(palette[y].g));
            b = (uint8_t) (curPercent * float(palette[y].b));

            buffer[curOffset] = findNearestColor(r, g, b);
        }
    }
}

void generate_lightdark( uint8_t* buffer )
{
    int curOffset;
    int curRed;
    int curGreen;
    int curBlue;

    for ( int y = 0; y < 256; y++ )
    {
        int x;
        for (x = 0; x <= 128; x++)
        {
            curOffset = x + (y << 8);
            curRed   = palette[y].r   * x / 128;
            curGreen = palette[y].g * x / 128;
            curBlue  = palette[y].b  * x / 128;

            buffer[curOffset] = findNearestColor(curRed, curGreen, curBlue);
        }

        for (x = 129; x < 256; x++)
        {
            curOffset = x + (y << 8);
            curRed   = palette[y].r + ((255 - palette[y].r) * (x-128) / 127);
            curGreen = palette[y].g + ((255 - palette[y].g) * (x-128) / 127);
            curBlue  = palette[y].b + ((255 - palette[y].b) * (x-128) / 127);

            buffer[curOffset] = findNearestColor(curRed, curGreen, curBlue);
        }
    }
}

void generate_alpha16( uint8_t * buffer )
{
    int r, g, b;
    int a1, a2;

    Color * c1;
    Color * c2;

    uint8_t final;

    int off_1;

    for ( int color_1 = 0; color_1 < 256; color_1++ )
    {
        c1 = &palette[color_1];

        for ( int alpha = 0; alpha < 16; alpha++ )
//        for ( int alpha = 0; alpha < 256; alpha++ )
        {
            off_1 = (color_1 * (16 * 256)) + (alpha*256) ;
//            off_1 = (color_1 * (256 * 256)) + (alpha*256) ;

            a1 = alpha;
            a2 = 15 - alpha;
//            a2 = 255 - alpha;

            for ( int color_2 = 0; color_2 < 256; color_2++ )
            {
                c2 = &palette[color_2];

                r = ((c1->r * a1 / 15) + (c2->r * a2 / 15));
                g = ((c1->g * a1 / 15) + (c2->g * a2 / 15));
                b = ((c1->b * a1 / 15) + (c2->b * a2 / 15));
//                r = ((c1->r * a1 / 255) + (c2->r * a2 / 255));
//                g = ((c1->g * a1 / 255) + (c2->g * a2 / 255));
//                b = ((c1->b * a1 / 255) + (c2->b * a2 / 255));

                final = findNearestColor(r, g, b);

                buffer[off_1 + color_2] = final;
            }
        }
    }
}

void generateFilters()
{
    uint8_t buffer[256*256*16];

    int n;

    // DarkGray256
    for ( n = 0; n < 256; n++ )
    {
        int c = palette[n].r / 3;// dark gray
        buffer[n] = findNearestColor(c, c, c); // dark gray
    }
    saveFile("filter_darkgray.fil", buffer, 256);

    // DarkBrown256
    for ( n = 0; n < 256; n++ )
    {
        // the magic values are for palete color 33
        int r = abs(palette[n].r - 0x16)/4.5;
        int g = abs(palette[n].g - 0x1a)/4.5;
        int b = abs(palette[n].b - 0x19)/4.5;
        buffer[n] = findNearestColor(0x16 + r,0x1a + g,0x19 + b);
    }
    saveFile("filter_darkbrown.fil", buffer, 256);

    // green256
    for ( n = 0; n < 256; n++ )
    {
        // the magic values are for palete color 41
        int r = abs(palette[n].r - 0x1b)/3;
        int g = abs(palette[n].g - 0x3e)/3;
        int b = abs(palette[n].b - 0x21)/3;
        buffer[n] = findNearestColor(0x1b + r,0x3e + g,0x21 + b);
    }
    saveFile("filter_green.fil", buffer, 256);

    // brightness256
    for ( n = 0; n < 256; n++ )
    {
        int c = palette[n].r / 1.3;
        buffer[n] = findNearestColor(c, c, c);
    }
    saveFile("filter_brightness.fil", buffer, 256);

    generate_blend(buffer, 20, 80);
    saveFile("blend_2080.tbl", buffer, 256*256);

    generate_blend(buffer, 40, 60);
    saveFile("blend_4060.tbl", buffer, 256*256);

    generate_blend(buffer, 60, 40);
    saveFile("blend_6040.tbl", buffer, 256*256);

    generate_blend(buffer, 80, 20);
    saveFile("blend_8020.tbl", buffer, 256*256);

    generate_brighten(buffer, 256);
    saveFile("blend_brighten.tbl", buffer, 256*256);

    generate_darken(buffer, 0.5f);
    saveFile("blend_darkenalot.tbl", buffer, 256*256);

    generate_darken(buffer, 0.15f);
    saveFile("blend_darkenalittle.tbl", buffer, 256*256);

    generate_lightdark(buffer);
    saveFile("blend_lightdark.tbl", buffer, 256*256);

    generate_alpha16(buffer);
    saveFile("full_alpha16.tbl", buffer, 256*256*16);

}

