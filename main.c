#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define BOARD_SIZE 8

typedef struct {
    int color; // 0 none, 1 red, 2 white
    int king;  // 0 not king, 1 king
} Piece;

Piece board[BOARD_SIZE][BOARD_SIZE];
int selX = -1, selY = -1;
int turn = 1; // 1 = red, 2 = white

typedef enum { HOME, PLAYING, PAUSED } GameState;
GameState state = HOME;

// Buttons
SDL_Rect playButton;
SDL_Rect restartButton;

// Initialize checkers board
void initBoard() {
    for(int y=0; y<BOARD_SIZE; y++) {
        for(int x=0; x<BOARD_SIZE; x++) {
            if((x+y)%2==1) {
                if(y<3) board[y][x] = (Piece){2,0}; // white
                else if(y>4) board[y][x] = (Piece){1,0}; // red
                else board[y][x] = (Piece){0,0};
            } else {
                board[y][x] = (Piece){0,0};
            }
        }
    }
    selX = selY = -1;
    turn = 1;
}

// Check legal move
int legalMove(int sx, int sy, int dx, int dy) {
    Piece p = board[sy][sx];
    if(p.color != turn) return 0;
    Piece d = board[dy][dx];
    if(d.color != 0) return 0;
    int dir = (p.color==1)? -1 : 1;

    if(abs(dx-sx)==1 && dy-sy==dir) return 1;
    if(p.king && abs(dx-sx)==1 && abs(dy-sy)==1) return 1;

    if(abs(dx-sx)==2 && abs(dy-sy)==2){
        int mx=(sx+dx)/2, my=(sy+dy)/2;
        Piece mid = board[my][mx];
        if(mid.color != 0 && mid.color != p.color) return 1;
    }

    return 0;
}

// Execute move
void doMove(int sx,int sy,int dx,int dy){
    Piece p = board[sy][sx];
    if(abs(dx-sx)==2){
        int mx=(sx+dx)/2, my=(sy+dy)/2;
        board[my][mx] = (Piece){0,0};
    }
    board[dy][dx] = p;
    board[sy][sx] = (Piece){0,0};

    if(p.color==1 && dy==0) board[dy][dx].king=1;
    if(p.color==2 && dy==BOARD_SIZE-1) board[dy][dx].king=1;

    turn = (turn==1)? 2 : 1;
}

// Check if point inside rect
int insideRect(int x,int y,SDL_Rect r){
    return x>=r.x && x<=r.x+r.w && y>=r.y && y<=r.y+r.h;
}

// Draw home screen
void drawHome(SDL_Renderer *ren,int winW,int winH){
    SDL_SetRenderDrawColor(ren,50,50,50,255);
    SDL_RenderClear(ren);

    playButton = (SDL_Rect){ winW/2 - 100, winH/2 - 50, 200, 100 };
    SDL_SetRenderDrawColor(ren,0,200,0,255);
    SDL_RenderFillRect(ren,&playButton);

    SDL_RenderPresent(ren);
}

// Draw checkers board
void drawBoard(SDL_Renderer *ren,int winW,int winH){
    SDL_SetRenderDrawColor(ren,0,0,0,255);
    SDL_RenderClear(ren);

    int tileSize = (winW < winH ? winW : winH) / BOARD_SIZE;
    int xOffset = (winW - tileSize*BOARD_SIZE)/2;
    int yOffset = (winH - tileSize*BOARD_SIZE)/2;

    restartButton = (SDL_Rect){ winW - 110, 10, 100, 40 };
    SDL_SetRenderDrawColor(ren,200,200,200,255);
    SDL_RenderFillRect(ren,&restartButton);

    for(int y=0;y<BOARD_SIZE;y++){
        for(int x=0;x<BOARD_SIZE;x++){
            SDL_Rect r = { x*tileSize + xOffset, y*tileSize + yOffset, tileSize, tileSize };
            SDL_SetRenderDrawColor(ren, (x+y)%2==0?240:181, (x+y)%2==0?217:136, (x+y)%2==0?181:99,255);
            SDL_RenderFillRect(ren,&r);

            if(x==selX && y==selY){
                SDL_SetRenderDrawColor(ren,0,255,0,100);
                SDL_RenderFillRect(ren,&r);
            }

            Piece p=board[y][x];
            if(p.color!=0){
                SDL_SetRenderDrawColor(ren,p.color==1?255:255,p.color==1?0:255,p.color==1?0:255,255);
                SDL_Rect pr={ x*tileSize+tileSize/8 + xOffset, y*tileSize+tileSize/8 + yOffset, tileSize*3/4, tileSize*3/4 };
                SDL_RenderFillRect(ren,&pr);

                if(p.king){
                    SDL_SetRenderDrawColor(ren,0,0,255,255);
                    SDL_Rect kr={ x*tileSize+tileSize/3 + xOffset, y*tileSize+tileSize/3 + yOffset, tileSize/3, tileSize/3 };
                    SDL_RenderFillRect(ren,&kr);
                }
            }
        }
    }

    SDL_RenderPresent(ren);
}

// Draw paused overlay
void drawPaused(SDL_Renderer *ren,int winW,int winH){
    SDL_SetRenderDrawColor(ren,0,0,0,200);
    SDL_Rect r = {0,0,winW,winH};
    SDL_RenderFillRect(ren,&r);
    SDL_RenderPresent(ren);
}

int main(int argc,char **argv){
    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        printf("SDL_Init Error: %s\n",SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Checkers SDL2 - Turn Based",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 640, SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);

    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    int running=1;
    while(running){
        int winW,winH;
        SDL_GetWindowSize(window,&winW,&winH);

        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT) running=0;

            if(e.type==SDL_MOUSEBUTTONDOWN){
                int mx=e.button.x, my=e.button.y;

                if(state==HOME){
                    if(insideRect(mx,my,playButton)){
                        initBoard();
                        state=PLAYING;
                    }
                } else if(state==PLAYING){
                    if(insideRect(mx,my,restartButton)){
                        initBoard();
                    } else {
                        int tileSize = (winW < winH ? winW : winH) / BOARD_SIZE;
                        int xOffset = (winW - tileSize*BOARD_SIZE)/2;
                        int yOffset = (winH - tileSize*BOARD_SIZE)/2;

                        int bx = (mx - xOffset)/tileSize;
                        int by = (my - yOffset)/tileSize;

                        if(bx<0 || bx>=BOARD_SIZE || by<0 || by>=BOARD_SIZE) continue;

                        if(selX==-1){
                            if(board[by][bx].color==turn){
                                selX=bx; selY=by;
                            }
                        } else {
                            if(legalMove(selX,selY,bx,by)){
                                doMove(selX,selY,bx,by);
                            }
                            selX=selY=-1;
                        }
                    }
                }
            }

            if(e.type==SDL_KEYDOWN){
                if(state==PLAYING && e.key.keysym.sym==SDLK_p){
                    state=PAUSED;
                } else if(state==PAUSED && e.key.keysym.sym==SDLK_p){
                    state=PLAYING;
                }
            }
        }

        if(state==HOME) drawHome(renderer,winW,winH);
        else if(state==PLAYING) drawBoard(renderer,winW,winH);
        else if(state==PAUSED) drawPaused(renderer,winW,winH);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
