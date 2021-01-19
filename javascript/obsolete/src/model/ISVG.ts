///<reference path="IObject.ts"/>

class ISVG extends IObject {
 
    protected fSvg: string; 
    
    constructor(name: string, parent: IObject) {
        super(name, parent);
        this.fTypeString = kSvgType;
        super.setHandlers(); 
    } 
    
    setSvg(svg: string): void { this.fSvg = svg; }
    
    getSvg(): string { return this.fSvg; } 
    
    set(msg:IMessage): eMsgStatus {
        let status = super.set(msg);
        if (status & (eMsgStatus.kProcessed + eMsgStatus.kProcessedNoChange)) return status;
        
        let n = msg.size();
        if (n == 3) {
            let svg = msg.paramStr(2);
            if (!svg.correct) { return eMsgStatus.kBadParameters; }
            
            if (svg.value != this.getSvg()) {
                this.setSvg(svg.value);
                this.newData(true);                
                status = eMsgStatus.kProcessed;
            }
            else status = eMsgStatus.kProcessedNoChange;
        }
        else status = eMsgStatus.kBadParameters;
        return status;                
    }
    
    getSet(): IMessage	{ 
    	return new IMessage(this.getOSCAddress(), [kset_SetMethod, this.fTypeString, this.getSvg()]); 
    }
}    