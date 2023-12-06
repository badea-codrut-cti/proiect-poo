import { Devices, type Device, type EthernetInterface } from "../../lib/types/device";
import { CursorType, type CanvasObject, type CanvasDevice } from "../../lib/types/entity";
import { TopologyCanvas } from "./canvas";
const icons: { [char: string]: {default: string}} = import.meta.glob('../../icons/toolbar/ports/*.svg', {  eager: true });

export function handleObjectClick(rawObject: HTMLElement, cvObject: CanvasObject) {
    switch (TopologyCanvas.getInstance().cursorType) {
        case CursorType.CONNECT: {
            showInterfacePrompt(rawObject, cvObject as unknown as CanvasDevice);
            break;
        }
        case CursorType.DEFAULT: {
            window.wOpenDeviceSettings({deviceIndex: cvObject.deviceIndex, deviceType: Devices.DESKTOP});
            break;
        }
    }
}

let activePort: {objectIndex: number, deviceIndex: number, interfaceIndex: number} | undefined;

function handleConnectClick(objectIndex: number, deviceIndex: number, interfaceIndex: number) {
    let newPort = {objectIndex, deviceIndex, interfaceIndex};
    if (activePort) {
        window.wDeviceConnect(activePort, newPort);
        let instance = TopologyCanvas.getInstance();
        //connectElements(instance.getRawElement(objectIndex) as HTMLElement, 
            //instance.getRawElement(activePort.objectIndex) as HTMLElement);
        activePort = undefined;
        TopologyCanvas.getInstance().setCursorType(CursorType.SELECT);
    }
    else 
        activePort = newPort;
}

export function showInterfacePrompt(rawDeviceElement: HTMLElement, cvDevice: CanvasDevice) {
    const overlay = document.getElementById("overlay-canvas") as HTMLElement;

    let maxIndex = 0;
    let device = TopologyCanvas.getInstance().deviceList[cvDevice.deviceIndex as number];

    let interfaceUL = document.createElement("ul");
    interfaceUL.setAttribute("type", "portlist");

    interfaceUL.style.overflow = "auto";
    interfaceUL.style.width = "auto";
    interfaceUL.style.height = "6rem";
    interfaceUL.style.position = "absolute";
    interfaceUL.style.backgroundColor = "#FFF";
    interfaceUL.style.top = (cvDevice.position.y - 8) + "px";
    interfaceUL.style.left = (cvDevice.position.x + 64) + "px";
    interfaceUL.style.zIndex = "1";

    device.interfaces.sort((a,b) => a.maxSpeed - b.maxSpeed).forEach((port, index) => {
        let label = "";
        switch (port.maxSpeed) {
            case 1000: label = "Gigabit"; break;
            case 100: label = "Fast"; break;
        }
        if (index != 0 && port.maxSpeed != device.interfaces[index-1].maxSpeed) 
            maxIndex = index;
        label += `Ethernet${index-maxIndex}/0`;
        let interfaceLI = document.createElement("li");
        interfaceLI.onclick = () => {
            handleConnectClick(cvDevice.id, cvDevice.deviceIndex, device.interfaces.indexOf(port));
            interfaceUL.remove();
        }
        interfaceLI.innerHTML = `<div style="display: inline-block; width: 1rem;">${icons['../../icons/toolbar/ports/ethernet.svg'].default}</div>
        <span style="font-size: 1rem">${label}</span>`;
        interfaceUL.appendChild(interfaceLI);
    });

    Array.from(overlay.children).filter(el => el.getAttribute("type") == "portlist")
    .forEach(el => el.remove());

    overlay.appendChild(interfaceUL);
}