import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import tailwindcss from '@tailwindcss/vite'

// https://vite.dev/config/
export default defineConfig({
  server: {
    proxy: {
      '/random': {
        target: 'https://www.randomnumberapi.com',
        changeOrigin: true,
        rewrite: path => path.replace(/^\/random/, '')
      }
    }
  },
  plugins: [vue(), tailwindcss()],
})
