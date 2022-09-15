#ifdef DIAG_DEVELOPER
/**

	@brief Create a simple interface for debugging purposes
	It can also be used in a "per-frame context"

	@code
		// Example usage for per-frame update
		void OnUpdate(float timeslice) {
			auto dui = SDebugUI.of("TestDebugUI");
			dui.begin();
			dui.window("Debug monitor");
				dui.text("Day Time : " + GetGame().GetDayTime());
				dui.newline();
				
				dui.textrich("<image set='dayz_gui' name='icon_pin' /> ");
				dui.textrich("You can click on the slider, or you can use the mouse wheel");
				dui.textrich("If you hold shift while using mouse wheel, it will go wrooom!");
				float sliderValue;
				dui.slider("mySlider", sliderValue);
				dui.textrich("The value of <font name='gui/fonts/amorserifpro'>sliderValue</font> is: <b>"+ sliderValue +"</b>");
				
				bool checkValue
				dui.check("myCheck", checkValue);
				dui.text("CheckValue: " + checkValue);
				dui.button("click me", this, "printSum", new Param2<int,int>(69, 420));
				dui.newline();
				dui.table({
					{"Attribute",    "Value"},
					{"Time",         ""+GetGame().GetTickTime()},
					{"Radio volume", ""+GetGame().GetSoundScene().GetRadioVolume()},
					{"VoIP volume",  ""+GetGame().GetSoundScene().GetVOIPVolume()},
					{"VoIP level",   ""+GetGame().GetSoundScene().GetAudioLevel()}
				});
				dui.plotlive("Sin", Easing.EaseInBounce(Math.AbsFloat(Math.Sin(m_time))));
			dui.end();
		}

		void printSum(int x, int y) {
			Print(x + y);
		}
		
*/
// @todo add a default static SDebugUI and window instance for quick debug
class SDebugUI : ScriptedWidgetEventHandler {
	
	static const ref array<int>   DEFAULT_WINDOW_SIZE  = {512 + 24, 512};
	static const ref array<int>   DEFAULT_WINDOW_POS   = {128, 128};
	static const ref array<int>   DEFAULT_WIDGET_SIZE  = {256, 256};
	static const ref array<float> DEFAULT_PLOT_SCALE   = {1.0, 1.0};
	static const ref array<float> DEFAULT_PLOT_OFFSET  = {0.0, 0.0};
	static const ref array<float> DEFAULT_SPACER_SIZE  = {0.3, 0.3};
	static const ref SColor       DEFAULT_PLOT_COLOR   = SColor.rgba(0xF0544Cff);
	
	static const int PLOT_HISTORY_MIN = 5;
	static const int PLOT_HISTORY_MAX = 1000;
	static const int DEFAULT_PLOT_HISTORY = 50;
	
	
	static ref map<string, ref SDebugUI> instances = new map<string, ref SDebugUI>;
	
	/**
	*	@brief Get an instance of SDebugUI
	*	@param name of instance
	*	@return instance of name, or new one if it doesn't exists
	*/
	static SDebugUI of(string name) {				
		SDebugUI dui = instances.Get(name);
		if (dui) return dui;
		dui = new SDebugUI();
		instances.Set(name, dui);
		return dui;
	}

	
	Widget root;
	
	/**
	*	Stack (LIFO) of the instantiated windows.
	*	The first element is the last created window
	*/
	ref SStack<Widget> windows;
	
	/**
	*	Disable the debug ui
	*/
	bool disabled;
	
	ref map<ButtonWidget, ref SDebugButtonCallback> buttonsCallbacks;
	
	ref map<string, bool> statesCheckbox;
	ref map<string, float> statesSlider;
	ref map<string, ref array<ref array<float>>> plotsHistory;

	ref SColor color;
	/**
	* <value, isPixel>
	*/
	ref Param2<float, bool> size[2];
	ref Param2<float, bool> pos[2];

