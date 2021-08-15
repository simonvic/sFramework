//@todo use mask when getting values because int are signed
class SColor {

	protected int argbValue = 0xff000000; //current color represented in ARGB format
	
	void SColor(int r = 0, int g = 0, int b = 0, int a = 255) {
		setRGBA(r, g, b, a);
	}
	
	/**
	*	@brief Set current color using r g b values
	*	 @param r \p int - Red
	*	 @param g \p int - Green
	*	 @param b \p int - Blue
	*/
	SColor setRGB(int r, int g, int b) {
		argbValue = (getAlpha() << 24 ) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8)  | ((b & 0xFF) << 0);
		return this;
	}
	
	/**
	*	@brief Set current color using RGB value (e.g. 0xF0544C)
	*	 @param rgb \p int - Color represented in RGB format
	*/
	SColor setRGB(int rgb){
		setARGB((getAlpha() << 24 ) | rgb);
		return this;
	}
	
	
	/**
	*	@brief Set current color using r g b a values
	*	 @param r \p int - Red
	*	 @param g \p int - Green
	*	 @param b \p int - Blue
	*	 @param a \p int - Alpha
	*/
	SColor setRGBA(int r, int g, int b, int a) {
		setARGB(a, r, g, b);
		return this;
	}
		
	/**
	*	@brief Set current color using RGBA value (e.g. 0xF0544Caa)
	*	 @param rgba \p int - Color represented in RGBA format
	*/
	SColor setRGBA(int rgba){
		argbValue = toARGB(rgba);
		return this;
	}
	
	
	/**
	*	@brief Set current color using a r g b  values
	*	 @param a \p int - Alpha
	*	 @param r \p int - Red
	*	 @param g \p int - Green
	*	 @param b \p int - Blue
	*/
	SColor setARGB(int a, int r, int g, int b) {
		argbValue = ((a & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8)  | ((b & 0xFF) << 0);
		return this;
	}
	
	/**
	*	@brief Set current color using ARGB value (e.g. 0xaaF0544C)
	*	 @param argb \p int - Color represented in ARGB format
	*/
	SColor setARGB(int argb){
		argbValue = argb;
		return this;
	}
	
	/**
	*	@brief Set current color using hue, saturation, brightness values [0 - 1] (e.g. 0.03, 0.683, 0.941)
	*	 @param hue \p float - Hue of color [0 - 1]
	*	 @param saturation \p float - Saturation of color [0 - 1]
	*	 @param brightness \p float - Brightness of color [0 - 1]
	*/
	SColor setHSB(float hue, float saturation, float brightness){
		setARGB((getAlpha() << 24) | SColor.HSBtoRGB(hue, saturation, brightness));
		return this;
	}
	
	/**
	*	@brief Brighten the current color
	*	 @param factor \p float - Factor of brightening [0 - 1]
	*/
	SColor brighten(float factor = 0.7) {
		int r = getRed();
		int g = getGreen();
		int b = getBlue();
		int alpha = getAlpha();
		
		int i = (1.0/(1.0-factor));
		if ( r == 0 && g == 0 && b == 0) {
			setRGBA(i, i, i, alpha);
			return this;
		}
		if ( r > 0 && r < i ) r = i;
		if ( g > 0 && g < i ) g = i;
		if ( b > 0 && b < i ) b = i;

		setRGBA(Math.Min((r/factor), 255),
		        Math.Min((g/factor), 255),
		        Math.Min((b/factor), 255),
		        alpha);
		return this;
		
	}

	/**
	*	@brief Darken the current color
	*	 @param factor \p float - Factor of darkening [0 - 1]
	*/
	SColor darken(float factor = 0.7) {
		setRGBA(Math.Max((getRed() * factor), 0),
		        Math.Max((getGreen() * factor), 0),
		        Math.Max((getBlue() * factor), 0),
		        getAlpha());
		return this;
	}

	/**
	*	@brief Get the current color values in RGBA format
	*	 @param rgba \p float[4] - Float array containing RGBA values
	*/
	SColor collectRGBA(out float rgba[4]) {
		rgba[0] = getRed();
		rgba[1] = getGreen();
		rgba[2] = getBlue();
		rgba[3] = getAlpha();
		return this;
	}

	/**
	*	@brief Get the current color values in RGB format
	*	 @param rgb \p float[3] - Float array containing RGB values
	*/
	SColor collectRGB(out float rgb[3]) {
		rgb[0] = getRed();
		rgb[1] = getGreen();
		rgb[2] = getBlue();
		return this;
	}
	
	/**
	*	@brief Get the current color represented in RGB format
	*	 @return int - RGB color
	*/
	int getRGB(){
		return (getRGBA() >> 8) & 0xFFFFFF; //@todo masked to reset sign bit. Find general solution to make it unsigned
	}
		
	/**
	*	@brief Get the current color represented in RGBA format
	*	 @return int - RGBA color
	*/
	int getRGBA(){
		return SColor.toRGBA(getARGB());
	}
	
	/**
	*	@brief Get the current color represented in ARGB format
	*	 @return int - ARGB color
	*/
	int getARGB() {
		return argbValue;
	}
	
	/**
	*	@brief Get the current alpha value (opacity)
	*	 @return int - Alpha
	*/
	int getAlpha() {
		return SColor.getAlpha(getARGB());
	}

	/**
	*	@brief Get the current red value
	*	 @return int - Red
	*/
	int getRed() {
		return SColor.getRed(getARGB());
	}

	/**
	*	@brief Get the current green value
	*	 @return int - Green
	*/
	int getGreen() {
		return SColor.getGreen(getARGB());
	}

	/**
	*	@brief Get the current blue value
	*	 @return int - Blue
	*/
	int getBlue() {
		return SColor.getBlue(getARGB());
	}
	
	
	/**
	*	@brief Get the current color name (if present in RGBColors)
	*	 @return string - Color name
	*/
	string getRGBName(){
		return SColor.getRGBName(this);
	}
	
	bool equals(SColor color){
		return SColor.equals(this, color);
	}
	
	bool equals(int argb){
		return SColor.equals(getARGB(), argb);
	}
	
	SColor debugPrint(bool verbose = false){
		SLog.d(string.Format("R: %1 | G: %2 | B: %3 | A: %4", getRed(), getGreen(), getBlue(), getAlpha()), ""+this);
		SLog.d(string.Format("RGB: %1 | RGBA: %2 | ARGB: %3 ", getRGB(), getRGBA(), getARGB()), "", 1, verbose);
		SLog.d(string.Format("Name: %1 | Hex: %2 ", getRGBName(), "#696969"), "", 1, verbose);
		return this;
	}
	
	////////////////////////////////////////////
	// STATIC METHODS
	
	/**
	*	@brief Build a color using another color represented in RGB format
	*	 @param rgb \p int - Color
	*	 @return SColor
	*/
	static SColor rgb(int rgb) {
		return (new SColor()).setRGB(rgb);
	}
	
	/**
	*	@brief Build a color using R G B values
	*	 @param r \p int - Red
	*	 @param g \p int - Green
	*	 @param b \p int - Blue
	*	 @return SColor
	*/
	static SColor rgb2(int r, int g, int b) {
		return (new SColor()).setRGB(r, g, b);
	}
	
	
	/**
	*	@brief Build a color using another color represented in RGBA format
	*	 @param rgba \p int - Color
	*	 @return SColor
	*/
	static SColor rgba(int rgba) {
		return (new SColor()).setRGBA(rgba);
	}
	
	/**
	*	@brief Build a color using R G B A values
	*	 @param r \p int - Red
	*	 @param g \p int - Green
	*	 @param b \p int - Blue
	*	 @param a \p int - Alpha
	*	 @return SColor
	*/
	static SColor rgba(int r, int g, int b, int a) {
		return (new SColor()).setRGBA(r, g, b, a);
	}
	
	
	/**
	*	@brief Build a color using another color represented in ARGB format
	*	 @param argb \p int - Color
	*	 @return SColor
	*/
	static SColor argb(int argb) {
		return (new SColor()).setARGB(argb);
	}
	
	/**
	*	@brief Build a color using A R G B values
	*	 @param a \p int - Alpha
	*	 @param r \p int - Red
	*	 @param g \p int - Green
	*	 @param b \p int - Blue
	*	 @return SColor
	*/
	static SColor argb(int a, int r, int g, int b){
		return (new SColor()).setARGB(a, r, g, b);
	}
	
	
	/**
	*	@brief Build a color using hue, saturation and brightness values
	*	 @param hue \p float - Hue of color [0 - 1]
	*	 @param saturation \p float - Saturation of color [0 - 1]
	*	 @param brightness \p float - Brightness of color [0 - 1]
	*	 @return SColor
	*/
	static SColor hsb(float hue, float saturation, float brightness){
		return (new SColor()).setHSB(hue, saturation, brightness);
	}
	
	/**
	*	@brief Get the alpha value of the color represented in ARGB
	*	 @param argb \p int - Color
	*	 @return int - Alpha
	*/
	static int getAlpha(int argb) {
		return (argb >> 24) & 0xff;
	}

	/**
	*	@brief Get the red value of the color represented in ARGB
	*	 @param argb \p int - Color
	*	 @return int - Red
	*/
	static int getRed(int argb) {
		return (argb >> 16) & 0xFF;
	}

	/**
	*	@brief Get the green value of the color represented in ARGB
	*	 @param argb \p int - Color
	*	 @return int - Green
	*/
	static int getGreen(int argb) {
		return (argb >> 8) & 0xFF;
	}

	/**
	*	@brief Get the blue value of the color represented in ARGB
	*	 @param argb \p int - Color
	*	 @return int - Blue
	*/
	static int getBlue(int argb) {
		return (argb >> 0) & 0xFF;
	}
	
	/**
	*	@brief Convert a color represented in ARGB to RGBA
	*	 @param argb \p int - Color
	*	 @return int - Color in RGBA
	*/
	static int toRGBA(int argb){
		//remove alpha by shifting left, add alpha at the end
		return ((argb << 8) | ((argb >> 24) & 0xff));
	}
	
	/**
	*	@brief Convert a color represented in RGBA to ARGB
	*	 @param rgba \p int - Color
	*	 @return int - Color in ARGB
	*/
	static int toARGB(int rgba){
		//get the alpha bits, move them to the left, move rgba to the right, insert alpha bits
		return (((rgba >> 8) & 0x00ffffff) | ((rgba & 0x000000ff) << 24)));
	}
 
	/**
	*	@brief Convert hue, saturation and brightness values to a RGB color
	*	 @param hue \p float - Hue of color [0 - 1]
	*	 @param saturation \p float - Saturation of color [0 - 1]
	*	 @param brightness \p float - Brightness of color [0 - 1]
	*	 @return int - Color in RGB
	*/
	static int HSBtoRGB(float hue, float saturation, float brightness) {
		int r = 0, g = 0, b = 0;
		if (saturation == 0) {
			r = (brightness * 255.0 + 0.5);
			g = (brightness * 255.0 + 0.5);
			b = (brightness * 255.0 + 0.5);
		} else {
			float h = (hue - Math.Floor(hue)) * 6.0;
			float f = h - Math.Floor(h);
			float p = brightness * (1.0 - saturation);
			float q = brightness * (1.0 - saturation * f);
			float t = brightness * (1.0 - (saturation * (1.0 - f)));
			int h2 = h;
			switch (h2) {
			case 0:
				r = (brightness * 255.0 + 0.5);
				g = (t * 255.0 + 0.5);
				b = (p * 255.0 + 0.5);
				break;
			case 1:
				r = (q * 255.0 + 0.5);
				g = (brightness * 255.0 + 0.5);
				b = (p * 255.0 + 0.5);
				break;
			case 2:
				r = (p * 255.0 + 0.5);
				g = (brightness * 255.0 + 0.5);
				b = (t * 255.0 + 0.5);
				break;
			case 3:
				r = (p * 255.0 + 0.5);
				g = (q * 255.0 + 0.5);
				b = (brightness * 255.0 + 0.5);
				break;
			case 4:
				r = (t * 255.0 + 0.5);
				g = (p * 255.0 + 0.5);
				b = (brightness * 255.0 + 0.5);
				break;
			case 5:
				r = (brightness * 255.0 + 0.5);
				g = (p * 255.0 + 0.5);
				b = (q * 255.0 + 0.5);
				break;
			}
		}
		return (r << 16) | (g << 8) | (b << 0);
	}
	
	/**
	*	@brief Convert an RGB color to hue, saturation and brightness values
	*	 @param r \p int - Red
	*	 @param g \p int - Green
	*	 @param b \p int - Blue
	*	 @param hue \p float - Hue of color [0 - 1]
	*	 @param saturation \p float - Saturation of color [0 - 1]
	*	 @param brightness \p float - Brightness of color [0 - 1]
	*/
	static void RGBtoHSB(int r, int g, int b, out float hue, out float saturation, out float brightness) {
		int cmax;
		if (r > g) cmax = r; else cmax = g;
		if (b > cmax) cmax = b;
		int cmin;
		if (r < g) cmin = r; else cmin = g;
		if (b < cmin) cmin = b;

		brightness = ( cmax) / 255.0;
		if (cmax != 0)
			saturation = ( (cmax - cmin)) / ( cmax);
		else
			saturation = 0;
		if (saturation == 0)
			hue = 0;
		else {
			float redc = ( (cmax - r)) / ( (cmax - cmin));
			float greenc = ( (cmax - g)) / ( (cmax - cmin));
			float bluec = ( (cmax - b)) / ( (cmax - cmin));
			if (r == cmax)
				hue = bluec - greenc;
			else if (g == cmax)
				hue = 2.0 + redc - bluec;
			else
				hue = 4.0 + greenc - redc;
			hue = hue / 6.0;
			if (hue < 0)
				hue = hue + 1.0;
		}
	}
	
	static string getRGBName(SColor color){
		return typename.EnumToString(RGBColors, Math.AbsInt(color.getRGB()));
	}
	
	static bool equals(SColor color1, SColor color2) {
		if(color1 == color2) return true;
		if(color1 == null || color2 == null) return false;
		return equals(color1.getARGB(), color2.getARGB());
	}
	
	static bool equals(int argb1, int argb2) {
		return argb1 == argb2;
	}
	
}

