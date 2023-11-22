import type { Device } from "../../lib/types/device";
import { CursorType, type CanvasObject, type CanvasDevice } from "../../lib/types/entity";
import { TopologyCanvas } from "./canvas";
const icons: { [char: string]: {default: string}} = import.meta.glob('../../icons/toolbar/ports/*.svg', {  eager: true });

export function handleObjectClick(rawObject: HTMLElement, cvObject: CanvasObject) {
    if (TopologyCanvas.getInstance().cursorType != CursorType.CONNECT)
        return;

    showInterfacePrompt(rawObject, cvObject as unknown as CanvasDevice);
}

export function showInterfacePrompt(rawDeviceElement: HTMLElement, cvDevice: CanvasDevice) {
    const overlay = document.getElementById("overlay-canvas") as HTMLElement;

    let maxIndex = 0;
    let device = TopologyCanvas.getInstance().deviceList[cvDevice.deviceIndex as number];
    let rawPrompt = device.interfaces.sort((a,b) => a.maxSpeed - b.maxSpeed).map((port, index) => {
        let label = "";
        switch (port.maxSpeed) {
            case 1000: label = "Gigabit"; break;
            case 100: label = "Fast"; break;
        }
        if (index != 0 && port.maxSpeed != device.interfaces[index-1].maxSpeed) 
            maxIndex = index;
        label += `Ethernet${index-maxIndex}/0`;
        return `<li>
                    <div style="display: inline-block; width: 1rem;">${icons['../../icons/toolbar/ports/ethernet.svg'].default}</div>
                    <span style="font-size: 1rem">${label}</span>
                </li>`;
    });
    
    let child = document.createElement("ul");
    child.setAttribute("type", "portlist");

    child.style.overflow = "auto";
    child.style.width = "auto";
    child.style.height = "6rem";
    child.style.position = "absolute";
    child.style.backgroundColor = "#FFF";
    child.style.top = (cvDevice.position.y - 8) + "px";
    child.style.left = (cvDevice.position.x + 64) + "px";
    child.style.zIndex = "1";
    child.innerHTML = rawPrompt.join("");

    Array.from(overlay.children).filter(el => el.getAttribute("type") == "portlist")
    .forEach(el => el.remove());

    overlay.appendChild(child);
}