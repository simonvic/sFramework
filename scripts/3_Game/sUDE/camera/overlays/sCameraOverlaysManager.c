enum eSCameraOverlayState {
	REQUESTED,
	ACTIVE
	PENDING_DELETION
}


//@todo Performance: Iterate overlays only once each frame
//@todo Performance: use ImageWidget pool
//@todo Performance: pre-cache images on boot

class SCameraOverlaysManager {
	
	private static ref SCameraOverlaysManager INSTANCE = new SCameraOverlaysManager();
	private void SCameraOverlaysManager(){}
	static SCameraOverlaysManager getInstance(){
		return INSTANCE;
	}
	
	protected typename m_activeCameraType = DayZPlayerCamera;
	
	protected Widget m_root;
	protected bool m_overlaysHaveChanged;
	protected bool m_ingameHUDIsVisible;
	protected ref map<ref SCameraOverlay, eSCameraOverlayState> m_overlays = new map<ref SCameraOverlay, eSCameraOverlayState>();

	
	/**
	*	@brief Request to show an overlay
	*	 @param overlay \p SCameraOverlay - overlay
	*/
	void activate(SCameraOverlay overlay){
		if(!overlay) return;
		if(GetGame().IsServer() && GetGame().IsMultiplayer()){
			SLog.w("ACTIVATING " + overlay + " ON SERVER!","SCameraOverlaysManager::activate");
			return;
		}
		
		m_overlays.Set(overlay, eSCameraOverlayState.REQUESTED);
		m_overlaysHaveChanged = true;
	}

	/**
	*	@brief Remove an overlay, decrease counter if it has been requested multiple time
	*	 @param overlay \p SCameraOverlay - overlay to remove
	*/
	void deactivate(SCameraOverlay overlay){
		if(!overlay) return;
		if(GetGame().IsServer() && GetGame().IsMultiplayer()){
			SLog.w("DEACTIVATING " + overlay + " ON SERVER!","SCameraOverlaysManager::deactivate");
			return;
		}
		
		m_overlays.Set(overlay, eSCameraOverlayState.PENDING_DELETION);
		m_overlaysHaveChanged = true;
	}
	
	/**
	*	@brief Remove all overlays
	*/	
	void deactivateAll(){
		foreach(SCameraOverlay overlay, eSCameraOverlayState state : m_overlays){
			deactivate(overlay);
		}
	}
	


	/**
	*	@brief Make visible an overlay
	*	 @param overlay \p SCameraOverlay - overlay to make visible
	*/
	void show(SCameraOverlay overlay){
		overlay.setVisible(true);
		if(overlay.IsInherited(SCameraOverlayAnimated)){
			SCameraOverlayAnimated.Cast(overlay).resume();
		}
	}
	
	/**
	*	@brief Hide an overlay
	*	 @param overlay \p SCameraOverlay - overlay to make invisible
	*/
	void hide(SCameraOverlay overlay){
		overlay.setVisible(false);
		if(overlay.IsInherited(SCameraOverlayAnimated)){
			SCameraOverlayAnimated.Cast(overlay).pause();
		}
	}
	
	/**
	*	@brief Make visible all overlays with the specified priority
	*	 @param priority \p eSCOPriority - 
	*/
	void show(eSCOPriority priority){
		foreach(SCameraOverlay overlay, eSCameraOverlayState state : m_overlays){
			if(overlay.getPriority() == priority){
				show(overlay);
			}
		}
	}
	
	/**
	*	@brief Hide all overlays with the specified priority
	*	 @param priority \p eSCOPriority - 
	*/
	void hide(eSCOPriority priority){
		foreach(SCameraOverlay overlay, eSCameraOverlayState state : m_overlays){
			if(overlay.getPriority() == priority){
				hide(overlay);
			}
		}
	}
	
	/**
	*	@brief Make visible all overlays with priority between the range (inclusive)
	*	 @param min \p eSCOPriority - 
	*	 @param max \p eSCOPriority - 
	*/
	void show(eSCOPriority min, eSCOPriority max){
		foreach(SCameraOverlay overlay, eSCameraOverlayState state : m_overlays){
			if(overlay.getPriority() >= min && overlay.getPriority() <= max){
				show(overlay);
			}
		}
	}
	
