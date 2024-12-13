#include<cstdint>
#include<cmath>
#include <cstdio>
#include <cstdlib>
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Input image is required.\n");
    return EXIT_FAILURE;
  }

  // 画像の読み込み
  cv::Mat img = cv::imread(argv[1], cv::IMREAD_ANYCOLOR);
  if (img.empty()) {  // 画像ファイルが見つからない場合の処理
    printf("Input image is not found.\n");
    return EXIT_FAILURE;
  }
auto clip = [](int v){
if(v>225){
  v=255;
}
if(v<0){
  v=0;
}
return v;
};
auto gamma_correction=[](int v,double gamma){
double in = static_cast<double>(v)/ 255.0;
double out =pow(in,gamma)*255.0;
return static_cast<int>(out);
};
auto quantization=[](int v,double delta){
double x = static_cast<double>(v);
double Qx=floor(x/delta+0.5);
return Qx;
};
auto dequantization=[](int v,double delta){
double rec=v*delta;
return static_cast<int>(rec);
};


  const int w=img.cols;
  const int h=img.rows;
  uint8_t * pixels=img.data;
  const double delta =100.0;

for(int y=0;y<h;y++){
  for(int x=0;x<w;++x){
    int idx=y*w+x;
    int val =pixels[idx];

    val= dequantization(quantization(val,delta),delta);

    pixels[idx]=clip(val);

   
  }
}
printf("\n");
  // 画像の表示
  cv::imshow("window", img);
  // キー入力を待つ
  cv::waitKey();
  // 全てのウインドウを破棄
  cv::destroyAllWindows();

  return EXIT_SUCCESS;
}
