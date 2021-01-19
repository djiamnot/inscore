
///<reference path="IMessage.ts"/>
///<reference path="../globals.ts"/>
///<reference path="../model/IAppl.ts"/>
///<reference path="../events/documentEvents.ts"/>
///<reference path="../view/ViewUpdater.ts"/>
///<reference path="../model/ModelUpdater.ts"/>
///<reference path="../globals.ts"/>

interface TTimerTask  	{ (): void; }

class IGlue { 
	private   fCurrentTime: number;

	protected fAppl: IAppl;
	protected fTimer: number;		// this is to catch multiple defs in nodes and in browser contexts
	private   fStack: Array<IMessage>;
	private   fModelUpdater: ModelUpdater;

    constructor() 			{ 
    	this.fAppl = new IAppl();
		this.fStack = new Array<IMessage>();
		this.fModelUpdater = new ModelUpdater(this.fAppl);
		this.fCurrentTime = 0;
    }

	getStack(): Array<IMessage> 	{ return this.fStack; }
	popStack(): IMessage 			{ return this.fStack.shift(); }
    pushStack(msg: IMessage): void 	{ this.fStack.push(msg); }

    initEventHandlers(): void {
		window.addEventListener("resize", inscore_resizeDocument);
    }
    
    start(scene: string, position: string): void {
    	let target = "/ITL/" + scene;
    	INScore.postMessage (target, ["new"]);
    	INScore.postMessage (target, ["position", position]);	
		if (gCreateView)
	    	this.fTimer = setTimeout (this._timetask(), this.fAppl.getRate()) ;		
		window.onbeforeunload = (): void => { INScore.postMessage ( "/ITL/log", ["show", 0]); };
    }

    stop(): void {
    	INScore.postMessage ("/ITL", ["quit"]);
    	clearTimeout (this.fTimer) ;		
    }

    getRoot(): IAppl 			{ return this.fAppl; }

	timetask() : void {
		let d= new Date();
		let now = d.getTime();
		if (this.fCurrentTime)
			gINScoreRealRate = now - this.fCurrentTime;
		this.fCurrentTime = now;
		let stack = this.fStack;
		this.fStack = [];
	try {
		this.fModelUpdater.update (stack);
		ViewUpdater.update (this.fAppl);
		IObject.timeTaskCleanup (this.fAppl);
	}
	catch (e) { 
		console.log ("Warning: " + e);
	}
    	this.fTimer = setTimeout (this._timetask(), this.fAppl.getRate()) ;		
	}
    _timetask()	: TTimerTask 			{ return () => this.timetask(); };
}
