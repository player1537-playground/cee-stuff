#include <stdio.h>
#include <SDL/SDL.h>
#include <math.h>

#define MAP_W 24
#define MAP_H 24
#define DIST 10
#define FPS 30
#define SCREEN_W 640
#define SCREEN_H 480
#define GUN_W 272
#define GUN_H 152
#define CROSSHAIR_W 88
#define CROSSHAIR_H 56
#define LOOKSPEED 20
#define SDL_LoadBMPWithAlpha(surface, path) surface = SDL_LoadBMP(path); if (surface != NULL) { SDL_SetColorKey(surface, SDL_SRCCOLORKEY, SDL_MapRGB(surface->format, 255, 0, 255)); } else { printf("Error loading image\n");  }


typedef struct EnemyNode {
  int x;
  int y;
  int health;
  SDL_Surface* sprite;
  struct EnemyNode* next;
} EnemyNode;

void FPS_Init(int* nextTick, int* interval) {
  *nextTick = 0;
  *interval = 1 * 1000 / FPS;
}

void FPS_Fn(int* nextTick, int* interval) {
  if ((*nextTick) > SDL_GetTicks())
    SDL_Delay((*nextTick) - SDL_GetTicks());
  *nextTick = SDL_GetTicks() + (*interval);
}

void walk(double* playerX, double* playerY, int x, int y) {
  if ((*playerX) + x < MAP_W
      && (*playerX) + x > 0
      && (*playerY) + y < MAP_H
      && (*playerY) + y > 0) {
    *playerX += x;
    *playerY += y;
  }
}

EnemyNode* addEnemy(EnemyNode* head, int x, int y, SDL_Surface* sprite) {
  EnemyNode* newNode = (EnemyNode*)malloc(sizeof(EnemyNode));
  if (head != NULL)
    head->next = newNode;
  newNode->x = x;
  newNode->y = y;
  newNode->health = 100;
  newNode->sprite = sprite;
  newNode->next = NULL;
  return newNode;
}


