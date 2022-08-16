class SPPEffectTimed : SPPEffectAnimated {
	protected float m_duration;
	protected bool m_deactivationOnStop;
	
	void SPPEffectTimed(float duration, bool deactivationOnStop = true) {
		m_duration = duration;
		m_deactivationOnStop = deactivationOnStop;
	}

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
	* @brief Set the duration of the animation
	* 	@param duration \p float - animation duration (seconds)
	*/
	void setDuration(float duration) {
		m_duration = duration;
	}
		
	/**
	* @brief Get the animation duration
	* 	@return \p float - animation duration(seconds)
	*/
	float getDuration() {
		return m_duration;
	}
		
	/**
	* @brief Get the time remaining to the completition
	* 	@return \p float - time remaining (seconds)
	*/
	float getRemaining() {
		return m_duration - m_time;
	}
	
	/**
	* @brief Get if the animation should be deactivated when ends
	*
	*/
	bool shouldDeactivateOnStop() {
		return m_deactivationOnStop;
	}
	
}