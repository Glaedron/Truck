#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class Camera
{
  public:

   Camera ();
   Camera (SDL_Renderer* Renderer);
 
   SDL_Texture* GetFrame ();
   void GetImageData (char *imgdata, std::string *pdata);

   void Test ();

  private:

    Mat _Frame;
    Mat _Frame8U;
    SDL_Texture* Texturecam;
    SDL_Surface* Surfacecam;
    VideoCapture _Cap;
    SDL_Renderer* _Renderer;

    IplImage CVcam1;
    IplImage* CVcam;
};

Camera::Camera ()
{
  _Cap = VideoCapture (0);

  if (!_Cap.isOpened ())
  {
    cout << "cannot open camera";
  }
}

Camera::Camera (SDL_Renderer* Renderer)
{
  _Renderer = Renderer;
  _Cap = VideoCapture (0);

  if (!_Cap.isOpened ())
  {
    cout << "cannot open camera";
  }
}

SDL_Texture* Camera::GetFrame ()
{
  //_Cap.read (_Frame);

  _Cap >> _Frame;

  _Frame.convertTo (_Frame8U, CV_8U);

  CVcam1 = (IplImage)_Frame8U;
  CVcam = &CVcam1;

  Surfacecam = SDL_CreateRGBSurfaceFrom ((void*)CVcam->imageData, CVcam->width, CVcam->height, CVcam->depth * CVcam->nChannels, CVcam->widthStep, 0xff0000, 0x00ff00, 0x0000ff, 0);
 
  Texturecam = SDL_CreateTextureFromSurface(_Renderer, Surfacecam);

  SDL_FreeSurface (Surfacecam);

  return Texturecam;
}

void Camera::GetImageData (char *imgdata, std::string *pdata)
{
  _Cap >> _Frame;

  _Frame.convertTo (_Frame8U, CV_8U);
  CVcam1 = (IplImage)_Frame8U;
  CVcam = &CVcam1;

  sprintf (imgdata, " %d %d %d %d ", CVcam->width, CVcam->height, CVcam->depth * CVcam->nChannels, CVcam->widthStep);
  *pdata = CVcam->imageData;
}

void Camera::Test ()
{
  while (1)
  {
    _Cap.read (_Frame);

    _Frame.convertTo (_Frame8U, CV_8UC3);

    imshow ("you can see the image now",_Frame8U);

    if (waitKey(20) == 27)
    {
      break;
    }
  }
}