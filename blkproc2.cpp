#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <vector>
#include <functional>

constexpr int DCTSIZE=8;

int clip(int v);
void myBGR2YCbCr(cv::Mat &in, cv::Mat &out);
void myYCbCr2BGR(cv::Mat &in, cv::Mat &out);
void quantize(cv::Mat &in, float stepsize);
void blkproc(cv::Mat &in,std::function<void(cv::Mat &)>func);
void line_mozaic(cv::Mat &);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Input image is required.\n");
    return EXIT_FAILURE;
  }

  // 画像の読み込み
  cv::Mat rgb = cv::imread(argv[1], cv::IMREAD_ANYCOLOR);
  if (rgb.empty()) {  // 画像ファイルが見つからない場合の処理
    printf("Input image is not found.\n");
    return EXIT_FAILURE;
  }

  // int W = rgb.cols;
  // int H = rgb.rows;
  // int nc = rgb.channels();

  cv::Mat YCbCr = rgb.clone();
  cv::Mat recimg = rgb .clone() ;

  myBGR2YCbCr(rgb, YCbCr);
  std::vector<cv::Mat> buf(3);
  cv::split(YCbCr,buf);
 
 for(size_t  c = 0; c < buf.size();++c){

  blkproc(buf[c],line_mozaic);
 }
  cv::merge(buf,YCbCr);
  myYCbCr2BGR(YCbCr, recimg);

  // 画像の表示
  cv::imshow("RGB", rgb);
  cv::imshow("YCbCr->RGB", recimg);
  // キー入力を待つ
  int keycode;
  do{keycode =  cv::waitKey();
  }while(keycode != 'q');
  // 全てのウインドウを破棄
  cv::destroyAllWindows();

  return EXIT_SUCCESS;
}

int clip(int v) {
  if (v > 255) {
    v = 255;
  }
  if (v < 0) {
    v = 0;
  }
  return v;
}

void myBGR2YCbCr(cv::Mat &in, cv::Mat &out) {
  int W = in.cols;
  int H = in.rows;
  int nc = in.channels();

  for (int y = 0; y < H; ++y) {
    for (int x = 0; x < nc * W; x += nc) {
      // BGR|BGR|BGR|BGR|...
      int idx = y * nc * W + x;
      int B = in.data[idx];
      int G = in.data[idx + 1];
      int R = in.data[idx + 2];
      int Y = 0.299 * R + 0.587 * G + 0.114 * B;
      int Cb = -0.169 * R - 0.331 * G + 0.5 * B;
      int Cr = 0.5 * R - 0.419 * G - 0.081 * B;
      out.data[idx] = clip(Y);
      out.data[idx + 1] = clip(Cb + 128);
      out.data[idx + 2] = clip(Cr + 128);
    }
  }
}

void myYCbCr2BGR(cv::Mat &in, cv::Mat &out) {
  int W = in.cols;
  int H = in.rows;
  int nc = in.channels();

  for (int y = 0; y < H; ++y) {
    for (int x = 0; x < nc * W; x += nc) {
      // BGR|BGR|BGR|BGR|...
      int idx = y * nc * W + x;
      int Y = in.data[idx];
      int Cb = in.data[idx + 1] - 128;
      int Cr = in.data[idx + 2] - 128;

      int R = Y + 1.402 * Cr;
      int G = Y - 0.344 * Cb - 0.714 * Cr;
      int B = Y + 1.772 * Cb;
      out.data[idx] = clip(B);
      out.data[idx + 1] = clip(G);
      out.data[idx + 2] = clip(R);
    }
  }
}

void quantize(cv::Mat &in, float stepsize) {
  int W = in.cols;
  int H = in.rows;
  int nc = in.channels();

  for (int y = 0; y < H; ++y) {
    for (int x = 0; x < nc * W; x += nc) {
      // BGR|BGR|BGR|BGR|...
      int idx = y * nc * W + x;
      int Y = in.data[idx];
      int Cb = in.data[idx + 1];
      int Cr = in.data[idx + 2];

      // Y = floor((floor(Y / stepsize) + 0.5) * stepsize);
      Cb = floor((floor(Cb / stepsize) + 0.5) * stepsize);
      Cr = floor((floor(Cr / stepsize) + 0.5) * stepsize);

      in.data[idx] = clip(Y);
      in.data[idx + 1] = clip(Cb);
      in.data[idx + 2] = clip(Cr);
    }
  }
}

void blkproc(cv::Mat &in,std::function<void(cv::Mat &)>func){
for (int y = 0; y < in.rows; y += DCTSIZE) {
for (int x = 0; x < in.cols; x += DCTSIZE) {
cv:: Mat blk_in, blk_out;
blk_in = in(cv:: Rect(x, y, DCTSIZE, DCTSIZE)) .clone();
blk_out = in(cv:: Rect(x, y, DCTSIZE, DCTSIZE)) ;
func(blk_in);
blk_in.convertTo(blk_out, blk_out.type());
  }
 }
}
void mozaic(cv::Mat &in){
    if(in.rows!=DCTSIZE||in.cols!=DCTSIZE||in.channels()!=1){
        printf("input for mozaic() shall be 8x8 monochrome.\n");
        exit(EXIT_FAILURE);
    }
    for (int y = 0; y < in.rows; ++y){
    for (int x = 0; x < in.cols; ++x ){
        in.data[y*DCTSIZE+x]=in.data[0];
    }   
    }
}
void line_mozaic(cv::Mat &in){
    if(in.rows!=DCTSIZE||in.cols!=DCTSIZE||in.channels()!=1){
        printf("input for mozaic() shall be 8x8 monochrome.\n");
        exit(EXIT_FAILURE);
    }
    for (int y = 0; y < in.rows; ++y){
    for (int x = 0; x < in.cols; ++x ){
        in.data[y*DCTSIZE+x]=in.data[y*DCTSIZE];
    }   
    }
}