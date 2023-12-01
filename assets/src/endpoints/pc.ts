import PCMenu from "./components/menus/pc/PCMenu.svelte";
import "./lib/workspace";

const app = new PCMenu({
  target: document.getElementById('app'),
})

export default app
