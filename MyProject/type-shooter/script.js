
/* script.js â€” core game logic for the typing shooter
   Comments are added to explain each section (assignment requires comments).
*/

/* -------------------------
   Configuration & assets
   ------------------------- */
const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');

// logical size (we'll scale canvas to displayed size)
const W = 900, H = 600;
canvas.width = W;
canvas.height = H;

// Game state
let enemies = [];        // array of enemy objects
let words = [            // sample word list; you can expand or replace
  'star', 'comet', 'orbit', 'laser', 'alien', 'rocket', 'planet',
  'asteroid', 'shield', 'warp', 'engine', 'galaxy', 'space', 'fuel', 'sergei','beegii','hello', 'ginganinja'
];
let activeTyped = '';    // current typed letters
let score = 0;
let gameOver = false;
let spawnInterval = 2000; // initial spawn every 2s
let lastSpawn = 0;
let difficultyTimer = 0;

// Assets (images/sound)
const playerImg = new Image();
playerImg.src = 'assets/images/player_ship.png'; // (you will provide this file)
const enemyImg = new Image();
enemyImg.src = 'assets/images/enemy_ship.png';   // (optional enemy image)

/* Sound: background music element is in HTML; user will place file later */
const bgm = document.getElementById('bgm');
const musicToggle = document.getElementById('musicToggle');

// Event: music toggle
musicToggle.addEventListener('click', () => {
  if (bgm.src) {
    if (bgm.paused) { bgm.play().catch(()=>{}); musicToggle.textContent = 'Mute'; }
    else { bgm.pause(); musicToggle.textContent = 'Unmute'; }
  } else {
    alert('Add a music file to assets/music/bgm.mp3 and set the src in the HTML audio element or via instructions.');
  }
});

/* -------------------------
   Utilities
   ------------------------- */
// returns a random integer between min and max (inclusive)
function randInt(min, max) { return Math.floor(Math.random() * (max - min + 1)) + min; }

/* -------------------------
   Enemy creation
   ------------------------- */
function spawnEnemy() {
  // choose a random word
  const word = words[randInt(0, words.length - 1)];
  // spawn position (x), keep inside canvas horizontally
  const x = randInt(60, W - 140);
  const speed = 0.4 + Math.random() * 0.7 + Math.min(0.6, score / 50); // speed up over time
  // enemy object holds position, word, typed progress, and whether it's dying
  const enemy = {
    x, y: -40, speed,
    word,
    typed: '',    // how many letters the player has typed matching this word
    dead: false,
    explodeTimer: 0
  };
  enemies.push(enemy);
}

/* -------------------------
   Input handling (event handler)
   ------------------------- */
// Keydown listener â€” this is the required event handler.
document.addEventListener('keydown', (e) => {
  if (gameOver) return;
  // only letters and backspace and space allowed
  if (e.key === 'Backspace') {
    activeTyped = activeTyped.slice(0, -1);
    updateTypedVisual();
    return;
  }
  if (e.key.length === 1) {
    // append lowercase char
    const ch = e.key.toLowerCase();
    if (/^[a-z]$/.test(ch)) {
      activeTyped += ch;
      checkTypedAgainstEnemies();
      updateTypedVisual();
    }
  }
});

/* -------------------------
   Typing logic:
   Check typed string against enemy words.
   We match the typed letters to the earliest enemy that matches the prefix.
   ------------------------- */
