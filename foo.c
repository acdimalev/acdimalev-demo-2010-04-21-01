#include "SDL.h"
#include <cairo.h>

#include <math.h>

int fps = 30;
int w = 320;
int h = 240;
float scale = 8.0;

int main(int argc, char **argv) {
  SDL_Surface *sdl_surface;
  cairo_surface_t *surface;
  cairo_t *cr;

  SDL_Event event;
  Uint8 *keystate;
  Uint32 next_frame, now;

  float theta, t, taccel, tvel, tdrag, x, y, xn, yn;

  int running;

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
  SDL_SetVideoMode(w, h, 32, 0);
  sdl_surface = SDL_GetVideoSurface();

  /* Initialize Canvas */
  surface = cairo_image_surface_create_for_data(
    sdl_surface->pixels,
    CAIRO_FORMAT_RGB24,
    sdl_surface->w,
    sdl_surface->h,
    sdl_surface->pitch
    );
  cr = cairo_create(surface);
  cairo_surface_destroy(surface);

  cairo_scale(cr, 1, -1);
  cairo_translate(cr, w/2.0, -h/2.0);

  cairo_scale(cr, h/scale, h/scale);

  /* Initialize Delay */
  next_frame = 1024.0 / fps;

  /* Game Logic */
  running = 1;
  t = M_PI_4;
  tdrag = 63/64.0;
  taccel = 4;

  SDL_LockSurface(sdl_surface);
  while (running) {

    /* Render Frame */
    cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
    cairo_paint(cr);
    cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

    theta = 2*M_PI * (0/3.0); x = sin(theta); y = cos(theta);
    xn = x*cos(t) - y*sin(t); yn = x*sin(t) + y*cos(t);
    cairo_move_to(cr, xn, yn);
    theta = 2*M_PI * (1/3.0); x = sin(theta); y = cos(theta);
    xn = x*cos(t) - y*sin(t); yn = x*sin(t) + y*cos(t);
    cairo_line_to(cr, xn, yn);
    theta = 2*M_PI * (2/3.0); x = sin(theta); y = cos(theta);
    xn = x*cos(t) - y*sin(t); yn = x*sin(t) + y*cos(t);
    cairo_line_to(cr, xn, yn);
    cairo_close_path(cr);

    theta = 2*M_PI * (1/3.0); x =  0.1; y = cos(theta) / 2.0;
    xn = x*cos(t) - y*sin(t); yn = x*sin(t) + y*cos(t);
    cairo_move_to(cr, xn, yn);
    theta = 2*M_PI * (1/3.0); x = -0.1; y = cos(theta) / 2.0;
    xn = x*cos(t) - y*sin(t); yn = x*sin(t) + y*cos(t);
    cairo_line_to(cr, xn, yn);
    theta = 2*M_PI * (1/3.0); x = -0.1; y = cos(theta);
    xn = x*cos(t) - y*sin(t); yn = x*sin(t) + y*cos(t);
    cairo_line_to(cr, xn, yn);
    theta = 2*M_PI * (1/3.0); x =  0.1; y = cos(theta);
    xn = x*cos(t) - y*sin(t); yn = x*sin(t) + y*cos(t);
    cairo_line_to(cr, xn, yn);
    cairo_close_path(cr);

    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_fill(cr);

    /* Update Display */
    SDL_UnlockSurface(sdl_surface);
    SDL_Flip(sdl_surface);
    SDL_LockSurface(sdl_surface);

    /* Delay */
    now = SDL_GetTicks();
    if (now < next_frame) {
      SDL_Delay( next_frame - SDL_GetTicks() );
    }
    next_frame = next_frame + 1024.0 / fps;

    /* Game Logic */
    SDL_PumpEvents();
    keystate = SDL_GetKeyState(NULL);
    if (keystate[SDLK_q]) {
      running = 0;
    }
    if (keystate[SDLK_LEFT]) {
      tvel = tvel + taccel*2*M_PI / fps;
    }
    if (keystate[SDLK_RIGHT]) {
      tvel = tvel - taccel*2*M_PI / fps;
    }

    t = t + tvel / fps;
    tvel = tvel * pow(1.0 - tdrag, 1.0/fps);

  }
  SDL_UnlockSurface(sdl_surface);

  cairo_destroy(cr);

  SDL_Quit();

  return 0;
}
