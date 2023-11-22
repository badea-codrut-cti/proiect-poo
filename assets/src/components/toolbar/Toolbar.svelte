<script lang="ts">
    import { CursorType } from "../../lib/types/entity.js";
    import { TopologyCanvas } from "../canvas/canvas.js";
    import { connectionTypes, deviceTypes, type UIDeviceCategory } from "./toolbar.js";
    const icons: { [char: string]: {default: string}} = import.meta.glob('../../icons/toolbar/**', {  eager: true });
    let shouldShowConnections = false;
    let active = deviceTypes[0];

    function showConnections() {
        active = {
            icon: "",
            label: "Connections",
            devices: []
        };
        shouldShowConnections = true;
    }

    function setActiveDeviceType(device: UIDeviceCategory) {
        active = device;
        shouldShowConnections = false;
    }
</script>

<div class="flex">
    <div class="flex overflow-x-auto">
        <div class="block">
            {#each deviceTypes as deviceType}
                <button class="items-center p-2 inline-block" on:click={() => {setActiveDeviceType(deviceType)}}>
                    <div class="toolbar-icon">
                        {@html icons["../../icons/toolbar/" + deviceType.icon + ".svg"].default}
                    </div>
                </button>
            {/each}
            <button class="items-center p-2 inline-block" on:click={() => {showConnections()}}>
                <div class="toolbar-icon">
                    {@html icons["../../icons/toolbar/connection.svg"].default}
                </div>
            </button>
            <span class="border-black border block text-center">{active.label}</span>
        </div>
    </div>
    <div class="flex overflow-x-auto">
        {#each active.devices as device}
          <button class="flex flex-col items-center p-2" on:click={() => {
                TopologyCanvas.getInstance().setCursorType(CursorType.DEVICE);
                TopologyCanvas.getInstance().setDeviceTarget({
                    name: device.label,
                    deviceType: device.device
                });
            }}>
            <div class="toolbar-icon">
                {@html icons["../../icons/toolbar/" + device.icon + ".svg"].default}
            </div>
            <span>{@html device.label}</span>
          </button>
        {/each}
        {#if shouldShowConnections}
          {#each connectionTypes as conType}
            <button class="flex flex-col items-center p-2" on:click={() => {
                TopologyCanvas.getInstance().setCursorType(CursorType.CONNECT);
            }}>
                <div class="toolbar-icon">
                    {@html icons["../../icons/toolbar/connections/" + conType.icon + ".svg"].default}
                </div>
            </button>
          {/each}
        {/if}
    </div>
</div>

<style>
    .toolbar-icon {
        width: 2rem;
        height: 2rem;
    }
</style>