class SCOMasks {
	
	static const string DEFAULT_PATH = "MyMODS/sFramework/GUI/textures/masks/";
	
	static const string BILINEAR_HORIZONTAL          = DEFAULT_PATH+"bilinear_horizontal.edds";
	static const string BILINEAR_HORIZONTAL_INVERTED = DEFAULT_PATH+"bilinear_horizontal_inverted.edds";
	static const string BILINEAR_VERTICAL            = DEFAULT_PATH+"bilinear_vertical.edds";
	static const string BILINEAR_VERTICAL_INVERTED   = DEFAULT_PATH+"bilinear_vertical_inverted.edds";
	
	static const string BOX                          = DEFAULT_PATH+"box.edds";
	static const string BOX_INVERTED                 = DEFAULT_PATH+"box_inverted.edds";
	
	static const string CIRCULAR                     = DEFAULT_PATH+"circular.edds";
	static const string CIRCULAR_INVERTED            = DEFAULT_PATH+"circular_inverted.edds";
	
	static const string LINEAR_DOWN_TOP              = DEFAULT_PATH+"linear_down_top.edds";
	static const string LINEAR_DOWNLEFT_TOPRIGHT     = DEFAULT_PATH+"linear_dl_tr.edds";
	static const string LINEAR_DOWNRIGHT_TOPLEFT     = DEFAULT_PATH+"linear_down_top.edds";
	static const string LINEAR_TOPDOWN               = DEFAULT_PATH+"linear_top_down.edds";
	static const string LINEAR_TOPLEFT_DOWNRIGHT     = DEFAULT_PATH+"linear_tl_dr.edds";
	static const string LINEAR_TOPRIGHT_DOWNLEFT     = DEFAULT_PATH+"linear_tl_dr.edds";
	static const string LINEAR_LEFT_RIGHT            = DEFAULT_PATH+"linear_left_right.edds";
	static const string LINEAR_RIGHT_LEFT            = DEFAULT_PATH+"linear_right_left.edds";
	
	static const string BINOCULARS                   = DEFAULT_PATH+"binoculars.edds";
	static const string BINOCULARS_INVERTED          = DEFAULT_PATH+"binoculars_inverted.edds";
	
	static const string DEBUG                        = DEFAULT_PATH+"misc.edds";

}

enum eSCOPriority {
	DEFAULT = 0,
	HEADGEAR = 10,
	EYEGEAR = 20,
	EYE = 30,
	UI = 100
}