
///<reference path="../controller/THandlersPrototypes.ts"/>
///<reference path="Constants.ts"/>

enum color {
    kHue     = 0,
    kSat     = 1,
    kVal     = 2,
    kRed     = 3,
    kGreen   = 4,
    kBlue    = 5,
    kAlpha   = 6,
}

class IColor {

// ATTRIBUTES
//--------------------------------------------------------------     
    protected fRGB : Array<number>;
    protected fHSB : Array<number>;
    
    protected fA : number = 255; 
    protected fModified : boolean; 

// CONSTRUTOR
//--------------------------------------------------------------    
    constructor(input : Array<number>|IColor) {
        if (input instanceof Array) { 
        	this.fRGB = input.slice(0,3);
        	if (input.length == 4) this.fA = input[3]; 
        }
        else if (input instanceof IColor) {
            this.set (input);
        }
        this.fHSB = new Array<number>();
        this.updateHSB();         
    }
  
// MODIFIED STATUS
//--------------------------------------------------------------      
   cleanup(): void 		{ this.fModified = false; }   
   modify(): void 		{ this.fModified = true; }   
   modified(): boolean	{ return this.fModified; }   


// PROPERTIES COPY
//--------------------------------------------------------------    
   set (color: IColor) {
        this.fRGB = color.fRGB.slice(0);
        this.fHSB = color.fHSB.slice(0);
        this.fA = color.fA;
        this.fModified = true;
   }

// GETS VALUES
//-------------------------------------------------------------- 
    getRGB(): Array<number> { return this.fRGB.concat(this.fA); }
    getHSB(): Array<number> { return this.fHSB.concat(this.fA); }
  
	getRGBAString(): string { return `rgba(${this.fRGB[0]}, ${this.fRGB[1]}, ${this.fRGB[2]}, ${this.fA})`; }
	getCSSRGBAString(): string { return `rgba(${this.fRGB[0]}, ${this.fRGB[1]}, ${this.fRGB[2]}, ${this.getSVGA()})`; }
	getRGBString(): string { return `rgb(${this.fRGB[0]}, ${this.fRGB[1]}, ${this.fRGB[2]})`; }

    
    getR(): number { return this.fRGB[0]; }
    getG(): number { return this.fRGB[1]; } 
    getB(): number { return this.fRGB[2]; } 
       
    getH(): number { return this.fHSB[0]; } 
    getS(): number { return this.fHSB[1]; } 
    getV(): number { return this.fHSB[2]; }      
        
    getA(): number { return this.fA; }
    getSVGA(): number { return this.fA/255; }

        
// SETS VALUES
//--------------------------------------------------------------         
    
    setParam( param: number , value:number , min:number , max:number , isHSV:boolean ) {
        if ( min <= value && value <= max ) {  
            switch(param) {
                case color.kHue      : this.fHSB[0] = value; break;
                case color.kSat      : this.fHSB[1] = value; break;
                case color.kVal      : this.fHSB[2] = value; break;
                case color.kRed      : this.fRGB[0] = value; break;
                case color.kGreen    : this.fRGB[1] = value; break;
                case color.kBlue     : this.fRGB[2] = value; break;
                case color.kAlpha    : this.fA = value; break;
                default: console.log ("IColor setParam " + param + " not found");
            }
            this.fModified = true;
            if ( isHSV )	this.updateRGB();
            else			this.updateHSB();
        }
    }
    
    setRGB(val: Array<number>): void { 
    	this.fRGB = val.slice(0,3);
        if (val.length == 4) this.fA = val[3]; 
		this.updateHSB(); 
		this.fModified = true; 
    }
    setHSB(val: Array<number>): void  {
    	this.fHSB = val.slice(0,3); 
        if (val.length == 4) this.fA = val[3]; 
    	this.updateRGB(); 
    	this.fModified = true; 
    }
    
    setH(h:number): void 		{ this.setParam(color.kHue , Math.floor(h), 0, 360, true); }  
    setS(s:number): void 		{ this.setParam(color.kSat , Math.floor(s), 0, 100, true); }
    setV(b:number): void 		{ this.setParam(color.kVal , Math.floor(b), 0, 100, true); }

