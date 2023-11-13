<script lang="ts">
    import type { UIDeviceType } from "../../types/device.ts";
    const icons = import.meta.glob('../../icons/*.svg', { as: 'raw', eager: true });
    const deviceTypes: UIDeviceType[] = [
        {
            label: "Network Devices",
            icon: "router",
            devices: [
                {
                    label: "Router",
                    icon: "router"
                },
                {
                    label: "Switch",
                    icon: "l2switch"
                },
                {
                    label: "Wireless<br>Router",
                    icon: "wireless-router"
                }
            ]
        },
        {
            label: "End Devices",
            icon: "pc",
            devices: [
                {
                    label: "PC",
                    icon: "pc"
                },
                {
                    label: "Laptop",
                    icon: "laptop"
                }
            ]
        }
    ];

    let active = deviceTypes[0];
</script>

<div class="flex">
    <div class="flex overflow-x-auto">
        <div class="block">
            {#each deviceTypes as deviceType}
                <button class="items-center p-2 inline-block" on:click={() => {active = deviceType}}>
                    <div class="toolbar-icon">
                        {@html icons["../../icons/" + deviceType.icon + ".svg"]}
                    </div>
                </button>
            {/each}
            <span class="border-black border block text-center">{active.label}</span>
        </div>
    </div>
    <div class="flex overflow-x-auto">
        {#each active.devices as device}
          <button class="flex flex-col items-center p-2">
            <div class="toolbar-icon">
                {@html icons["../../icons/" + device.icon + ".svg"]}
            </div>
            <span>{@html device.label}</span>
          </button>
        {/each}
    </div>
</div>

<style>
    .toolbar-icon {
        width: 2rem;
        height: 2rem;
    }
</style>