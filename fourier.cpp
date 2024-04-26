// g++ -c .\fourier.cpp -ID:\Workspace\SFML-2.6.1\include
// g++ .\fourier.o -o Fourier -LD:\Workspace\SFML-2.6.1\lib -lsfml-graphics -lsfml-window -lsfml-system

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <cmath>

using namespace std;
using namespace sf;

int width = 1250;
int height = 1000;

struct CircleComp{
    double radius;
    double speed;
    double start;
    Color color;
    CircleComp(double r, double sp, double st){ 
        radius = r; 
        speed = sp; 
        start = st;
        color = Color(rand()%255,rand()%255,rand()%255);
    }
}typedef CircleComp;

Vertex getNextPoint(Vertex p, CircleComp c, int t){
    Vertex res(Vector2f(c.radius * cos(c.start + c.speed * t), c.radius * sin(c.start + c.speed * t)));
    res.position.x += p.position.x;
    res.position.y += p.position.y;
    return res;
}
void circleIteration(CircleComp c, Vertex& last, RenderWindow& window, int t){
    CircleShape curCir;
    curCir.setRadius(c.radius);
    curCir.setPosition(last.position);
    curCir.setOrigin(c.radius,c.radius);
    curCir.setFillColor(Color::Transparent);
    curCir.setOutlineColor(c.color);
    curCir.setOutlineThickness(2);
    window.draw(curCir);

    Vertex next = getNextPoint(last,c,t);
    last.color = c.color;
    next.color = c.color;
    Vertex line[] = {last,next};
    last = next;
    window.draw(line, 2, Lines);
}

int main(){
    // create the window

    RenderWindow window(VideoMode(width, height), "My window");
    double pi = atan(1)*4;

    int graphStart = 500;
    int pointAmount = 1000;
    VertexArray graph(LineStrip,pointAmount);

    int t = 0;
    Vertex startPoint(Vector2f(graphStart/2,height/2));

    double circleSize = 100;
    double spd = (double)1/50;

    vector<CircleComp> circles;
    for(int i = 0; i < 100; i++){
        // Rect
        circles.push_back(CircleComp(circleSize/(2*i+1), spd*(2*i+1), 0));
        // Sawtooth
        circles.push_back(CircleComp(circleSize/(i+1)*pow(-1,i), spd*i, 0));
        // Blunt sawtooth
        circles.push_back(CircleComp(circleSize/((i+1)*(i+1)), spd*i, 0));
    }
    //circles.push_back(CircleComp(circleSize/2, spd, 0));
    //circles.push_back(CircleComp(circleSize/3, -spd, 0));
    //circles.push_back(CircleComp(circleSize/4, spd/2, 0));
    // circles.push_back(CircleComp(circleSize, spd, 0));
    // circles.push_back(CircleComp(circleSize/2, -spd*1.5, 0));
    // circles.push_back(CircleComp(circleSize/3, spd/3, 0));

    Vertex first = startPoint;
    for(int i = 0; i < circles.size(); i++) first = getNextPoint(first,circles[i],0);
    first.position.x = graphStart;
    for(int i = 0; i < pointAmount; i++) graph[i] = first;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(Color::Black);

        // draw everything here...
        // window.draw(...);

        Vertex last = startPoint;
        for(int i = 0; i < circles.size(); i++) circleIteration(circles[i],last,window,t);

        last.color = Color::White;
        Vertex graphNewPoint = last;
        graphNewPoint.position.x = graphStart;
        Vertex graphAdder[] = {
            last,
            graphNewPoint
        };
        window.draw(graphAdder,2,Lines);
        Vertex sepLine[] = {
            Vertex(Vector2f(graphStart,0)),
            Vertex(Vector2f(graphStart,height))
        };
        window.draw(sepLine,2,Lines);

        for(int i = pointAmount-1; i >= 1; i--){
            graph[i-1].position.x++;
            graph[i] = graph[i-1];
        }
        graph[0] = graphNewPoint.position;
        window.draw(graph);

        t--;

        // end the current frame
        window.display();
    }

    return 0;
}