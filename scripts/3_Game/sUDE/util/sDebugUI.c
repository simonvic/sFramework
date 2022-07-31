/**

	@brief Create a simple interface for debugging purposes
	It can also be used in a "per-frame context"

	@code
		// Example usage for per-frame update
		ref SDebugUI dui = new SDebugUI();

		void OnUpdate(float timeslice) {
			dui.begin();
			dui.window("Debug monitor");
			dui.text("Time : " + GetGame().GetDayTime());
			dui.newline();
			dui.slider(0.5, 0.1, 0, 1);
			dui.check("some text");
			dui.button("click me", this, "printSum", new Param2<int,int>(69, 420));
			dui.newline();
			dui.table({
				{"Attribute",    "Value"},
				{"Time",         ""+m_time},
				{"Radio volume", ""+GetGame().GetSoundScene().GetRadioVolume()},
				{"VoIP volume",  ""+GetGame().GetSoundScene().GetVOIPVolume()},
				{"VoIP level",   ""+GetGame().GetSoundScene().GetAudioLevel()}
			});
			dui.plotlive("Sin", Math.AbsFloat(Math.Sin(GetGame().GetTickTime())));
			dui.end();
		}

		void printSum(int x, int y) {
			Print(x + y);
		}
		
*/
class SDebugUI : ScriptedWidgetEventHandler {
	
	static const ref array<int>   DEFAULT_WINDOW_SIZE  = {512, 512};
	static const ref array<int>   DEFAULT_WINDOW_POS   = {128, 128};
	static const ref array<int>   DEFAULT_WIDGET_SIZE  = {256, 256};
	static const ref array<float> DEFAULT_PLOT_SCALE   = {1.0, 1.0};
	static const ref array<float> DEFAULT_PLOT_OFFSET  = {0.0, 0.0};
	static const ref array<float> DEFAULT_SPACER_SIZE  = {0.3, 0.3};
	static const ref SColor       DEFAULT_PLOT_COLOR   = SColor.rgba(0xF0544Cff);
	
	static const int PLOT_HISTORY_MIN = 5;
	static const int PLOT_HISTORY_MAX = 1000;
	static const int DEFAULT_PLOT_HISTORY = 50;
	
	Widget root;
	
	/**
	*	Stack (LIFO) of the instantiated windows.
	*	The first element is the last created window
	*/
	ref SStack<Widget> windows;

	ref map<ButtonWidget, ref SDebugButtonCallback> buttonsCallbacks;
	
	ref map<string, ref array<ref array<float>>> plotsHistory;
	
	void SDebugUI() {
		root = GetGame().GetWorkspace().CreateWidgets("MyMODS/sFramework/GUI/layouts/debug/root.layout");
		root.SetHandler(this);
		root.SetSort(999);
		windows = new SStack<Widget>();
		buttonsCallbacks = new map<ButtonWidget, ref SDebugButtonCallback>;
		plotsHistory = new map<string, ref array<ref array<float>>>;
	}
	
	/**
	*	@brief Build a new window
	*	@param title - title of the window
	*	@param sizePx - X and Y size defined in pixels
	*	@param posPx - X and Y position defined in pixels
	*	@return Widget - new window created
	*/
	Widget window(string title = "", array<int> sizePx = null, array<int> posPx = null) {		
		if (!sizePx || sizePx.Count() < 2) sizePx = DEFAULT_WINDOW_SIZE;
		if (!posPx || posPx.Count() < 2) posPx = DEFAULT_WINDOW_POS;
		
		Widget w = GetGame().GetWorkspace().CreateWidgets("MyMODS/sFramework/GUI/layouts/debug/window.layout", root);
		w.SetHandler(this);
		w.SetSize(sizePx[0], sizePx[1]);
		w.SetPos(posPx[0], posPx[1]);
		
		TextWidget.Cast(w.FindAnyWidget("title")).SetText(title);
		windows.push(w);
		return w;
	}
		
	
	/**
	*	@brief Create a checkbox
	*	@param text - text to show next to the checkbox
	*	@return CheckBoxWidget
	*/
	CheckBoxWidget check(string text) {
		CheckBoxWidget w = CheckBoxWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/checkbox.layout"));
		w.SetName(text);
		w.SetText(text);
		return w;
	}

