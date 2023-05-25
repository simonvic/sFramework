//if we had interfaces...
class SCameraOverlayAnimated : SCameraOverlay {
	
	static SCameraOverlayAnimatedBuilder builderAnimated() {
		return new SCameraOverlayAnimatedBuilder();
	}
	
	protected eSAnimableState m_animationState = eSAnimableState.STOPPED;
	protected float m_time;
	
	void onStart();
	void onAnimate(float deltaTime);
	void onStop();
	void onPause();
	void onResume();
	
	/**
	*	@brief Set the animation state to PLAYING and reset the time
	*/
	void start() {
		m_time = 0;
		setAnimationState(eSAnimableState.PLAYING);
		onStart();
	}
	
	/**
	*	@brief Called on each frame
	*/
	void animate(float deltaTime) {
		if (!isPlaying()) return;
		
		m_time += deltaTime;
		onAnimate(deltaTime);
	}
	
	/**
	*	@brief Reset values to default and prepare to stop
	*/
	void stop() {
		setAnimationState(eSAnimableState.STOPPED);
		onStop();
	}
	
	/**
	*	@brief Pause the animation from animating without resetting the time
	*/
	void pause() {
		setAnimationState(eSAnimableState.PAUSED);
		onPause();
	}
	
	/**
	*	@brief Resume the animation
	*/
	void resume() {
		setAnimationState(eSAnimableState.PLAYING);
		onResume();
	}
	
	/**
	*	@brief Get the time elapsed from the animation start
	* 	@return \p float - time elapsed
	*/
	float getTime() {
		return m_time;
	}
	
	bool isPlaying() {
		return m_animationState == eSAnimableState.PLAYING;
	}
	
	bool isPaused() {
		return m_animationState == eSAnimableState.PAUSED;
	}
	
	bool hasStopped() {
		return m_animationState == eSAnimableState.STOPPED;
	}
	
	eSAnimableState getAnimationState() {
		return m_animationState;
	}
	
	protected void setAnimationState(eSAnimableState state) {
		m_animationState = state;
	}
}

class SCameraOverlayAnimatedBuilder : SCameraOverlayBuilder {
	
	SCameraOverlayAnimated buildAnimated() {
		SCameraOverlayAnimated o = new SCameraOverlayAnimated();
		o.setImage(m_image);
		o.setAlpha(m_alpha);
		o.setMask(m_mask);
		o.setMaskProgress(m_maskProgress);
		o.setMaskTransitionWidth(m_maskTransitionWidth);
		o.setPosition(m_position);
		o.setSize(m_size);
		o.setRotation(m_rotation);
		o.setPriority(m_priority);
		array<typename> temp = {};
		temp.Copy(m_targetCameras);
		o.setTargetCameras(temp);
		o.setHidesWithIngameHUD(m_hidesWithIngameHUD);
		return o;
	}
	
}