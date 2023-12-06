<script lang="ts">
    import type { Device, EthernetInterface } from "../../../lib/types/device";
    import { pseudoDevice } from "../../../lib/workspace";
    import IpConfig from "./apps/IPConfig.svelte";
    let device: Device = pseudoDevice;
    window["wGetDeviceData"]().then(out => {
        device = out;
    });
</script>

<div class="flex items-center justify-start space-x-4">
    <button class="px-2 py-1 text-xs">Physical</button>
    <button class="px-2 py-1 text-xs">Desktop</button>
</div>

<IpConfig interfaces={device.interfaces}/>