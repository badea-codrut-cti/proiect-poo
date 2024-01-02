<script lang="ts">
    import type { FormEventHandler } from "svelte/elements";
    import type { Device } from "../../../../lib/types/device";
    import { IPConfig } from "./cmd/commands";

    export let dev: Device;

    let tInput: any, tOutput: any;

    function handleTerminalInput() {
        if (tInput.value[tInput.value.length-1] != '\n')
            return;

        tOutput.innerText += new IPConfig().handleCommand([], dev);
    }
</script>

<div class="terminal-window">
    <span bind:this={tOutput} class="block">
        
    </span>
    <span class="inline-block">C:\></span>
    <textarea class="invisible-textarea" bind:this={tInput} on:input={handleTerminalInput}></textarea>
</div>

<style>
    .terminal-window {
        font-family: 'Courier New', Courier, monospace;
        background-color: #000;
        color: #FFF;
    }

    .invisible-textarea {
        background: transparent;
        display: inline-block;
        border: none;
        color: #FFF;
        outline: none;
        resize: none;
        width: auto;
        overflow: auto;
        height: 1rem;
    }
</style>