	private void SDebugUI() {
		if (isServer()) return;
		root = GetGame().GetWorkspace().CreateWidgets("MyMODS/sFramework/GUI/layouts/debug/root.layout");
		root.SetHandler(this);
		root.SetSort(999);
		windows = new SStack<Widget>();
		buttonsCallbacks = new map<ButtonWidget, ref SDebugButtonCallback>;
		statesCheckbox = new map<string, bool>;
		statesSlider = new map<string, float>;
		plotsHistory = new map<string, ref array<ref array<float>>>;
	}

	/**
	*	@brief Specify widget color
	*	@param color
	*	@code
	*	dui.color(SColor.rgba(0xFF0000aa)).window() // semi transparent red window
	*/
	SDebugUI color(SColor col) {
		color = col;
		return this;
	}
	
	/**
	*	@brief Specify widget size in pixel or percentage
	*	@param size with unit suffix
	*	@code
	*	dui.size("69px 420px").window(); //69px width and 420px height
	*	dui.size("0.5 0.42").window();   //69% width and 42% height
	*	dui.size("69px 0.42").window();  //69px width and 42% height
	*	dui.size("69px").window();       //69px width and height
	*	dui.size("69").window();         //69% width and height
	*/
	SDebugUI size(string words) {
		array<string> temp = {};
		words.Split(" ", temp);
		if (!temp || temp.Count() < 1) return this;
		
		Param2<float, bool> x = parseSizePos(temp[0]);
		size[0] = x;
		if (temp.Count() < 2) {
			size[1] = x;
		} else {
			size[1] = parseSizePos(temp[1]);
		}
		
		return this;
	}
	
	/**
	*	@brief Specify widget position in pixel or percentage
	*	@param position with unit suffix
	*	@code
	*	dui.pos("69px 420px").window(); //69px x and 420px y
	*	dui.pos("0.5 0.42").window();   //69% x and 42% y
	*	dui.pos("69px 0.42").window();  //69px x and 42% y
	*	dui.pos("69px").window();       //69px x and y
	*	dui.pos("69").window();         //69% x and y
	*/
	SDebugUI pos(string words) {
		array<string> temp = {};
		words.Split(" ", temp);
		if (!temp || temp.Count() < 1) return this;
		
		
		Param2<float, bool> x = parseSizePos(temp[0]);
		pos[0] = x;
		if (temp.Count() < 2) {
			pos[1] = x;
		} else {
			pos[1] = parseSizePos(temp[1]);
		}
		
		return this;
	}
	
	/**
	*	@brief This little boy is rudimentary. He's trying his hardest :)
	*/
	protected static Param2<float, bool> parseSizePos(string word) {
		bool exact = word.Contains("px");
		word.Replace("px", "");
		return new Param2<float, bool>(word.ToFloat(), exact);
	}


	/**
	*	@brief Recolor the widget and consume the color
	*	@param widget to color
	*/
	protected void recolor(Widget w) {
		if (!color) return;
		w.SetColor(color.getARGB());
		color = null;
	}
	
	/**
	*	@brief Resize the widget and consume the size
	*	@param widget to resize
	*/
	protected void resize(Widget w) {
		if (size[0] && size[1]) {
		
			if (size[0].param2) {
				w.SetFlags(WidgetFlags.HEXACTSIZE);
			} else {
				w.ClearFlags(WidgetFlags.HEXACTSIZE);
			}
			
			if (size[1].param2) {
				w.SetFlags(WidgetFlags.VEXACTSIZE);
			} else {
				w.ClearFlags(WidgetFlags.VEXACTSIZE);
			}
			
			w.SetSize(size[0].param1, size[1].param1);
			
		}
		size[0] = null;
		size[1] = null;
	}
	
