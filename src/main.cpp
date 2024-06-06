#include <SFML/Graphics.hpp>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
int main()
{

    char label[40];
    sprintf(label, "Hello, FLTK %d.%d.%d !", 
            FL_MAJOR_VERSION, FL_MINOR_VERSION, FL_PATCH_VERSION);
    printf("%s\n", label); fflush(stdout);
    Fl_Window *window2 = new Fl_Window(340, 120);
    Fl_Box *box = new Fl_Box(20, 20, 300, 80, label);
    box->box(FL_FLAT_BOX);
    box->labelfont(FL_BOLD + FL_ITALIC);
    box->labelsize(24);
    window2->end();
    window2->show();
    
    auto window = sf::RenderWindow{ { 1920u, 1080u }, "CMake SFML Project" };
    window.setFramerateLimit(144);

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
        window.display();
    }

    
}
