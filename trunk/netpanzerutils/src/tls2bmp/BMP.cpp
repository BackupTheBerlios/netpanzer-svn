/*
Copyright (C) 2003 Matthias Braun <matze@braunis.de>
                                                                                
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
                                                                                
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
                                                                                
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <config.h>

#include "BMP.hpp"
#include "2D/Surface.hpp"
#include "Util/FileSystem.hpp"
#include "2D/Palette.hpp"

class BitmapFileHeader
  {
   public:
   uint16_t    bfType;
   uint32_t   bfSize;
   uint16_t    bfReserved1;
   uint16_t    bfReserved2;
   uint32_t   bfOffBits;
  } __attribute__((packed));

#define BI_RGB      0L
#define BI_RLE8     1L
#define BI_RLE4     2L
                                                                                
class BitmapInfoHeader
 {
  public:
  uint32_t  biSize;
  uint32_t  biWidth;
  uint32_t  biHeight;
  uint16_t   biPlanes;
  uint16_t   biBitCount;
  uint32_t  biCompression;
  uint32_t  biSizeImage;
  uint32_t  biXPelsPerMeter;
  uint32_t  biYPelsPerMeter;
  uint32_t  biClrUsed;
  uint32_t  biClrImportant;
 } __attribute__((packed));

class RGBQuad
{
public:
    uint8_t    rgbBlue;
    uint8_t    rgbGreen;
    uint8_t    rgbRed;
    uint8_t    rgbReserved;
}
__attribute__((packed));


void BMPSaver::save(const std::string& filename, Surface* surface, Palette& pal)
{
  std::auto_ptr<WriteFile> file(FileSystem::openWrite(filename));

  BitmapFileHeader file_header;
  BitmapInfoHeader info_header;
  RGBQuad palette[256];
                                                                                
  file_header.bfType = 0x4D42;
  file_header.bfOffBits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader)
                           + (sizeof(RGBQuad) * 256);
  file_header.bfSize = file_header.bfOffBits + (surface->getPix().x * surface->getPix().y);
  file_header.bfReserved1 = 0;
  file_header.bfReserved2 = 0;
                                                                                
  info_header.biSize = sizeof(BitmapInfoHeader);
  info_header.biWidth = surface->getPix().x;
  info_header.biHeight = surface->getPix().y;
  info_header.biPlanes = 1;
  info_header.biBitCount = 8;
  info_header.biCompression = BI_RGB;
  info_header.biSizeImage= surface->getPix().x * surface->getPix().y;
  info_header.biXPelsPerMeter = 0;
  info_header.biYPelsPerMeter = 0;
  info_header.biClrUsed = 256;
  info_header.biClrImportant = 256;
  
  file->write(&file_header, sizeof(BitmapFileHeader), 1);
  file->write(&info_header, sizeof(BitmapInfoHeader), 1);
                                                                                
  for(int index = 0; index < 256; index++)
   {
        RGBColor color;
        color = pal[ index ];
                                                                                
        palette[index].rgbRed = color.red;
        palette[index].rgbGreen = color.green;
        palette[index].rgbBlue = color.blue;
        palette[index].rgbReserved = 0;
   }//end for index that loads the palette
                                                                                
  file->write( palette, sizeof(RGBQuad), 256);
                                                                                
  surface->flipVertical();
                                                                                
    if ( (info_header.biWidth % 4) == 0 )
     {
      file->write( surface->getMem(), info_header.biSizeImage, 1);
     }
    else
     {
      int padding = ((info_header.biWidth / 4 + 1) * 4) - info_header.biWidth;
                                                                                
          PIX buffer[10];
      int numRows = surface->getPixY();
                                                                                
      //PIX *sPtr = mem;
                                                                                
      for (int row = 0; row < numRows; row++)
      {
        file->write(surface->mem, surface->getPix().x, 1);
        file->write(buffer, padding, 1);
        surface->mem += surface->getStride();
      }
    }                                                                       
  surface->flipVertical();
}

