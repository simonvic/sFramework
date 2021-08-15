# sFramework

<p align="center">
        <img src="https://imgur.com/cQvynpF.png" width="50%">
        <br>
		<img src="https://img.shields.io/github/v/tag/simonvic/sFramework?color=F0544C&label=latest%20stable%20release&style=for-the-badge">
        <br>
		<img src="https://img.shields.io/github/license/simonvic/sFramework?color=F0544C&style=flat-square">
</p>

# Getting started

---
---

<br>
<br>

# Camera Overlays
A camera overlay is nothing else than an image, used like an HUD.
The fundemental unit of camera overlays is the `SCameraOverlay`, a very simple wrapper for the `ImageWidget` (the DayZ UI component that holds an image).

Define the overlay, with all the attributes you want:
```csharp
class MyAnimatedOverlay : SCameraOverlayAnimated {
    //onInit() gets called only once
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
        //etc.
    }
}
```
And activate/deactivate it using the SCameraOverlayManager:
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
```
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

# Contact me
<h2 align="center">Found a bug or want to give a suggestion? Feel free to contact me!</h2>
<p align="center">
    <a style="margin: 0 10px" href = "mailto: simonvic.dev@gmail.com">
        <img src="https://img.shields.io/badge/Email-simonvic.dev%40gmail.com-F0544C?style=social&logo=data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAOEAAADhCAMAAAAJbSJIAAAAh1BMVEX///8zMzMsLCxsbGwwMDAVFRUmJiYbGxsWFhYtLS0oKCj4+PgREREjIyMgICAZGRnp6ena2to4ODjj4+Py8vKzs7OZmZnMzMygoKB3d3fIyMiDg4NTU1NoaGheXl7S0tJJSUmvr6+GhoY+Pj5VVVWRkZFzc3NLS0u/v7+ioqIAAAB9fX2xsbHNtidiAAAIyUlEQVR4nO2d6XbaShCER7JYxGpssBMveMFOHOz3f75AwAZJVbMbSTn9/ck5N3CjAqlrqmdBKUEQBEEQBEEQBEEQBEEQBEEQBEEQBEEQBEEQhP+V17P/gec3rnA26aatZ/oy1HyJ50matJzeT/19Orzr1n2JYXRejc/i/aTuiwxh9GQUqNR7r+7L9Cbr3FgIVOqqU/eVepIPZlYClfoY1X2tXqQX55YClbqZZnVfrjtd4hLnv9B/nY3zui/Ylck9ErIRvVgvofBVy4xx+gxldK/VbTq4Q9/u8HFQ90W7MPoBb8XB+FXdZZthDHxCW2SM2RQORW9GWbZUq4RW2dYYY9Z/QNf/tLWEldq9ZASd8qodrpEml+jqX3e2rvav6i/Qiz7a4P2Dxzm69p/7O/BTYTK6gndy841xDL1ALcf7v/9SmPRu0Qtng4YbIw5L84MTHBQmkzP02vOLRhvjCIaly+xw0UcKky42xt8NNkYclh56R8/WscIkXcFntrHGmHWgDb51jotHQWGSd9tkjORqfxQ9rqgwyXrQOxtpjCQsPZe+jpLCjfe3xRhx1VBn5UeqrHDz8P5Bb7zpN8wYceUHjbSqwqQPc8is2yhjnEL3RpkPKEwmMBQ3yhg7OCyh3I4UkoHQ8HdTWqnZCIelHnqSoMJk0GhjzCY8LFXBClkoboQx5gMYllgPlChssDEawlIFppA1BsitcDpMYakCVZgkaziqrTkxkrC04ulAozDpw2RSqzHiS7rUzQnqFCa9d/T/q9EYLcJSBa3ChhljNoX9smJYqqBXmAx+w+FtLcaYj23CUgWDQhZRbk9vjCwsTQ3vMylkn9zJjbGL76ZKWKpgVMjmVRenTYzWYamCWeGmgsFQfNLESMKSTVW3UUi6xbP0ZMZIwpKVM1spJN3iU80xks6K5ejKTiGbYD3J4pusd43+cdsRsqVC675PfPIMzyz1Ld9vq5B1i29NdhQKCeM0LFWwVsimUU1DikCcw1IFe4VJNoHPw+I7JfZgU2zussbAQeEmMcJQfK0d2geBl+FdOs34OSkM8iUPSFiaOH2ibgpJt/h7jJH0Ud5GbreMo0LSLf4OY8wnuLK5joddFSZjvHosujH6hqUKzgpZNy9yYvQOSxXcFSZpjjuyMV0j4ijRQyHrqkdMjGSk79UC81HI5s9vHKschaQ1vxWhXgrZNOpllMRotf7AHk+FpDUbo5UaGJYq+Cok7fVwY8xxHXv1rmPeCtmoP7CVGhyWKvgrpMYYkhjHL/BjcwhLFQIUsmlU53HVAeMyPA9CFLJQvPB1DRKWsqDyFaSQrS32TIwdHJYC+7JhCtlV+ZgzWWDxFtp4DlVIxh/uidFuGZ4HwQrJGNJ1jpFULfewVCFcIckBbkGHtGNjzFNGUMguzyExfmdLPYbCTR6H3m/9DAXMLJmJojDJU2yMdnGgA4vVQ6RdAnEUJn3YoLKaHiIzS+o5Us8gjkJsGcomMZJOuoq2/CqGQvYtbDEZI1mGt7sDovQMIijMx7Bts2f4oiuIJCztibJhJ1yhcX+xxhhJ8/XoDgjfzRKs0HSRSjMwwTNLxwxDkuGOUIWmUyj+QYwR93pKBKT7HYEKaREtAhMj7tdV8O/Q7AhSmOF9mYBqYsz6mlNkCjyFdZpDFOZwpfQQfq3lWU2ymAxax41npzRYYbpCRfT8ogOH0fOCMZICvFjjPkYe4BoB3URYRLcLpfDxDcdJYfAIC/BVh03cB7hG5I7wboKWfENfxkgS5fvWVUgU83cNX4W4iH42EknW2LdS8eLqz7iLb35/1/BTSIro+5e1kybcn23lxwc8HJqipGPje2qOl0JcRIt3UucDvWSTGDvwwzluihIj8WzD+ijE91G5GuD7+HoEw1LJL/FgwK8N66GQFNFKDiCPm9XXg1e1eHW/3RWSIgpcmZTMKmAJAH6vzwyGs0LcVPkDB4/E9srAHXGsv+jsGo4KSZxnM2pWZ1MRq2MbBF17xG4KSZxf0oyb9Uzni/HhCtkg6Nrnd1KIvxLtw5H19WfE6YaccVzDRSEfierQxkCDAeD36ndylXFQiIuoeduF5qxG49eBp320u/HK2CvEDo6LaOkyWTvGYqEYmdFwOG/FViEZbNnNfo2XsPJbDabJegj7WSlLhTleymb778DLtAxE5Jwr6yUfdgrJSNR+hFFdz2gfaslZZbZLPqwU4iJ6mTs871nJSV0aEySrWS6HtlFIiqibLWWFhf6Ob8ZV7iHWzi4yEnUOpJ2DuTkfd0P2XNu4hlEhGYk+eyTur0jk0eQle64j7LAkI1G/JQT7hf5eLRfiGsG7ZIOLaJFtn3H44tc2IzONz6Y9bIb9+LiIpt5LCNLVzHsZXtbD3S3D86I/UwEX0ZB9TlnAm0lJCDhxoA+LaJ2nm+BBvH4VHVeY4Z1q1ns3vwV8V2lX3lCF+SBiEY0Hrgw612AKcZdkXv9hWGS37i/6yROF0YtoPEjTnz49WCEpom5bG78LcowFq4BQIR7oPsXa9BOM08GHQCGJ8/UW0SLkfCfYE6sqJHH+BGcLOMBOBAbjpYpC0hOtv4gWIScCg85IWeEYvtOpfXca2Ex6JbeUFOIiet3EU73JTujX8kC8qBAX0Y/GFNEC9FTA4tUeK2RFtO6DBClr7BrF+HKkkMR5Ph6qH/yzHcXp6INC/Og2/BeuyFTxcS/2SyEZiTaviBYh+/aPXONTIZ49uW7GSFQHcY1Dt0td/PsDF1HvjYSnhJwO8DnIXKnHjDZAmjQS1UB2ty/W//7yTv1MaRFtwqHBVpDFSdtHLL1VT+NWFtEieOPUdjvLeKHO17CINup8cjMT2PSfPw6mc6XwYoBJo86YN0Oa/ku2LKsVRbRIig+uI3uOWvkDiOQ8DUjwJo56IMGhyvAueCNOXdjtb2ndzwIeY7NH6aHXsiJaxLzPbLGu+xeZA5nglf5fvC3r/k3tYO7vdXs2BUEQBEEQBEEQBEEQBEEQBEEQBEEQBEEQBEEQWs5frubG6Ho4CnAAAAAASUVORK5CYII=&logoColor=F0544C">
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