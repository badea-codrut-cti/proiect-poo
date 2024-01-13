<script lang="ts">
    import type { EthernetInterface } from "../../../../lib/types/device";
    import CallbackTextarea from "../../CallbackTextarea.svelte";

    export let interfaces: EthernetInterface[];

    let parsedInterfaces: {name: string, index: number}[] = [];
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

    let ipAddr: string, subnetMask: string, defaultGateway: string;

    let activeIndex = {
        inProp: 0,

        get prop() {
            return this.inProp;
        },

        set prop(val) {
            ipAddr = interfaces[val].ip.address;
            subnetMask = interfaces[val].ip.subnetMask.dotNotation;
            this.inProp = val;
        }
    }

    activeIndex.prop = 0;

    function handleSubnetMaskChange(val: string) {
        window.wChangeDeviceSettings({
            editMode: "interface",
            interfaceIndex: activeIndex.prop,
            ip: {
                address: ipAddr,
                subnetMask: {
                    dotNotation: val
                }
            }
        });
    }

    function handleIPChange(val: string) {
        if (val.split(".").length != 4) 
            return;
        

        window.wChangeDeviceSettings({
            editMode: "interface",
            interfaceIndex: activeIndex.prop,
            ip: {
                address: val
            }
        });
    }

    function handleDefaultGatewayChange(val: string) {
        if (val.split(".").length != 4) 
            return;

            window.wChangeDeviceSettings({
            editMode: "interface",
            interfaceIndex: activeIndex.prop,
            ip: {
                defaultGateway: val
            }
        });
    }
</script>

<div class="w-64">
    <select bind:value={activeIndex.prop} class="block appearance-none w-full bg-white border border-gray-400">
        {#each parsedInterfaces as intf}
            <option value={intf.index}>{intf.name}</option>
        {/each}
    </select>
</div>

<h1>IP Configuration</h1>

<div>
    <div>
        <CallbackTextarea label={"IPv4 Address"} bind:text={ipAddr} callback={handleIPChange}/>
    </div>
    <div>
        <CallbackTextarea label={"Subnet Mask"} bind:text={subnetMask} callback={handleSubnetMaskChange}/>
    </div>
    <div>
        <CallbackTextarea label={"Default Gateway"} bind:text={defaultGateway} callback={handleDefaultGatewayChange}/>
    </div>
    <div>
        <div class="inline-block w-32">DNS Server</div>
        <textarea class="inline-block resize-none w-32 h-6 text-xs"></textarea>
    </div>
</div>