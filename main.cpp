#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <raylib.h>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

class Block{
private:
    Vector2 Pos; // Position of the block
    Vector2 TextPos; // Position of the text
    Vector2 Size; // Size of block
    int cell_size; // Cell Size
    int Value = 2; // Start with 2
    int offset = 4; // Offset to make it look pretty
    bool Empty;
    Color color;
    Color textColor;

    void SetColor(){
        Color newColor;
        textColor = (Color){255, 255, 255, 255};
        switch(Value){
            case 2:
                newColor = (Color){238, 228, 218, 255};
                textColor = (Color){119, 110, 101, 255};
                break;
            case 4:
                newColor = (Color){237, 224, 200, 255};
                textColor = (Color){119, 110, 101, 255};
                break;
            case 8:
                newColor = (Color){242, 177, 121, 255};
                break;
            case 16:
                newColor = (Color){245, 149, 99, 255};
                break;
            case 32:
                newColor = (Color){246, 124, 96, 255};
                break;
            case 64:
                newColor = (Color){246, 94, 59, 255};
                break;
            case 128:
                newColor = (Color){237, 207, 115, 255};
                break;
            case 256:
                newColor = (Color){237, 204, 98, 255};
                break;
            case 512:
                newColor = (Color){237, 200, 80, 255};
                break;
            case 1024:
                newColor = (Color){237, 197, 63, 255};
                break;
            case 2048:
                newColor = (Color){237, 194, 45, 255};
                break;
            default:
                newColor = (Color){204, 192, 178, 255};
                break;
        }
        color = newColor;
    }

public:
    int getValue(){
        return Value;
    }

    bool getEmpty(){
        return Empty;
    }

    void setEmpty(bool empty, int val=0){
        Empty = empty;
        if(Empty) Value = 0;
        else Value = val;
    }

    Block(Vector2 tempPos, int CELLSIZE, bool empty=false){
        cell_size = CELLSIZE;
        Size = {(float)cell_size - offset * 2, (float)cell_size - offset * 2};
        Vector2 pos = {tempPos.x * cell_size, tempPos.y * cell_size};
        Pos = {pos.x + offset, pos.y + offset};
        // TextPos = {pos.x + (float)cell_size/2, pos.y + (float)cell_size/2};
        Empty = empty;
        if(Empty) Value = 0;

    }

    void Show(Font font){
        // if(Empty) return;
        SetColor();

        Rectangle rect = {Pos.x, Pos.y, Size.x, Size.y};
        DrawRectangleRounded(rect, 0.2, 10, color);

        if(!Empty){
            std::string tmp = std::to_string(Value);
            char const *num_char = tmp.c_str();

            float font_size = 40;
            if(Value < 10) font_size = 60;
            else if(Value < 100) font_size = 50;
            else if(Value < 1000) font_size = 40;
            else if(Value < 10000) font_size = 30;

            Vector2 vec = MeasureTextEx(font, num_char, font_size, 0);
            TextPos.x = Pos.x + (float)cell_size/2 - vec.x/2;
            TextPos.y = Pos.y + (float)cell_size/2 - vec.y/2;

            DrawTextEx(font, num_char, TextPos, font_size, 0, textColor);
            //DrawText(num_char, (int)TextPos.x, (int)TextPos.y, 60, textColor);
        }
    }
};


std::vector<std::vector<Block>> EnableRandomBlock(std::vector<std::vector<Block>> Board, int ROWS){
    int x1 = int((float)rand()/(float)RAND_MAX * ROWS);
    int y1 = int((float)rand()/(float)RAND_MAX * ROWS);
    if(Board[x1][y1].getEmpty()) Board[x1][y1].setEmpty(false, 2);
    return Board;
}

