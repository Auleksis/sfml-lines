#include "Vector.h"
#include "TextTools.h"
#include <vector>
#include<list>
#include "imgui.h"
#include "imgui-SFML.h"
#include"misc/cpp/imgui_stdlib.h"
#include "SFML/System/Clock.hpp"
#include <sstream>
#include <iostream>

using namespace sf;
int main()
{
    int WIDTH = 800;
    int HEIGHT = 600;    

    int CENTER_X = WIDTH / 2;
    int CENTER_Y = HEIGHT / 2;

    float rotation_velocity = 5.f;

    Font *font = new Font();
    font->loadFromFile("BRLNSB.TTF");


    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Vectors and polygons");    
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    //for imgui start

    char vector_id[256] = "1";
    int int_v_id = 1;
    Vector* currentVector = nullptr;
    int selected_item = 0;

    std::string label_angle = "";
    std::string label_vector_size = "";
    bool show_next_line = false;
    //for imgui end


    std::list<Vector*> vectors = std::list<Vector*>();    

    Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();      
            if (event.type == Event::KeyPressed) {
                if (currentVector == nullptr && vectors.size() > 0)
                    currentVector = vectors.back();
                if (Keyboard::isKeyPressed(Keyboard::Right)) {
                    currentVector->rotate(rotation_velocity);
                    label_angle = std::to_string(currentVector->getAngle());
                }
                if (Keyboard::isKeyPressed(Keyboard::Left)) {
                    currentVector->rotate(-rotation_velocity);
                    label_angle = std::to_string(currentVector->getAngle());
                }
            }
        }        

        //ImGui start

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Settings");

        ImGui::SetWindowSize(ImVec2(320, 350));
        ImGui::SetWindowPos(ImVec2(0, 0));
        
        //list box is so strange!
        ImGui::BeginListBox("Vectors", ImVec2(100, 100));
        for (auto v : vectors) {            
            std::string output = "Vector " + std::to_string(v->getID());
            if (ImGui::Selectable(output.c_str(), false, 0, ImVec2(90, 15))) {
                int_v_id = v->getID();
                currentVector = v;
                label_angle = std::to_string(currentVector->getAngle());
                label_vector_size = std::to_string(currentVector->getLength());
                show_next_line = currentVector->getShowNextLine();
            }
        }
        ImGui::EndListBox();

        std::string label_id = "Curent Vector is " + std::to_string(int_v_id);
        ImGui::LabelText("", label_id.c_str());       

        if (currentVector != nullptr) {           
            ImGui::InputText("Vector angle", &label_angle);
            if (ImGui::Button("Set Angle")) {
                currentVector->setAngle(std::stof(label_angle));
            }

            ImGui::InputText("Vector length", &label_vector_size);
            if (ImGui::Button("Resize")) {
                currentVector->resize(std::stof(label_vector_size));
            }

            if (ImGui::Checkbox("Show next line", &show_next_line)) {
                currentVector->setShowNextLine(show_next_line);
            }

            if (ImGui::Button("Remove Vector")) {
                vectors.remove(currentVector);
                currentVector = nullptr;
            }
        }

        if (ImGui::Button("Add new Vector")) {
            Vector* v = new Vector(font, CENTER_X, CENTER_Y, CENTER_X, CENTER_Y + 100);
            vectors.emplace_back(v);      
            std::string id = std::to_string(v->getID());
        }

        ImGui::End();

        //ImGui end

        window.clear();

        ImGui::SFML::Render(window);
        
        
        for (auto v : vectors) {
            v->draw(window);
        }

        window.display();
    }

    ImGui::SFML::Shutdown();
    delete font;

    return 0;
}