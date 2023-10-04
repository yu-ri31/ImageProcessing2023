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

  // 画像の表示
  cv::imshow("window", img);
  // キー入力を待つ
  cv::waitKey();
  // 全てのウインドウを破棄
  cv::destroyAllWindows();

  return EXIT_SUCCESS;
}
