# sFramework

<p align="center">
        <img src="https://imgur.com/wHfyEnv.png" width="50%">
        <br>
        <img src="https://img.shields.io/github/v/tag/simonvic/sFramework?color=F0544C&label=latest%20stable%20release&style=for-the-badge">
        <br>
        <img src="https://img.shields.io/github/license/simonvic/sFramework?color=F0544C&style=flat-square">
</p>

# Getting started

sFramework is the core of the sUDE project.

It ships many features and utilities used and/or implemented by sUDE modules :

- [Advanced PostProcessing](#PostProcessing-effects)
- [Cameras Overlays](#Camera-Overlays)
- [Game and User configuration interface](#Configurations-interfaces)
- [Helper classes and utilities for developing and debugging](#Utilities)
- Improvements to base game classes
- ...more

---
---

<br>
<br>

<h1 align="center"> PostProcessing effects </h1>

sFramework ships a centralized post processing effects manager, with the goal of allowing multiple requests of the same effects, without hardcoding them.

<br>

## SPPEffect

`SPPEffect` is the "container" of any PostProcess Effect you wish to add to it (e.g. saturation, vignette, motion blur etc.).

```csharp
SPPEffect myPPE = new SPPEffect();
```

To add a parameter use the provided setters:

```csharp
myPPE.setVignette(intensity, color);
myPPE.setRadialBlur(powerX, powerY, offsetX, offsetY);
myPPE.setChromAber(powerX, powerY);
//...
```

To apply it, "hand it over" to the SPPEManager, which will calculate the correct value of all active SPPEffect and then apply it

```csharp
SPPEManager.activate(myPPE);
```

and to deactivate it:

```csharp
SPPEManager.deactivate(myPPE);
```

<br>

## SPPEffectAnimated

A `SPPEffectAnimated` is just like a `SPPEffect`, but it has an animation mechanism which allows you to animate the values of a PostProcess effect.

A `SPPEffectAnimated` is an *abstract* class. You need to implement it with your own class and override the `onAnimate()` method, which will be called on every frame.

There also is a timed variant `SPPEffectTimed`, which will be automatically deactivated once a certain amount has passed.

To create your animation, simply extend either `SPPEffectAnimated` or `SPPEffectTimed`

```csharp
class MyLoopAnimation : PPELoopedParams{
    override void onAnimate(float deltaTime){
        /* change PPE values here
        setOverlay(...);
        setChromAber(...);
        setCameraEffects(...);
        */
        setVignetteIntensity( Math.Sin(getTime()) );
    }
}

class MyTimedAnimation : SPPEffectTimed{
    override void onAnimate(float deltaTime){
        setVignetteIntensity( Math.Cos(getTime()) );
    }
}
```

A `SPPEffectTimed` also has a "duration" which can be set with the constructor, or the provided method:

```csharp
MyTimedAnimation myTimedAnimation = new MyTimedAnimation(6); // the animation will last 6 seconds
myTimedAnimation.setDuration(10.0); // the animation will last 10 seconds
```

The activation of the animation is identical to any other `SPPEffect`

```csharp
MyLoopAnimation myAnimation = new MyLoopAnimation();
SPPEManager.activate(myAnimation);

MyTimedAnimation myTimedAnimation = new MyTimedAnimation(5.5);
SPPEManager.activate(myTimedAnimation);
```

If you want to manually manage the animation you can use the provided methods

```csharp
myAnimation.start();  // Set the animation state to "Playing"
myAnimation.stop();   // Reset the time and set the animation state to "Stopped"
myAnimation.pause();  // Freeze the animation values and set the animation state to "Paused"
myAnimation.resume(); // Resume the the animation and set the animation state to "Playing"
```

## The insides of SPPEManager

<h3 align="center">PostProcess Effect Manager</h3>
The SPPEManager is in charge of managing the PostProcessing effects; this is a small diagram roughly showing how it works
<img src="https://imgur.com/IKHcKQR.png">
---
---
<br>
<br>

<br>
<br>

<h1 align="center"> Camera Overlays </h1>

A camera overlay is nothing else than an image, used like an HUD.
The fundemental unit of camera overlays is the `SCameraOverlay`, a very simple wrapper for the `ImageWidget` (the DayZ UI component that holds an image).

It can be used in countless ways:

As an animated UI :

<img src="https://imgur.com/78gLf3X.gif" width="50%">

or for emulating headgear damage:

<img src="https://imgur.com/u4Sng17.gif" width="50%">

##### (*from sVisual, MotoHelmet in various health state: Pristine, Worn, Damaged, BadlyDamaged and Ruined)*

<br>
<br>

Defining an overlay is very simple and very similar to SPPEffects, in fact there are three types as well and the logic is identical to the SPPEffects:

- `SCameraOverlay`
- `SCameraOverlayAnimated`
- `SCameraOverlayTimed`

```csharp
class MyAnimatedOverlay : SCameraOverlayAnimated {

    override void onInit(){
        setImage("path/to/texture.edds");
        //...
    }

    //onAnimate() gets called every frame!
    override void onAnimate(float deltaTime){
        setSize(Math.Sin(getTime()));
        //setPosition(...)
        //setRotation(...)
        //setMask(...)
        //...
    }
}
```

To activate/deactivate an overlay, you use the `SCameraOverlayManager`:

```csharp
// NOTE: SCameraOverlaysManager is a singleton
SCameraOverlaysManager.getInstance().activate(myOverlay);
```

## Clothing overlays

sFramework is capable of automatically activating/deactivating overlays when a clothing item is equipped/unequipped; making use of this feature is super easy. You just need to define a list of overlays inside your clothing item in your `config.cpp` as follows:

```cpp
class YOUR_CLOTHING_ITEM_CLASSNAME{
    class sUDE {
        class CameraOverlays {
            class overlay_0 : SCameraOverlay {
                image="path/to/your/image/pristine.edds";
            };
            class overlay_1 : SCameraOverlay {
                image="path/to/your/image/worn.edds";
            };
            class overlay_2 : SCameraOverlay {
                image="path/to/your/image/damaged.edds";
            };
            class overlay_3 : SCameraOverlay {
                image="path/to/your/image/badlydamaged.edds";
            };
            /*
            class overlay_X : SCameraOverlay {
                image="path/to/your/image/xxx.edds";
            };
            */
        };
    };
};
```

A `SCameraOverlay` has many attributes you can play with, which can be set either by scripts or in the config.
Currently available attributes are:

```cpp
image="";                                 // Resource image path, can be whatever an ImageWidget accepts texture
alpha=1.0;                                // [0.0 - 1.0] Alpha value (transparency)
mask="";                                  // Resource image path, can be whatever an ImageWidget accepts as mask
maskProgress=1.0;                         // [0.0 - 1.0] Mask progress
maskTransitionWidth=1.0;                  // Mask transition width (used as progress + transitionWidth)
position[] = {0.0, 0.0};                  // [0.0 - 1.0] X and Y position in screenspace
size[] = {1.0, 1.0};                      // [0.0 - 1.0] X and Y size in screenspace
rotation[] = {0.0, 0.0, 0.0};             // Yaw, Pitch and Roll defined in degrees
priority = 0;                             // Higher priority means closer to the camera (also known as z-depth)
targetCameras[] = {"DayZPlayerCamera"};   // Camera typename on which the overlay will be visible
hidesWithIngameHUD = 0;                   // [0 = false, 1 = true] Determines if it must hides when the player hides the ingame HUD
```

<br>
<br>

# Configurations interfaces

## SUserConfig

`SUserConfig` has the purpose to help in creating user (client) settings in just few lines of code.

Implement the `SUserConfigBase` as follows:

```csharp
class MySUserConfig : SUserConfigBase {

    // Configuration options (and their default values) you want to store
    float myFloatValue = 0.69;
    //int myIntValue = 69;
    //bool myBooleanValue = true;
    //float myArray[4] = {0.69, 42.0, 420.69, 0.42069};
    //any other options 

    /**
    *   Where the config will be saved
    */
    override string getPath(){
        return "$saves:\\path\\to\\my\\config.json";
    }
    
    /**
    *   Where the config with default values will be saved
    */
    override string getDefaultPath(){
        return "$profile:\\path\\to\\my\\config_default.json";
    }

    /**
    *   Implement the deserialization
    */
    override void deserialize(string data, out string error){
        MySUserConfig cfg = this;
        m_serializer.ReadFromString(cfg, data, error);
    }
    
    /**
    *   Implement the serialization
    */
    override string serialize(bool serializeDefault = false){
        string result;
        MySUserConfig cfg;
        if (serializeDefault) {
            cfg = new MySUserConfig();
        } else {
            cfg = this;
        }
        m_serializer.WriteToString(cfg, true, result);
        return result;
    }
}
```

you can now save it, load it and more with few lines

```csharp
MyUserConfig myCfg = new MyUserConfig();
myCfg.save();
myCfg.load();
//myCfg.isValid()
// etc.
```

## SGameConfig

`SGameConfig` contains just a set of utilities to read the game `config.cpp` more easily

<br>
<br>

# sTest (UnitTesting framework)

`sFramework` also ships `sTest`, a UnitTesting framework for Enforce scripts, based on industry standard frameworks such as JUnit for Java.

Test units are super simple to define and use:

1. Create a TestUnit class (extends `STestUnit`)
2. Create some test cases function
3. Register the test cases by passing the test case name (function name) to `registerTestCases` method

```csharp
class MyTestUnit : STestUnit {
    
    override void init() {
        registerTestCases({
            "testThisFeature",
            "testThisOtherFeature",
            "shouldFail"
        });
    }

    void testThisFeature() {
        // do something...
        // assert something
        assertEqual(10, 5 + 5);
    }

    void testThisOtherFeature() {
        // do something...
        // assert something
        assertTrue(true);
    }

    void shouldFail() {
        // this test case will fail!
        Class someClass = null;
        assertNotNull(someClass);
    }
}
```

You can now run the test unit by passing its name to `sTest`:

TIP: you can execute the following in the workbench console

```csharp
STest.run(MyTestUnit);

// optionally an array of test units can be used to run multiple test units
STest.run({MyTestUnit, MyOtherTestUnit, MyLastTestUnit});
```

The result of the tests can be seen in the output window of the workbench or inside sUDE logs.

```text
=======================================================================
Running tests...
-----------------------------------------------------------------------
MyTestUnit
│    ├ [ ✓ ] PASSED  - testThisFeature
│    ├ [ ✓ ] PASSED  - testThisOtherFeature
│    ├ [ × ] FAILED  - shouldFail
│    │    ├ Expected: true
│    │    ├ Actual:   false
-----------------------------------------------------------------------
                   PASSED    |    FAILED    |    SKIPPED
                     2              1               0
=======================================================================
```

You can decide not to stop when a test fails:

```csharp
STest.shouldContinueAtFail = true; // default: false
```

or to change verbosity in logging:

```csharp
STest.verbosity = 3; // default: 1
```

<br>

## Assertions

You have access to multiple assertions:

- `assertEqual(x, y)` with x and y of type `float`, `int`, `string`, `bool`, `array<float>`
- `assertTrue(x)` with x of type `bool`
- `assertFalse(x)` with x of type `bool`
- `assertNull(x)` with x of type `Class`
- `assertNotNull(x)` with x of type `Class`


## Advanced TestUnit usage

If you need to perform some actions before or after each test unit or test case you can define and register some callbacks:

```csharp
class MyTestUnit : STestUnit {
    
    override void init() {

        registerBeforeClassCallbacks({
            "doSomethingBeforeTestUnit"
        });
        
        registerBeforeCallbacks({
            "doSomethingBeforeEachTestCase"
        });

        registerAfterCallbacks({
            "doSomethingAfterEachTestCase"
        });

        registerAfterClassCallbacks({
            "doSomethingAfterTestUnit"
        });

        // registerTestCases({
        //     ...
        // });
    }

    void doSomethingBeforeTestUnit() {
        // do something ...
    }

    void doSomethingBeforeEachTestCase() {
        // do something ...
    }

    void doSomethingAfterEachTestCase() {
        // do something ...
    }

    void doSomethingAfterTestUnit() {
        // do something ...
    }

}
```

If you need to write some more complext test cases, you can also manually `fail()`, `pass()` or `skip()`. Example:

```csharp
void testSomethingComplex() {
    int x = 2;
    int y = 2;
    int actual = x + y;
    int expected = 4;

    if ( x == y) {
        fail("x and y not equal", "x and y are equal", "Failed during X and Y comparison");
    } else {
        assertEqual(expected, actual);
    }
}
```

<br>
<br>

# Utilities

## SColor

`SColor` helps you defining and using colors. A few examples:

```csharp
//hex values, like in CSS
SColor.rgb(0xFF0000);    //red
SColor.rgba(0xFF000055); //red slightly transparent
SColor.argb(0x55FF0000); //red slightly transparent

//separated rgb channels
SColor.rgb(60, 97, 178);      //blueish
SColor.rgba(60, 97, 178, 0);  //blueish
SColor.argb(0, 60, 97, 178);  //blueish

// hue saturation and brightness
SColor.hsb(0.60, 0.65, 0.87); //yellowish

// presets (taken from https://www.w3schools.com/cssref/css_colors.asp)
SColor.rgb(RGBColors.RED);
SColor.rgb(RGBColors.AQUAMARINE);
SColor.rgb(RGBColors.YELLOW_GREEN);
```

## SObservableArray

A list that allows listeners to track changes when they occur.

```csharp
class MyClass {
    
    ref SObservableArray<int> observableArray = new SObservableArray<int>();

    void MyClass() {
        observableArray.addOnChangeListener(new SArrayChangeListener(this, "onChange"));

        // multiple listeners can be added
        observableArray.addOnInsertListener(new SArrayInsertListener(this, "onInsert"));
        observableArray.addOnInsertListener(new SArrayInsertListener(this, "onInsert2"));

        observableArray.addOnPreRemoveListener(new SArrayPreRemoveListener(this, "onPreRemove"));
        observableArray.addOnClearListener(new SArrayClearListener(this, "onClear"));
    }

    void onChange() {
        SLog.d("Array has changed");
    }

    void onInsert(int value, int position) {
        SLog.d("Value " + value + " has been inserted in position " + position);
    }

    void onInsert2(int value, int position) {
        // do somehting... 
    }

    void onPreRemove(int indexToBeRemoved) {
        SLog.d("Index " + indexToBeRemoved + " will be removed");
    }

    void onClear() {
        SLog.d("Array has been cleared");
    }

}
```

```csharp
observableArray.insert(69); // onChange, onInsert and onInsert2 will be called
observableArray.insert(420); // onChange, onInsert and onInsert2 will be called

observableArray.removeItem(69); // onPreRemove and onChange will be called
observableArray.remove(0); // onPreRemove and onChange will be called
observableArray.clear(); // onClear and onChange will be called
```

## SSpawnable

`SSpawnable` helps you in quickly spawn items with a lot of attachments:

```csharp
// Build an M4A1 with multiple attachments
SSpawnable m4 = SSpawnable.build("M4A1").withAttachments({
    "M4_Suppressor",
    "M4_OEBttstck",
    "M4_RISHndgrd"
});

// Build an M16A2 with no attachments
SSpawnable m16 = SSpawnable.build("M16A2");

// Build an AK101 with multiple attachments (and they attachments too)
SSpawnable ak = SSpawnable.build("AK101").withAttachments({
    "AK_Suppressor",
    "AK_PlasticBttstck",
    "AK_RailHndgrd"
}).withSpawnableAttachments(
    (new SSpawnable("PSO11Optic")).withAttachment("Battery9V"),
    (new SSpawnable("UniversalLight")).withAttachment("Battery9V"));

// Actually spawn the items
m4.spawn(position);
m16.spawn(position);
ak.spawn(position);
```

## SRaycast

`SRaycast` helps you launching raycasts with more flexibility:

```csharp
SRaycast ray = new SRaycast(/**...*/);
vector contactPositon = ray
    .from(thisPosition)
    .to(thisOtherPosition)
    .ignore(thisItem, thisOtherItem)
    .launch()
    .getContactPosition();

if (ray.hasHit()){
    SLog.d("Raycast has hit at this position" + contactPositon);
}
```

## SFlagOperator

`SFlagOperator` helps you in bitwise operations, especially when working with flags, hence the name.

```csharp
enum MyFlags {
    A = 1,
    B = 2,
    C = 4,
    D = 8,
    E = 16
}

SFlagOperator fop = new SFlagOperator(MyFlags.A | MyFlags.C);
SLog.d("Result : " + fop.collectBinaryString());
// Result : 0000 00101

fop.set(MyFlags.B);
fop.reset(MyFlags.A)
SLog.d("Result : " + fop.collectBinaryString());
// Result : 0000 00110

SLog.d("A is set : " + fop.check(MyFlags.A));
//A is set : false

SLog.d("B is set : " + fop.check(MyFlags.B));
//B is set : true
```

<br>
<br>

# Contact me

<h2 align="center">Found a bug or want to give a suggestion? Feel free to contact me!</h2>
<p align="center">
    <a style="margin: 0 10px" href = "mailto: simonvic.dev@gmail.com">
        <img src="https://img.shields.io/badge/Email-simonvic.dev%40gmail.com-F0544C?style=social&logo=data:image/png;base64,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&logoColor=F0544C">
    </a>
    <br>
    <a style="margin: 0 10px" href="https://matrix.to/#/@simonvic:matrix.org">
        <img src="https://img.shields.io/badge/Matrix-%40simonvic%3Amatrix%3Aorg-F0544C?logo=element&style=social">
    </a>
    <br>
    <img style="margin: 0 10px" src="https://img.shields.io/badge/Discord-simonvic%239804-F0544C?logo=Discord&style=social">
    <br>
    <a style="margin: 0 10px" href="https://twitter.com/_simonvic_">
        <img src="https://img.shields.io/badge/Twitter-%40simonvic-F0544C?logo=twitter&style=social">
    </a>
</p>

---

<h3 align="center" >Buy me a coffee :)</h3>
<p align="center">
    <img width="40px" src="https://i.imgur.com/e3kk9J4.png">
    <a href = "https://paypal.me/simonvic">
        <img src="https://img.shields.io/badge/Paypal-donate-F0544C?style=for-the-badge&logo=paypal&logoColor=F0544C">
    </a>
</p>
