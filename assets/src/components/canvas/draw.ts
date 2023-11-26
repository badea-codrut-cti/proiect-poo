import { jsPlumb } from "jsplumb";

export function connectElements(div1: HTMLElement, div2: HTMLElement) { 
    jsPlumb.connect({
        source: div1,
        target: div2,
        connector: ["Straight"],
        paintStyle: { stroke: '#000', strokeWidth: 2 },
        connectorStyle: { stroke: '#000', strokeWidth: 2 },
        anchorStyle: { stroke: '#000', strokeWidth: 2 },
        endpoint: "Blank",
        anchor: ["Center", "Center"],
        detachable: false // Make the line non-detachable
    });
      
}