
///<reference types="@grame/libfaust"/>

declare var FaustModule : any;

interface Window {
	webkitAudioContext ?: typeof AudioContext;
}

//----------------------------------------------------------------------------
// the libMusicXML interface
//----------------------------------------------------------------------------
class faust {
	private fModule: Faust.FaustModule;
	private fLib: Faust.LibFaust;

    async initialise():Promise<any> { 
        return new Promise( (success: any, failure: any) => {
            FaustModule().then ( (module: any) => {
				this.fModule = module;
				this.fLib = Faust.createLibFaust (module);
                success ( this ); 
            });
		});
	}
	
	version () : string			{ return this.fLib.version(); }
	module () 					{ return this.fModule; }
	lib () 						{ return this.fLib; }

	async test() {
		let audioCtx = new (window.AudioContext || window.webkitAudioContext)();

		Faust.compileAudioNode (audioCtx, this.fModule, "process=+;", null, 0, 0).then ( (node => {
			console.log ("test function: " + node.setParamValue ("/toto", 1))
		})
		);
	}
}
