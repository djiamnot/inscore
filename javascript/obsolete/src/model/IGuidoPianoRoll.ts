///<reference path="../externals/libGUIDOEngine.d.ts"/>
///<reference path="../vhtmlview/VHtmlTools.ts"/>
///<reference path="IGuidoCode.ts"/>

const kCLine        : number =  1;
const kCSharpLine   : number =  1<<1;
const kDLine        : number =  1<<2;
const kDSharpLine   : number =  1<<3;
const kELine        : number =  1<<4;
const kFLine        : number =  1<<5;
const kFSharpLine   : number =  1<<6;
const kGLine        : number =  1<<7;
const kGSharpLine   : number =  1<<8;
const kALine        : number =  1<<9;
const kASharpLine   : number =  1<<10;
const kBLine        : number =  1<<11;
const kAutoLines    : number =  0;
const kNoLine       : number = -1;

class IGuidoPianoRoll extends IGuidoCode {

	protected fLimits: LimitParams;
	protected fKeyboard: boolean;
	protected fAutoVoiceColor: boolean;
	protected fMeasureBars: boolean;
	protected fPitchLines: number;
 	//std::map<int, IColor> fVoicesColor;
   
    constructor(name: string, parent: IObject) {
        super(name, parent);

        this.fTypeString 	= kGuidoPianoRollType;
        this.fKeyboard 		= false;
        this.fAutoVoiceColor= false;
        this.fMeasureBars 	= false;
        this.fPitchLines 	= kAutoLines;
        //this.fLimits.lowPitch = -1;
        //this.fLimits.highPitch = -1;
        
        this.setWidth(1);
        this.setHeight(0.5);
        super.setHandlers();
        this.fMsgHandlerMap[kwidth_GetSetMethod]  = new TMsgHandlerNum( (n: number): void => { this.setWidth(n); });
        this.fMsgHandlerMap[kheight_GetSetMethod] = new TMsgHandlerNum( (n: number): void => { this.setHeight(n); });
    }      

    getPRollMap(pr: PianoRoll): void { this.guidoMap2inscoreMap (IGuidoCode.fGuidoMap.getPianoRollMap(pr, 1, 1)) };
    
    AR2SVG(size: TSize): string {
        if (this.fAR) {
			let pr = IGuidoCode.fPianoRoll.ar2PianoRoll(PianoRollType.kSimplePianoRoll, this.fAR);
			this.fSVG = IGuidoCode.fPianoRoll.svgExport(pr, size.w, size.h);
			this.getPRollMap (pr);
			IGuidoCode.fPianoRoll.destroyPianoRoll(pr);
		}
	    return this.fSVG;
    }
}