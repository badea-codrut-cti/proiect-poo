<script lang="ts">
    import { AppType } from "../menu";

    export let currentApp: AppType;
    const icons: { [char: string]: {default: string}} = import.meta.glob('../../../../icons/pc/**', {  eager: true });
    const apps = [
        {
            label: "IP Configuration",
            icon: "ip-config",
            app: AppType.IPCONFIG
        },
        {
            label: "Terminal",
            icon: "terminal",
            app: AppType.TERMINAL
        },
        {
            label: "Command prompt",
            icon: "cmd",
            app: AppType.CMD
        }
    ];

    console.log(icons);
</script>

<div class="window-bg">
    {#each apps as app}
        <button class="app-button" on:click={() => {
            currentApp = app.app;
        }}>
            <div class="menu-icon">
                {@html icons["../../../../icons/pc/" + app.icon + ".svg"].default}
            </div>
            {app.label}
        </button>
    {/each}
</div>


<style>
    .window-bg {
        background-color: rgb(13, 39, 77);
        color: #FFF;
    }
    .app-button {
        padding-left: 2rem;
        padding-right: 2rem;
    }
    .menu-icon {
        background-color: rgb(83, 190, 212);
        width: 8rem;
        height: 8rem;
    }
</style>