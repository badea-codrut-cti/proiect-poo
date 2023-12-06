<script lang="ts">
    const icons: { [char: string]: {default: string}} = import.meta.glob('../../../icons/pc/**', {  eager: true });
</script>

<div class="window-bg">
    <button>
        <div class="menu-icon">
            {@html icons["../../../icons/pc/ip-config.svg"].default}
        </div>
        IP Configuration
    </button>
    <button>
        <div class="menu-icon">
            {@html icons["../../../icons/pc/terminal.svg"].default}
        </div>
        Terminal
    </button>
    <button>Command prompt</button>
    <button>Web browser</button>
    <button>EMail</button>
</div>


<style>
    .window-bg {
        background-color: rgb(13, 39, 77);
        color: #FFF;
    }
    button {
        padding-left: 2rem;
        padding-right: 2rem;
    }
    .menu-icon {
        background-color: rgb(83, 190, 212);
        width: 8rem;
        height: 8rem;
    }
</style>