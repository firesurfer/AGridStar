#ifndef MAP_H
#define MAP_H

#include <opencv2/opencv.hpp>
#include <vector>



class Map
{
public:



private:

};


class GridMap: Map
{
public:
    GridMap(size_t x, size_t y, size_t resolution):
        grid_resolution(resolution)
    {
        grid_width = std::ceil((double)x / (double)resolution);
        grid_height = std::ceil((double)y / (double)resolution);

        std::cout << "(" << x << "," << y <<")" << " " << resolution << " (" << grid_width << "," << grid_height<< ")" << std::endl;
        grid_map.reserve(grid_width);
        for(size_t x= 0; x < grid_width;x++)
        {
            std::vector<double> vec;
            vec.resize(grid_height,-1.0);
            grid_map.push_back(vec);
        }
        std::cout << grid_map.size() << "," << grid_map[0].size()  << " memory: " << grid_height * grid_width * sizeof(double)<< std::endl;
    }

    GridMap(GridMap const&) = default;

    double& operator() (int x, int y)
    {
        x -= offset_x;
        y -= offset_y;
        if(x < grid_width && y < grid_height && x >= 0 && y >= 0)
        {
            return grid_map[x][y];
        }
        else
            throw std::runtime_error(std::to_string(x) + "," + std::to_string(y) + " are out of bounds");
    }

    double & operator[](std::pair<int,int> p)
    {
        const auto x =p.first - offset_x;
        const auto y =p.second - offset_y;
        return grid_map[x][y];
    }

    std::optional<double> get(int x,int y) const
    {
        x -= offset_x;
        y -= offset_y;
        if(x < grid_width && y < grid_height && x >= 0 && y >= 0)
        {
            return grid_map[x][y];
        }
        else
            return std::nullopt;
    }

    bool set(int x, int y,const double& value)
    {
        x -= offset_x;
        y -= offset_y;

        if(x < grid_width && y < grid_height && x >= 0 && y >= 0)
        {
            grid_map[x][y] = value;
            return true;
        }
        else
            return false;
    }

    void setOffset(int x, int y)
    {
        offset_x = x;
        offset_y = y;
    }
    cv::Mat toMat()
    {
        cv::Mat mat = cv::Mat::zeros(grid_height, grid_width, CV_8UC1);
        for(size_t y = 0; y < grid_height;y++)
        {
            for(size_t x = 0; x < grid_width;x++)
            {
                mat.at<uint8_t>(cv::Point(x,y)) = (*this)(x,y);
            }
        }
        std::cout << mat.size << std::endl;
        return mat;
    }

    static GridMap fromMat(cv::Mat& mat, size_t resolution)
    {
        size_t height = mat.rows;
        size_t width = mat.cols;

        GridMap map(width,height, resolution);


        for(size_t x = 0; x < width;x++)
        {
            for(size_t y = 0; y < height;y++)
            {
                const int cell_x = x / map.grid_resolution;
                const int cell_y = y / map.grid_resolution;

                if(map(cell_x,cell_y) == -1)
                {
                    map(cell_x,cell_y) = mat.at<uint8_t>(cv::Point(x,y));
                }
                else
                {
                    auto prev_value = map(cell_x,cell_y);
                    const double val = mat.at<uint8_t>(cv::Point(x,y));
                    if(prev_value >val)
                        prev_value = val;

                     map(cell_x,cell_y)  = prev_value;
                }
            }
        }
        //Print
        /*for(size_t y = 0; y < map.grid_height;y++)
        {
            for(size_t x = 0; x < map.grid_width;x++)
            {

                std::cout << std::setprecision(4) << map(x,y) << "\t";
            }
            std::cout << std::endl;
        }*/
        return map;
    }

    size_t grid_width;
    size_t grid_height;
    size_t grid_resolution;

    int offset_x = 0;
    int offset_y = 0;
private:
    std::vector<std::vector<double>> grid_map;


};

#endif // MAP_H