function checkTypedAgainstEnemies() {
  // find best match: an enemy whose word starts with activeTyped
  let matched = null;
  for (let en of enemies) {
    if (en.dead) continue;
    if (en.word.startsWith(activeTyped)) {
      matched = en;
      break;
    }
  }
  if (matched) {
    // update that enemy's typed progress
    matched.typed = activeTyped;
    // if completed, mark as dead and give score
    if (matched.typed === matched.word) {
      matched.dead = true;
      matched.explodeTimer = 20; // frames of explosion
      score += matched.word.length * 10;
      activeTyped = ''; // reset typed buffer after successful hit
      updateScoreVisual();
    }
  } else {
    // if no match then we don't clear typed buffer â€” player can continue typing
    // optionally, you could penalize for wrong typing
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
  if (gameOver) return;

  // spawn new enemies over time
  lastSpawn += dt;
  difficultyTimer += dt;
  if (lastSpawn > spawnInterval) {
    spawnEnemy();
    lastSpawn = 0;
  }
  // gradually increase difficulty (reduce spawn interval every 15s)
  if (difficultyTimer > 15000) {
    spawnInterval = Math.max(600, spawnInterval - 150); // floor at 600ms
    difficultyTimer = 0;
  }

  // update enemies
  for (let en of enemies) {
    if (en.dead) {
      en.explodeTimer--;
      continue;
    }
    en.y += en.speed * dt * 0.06; // movement scaled by dt
    // if enemy reaches bottom => game over
    if (en.y > H - 90) {
      gameOver = true;
    }
  }

  // clean up exploded enemies
  enemies = enemies.filter(e => !(e.dead && e.explodeTimer <= 0));
}

function draw() {
  // clear canvas
  ctx.clearRect(0, 0, W, H);

  // draw background (canvas background is set via CSS; we also paint a faint overlay)
  ctx.fillStyle = 'rgba(0,0,0,0.0)';
  ctx.fillRect(0, 0, W, H);

  // draw player ship at bottom center
  const playerX = W / 2;
  const playerY = H - 60;
  // if playerImg not loaded yet, draw a triangle fallback
  if (playerImg.complete && playerImg.naturalWidth !== 0) {
    const pw = 64, ph = 64;
    ctx.drawImage(playerImg, playerX - pw/2, playerY - ph/2, pw, ph);
  } else {
    // fallback triangle ship
    ctx.fillStyle = '#8be9fd';
    ctx.beginPath();
    ctx.moveTo(playerX, playerY - 24);
    ctx.lineTo(playerX - 20, playerY + 20);
    ctx.lineTo(playerX + 20, playerY + 20);
    ctx.closePath();
    ctx.fill();
  }

  // draw enemies with their words and small enemy ship image
  for (let en of enemies) {
    const ex = en.x;
    const ey = en.y;

    // draw enemy ship image if available
    if (enemyImg.complete && enemyImg.naturalWidth !== 0) {
      ctx.drawImage(enemyImg, ex, ey, 48, 48);
    } else {
      // fallback: small square
      ctx.fillStyle = '#ff6b6b';
      ctx.fillRect(ex, ey, 48, 32);
    }

    // draw word next to the enemy
    ctx.font = '18px Arial';
    // highlight typed portion
    const typed = en.typed || '';
    const remaining = en.word.slice(typed.length);

    // draw typed part (cyan)
    ctx.fillStyle = '#7be8ff';
    ctx.fillText(typed, ex + 56, ey + 20);
    // draw remaining part (white)
    ctx.fillStyle = '#ffffff';
    ctx.fillText(remaining, ex + 56 + ctx.measureText(typed).width, ey + 20);

    // if exploding, draw a simple circle explosion
    if (en.dead) {
      const t = en.explodeTimer;
      ctx.beginPath();
      ctx.arc(ex + 24, ey + 16, (20 - t) * 1.8, 0, Math.PI * 2);
      ctx.fillStyle = `rgba(255,200,50,${t/20})`;
      ctx.fill();
    }
  }

  // HUD: score already in sidebar, but draw small top-left indicator too
  ctx.font = '14px Arial';
  ctx.fillStyle = '#ffd166';
  ctx.fillText('Score: ' + score, 10, 20);

  if (gameOver) {
    ctx.fillStyle = 'rgba(0,0,0,0.6)';
    ctx.fillRect(0, H/2 - 60, W, 120);
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
// Preload images (simple)
playerImg.addEventListener('error', () => {
  // If user didn't provide player image, the fallback triangle will appear.
  console.warn('Player image not found at assets/images/player_ship.png');
});
enemyImg.addEventListener('error', () => {
  console.warn('Enemy image not found at assets/images/enemy_ship.png');
});

// Optional: set background music source if file exists at expected path
// NOTE: You will provide the music file later at assets/music/bgm.mp3
fetch('assets/music/bgm.mp3', { method: 'HEAD' })
  .then(res => {
    if (res.ok) {
      bgm.src = 'assets/music/bgm.mp3';
      // try to autoplay muted (browsers block autoplay with sound)
      bgm.volume = 0.6;
      bgm.play().catch(()=>{ /* autoplay blocked; user can click Mute/Unmute */ });
    }
  }).catch(()=>{/* ignoring missing file for now */});

/* -------------------------
   End of script.js
   ------------------------- */
