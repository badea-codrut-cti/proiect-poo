<script lang="ts">
    import { onMount } from "svelte";
    import { TopologyCanvas } from "./canvas";
    import { CursorType } from "../../lib/types/entity";

    let canvas: HTMLElement;
    onMount(() => {
        new TopologyCanvas(canvas);
    });
</script>

<div bind:this={canvas} class="topology-canvas"></div>

<style>
    .topology-canvas {
        width: 100vw; 
        height: 80vh;
        overflow: auto;
    }
</style>

