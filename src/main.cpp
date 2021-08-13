

#include <opencv4/opencv2/opencv.hpp>
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc.hpp>
#include "Map.h"
#include "Astar.h"
#include <chrono>
cv::Mat process(cv::Mat input)
{

    cv::Mat out;
    cv::floodFill(input,cv::Point(0,0), cv::Scalar(255));
    cv::Mat orig;
    input.copyTo(orig);

    cv::Mat element = getStructuringElement( cv::MORPH_RECT,
                         cv::Size( 2*6 + 1, 6*2+1 ),
                         cv::Point( 2, 2 ) );


    cv::erode(input,input,element );
    cv::imshow("Map3", input);
    cv::GaussianBlur(input,out, cv::Size(211,211),90);

    cv::bitwise_and(out, orig, out);

    return out;

}





int main(int argc, char *argv[])
{
    const auto path = argv[1];

    auto src = cv::imread(path);
    cv::Mat src_gray,detected_edges;

    cv::cvtColor( src, src_gray, cv::COLOR_BGR2GRAY );
    cv::blur( src_gray, src_gray, cv::Size(3,3) );
    cv::Canny(src_gray, detected_edges, 10, 30,3);



    cv::Mat processed = process(detected_edges);
    cv::imshow("Map2", processed);
    std::cout << "Creating map" << std::endl;
    auto start_time = std::chrono::high_resolution_clock::now();

    GridMap map = GridMap::fromMat(processed, 5);


    cv::Mat gridMap = map.toMat();

    auto dur = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time).count()/1000.0;

    std::cout << "Creation of gridmap took: " << dur << "ms"<< std::endl;

    start_time = std::chrono::high_resolution_clock::now();



    GridAstar astar(map, {0,0}, {map.grid_width-1,map.grid_height-1});
    auto result = astar.start();
    dur = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time).count()/1000.0;

    std::cout << "Astar of gridmap took: " << dur << "ms"<< std::endl;


    if(result)
    {
        std::cout << "Way found" << std::endl;

        GridAstar::Node* pre = result.value().predecessor;
          cv::cvtColor( gridMap, gridMap, cv::COLOR_GRAY2BGR );
        while(pre != nullptr)
        {
            if(pre->predecessor)
                cv::line(gridMap, cv::Point(pre->pos.first, pre->pos.second), cv::Point(pre->predecessor->pos.first, pre->predecessor->pos.second), cv::Scalar(255-pre->val,0,0));
            //std::cout << pre->pos.first <<"," << pre->pos.second<< "," << pre->val << std::endl;
            pre = pre->predecessor;
        }
        cv::resize(gridMap, gridMap, cv::Size(),map.grid_resolution,map.grid_resolution,cv::INTER_AREA);
            cv::imshow("Grid", gridMap);
    }
    else
    {
        std::cout << "No way found" << std::endl;
    }


    cv::waitKey(0);

}
