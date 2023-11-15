import { defineConfig } from 'vite';
import { svelte } from '@sveltejs/vite-plugin-svelte';
import { viteSingleFile } from 'vite-plugin-singlefile';
import svg from 'vite-plugin-svgo';
import config from './svgo.config.js';

export default defineConfig({
  plugins: [
    svelte(), 
    svg(config),
    viteSingleFile()
  ]
})
