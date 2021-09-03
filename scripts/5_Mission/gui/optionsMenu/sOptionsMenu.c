class SOptionsMenuBase : ScriptedWidgetEventHandler {
	
	string getName();
	string getLayout();
	
	protected Widget m_root;
	void setRoot(Widget root){
		m_root = root;
		m_root.SetHandler(this);
	}
	
	void onBuild();
	
	
	
	
	
	protected void initOption(out SSliderWidget slider, string name, float value){
		slider = new SSliderWidget(m_root, name, value, this);
	}
	
	protected void initOption(out CheckBoxWidget checkbox, string name, bool value){
		checkbox = CheckBoxWidget.Cast(m_root.FindAnyWidget( name ));
		checkbox.SetChecked(value);
		checkbox.SetHandler(this);
	}
	
	
		
	protected void lockOption(SSliderWidget slider){
		lockOption(slider.getSliderWidget());
		
		TextWidget txtWidget = slider.getTextWidget();
		txtWidget.SetText("" + slider.getValue() + " ( #STR_SUDE_LAYOUT_OPTIONS_LOCKED )");
		lockOption(txtWidget);
	}
	
	protected void lockOption(Widget widget){
		widget.Enable(false);
	}
		
	
	
	
		
	override bool OnChange(Widget w, int x, int y, bool finished){
		if(!w) return false;
		
		if(w.IsInherited( SliderWidget ))   return onChange(SliderWidget.Cast(w));
		if(w.IsInherited( CheckBoxWidget )) return onChange(CheckBoxWidget.Cast(w));		

		return true;
	}
	
	void show(){
		m_root.Show(true);
	}
	
	void hide(){
		m_root.Show(false);	
	}
	
	protected bool onChange(SliderWidget w);
	protected bool onChange(CheckBoxWidget w);

}