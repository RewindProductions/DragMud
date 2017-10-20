#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>

using namespace std;

const int MAP_HIEGHT = 20;
const int MAP_WIDTH = 20;
const int UI_LENGTH = 18;
const int UI_HIEGHT = 4;
const int TOTAL_IDS = 8;

bool running = true;
bool editor_mode = true;

typedef struct player
{
    string name;
    string char_class;
    int level;
    int x;
    int y;
    int max_health;
    int cur_health;
} Player;

typedef struct enemy
{
    string name;
    int x;
    int y;
    int cur_health;
    int damage;
} Enemy;

int tile_id[TOTAL_IDS];
string tile_name[TOTAL_IDS];

char keypressed;
int map[MAP_HIEGHT][MAP_WIDTH];
int cur_id = 3;

void hide_cursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void init_tiles()
{
    for(int i = 0; i < TOTAL_IDS; ++i)
    {
        tile_id[i] = 255;
        tile_name[i] = "Null";
    }

    tile_id[0] = 255; tile_name[0] = "Plain   ";
    tile_id[1] = 24; tile_name[1] = "Forest  ";
    tile_id[2] = 231; tile_name[2] = "Grove   ";
    tile_id[3] = 176; tile_name[3] = "Water   ";
    tile_id[4] = 177; tile_name[4] = "Road    ";
    tile_id[5] = 30; tile_name[5] = "Mountain";
}

void display_ascii()
{
    for(int i = 0; i < 256; ++i)
    {
        if(i == 7)
            continue;
        printf("   %d: %c", i ,i);

        if(i % 3 == 0)
            printf("\n");
    }
}

void init_map()
{
    for(int i = 0; i < MAP_HIEGHT; ++i)
    {
        for(int j = 0; j < MAP_WIDTH; ++j)
        {
            map[j][i] = 255;
        }
    }
}

bool is_near(player Player, enemy Enemy)
{
    if(Enemy.x + 1 == Player.x || Enemy.x - 1 == Player.x)
        return true;
    else if(Enemy.y + 1 == Player.y || Enemy.y - 1 == Player.y)
        return true;
    else
        return false;
}

void set_cursor_pos(int x, int y)
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = x;
    Position.Y = y;
    SetConsoleCursorPosition(hOut, Position);
}

void draw_border(int m, int n, int o, int p)
{
    printf("%c", m);

    for(int i = 0; i < 9; ++i)
        printf("%c", 205);

    printf("%c", n);

    for(int i = 0; i < 9; ++i)
        printf("%c", 205);

    printf("%c", o);

    for(int i = 0; i < 8; ++i)
        printf("%c", 205);

    printf("%c\n", p);
}

void draw_map(player Player)
{
    draw_border(201, 203, 203, 187);
    int count = -1;

    for(int i = Player.y - 3; i < Player.y + 4; ++i)
    {
        printf("%c", 186);

        for(int j = Player.x - 4; j < Player.x + 5; ++j)
        {
            if(i == Player.y && j == Player.x)
            {
                printf("@");
                continue;
            }

            if(i < 1 || j < 1 || i > MAP_HIEGHT -1 || j > MAP_WIDTH -1)
            {
                printf("%c", 176);
                continue;
            }
            printf("%c", map[j][i]);
        }

        if(count != 3)
            printf("%c", 186);
        else
            printf("%c", 204);

        int name_length = Player.name.length();

        ++count;
        if(count == 0)
        {
            cout << Player.name;

            for(int i = name_length; i < 9; ++i)
                printf(" ");
            printf("%c", 186);

            printf("level %d", Player.level);

            int level_name_length;

            if(Player.level > 9)
                level_name_length = 1;
            else
                level_name_length = 2;

            for(int i = level_name_length + 5; i < 8; ++i)
                printf(" ");
            printf("%c\n",186);
            continue;
        }
        if(count == 1)
        {
            cout << "HP:"<< Player.cur_health << "/" << Player.max_health;

            printf(" %c", 186);

            cout << Player.char_class;

            for(int i = Player.char_class.length(); i < 8; ++i)
                printf(" ");
            printf("%c\n", 186);
            continue;
        }

        if(count == 4)
        {
            for(int i = 0; i < 9; ++i)
            {
                printf("%c", 205);
            }
            printf("%c", 185);

            for(int i = 10; i < UI_LENGTH; ++i)
                printf(" ");
            printf("%c\n", 186);
            continue;
        }

        if(count == 5)
        {
            printf("    %c    %c", 31, 186);

            for(int i = 10; i < UI_LENGTH; ++i)
                printf(" ");
            printf("%c\n", 186);
            continue;
        }

        if(count == 6)
        {
            printf("%c %c %c %c %c%c", tile_id[cur_id - 2], tile_id[cur_id - 1], tile_id[cur_id], tile_id[cur_id + 1], tile_id[cur_id + 2], 186);

            for(int i = 10; i < UI_LENGTH; ++i)
                printf(" ");
            printf("%c\n", 186);
            continue;
        }

        for(int i = 0; i < UI_LENGTH; ++i)
            printf(" ");
        printf("%c", 186);

        printf("\n");

    }
    draw_border(204, 202, 202, 185);

    for(int i = 0; i < UI_HIEGHT; ++i)
    {
        printf("%c", 186);
        for(int i = 0; i < UI_LENGTH + 10; ++i)
            printf(" ");
        printf("%c\n", 186);
    }
    draw_border(200, 205, 205, 188);
}

