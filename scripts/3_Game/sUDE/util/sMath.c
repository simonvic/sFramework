class SMath {
	
	static const float e = 2.7182818284;
	
	static float Exp(float x){
		return Math.Pow(e,x);
	}
	
	/**
	* @brief Return position on the GaussBell				// β ππ^((β(π₯βπ)^2)/(2π^2 )) β
	* 	@param height \p float Height of the curve
	* 	@param center \p float Center of the curve
	* 	@param width \p float Standard deviation of the curve (Gaussian RMS width) ( > 0 )
	* 	@param x \p float Input value
	* 	@return \p float - position on curve
	*/
	static float Gauss(float height, float center, float width, float x){
		return height * Exp((- Math.Pow(x - center,2)) / 2 * Math.Pow(width,2));
	}
	
	/**
	* @brief Return position on the GaussBell (probability density function)		β 1/(π€β2π) π^((β1/2) ((π₯βπ)/π€)^2 ) β
	* 	@param center \p float Center of the curve
	* 	@param width \p float Standard deviation of the curve (Gaussian RMS width) ( > 0 )
	* 	@param x \p float Input value
	* 	@return \p float - position on curve
	*/
	static float GaussPDF(float center, float width, float x){
		return ( 1 / width * Math.Sqrt(2 * Math.PI) ) * Exp((- Math.Pow(x - center,2)) / 2 * Math.Pow(width,2));
	}
	
	/**
	* @brief Return position on a damping down sin function reaching for target.		β ππ^(βππ₯) (cos(ππ₯+π))+π‘ β
	* 	@param amplitude \p float Initial amplitude. (0: none, 0.5: smooth , 1: sharp)
	* 	@param decay \p float How the function will decay. (0: none, 0.5: smooth , 1: sharp)
	* 	@param phase \p float Phase (shifting) of function
	* 	@param frequency \p float Target of damping function
	* 	@param target \p float Target of damping function
	* 	@param x \p float Input value
	* 	@return \p float - position on curve
	*/
	static float DampedSin(float amplitude, float decay, float phase, float frequency, float target, float x){
		return amplitude * Exp(- decay * x) * (Math.Cos(frequency * x + phase)) + target;
	}
	
	/**
	* @brief Return position on a damping down sin function reaching for target.		β πarctan(π₯π+β)+π£ β
	* 	@param amplitude \p float Initial amplitude.
	* 	@param sharpness \p float Smoothness of curve, (0.1: linear, 1.5: smooth, 2.5: sharp etc)
	* 	@param verticalOffset \p float Center of function, Y intersection
	* 	@param horizontalOffset \p float Center of function, Y intersection
	* 	@param x \p float Input value
	* 	@return \p float - position on curve
	*/
	static float Arctan(float amplitude, float sharpness, float horizontalOffset, float verticalOffset, float x){
		return amplitude * Math.Atan2(x * sharpness + horizontalOffset, 1) + verticalOffset;
	}
	
	/**
	* @brief Return if two float numbers are equal
	* 	@param x \p float Frist float value
	* 	@param y \p float Second float value
	* 	@param y \p float Epsilon value (tolerance)
	* 	@return \p bool - if are equals
	*/
	static bool equal(float x, float y, float epsilon = 1e-32){
		return Math.AbsFloat(x - y) < epsilon;		
	}
	
	/**
	* @brief Return if two arrays float are equal
	* 	@param x \p float Frist float array value
	* 	@param y \p float Second float array value
	* 	@param y \p float Epsilon value (tolerance)
	* 	@return \p bool - if are equals
	*/
	static bool equal(array<float> x, array<float> y, float epsilon = 1e-32){
		if (!x || !y) return false;
		
		int xCount = x.Count();
		if (xCount != y.Count()) {
			return false;
		}
		
		for (int i = 0; i < xCount; i++) {
			if (!SMath.equal(x[i], y[i], epsilon)) {
				return false;			
			}
		}
		
		return true;
	}
	
	/**
	* @brief Map a value from a range to another
	* 	@param value \p float - Value to map
	* 	@param min \p float - Value minimum range
	* 	@param max \p float - Value maximum range
	* 	@param mappedMin \p float - Mapped value maximum range
	* 	@param mappedMax \p float - Mapped value maximum range
	* 	@return \p float - mapped value
	*/
	static float mapTo(float value, float min, float max, float mappedMin = 0, float mappedMax = 1){
		return ((value - min) / (max - min)) * (mappedMax - mappedMin) + mappedMin;
	}
	
	/**
	*	@brief Truncate the less significant digits of a float
	*	 @param value \p float - value
	*	 @param digits \p int - number of desired deciaml digits (min 1, max 6)
	*	 @return float - value with reduced precision
	*/
	static float reducePrecision(float value, int digits){
		digits = Math.Clamp(digits, 1, 6);
		digits = Math.Pow(10, digits);
		return (float)((int) (value * digits)) / digits;
	}
	
	
	/**
	*	@brief Clamp a float array given two arrays containing the minimum and maximum values.
	*	       The min and max array MUST have an equal or greater count than the array to clamp, unclamped array will be returned otherwise
	*	 @param toClamp \p array<float> - array to clamp
	*	 @param min \p array<float> - array of minimums
	*	 @param max \p array<float> - array of maximums
	*/
	static void clamp(array<float> toClamp, array<float> min, array<float> max) {
		if (!toClamp || !min || !max) return;
		
		int toClampCount = toClamp.Count();
		if (min.Count() < toClampCount || max.Count() < toClampCount) return;
		
		for (int i = 0; i < toClampCount; i++) {
			toClamp[i] = (Math.Clamp(toClamp[i], min[i], max[i]));
		}
	}

	/**
	*	@brief Check if every float array is in range of the given minium and maximum
	*	       The min and max array MUST have an equal or greater count than the array to verify, false will be returned otherwise
	*	 @param values \p array<float> - array to verify
	*	 @param min \p array<float> - array of minimums
	*	 @param max \p array<float> - array of maximums
	*	 @return bool - if all values are in range
	*/
	static bool isInRange(array<float> values, array<float> min, array<float> max) {
		if (!values || !min || !max) return false;
		
		int valuesCount = values.Count();
		if (min.Count() < valuesCount || max.Count() < valuesCount) return false;
		
		for (int i = 0; i < valuesCount; i++) {
			if (!SMath.isInRange(values[i], min[i], max[i])) return false;
		}
		
		return true;
	}
	
	/**
	*	@brief Check if a float value is in range of the given minium and maximum [ INCLUSIVE ]
	*	 @param value \p float - value to verify
	*	 @param min \p float - minimum
	*	 @param max \p float - maximum
	*	 @return bool - if value is in range
	*/
	static bool isInRange(float value, float min, float max) {
		return value >= min && value <= max;
	}
	
	
}