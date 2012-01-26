/*
 The contents of this file are subject to the Mozilla Public License        
 Version 1.1 (the "License"); you may not use this file except in           
 compliance with the License. You may obtain a copy of the License at       
 http://www.mozilla.org/MPL/                                                
                                                                            
 Software distributed under the License is distributed on an "AS IS"        
 basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the    
 License for the specific language governing rights and limitations         
 under the License.                                                         
                                                                            
 Alternatively, the contents of this file may be used under the terms       
 of the GNU Lesser General Public license (the  "LGPL License"), in which case the  
 provisions of LGPL License are applicable instead of those                  
 above.                                                                     
                                                                            
 For feedback and questions about my Files and Projects please mail me,     
 Alexander Matthes (Ziz) , zizsdl_at_googlemail.com                         
*/
#include "settings.h"
#include "../sparrow3d/sparrow3d.h"
#include <SDL.h>

int settings_stone_quality; //2 perfect, 1 okay, 0 gp2x
int settings_stars_rotating;
int settings_particles;
int settings_font_quality;
int settings_volume;
int settings_color;
int settings_difficult;
int settings_mode;
int settings_control;
int settings_first_start;
char settings_name[3];
int highscore_choice;

spFontPointer settings_font;
spFontPointer settings_small_font;
spFontPointer settings_middle_font;
spFontPointer settings_countdown_font;

int highscore[3][2][2][3];
char highscore_name[3][2][2][3][3]; //game mode, normal/hard, (no) special, top3

char* get_highscore_name(int game_mode,int difficult,int special,int rank)
{
  return highscore_name[game_mode][difficult][special][rank];
}

int get_highscore(int game_mode,int difficult,int special,int rank)
{
  return highscore[game_mode][difficult][special][rank];
}

void insert_highscore(int game_mode,int difficult,int special,char* name,int points)
{
  if (game_mode!=2 && highscore[game_mode][difficult][special][2] > points)
    return;
  if (game_mode==2 && highscore[game_mode][difficult][special][2] < points)
    return;
  highscore_name[game_mode][difficult][special][2][0]=name[0];
  highscore_name[game_mode][difficult][special][2][1]=name[1];
  highscore_name[game_mode][difficult][special][2][2]=name[2];
  highscore[game_mode][difficult][special][2]=points;
  //sort
  int l;
  for (l = 2 ; l > 0; l--)
    if ((game_mode!=2 && highscore[game_mode][difficult][special][l] > highscore[game_mode][difficult][special][l-1]) ||
        (game_mode==2 && highscore[game_mode][difficult][special][l] < highscore[game_mode][difficult][special][l-1]))
    {
      int points = highscore[game_mode][difficult][special][l];
      char c1 = highscore_name[game_mode][difficult][special][l][0];
      char c2 = highscore_name[game_mode][difficult][special][l][1];
      char c3 = highscore_name[game_mode][difficult][special][l][2];
      
      highscore[game_mode][difficult][special][l] = highscore[game_mode][difficult][special][l-1];
      highscore_name[game_mode][difficult][special][l][0] = highscore_name[game_mode][difficult][special][l-1][0];
      highscore_name[game_mode][difficult][special][l][1] = highscore_name[game_mode][difficult][special][l-1][1];
      highscore_name[game_mode][difficult][special][l][2] = highscore_name[game_mode][difficult][special][l-1][2];
      
      highscore[game_mode][difficult][special][l-1] = points;
      highscore_name[game_mode][difficult][special][l-1][0] = c1;
      highscore_name[game_mode][difficult][special][l-1][1] = c2;
      highscore_name[game_mode][difficult][special][l-1][2] = c3;
    }
    else
      break;
}

void settings_load()
{
  #ifdef REALGP2X
    settings_stone_quality = 0;
  #elif PANDORA
    settings_stone_quality = 0;
  #else
    settings_stone_quality = 2;
  #endif
  settings_stars_rotating = 2;
  settings_particles = 1;
  settings_volume = 100;
  settings_color = 0;
  settings_difficult = 0;
  settings_control = 0; //simple control
  settings_mode = 2;
  settings_font = NULL;
  settings_middle_font = NULL;
  settings_small_font = NULL;
  settings_countdown_font = NULL;
  settings_name[0]='A';
  settings_name[1]='A';
  settings_name[2]='A';
  highscore_choice = 0;
  settings_first_start = 0;
  SDL_RWops *file=SDL_RWFromFile("./settings3.dat","rb");
  if (file == NULL)
  {
    settings_first_start = 1;
    file=SDL_RWFromFile("./settings2.dat","rb");
    if (file == NULL)
      return;
  }
  SDL_RWread(file,&settings_stone_quality,sizeof(int),1);
  SDL_RWread(file,&settings_stars_rotating,sizeof(int),1);
  SDL_RWread(file,&settings_particles,sizeof(int),1);
  SDL_RWread(file,&settings_volume,sizeof(int),1);
  SDL_RWread(file,&settings_color,sizeof(int),1);
  SDL_RWread(file,&settings_difficult,sizeof(int),1);
  SDL_RWread(file,&settings_control,sizeof(int),1);
  SDL_RWread(file,&settings_mode,sizeof(int),1);
  SDL_RWread(file,&highscore_choice,sizeof(int),1);
  SDL_RWread(file,settings_name,sizeof(char)*3,1);
  SDL_RWclose(file);
}