void redraw(player Player)
{
    set_cursor_pos(0, 0);
    draw_map(Player);
}

void move_player(int dx, int dy, char key, player* Player)
{
    int newX = Player->x + dx;
    int newY = Player->y + dy;

    if(newX < MAP_WIDTH && newX > 0 && newY < MAP_HIEGHT && newY > 0)
    {
        if(map[newX][newY] != 176 || editor_mode == true)
        {
            if(keypressed == key)
            {
                Player->x = newX;
                Player->y = newY;
            }
        }
    }
}

void save_map()
{
    string filename;
    cout << "Save as >> ";
    cin >> filename;

    ofstream myfile;
    filename += ".txt";

    myfile.open(filename.c_str());

    for(int i = 0; i < MAP_HIEGHT; ++i)
    {
        for(int j = 0; j < MAP_WIDTH; ++j)
        {
            myfile << map[i][j] << " ";
        }
        myfile << "\n";
    }

    myfile.close();
    system("cls");
}

void load_map()
{
    string filename;

    cout << "load as >> ";
    cin >> filename;

    filename += ".txt";

    ifstream myfile(filename.c_str());

    for(int i = 0; i < MAP_HIEGHT; ++i)
    {
        for(int j = 0; j < MAP_WIDTH; ++j)
        {
            myfile >> map[i][j];
        }
    }        printf("%c", 186);

    myfile.close();
    system("cls");
}

string detect_cur_tile(player Player)
{
    string cur_tile = "Blank";

    for(int i = 0; i < TOTAL_IDS - 2; ++i)
    {
        if(map[Player.x][Player.y] == tile_id[i])
        {
            cur_tile = tile_name[i];
            break;
        }
    }

    return cur_tile;
}

void set_tile(int tile, char key, player* Player)
{
    if(keypressed == key)
        map[Player->x][Player->y] = tile;
}

void get_player_input(player* Player)
{
    keypressed = getch();

    move_player(1, 0, 'd', Player);
    move_player(-1, 0, 'a', Player);
    move_player(0, 1, 's', Player);
    move_player(0, -1, 'w', Player);

    set_tile(255, 'm', Player);

    if(editor_mode == true)
        set_tile(tile_id[cur_id], ' ', Player);

    if(keypressed == '\\')
        load_map();

    if(keypressed == ']')
        save_map();

    if(keypressed == 'o')
    {
        if(editor_mode == true)
            editor_mode = false;
        else
            editor_mode = true;
    }

    if(keypressed == ',' && cur_id > 1)
        --cur_id;
    if(keypressed == '.' && cur_id < TOTAL_IDS - 3)
        ++cur_id;

    if(keypressed == '`')
        running = false;
}

//6x8
//8x12

void init_player(player* Player, string name, string char_class, int x, int y)
{
    Player->name = name;
    Player->char_class = char_class;
    Player->x = x;
    Player->y = y;
    Player->max_health = 10;
    Player->cur_health = 10;
    Player->level = 1;
}

int main()
{
    player Player;
    init_player(&Player, "Guest", "Warrior", 10, 10);

    init_map();
    init_tiles();
    draw_map(Player);
    display_ascii();

    redraw(Player);
    while(running)
    {
        hide_cursor();
        get_player_input(&Player);
        redraw(Player);

        string test = detect_cur_tile(Player);
        cout << "Tile: " << test << endl;
        Sleep(60);
    }

    return 0;
}
