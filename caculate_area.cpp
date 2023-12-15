#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Output.H>
#include <vector>
#include<FL/Fl_Input.H>
#include<FL/Fl_Button.H>
#include <cmath>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;


//先实现一个简单的gui界面
//再界面里，用户输出比例尺。
//然后用户点击界面上的点，然后程序计算出像素面积和真实。(点击标记点，按住enter后形成封闭多边形并且高亮。然后计算面积
//并且显示出来。
class caculate_area : public Fl_Window {
    public:
    Fl_Input *input;
    Fl_Output *output;
    Fl_PNG_Image *image;
    vector<pair<int, int>> points;
    vector<pair<int, int>> points_scale;
    bool completed = false;
    double scale = 0;
    double area = 0;



    caculate_area(int w, int h, const char *title = 0) : Fl_Window(w, h, title) {
        image = new Fl_PNG_Image("./fudan.png");
        Fl_Box *box = new Fl_Box(0, 0, w, h);
        box->image(image);
        box->box(FL_FLAT_BOX);
        box->align(FL_ALIGN_IMAGE_BACKDROP);
        box->redraw();
        input = new Fl_Input(100, 100, 100, 30, "scale");
        output = new Fl_Output(100, 150, 100, 30, "area");
        end();
    }

    void draw() {
        //draw函数和redraw的关系是：redraw会调用draw函数。
        Fl_Window::draw();
        image->draw(0, 0, w(), h());
        for (auto &p : points) {
            fl_color(FL_RED);
            fl_pie(p.first-5, p.second-5, 10, 10, 0, 360);
        }

        if (completed && points.size() > 2) {
            fl_color(FL_GREEN);
            fl_begin_complex_polygon();
            for (auto &p : points) {
                fl_vertex(p.first, p.second);
            }
            fl_end_complex_polygon();
        }
        
        //让用户输入比利尺
        if (scale == 0)
        {


            
            std::cout<<"多少像素对应多少米，请输入两个参数"<<std::endl;
            double pix = 0;
            double meter = 0;
            cin >> pix >> meter;
            scale = meter / pix;

            std::cout<<"比例尺是"<<scale<<std::endl;
            


            
            std::cout<<"请在图像中左键点击标记点，如果想撤回，请按backspace键，按住enter后形成封闭多边形并且高亮。程序会计算面积并且显示出来。"<<std::endl;
        }

    }




    int handle(int event) {
        switch (event) {
            case FL_PUSH:
                if (Fl::event_button() == FL_LEFT_MOUSE  ) {
                    int x = Fl::event_x();
                    int y = Fl::event_y();
                    points.emplace_back(x, y);
                    redraw();                   
                }else{
                    int x = Fl::event_x();
                    int y = Fl::event_y();
                    points.emplace_back(x, y);
                }
                return 1;
            case FL_KEYDOWN:
                if (Fl::event_key() == FL_Enter) {
                    if (points.size() > 2) {
                        completed = true;
                        redraw();
                        double area = 0;
                        for (int i = 0; i < points.size(); i++) {
                            int j = (i + 1) % points.size();
                            area += points[i].first * points[j].second - points[j].first * points[i].second;
                        }
                        area = abs(area) / 2;
                        double area_true = area * scale * scale;
                        std::cout << "像素面积是" << area << std::endl;

                        //写成科学表达式
                        std::cout<<scientific<<setprecision(6)<<"真实面积是" << area_true <<"(m^2)"<< std::endl;
                        // std::cout << "真实面积是" << area_true <<"(m^2)"<< std::endl;
                        
                    }
                }
                return 1;
            //实现撤销点
            case FL_KEYUP:
                if (Fl::event_key() == FL_BackSpace) {
                    if (points.size() > 0) {
                        points.pop_back();
                        redraw();
                    }
                }
                return 1;
            default:
                return Fl_Window::handle(event);
        }
    }

    void set_scale(double scale) {
        this->scale = scale;

    }

    double get_scale() {
        return scale;
    }

    void set_area(double area) {
        this->area = area;
    }

    double get_area() {
        return area;
    }   

};


int main(int argc, char **argv) {
    caculate_area *window = new caculate_area(800, 600, "caculate_area");
    window->show(argc, argv);
    return Fl::run();
}