	/**
	*	@brief Hide all overlays with priority between the range (inclusive)
	*	 @param min \p eSCOPriority - 
	*	 @param max \p eSCOPriority - 
	*/
	void hide(eSCOPriority min, eSCOPriority max){
		foreach(SCameraOverlay overlay, eSCameraOverlayState state : m_overlays){
			if(overlay.getPriority() >= min && overlay.getPriority() <= max){
				hide(overlay);
			}
		}
	}
	
	/**
	*	@brief Make all overlays visible
	*/
	void showAll(){
		if(m_root) m_root.Show(true);
		
		foreach(SCameraOverlay overlay, eSCameraOverlayState state : m_overlays){
			show(overlay);
		}
		
	}
	
	/**
	*	@brief Hide all overlays
	*/
	void hideAll(){
		if(m_root) m_root.Show(false);
		
		foreach(SCameraOverlay overlay, eSCameraOverlayState state : m_overlays){
			hide(overlay);
		}
		
	}
	
	/**
	*	@brief Called when the player hides the ingame HUD
	*/
	void setIngameHUDVisibility(bool visible){
		m_ingameHUDIsVisible = visible;
		updateVisibilityAll();
	}
	
	/**
	*	@brief Set the currently active camera and update the visibility of overlays
	*	 @param cameraType \p typename - typename of a camera
	*/
	void setActiveCameraType(typename cameraType){
		if(m_activeCameraType == cameraType) return;
		m_activeCameraType = cameraType;
		updateVisibilityAll();
	}
	
	/**
	*	@brief Update visibility state of overlays based	
	*/
	void updateVisibilityAll(){
		foreach(SCameraOverlay overlay, eSCameraOverlayState state : m_overlays){
			updateVisibility(overlay);
		}	
	}
	
	/**
	*	@brief Change visibility state of an overlay based on a camera type
	*	 @param overlay \p SCameraOverlay
	*/
	void updateVisibility(SCameraOverlay overlay){
		if( (!m_ingameHUDIsVisible && overlay.hidesWithIngameHUD()) || !overlay.canBeShownOn(m_activeCameraType)){
			hide(overlay);
			return;
		}
		show(overlay);		
	}
	
	/**
	*	@brief Update requested overlays
	*	 @param root \p Widget - root widget of overlays
	*/
	void onUpdate(float deltaTime, Widget root){
		m_root = root;
		animateOverlays(deltaTime, root);

		if(!m_overlaysHaveChanged) return;
		
		updateOverlays(deltaTime, root);
		
		m_overlaysHaveChanged = false;
	}
	
	protected void animateOverlays(float deltaTime, Widget root){
		foreach(SCameraOverlay overlay, eSCameraOverlayState state : m_overlays){
			SCameraOverlayAnimated animated = SCameraOverlayAnimated.Cast(overlay);
			if(!animated) continue;
			
			if(animated.isPlaying()){
				animated.animate(deltaTime);
			
			    //if an active timed overlay has stopped and needs deactivation
			}else if(animated.hasStopped() && state == eSCameraOverlayState.ACTIVE && animated.IsInherited(SCameraOverlayTimed) && SCameraOverlayTimed.Cast(animated).shouldDeactivateOnStop()){
				deactivate(animated);
			}
		}
	}
	
	protected void updateOverlays(float deltaTime, Widget root){
		foreach(SCameraOverlay overlay, eSCameraOverlayState state : m_overlays){
			if(!overlay) SLog.w("Found null overlay!","SCameraOverlaysManager");
			
			switch(m_overlays.Get(overlay)){
				case eSCameraOverlayState.REQUESTED:
					performActivate(overlay, root);
					break;
				case eSCameraOverlayState.PENDING_DELETION:
					//@todo decrease alpha until 0 then perform deactivate
					performDeactivate(overlay, root);
					break;
			}
			
			updateVisibility(overlay);
		}
	}

	
	/**
	*	@brief Actual creation of widget
	*	 @param overlay \p SCameraOverlay - overlay to show
	*	 @param parent \p Widget - parent of widget to add
	*/
	protected void performActivate(SCameraOverlay overlay, Widget parent){
		overlay.buildWidget(parent);
		m_overlays.Set(overlay, eSCameraOverlayState.ACTIVE);
		if(overlay.IsInherited(SCameraOverlayAnimated)){
			SCameraOverlayAnimated.Cast(overlay).start();
		}
	}
	
	
	/**
	*	@brief Actual removal of widget
	*	 @param overlay \p SCameraOverlay - overlay to remove
	*	 @param root \p Widget - parent of widget to remove
	*/
	protected void performDeactivate(SCameraOverlay overlay, Widget parent){ //@todo unnecesary to pass parent
		if(overlay.getWidget()){
			parent.RemoveChild(overlay.getWidget());
		}
		m_overlays.Remove(overlay);
		if(overlay.IsInherited(SCameraOverlayAnimated)){
			SCameraOverlayAnimated.Cast(overlay).stop();
		}
	}
	
