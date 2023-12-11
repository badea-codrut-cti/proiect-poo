<script lang="ts">
    import type { EthernetInterface } from "../../../../lib/types/device";

    export let interfaces: EthernetInterface[];

    let parsedInterfaces: {name: string, index: number}[] = [];
    $: {
        let maxIndex = 0;

        parsedInterfaces = interfaces.sort((a,b) => a.maxSpeed - b.maxSpeed).map((port, index) => {
            let label = "";
            switch (port.maxSpeed) {
                case 1000: label = "Gigabit"; break;
                case 100: label = "Fast"; break;
            }

            if (index != 0 && port.maxSpeed != interfaces[index-1].maxSpeed) 
                maxIndex = index;

            label += `Ethernet${index-maxIndex}/0`;
            return {
                name: label,
                index: interfaces.indexOf(port)
            }
        });
    }
</script>

<div class="w-64">
    <select class="block appearance-none w-full bg-white border border-gray-400">
        {#each parsedInterfaces as intf}
            <option value={intf.index}>{intf.name}</option>
        {/each}
    </select>
</div>

<h1>IP Configuration</h1>

<div>
    <div>
        <div class="inline-block w-32">IPv4 Address</div>
        <textarea class="inline-block resize-none w-32 h-6 text-xs"></textarea>
    </div>
    <div>
        <div class="inline-block w-32">Subnet Mask</div>
        <textarea class="inline-block resize-none w-32 h-6 text-xs"></textarea>
    </div>
    <div>
        <div class="inline-block w-32">Default Gateway</div>
        <textarea class="inline-block resize-none w-32 h-6 text-xs"></textarea>
    </div>
    <div>
        <div class="inline-block w-32">DNS Server</div>
        <textarea class="inline-block resize-none w-32 h-6 text-xs"></textarea>
    </div>
</div>