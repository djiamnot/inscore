
///<reference path="JSAutoSize.ts"/>

class JSHtmlView extends JSAutoSize {

    constructor(parent: JSObjectView) {
		super( document.createElement('div'), parent); 
        this.getElement().className = "inscore-html";
    }

	toString() : string					{ return "JSHtmlView"; }

	// CSS weight are used as numbers
	static fontWeight2Num	( weight: string) : string {
		switch (weight) {
            case "normal": 		return "400";
			case "light":		return "200";
			case "demibold":	return "550";
			case "bold":		return "700";
			case "black":		return "900";
			default: return "400";
		}
	}

	setFont	(font : OTextInfo) : void {
    	let elt = this.getElement();
        elt.style.fontSize 		= font.size+"px";
        elt.style.fontFamily 	= font.family;
        elt.style.fontStyle 	= font.style;
        elt.style.fontWeight 	= JSHtmlView.fontWeight2Num(font.weight);
    }

	setNone ()	: void				{ this.getElement().style.textShadow = "0px 0px 0px";  }
	setBlur (val: number) : void  	{ this.getElement().style.textShadow = "0px 0px " + val + "px"; }
	// setShadow (params: Array<number>) : void {
	// 	let color = new IColor( params.slice(2,6) );
	// 	this.getHtml().style.textShadow = color.getCSSRGBAString() + params[0] +"px " + params[1] +"px " + params[6] +"px";
	// }

	// don't update text dimensions
	updateDimensions(pos: OPosition) : void {}

	updateSpecific(obj: INScoreObject)	: void { 
		this.setFont (obj.getTextInfos());
	}

	getText (infos: OTextInfo) : string { return infos.text; }

	updateSpecial ( obj: INScoreObject, objid: number)	: boolean {		
		let infos = obj.getTextInfos();
// console.log(this + " content : " + this.getText(infos) );
		this.getElement().innerHTML  = this.getText(infos);
		this.setFont (infos);
		return super.updateSpecial (obj, objid );
	}
}