	/**
	*	@brief Create a button
	*	@param text - text to show next i nthe button
	*	@return ButtonWidget
	*/
	ButtonWidget button(string text, Class instance, string function, Param params) {
		ButtonWidget w  = ButtonWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/button.layout"));
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
		TextWidget w  = TextWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/text.layout"));
		w.SetText(text);
		return w;
	}
	
	/**
	*	@brief Create a slider
	*	@param float - current value of the slider
	*	@param float - step value of the slider
	*	@param float - max value of the slider
	*	@param float - min value of the slider
	*	@return SliderWidget
	*/
	SliderWidget slider(float value, float step = 0.1, float min = 0, float max = 1) {
		SliderWidget w = SliderWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/slider.layout"));
		w.SetStep(step);
		w.SetMinMax(min, max);
		w.SetCurrent(value);
		return w;
	}
	
	/**
	*	@brief Create a table which contains text.
	*	@param sizePx - X and Y size defined in pixel
	*	@param data - matrix of rows and columns of string data
	*	@return WrapSpacerWidget
	*/
	WrapSpacerWidget table(array<ref array<string>> data, array<int> sizePx = DEFAULT_WIDGET_SIZE) {
		if (!sizePx || sizePx.Count() < 2) sizePx = DEFAULT_WIDGET_SIZE;
		WrapSpacerWidget w = WrapSpacerWidget.Cast(widget("MyMODS/sFramework/GUI/layouts/debug/table.layout"));
		w.SetSize(sizePx[0], sizePx[1]);

		if (data == null || data.Count() == 0) return w;
		
		float height = 1 / data.Count();
		foreach (auto row : data) {
			if (row == null || row.Count() == 0) continue;
			float width = 1 / row.Count();
			foreach (auto entry : row) {
				TextWidget t = TextWidget.Cast(GetGame().GetWorkspace().CreateWidget(TextWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, 0xffffffff, 0, w));
				t.ClearFlags(WidgetFlags.EXACTSIZE);
				t.SetFlags(WidgetFlags.CENTER);
				t.SetSize(width, height);
				t.SetText(entry);
			}
		}
		return w;
	}
	