	/**
	*	@brief Reposition the widget and consume the position
	*	@param widget to reposition
	*/
	protected void reposition(Widget w) {
		if (pos[0] && pos[1]) {
			
			if (pos[0].param2) {
				w.SetFlags(WidgetFlags.HEXACTPOS);
			} else {
				w.ClearFlags(WidgetFlags.HEXACTPOS);
			}
			
			if (pos[1].param2) {
				w.SetFlags(WidgetFlags.VEXACTPOS);
			} else {
				w.ClearFlags(WidgetFlags.VEXACTPOS);
			}
			
			w.SetPos(pos[0].param1, pos[1].param1);
		}
		pos[0] = null;
		pos[1] = null;
	}

	
	
	
	/**
	*	@brief Build a new window
	*	@param title - title of the window
	*	@param sizePx - X and Y size defined in pixels
	*	@param posPx - X and Y position defined in pixels
	*	@return Widget - new window created
	*/
	Widget window(string title = "") {
		if (isServer()) return null;

		Widget w = GetGame().GetWorkspace().CreateWidgets("MyMODS/sFramework/GUI/layouts/debug/window.layout", root);
		w.SetHandler(this);
		resize(w);
		reposition(w);
		recolor(w);
		
		TextWidget.Cast(w.FindAnyWidget("title")).SetText(title);
		windows.push(w);
		
		CheckBoxWidget.Cast(w.FindAnyWidget("disable")).SetChecked(!disabled);
		return w;
	}
	
	
	/**
	*	@brief Create a checkbox
	*	@param name - name of the widget
	*	@param variable - variable to link
	*	@return CheckBoxWidget
	*/
	CheckBoxWidget check(string name, out bool variable) {
		if (isServer() || disabled) return null;
		CheckBoxWidget w = CheckBoxWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/checkbox.layout"));
		if (!w) return null;
		w.SetName(name);
		w.SetText(name);
		if (statesCheckbox.Contains(name)) {
			bool checked = statesCheckbox.Get(name);
			w.SetChecked(checked);
			variable = checked;
		} else {
			w.SetChecked(variable);
			statesCheckbox.Set(name, variable);
		}
		return w;
	}

	/**
	*	@brief Create a button
	*	@param text - text to show next i nthe button
	*	@return ButtonWidget
	*/
	ButtonWidget button(string text, Class instance, string function, Param params = null) {
		if (isServer() || disabled) return null;
		ButtonWidget w  = ButtonWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/button.layout"));
		if (!w) return null;
		w.SetText(text);
		w.SetName(text);
		buttonsCallbacks.Set(w, new SDebugButtonCallback(instance, function, params));
		
		return w;
	}
	
	/**
	*	@brief Create a text widget
	*	@param text -
	*	@return TextWidget
	*/
	TextWidget text(string text) {
		if (isServer() || disabled) return null;
		TextWidget w  = TextWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/text.layout"));
		if (!w) return null;
		w.SetText(text);
		return w;
	}
	
	/**
	*	@brief Create a rich text widget
	*	@param text -
	*	@return RichTextWidget
	*/
	RichTextWidget textrich(string text) {
		if (isServer() || disabled) return null;
		RichTextWidget w  = RichTextWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/textrich.layout"));
		if (!w) return null;
		w.SetText(text);
		return w;
	}
	
	/**
	*	@brief Create a slider
	*	@param name - name of the slider
	*	@param float - value to link to the slider
	*	@param float - step value of the slider
	*	@param float - max value of the slider
	*	@param float - min value of the slider
	*	@return SliderWidget
	*/
	SliderWidget slider(string name, out float value, float step = 0.1, float min = 0, float max = 1) {
		if (isServer() || disabled) return null;
		SliderWidget w = SliderWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/slider.layout"));
		if (!w) return null;
		w.SetName(name);
		w.SetStep(step);
		w.SetMinMax(min, max);
		if (statesSlider.Contains(name)) {
			value = statesSlider.Get(name);
		}
		w.SetCurrent(value);
		TextWidget.Cast(w.FindAnyWidget("name")).SetText(name);
		TextWidget.Cast(w.FindAnyWidget("value")).SetText(""+value);
		return w;
	}
	