    setR(r:number): void 		{ this.setParam(color.kRed   , Math.floor(r), 0, 255, false); }
    setG(g:number): void 		{ this.setParam(color.kGreen , Math.floor(g), 0, 255, false); }
	setB(b:number): void 		{ this.setParam(color.kBlue  , Math.floor(b), 0, 255, false); }
	setA(a:number): void 		{ this.setParam(color.kAlpha , Math.floor(a), 0, 255, false); }	
		
	dA(a:number): void 			{ this.setA( this.getA() + Math.floor(a) ); }	
	dR(r:number): void 			{ this.setR( this.getR() + Math.floor(r) ); }
    dG(g:number): void 			{ this.setG( this.getG() + Math.floor(g) ); }
    dB(b:number): void 			{ this.setB( this.getB() + Math.floor(b) ); }
    dH(h:number): void 			{ this.setH( this.getH() + Math.floor(h) ); }
    dS(s:number): void 			{ this.setS( this.getS() + Math.floor(s) ); }
    dV(b:number): void 			{ this.setB( this.getB() + Math.floor(b) ); }
    		
// UPDATE COLORS
//--------------------------------------------------------------
    updateRGB(): void { this.hsb2rgb(); }
	updateHSB(): void { this.rgb2hsb(); }

  
// CONVERSIONS COLORS
//--------------------------------------------------------------    
   hsb2rgb():void {
        let H = this.fHSB[0]/360;
        let S = this.fHSB[1]/100;  
        let B = this.fHSB[2]/100; 
        
        let F:number; let M:number;  let N:number;  let K:number; let I:number; 
        if ( S == 0 ) {
            B = Math.floor(B);
            this.fRGB[0] = B;
            this.fRGB[1] = B;
            this.fRGB[2] = B;
        }       
        else {
            if (H >= 1) { H = 0.0; } 
            else { H *= 6; } 
            
            I = Math.floor(H);   
            F = H - I;     

            M = B * (1 - S);
            N = B * (1 - S * F);
            K = B * (1 - S * (1 - F));
           
            switch(I) {
                case 0: this.fRGB[0] = B; this.fRGB[1] = K; this.fRGB[2] = M; break;  
                case 1: this.fRGB[1] = B; this.fRGB[0] = N; this.fRGB[2] = M; break;
                case 2: this.fRGB[0] = M; this.fRGB[1] = B; this.fRGB[2] = K; break;
                case 3: this.fRGB[0] = M; this.fRGB[1] = N; this.fRGB[2] = B; break;
                case 4: this.fRGB[0] = K; this.fRGB[1] = M; this.fRGB[2] = B; break;
                case 5: this.fRGB[0] = B; this.fRGB[1] = M; this.fRGB[2] = N; break;
            }            
            this.fRGB[0] = Math.floor(this.fRGB[0] * 255); 
            this.fRGB[1] = Math.floor(this.fRGB[1] * 255); 
            this.fRGB[2] = Math.floor(this.fRGB[2] * 255);        
        }
    }

   //-----------------------------  
   rgb2hsb():void {
        let r = this.fRGB[0]/255;
        let g = this.fRGB[1]/255;
        let b = this.fRGB[2]/255;

        // Calculation of maxC and minC
        let maxC = Math.max(r, g, b);
        let minC = Math.min(r, g, b);

        // Calculation of delta
        let delta = maxC - minC;
        let H = 0; let S = 0; let B = maxC;

        if (delta == 0) { H=0; S=0;}
        else {
            S = delta / maxC;
            let dR = 60*(maxC - r)/delta + 180;
            let dG = 60*(maxC - g)/delta + 180;
            let dB = 60*(maxC - b)/delta + 180;
            
            if (r == maxC) { H = dB - dG; }
            else if (g == maxC) { H = 120 + dR - dB; }
            else { H = 240 + dG - dR; }            
        }

        if (H<0)	H+=360;
        if (H>=360) H-=360;

        this.fHSB[0] = Math.floor(H);
        this.fHSB[1] = Math.floor(S * 100);
        this.fHSB[2] = Math.floor(B * 100);   
    }   
}
