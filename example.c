#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif // #define STB_IMAGE_IMPLEMENTATION
#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL_events.h>
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_scancode.h>
#include <SDL_surface.h>
#include <SDL_timer.h>
#include <SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Surface *load_image(const char *filename);

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

int main() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
    fprintf(stderr, "SDL failed to initialize.");
    exit(EXIT_FAILURE);
  }

  SDL_Window *window =
      SDL_CreateWindow("SDL2 Image Loading Example", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,
                       SDL_WINDOW_RESIZABLE);

  SDL_Surface *window_surface = SDL_GetWindowSurface(window);

  SDL_Surface *image = load_image("./sdl2_logo_trans.png");
  Sint32 image_x = (WINDOW_WIDTH / 2) - (image->w / 2);
  Sint32 image_y = (WINDOW_HEIGHT / 2) - (image->h / 2);
  SDL_bool should_close = SDL_FALSE;

  while (!should_close) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

      switch (event.type) {
      case SDL_QUIT: {
        should_close = SDL_TRUE;
        break;
      }

      case (SDL_KEYDOWN): {
        switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE:
          should_close = SDL_TRUE;
          break;

        case SDL_SCANCODE_LEFT: {
          if (image_x > 14)
            image_x -= 14;
          break;
        }

        case SDL_SCANCODE_RIGHT: {
          if ((image_x + image->w) < (WINDOW_WIDTH - 14))
            image_x += 14;
          break;
        }

        case SDL_SCANCODE_UP: {
          if (image_y > 14)
            image_y -= 14;
          break;
        }

        case SDL_SCANCODE_DOWN: {
          if ((image_y + image->h) < (WINDOW_HEIGHT - 14))
            image_y += 14;
          break;
        }
        default:
          break;
        }
      }
      default:
        break;
      }
    }
    SDL_FillRect(window_surface, NULL,
                 SDL_MapRGB(window_surface->format, 255, 0, 0));

    SDL_BlitSurface(image, NULL, window_surface,
                    &(SDL_Rect){image_x, image_y, image->w, image->h});

    SDL_UpdateWindowSurface(window);
  }

  SDL_FreeSurface(image);
  SDL_DestroyWindowSurface(window);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_SUCCESS);
}

SDL_Surface *load_image(const char *filename) {
  Sint32 width = 0;
  Sint32 height = 0;
  Sint32 bytes_per_pixel =
      0; // Since we work with bytes instead of bits here in C.

  // Now the good stuff:
  Uint8 *data = stbi_load(filename, &width, &height, &bytes_per_pixel, 0);

  // What is a "pitch"? Its the "Dot Pitch" of an computer image.
  // Taken from https://en.wikipedia.org/wiki/Dot_pitch
  // Dot pitch (sometimes called line pitch, stripe pitch, or phosphor pitch) is
  // a specification for a computer display, computer printer, image scanner, or
  // other pixel-based devices that describe the distance, for example, between
  // dots (sub-pixels) on a display screen.
  Sint32 dot_pitch = width * bytes_per_pixel;
  dot_pitch = (dot_pitch + 3) & ~3;

  Sint32 red_mask;
  Sint32 green_mask;
  Sint32 blue_mask;
  Sint32 alpha_mask;

  // This only works with little endian computer processors,
  // if you want support for big endian (if you even still use it), implement it
  // yourself.
  red_mask = 0x000000FF;
  green_mask = 0x0000FF00;
  blue_mask = 0x00FF0000;
  alpha_mask = (bytes_per_pixel == 4) ? 0xFF000000 : 0;

  // Now finally we can get that damn surface.
  SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(
      data, width, height, bytes_per_pixel * 8, dot_pitch, red_mask, green_mask,
      blue_mask, alpha_mask);
  if (!surface) {
    SDL_free(data);
    return NULL;
  }

  return surface;
}