	/**
	*	@brief Create a table which contains text.
	*	@param sizePx - X and Y size defined in pixel
	*	@param data - matrix of rows and columns of string data
	*	@return WrapSpacerWidget
	*/
	WrapSpacerWidget table(array<ref array<string>> data) {
		if (isServer() || disabled) return null;
		WrapSpacerWidget w = WrapSpacerWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/table.layout"));
		if (!w) return null;

		if (data == null || data.Count() == 0) return w;
		
		float height = 1 / data.Count();
		foreach (auto row : data) {
			if (row == null || row.Count() == 0) continue;
			float width = 1 / row.Count();
			foreach (auto entry : row) {
				TextWidget t = TextWidget.Cast(GetGame().GetWorkspace().CreateWidget(TextWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, 0xffffffff, 0, w));
				t.ClearFlags(WidgetFlags.EXACTSIZE);
				t.SetFlags(WidgetFlags.CENTER | WidgetFlags.VCENTER | WidgetFlags.IGNOREPOINTER);
				t.SetSize(width, height);
				t.SetText(entry);
				
				//Print(SFlagOperator.from(t.GetFlags()).collectBinaryString());
			}
		}
		return w;
	}
	
	/**
	*	@brief Plot live data
	*	@param title - title of the plot
	*	@param y - y axis data
	*	@param min - min y
	*	@param max - max y
	*	@param sizePx - X and Y size of the plot defined in pixel
	*	@param historySize - how many points to store and plot
	*	@param scale - X and Y scale of the plot data
	*	@param offset - X and Y offset of the plot data
	*	@param widthPx - width of the lines defined in pixels
	*	@param color - color of the line
	*	@return CanvasWidget
	*/
	CanvasWidget plotlive(string title, float y, float min = 0, float max = 1, array<int> sizePx = null, int historySize = 50, array<float> scale = null, array<float> offset = null, int widthPx = 3, SColor lineColor = null) {
		if (isServer() || disabled) return null;
		if (!sizePx || sizePx.Count() < 2) sizePx = DEFAULT_WIDGET_SIZE;
		if (!scale  || scale.Count()  < 2) scale  = DEFAULT_PLOT_SCALE;
		if (!offset || offset.Count() < 2) offset = DEFAULT_PLOT_OFFSET;
		if (!lineColor) lineColor = DEFAULT_PLOT_COLOR;
		historySize = Math.Clamp(historySize, PLOT_HISTORY_MIN, PLOT_HISTORY_MAX);
		CanvasWidget c = canvas(sizePx, title);
		if (!c) return null;
		if (min != 0 || max != 1) {
			TextWidget.Cast(c.FindAnyWidget("min")).SetText(""+min);
			TextWidget.Cast(c.FindAnyWidget("max")).SetText(""+max);
			TextWidget.Cast(c.FindAnyWidget("current")).SetText(""+y);
		}
		
		auto line = plotsHistory.Get(title);
		if (!line) {
			line = {{0.5,0.5}, {0,y}};
			plotsHistory.Set(title, line);
		} else {
			line.InsertAt({0,y}, 0);
		}
		line.Remove(line.Count() - 1);
		line.Resize(historySize);

		
		float precision = 1 / historySize;
		float x;
		for (int i = line.Count() - 1; i > 0; i--) {
			if (line[i] == null) continue;
			c.DrawLine(
				SMath.mapTo(offset[0] + scale[0] * x,              0,   1,   0,         sizePx[0]),
				SMath.mapTo(offset[1] + scale[1] * line[i][1],     min, max, sizePx[1], 0),
				SMath.mapTo(offset[0] + scale[0] * x + precision,  0,   1,   0,         sizePx[0]),
				SMath.mapTo(offset[1] + scale[1] * line[i - 1][1], min, max, sizePx[1], 0),
				widthPx,
				lineColor.getARGB());
			x += precision;
		}
		return c;
	}
	
