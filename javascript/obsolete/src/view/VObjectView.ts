
///<reference path="../lib/TTypes.ts"/>
///<reference path="../model/IObject.ts"/>

interface scaleMethod { (x: number, y: number): void; }

abstract class VObjectView {
    
	abstract updateView			( obj: IObject) 		: void;
	abstract updateObjectSize	( obj: IObject ) 		: void;
	abstract setPositionHandler	( fpos: TPositionHandler) : void;
			 setMapScaleHandler	( scale: scaleMethod ) : void {}

	// Removes an element
	abstract remove() : void;

	// Maps the IObject [-1,1] y coordinate to the referenceRect().
	abstract relative2SceneY(y: number) : number;

	// \brief Maps the IObject [-1,1] x coordinate to the referenceRect().
	abstract relative2SceneX(x: number) : number;

	// \brief Maps the IObject [0,2] width value to the corresponding referenceRect() value.
	abstract relative2SceneWidth(width: number) : number;

	// \brief Maps the IObject [0,2] height value to the corresponding referenceRect() value.
	abstract relative2SceneHeight(height: number) : number;

	// \brief Maps the referenceRect() width value to the corresponding [0,2] value.
	abstract scene2RelativeWidth(width: number) : number;

	// \brief Maps the referenceRect() height value to the corresponding [0,2] value.
	abstract scene2RelativeHeight(height: number) : number;

	// \brief Maps the referenceRect() x value to the corresponding [-1,1] value.
	abstract scene2RelativeX(x: number) : number;

	// \brief Maps the referenceRect() y value to the corresponding [-1,1] value.
	abstract scene2RelativeY(y: number) : number;

	// \brief updates the local mapping (do nothing at IObject level) 
//	abstract updateLocalMapping (obj: IObject ): void;
		
	// \brief initialize an object view (do nothing at IObject level)
	abstract initialize (obj: IObject ): void
}
