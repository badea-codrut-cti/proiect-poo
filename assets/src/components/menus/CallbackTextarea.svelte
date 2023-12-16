<script lang="ts">
    export let callback: (val: string) => void;
    export let label: string, text: string;
  
    let timeout: number;

    let txt: string = text;

    $: txt = text;
  
    function handleInput() {
      clearTimeout(timeout);
      timeout = window.setTimeout(() => {
        text = txt;
        callback(text)
    }, 500);
    }
  </script>
  
<div class="inline-block w-32">{label}</div>
<textarea 
class="inline-block resize-none w-32 h-6 text-xs" 
bind:value={txt} on:input={handleInput}></textarea>