	/**
	*	@brief Plot live data
	*	@param title - title of the plot
	*	@param y - y axis data
	*	@param sizePx - X and Y size of the plot defined in pixel
	*	@param historySize - how many points to store and plot
	*	@param scale - X and Y scale of the plot data
	*	@param offset - X and Y offset of the plot data
	*	@param widthPx - width of the lines defined in pixels
	*	@param color - color of the line
	*	@return CanvasWidget
	*/
	CanvasWidget plotlive(string title, float y, array<int> sizePx = null, int historySize = 50, array<float> scale = null, array<float> offset = null, int widthPx = 5, SColor color = null) {
		if (!sizePx || sizePx.Count() < 2) sizePx = DEFAULT_WIDGET_SIZE;
		if (!scale  || scale.Count()  < 2) scale  = DEFAULT_PLOT_SCALE;
		if (!offset || offset.Count() < 2) offset = DEFAULT_PLOT_OFFSET;
		if (!color)                        color  = DEFAULT_PLOT_COLOR;
		historySize = Math.Clamp(historySize, PLOT_HISTORY_MIN, PLOT_HISTORY_MAX);
		CanvasWidget c = canvas(sizePx, title);
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
				SMath.mapTo(offset[0] + scale[0] * x,              0, 1, 0,         sizePx[0]),
				SMath.mapTo(offset[1] + scale[1] * line[i][1],     0, 1, sizePx[1], 0),
				SMath.mapTo(offset[0] + scale[0] * x + precision,  0, 1, 0,         sizePx[0]),
				SMath.mapTo(offset[1] + scale[1] * line[i - 1][1], 0, 1, sizePx[1], 0),
				widthPx,
				color.getARGB());
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
	CanvasWidget plot(array<ref array<ref array<float>>> lines = null, array<int> sizePx = null, string title = "", array<float> scale = null, array<float> offset = null, int widthPx = 5, SColor color = null) {
		if (!sizePx || sizePx.Count() < 2) sizePx = DEFAULT_WIDGET_SIZE;
		if (!scale  || scale.Count()  < 2) scale  = DEFAULT_PLOT_SCALE;
		if (!offset || offset.Count() < 2) offset = DEFAULT_PLOT_OFFSET;
		if (!color)                        color  = DEFAULT_PLOT_COLOR;
		CanvasWidget c = canvas(sizePx, title);
		foreach (auto line : lines) {	
			for (int i = 0; i < line.Count() - 1; i++) {
				c.DrawLine(
					SMath.mapTo(offset[0] + scale[0] * line[i][0],   0, 1, 0,         sizePx[0]),
					SMath.mapTo(offset[1] + scale[1] * line[i][1],   0, 1, sizePx[1], 0),
					SMath.mapTo(offset[0] + scale[0] * line[i+1][0], 0, 1, 0,         sizePx[0]),
					SMath.mapTo(offset[1] + scale[1] * line[i+1][1], 0, 1, sizePx[1], 0),
					widthPx,
					color.getARGB());
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
		if (!sizePx || sizePx.Count() < 2) sizePx = DEFAULT_WIDGET_SIZE;
		Widget r = widget("MyMODS/sFramework/GUI/layouts/debug/canvas.layout");
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
	Widget spacer(array<float> size = null) {
		if (!size || size.Count() < 2) size = DEFAULT_SPACER_SIZE;
		Widget w = GetGame().GetWorkspace().CreateWidget(FrameWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, 0xffffffff, 0, windows.peek().FindAnyWidget("body"));		
		w.SetSize(size[0], size[1]);
		return w;
	}
	
	
	/**
	*	@brief Create a new line
	*	@param sizePx - height in pixels of the empty line
	*	@return Widget
	*/
	Widget newline(int sizePx = 1) {
		Widget w = spacer();
		w.SetFlags(WidgetFlags.VEXACTSIZE);
		w.SetSize(1.0, sizePx);
		return w;
	}
	
	/**
	*	@brief Create a widget
	*	@param layout - .layout file to load
	*	@param name - name of the root widget
	*	@return Widget
	*/
	Widget widget(string layout, string name = string.Empty) {
		Widget w = GetGame().GetWorkspace().CreateWidgets(layout, windows.peek().FindAnyWidget("body"));
		if (name != string.Empty) {
			w.SetName(name);
		}
		w.SetHandler(this);
		return w;
	}
	
	
	void begin() {
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
		switch (w.GetName()) {
			case "collapse":
			Widget body = w.GetParent().GetParent().GetParent().GetParent().FindAnyWidget("body");
			if (body.IsVisible()) {
				ButtonWidget.Cast(w).SetText("+");
			} else {
				ButtonWidget.Cast(w).SetText("-");
			}
			body.Show(!body.IsVisible());
			break;
			
			
			case "maximize":
			break;
			
			case "close":
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
	override bool OnMouseWheel(Widget w, int x, int y, int wheel);
	override bool OnMouseButtonDown(Widget w, int x, int y, int button);
	override bool OnMouseButtonUp(Widget w, int x, int y, int button);
	override bool OnController(Widget w, int control, int value);
	override bool OnKeyDown(Widget w, int x, int y, int key);
	override bool OnKeyUp(Widget w, int x, int y, int key);
	override bool OnKeyPress(Widget w, int x, int y, int key);
	override bool OnChange(Widget w, int x, int y, bool finished);
	override bool OnDrag(Widget w, int x, int y);
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever) {
		w.GetParent().SetPos(x, y);
		return true;
	}
	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever);
	override bool OnDrop(Widget w, int x, int y, Widget reciever) {
		return true;
	}
	
	override bool OnDropReceived(Widget w, int x, int y, Widget reciever);
	override bool OnResize(Widget w, int x, int y);
	override bool OnChildAdd(Widget w, Widget child);
	override bool OnChildRemove(Widget w, Widget child);
	override bool OnUpdate(Widget w);
	override bool OnEvent(EventType eventType, Widget target, int parameter0, int parameter1);
	
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