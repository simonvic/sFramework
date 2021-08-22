class SSliderWidget : Managed {
	protected ref SliderWidget m_slider;
	protected ref TextWidget m_text;
	
	void SSliderWidget(Widget root, string name, float value, ScriptedWidgetEventHandler handler){
		m_slider = SliderWidget.Cast(root.FindAnyWidget( name+"option" ));
		m_text = TextWidget.Cast(root.FindAnyWidget( name+"value" ));
		setValue(value);
		m_slider.SetHandler(handler);
		m_text.SetHandler(handler);
	}
	
	void setValue(float value){
		m_slider.SetCurrent(value);
		m_text.SetText(value.ToString());
	}
	
	float getValue(){
		return m_slider.GetCurrent();
	}
	
	void updateTextValue(){
		m_text.SetText(m_slider.GetCurrent().ToString());
	}
	
	SliderWidget getSliderWidget(){
		return m_slider;
	}
	
	TextWidget getTextWidget(){
		return m_text;
	}
}