class SCameraOverlayTimed : SCameraOverlayAnimated {
	
	static SCameraOverlayTimedBuilder builderTimed() {
		return new SCameraOverlayTimedBuilder();
	}
	
	protected float m_duration;
	protected bool m_deactivateOnStop;
	
	override void animate(float deltaTime) {
		if ( !isPlaying() ) {
			return; 
		}
		
		if (getTime() >= m_duration ) {
			stop();
			return;
		}
		super.animate(deltaTime);
	}
	
	/**
	*	@brief Set the duration of the animation
	*	@param duration \p float - animation duration (seconds)
	*/
	void setDuration(float duration) {
		m_duration = duration;
	}
		
	/**
	*	@brief Get the animation duration
	*	@return \p float - animation duration(seconds)
	*/
	float getDuration() {
		return m_duration;
	}
		
	/**
	*	@brief Get the time remaining to the completition
	*	@return \p float - time remaining (seconds)
	*/
	float getRemaining() {
		return m_duration - m_time;
	}
	
	/**
	*	@brief Get if the animation should be deactivated when ends
	*/
	bool shouldDeactivateOnStop() {
		return m_deactivateOnStop;
	}
	
	/**
	*	@brief Set if the overlay has to be deactivated on has stopped
	*	@param deactivateOnStop \p bool 
	*/
	void setDeactivateOnStop(bool deactivateOnStop) {
		m_deactivateOnStop = deactivateOnStop;
	}
	
}

class SCameraOverlayTimedBuilder : SCameraOverlayAnimatedBuilder {
	
	protected float m_duration = Math.PI;
	protected bool m_deactivateOnStop = true;
	
	SCameraOverlayTimedBuilder duration(float duration) {
		m_duration = duration;
		return this;
	}
	
	SCameraOverlayTimedBuilder deativateOnStop(bool deactivateOnStop) {
		m_deactivateOnStop = deactivateOnStop;
		return this;
	}
	
	SCameraOverlayTimed buildTimed() {
		SCameraOverlayTimed o = new SCameraOverlayTimed();
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

		o.setDuration(m_duration);
		o.setDeactivateOnStop(m_deactivateOnStop);
		return o;
	}

}
	

