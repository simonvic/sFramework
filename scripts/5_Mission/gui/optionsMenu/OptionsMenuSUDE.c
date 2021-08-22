//@todo refactor this
//forgive me lol, I have to rush
class OptionsMenuSUDE : ScriptedWidgetEventHandler{
	
	protected Widget m_root;
	protected ref array<ref SOptionsMenuBase> m_menus = new array<ref SOptionsMenuBase>;
	
	
	protected string getLayout(){
		return "MyMODS/sFramework/GUI/layouts/sTabPane.layout";
	}
	
	void OptionsMenuSUDE(Widget parent){
		m_root = GetGame().GetWorkspace().CreateWidgets( getLayout(), parent );
		initMenus();
	}
	
	void initMenus(){}
	
	protected Widget getMenusRoot(){
		return m_root.FindWidget("body");
	}
	
	protected void addMenu(SOptionsMenuBase menu){
		ButtonWidget btn = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidgets("MyMODS/sFramework/GUI/layouts/tab_head_button.layout",m_root.FindAnyWidget("header")) );
		TextWidget.Cast(btn.FindAnyWidget("txt")).SetText(menu.getName());
		btn.SetName(menu.getName());
		btn.SetHandler(this);
		
		menu.setRoot( GetGame().GetWorkspace().CreateWidgets(menu.getLayout(), getMenusRoot()) );
		menu.onBuild();
		if(m_menus.Count() == 0){
			TextWidget.Cast(btn.FindAnyWidget("txt")).SetColor(ARGB(255, 240, 84, 76));
		}else{
			menu.hide();
		}
		m_menus.Insert(menu);
	}
	
	override bool OnClick(Widget w, int x, int y, int button){
		//Show only selected tabs
		foreach(SOptionsMenuBase menu : m_menus){
			if(w.GetName() == menu.getName()){
				menu.show();
			}else{
				menu.hide();
			}
		}
		
		// Reset to white all other buttons
		ButtonWidget btn = ButtonWidget.Cast(w.GetParent().GetChildren());
		while(btn){
			TextWidget.Cast(btn.FindAnyWidget("txt")).SetColor(ARGB(255, 255, 255, 255));
			btn = ButtonWidget.Cast(btn.GetSibling());
		}
		
		// Set to red selected button
		TextWidget.Cast(w.FindAnyWidget("txt")).SetColor(ARGB(255, 240, 84, 76));
		return true;
	}
	

	override bool OnMouseEnter(Widget w, int x, int y){
		if(w.IsInherited(ButtonWidget)){
			w.SetColor(ARGB(200, 0, 0, 0));
		}
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y){
		if(w.IsInherited(ButtonWidget)){
			w.SetColor(ARGB(140, 0, 0, 0));
		}
		return true;
	}
	
}