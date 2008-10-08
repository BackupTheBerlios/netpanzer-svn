print("Setting colors");
    Color.unitAqua        = ColorUtil.makeColor(  0, 204, 255);
print("unitAqua is " .. Color.unitAqua);
    Color.unitYellow      = ColorUtil.makeColor(255, 255,   0);
    Color.unitRed         = ColorUtil.makeColor(255,   0,   0);
    Color.unitBlue        = ColorUtil.makeColor(  3,   3, 255);
    Color.unitDarkBlue    = ColorUtil.makeColor(  0,   0, 128);
    Color.unitLightGreen  = ColorUtil.makeColor(163, 214, 159);
    Color.unitGreen       = ColorUtil.makeColor( 31, 166,  36);
    Color.unitBlueGray    = ColorUtil.makeColor( 43,  85, 131);
    Color.unitDarkRed     = ColorUtil.makeColor(150,   0,   0);
    Color.unitBlack       = ColorUtil.makeColor(  6,   2,   2);
    Color.unitDarkGreen   = ColorUtil.makeColor( 26,  88,  36);
    Color.unitWhite       = ColorUtil.makeColor(250, 253, 248);
    Color.unitLightOrange = ColorUtil.makeColor(246, 210,  99);
    Color.unitOrange      = ColorUtil.makeColor(205, 144,   9);
    Color.unitGray        = ColorUtil.makeColor(186, 186, 186);
    Color.unitDarkGray    = ColorUtil.makeColor( 99, 100, 102);

    -- Grays
    Color.black   = ColorUtil.makeColor(  0,   0,   0, true);
    Color.gray32  = ColorUtil.makeColor( 32,  32,  32);
    Color.gray64  = ColorUtil.makeColor( 64,  64,  64);
    Color.gray96  = ColorUtil.makeColor( 96,  96,  96);
    Color.gray128 = ColorUtil.makeColor(128, 128, 128);
    Color.gray160 = ColorUtil.makeColor(160, 160, 160);
    Color.gray192 = ColorUtil.makeColor(192, 192, 192);
    Color.gray224 = ColorUtil.makeColor(224, 224, 224);
    Color.white   = ColorUtil.makeColor(255, 255, 255);
print("white is " .. Color.white);
    -- Dark Colors !FIXME!
    Color.darkGray    = Color.gray64;
    Color.darkRed     = ColorUtil.makeColor(128,   0,   0); -- fixed
    Color.darkGreen   = ColorUtil.makeColor(  0, 128,   0); -- fixed
    Color.darkBlue    = ColorUtil.makeColor(  0,   0, 128); -- fixed
    Color.darkYellow  = ColorUtil.makeColor(255, 168,  18); -- fixed
    Color.darkOrange  = ColorUtil.makeColor(255, 140,   0); -- fixed
    Color.darkMagenta = ColorUtil.makeColor(148,   0, 211); -- fixed
    Color.darkCyan    = ColorUtil.makeColor(  0, 206, 209); -- fixed

    -- Normal Colors
    Color.gray    = Color.gray128;
    Color.red     = ColorUtil.makeColor(255,   0,   0); -- fixed
    Color.green   = ColorUtil.makeColor(  0, 255,   0); -- fixed
    Color.blue    = ColorUtil.makeColor(  0,   0, 255); -- fixed
    Color.yellow  = ColorUtil.makeColor(255, 255,   0); -- fixed
    Color.orange  = ColorUtil.makeColor(255, 128,   0); -- fixed
    Color.magenta = ColorUtil.makeColor(255,   0, 255); -- fixed
    Color.cyan    = ColorUtil.makeColor(  0, 255, 255); -- fixed
    --Color.brown   = ColorUtil.makeColor(128,  42,  42); -- fixed
    Color.brown   = ColorUtil.makeColor(200,  80,  80); -- new brown is lighter
    
    --tan                   210 180 140   0.8235 0.7059 0.5490
    Color.tan            = ColorUtil.makeColor(210, 180, 140); -- fixed
    --chartreuse            127 255   0   0.4980 1.0000 0.0000
    Color.chartreuse     = ColorUtil.makeColor(127, 255,   0); -- fixed
    --cobalt_green           61 145  64   0.2400 0.5700 0.2500
    Color.cobaltGreen    = ColorUtil.makeColor( 61, 145,  64); -- fixed
    --emerald_green           0 201  87   0.0000 0.7900 0.3400
    Color.emeraldGreen   = ColorUtil.makeColor(  0, 201,  87); -- fixed
    --forest_green           34 139  34   0.1333 0.5451 0.1333
    Color.forestGreen    = ColorUtil.makeColor( 34, 139,  34); -- fixed
    --olive_green_dark       85 107  47   0.3333 0.4196 0.1843
    Color.darkOliveGreen = ColorUtil.makeColor( 85, 107,  47); -- fixed
    --terre_verte            56  94  15   0.2200 0.3700 0.0600
    Color.terreVerte     = ColorUtil.makeColor( 56,  94,  15); -- fixed

    -- Normal Colors !FIXME!
    Color.lightGray    = Color.gray192;
    Color.lightRed     = ColorUtil.makeColor(255, 160, 122); -- fixed
    Color.lightGreen   = ColorUtil.makeColor( 50, 205,  50); -- fixed
    Color.lightBlue    = ColorUtil.makeColor(135, 206, 250); -- fixed
    Color.lightYellow  = ColorUtil.makeColor(255, 255, 224); -- fixed
    Color.lightOrange  = ColorUtil.makeColor(237, 145,  33); -- fixed
    Color.lightMagenta = ColorUtil.makeColor(  0, 255, 255);
    Color.lightCyan    = ColorUtil.makeColor(224, 255, 255); -- fixed