////////////////////////////////////////////
// COLORS PRESETS
// Colors from https://www.w3schools.com/cssref/css_colors.asp

enum RGBColors{
	ALICE_BLUE               = 0xF0F8FF ,
	ANTIQUE_WHITE            = 0xFAEBD7 ,
	AQUA                     = 0x00FFFF ,
	AQUAMARINE               = 0x7FFFD4 ,
	AZURE                    = 0xF0FFFF ,
	BEIGE                    = 0xF5F5DC ,
	BISQUE                   = 0xFFE4C4 ,
	BLACK                    = 0x000000 ,
	BLANCHED_ALMOND          = 0xFFEBCD ,
	BLUE                     = 0x0000FF ,
	BLUE_VIOLET              = 0x8A2BE2 ,
	BROWN                    = 0xA52A2A ,
	BURLY_WOOD               = 0xDEB887 ,
	CADET_BLUE               = 0x5F9EA0 ,
	CHARTREUSE               = 0x7FFF00 ,
	CHOCOLATE                = 0xD2691E ,
	CORAL                    = 0xFF7F50 ,
	CORNFLOWER_BLUE          = 0x6495ED ,
	CORNSILK                 = 0xFFF8DC ,
	CRIMSON                  = 0xDC143C ,
	CYAN                     = 0x00FFFF ,
	DARK_BLUE                = 0x00008B ,
	DARK_CYAN                = 0x008B8B ,
	DARK_GOLDENROD           = 0xB8860B ,
	DARK_GRAY                = 0xA9A9A9 ,
	DARK_GREY                = 0xA9A9A9 ,
	DARK_GREEN               = 0x006400 ,
	DARK_KHAKI               = 0xBDB76B ,
	DARK_MAGENTA             = 0x8B008B ,
	DARK_OLIVEGREEN          = 0x556B2F ,
	DARK_ORANGE              = 0xFF8C00 ,
	DARK_ORCHID              = 0x9932CC ,
	DARK_RED                 = 0x8B0000 ,
	DARK_SALMON              = 0xE9967A ,
	DARK_SEAGREEN            = 0x8FBC8F ,
	DARK_SLATEBLUE           = 0x483D8B ,
	DARK_SLATEGRAY           = 0x2F4F4F ,
	DARK_SLATEGREY           = 0x2F4F4F ,
	DARK_TURQUOISE           = 0x00CED1 ,
	DARK_VIOLET              = 0x9400D3 ,
	DEEP_PINK                = 0xFF1493 ,
	DEEP_SKYBLUE             = 0x00BFFF ,
	DIM_GRAY                 = 0x696969 ,
	DIM_GREY                 = 0x696969 ,
	DODGER_BLUE              = 0x1E90FF ,
	FIRE_BRICK               = 0xB22222 ,
	FLORAL_WHITE             = 0xFFFAF0 ,
	FOREST_GREEN             = 0x228B22 ,
	FUCHSIA                  = 0xFF00FF ,
	GAINSBORO                = 0xDCDCDC ,
	GHOST_WHITE              = 0xF8F8FF ,
	GOLD                     = 0xFFD700 ,
	GOLDEN_ROD               = 0xDAA520 ,
	GRAY                     = 0x808080 ,
	GREY                     = 0x808080 ,
	GREEN                    = 0x008000 ,
	GREEN_YELLOW             = 0xADFF2F ,
	HONEY_DEW                = 0xF0FFF0 ,
	HOT_PINK                 = 0xFF69B4 ,
	INDIAN_RED               = 0xCD5C5C ,
	INDIGO                   = 0x4B0082 ,
	IVORY                    = 0xFFFFF0 ,
	KHAKI                    = 0xF0E68C ,
	LAVENDER                 = 0xE6E6FA ,
	LAVENDER_BLUSH           = 0xFFF0F5 ,
	LAWN_GREEN               = 0x7CFC00 ,
	LEMON_CHIFFON            = 0xFFFACD ,
	LIGHT_BLUE               = 0xADD8E6 ,
	LIGHT_CORAL              = 0xF08080 ,
	LIGHT_CYAN               = 0xE0FFFF ,
	LIGHT_GOLDENRODYELLOW    = 0xFAFAD2 ,
	LIGHT_GRAY               = 0xD3D3D3 ,
	LIGHT_GREY               = 0xD3D3D3 ,
	LIGHT_GREEN              = 0x90EE90 ,
	LIGHT_PINK               = 0xFFB6C1 ,
	LIGHT_SALMON             = 0xFFA07A ,
	LIGHT_SEAGREEN           = 0x20B2AA ,
	LIGHT_SKYBLUE            = 0x87CEFA ,
	LIGHT_SLATEGRAY          = 0x778899 ,
	LIGHT_SLATEGREY          = 0x778899 ,
	LIGHT_STEELBLUE          = 0xB0C4DE ,
	LIGHT_YELLOW             = 0xFFFFE0 ,
	LIME                     = 0x00FF00 ,
	LIME_GREEN               = 0x32CD32 ,
	LINEN                    = 0xFAF0E6 ,
	MAGENTA                  = 0xFF00FF ,
	MAROON                   = 0x800000 ,
	MEDIUM_AQUAMARINE        = 0x66CDAA ,
	MEDIUM_BLUE              = 0x0000CD ,
	MEDIUM_ORCHID            = 0xBA55D3 ,
	MEDIUM_PURPLE            = 0x9370DB ,
	MEDIUM_SEAGREEN          = 0x3CB371 ,
	MEDIUM_SLATEBLUE         = 0x7B68EE ,
	MEDIUM_SPRINGGREEN       = 0x00FA9A ,
	MEDIUM_TURQUOISE         = 0x48D1CC ,
	MEDIUM_VIOLETRED         = 0xC71585 ,
	MIDNIGHT_BLUE            = 0x191970 ,
	MINT_CREAM               = 0xF5FFFA ,
	MISTY_ROSE               = 0xFFE4E1 ,
	MOCCASIN                 = 0xFFE4B5 ,
	NAVAJO_WHITE             = 0xFFDEAD ,
	NAVY                     = 0x000080 ,
	OLD_LACE                 = 0xFDF5E6 ,
	OLIVE                    = 0x808000 ,
	OLIVE_DRAB               = 0x6B8E23 ,
	ORANGE                   = 0xFFA500 ,
	ORANGE_RED               = 0xFF4500 ,
	ORCHID                   = 0xDA70D6 ,
	PALE_GOLDENROD           = 0xEEE8AA ,
	PALE_GREEN               = 0x98FB98 ,
	PALE_TURQUOISE           = 0xAFEEEE ,
	PALE_VIOLETRED           = 0xDB7093 ,
	PAPAYA_WHIP              = 0xFFEFD5 ,
	PEACH_PUFF               = 0xFFDAB9 ,
	PERU                     = 0xCD853F ,
	PINK                     = 0xFFC0CB ,
	PLUM                     = 0xDDA0DD ,
	POWDER_BLUE              = 0xB0E0E6 ,
	PURPLE                   = 0x800080 ,
	REBECCA_PURPLE           = 0x663399 ,
	RED                      = 0xFF0000 ,
	ROSY_BROWN               = 0xBC8F8F ,
	ROYAL_BLUE               = 0x4169E1 ,
	SADDLE_BROWN             = 0x8B4513 ,
	SALMON                   = 0xFA8072 ,
	SANDY_BROWN              = 0xF4A460 ,
	SEA_GREEN                = 0x2E8B57 ,
	SEA_SHELL                = 0xFFF5EE ,
	SIENNA                   = 0xA0522D ,
	SILVER                   = 0xC0C0C0 ,
	SKY_BLUE                 = 0x87CEEB ,
	SLATE_BLUE               = 0x6A5ACD ,
	SLATE_GRAY               = 0x708090 ,
	SLATE_GREY               = 0x708090 ,
	SNOW                     = 0xFFFAFA ,
	SPRING_GREEN             = 0x00FF7F ,
	STEEL_BLUE               = 0x4682B4 ,
	TAN                      = 0xD2B48C ,
	TEAL                     = 0x008080 ,
	THISTLE                  = 0xD8BFD8 ,
	TOMATO                   = 0xFF6347 ,
	TURQUOISE                = 0x40E0D0 ,
	VIOLET                   = 0xEE82EE ,
	WHEAT                    = 0xF5DEB3 ,
	WHITE                    = 0xFFFFFF ,
	WHITE_SMOKE              = 0xF5F5F5 ,
	YELLOW                   = 0xFFFF00 ,
	YELLOW_GREEN             = 0x9ACD32

}
