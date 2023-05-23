class SCameraOverlay : Managed {
	
	/**
	*	@brief Path of the image to be loaded. It can be whatever an ImageWidget accepts
	*	 @code
	*	 	"set:dayz_crosshairs image:imperfect"                // image sets
	*	 	"MyMODS/sFramework/GUI/textures/overlays/blood.edds" // image path
	*	 	"MyMODS/sFramework/GUI/icons/logo/sUDE.paa"          // image path
	*/
	protected string m_image;
	
	/**
	*	@brief Alpha of the widget, [0.0 - 1.0]
	*/
	protected float m_alpha;
	
	/**
	*	@brief Path of the image to be loaded used as an alpha mask
	*	        Usually a white and black image, where 
	*	        WHITE : alpha=1.0
	*	        GREY  : alpha=0.5
	*	        BLACK : alpha=0.0
	*/
	protected string m_mask;
	
	/**
	*	@brief Progress determines which alpha values are opaque using the mask. For progress x, 
	*	        pixels with alpha in mask < x will be opaque and alpha in mask > x will be transparent.
	*	        For smooth transition @see m_maskTransitionWidth.
	*/
	protected float m_maskProgress;
	
	/**
	*	@brief Alpha values will be fully opaque at m_maskProgress. Values between m_maskProgress and
	*	        m_maskProgress + m_maskTransitionWidth will be smooth
	*/
	protected float m_maskTransitionWidth;
	
	/**
	*	@brief Position of the widget (ORIGIN) defined in screen space [0.0 - 1.0]
	*	        Only position[0] and position[1] is used to determine X and Y position where
	*	        0.0 : top left of the screen
	*	        0.5 : center of the screen
	*	        1.0 : bottom right of the screen
	*	 @note Values lower than 0.0 and higher than 1.0 are accepted
	*/
	protected vector m_position;
	
	/**
	*	@brief Size of the widget defined in screen space [0.0 - 1.0]
	*	        Only size[0] and size[1] is used to determine X and Y size where
	*	        0.5 : half size
	*	        1.0 : full screen
	*	 @note Values lower than 0.0 and higher than 1.0 are accepted
	*/
	protected vector m_size;
	
	/**
	*	@brief Rotation of the widget defined with Yaw, Pitch and Roll angles
	*/
	protected vector m_rotation;
	
	/**
	*	@brief Priority of the ImageWidget. It determines how close to the camera will be (also known
	*	        as z-depth).
	*	        Higher priority will bring the image widget closer to the camera
	*	        @note Vanilla UI priority is ~200. Setting values higher than this may cover the UI 
	*/
	protected int m_priority;
	
	/**
	*	@brief List of cameras, on which the overlay must be shown
	*	        Super types can be used to define multiple cameras
	*/
	protected ref array<typename> m_targetCameras = {DayZPlayerCamera};
	
	/**
	*	@brief Current visibility status of the overlay (and of the widget if already built)
	*/
	protected bool m_isVisible;
	
	/**
	*	@brief Determines if the overlay must hide when players hides ingame HUD
	*/
	protected bool m_hidesWithIngameHUD;	

	protected ref ImageWidget m_widget = null;
	
	//maybe use a builder? lol
	void SCameraOverlay(
		string image = "",
		float alpha = 1.0,
		string mask = "",
		float maskProgress = 1.0,
		float maskTransitionWidth = 0.1,
		vector position = "0.0 0.0 0.0",
		vector size = "1.0 1.0 1.0",
		vector rotation = "0.0 0.0 0.0",
		int priority = 0,
		array<typename> targetCameras = null,
		bool hidesWithIngameHUD = false) {
		
		m_image = image;
		m_alpha = alpha;
		m_mask = mask;
		m_maskProgress = maskProgress;
		m_maskTransitionWidth = maskTransitionWidth;
		m_position = position;
		m_size = size;
		m_rotation = rotation;
		m_priority = priority;
		if (targetCameras) m_targetCameras = targetCameras;
		m_hidesWithIngameHUD = hidesWithIngameHUD;
		onInit();
	}
	
	void onInit();
	void onPreBuild();
	void onPostBuild();
	
	
	///////////////////////////
	// IMAGE
	string getImageName() {
		return m_image;
	}
	
	void setImage(string image) {
		m_image = image;
		if (m_widget) m_widget.LoadImageFile(0, image);
	}
	
	
	
	///////////////////////////
	// ALPHA
	float getAlpha() {
		return m_alpha;
	}
	
	void setAlpha(float alpha) {
		m_alpha = alpha;
		if (m_widget) m_widget.SetAlpha(alpha);
	}
	
	
	
	///////////////////////////
	// MASK
	string getMask() {
		return m_mask;
	}
	
	void setMask(string mask) {
		m_mask = mask;
		if (m_widget) m_widget.LoadMaskTexture(mask);
	}
	
	float getMaskProgress() {
		return m_maskProgress;
	}
	
	void setMaskProgress(float maskProgress) {
		m_maskProgress = maskProgress;
		if (m_widget) m_widget.SetMaskProgress(maskProgress);
	}
	
	float getMaskTransitionWidth() {
		return m_maskTransitionWidth;
	}
	
	void setMaskTransitionWidth(float maskTransitionWidth) {
		m_maskTransitionWidth = maskTransitionWidth;
		if (m_widget) m_widget.SetMaskTransitionWidth(maskTransitionWidth);
	}
	
	
	
	///////////////////////////
	// POSITION
	vector getPosition() {
		return m_position;
	}
	
	void setPosition(float x, float y) {
		setPosition(Vector(x, y, 0));
	}
	
	void setPosition(vector position) {
		m_position = position;
		if (m_widget) m_widget.SetPos(position[0], position[1]);
	}
	
	
	
	///////////////////////////
	// SIZE
	vector getSize() {
		return m_size;
	}
	
	void setSize(float size) {
		setSize(Vector(size, size, 0));
	}
	
	void setSize(float x, float y) {
		setSize(Vector(x, y, 0));
	}
	
	void setSize(vector size) {
		m_size = size;
		if (m_widget) m_widget.SetSize(size[0], size[1]);
	}
	
	
	
	///////////////////////////
	// ROTATION
	vector getRotation() {
		return m_rotation;
	}
	
	void setRotation(float yaw, float pitch, float roll) {
		setRotation(Vector(yaw, pitch, roll));
	}
	
	void setRotation(vector rotation) {
		m_rotation = rotation;
		if (m_widget) m_widget.SetRotation(rotation[1], rotation[0], rotation[2]);
	}
	
	
	
	///////////////////////////
	// PRIORITY
	int getPriority() {
		return m_priority;
	}
	
	void setPriority(int priority) {
		m_priority = priority;
		if (m_widget) m_widget.SetSort(priority);
	}
	
	
	
	///////////////////////////
	// TARGET CAMAERAS
	array<typename> getTargetCameras() {
		return m_targetCameras;
	}
	
	void setTargetCameras(array<typename> targetCameras) {
		if (targetCameras) m_targetCameras = targetCameras;
	}
	
	
	
	///////////////////////////
	// VISIBILITY
	bool isVisible() {
		return m_isVisible;
	}
	
	void setVisible(bool visible) {
		m_isVisible = visible;
		if (m_widget) m_widget.Show(visible);
	}
	
	
	bool hidesWithIngameHUD() {
		return m_hidesWithIngameHUD;
	}
	
	void setHidesWithIngameHUD(bool hidesWithIngameHUD) {
		m_hidesWithIngameHUD = hidesWithIngameHUD;
	}
	
	bool canBeShownOn(typename cameraType) {
		foreach (typename t : m_targetCameras) {	
			if (cameraType.IsInherited(t)) return true;
		}
		return false;
	}
	

	
	///////////////////////////
	// WIDGET
	ImageWidget getWidget() {
		return m_widget;
	}
	
	ImageWidget buildWidget(Widget parent) {
		if (!m_widget) {
			m_widget = ImageWidget.Cast(GetGame().GetWorkspace().CreateWidgets(getLayout(), parent));
		} else {
			parent.AddChild(m_widget);
		}
		onPreBuild();
		m_widget.LoadImageFile(0, getImageName());
		m_widget.SetAlpha(getAlpha());
		m_widget.LoadMaskTexture(getMask());
		m_widget.SetMaskProgress(getMaskProgress());
		m_widget.SetMaskTransitionWidth(getMaskTransitionWidth());
		m_widget.SetPos(m_position[0], m_position[1]);
		m_widget.SetSize(m_size[0], m_size[1]);
		m_widget.SetRotation(m_rotation[0], m_rotation[1], m_rotation[2]);
		m_widget.SetSort(getPriority());
		//m_widget.SetName(getImageName());
		setVisible(true);
		onPostBuild();
		return m_widget;
	}
	
	protected string getLayout() {
		return "MyMODS/sFramework/GUI/layouts/ingame/hud/camera_overlay.layout";
	}
	
	string toString() {
		return string.Format(
			"image= %1 \n alpha= %2 \n mask= %3 \n maskProgress= %4 \n maskTransitionWidth= %5 \n position= %6 \n size= %7 \n rotation= %8 \n priority= %9",
			getImageName(),
			getAlpha(),
			getMask(),
			getMaskProgress(),
			getMaskTransitionWidth(),
			getPosition(),
			getSize(),
			getRotation(),
			getPriority()) + string.Format("targetCameras= %1 \n visible= %2 \n hidesWithIngameHUD= %3",
			getTargetCameras(),
			isVisible(),
			hidesWithIngameHUD());
	}
	
	void debugPrint(int depth = 0) {
		SLog.d(this,"",depth);
		depth++;
		SLog.d(getImageName(),"imageName",depth);
		SLog.d(getAlpha(),"alpha",depth);
		SLog.d(getMask(),"mask",depth);
		SLog.d(getMaskProgress(),"maskProgress",depth);
		SLog.d(getMaskTransitionWidth(),"maskTranstitionWidth",depth);
		SLog.d(getPosition(),"position",depth);
		SLog.d(getSize(),"size",depth);
		SLog.d(getRotation(),"rotation",depth);
		SLog.d(getPriority(),"priority",depth);
		SLog.d(getTargetCameras(),"targetCameras",depth);
		foreach (typename cam : m_targetCameras) {
			SLog.d(cam,"",depth+1);
		}
		SLog.d(isVisible(),"isVisible",depth);
		SLog.d(hidesWithIngameHUD(),"hidesWithIngameHUD",depth);
	}

}



