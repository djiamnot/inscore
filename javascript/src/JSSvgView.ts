
///<reference path="JSSVGBase.ts"/>

class JSSVGView extends JSSvgBase {

	constructor(parent: JSObjectView) {
		super(parent);
		this.getElement().className = "inscore-svg";
	}

	getSVGTarget() : SVGShape   { return this.fSVG; }
	toString() : string		    { return "JSSVGView"; }
	updateSVGDimensions(w: number, h: number) : void { }

	updateSpecial(obj: INScoreObject, oid: number)	: boolean {
		let svg = obj.getSVGInfos();
		this.fSVG.innerHTML = svg;
		let bb = this.fSVG.getBBox();
		this.updateObjectSizeSync (obj, bb.width + bb.x, bb.height + bb.y);
		return true;
    }
}