void highscore_load()
{
  int i,j,k,l;
  for (i = 0; i < 3; i++)
    for (j = 0; j < 2; j++)
      for (k = 0; k < 2; k++)
        for (l = 0; l < 3; l++)
        {
          highscore_name[i][j][k][l][0]='N';
          highscore_name[i][j][k][l][1]='U';
          highscore_name[i][j][k][l][2]='B';
          if (i == 2)
            highscore[i][j][k][l] = 10000;
          else
            highscore[i][j][k][l] = 0;
        }
  SDL_RWops *file=SDL_RWFromFile("./highscore2.dat","rb");
  if (file == NULL)
    return;
  SDL_RWread(file,highscore,3*2*2*3*sizeof(int),1);
  SDL_RWread(file,highscore_name,3*2*2*3*3,1);
  SDL_RWclose(file);
}

void settings_save()
{
  SDL_RWops *file=SDL_RWFromFile("./settings3.dat","wb");
  SDL_RWwrite(file,&settings_stone_quality,sizeof(int),1);
  SDL_RWwrite(file,&settings_stars_rotating,sizeof(int),1);
  SDL_RWwrite(file,&settings_particles,sizeof(int),1);
  SDL_RWwrite(file,&settings_volume,sizeof(int),1);
  SDL_RWwrite(file,&settings_color,sizeof(int),1);
  SDL_RWwrite(file,&settings_difficult,sizeof(int),1);
  SDL_RWwrite(file,&settings_control,sizeof(int),1);
  SDL_RWwrite(file,&settings_mode,sizeof(int),1);
  SDL_RWwrite(file,&highscore_choice,sizeof(int),1);
  SDL_RWwrite(file,settings_name,sizeof(char)*3,1);
  SDL_RWclose(file);
}

void highscore_save()
{
  SDL_RWops *file=SDL_RWFromFile("./highscore2.dat","wb");
  SDL_RWwrite(file,highscore,3*2*2*3*sizeof(int),1);
  SDL_RWwrite(file,highscore_name,3*2*2*3*3,1);
  SDL_RWclose(file);
}

int settings_get_stone_quality()
{
  return settings_stone_quality;
}

int settings_get_stars_rotating()
{
  return settings_stars_rotating;
}

int settings_get_particles()
{
  return settings_particles;
}

int settings_get_volume()
{
  return settings_volume;
}

void settings_set_stone_quality(int value)
{
  settings_stone_quality = value;
}

void settings_set_stars_rotating(int value)
{
  settings_stars_rotating = value;
}

void settings_set_particles(int value)
{
  settings_particles = value;
}

void settings_set_volume(int value)
{
  settings_volume = value;
}

int settings_get_color()
{
  return settings_color;
}

int settings_get_difficult()
{
  return settings_difficult;
}

int settings_get_mode()
{
  return settings_mode;
}

int settings_get_control()
{
  return settings_control;
}

void settings_set_color(int value)
{
  settings_color = value;
}

void settings_set_difficult(int value)
{
  settings_difficult = value;
}

void settings_set_mode(int value)
{
  settings_mode = value;
}

void settings_set_control(int value)
{
  settings_control = value;
}

void settings_set_highscore_choice(int value)
{
  highscore_choice = value;
}

int settings_get_highscore_choice()
{
  return highscore_choice;
}

spFontPointer settings_get_font()
{
  return settings_font;
}

void settings_set_font(spFontPointer font)
{
  settings_font = font;
}

spFontPointer settings_get_small_font()
{
  return settings_small_font;
}

void settings_set_small_font(spFontPointer small_font)
{
  settings_small_font = small_font;
}

spFontPointer settings_get_middle_font()
{
  return settings_middle_font;
}

void settings_set_middle_font(spFontPointer middle_font)
{
  settings_middle_font = middle_font;
}

spFontPointer settings_get_countdown_font()
{
  return settings_countdown_font;
}

void settings_set_countdown_font(spFontPointer countdown_font)
{
  settings_countdown_font = countdown_font;
}

void settings_reset_highscore_name(char* name)
{
  name[0]=settings_name[0];
  name[1]=settings_name[1];
  name[2]=settings_name[2];
}

void settings_set_highscore_name(char* name)
{
  settings_name[0]=name[0];
  settings_name[1]=name[1];
  settings_name[2]=name[2];
}

int settings_get_first_start()
{
  return settings_first_start;
}
