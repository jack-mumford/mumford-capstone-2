import { defineConfig } from '@playwright/test';

export default defineConfig({
  testDir: '.',
  timeout: 30000,
  outputDir: 'test-results',
  use: {
    baseURL: 'http://localhost:8080',
    headless: true,
    launchOptions: {
      args: [
        '--enable-webgl',
        '--use-gl=angle',
        '--use-angle=swiftshader',
        '--ignore-gpu-blocklist',
        '--no-sandbox',
        '--disable-setuid-sandbox',
      ],
    },
  },
});
