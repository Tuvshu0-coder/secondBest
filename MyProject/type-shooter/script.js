/* script.js — core game logic for the typing shooter
   Comments are added to explain each section (assignment requires comments).
*/

/* -------------------------
   Configuration & assets
   ------------------------- */
const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');

const W = 900, H = 600;
canvas.width = W;
canvas.height = H;

// Game state
let enemies = [];
let words = [
  'forest', 'mirror', 'puzzle', 'candle', 'button', 'planet', 'rocket', 'silver', 'stream', 'castle',
  'market', 'letter', 'orange', 'school', 'bridge', 'corner', 'window', 'garden', 'island', 'pirate',
  'pocket', 'button', 'journey', 'signal', 'temple', 'shadow', 'flight', 'circle', 'forest', 'gather',
  'danger', 'cactus', 'pattern', 'monster', 'hidden', 'bright', 'kingdom', 'laptop', 'castle', 'needle',
  'thunder', 'whistle', 'picture', 'diamond', 'mystery', 'pirate', 'rocket', 'captain', 'diamond', 'fishing', 'bumaa', 'suhbat', 'c'
];

let activeTyped = '';
let score = 0;
let gameOver = false;
let spawnInterval = 2000;
let lastSpawn = 0;
let difficultyTimer = 0;
let gameStarted = false; // new: tracks if Start button pressed

// Assets
const playerImg = new Image();
playerImg.src = 'assets/images/player_ship.png';
const enemyImg = new Image();
enemyImg.src = 'assets/images/enemy_ship.png';

const bgm = document.getElementById('bgm');
const musicToggle = document.getElementById('musicToggle');

// Start menu elements
const startMenu = document.getElementById('startMenu');
const startBtn = document.getElementById('startBtn');

/* -------------------------
   Event handlers
   ------------------------- */
// Music toggle
musicToggle.addEventListener('click', () => {
  if (bgm.src) {
    if (bgm.paused) { bgm.play().catch(()=>{}); musicToggle.textContent = 'Mute'; }
    else { bgm.pause(); musicToggle.textContent = 'Unmute'; }
  } else {
    alert('Add a music file to assets/music/bgm.mp3');
  }
});

// Start button
startBtn.addEventListener('click', () => {
  gameStarted = true;
  startMenu.style.display = 'none';

  if (bgm.src) {
    bgm.volume = 0.6;
    bgm.play().catch(()=>{});
    musicToggle.textContent = 'Mute';
  }
});

// Typing input
document.addEventListener('keydown', (e) => {
  if (!gameStarted || gameOver) return;
  if (e.key === 'Backspace') {
    activeTyped = activeTyped.slice(0, -1);
    updateTypedVisual();
    return;
  }
  if (e.key.length === 1) {
    const ch = e.key.toLowerCase();
    if (/^[a-z]$/.test(ch)) {
      activeTyped += ch;
      checkTypedAgainstEnemies();
      updateTypedVisual();
    }
  }
});

/* -------------------------
   Utilities
   ------------------------- */
function randInt(min, max) { return Math.floor(Math.random() * (max - min + 1)) + min; }

/* -------------------------
   Enemy creation
   ------------------------- */
function spawnEnemy() {
  const word = words[randInt(0, words.length - 1)];
  const x = randInt(60, W - 140);
  const speed = 0.4 + Math.random() * 0.7 + Math.min(0.6, score / 50);
  const enemy = { x, y: -40, speed, word, typed:'', dead:false, explodeTimer:0 };
  enemies.push(enemy);
}

/* -------------------------
   Typing logic
   ------------------------- */
function checkTypedAgainstEnemies() {
  let matched = null;
  for (let en of enemies) {
    if (en.dead) continue;
    if (en.word.startsWith(activeTyped)) { matched = en; break; }
  }
  if (matched) {
    matched.typed = activeTyped;
    if (matched.typed === matched.word) {
      matched.dead = true;
      matched.explodeTimer = 20;
      score += matched.word.length * 10;
      activeTyped = '';
      updateScoreVisual();
    }
  }
}

/* -------------------------
   Visual helpers
   ------------------------- */
