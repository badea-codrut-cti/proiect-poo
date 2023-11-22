import { Draggable } from '@shopify/draggable';
import type { Device } from '../../lib/types/device';
import { CursorType, type CanvasObject, ObjectTypes, type CanvasDevice } from '../../lib/types/entity';
import { handleObjectClick } from './element';
const icons: { [char: string]: {default: string}} = import.meta.glob('../../icons/toolbar/*.svg', {  eager: true });

const overlay = document.getElementById("overlay-canvas") as HTMLElement;

export class TopologyCanvas {
    private canvasBoundingBox: DOMRect;
    private canvasObjects: CanvasObject[] = [];
    private idCounter = 0;

    private mouseCoords = {x: 0, y: 0};
    private canvas: HTMLElement;

    private activeObject: CanvasObject | undefined;
    private activeCursorType: CursorType = CursorType.SELECT;

    private devices: Device[] = [];
    private deviceTarget: CanvasDevice | undefined; 

    private async handleElementCreation(element: HTMLElement, cursorType: CursorType) {
        let outType: ObjectTypes = ObjectTypes.DEFAULT;
        let data: Object = {};
    
        switch (cursorType) {
            case CursorType.TEXT: {
                element.innerHTML = `<textarea></textarea>`;
                outType = ObjectTypes.TEXTBOX;
                break;
            }
    
            case CursorType.DEVICE: {
                if (!this.deviceTarget)
                    return; // why?
    
                let icon = this.deviceTarget.deviceType ? this.deviceTarget.deviceType : this.devices[this.deviceTarget.deviceIndex];
                outType = ObjectTypes.DEVICE;
                
                element.innerHTML = `<div style="width: 2rem">${icons["../../icons/toolbar/" + icon + ".svg"].default}
                </div><span>${this.deviceTarget?.name}</span>`;
    
                let device = await window["wDeviceAdd"](this.deviceTarget);

                if (device.device)
                    this.deviceList.push(device.device);
    
                data = {
                    name: this.deviceTarget.name,
                    deviceIndex: device.device?.deviceIndex
                } as CanvasDevice;
                break;
            }
       }
    
       return {type: outType, data};
    }

    private static instance: TopologyCanvas;

    private moveElementToCursor(element: HTMLElement) {
        let x = Math.min(Math.max(this.mouseCoords.x-8, 0), this.canvasBoundingBox.right);
        let y = Math.min(Math.max(this.mouseCoords.y-8, 0), this.canvasBoundingBox.bottom);
        element.style.top = y + "px";
        element.style.left = x + "px";
    
        return {x, y};
    }

    private async createCanvasElement(canvas: HTMLElement) {
        let devElement = document.createElement("div");
        devElement.classList.add("draggable");
        devElement.setAttribute("objectId", `${this.idCounter}`);
        devElement.style.position = "absolute";
        let data = await this.handleElementCreation(devElement, this.activeCursorType) as {type: ObjectTypes, data: Object};
        let obj: CanvasObject = {
            id: this.idCounter,
            type: data.type,
            position: this.moveElementToCursor(devElement)
        }
        let cvObject = {...data?.data, ...obj};

        devElement.onclick = () => {
            handleObjectClick(devElement, cvObject);
        }
        this.canvasObjects.push(cvObject);
        
        canvas.appendChild(devElement);
        this.idCounter++;
    }

    constructor(canvas: HTMLElement) {
        TopologyCanvas.instance = this;
        this.canvas = canvas;
        let div = document.createElement("div");
        canvas.appendChild(div);

        let overlay = document.createElement("div");
        overlay.id = "overlay-canvas";
        canvas.appendChild(overlay);

        this.canvasBoundingBox = canvas.getBoundingClientRect();

        const draggable = new Draggable(div, {
            draggable: "div.draggable"
        });
    
        draggable.on('drag:start', (event) => {
            if (this.activeCursorType != CursorType.SELECT)
                event.cancel();

            Array.from(overlay.children).filter(el => el.getAttribute("type") == "portlist")
            .forEach(el => el.remove());

            this.activeObject = this.canvasObjects.find(el => el.id == parseInt(event.source.getAttribute("objectid") || "0"));
        });
    
        draggable.on('drag:stop', (event) => {
            if (!this.activeObject)
                return; //why?
            
            let index = this.canvasObjects.indexOf(this.activeObject) || 0;
            this.canvasObjects[index].position.x = Math.min(this.mouseCoords.x, this.canvasBoundingBox.right);
            this.canvasObjects[index].position.y = Math.min(this.mouseCoords.y, this.canvasBoundingBox.bottom);
            setTimeout(() => {
                let rawElement = Array.from(canvas.children)
                .find(child => child.getAttribute("objectid") == `${this.activeObject?.id}`) as HTMLElement;
                this.moveElementToCursor(rawElement);
            }, 10);
            
        });
    
        window.onmousemove = (event) => {
            this.mouseCoords = {
                x: event.pageX,
                y: event.pageY
            };
        }
    
        canvas.onclick = (event) => {
            switch (this.activeCursorType) {
                case CursorType.COPY:
                case CursorType.TEXT: 
                case CursorType.DEVICE: {
                    this.createCanvasElement(canvas);
                    break;
                }
            }
        }
    }

    static getInstance() {
        return TopologyCanvas.instance;
    }

    setCursorType(csType: CursorType) {
        this.activeCursorType = csType;
    }

    setDeviceTarget(cvDevice: CanvasDevice) {
        this.deviceTarget = cvDevice;
    }

    get cursorType() {
        return this.activeCursorType;
    }

    get deviceList() {
        return this.devices;
    }

    get canvasElement() {
        return this.canvas;
    }
}