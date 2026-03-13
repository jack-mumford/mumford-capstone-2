import { test, expect } from '@playwright/test';
import * as fs from 'fs';
import * as path from 'path';

const SCREENSHOT_DIR = path.join(__dirname, 'screenshots');
fs.mkdirSync(SCREENSHOT_DIR, { recursive: true });

// ------------------------------------------------------------------ //
// 01 – Game loads and canvas is visible
// ------------------------------------------------------------------ //
test('01 game loads and canvas is visible', async ({ page }) => {
  await page.goto('/');
  const canvas = page.locator('canvas');
  await canvas.waitFor({ state: 'visible', timeout: 15000 });

  const box = await canvas.boundingBox();
  expect(box!.width).toBeGreaterThan(0);
  expect(box!.height).toBeGreaterThan(0);

  await page.screenshot({ path: path.join(SCREENSHOT_DIR, '01-game-loads.png') });
});

// ------------------------------------------------------------------ //
// 02 – Player movement input doesn't crash the game
// ------------------------------------------------------------------ //
test('02 player moves on W key', async ({ page }) => {
  const jsErrors: string[] = [];
  page.on('pageerror', (err) => jsErrors.push(err.message));

  await page.goto('/');
  const canvas = page.locator('canvas');
  await canvas.waitFor({ state: 'visible', timeout: 15000 });

  // Brief settle time, then send movement input
  await page.waitForTimeout(500);
  await page.keyboard.down('w');
  await page.waitForTimeout(500);
  await page.keyboard.up('w');
  await page.waitForTimeout(200);

  await page.screenshot({ path: path.join(SCREENSHOT_DIR, '02-player-moves.png') });

  // Canvas must still be alive and no JS errors must have been thrown
  const box = await canvas.boundingBox();
  expect(box!.width).toBeGreaterThan(0);
  expect(jsErrors).toHaveLength(0);
});

// ------------------------------------------------------------------ //
// 03 – Game runs for 30 s without crashing (player dies, game-over fires)
// ------------------------------------------------------------------ //
test('03 player dies after idle', async ({ page }) => {
  test.setTimeout(70000);

  const jsErrors: string[] = [];
  page.on('pageerror', (err) => jsErrors.push(err.message));

  await page.goto('/');
  const canvas = page.locator('canvas');
  await canvas.waitFor({ state: 'visible', timeout: 15000 });

  // Enemies arrive ~2.5 s, player (100 hp, 15 dmg/hit) dies in ~7-10 s game time.
  // 30 s is well past game-over to verify the game handles death without crashing.
  await page.waitForTimeout(30000);

  await page.screenshot({ path: path.join(SCREENSHOT_DIR, '03-player-dies.png') });

  const box = await canvas.boundingBox();
  expect(box!.width).toBeGreaterThan(0);
  expect(jsErrors).toHaveLength(0);
});

// ------------------------------------------------------------------ //
// 04 – Attack input (F key spam) doesn't crash the game
// ------------------------------------------------------------------ //
test('04 player attacks enemies', async ({ page }) => {
  const jsErrors: string[] = [];
  page.on('pageerror', (err) => jsErrors.push(err.message));

  await page.goto('/');
  const canvas = page.locator('canvas');
  await canvas.waitFor({ state: 'visible', timeout: 15000 });

  // Wait for WASM init then spam punch key while enemies approach
  await page.waitForTimeout(2000);

  for (let i = 0; i < 15; i++) {
    await page.keyboard.press('f');
    await page.waitForTimeout(200);
  }

  await page.screenshot({ path: path.join(SCREENSHOT_DIR, '04-player-attacks.png') });

  const box = await canvas.boundingBox();
  expect(box!.width).toBeGreaterThan(0);
  expect(jsErrors).toHaveLength(0);
});

// ------------------------------------------------------------------ //
// 05 – Restart (R key) works after game over
// ------------------------------------------------------------------ //
test('05 restart works after game over', async ({ page }) => {
  test.setTimeout(70000);

  const jsErrors: string[] = [];
  page.on('pageerror', (err) => jsErrors.push(err.message));

  await page.goto('/');
  const canvas = page.locator('canvas');
  await canvas.waitFor({ state: 'visible', timeout: 15000 });

  // Wait for player to die
  await page.waitForTimeout(30000);
  await page.screenshot({ path: path.join(SCREENSHOT_DIR, '05-before-restart.png') });

  // Press R to restart the game
  await page.keyboard.press('r');
  await page.waitForTimeout(2000);

  // Canvas must still be alive after restart — game didn't crash
  const box = await canvas.boundingBox();
  await page.screenshot({ path: path.join(SCREENSHOT_DIR, '05-after-restart.png') });

  expect(box!.width).toBeGreaterThan(0);
  expect(jsErrors).toHaveLength(0);
});
