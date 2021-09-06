// sfml_01.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

int main()
{
    // first param and second are the size and name of window
    // resize is enable resize, close is enable the close option, but if we don't write param three, we still have the function
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window"); //, sf::Style::Resize | sf::Style::Close);

    // window.setVerticalSyncEnabled(true); set refresh rate to same as monitor 
    window.setFramerateLimit(60); // same as above, when monitor is 60

    window.setKeyRepeatEnabled(false);

    bool play = true;
    sf::Event event;
    sf::Font font;
    if (!font.loadFromFile("data/font1.otf")) {
        // code when fail
        return 0;
    }
    
    // text
    sf::Text score;
    score.setFont(font);
    score.setCharacterSize(30);
    score.setFillColor(sf::Color::Red);
    score.setPosition(350, 10);
    score.setString("0 : 0");

    // images
    sf::Texture tex_pad;
    sf::Texture tex_ball;
    sf::Texture tex_background;
    if (!tex_pad.loadFromFile("data/box.png")) 
    {
        return -1;
    }
    if (!tex_ball.loadFromFile("data/ball.png"))
    {
        return -1;
    }
    if (!tex_background.loadFromFile("data/background.png"))
    {
        return -1;
    }

    // sounds
    sf::SoundBuffer buff_hit;
    if (!buff_hit.loadFromFile("data/ball.ogg"))
    {
        return -1;
    }

    sf::Sound hit;
    hit.setBuffer(buff_hit);

    // states
    bool up = false, down = false;


    // Variables
    int yVelocityPad1 = 0;
    int xVelocityBall = -3;
    int yVelocityBall = -3;
    int yVelocityPad2 = 0;
    int pd1score = 0;
    int pd2score = 0;

    /////////// Shape
    // -background
    sf::RectangleShape background;
    background.setSize(sf::Vector2f(800, 600));
    background.setPosition(0, 0);
    background.setTexture(&tex_background);

    // pads
    sf::RectangleShape pd1;
    pd1.setSize(sf::Vector2f(50, 100));
    pd1.setPosition(50, 250);
    pd1.setTexture(&tex_pad);

    sf::RectangleShape pd2;
    pd2.setSize(sf::Vector2f(50, 100));
    pd2.setPosition(700, 250);
    pd2.setTexture(&tex_pad);

    // ball
    sf::RectangleShape ball;
    ball.setSize(sf::Vector2f(50, 50));
    ball.setPosition(400, 270);
    ball.setTexture(&tex_ball);

    while (play)
    {
        while (window.pollEvent(event))
        {
            // close the window when click X
            if (event.type == sf::Event::Closed)
            {
                play = false;
            }
            // press key
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    up = true;
                }
                if (event.key.code == sf::Keyboard::Down)
                {
                    down = true;
                }
            }
            // release key
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    up = false;
                }
                if (event.key.code == sf::Keyboard::Down)
                {
                    down = false;
                }
            }
        }
        // reset
        yVelocityPad1 = 0;
        yVelocityPad2 = 0;
        // Logic handling
        if (up)
        {
            yVelocityPad1 -= 5;
        }
        if (down)
        {
            yVelocityPad1 += 5; 
        }

        pd1.move(0, yVelocityPad1);

        // out of screen
        if (pd1.getPosition().y < 0) 
        {
            pd1.setPosition(50, 0);
        }
        if (pd1.getPosition().y > 500)
        {
            pd1.setPosition(50, 500);
        }

        ////Pad2
        if (ball.getPosition().y < pd2.getPosition().y)
        {
            yVelocityPad2 = -3;
        }
        if (ball.getPosition().y > pd2.getPosition().y)
        {
            yVelocityPad2 = 3;
        }

        pd2.move(0, yVelocityPad2);

        // ball
        ball.move(xVelocityBall, yVelocityBall);
        if (ball.getPosition().y < 0)
        {
            yVelocityBall = -yVelocityBall;
        }
        if (ball.getPosition().y > 500)
        {
            yVelocityBall = -yVelocityBall;
        }

        if (ball.getGlobalBounds().intersects(pd1.getGlobalBounds()))
        {
            if (xVelocityBall < 0)
            {
                xVelocityBall -= 1;
            }
            if (xVelocityBall > 0)
            {
                xVelocityBall += 1;
            }
            xVelocityBall = -xVelocityBall;
            hit.play();
        }
        if (ball.getGlobalBounds().intersects(pd2.getGlobalBounds()))
        {
            xVelocityBall = -xVelocityBall;
            hit.play();
        }

        if (ball.getPosition().x < -50)
        {
            pd2score++;
            xVelocityBall = -3;
            ball.setPosition(400, 270);
        }
        if (ball.getPosition().x > 800)
        {
            xVelocityBall = 3;
            pd1score++;
            ball.setPosition(400, 270);
        }

        // Rendering part of the main game loop
        //window.clear(sf::Color(255, 0, 0));
        window.clear();
        // Draw things
        window.draw(background); 
        window.draw(pd1);
        window.draw(pd2);
        window.draw(ball);

        //Score
        std::string text;
        text = std::to_string(pd1score) + " : " + std::to_string(pd2score);
        score.setString(text);
        window.draw(score);

        window.display(); // enable resize
    }
    return 0;
}