	bool overlaysHaveChanged(){
		return m_overlaysHaveChanged;
	}
	
	bool isActive(SCameraOverlay overlay){
		return m_overlays.Contains(overlay));
	}
		
	TSCameraOverlaySet getActive(){
		TSCameraOverlaySet overlays = new TSCameraOverlaySet();
		foreach(auto overlay, auto state: m_overlays){
			if(state == eSCameraOverlayState.ACTIVE){
				overlays.Insert(overlay);
			}
		}
		return overlays;
	}
	
	TSCameraOverlaySet getRequested(){
		TSCameraOverlaySet overlays = new TSCameraOverlaySet();
		foreach(auto overlay, auto state: m_overlays){
			if(state == eSCameraOverlayState.REQUESTED){
				overlays.Insert(overlay);
			}
		}
		return overlays;
	}
	
	TSCameraOverlaySet getPendingDeletion(){
		TSCameraOverlaySet overlays = new TSCameraOverlaySet();
		foreach(auto overlay, auto state: m_overlays){
			if(state == eSCameraOverlayState.PENDING_DELETION){
				overlays.Insert(overlay);
			}
		}
		return overlays;
	}
	
	set<SCameraOverlayAnimated> getAnimating(){
		set<SCameraOverlayAnimated> overlays  = new set<SCameraOverlayAnimated>();
		foreach(auto overlay, auto state : m_overlays){
			if(overlay.IsInherited(SCameraOverlayAnimated)){
				overlays.Insert(SCameraOverlayAnimated.Cast(overlay));
			}
		}
		return overlays;
	}
	
	void debugPrint(){
		SLog.d("=============== DEBUG PRINT =================");
		TSCameraOverlaySet requested = SCameraOverlaysManager.getInstance().getRequested();
		SLog.d(""+requested.Count(),"requested");
		foreach(SCameraOverlay r : requested){
			SLog.d(r,"",1);
		}
				
		TSCameraOverlaySet active = SCameraOverlaysManager.getInstance().getActive();
		SLog.d(""+active.Count(),"active");
		foreach(SCameraOverlay a : active){
			SLog.d(a,"",1);
		}
		
		TSCameraOverlaySet pending = SCameraOverlaysManager.getInstance().getPendingDeletion();
		SLog.d(""+pending.Count(),"pending deletion");
		foreach(SCameraOverlay p : pending){
			SLog.d(p,"",1);
		}
		
		set<SCameraOverlayAnimated> animating = SCameraOverlaysManager.getInstance().getAnimating();
		SLog.d(""+animating.Count(),"animating");
		foreach(SCameraOverlayAnimated ao : animating){
			SLog.d(ao,"",1);
			SLog.d(typename.EnumToString(eSAnimableState, ao.getAnimationState()),"",2);
		}
		
		SLog.d("-------------- deep debug --------------");
		foreach(SCameraOverlay co, eSCameraOverlayState state: m_overlays){
			co.debugPrint(1);
		}
		SLog.d("-------------- tree debug --------------");
		debugPrintOverlayRoot(m_root);
	}
	
	void debugPrintOverlayRoot(Widget root, int depth = 0){
		if(!root) return;
		SLog.d(""+root.GetName(),"",depth);
		
		if(root.GetChildren()){
			debugPrintOverlayRoot(root.GetChildren(), depth + 1);
		}
		
		if(root.GetSibling() && depth > 0){
			debugPrintOverlayRoot(root.GetSibling(), depth);
		}
	}
		
	static void debugPrintLayout(Widget root, int depth = 0){
		if(!root) return;
		SLog.d(""+root.GetName(),"",depth);
		
		if(root.GetChildren()){
			debugPrintLayout(root.GetChildren(), depth + 1);
		}
		
		if(root.GetSibling()){
			debugPrintLayout(root.GetSibling(), depth);
		}
	}
	

}