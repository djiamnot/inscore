///<reference path="../controller/THandlersPrototypes.ts"/>
///<reference path="../lib/OSCAddress.ts"/>
///<reference path="IObject.ts"/>
///<reference path="Constants.ts"/>
///<reference path="IProxy.ts"/>
///<reference path="IApplStaticNodes.ts"/>


class IAppl extends IObject {

	protected fReceivedMsgs: number;
	protected fRate: number;
	protected fCurrentTime: number;
	
	constructor() {
		super('ITL');
		this.fTypeString = kApplType;
		this.fReceivedMsgs = 0;
		this.fRate = kDefaultRate;
	}

	createStaticNodes(): void {
		let log = new IApplLog("log", this);
		this.addChild(log);
	}

    setHandlers(): void {
 	    this.eventAble();
        this.fMsgHandlerMap[kget_SetMethod] 		= new TMethodHandler( (msg: IMessage): eMsgStatus => { return this.get(msg); } );
        this.fMsgHandlerMap[krate_GetSetMethod] 	= new TMsgHandlerNum( (n: number): void => { this.setRate(n); } );
		this.fGetMsgHandlerMap[krate_GetSetMethod] 	= new TGetMsgHandlerNum( (): number => { return this.getRate(); } );
	}

	getAppl(): IObject { return this; }
	getSet(): IMessage { let msg: IMessage; return msg; }
	getOSCAddress(): string { return "/ITL"; }
	getRate(): number { return this.fRate; }
	setRate(rate: number): void { this.fRate = rate; }

	processMsg(address: string, addressTail: string, msg: IMessage): eMsgStatus {
		this.fReceivedMsgs++;

		let status = eMsgStatus.kBadAddress;
		let head = address;
		let tail = addressTail;

		/*
				SIMessage msg = IMessage::create (*imsg);
				TAliasesMap::const_iterator i = fAliases.find(imsg->address());
				if (i != fAliases.end()) {
					msg->setAddress (i->second.first);
					if (i->second.second.size()) 
						msg->setMessage(i->second.second);
					head = OSCAddress.addressFirst(i->second.first);
					tail = OSCAddress.addressTail(i->second.first);
				}
		*/

		if (tail.length) {			// application is not the final destination of the message
			status = super.processMsg(head, tail, msg);
		}

		else if (this.match(head)) {		// the message is for the application itself
			status = this.execute(msg);
			if (status & eMsgStatus.kProcessed)
				this.setState(eObjState.kModified);
		}
		return status;
	}
}