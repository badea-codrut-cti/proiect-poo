<script lang="ts">
    import { onMount } from "svelte";
    import { Draggable } from '@shopify/draggable';
    import { CursorType } from "./canvas";

    let canvas: HTMLElement;
    let cursorType: CursorType = CursorType.SELECT;
    let device: Device;
    let positions = {};

    onMount(() => {
        const draggable = new Draggable(canvas, {
            draggable: "div.draggable"
        })

        /*
        draggable.on('drag:start', (event) => {
            let id = event.data.source.id;
            let position = positions[id];

            if (position) {
                event.data.source.style.transform = `translate3d(${position.x}px, ${position.y}px, 0)`;
            }
        });

        draggable.on('drag:stop', (event) => {
            let id = event.data.source.id;
            positions[id] = event.data.sensorEvent.data.pointer;
        });*/

        window["changeCursorType"] = (cursorType_: CursorType) => {
            cursorType = cursorType_;
        };
    });
</script>

<div bind:this={canvas} style="width: 100vw; height: 80vh;"></div>