	/**
	*	@brief Create a plot to draw lines
	*	@param lines - list of lines (which is a list of point (which is a couple of X and Y coordinates))
	*	@param sizePx - X and Y size defined in pixel
	*	@param title - title of the plot
	*	@param scale - X and Y scale of the plot data
	*	@param offset - X and Y offset of the plot data
	*	@param widthPx - width of the lines defined in pixels
	*	@param color - color of the line
	*	@return CanvasWidget
	*/
	CanvasWidget plot(string title = "", array<ref array<ref array<float>>> lines = null, array<int> sizePx = null, array<float> scale = null, array<float> offset = null, int widthPx = 5, SColor lineColor = null) {
		if (isServer() || disabled) return null;
		if (!sizePx || sizePx.Count() < 2) sizePx = DEFAULT_WIDGET_SIZE;
		if (!scale  || scale.Count()  < 2) scale  = DEFAULT_PLOT_SCALE;
		if (!offset || offset.Count() < 2) offset = DEFAULT_PLOT_OFFSET;
		if (!lineColor) lineColor = DEFAULT_PLOT_COLOR;
		CanvasWidget c = canvas(sizePx, title);
		if (!c) return null;
		
		foreach (auto line : lines) {	
			for (int i = 0; i < line.Count() - 1; i++) {
				c.DrawLine(
					SMath.mapTo(offset[0] + scale[0] * line[i][0],   0, 1, 0,         sizePx[0]),
					SMath.mapTo(offset[1] + scale[1] * line[i][1],   0, 1, sizePx[1], 0),
					SMath.mapTo(offset[0] + scale[0] * line[i+1][0], 0, 1, 0,         sizePx[0]),
					SMath.mapTo(offset[1] + scale[1] * line[i+1][1], 0, 1, sizePx[1], 0),
					widthPx,
					lineColor.getARGB());
			}
		}
		return c;
	}
	
	/**
	*	@brief Create a plot to draw lines
	*	@param sizePx - X and Y size defined in pixel
	*	@param title - title of the plot
	*	@return CanvasWidget
	*/
	CanvasWidget canvas(array<int> sizePx = null, string title = "") {
		if (isServer() || disabled) return null;
		if (!sizePx || sizePx.Count() < 2) sizePx = DEFAULT_WIDGET_SIZE;
		Widget r = widget("MyMODS/sFramework/GUI/layouts/debug/canvas.layout");
		if (!r) return null;
		CanvasWidget c = CanvasWidget.Cast(r.FindAnyWidget("canvas"));
		c.SetSize(sizePx[0], sizePx[1]);
		TextWidget t = TextWidget.Cast(r.FindAnyWidget("title"));
		t.SetText(title);
		return c;
	}
	
	/**
	*	@brief Create a spacer
	*	@param size - X and Y size defined in screen space (0.0 - 1.0)
	*	@return Widget
	*/
	Widget spacer() {
		if (isServer() || disabled) return null;
		return widget("MyMODS/sFramework/GUI/layouts/debug/spacer.layout");
	}
	
	
	/**
	*	@brief Create a new line
	*	@param sizePx - height in pixels of the empty line
	*	@return Widget
	*/
	Widget newline(string height = "1px") {
		if (isServer() || disabled) return null;
		return size("1 " + height).spacer();
	}
	
	/**
	*	@brief Create a widget
	*	@param layout - .layout file to load
	*	@param name - name of the root widget
	*	@return Widget
	*/
	Widget widget(string layout, string name = string.Empty) {
		if (isServer() || disabled) return null;
		Widget window = windows.peek();
		if (!window) {
			SLog.e("No window to place widget on! ("+name+")",""+this);
			return null;
		}
		Widget w = GetGame().GetWorkspace().CreateWidgets(layout, window.FindAnyWidget("body"));
		if (name != string.Empty) {
			w.SetName(name);
		}
		w.SetHandler(this);
		resize(w);
		recolor(w);
		return w;
	}
	
	
	void begin() {
		if (isServer()) return;
		foreach (auto button, auto callback : buttonsCallbacks) {
			if (button.GetState()) {
				GetGame().GameScript.CallFunctionParams(callback.instance, callback.function, null, callback.params);
			}
		}
		clear();
	}
	
	void end() {
	}
	
	void show() {
		root.Show(true);
	}
	
	void hide() {
		root.Show(false);
	}
	
	/**
	*	@brief Clear the widgets
	*/
	void clear() {
		buttonsCallbacks.Clear();
		Widget w = windows.pop();
		while (w != null) {
			w.Unlink();
			w = windows.pop();
		}
	}
	
	
	
