#include <format>
#include <iostream>

#include "imgui.h"         // for ImGui::* functions
#include "imgui-SFML.h"    // for ImGui::SFML::* functions and SFML-specific overloads
#include <SFML/Graphics.hpp>
#include <math.h>

void ShowExampleMenuFile()
{
    ImGui::MenuItem("(demo menu)", NULL, false, false);
    if (ImGui::MenuItem("New"))
    {
    }
    if (ImGui::MenuItem("Open", "Ctrl+O"))
    {
    }
    if (ImGui::BeginMenu("Open Recent"))
    {
        ImGui::MenuItem("fish_hat.c");
        ImGui::MenuItem("fish_hat.inl");
        ImGui::MenuItem("fish_hat.h");
        if (ImGui::BeginMenu("More.."))
        {
            ImGui::MenuItem("Hello");
            ImGui::MenuItem("Sailor");
            if (ImGui::BeginMenu("Recurse.."))
            {
                ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
}

static int TextCallback(ImGuiInputTextCallbackData* data)
{
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        auto* str = static_cast<std::string*>(data->UserData);
        str->resize(data->BufTextLen);
        data->Buf = str->data();
    }
    return 0;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "ImGui + SFML Demo");
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window))
        return -1;

    bool* show_demo = new bool(true);
    float slider_value = 0.5f;
    float Frac{}, SizeX{}, SizeY{}, Overlay{};
    static const char* ExampleNames[] =
{
        "Artichoke", "Arugula", "Asparagus", "Avocado", "Bamboo Shoots", "Bean Sprouts", "Beans", "Beet", "Belgian Endive", "Bell Pepper",
        "Bitter Gourd", "Bok Choy", "Broccoli", "Brussels Sprouts", "Burdock Root", "Cabbage", "Calabash", "Capers", "Carrot", "Cassava",
        "Cauliflower", "Celery", "Celery Root", "Celcuce", "Chayote", "Chinese Broccoli", "Corn", "Cucumber"
    };



    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        const ImGuiViewport* MainViewport = ImGui::GetMainViewport();

        // Show the standard ImGui demo window.
        ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(ImVec2{100, MainViewport->WorkSize.y});
        if (ImGui::Begin("Demo window", nullptr, ImGuiWindowFlags_MenuBar))
        {
            ImGui::ShowDemoWindow(show_demo);
            ImGui::End();
        }

        //NOTE: My STUFF Starting here
        ImGui::SetNextWindowPos(ImVec2{0, 0}, ImGuiCond_Appearing);
        ImGui::SetNextWindowSize(ImVec2{300, MainViewport->WorkSize.y});

        ImGui::Begin("My window", nullptr, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Menu"))
            {
                ShowExampleMenuFile();
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Examples"))
            {
                ImGui::EndMenu(); // Add this to close the "Examples" menu.
            }

            ImGui::EndMenuBar();

            //NOTE: Widgets
            if (ImGui::CollapsingHeader("Widgets"))
            {
                if (ImGui::TreeNode("Basic"))
                {
                    ImGui::SeparatorText("General");
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Hold to repeat:");
                    ImGui::SameLine();

                    static int counter = 0;
                    float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
                    ImGui::PushItemFlag(ImGuiItemFlags_ButtonRepeat, true);
                    if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { counter--; }
                    ImGui::SameLine();
                    if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { counter++; }
                    ImGui::PopItemFlag();
                    ImGui::SameLine();
                    ImGui::Text("%d", counter);
                    ImGui::TreePop();
                }

                //NOTE: Dynamic Input
                if (ImGui::TreeNode("Dynamic input"))
                {
                    for (int i = 0; i < 8; ++i)
                    {
                        ImGui::LabelText("sa", "%d", i);
                    }

                    static std::string input;
                    ImGui::InputText("Dynamic Input", input.data(), input.capacity() + 1,
                                     ImGuiInputTextFlags_CallbackResize, TextCallback, &input);
                    std::cout << input << std::endl;

                    ImGui::TreePop();
                }
                //NOTE: Enum selector
                if (ImGui::TreeNode("Enum selector"))
                {
                    enum Element { El_Fire, El_Earth, El_Air, El_Water, El_COUNT };
                    static int elem = El_Fire;
                    const char* el_names[El_COUNT] = {"Fire", "Earth", "Air", "Water"};
                    const char* el_name = (elem >= 0 && elem < Element::El_COUNT) ? el_names[elem] : "Unknown";
                    ImGui::SliderInt("Slider Enum", &elem, 0, Element::El_COUNT - 1, el_name);

                    ImGui::TreePop();
                }

                //NOTE: ProgressBar
                if (ImGui::TreeNode("ProgressBar In inifinite tooltip"))
                {
                    ImGui::DragFloat("Fraction: ", &Frac, 0.01, 0, 1);
                    ImGui::DragFloat("SizeX: ", &SizeX);
                    ImGui::DragFloat("SizeY: ", &SizeY);
                    ImGui::BeginTooltip();

                    Frac = sin(ImGui::GetTime()) * 0.5 + 0.5f;
                    ImGui::ProgressBar(Frac, ImVec2(SizeX, SizeY), std::format("{:.1f}%", Frac * 100.0f).c_str());
                    ImGui::EndTooltip();
                    ImGui::TreePop();
                }

                //NOTE: DRAG DROP (I am gladd finally I understood all) 
                static std::vector<std::string> drag_items = {"Item 0", "Item 1", "Item 2", "Item 3", "Item 4"};
                static int dragged_index = -1;

                if (ImGui::TreeNode("Drag drop"))
                {
                    // Display our list of draggable items
                    for (int i = 0; i < drag_items.size(); ++i)
                    {
                        ImGui::PushID(i);

                        // Create a selectable item for each element TreeNode also selectable
                        ImGui::Selectable(drag_items[i].c_str());

                        //Return true when dragging initiated
                        if (ImGui::BeginDragDropSource())
                        {
                            // Set payload (carry index)
                            dragged_index = i;
                            ImGui::SetDragDropPayload("DRAG_ITEM", &dragged_index, sizeof(int));

                            // Preview what's being dragged
                            ImGui::Text("Moving %s", drag_items[i].c_str());
                            ImGui::EndDragDropSource();
                        }

                        // Drag target: where we can drop
                        if (ImGui::BeginDragDropTarget())
                        {
                            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAG_ITEM")) //Invoke only when drop initiated
                            {
                                int payload_index = *static_cast<int*>(payload->Data);
                                if (payload_index != i)
                                {
                                    // Swap items
                                    std::swap(drag_items[payload_index], drag_items[i]);
                                }
                            }
                            ImGui::EndDragDropTarget();
                        }

                        ImGui::PopID();
                    }

                    // Optional: Add some visual feedback
                    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Drag items up/down to reorder!");
                    ImGui::TreePop();
                }

                //NOTE: Collapsing header
                if (ImGui::TreeNode("Collapsing Headers"))
                {
                    static bool closable_group = true;
                    ImGui::Checkbox("Show 2nd header", &closable_group);
                    if (ImGui::CollapsingHeader("Header", ImGuiTreeNodeFlags_None))
                    {
                        ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
                        for (int i = 0; i < 5; i++)
                            ImGui::Text("Some content %d", i);
                    }
                    if (ImGui::CollapsingHeader("Header with a close button", &closable_group))
                    {
                        ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
                        for (int i = 0; i < 5; i++)
                            ImGui::Text("More content %d", i);
                    }
                    if (ImGui::CollapsingHeader("Header with a bullet", ImGuiTreeNodeFlags_Bullet))
                        ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());

                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Images"))
                {
                    ImGuiIO& io = ImGui::GetIO();

                    ImTextureID my_tex_id = io.Fonts->TexID;
                    float my_tex_w = (float)io.Fonts->TexWidth;
                    float my_tex_h = (float)io.Fonts->TexHeight;
                    {
                        static bool use_text_color_for_tint = false;
                        ImGui::Checkbox("Use Text Color for Tint", &use_text_color_for_tint);
                        ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
                        ImVec2 pos = ImGui::GetCursorScreenPos();

                        ImVec2 uv_min = ImVec2(0.0f, 0.0f); // Top-left
                        ImVec2 uv_max = ImVec2(1.0f, 1.0f); // Lower-right
                        ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
                        ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
                        ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
                        if (ImGui::BeginItemTooltip())
                        {
                            float region_sz = 32.0f;
                            float region_x = io.MousePos.x - pos.x - region_sz;
                            float region_y = io.MousePos.y - pos.y - region_sz;
                            float zoom = 4.0f;

                            //Clamp to stay within texture bounds
                            if (region_x < 0.0f) { region_x = 0.0f; }
                            else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
                            if (region_y < 0.0f) { region_y = 0.0f; }
                            else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }

                            ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
                            ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);

                            // Maps the 32x32 pixel region to normalized UV coordinates. If texture is 512x512, 32px = 32/512 = 0.0625 in UV space
                            ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
                            ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);

                            //Thanks to zoom Renders the 32x32 region at 128x128 pixels (32 * 4). Scaling happens by rendering the small UV region at a larger size
                            ImGui::Image(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
                            ImGui::EndTooltip();
                        }
                    }

                    static int pressed_count = 0;
                    for (int i = 0; i < 8; i++)
                    {
                        // UV coordinates are often (0.0f, 0.0f) and (1.0f, 1.0f) to display an entire textures.
                        // Here are trying to display only a 32x32 pixels area of the texture, hence the UV computation.
                        // Read about UV coordinates here: https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
                        ImGui::PushID(i);
                        ImVec2 size = ImVec2(32.0f, 32.0f); // Size of the image we want to make visible
                        ImVec2 uv0 = ImVec2(i * 32.0f / my_tex_w, 0.0f);
                        ImVec2 uv1 = ImVec2((i + 1) * 32.0f / my_tex_w, 32.0f / my_tex_h);
                        ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // Black background
                        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
                        if (ImGui::ImageButton("", my_tex_id, size, uv0, uv1, bg_col, tint_col))
                            pressed_count += 1;
                        ImGui::PopID();
                        ImGui::SameLine();
                    }
                    ImGui::NewLine();
                    ImGui::Text("Pressed %d times.", pressed_count);
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("table"))
                {
                    if (ImGui::BeginTable("split2", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
                    {
                        static bool selected[10] = {};

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::Text("First");

                        ImGui::TableNextColumn();
                        ImGui::Text("Sec");
                        
                        for (int i = 0; i < 10; i++)
                        {
                            char label[32];
                            sprintf(label, "Item %d", i);
                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            ImGui::Text(label);

                            ImGui::TableNextColumn();
                            ImGui::Text("Some other contents");

                        }
                        ImGui::EndTable();
                    }

                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Selections"))
                {
                    if (ImGui::TreeNode("Single-Select"))
                    {
                        static int selected = -1;
                        for (int n = 0; n < 5; n++)
                        {
                            char buf[32];
                            sprintf(buf, "Object %d", n);
                            if (ImGui::Selectable(buf, selected == n))
                                selected = n;
                        }
                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode("Multi Select"))
                    {
                        static bool selection[5] = {false,false,false,false,false};
                        for (int n = 0; n < 5; n++)
                        {
                            char buf[32];
                            sprintf(buf, "Object %d", n);
                            if (ImGui::Selectable(buf,selection[n]))
                            {
                                //Clear selection when CTRL is not held
                                if (!ImGui::GetIO().KeyShift)
                                    memset(selection, 0, sizeof(selection));

                                selection[n] ^= 1;
                            }
                        }
                        
                        ImGui::TreePop();
                    }

                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Multi-Select (with clipper)"))
                {
                    // Use default selection.Adapter: Pass index to SetNextItemSelectionUserData(), store index in Selection
                    static ImGuiSelectionBasicStorage selection;

                    ImGui::Text("Added features:");
                    ImGui::BulletText("Using ImGuiListClipper.");

                    const int ITEMS_COUNT = 10000;
                    ImGui::Text("Selection: %d/%d", selection.Size, ITEMS_COUNT);
                    if (ImGui::BeginChild("##Basket", ImVec2(-FLT_MIN, ImGui::GetFontSize() * 20), ImGuiChildFlags_FrameStyle | ImGuiChildFlags_ResizeY))
                    {
                        ImGuiMultiSelectFlags flags = ImGuiMultiSelectFlags_ClearOnEscape | ImGuiMultiSelectFlags_BoxSelect1d;
                        ImGuiMultiSelectIO* ms_io = ImGui::BeginMultiSelect(flags, selection.Size, ITEMS_COUNT);
                        selection.ApplyRequests(ms_io);

                        ImGuiListClipper clipper;
                        clipper.Begin(ITEMS_COUNT);
                        if (ms_io->RangeSrcItem != -1)
                            clipper.IncludeItemByIndex((int)ms_io->RangeSrcItem); // Ensure RangeSrc item is not clipped.
                        while (clipper.Step())
                        {
                            for (int n = clipper.DisplayStart; n < clipper.DisplayEnd; n++)
                            {
                                char label[64];
                                sprintf(label, "Object %05d: %s", n, ExampleNames[n % IM_ARRAYSIZE(ExampleNames)]);
                                bool item_is_selected = selection.Contains((ImGuiID)n);
                                ImGui::SetNextItemSelectionUserData(n);
                                ImGui::Selectable(label, item_is_selected);
                            }
                        }

                        ms_io = ImGui::EndMultiSelect();
                        selection.ApplyRequests(ms_io);
                    }
                    ImGui::EndChild();
                    ImGui::TreePop();
                }
            }
        }

        ImGui::End();


        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    delete show_demo;
    return 0;
}
