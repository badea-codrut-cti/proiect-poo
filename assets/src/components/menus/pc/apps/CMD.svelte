<script lang="ts">
    import type { Device } from "../../../../lib/types/device";
    import { IPConfig, Ping } from "./cmd/commands";

    export let dev: Device;

    let tInput: any, tOutput: any;

    const commands = [new IPConfig(), new Ping()];

    async function handleCommandInput() {
        let params = `${tInput.value}`.substring(0, `${tInput.value}`.length-1).split(" ");

        let cmds = commands.filter(command => command.name.startsWith(params[0]));

        if (cmds.length == 0)
            tOutput.innerText += "Command does not exist.\n";

        if (cmds.length > 1)
            tOutput.innerText += "Command is ambiguous.\n";

        tOutput.innerText += await cmds[0].handleCommand(params, dev);
    }

    function handleTerminalInput() {
        if (tInput.value[tInput.value.length-1] != '\n')
            return;
        
        handleCommandInput();

        tInput.value = "";
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