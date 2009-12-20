LOGGER:log("Setting colors");
    Color.unitAqua        = Palette:makeColor(  0, 204, 255);
LOGGER:log("unitAqua is " .. Color.unitAqua);
    Color.unitYellow      = Palette:makeColor(255, 255,   0);
    Color.unitRed         = Palette:makeColor(255,   0,   0);
    Color.unitBlue        = Palette:makeColor(  3,   3, 255);
    Color.unitDarkBlue    = Palette:makeColor(  0,   0, 128);
    Color.unitLightGreen  = Palette:makeColor(163, 214, 159);
    Color.unitGreen       = Palette:makeColor( 31, 166,  36);
    Color.unitBlueGray    = Palette:makeColor( 43,  85, 131);
    Color.unitDarkRed     = Palette:makeColor(150,   0,   0);
    Color.unitBlack       = Palette:makeColor(  6,   2,   2);
    Color.unitDarkGreen   = Palette:makeColor( 26,  88,  36);
    Color.unitWhite       = Palette:makeColor(250, 253, 248);
    Color.unitLightOrange = Palette:makeColor(246, 210,  99);
    Color.unitOrange      = Palette:makeColor(205, 144,   9);
    Color.unitGray        = Palette:makeColor(186, 186, 186);
    Color.unitDarkGray    = Palette:makeColor( 99, 100, 102);

    -- Grays
    Color.black   = Palette:makeColor(  0,   0,   0); -- had a ", true" at end
    Color.gray32  = Palette:makeColor( 32,  32,  32);
    Color.gray64  = Palette:makeColor( 64,  64,  64);
    Color.gray96  = Palette:makeColor( 96,  96,  96);
    Color.gray128 = Palette:makeColor(128, 128, 128);
    Color.gray160 = Palette:makeColor(160, 160, 160);
    Color.gray192 = Palette:makeColor(192, 192, 192);
    Color.gray224 = Palette:makeColor(224, 224, 224);
    Color.white   = Palette:makeColor(255, 255, 255);
LOGGER:log("white is " .. Color.white);
    -- Dark Colors !FIXME!
    Color.darkGray    = Color.gray64;
    Color.darkRed     = Palette:makeColor(128,   0,   0); -- fixed
    Color.darkGreen   = Palette:makeColor(  0, 128,   0); -- fixed
    Color.darkBlue    = Palette:makeColor(  0,   0, 128); -- fixed
    Color.darkYellow  = Palette:makeColor(255, 168,  18); -- fixed
    Color.darkOrange  = Palette:makeColor(255, 140,   0); -- fixed
    Color.darkMagenta = Palette:makeColor(148,   0, 211); -- fixed
    Color.darkCyan    = Palette:makeColor(  0, 206, 209); -- fixed

    -- Normal Colors
    Color.gray    = Color.gray128;
    Color.red     = Palette:makeColor(255,   0,   0); -- fixed
    Color.green   = Palette:makeColor(  0, 255,   0); -- fixed
    Color.blue    = Palette:makeColor(  0,   0, 255); -- fixed
    Color.yellow  = Palette:makeColor(255, 255,   0); -- fixed
    Color.orange  = Palette:makeColor(255, 128,   0); -- fixed
    Color.magenta = Palette:makeColor(255,   0, 255); -- fixed
    Color.cyan    = Palette:makeColor(  0, 255, 255); -- fixed
    --Color.brown   = Palette:makeColor(128,  42,  42); -- fixed
    Color.brown   = Palette:makeColor(200,  80,  80); -- new brown is lighter
    
    --tan                   210 180 140   0.8235 0.7059 0.5490
    Color.tan            = Palette:makeColor(210, 180, 140); -- fixed
    --chartreuse            127 255   0   0.4980 1.0000 0.0000
    Color.chartreuse     = Palette:makeColor(127, 255,   0); -- fixed
    --cobalt_green           61 145  64   0.2400 0.5700 0.2500
    Color.cobaltGreen    = Palette:makeColor( 61, 145,  64); -- fixed
    --emerald_green           0 201  87   0.0000 0.7900 0.3400
    Color.emeraldGreen   = Palette:makeColor(  0, 201,  87); -- fixed
    --forest_green           34 139  34   0.1333 0.5451 0.1333
    Color.forestGreen    = Palette:makeColor( 34, 139,  34); -- fixed
    --olive_green_dark       85 107  47   0.3333 0.4196 0.1843
    Color.darkOliveGreen = Palette:makeColor( 85, 107,  47); -- fixed
    --terre_verte            56  94  15   0.2200 0.3700 0.0600
    Color.terreVerte     = Palette:makeColor( 56,  94,  15); -- fixed

    -- Normal Colors !FIXME!
    Color.lightGray    = Color.gray192;
    Color.lightRed     = Palette:makeColor(255, 160, 122); -- fixed
    Color.lightGreen   = Palette:makeColor( 50, 205,  50); -- fixed
    Color.lightBlue    = Palette:makeColor(135, 206, 250); -- fixed
    Color.lightYellow  = Palette:makeColor(255, 255, 224); -- fixed
    Color.lightOrange  = Palette:makeColor(237, 145,  33); -- fixed
    Color.lightMagenta = Palette:makeColor(  0, 255, 255);
    Color.lightCyan    = Palette:makeColor(224, 255, 255); -- fixed