int main(void){

    const int WIDTH = 400;
    const int HEIGHT = 400;
    const int FPS = 60;
    const int ROWS = 4;
    const int CELLSIZE = WIDTH / ROWS;

    int score = 0;

    // Random numbers
    srand((unsigned)time(NULL));

    InitWindow(WIDTH, HEIGHT, "Title");
    SetTargetFPS(FPS);

    // Font loading
    Font robaga = LoadFontEx("rounded-font.otf",96,0,0);

    std::vector<std::vector<Block>> Board;
    for(int i = 0; i < ROWS; i++){
        std::vector<Block> newBlock;
        Board.push_back(newBlock);
        for(int j = 0; j < ROWS; j++){
            // All Empty
            Board[i].push_back(Block({(float)i, (float)j}, CELLSIZE, true));
        }
    }

    // Fill 1 or 2 Random Blocks
    int x1 = int((float)rand()/(float)RAND_MAX * ROWS);
    int y1 = int((float)rand()/(float)RAND_MAX * ROWS);
    Board[x1][y1].setEmpty(false, 2);

    int x2 = int((float)rand()/(float)RAND_MAX * ROWS);
    int y2 = int((float)rand()/(float)RAND_MAX * ROWS);

    while (x1 == x2) {
        x2 = int((float)rand()/(float)RAND_MAX * ROWS);
    }

    Board[x2][y2].setEmpty(false, 2);

    std::cout << x1 << " " << y1 << " | " << x2 << " " << y2 << "\n";

    // ---------

    while(!WindowShouldClose()){
        // Update
        if(IsKeyPressed(KEY_LEFT)){
            std::cout << "Left Pressed\n";
            int changes = -1;
            while(changes != 0){
                changes = 0;
                for(int j = 0; j < ROWS; j++){
                    for(int i = 1; i < ROWS; i++){
                        if(Board[i][j].getEmpty()){
                            continue;
                        }
                        bool w = Board[i][j].getValue() == Board[i-1][j].getValue();
                        if(w){
                            score+=2*Board[i][j].getValue();
                            Board[i-1][j].setEmpty(false, 2 * Board[i][j].getValue());
                            Board[i][j].setEmpty(true);
                            changes++;
                        }else if(Board[i-1][j].getEmpty()){
                            Board[i-1][j].setEmpty(false, Board[i][j].getValue());
                            Board[i][j].setEmpty(true);
                            changes++;
                        }else{
                            continue;
                        }
                    }
                }
            }
            Board = EnableRandomBlock(Board, ROWS);
        }
        if(IsKeyPressed(KEY_RIGHT)){
            std::cout << "Right Pressed\n";
            int changes = -1;
            while(changes != 0){
                changes = 0;
                for(int j = 0; j < ROWS; j++){
                    for(int i = ROWS - 2; i >= 0; i--){
                        if(Board[i][j].getEmpty()) continue;
                        bool w = Board[i][j].getValue() == Board[i+1][j].getValue();
                        if(w){
                            score+=2*Board[i][j].getValue();
                            Board[i+1][j].setEmpty(false, 2 * Board[i][j].getValue());
                            Board[i][j].setEmpty(true);
                            changes++;
                        }else if(Board[i+1][j].getEmpty()){
                            Board[i+1][j].setEmpty(false, Board[i][j].getValue());
                            Board[i][j].setEmpty(true);
                            changes++;
                        }else{
                            continue;
                        }
                    }
                }
            }
            Board = EnableRandomBlock(Board, ROWS);
        }
        if(IsKeyPressed(KEY_UP)){
            std::cout << "Up Pressed\n";
            int changes = -1;
            while(changes != 0){
                changes = 0;
                for(int i = 0; i < ROWS; i++){
                    for(int j = 1; j < ROWS; j++){
                        if(Board[i][j].getEmpty()) continue;
                        bool w = Board[i][j].getValue() == Board[i][j-1].getValue();
                        if(w){
                            score+=2*Board[i][j].getValue();
                            Board[i][j-1].setEmpty(false, 2 * Board[i][j].getValue());
                            Board[i][j].setEmpty(true);
                            changes++;
                        }else if(Board[i][j-1].getEmpty()){
                            Board[i][j-1].setEmpty(false, Board[i][j].getValue());
                            Board[i][j].setEmpty(true);
                            changes++;
                        }else{
                            continue;
                        }
                    }
                }
            }
            Board = EnableRandomBlock(Board, ROWS);
        }
        if(IsKeyPressed(KEY_DOWN)){
            std::cout << "Down Pressed\n";
            int changes = -1;
            while(changes != 0){
                changes = 0;
                for(int i = 0; i < ROWS; i++){
                    for(int j = ROWS - 2; j >= 0; j--){
                        if(Board[i][j].getEmpty()) continue;
                        bool w = Board[i][j].getValue() == Board[i][j+1].getValue();
                        if(w){
                            score+=2*Board[i][j].getValue();
                            Board[i][j+1].setEmpty(false, 2 * Board[i][j].getValue());
                            Board[i][j].setEmpty(true);
                            changes++;
                        }else if(Board[i][j+1].getEmpty()){
                            Board[i][j+1].setEmpty(false, Board[i][j].getValue());
                            Board[i][j].setEmpty(true);
                            changes++;
                        }else{
                            continue;
                        }
                    }
                }
            }
            Board = EnableRandomBlock(Board, ROWS);
        }

        // Show
        BeginDrawing();
        ClearBackground((Color){187, 173, 160, 255});
        
        // Show all blocks
        for(int i = 0; i < Board.size(); i++){
            for(int j = 0; j < Board[0].size(); j++){
                Board[i][j].Show(robaga);
            }
        }

        std::stringstream ss;
        ss << "Score : ";
        ss << score;
        DrawText(ss.str().c_str(), 150, 180, 20, GRAY);

        EndDrawing();
    }

    return 0;
}