main() {
  SDL_Event event;
  int bRun = 1;
  SDL_Surface* screen;
  SDL_Surface* gun;
  SDL_Surface* crosshair;
  SDL_Surface* crosshairfire;
  SDL_Surface* crosshairptr;
  SDL_Surface* enemy;
  EnemyNode* head = NULL;
  double playerY = 22;
  double playerX = 12;
  double cameraZ = 0;
  double dirX = -1;
  double dirY = 0;
  double planeX = 0;
  double planeY = 0.66;
  double moveSpeed = 0.1;
  double rotSpeed = 0.1;
  double velZ = 0;
  int nextTick;
  int interval;
  int gunShake = 0;
  int upp = 0;
  int downp = 0;
  int rightp = 0;
  int leftp = 0;
  int qp = 0;
  int ep = 0;
  int spacep = 0;
  int fp = 0;
  int pp = 0;
  char map[MAP_H][MAP_W] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
  };
  FPS_Init(&nextTick, &interval);
  SDL_Rect rect;
  rect.x = 0;
  rect.y = 0;
  rect.w = 10;
  rect.h = 10;
  int i;

  atexit(SDL_Quit);
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    exit(1);
  SDL_WM_SetCaption("SDL Window", NULL);

  screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32,
			    SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_ANYFORMAT);
  SDL_LoadBMPWithAlpha(gun, "./sdltest/gun.bmp");
  SDL_LoadBMPWithAlpha(crosshair, "./sdltest/crosshair.bmp");
  SDL_LoadBMPWithAlpha(crosshairfire, "./sdltest/crosshair-fire.bmp");
  SDL_LoadBMPWithAlpha(enemy, "./sdltest/enemy.bmp");
  crosshairptr = crosshair;

  while (bRun) {
    SDL_FillRect(screen, NULL, 0x000000);
    int x;
    for (x = 0; x < SCREEN_W; x++) {
      double cameraX = 2 * x / ((double) SCREEN_W) - 1;
      double rayPosX = playerX;
      double rayPosY = playerY;
      double rayDirX = dirX + planeX * cameraX;
      double rayDirY = dirY + planeY * cameraX; // Watch out!
      int mapX = (int)rayPosX;
      int mapY = (int)rayPosY;
      double sideDistX;
      double sideDistY;
      double perpWallDist;
      double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
      double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
      int stepX;
      int stepY;
      int hit = 0;
      int side;

      if (rayDirX < 0) {
	stepX = -1;
	sideDistX = (rayPosX - mapX) * deltaDistX;
      } else {
	stepX = 1;
	sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
      }
      if (rayDirY < 0) {
	stepY = -1;
	sideDistY = (rayPosY - mapY) * deltaDistY;
      } else {
	stepY = 1;
	sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
      }
      while (hit == 0) {
	if (sideDistX <= sideDistY) {
	  sideDistX += deltaDistX;
	  mapX += stepX;
 	  side = 0;
	} else {
	  sideDistY += deltaDistY;
	  mapY += stepY;
	  side = 1;
	}
	if (map[mapX][mapY] > 0)
	  hit = 1;
      }
      if (side == 0)
	perpWallDist = fabs((mapX - rayPosX + (1 - stepX) / 2) / rayDirX);
      else
	perpWallDist = fabs((mapY - rayPosY + (1 - stepY) / 2) / rayDirY);
      int lineHeight = abs((int)(SCREEN_H / perpWallDist));
      int drawStart = -lineHeight / 2 + SCREEN_H / 2;
      if (drawStart < 0)
	drawStart = 0;
      int drawEnd = lineHeight / 2 + SCREEN_H / 2;
      if (drawEnd >= SCREEN_H)
	drawEnd = SCREEN_H - 1;
      int color;
      switch (map[mapX][mapY]) {
      case 01: color = 0xFF0000; break;
      case 02: color = 0x00FF00; break;
      case 03: color = 0x0000FF; break;
      case 04: color = 0xFFFFFF; break;
      default: color = 0x00FFFF; break;
      }
      if (side == 1)
	color = color / 2;
      rect.x = x;
      rect.y = SCREEN_H / 2 - lineHeight / 2 - cameraZ;
      rect.h = lineHeight;
      rect.w = 1;  // Change to 5 ;)
      SDL_FillRect(screen, &rect, color);
    }
    rect.x = SCREEN_W - GUN_W + gunShake;
    rect.y = SCREEN_H - GUN_H + gunShake;
    SDL_BlitSurface(gun, NULL, screen, &rect);
    rect.x = SCREEN_W / 2 - CROSSHAIR_W / 2;
    rect.y = SCREEN_H / 2 - CROSSHAIR_H / 2;
    SDL_BlitSurface(crosshairptr, NULL, screen, &rect);
    SDL_Flip(screen);
    FPS_Fn(&nextTick, &interval);
    if (upp) {
      if (map[(int)(playerX + dirX * moveSpeed)][(int)playerY] == 0)
	playerX += dirX * moveSpeed;
      if (map[(int)playerX][(int)(playerY + dirY * moveSpeed)] == 0)
	playerY += dirY * moveSpeed;
    }
    if (downp) {
      if (map[(int)(playerX - dirX * moveSpeed)][(int)playerY] == 0)
	playerX -= dirX * moveSpeed;
      if (map[(int)playerX][(int)(playerY - dirY * moveSpeed)] == 0)
	playerY -= dirY * moveSpeed;
    }
    if (rightp) {
      double oldDirX = dirX;
      dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
      dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
      double oldPlaneX = planeX;
      planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
      planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }
    if (leftp) {
      double oldDirX = dirX;
      dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
      dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
      double oldPlaneX = planeX;
      planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
      planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }
    if (qp) {
      cameraZ -= LOOKSPEED;
    }
    if (ep) {
      cameraZ += LOOKSPEED;
    }
    if (fp && crosshairptr == crosshair) {
      crosshairptr = crosshairfire;
      gunShake = 20;
    } else {
      crosshairptr = crosshair;
      gunShake = 0;
    }
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
	switch (event.key.keysym.sym) {
	case SDLK_UP:
	case SDLK_w:
	  upp = 1;
	  break;
	case SDLK_DOWN:
	case SDLK_s:
	  downp = 1;
	  break;
	case SDLK_RIGHT:
	case SDLK_d:
	  rightp = 1;
	  break;
	case SDLK_LEFT:
	case SDLK_a:
	  leftp = 1;
	  break;
	case SDLK_ESCAPE:
	  bRun = 0;
	  break;
	case SDLK_q:
	  qp = 1;
	  break;
	case SDLK_e:
	  ep = 1;
	  break;
	case SDLK_SPACE:
	  velZ = 10;
	  spacep = 1;
	case SDLK_f:
	  fp = 1;
	  break;
	case SDLK_p:
	  //	  addEnemy(&head, playerX, playerY, enemy);
	  pp = 1;
	  break;
	default:
	  break;
	}
	break;
      case SDL_KEYUP:
	switch (event.key.keysym.sym) {
	case SDLK_UP:
	case SDLK_w:
	  upp = 0;
	  break;
	case SDLK_DOWN:
	case SDLK_s:
	  downp = 0;
	  break;
	case SDLK_RIGHT:
	case SDLK_d:
	  rightp = 0;
	  break;
	case SDLK_LEFT:
	case SDLK_a:
	  leftp = 0;
	  break;
	case SDLK_ESCAPE:
	  bRun = 0;
	  break;
	case SDLK_q:
	  qp = 0;
	  break;
	case SDLK_e:
	  ep = 0;
	  break;
	case SDLK_SPACE:
	  spacep = 0;
	  break;
	case SDLK_f:
	  fp = 0;
	  break;
	case SDLK_p:
	  pp = 0;
	  break;
	default:
	  break;
	}
	break;
      case SDL_QUIT:
	bRun = 0;
	break;
      default:
	break;
      }
    }
  }
}