function updateScoreVisual() {
  const el = document.getElementById('score');
  if (el) el.textContent = score;
}
function updateTypedVisual() {
  const el = document.getElementById('typed');
  if (el) el.textContent = activeTyped || ' (type to start) ';
}

/* -------------------------
   Game loop & rendering
   ------------------------- */
function update(dt) {
  if (!gameStarted || gameOver) return;

  lastSpawn += dt;
  difficultyTimer += dt;
  if (lastSpawn > spawnInterval) {
    spawnEnemy();
    lastSpawn = 0;
  }
  if (difficultyTimer > 15000) {
    spawnInterval = Math.max(600, spawnInterval - 150);
    difficultyTimer = 0;
  }

  for (let en of enemies) {
    if (en.dead) { en.explodeTimer--; continue; }
    en.y += en.speed * dt * 0.06;
    if (en.y > H - 90) { gameOver = true; }
  }
  enemies = enemies.filter(e => !(e.dead && e.explodeTimer <= 0));
}

function draw() {
  if (!gameStarted) {
    ctx.clearRect(0,0,W,H);
    return;
  }

  ctx.clearRect(0, 0, W, H);
  ctx.fillStyle = 'rgba(0,0,0,0.0)';
  ctx.fillRect(0,0,W,H);

  const playerX = W/2, playerY = H-60;
  if (playerImg.complete && playerImg.naturalWidth !== 0) {
    ctx.drawImage(playerImg, playerX-32, playerY-32, 64,64);
  } else {
    ctx.fillStyle = '#8be9fd';
    ctx.beginPath();
    ctx.moveTo(playerX, playerY-24);
    ctx.lineTo(playerX-20, playerY+20);
    ctx.lineTo(playerX+20, playerY+20);
    ctx.closePath();
    ctx.fill();
  }

  for (let en of enemies) {
    const ex = en.x, ey = en.y;
    if (enemyImg.complete && enemyImg.naturalWidth !== 0) ctx.drawImage(enemyImg, ex, ey, 48,48);
    else { ctx.fillStyle = '#ff6b6b'; ctx.fillRect(ex, ey, 48,32); }

    ctx.font = '18px Arial';
    const typed = en.typed||'', remaining = en.word.slice(typed.length);
    ctx.fillStyle = '#7be8ff';
    ctx.fillText(typed, ex+56, ey+20);
    ctx.fillStyle = '#ffffff';
    ctx.fillText(remaining, ex+56 + ctx.measureText(typed).width, ey+20);

    if (en.dead) {
      const t = en.explodeTimer;
      ctx.beginPath();
      ctx.arc(ex+24, ey+16, (20-t)*1.8,0,Math.PI*2);
      ctx.fillStyle = `rgba(255,200,50,${t/20})`;
      ctx.fill();
    }
  }

  ctx.font = '14px Arial';
  ctx.fillStyle = '#ffd166';
  ctx.fillText('Score: ' + score, 10, 20);

  if (gameOver) {
    ctx.fillStyle = 'rgba(0,0,0,0.6)';
    ctx.fillRect(0,H/2-60,W,120);
    ctx.fillStyle = '#ff6b6b';
    ctx.font = '48px Arial';
    ctx.fillText('GAME OVER', W/2 - 150, H/2);
    ctx.font = '20px Arial';
    ctx.fillText('Press F5 to restart', W/2 - 100, H/2 + 36);
  }
}

/* -------------------------
   Main loop timing
   ------------------------- */
let lastTime = performance.now();
function loop(now) {
  const dt = now - lastTime;
  lastTime = now;
  update(dt);
  draw();
  requestAnimationFrame(loop);
}
requestAnimationFrame(loop);

/* -------------------------
   Initialization
   ------------------------- */
playerImg.addEventListener('error', ()=>{ console.warn('Player image not found'); });
enemyImg.addEventListener('error', ()=>{ console.warn('Enemy image not found'); });

fetch('assets/music/bgm.mp3', { method: 'HEAD' })
  .then(res=>{
    if(res.ok){ bgm.src='assets/music/bgm.mp3'; bgm.volume=0.6; bgm.play().catch(()=>{});}
  }).catch(()=>{});
