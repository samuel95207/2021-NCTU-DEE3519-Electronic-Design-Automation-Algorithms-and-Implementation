#include <iostream>
#include <sstream>
#include <string>

#include "Grid.h"

using namespace std;


Grid::Grid(int height, int width) : height(height), width(width)
{
    setSize(height, width);
}

void Grid::setSize(int height_in, int width_in){
    height = height_in;
    width = width_in;
    gridbox.clear();
    for(int y = 0; y < height;y++){
        vector<GridBox> row;
        for(int x = 0;x < width;x++){
            row.push_back(GridBox(x,y,false));
        }
        gridbox.push_back(row);
    }
}

void Grid::addBlock(int leftDownX, int leftDownY, int rightUpX, int rightUpY){
    for(int y = leftDownY; y <= rightUpY;y++){
        for(int x = leftDownX; x <= rightUpX;x++){
            gridbox[y][x].setObstacle();
        }
    }
}


std::istream &operator>>(std::istream &in, Grid &G){
    string line;
    istringstream iss;

    int col, row, blockNum;

    while(getline(in, line)){
        iss.clear();
        iss.str(line);

        string selection;
        iss>>selection;

        if(selection == ".row"){
                iss>>row;
        }else if(selection == ".col"){
                iss>>col;
        }else if(selection == ".blk"){
            G.setSize(row, col);
            iss>>blockNum;
            for(int i = 0;i < blockNum;i++){
                getline(in, line);
                
                iss.clear();
                iss.str(line);

                int leftDownX, leftDownY, rightUpX, rightUpY;
                iss>>leftDownX>>leftDownY>>rightUpX>>rightUpY;
                G.addBlock(leftDownX, leftDownY, rightUpX, rightUpY);
            }

        }else if(selection == ".net"){

        }


    }


    return in;
}


std::ostream &operator<<(std::ostream &out, const Grid G){
    cout<<"out"<<endl;
    cout<<G.height<<" "<<G.width<<endl;
    for(int y = G.height-1; y >= 0;y--){
        for(int x = 0; x < G.width;x++){
            out<<G.gridbox[y][x].isObstacle();
        }
        out<<"\n";
    }
    return out;
}