	override bool OnClick(Widget w, int x, int y, int button) {
		//                    reeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
		Widget rootWindow = w.GetParent().GetParent().GetParent().GetParent();
		switch (w.Type()) {
			case ButtonWidget:
			ButtonWidget btn = ButtonWidget.Cast(w);
			switch (w.GetName()) {
				case "collapse":
				Widget body = rootWindow.FindAnyWidget("body");
				if (body.IsVisible()) {
					btn.SetText("+");
				} else {
					btn.SetText("-");
				}
				body.Show(!body.IsVisible());
				break;
				
				case "maximize":
				break;
				
				case "close":
				break;
			}
			break;
			
			case CheckBoxWidget:
			CheckBoxWidget chk = CheckBoxWidget.Cast(w);
			string name = chk.GetName();
			switch (name) {
				case "disable":
				rootWindow.FindAnyWidget("body").Show(disabled);
				disabled = !disabled;
				break;
				
				default:
				if (statesCheckbox.Contains(name)) {
					statesCheckbox.Set(name, !statesCheckbox.Get(name));
				}
			}
			break;
			
		}
				
		return true;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y);
	override bool OnModalResult(Widget w, int x, int y, int code, int result);
	override bool OnDoubleClick(Widget w, int x, int y, int button);
	override bool OnSelect(Widget w, int x, int y);
	override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn);
	override bool OnFocus(Widget w, int x, int y);
	override bool OnFocusLost(Widget w, int x, int y);
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y);
	override bool OnMouseWheel(Widget w, int x, int y, int wheel) {
		switch (w.Type()) {
			//@todo make atomic method
			case SliderWidget:
			auto slider = SliderWidget.Cast(w);
			if (KeyState(KeyCode.KC_LSHIFT)) wheel *= 10;
			slider.SetCurrent(slider.GetCurrent() + (wheel * slider.GetStep()));
			statesSlider.Set(slider.GetName(), slider.GetCurrent());
			break;
		}
		return true;
	}
	override bool OnMouseButtonDown(Widget w, int x, int y, int button);
	override bool OnMouseButtonUp(Widget w, int x, int y, int button);
	override bool OnController(Widget w, int control, int value);
	override bool OnKeyDown(Widget w, int x, int y, int key);
	override bool OnKeyUp(Widget w, int x, int y, int key);
	override bool OnKeyPress(Widget w, int x, int y, int key);
	override bool OnChange(Widget w, int x, int y, bool finished) {
		switch (w.Type()) {
			case SliderWidget:
			SliderWidget slider = SliderWidget.Cast(w);
			statesSlider.Set(slider.GetName(), slider.GetCurrent());
			break;
		}
		return true;
	}
	
	
	vector dragOffset;
	override bool OnDrag(Widget w, int x, int y) {
		float wx,wy;
		w.GetParent().GetPos(wx,wy);
		dragOffset[0] = x - wx;
		dragOffset[1] = y - wy;
		return true;
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever) {
		w.GetParent().SetFlags(WidgetFlags.EXACTPOS);
		w.GetParent().SetPos(x - dragOffset[0], y - dragOffset[1]);
		return true;
	}
	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever);
	override bool OnDrop(Widget w, int x, int y, Widget reciever);	
	override bool OnDropReceived(Widget w, int x, int y, Widget reciever);
	override bool OnResize(Widget w, int x, int y);
	override bool OnChildAdd(Widget w, Widget child);
	override bool OnChildRemove(Widget w, Widget child);
	override bool OnUpdate(Widget w);
	override bool OnEvent(EventType eventType, Widget target, int parameter0, int parameter1);
	
	
	protected bool isServer() {
		return GetGame().IsDedicatedServer();
	}
	
}


class SDebugButtonCallback : Managed {
	Class instance;
	string function;
	ref Param params;
	
	void SDebugButtonCallback(Class i, string f, Param p) {
		instance = i;
		function = f;
		params = p;
	}
}

#endif