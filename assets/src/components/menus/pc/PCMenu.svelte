<script lang="ts">
    import type { Device, EthernetInterface } from "../../../lib/types/device";
    import { pseudoDevice } from "../../../lib/workspace";
    import Cmd from "./apps/CMD.svelte";
    import Desktop from "./apps/Desktop.svelte";
    import IpConfig from "./apps/IPConfig.svelte";
    import { AppType } from "./menu";
    let device: Device = pseudoDevice;
    window["wGetDeviceData"]().then(out => {
        device = out;
    });

    window.wDeviceUpdateListener = () => {
        window["wGetDeviceData"]().then(out => {
            device = out;
        });
    }
    
    const tabs = [
        {
            label: "Physical",
            app: AppType.PHYSICAL
        },
        {
            label: "Desktop",
            app: AppType.DESKTOP
        }
    ];

    let currentApp: AppType = AppType.DESKTOP;
</script>

<div class="settings-menu">
    <div class="flex items-center justify-start space-x-4">
        {#each tabs as tab}
            <button class="px-2 py-1 text-xs" on:click={() => {currentApp = tab.app}}>{tab.label}</button>
        {/each}
    </div>
    
    {#if currentApp == AppType.DESKTOP}
        <Desktop bind:currentApp={currentApp}/>
    {/if}
    {#if currentApp == AppType.IPCONFIG}
        <IpConfig interfaces={device.interfaces}/>
    {/if}
    {#if currentApp == AppType.CMD}
        <Cmd/>
    {/if}
</div>


<style>
    .settings-menu {
        background-color: #ededed;
    }
</style>