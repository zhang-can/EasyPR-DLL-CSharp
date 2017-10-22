#include "easypr/core/core_func.h"
#include "easypr/core/plate.hpp"
#include "easypr/core/chars_identify.h"
#include "easypr/config.h"
#include "easypr/core/params.h"
#include "thirdparty/mser/mser2.hpp"

using namespace cv;

namespace easypr {


Mat colorMatch(const Mat &src, Mat &match, const Color r,
               const bool adaptive_minsv) {

  // if use adaptive_minsv
  // min value of s and v is adaptive to h

  const float max_sv = 255;
  const float minref_sv = 64;

  const float minabs_sv = 95;

  // H range of blue 

  const int min_blue = 100;  // 100
  const int max_blue = 140;  // 140

  // H range of yellow

  const int min_yellow = 15;  // 15
  const int max_yellow = 40;  // 40

  // H range of white

  const int min_white = 0;   // 0
  const int max_white = 30;  // 30

  // H range of green

  const int min_green = 40;   // 40
  const int max_green = 80;  // 80

  Mat src_hsv;

  // convert to HSV space
  cvtColor(src, src_hsv, CV_BGR2HSV);

  std::vector<cv::Mat> hsvSplit;
  split(src_hsv, hsvSplit);
  equalizeHist(hsvSplit[2], hsvSplit[2]);
  merge(hsvSplit, src_hsv);

  // match to find the color

  int min_h = 0;
  int max_h = 0;
  switch (r) {
    case BLUE:
      min_h = min_blue;
      max_h = max_blue;
      break;
    case YELLOW:
      min_h = min_yellow;
      max_h = max_yellow;
      break;
    case WHITE:
      min_h = min_white;
      max_h = max_white;
      break;
	case GREEN:
		min_h = min_green;
		max_h = max_green;
		break;
    default:
      // Color::UNKNOWN
      break;
  }

  float diff_h = float((max_h - min_h) / 2);
  float avg_h = min_h + diff_h;

  int channels = src_hsv.channels();
  int nRows = src_hsv.rows;

  // consider multi channel image
  int nCols = src_hsv.cols * channels;
  if (src_hsv.isContinuous()) {
    nCols *= nRows;
    nRows = 1;
  }

  int i, j;
  uchar* p;
  float s_all = 0;
  float v_all = 0;
  float count = 0;
  for (i = 0; i < nRows; ++i) {
    p = src_hsv.ptr<uchar>(i);
    for (j = 0; j < nCols; j += 3) {
      int H = int(p[j]);      // 0-180
      int S = int(p[j + 1]);  // 0-255
      int V = int(p[j + 2]);  // 0-255

      s_all += S;
      v_all += V;
      count++;

      bool colorMatched = false;

      if (H > min_h && H < max_h) {
        float Hdiff = 0;
        if (H > avg_h)
          Hdiff = H - avg_h;
        else
          Hdiff = avg_h - H;

        float Hdiff_p = float(Hdiff) / diff_h;

        float min_sv = 0;
        if (true == adaptive_minsv)
          min_sv =
              minref_sv -
                  minref_sv / 2 *
                      (1
                          - Hdiff_p);  // inref_sv - minref_sv / 2 * (1 - Hdiff_p)
        else
          min_sv = minabs_sv;  // add

        if ((S > min_sv && S < max_sv) && (V > min_sv && V < max_sv))
          colorMatched = true;
      }

      if (colorMatched == true) {
        p[j] = 0;
        p[j + 1] = 0;
        p[j + 2] = 255;
      } else {
        p[j] = 0;
        p[j + 1] = 0;
        p[j + 2] = 0;
      }
    }
  }

  // cout << "avg_s:" << s_all / count << endl;
  // cout << "avg_v:" << v_all / count << endl;

  // get the final binary

  Mat src_grey;
  std::vector<cv::Mat> hsvSplit_done;
  split(src_hsv, hsvSplit_done);
  src_grey = hsvSplit_done[2];

  match = src_grey;

  return src_grey;
}

bool bFindLeftRightBound1(Mat &bound_threshold, int &posLeft, int &posRight) {

  float span = bound_threshold.rows * 0.2f;

  for (int i = 0; i < bound_threshold.cols - span - 1; i += 3) {
    int whiteCount = 0;
    for (int k = 0; k < bound_threshold.rows; k++) {
      for (int l = i; l < i + span; l++) {
        if (bound_threshold.data[k * bound_threshold.step[0] + l] == 255) {
          whiteCount++;
        }
      }
    }
    if (whiteCount * 1.0 / (span * bound_threshold.rows) > 0.15) {
      posLeft = i;
      break;
    }
  }
  span = bound_threshold.rows * 0.2f;


  for (int i = bound_threshold.cols - 1; i > span; i -= 2) {
    int whiteCount = 0;
    for (int k = 0; k < bound_threshold.rows; k++) {
      for (int l = i; l > i - span; l--) {
        if (bound_threshold.data[k * bound_threshold.step[0] + l] == 255) {
          whiteCount++;
        }
      }
    }

    if (whiteCount * 1.0 / (span * bound_threshold.rows) > 0.06) {
      posRight = i;
      if (posRight + 5 < bound_threshold.cols) {
        posRight = posRight + 5;
      } else {
        posRight = bound_threshold.cols - 1;
      }

      break;
    }
  }

  if (posLeft < posRight) {
    return true;
  }
  return false;
}

bool bFindLeftRightBound(Mat &bound_threshold, int &posLeft, int &posRight) {


  float span = bound_threshold.rows * 0.2f;

  for (int i = 0; i < bound_threshold.cols - span - 1; i += 2) {
    int whiteCount = 0;
    for (int k = 0; k < bound_threshold.rows; k++) {
      for (int l = i; l < i + span; l++) {
        if (bound_threshold.data[k * bound_threshold.step[0] + l] == 255) {
          whiteCount++;
        }
      }
    }
    if (whiteCount * 1.0 / (span * bound_threshold.rows) > 0.36) {
      posLeft = i;
      break;
    }
  }
  span = bound_threshold.rows * 0.2f;


  for (int i = bound_threshold.cols - 1; i > span; i -= 2) {
    int whiteCount = 0;
    for (int k = 0; k < bound_threshold.rows; k++) {
      for (int l = i; l > i - span; l--) {
        if (bound_threshold.data[k * bound_threshold.step[0] + l] == 255) {
          whiteCount++;
        }
      }
    }

    if (whiteCount * 1.0 / (span * bound_threshold.rows) > 0.26) {
      posRight = i;
      break;
    }
  }

  if (posLeft < posRight) {
    return true;
  }
  return false;
}

bool bFindLeftRightBound2(Mat &bound_threshold, int &posLeft, int &posRight) {

  float span = bound_threshold.rows * 0.2f;

  for (int i = 0; i < bound_threshold.cols - span - 1; i += 3) {
    int whiteCount = 0;
    for (int k = 0; k < bound_threshold.rows; k++) {
      for (int l = i; l < i + span; l++) {
        if (bound_threshold.data[k * bound_threshold.step[0] + l] == 255) {
          whiteCount++;
        }
      }
    }
    if (whiteCount * 1.0 / (span * bound_threshold.rows) > 0.32) {
      posLeft = i;
      break;
    }
  }
  span = bound_threshold.rows * 0.2f;


  for (int i = bound_threshold.cols - 1; i > span; i -= 3) {
    int whiteCount = 0;
    for (int k = 0; k < bound_threshold.rows; k++) {
      for (int l = i; l > i - span; l--) {
        if (bound_threshold.data[k * bound_threshold.step[0] + l] == 255) {
          whiteCount++;
        }
      }
    }

    if (whiteCount * 1.0 / (span * bound_threshold.rows) > 0.22) {
      posRight = i;
      break;
    }
  }

  if (posLeft < posRight) {
    return true;
  }
  return false;
}


bool plateColorJudge(const Mat &src, const Color r, const bool adaptive_minsv,
                     float &percent) {

  const float thresh = 0.45f;

  Mat src_gray;
  colorMatch(src, src_gray, r, adaptive_minsv);

  percent =
      float(countNonZero(src_gray)) / float(src_gray.rows * src_gray.cols);
  // cout << "percent:" << percent << endl;

  if (percent > thresh)
    return true;
  else
    return false;
}

Color getPlateType(const Mat &src, const bool adaptive_minsv) {
  float max_percent = 0;
  Color max_color = UNKNOWN;

  float blue_percent = 0;
  float yellow_percent = 0;
  float white_percent = 0;

  if (plateColorJudge(src, BLUE, adaptive_minsv, blue_percent) == true) {
    // cout << "BLUE" << endl;
    return BLUE;
  } else if (plateColorJudge(src, YELLOW, adaptive_minsv, yellow_percent) ==
      true) {
    // cout << "YELLOW" << endl;
    return YELLOW;
  } else if (plateColorJudge(src, WHITE, adaptive_minsv, white_percent) ==
      true) {
    // cout << "WHITE" << endl;
    return WHITE;
  } else {
    //std::cout << "OTHER" << std::endl;

    /*max_percent = blue_percent > yellow_percent ? blue_percent : yellow_percent;
    max_color = blue_percent > yellow_percent ? BLUE : YELLOW;
    max_color = max_percent > white_percent ? max_color : WHITE;*/

    // always return blue
    return BLUE;
  }
}

void clearLiuDingOnly(Mat &img) {
  const int x = 7;
  Mat jump = Mat::zeros(1, img.rows, CV_32F);
  for (int i = 0; i < img.rows; i++) {
    int jumpCount = 0;
    int whiteCount = 0;
    for (int j = 0; j < img.cols - 1; j++) {
      if (img.at<char>(i, j) != img.at<char>(i, j + 1)) jumpCount++;

      if (img.at<uchar>(i, j) == 255) {
        whiteCount++;
      }
    }

    jump.at<float>(i) = (float) jumpCount;
  }

  for (int i = 0; i < img.rows; i++) {
    if (jump.at<float>(i) <= x) {
      for (int j = 0; j < img.cols; j++) {
        img.at<char>(i, j) = 0;
      }
    }
  }
}

bool clearLiuDing(Mat &img) {
  std::vector<float> fJump;
  int whiteCount = 0;
  const int x = 7;
  Mat jump = Mat::zeros(1, img.rows, CV_32F);
  for (int i = 0; i < img.rows; i++) {
    int jumpCount = 0;

    for (int j = 0; j < img.cols - 1; j++) {
      if (img.at<char>(i, j) != img.at<char>(i, j + 1)) jumpCount++;

      if (img.at<uchar>(i, j) == 255) {
        whiteCount++;
      }
    }

    jump.at<float>(i) = (float) jumpCount;
  }

  int iCount = 0;
  for (int i = 0; i < img.rows; i++) {
    fJump.push_back(jump.at<float>(i));
    if (jump.at<float>(i) >= 16 && jump.at<float>(i) <= 45) {

      // jump condition
      iCount++;
    }
  }

  // if not is not plate
  if (iCount * 1.0 / img.rows <= 0.40) {
    return false;
  }

  if (whiteCount * 1.0 / (img.rows * img.cols) < 0.15 ||
      whiteCount * 1.0 / (img.rows * img.cols) > 0.50) {
    return false;
  }

  for (int i = 0; i < img.rows; i++) {
    if (jump.at<float>(i) <= x) {
      for (int j = 0; j < img.cols; j++) {
        img.at<char>(i, j) = 0;
      }
    }
  }
  return true;
}

void clearLiuDing(Mat mask, int &top, int &bottom) {
  const int x = 7;

  for (int i = 0; i < mask.rows / 2; i++) {
    int whiteCount = 0;
    int jumpCount = 0;
    for (int j = 0; j < mask.cols - 1; j++) {
      if (mask.at<char>(i, j) != mask.at<char>(i, j + 1)) jumpCount++;

      if ((int) mask.at<uchar>(i, j) == 255) {
        whiteCount++;
      }
    }
    if ((jumpCount < x && whiteCount * 1.0 / mask.cols > 0.15) ||
        whiteCount < 4) {
      top = i;
    }
  }
  top -= 1;
  if (top < 0) {
    top = 0;
  }

  // ok, find top and bottom boudnadry

  for (int i = mask.rows - 1; i >= mask.rows / 2; i--) {
    int jumpCount = 0;
    int whiteCount = 0;
    for (int j = 0; j < mask.cols - 1; j++) {
      if (mask.at<char>(i, j) != mask.at<char>(i, j + 1)) jumpCount++;
      if (mask.at<uchar>(i, j) == 255) {
        whiteCount++;
      }
    }
    if ((jumpCount < x && whiteCount * 1.0 / mask.cols > 0.15) ||
        whiteCount < 4) {
      bottom = i;
    }
  }
  bottom += 1;
  if (bottom >= mask.rows) {
    bottom = mask.rows - 1;
  }

  if (top >= bottom) {
    top = 0;
    bottom = mask.rows - 1;
  }
}

int ThresholdOtsu(Mat mat) {
  int height = mat.rows;
  int width = mat.cols;

  // histogram
  float histogram[256] = {0};
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      unsigned char p = (unsigned char) ((mat.data[i * mat.step[0] + j]));
      histogram[p]++;
    }
  }
  // normalize histogram
  int size = height * width;
  for (int i = 0; i < 256; i++) {
    histogram[i] = histogram[i] / size;
  }

  // average pixel value
  float avgValue = 0;
  for (int i = 0; i < 256; i++) {
    avgValue += i * histogram[i];
  }

  int thresholdV;
  float maxVariance = 0;
  float w = 0, u = 0;
  for (int i = 0; i < 256; i++) {
    w += histogram[i];
    u += i * histogram[i];

    float t = avgValue * w - u;
    float variance = t * t / (w * (1 - w));
    if (variance > maxVariance) {
      maxVariance = variance;
      thresholdV = i;
    }
  }

  return thresholdV;
}


Mat histeq(Mat in) {
  Mat out(in.size(), in.type());
  if (in.channels() == 3) {
    Mat hsv;
    std::vector<cv::Mat> hsvSplit;
    cvtColor(in, hsv, CV_BGR2HSV);
    split(hsv, hsvSplit);
    equalizeHist(hsvSplit[2], hsvSplit[2]);
    merge(hsvSplit, hsv);
    cvtColor(hsv, out, CV_HSV2BGR);
  } else if (in.channels() == 1) {
    equalizeHist(in, out);
  }
  return out;
}

#define HORIZONTAL 1
#define VERTICAL 0

Mat CutTheRect(Mat &in, Rect &rect) {
  int size = in.cols;  // (rect.width>rect.height)?rect.width:rect.height;
  Mat dstMat(size, size, CV_8UC1);
  dstMat.setTo(Scalar(0, 0, 0));

  int x = (int) floor((float) (size - rect.width) / 2.0f);
  int y = (int) floor((float) (size - rect.height) / 2.0f);

  for (int i = 0; i < rect.height; ++i) {

    for (int j = 0; j < rect.width; ++j) {
      dstMat.data[dstMat.step[0] * (i + y) + j + x] =
          in.data[in.step[0] * (i + rect.y) + j + rect.x];
    }
  }

  //
  return dstMat;
}

Rect GetCenterRect(Mat &in) {
  Rect _rect;

  int top = 0;
  int bottom = in.rows - 1;

  // find the center rect

  for (int i = 0; i < in.rows; ++i) {
    bool bFind = false;
    for (int j = 0; j < in.cols; ++j) {
      if (in.data[i * in.step[0] + j] > 20) {
        top = i;
        bFind = true;
        break;
      }
    }
    if (bFind) {
      break;
    }

  }
  for (int i = in.rows - 1;
  i >= 0;
  --i) {
    bool bFind = false;
    for (int j = 0; j < in.cols; ++j) {
      if (in.data[i * in.step[0] + j] > 20) {
        bottom = i;
        bFind = true;
        break;
      }
    }
    if (bFind) {
      break;
    }

  }


  int left = 0;
  int right = in.cols - 1;
  for (int j = 0; j < in.cols; ++j) {
    bool bFind = false;
    for (int i = 0; i < in.rows; ++i) {
      if (in.data[i * in.step[0] + j] > 20) {
        left = j;
        bFind = true;
        break;
      }
    }
    if (bFind) {
      break;
    }

  }
  for (int j = in.cols - 1;
  j >= 0;
  --j) {
    bool bFind = false;
    for (int i = 0; i < in.rows; ++i) {
      if (in.data[i * in.step[0] + j] > 20) {
        right = j;
        bFind = true;

        break;
      }
    }
    if (bFind) {
      break;
    }
  }

  _rect.x = left;
  _rect.y = top;
  _rect.width = right - left + 1;
  _rect.height = bottom - top + 1;

  return _rect;
}

// 计算大值
float countOfBigValue(Mat &mat, int iValue) {
  float iCount = 0.0;
  if (mat.rows > 1) {
    for (int i = 0; i < mat.rows; ++i) {
      if (mat.data[i * mat.step[0]] > iValue) {
        iCount += 1.0;
      }
    }
    return iCount;

  } else {
    for (int i = 0; i < mat.cols; ++i) {
      if (mat.data[i] > iValue) {
        iCount += 1.0;
      }
    }

    return iCount;
  }
}

// 直方图计算
Mat ProjectedHistogram(Mat img, int t) {
  int sz = (t) ? img.rows : img.cols;
  Mat mhist = Mat::zeros(1, sz, CV_32F);

  for (int j = 0; j < sz; j++) {
    Mat data = (t) ? img.row(j) : img.col(j);

    mhist.at<float>(j) = countOfBigValue(data, 20);
  }

  // 标准化直方图
  double min, max;
  minMaxLoc(mhist, &min, &max);

  if (max > 0)
    mhist.convertTo(mhist, -1, 1.0f / max, 0);

  return mhist;
}

Mat preprocessChar(Mat in, int char_size) {
  // Remap image
  int h = in.rows;
  int w = in.cols;

  int charSize = char_size;

  Mat transformMat = Mat::eye(2, 3, CV_32F);
  int m = max(w, h);
  transformMat.at<float>(0, 2) = float(m / 2 - w / 2);
  transformMat.at<float>(1, 2) = float(m / 2 - h / 2);

  Mat warpImage(m, m, in.type());
  warpAffine(in, warpImage, transformMat, warpImage.size(), INTER_LINEAR,
    BORDER_CONSTANT, Scalar(0));

  Mat out;
  cv::resize(warpImage, out, Size(charSize, charSize));

  return out;
}

Rect GetChineseRect(const Rect rectSpe) {
  int height = rectSpe.height;
  float newwidth = rectSpe.width * 1.10f;
  int x = rectSpe.x;
  int y = rectSpe.y;

  int newx = x - int(newwidth * 1.10f);
  newx = newx > 0 ? newx : 0;

  Rect a(newx, y, int(newwidth), height);

  return a;
}

bool verifyCharSizes(Rect r) {
  // Char sizes 45x90
  float aspect = 45.0f / 90.0f;
  float charAspect = (float)r.width / (float)r.height;
  float error = 0.35f;
  float minHeight = 25.f;
  float maxHeight = 50.f;
  // We have a different aspect ratio for number 1, and it can be ~0.2
  float minAspect = 0.05f;
  //float minAspect = aspect - aspect * error;//zc
  float maxAspect = aspect + aspect * error;

  // bb area
  int bbArea = r.width * r.height;

  if (charAspect > minAspect && charAspect < maxAspect /*&&
                                                       r.rows >= minHeight && r.rows < maxHeight*/)
                                                       return true;
  else
    return false;
}

//按比例缩小图片
Mat scaleImage(const Mat& image, const Size& maxSize, double& scale_ratio) {
  Mat ret;

  if (image.cols > maxSize.width || image.rows > maxSize.height) {
    double widthRatio = image.cols / (double)maxSize.width;
    double heightRatio = image.rows / (double)maxSize.height;
    double m_real_to_scaled_ratio = max(widthRatio, heightRatio);

    int newWidth = int(image.cols / m_real_to_scaled_ratio);
    int newHeight = int(image.rows / m_real_to_scaled_ratio);

    cv::resize(image, ret, Size(newWidth, newHeight), 0, 0);
    scale_ratio = m_real_to_scaled_ratio;
  }
  else {
    ret = image;
    scale_ratio = 1.0;
  }

  return ret;
}


// Scale back RotatedRect
RotatedRect scaleBackRRect(const RotatedRect& rr, const float scale_ratio) {
  float width = rr.size.width * scale_ratio;
  float height = rr.size.height * scale_ratio;
  float x = rr.center.x * scale_ratio;
  float y = rr.center.y * scale_ratio;
  RotatedRect mserRect(Point2f(x, y), Size2f(width, height), rr.angle);
  
  return mserRect;
}

bool verifyPlateSize(Rect mr) {
  float error = 0.6f;
  // Spain car plate size: 52x11 aspect 4,7272
  // China car plate size: 440mm*140mm，aspect 3.142857

  // Real car plate size: 136 * 32, aspect 4
  float aspect = 3.75;

  // Set a min and max area. All other patchs are discarded
  // int min= 1*aspect*1; // minimum area
  // int max= 2000*aspect*2000; // maximum area
  int min = 34 * 8 * 1;  // minimum area
  int max = 34 * 8 * 200;  // maximum area

  // Get only patchs that match to a respect ratio.
  float rmin = aspect - aspect * error;
  float rmax = aspect + aspect * error;

  float area = float(mr.height * mr.width);
  float r = (float)mr.width / (float)mr.height;
  if (r < 1) r = (float)mr.height / (float)mr.width;

  // cout << "area:" << area << endl;
  // cout << "r:" << r << endl;

  if ((area < min || area > max) || (r < rmin || r > rmax))
    return false;
  else
    return true;
}

// 根据面积、宽高比、角度、宽度指标判断是否为车牌
bool verifyRotatedPlateSizes(RotatedRect mr, bool showDebug) {
  float error = 0.65f;
  // 中国车牌尺寸: 440mm*140mm，宽高比：3.142857
  // 图片车牌大小: 136 * 36, 宽高比：3.8
  float aspect = 3.75f;

  // 仅仅保留符合预期宽高比的区域
  float aspect_min = aspect - aspect * error;
  float aspect_max = aspect + aspect * error;

  float width_max = 600.f;
  float width_min = 30.f;

  float min = float(width_min * width_min / aspect_max);  // minimum area
  float max = float(width_max * width_max / aspect_min);  // maximum area

  float width = mr.size.width;
  float height = mr.size.height;
  float area = width * height;

  float ratio = width / height;
  float angle = mr.angle;
  if (ratio < 1) {
    swap(width, height);
    ratio = width / height;

    angle = 90.f + angle;
    //std::cout << "angle:" << angle << std::endl;
  }

  float angle_min = -60.f;
  float angle_max = 60.f;

  //由面积、宽高比、角度、宽度指标来判断候选区域是否为车牌
  if (area < min || area > max) {
    if (0 && showDebug) {
      std::cout << "area < min || area > max: " << area << std::endl;
    }

    return false;
  }
  else if (ratio < aspect_min || ratio > aspect_max) {
    if (0 && showDebug) {
      std::cout << "ratio < aspect_min || ratio > aspect_max: " << ratio << std::endl;
    }
    
    return false;
  }
  else if (angle < angle_min || angle > angle_max) {
    if (0 && showDebug) {
      std::cout << "angle < angle_min || angle > angle_max: " << angle << std::endl;
    }
    
    return false;
  }
  else if (width < width_min || width > width_max) {
    if (0 && showDebug) {
      std::cout << "width < width_min || width > width_max: " << width << std::endl;
    }
    
    return false;  
  }
  else {
    return true;
  }

  return true;
}

//非极大值抑制方法，overlap=0.6
void NMStoCharacter(std::vector<CCharacter> &inVec, double overlap) {
  //按m_score排序
  std::sort(inVec.begin(), inVec.end());

  std::vector<CCharacter>::iterator it = inVec.begin();
  for (; it != inVec.end(); ++it) {
    CCharacter charSrc = *it;
    //std::cout << "plateScore:" << plateSrc.getPlateScore() << std::endl;
    Rect rectSrc = charSrc.getCharacterPos();

    std::vector<CCharacter>::iterator itc = it + 1;

    for (; itc != inVec.end();) {
      CCharacter charComp = *itc;
      Rect rectComp = charComp.getCharacterPos();
      //Rect rectInter = rectSrc & rectComp;
      //Rect rectUnion = rectSrc | rectComp;
      //double r = double(rectInter.area()) / double(rectUnion.area());

	  //计算重叠面积比
      float iou = computeIOU(rectSrc, rectComp);

      if (iou > overlap) {
        itc = inVec.erase(itc);
      }
      else {
        ++itc;
      }
    }
  }
}

// 判断两个CCharacter是否大致相同
bool compareCharRect(const CCharacter& character1, const CCharacter& character2)
{
  Rect rect1 = character1.getCharacterPos();
  Rect rect2 = character2.getCharacterPos();

  // 判断height是否相似，若height相似，继续
  float width_1 = float(rect1.width);
  float height_1 = float(rect1.height);

  float width_2 = float(rect2.width);
  float height_2 = float(rect2.height);

  float height_diff = abs(height_1 - height_2);
  double height_diff_ratio = height_diff / min(height_1, height_2);

  if (height_diff_ratio > 0.25)
    return false;

  // 判断y坐标是否相似，若相似，继续
  float y_1 = float(rect1.tl().y);
  float y_2 = float(rect2.tl().y);

  float y_diff = abs(y_1 - y_2);
  double y_diff_ratio = y_diff / min(height_1, height_2);

  if (y_diff_ratio > 0.5)
    return false;

  // 判断中心是否接近，若接近，继续
  float x_1 = float(rect1.tl().x + rect1.width / 2);
  float x_2 = float(rect2.tl().x + rect2.width / 2);

  float x_diff = abs(x_1 - x_2);
  double x_diff_ratio = x_diff / min(height_1, height_2);

  if (x_diff_ratio < 0.25)
    return false;

  // 判断x坐标是否接近，但不特别近，继续
  float x_margin_left = float(min(rect1.br().x, rect2.br().x));
  float x_margin_right = float(max(rect1.tl().x, rect2.tl().x));

  float x_margin_diff = abs(x_margin_left - x_margin_right);
  double x_margin_diff_ratio = x_margin_diff / min(height_1, height_2);

  if (x_margin_diff_ratio > 1.0)
    return false;

  return true;
}

//合并字符为组,根据相似性
void mergeCharToGroup(std::vector<CCharacter> vecRect,
  std::vector<std::vector<CCharacter>>& charGroupVec) {

  std::vector<int> labels;

  int numbers = 0;
  //应用cv::partition聚类函数，对相似字符Rect进行聚类
  if (vecRect.size() > 0)
    numbers = partition(vecRect, labels, &compareCharRect);

  for (size_t j = 0; j < size_t(numbers); j++) {
    std::vector<CCharacter> charGroup;

    for (size_t t = 0; t < vecRect.size(); t++) {
      int label = labels[t];

      if (label == j)
        charGroup.push_back(vecRect[t]);
    }

    if (charGroup.size() < 2)
      continue;

    charGroupVec.push_back(charGroup);
  }
}

// 在途中绘制RotatedRect区域，标为红色
void rotatedRectangle(InputOutputArray image, RotatedRect rrect, const Scalar& color, int thickness, int lineType, int shift) {
  Point2f rect_points[4];
  // 返回4个顶点
  rrect.points(rect_points);
  for (int j = 0; j < 4; j++) {
    cv::line(image, rect_points[j], rect_points[(j + 1) % 4], color, thickness, lineType, shift);
  }
}

// 寻找弱种子
void searchWeakSeed(const std::vector<CCharacter>& charVec, std::vector<CCharacter>& mserCharacter, double thresh1, double thresh2,
  const Vec4f& line, Point& boundaryPoint, const Rect& maxrect, Rect& plateResult, Mat result, CharSearchDirection searchDirection) {

  float k = line[1] / line[0];
  float x_1 = line[2];
  float y_1 = line[3];

  std::vector<CCharacter> searchWeakSeedVec;
  searchWeakSeedVec.reserve(8);

  for (auto weakSeed : charVec) {
    Rect weakRect = weakSeed.getCharacterPos();

    //cv::rectangle(result, weakRect, Scalar(255, 0, 255));

    Point weakCenter(weakRect.tl().x + weakRect.width / 2, weakRect.tl().y + weakRect.height / 2);
    float x_2 = (float)weakCenter.x;

	// 边界限定
    if (searchDirection == CharSearchDirection::LEFT) {
      if (weakCenter.x + weakRect.width / 2 > boundaryPoint.x) {
        continue;
      }
    }
    else if (searchDirection == CharSearchDirection::RIGHT) {
      if (weakCenter.x - weakRect.width / 2 < boundaryPoint.x) {
        continue;
      }
    }

    float y_2l = k * (x_2 - x_1) + y_1;
    float y_2 = (float)weakCenter.y;

    float y_diff_ratio = abs(y_2l - y_2) / maxrect.height;

    if (y_diff_ratio < thresh1) {
      float width_1 = float(maxrect.width);
      float height_1 = float(maxrect.height);

      float width_2 = float(weakRect.width);
      float height_2 = float(weakRect.height);

      float height_diff = abs(height_1 - height_2);
      double height_diff_ratio = height_diff / min(height_1, height_2);

      float width_diff = abs(width_1 - width_2);
      double width_diff_ratio = width_diff / maxrect.width;

      if (height_diff_ratio < thresh1 && width_diff_ratio < 0.5) {
        //std::cout << "h" << height_diff_ratio << std::endl;
        //std::cout << "w" << width_diff_ratio << std::endl;
        searchWeakSeedVec.push_back(weakSeed);
      }
      else {

      }
    }
  }

  // 判断第一个弱种子字符是否符合要求
  if (searchWeakSeedVec.size() != 0) {
    if (searchDirection == CharSearchDirection::LEFT) {
      std::sort(searchWeakSeedVec.begin(), searchWeakSeedVec.end(),
        [](const CCharacter& r1, const CCharacter& r2) {
        return r1.getCharacterPos().tl().x > r2.getCharacterPos().tl().x;
      });
    }
    else if (searchDirection == CharSearchDirection::RIGHT) {
      std::sort(searchWeakSeedVec.begin(), searchWeakSeedVec.end(),
        [](const CCharacter& r1, const CCharacter& r2) {
        return r1.getCharacterPos().tl().x < r2.getCharacterPos().tl().x;
      });
    }

    CCharacter firstWeakSeed = searchWeakSeedVec.at(0);
    Rect firstWeakRect = firstWeakSeed.getCharacterPos();
    Point firstWeakCenter(firstWeakRect.tl().x + firstWeakRect.width / 2,
      firstWeakRect.tl().y + firstWeakRect.height / 2);

    float ratio = (float)abs(firstWeakCenter.x - boundaryPoint.x) / (float)maxrect.height;
    if (ratio > thresh2) {
      if (0) {
        std::cout << "search seed ratio:" << ratio << std::endl;
      }
      return;
    }
      
    mserCharacter.push_back(firstWeakSeed);
    plateResult |= firstWeakRect;
    boundaryPoint = firstWeakCenter;

    for (size_t weakSeedIndex = 0; weakSeedIndex + 1 < searchWeakSeedVec.size(); weakSeedIndex++) {
      CCharacter weakSeed = searchWeakSeedVec[weakSeedIndex];
      CCharacter weakSeedCompare = searchWeakSeedVec[weakSeedIndex + 1];

      Rect rect1 = weakSeed.getCharacterPos();
      Rect rect2 = weakSeedCompare.getCharacterPos();

      Rect weakRect = rect2;
      Point weakCenter(weakRect.tl().x + weakRect.width / 2, weakRect.tl().y + weakRect.height / 2);

      // 车牌中字符高度相近
      float width_1 = float(rect1.width);
      float height_1 = float(rect1.height);

      float width_2 = float(rect2.width);
      float height_2 = float(rect2.height);

      // 车牌中字符x坐标接近但不很近
      float x_margin_left = float(min(rect1.br().x, rect2.br().x));
      float x_margin_right = float(max(rect1.tl().x, rect2.tl().x));

      float x_margin_diff = abs(x_margin_left - x_margin_right);
      double x_margin_diff_ratio = x_margin_diff / min(height_1, height_2);

      if (x_margin_diff_ratio > thresh2) {
        if (0) {
          std::cout << "search seed x_margin_diff_ratio:" << x_margin_diff_ratio << std::endl;
        }       
        break;
      }
      else {
        //::rectangle(result, weakRect, Scalar(255, 0, 0), 1);
        mserCharacter.push_back(weakSeedCompare);
        plateResult |= weakRect;
        if (searchDirection == CharSearchDirection::LEFT) {
          if (weakCenter.x < boundaryPoint.x) {
            boundaryPoint = weakCenter;
          }
        }
        else if (searchDirection == CharSearchDirection::RIGHT) {
          if (weakCenter.x > boundaryPoint.x) {
            boundaryPoint = weakCenter;
          }
        }      
      }
    }
  }
}

// 滑动窗口搜索法
// slideWindowSearch(image, slideLeftWindow, line, 
// leftPoint, dist, ostu_level, ratioWindow = 0.4, threshIsCharacter = 0.8,maxrect, plateResult, 
// CharSearchDirection::LEFT, true, result);
void slideWindowSearch(const Mat &image, std::vector<CCharacter>& slideCharacter, const Vec4f& line, 
  Point& fromPoint, const Vec2i& dist, double ostu_level, float ratioWindow, float threshIsCharacter, const Rect& maxrect, Rect& plateResult,
  CharSearchDirection searchDirection, bool isChinese, Mat& result) {
  float k = line[1] / line[0];
  float x_1 = line[2];
  float y_1 = line[3];

  int slideLength = int(ratioWindow * maxrect.width);
  int slideStep = 1;
  int fromX = 0;
  //滑动方向判断
  if (searchDirection == CharSearchDirection::LEFT) {
    fromX = fromPoint.x - dist[0];
  }
  else if (searchDirection == CharSearchDirection::RIGHT) {
    fromX = fromPoint.x + dist[0];
  }
  
  std::vector<CCharacter> charCandidateVec;
  for (int slideX = -slideLength; slideX < slideLength; slideX += slideStep) {
    float x_slide = 0; 

    if (searchDirection == CharSearchDirection::LEFT) {
      x_slide = float(fromX - slideX);
    }
    else if (searchDirection == CharSearchDirection::RIGHT) {
      x_slide = float(fromX + slideX);
    }

    float y_slide = k * (x_slide - x_1) + y_1;
    Point2f p_slide(x_slide, y_slide);
	// 紫色显示滑动中心点范围
    cv::circle(result, p_slide, 1, Scalar(255, 0, 255), 3);

    int chineseWidth = int(maxrect.width * 1.05);
    int chineseHeight = int(maxrect.height * 1.05);

    Rect rect(Point2f(x_slide - chineseWidth / 2, y_slide - chineseHeight / 2), Size(chineseWidth, chineseHeight));

    if (rect.tl().x < 0 || rect.tl().y < 0 || rect.br().x >= image.cols || rect.br().y >= image.rows)
      continue;

    Mat region = image(rect);
    Mat binary_region;

    cv::threshold(region, binary_region, ostu_level, 255, CV_THRESH_BINARY);
    //double ostu_level = threshold(region, binary_region, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    //std::cout << "ostu_level:" << ostu_level << std::endl;*/

    Mat charInput = preprocessChar(binary_region, 20);

    if (0) {
      imshow("charInput", charInput);
      waitKey(0);
      destroyWindow("charInput");
    }

    CCharacter charCandidate;
    charCandidate.setCharacterPos(rect);
    charCandidate.setCharacterMat(charInput);
    charCandidate.setIsChinese(isChinese);
    charCandidateVec.push_back(charCandidate);
  }

  if (isChinese) {
    CharsIdentify::instance()->classifyChinese(charCandidateVec);
  }
  else {
    CharsIdentify::instance()->classify(charCandidateVec);
  }

  double overlapThresh = 0.1;
  // NMS去掉重合部分较大的charCandidateVec，由于滑动范围较小，overlapThresh很低
  // 最终可以得到使得charCandidateVec.size() = 1
  // 即滑动得到一个字符
  NMStoCharacter(charCandidateVec, overlapThresh);

  for (auto character : charCandidateVec) {
    Rect rect = character.getCharacterPos();
    Point center(rect.tl().x + rect.width / 2, rect.tl().y + rect.height / 2);

    if (character.getCharacterScore() > threshIsCharacter && character.getCharacterStr() != "1") {
      //cv::rectangle(result, rect, Scalar(255, 255, 255), 1);
      plateResult |= rect;
      slideCharacter.push_back(character);
      fromPoint = center;

      if (0) {
        std::cout << "label:" << character.getCharacterStr();
        std::cout << "__score:" << character.getCharacterScore() << std::endl;
      }
    }
  }
}


bool judegMDOratio2(const Mat& image, const Rect& rect, std::vector<Point>& contour, Mat& result){

  Mat mser = image(rect);
  Mat mser_mat;
  cv::threshold(mser, mser_mat, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

  Rect normalRect = adaptive_charrect_from_rect(rect, image.cols, image.rows);
  Mat region = image(normalRect);
  Mat thresh_mat;
  cv::threshold(region, thresh_mat, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

  // count mser diff ratio
  int countdiff = countNonZero(thresh_mat) - countNonZero(mser_mat);

  float MserDiffOstuRatio = float(countdiff) / float(rect.area());

  if (MserDiffOstuRatio > 1) {
    /*std::cout << "MserDiffOstuRatio:" << MserDiffOstuRatio << std::endl;
    imshow("tmpMat", mser_mat);
    waitKey(0);
    imshow("tmpMat", thresh_mat);
    waitKey(0);*/

    cv::rectangle(result, rect, Scalar(0, 0, 0), 2);
    return false;
  }

  return true;
}

Rect interRect(const Rect& a, const Rect& b) {
  Rect c;
  int x1 = a.x > b.x ? a.x : b.x;
  int y1 = a.y > b.y ? a.y : b.y;
  c.width = (a.x + a.width < b.x + b.width ? a.x + a.width : b.x + b.width) - x1;
  c.height = (a.y + a.height < b.y + b.height ? a.y + a.height : b.y + b.height) - y1;
  c.x = x1;
  c.y = y1;
  if (c.width <= 0 || c.height <= 0)
    c = Rect();
  return c;
}

Rect mergeRect(const Rect& a, const Rect& b) {
  Rect c;
  int x1 = a.x < b.x ? a.x : b.x;
  int y1 = a.y < b.y ? a.y : b.y;
  c.width = (a.x + a.width > b.x + b.width ? a.x + a.width : b.x + b.width) - x1;
  c.height = (a.y + a.height > b.y + b.height ? a.y + a.height : b.y + b.height) - y1;
  c.x = x1;
  c.y = y1;
  return c;
}

// 计算两个长方形的交集与并集的比值
bool computeIOU(const RotatedRect& rrect1, const RotatedRect& rrect2, const int width, const int height, const float thresh, float& result) {
  Rect_<float> safe_rect1;
  calcSafeRect(rrect1, width, height,safe_rect1);

  Rect_<float> safe_rect2;
  calcSafeRect(rrect2, width, height, safe_rect2);

  Rect inter = interRect(safe_rect1, safe_rect2);
  Rect urect = mergeRect(safe_rect1, safe_rect2);

  float iou = (float)inter.area() / (float)urect.area();

  result = iou;

  if (iou > thresh) {
    return true;
  }

  return false;
}

float computeIOU(const RotatedRect& rrect1, const RotatedRect& rrect2, const int width, const int height) {
  Rect_<float> safe_rect1;
  calcSafeRect(rrect1, width, height, safe_rect1);

  Rect_<float> safe_rect2;
  calcSafeRect(rrect2, width, height, safe_rect2);

  Rect inter = interRect(safe_rect1, safe_rect2);
  Rect urect = mergeRect(safe_rect1, safe_rect2);

  float iou = (float)inter.area() / (float)urect.area();
  //std::cout << "iou" << iou << std::endl;

  return iou;
}

bool computeIOU(const Rect& rect1, const Rect& rect2, const float thresh, float& result) {

  Rect inter = interRect(rect1, rect2);
  Rect urect = mergeRect(rect1, rect2);

  float iou = (float)inter.area() / (float)urect.area();
  result = iou;

  if (iou > thresh) {
    return true;
  }

  return false;
}

float computeIOU(const Rect& rect1, const Rect& rect2) {
  //算重叠部分的Rect
  Rect inter = interRect(rect1, rect2);
  //算合并最小外接Rect
  Rect urect = mergeRect(rect1, rect2);
  //算二者面积比值
  float iou = (float)inter.area() / (float)urect.area();
 
  return iou;
}


// 直线的斜率几乎是相同的
// 若某条直线的斜率和其他非常不同，则其为离群值
// 用这个函数移除它
// removeRightOutliers(charGroup, roCharGroup, 0.2, 0.5, result);
void removeRightOutliers(std::vector<CCharacter>& charGroup, std::vector<CCharacter>& out_charGroup, double thresh1, double thresh2, Mat result) {
  //按中心点x坐标从小到大排序
  std::sort(charGroup.begin(), charGroup.end(),
    [](const CCharacter& r1, const CCharacter& r2) {
    return r1.getCenterPoint().x < r2.getCenterPoint().x;
  });

  std::vector<float> slopeVec;
  float slope_last = 0;
  for (size_t charGroup_i = 0; charGroup_i + 1 < charGroup.size(); charGroup_i++) {
    // line_between_two_points
    Vec4f line_btp;
    CCharacter leftChar = charGroup.at(charGroup_i);
    CCharacter rightChar = charGroup.at(charGroup_i + 1);
    std::vector<Point> two_points;
    two_points.push_back(leftChar.getCenterPoint());
    two_points.push_back(rightChar.getCenterPoint());
	// 计算出的直线信息存放在 line_btp 中，为 cv::Vec4f 类型
	// line[0]、line[1] 存放的是直线的方向向量
	// line[2]、line[3] 存放的是直线上一个点的坐标
	// 如果直线用 y = kx + b 来表示，那么 k = line[1] / line[0]，b = line[3] - k * line[2]。
    fitLine(Mat(two_points), line_btp, CV_DIST_L2, 0, 0.01, 0.01);
    float slope = line_btp[1] / line_btp[0];
    slopeVec.push_back(slope);

    if (0) {
      cv::line(result, leftChar.getCenterPoint(), rightChar.getCenterPoint(), Scalar(0, 0, 255));
    }
  }

  int uniformity_count = 0;
  int outlier_index = -1;
  for (size_t slopeVec_i = 0; slopeVec_i + 1 < slopeVec.size(); slopeVec_i++) {
	// 若相邻两个斜率差值小于0.2，则相似计数（uniformity_count）+1
    float slope_1 = slopeVec.at(slopeVec_i);
    float slope_2 = slopeVec.at(slopeVec_i+1);
    float slope_diff = abs(slope_1 - slope_2);
    if (0) {
      std::cout << "slope_diff:" << slope_diff << std::endl;
    }  
    if (slope_diff <= thresh1) {
      uniformity_count++;
    }
    if (0) {
      std::cout << "slope_1:" << slope_1 << std::endl;
      std::cout << "slope_2:" << slope_2 << std::endl;
    }
	// 若相邻两个斜率差值大于0.5且前面已经有至少2次斜率相似
	// 则记录离群值的索引
    if (1) {
      if (uniformity_count >= 2 && slope_diff >= thresh2) {
        outlier_index = slopeVec_i + 2;
        break;
      }
    }
  }
  if (0) {
    std::cout << "uniformity_count:" << uniformity_count << std::endl;
    std::cout << "outlier_index:" << outlier_index << std::endl;
  }

  // 剔除离群值
  for (int charGroup_i = 0; charGroup_i < (int)charGroup.size(); charGroup_i++) {
    if (charGroup_i != outlier_index) {
      CCharacter theChar = charGroup.at(charGroup_i);
      out_charGroup.push_back(theChar);
    }
  }

  if (0) {
    std::cout << "end:" << std::endl;
  }
}

Rect getSafeRect(Point2f center, float width, float height, Mat image) {
  int rows = image.rows;
  int cols = image.cols;

  float x = center.x;
  float y = center.y;

  float x_tl = (x - width / 2.f);
  float y_tl = (y - height / 2.f);

  float x_br = (x + width / 2.f);
  float y_br = (y + height / 2.f);

  x_tl = x_tl > 0.f ? x_tl : 0.f;
  y_tl = y_tl > 0.f ? y_tl : 0.f;
  x_br = x_br < (float)image.cols ? x_br : (float)image.cols;
  y_br = y_br < (float)image.rows ? y_br : (float)image.rows;

  Rect rect(Point((int)x_tl, int(y_tl)), Point((int)x_br, int(y_br)));
  return rect;
}

// 基于假设：车牌中两个相邻字符间距相等
// 添加未找到的字符，并且合并两个相近的小rect
void reFoundAndCombineRect(std::vector<CCharacter>& mserCharacter, float min_thresh, float max_thresh, 
  Vec2i dist, Rect maxrect, Mat result) {
  if (mserCharacter.size() == 0) {
    return;
  }

  std::sort(mserCharacter.begin(), mserCharacter.end(),
    [](const CCharacter& r1, const CCharacter& r2) {
    return r1.getCenterPoint().x < r2.getCenterPoint().x;
  });

  int comparDist = dist[0] * dist[0] + dist[1] * dist[1];
  if (0) {
    std::cout << "comparDist:" << comparDist << std::endl;
  }

  std::vector<CCharacter> reCharacters;

  size_t mserCharacter_i = 0;
  for (; mserCharacter_i + 1 < mserCharacter.size(); mserCharacter_i++) {
    CCharacter leftChar = mserCharacter.at(mserCharacter_i);
    CCharacter rightChar = mserCharacter.at(mserCharacter_i + 1);
    
    Point leftCenter = leftChar.getCenterPoint();
    Point rightCenter = rightChar.getCenterPoint();

    int x_diff = leftCenter.x - rightCenter.x;
    int y_diff = leftCenter.y - rightCenter.y;

    // 两个相邻中心点间的距离
    int distance2 = x_diff * x_diff + y_diff * y_diff;
    
    if (0) {
      std::cout << "distance2:" << distance2 << std::endl;
    }

    float ratio = (float)distance2 / (float)comparDist;
	// 若两相邻字符相距太远，则中间必有一个字符需要加入
    if (ratio > max_thresh) {//2.5
      float x_add = (float)(leftCenter.x + rightCenter.x) / 2.f;
      float y_add = (float)(leftCenter.y + rightCenter.y) / 2.f;

      float width = (float)maxrect.width;
      float height = (float)maxrect.height;

      float x_tl = (x_add - width / 2.f);
      float y_tl = (y_add - height / 2.f);

      //Rect rect_add((int)x_tl, (int)y_tl, (int)width, (int)height);
      Rect rect_add = getSafeRect(Point2f(x_add, y_add), width, height, result);

      reCharacters.push_back(leftChar);

      CCharacter charAdd;
      charAdd.setCenterPoint(Point((int)x_add, (int)y_add));
      charAdd.setCharacterPos(rect_add);
      reCharacters.push_back(charAdd);

	  // 强种子之间新找到的字符rect用橙色标出
      if (1) {
        cv::rectangle(result, rect_add, Scalar(0, 128, 255));
      }
    }//否则若两相邻字符相距太近，则需取交集合并
    else if (ratio < min_thresh) {
      Rect rect_union = leftChar.getCharacterPos() | rightChar.getCharacterPos();
      int x_add = rect_union.tl().x + rect_union.width / 2;
      int y_add = rect_union.tl().y + rect_union.height / 2;

      CCharacter charAdd;
      charAdd.setCenterPoint(Point(x_add, y_add));
      charAdd.setCharacterPos(rect_union);
      reCharacters.push_back(charAdd);
	  // 两个距离过近的合并rect用橙色标出
      if (1) {
        cv::rectangle(result, rect_union, Scalar(0, 128, 255));
      }

      mserCharacter_i++;
    }
    else {
      reCharacters.push_back(leftChar);
    }
  }

  if (mserCharacter_i + 1 == mserCharacter.size()) {
    reCharacters.push_back(mserCharacter.at(mserCharacter_i));
  }

  mserCharacter = reCharacters;
}


void removeOutliers(std::vector<CCharacter>& charGroup, double thresh, Mat result) {
  std::vector<Point> points;
  Vec4f line;
  for (auto character : charGroup) {
    points.push_back(character.getCenterPoint());
  }

  fitLine(Mat(points), line, CV_DIST_L2, 0, 0.01, 0.01);

  float k = line[1] / line[0];
  float x_1 = line[2];
  float y_1 = line[3];
  float step = 100;
  cv::line(result, Point2f(x_1 - step, y_1 - k*step), Point2f(x_1 + step, k*step + y_1), Scalar(0, 0, 255));

  float a = k;
  float b = -1;
  float c = y_1 - k * x_1;
  float sumdistance = 0;
  for (auto character : charGroup) {
    Point center = character.getCenterPoint();
    float distance = (a * center.x + b * center.y + c) / std::sqrt(a * a + b * b);
    std::cout << "distance:" << distance << std::endl;
    sumdistance += distance;
  }
  float avgdistance = sumdistance / (float)charGroup.size();

  std::vector<CCharacter>::iterator it = charGroup.begin();
  for (; it != charGroup.end();) {
    Point center = it->getCenterPoint();
    float distance = a * center.x + b * center.y + c;
    float ratio = distance / avgdistance;
    std::cout << "ratio:" << ratio << std::endl;
    if (ratio > (float)thresh) {
      it = charGroup.erase(it);
    }
    else {
      ++it;
    }
  }
}

//核心MSER文字匹配函数
void mserCharMatch(const Mat &src, const Mat &src_color, std::vector<Mat> &match, std::vector<CPlate>& out_plateVec_blue, std::vector<CPlate>& out_plateVec_yellow,
	bool usePlateMser, std::vector<RotatedRect>& out_plateRRect_blue, std::vector<RotatedRect>& out_plateRRect_yellow, const int show_type, int img_index,
  bool showDebug) {

	int show_flag;
	int write_flag;
	int write_num_flag = 0;

	if (show_type == 3){
		show_flag = 1;
		write_flag = 1;
	}
	else if (show_type == 2){
		show_flag = 0;
		write_flag = 1;
	}
	else if (show_type == 1){
		show_flag = 1;
		write_flag = 0;
	}
	else{
		show_flag = 0;
		write_flag = 0;
	}

  Mat image = src;

  std::vector<std::vector<std::vector<Point>>> all_contours;
  std::vector<std::vector<Rect>> all_boxes;
  all_contours.resize(2);
  all_contours.at(0).reserve(1024);
  all_contours.at(1).reserve(1024);
  all_boxes.resize(2);
  all_boxes.at(0).reserve(1024);
  all_boxes.at(1).reserve(1024);

  match.resize(2);

  std::vector<Color> flags;
  flags.push_back(BLUE);
  flags.push_back(YELLOW);

  const int imageArea = image.rows * image.cols;
  const int delta = 1;
  //const int delta = CParams::instance()->getParam2i();;
  const int minArea = 30;
  const double maxAreaRatio = 0.05;

  Ptr<MSER2> mser;

  //创建MSER对象
  //delta为灰度值的变化量，即公式1和2中的△；
  //_min_area和_max_area为检测到的组块面积的范围；
  mser = MSER2::create(delta, minArea, int(maxAreaRatio * imageArea));
  mser->detectRegions(image, all_contours.at(0), all_boxes.at(0), all_contours.at(1), all_boxes.at(1));

  //zc
  
  std::vector<Mat> image_DR_countours;
  image_DR_countours.resize(2);
  image_DR_countours.at(0) = image;
  image_DR_countours.at(1) = image;
  std::vector<Mat> image_DR_boxes;
  image_DR_boxes.resize(2);
  image_DR_boxes.at(0) = image;
  image_DR_boxes.at(1) = image;
  std::vector<Mat> image_charVec;
  image_charVec.resize(4);
  image_charVec.at(0) = image;
  image_charVec.at(1) = image;
  image_charVec.at(2) = image;
  image_charVec.at(3) = image;
  cvtColor(image_DR_countours.at(0), image_DR_countours.at(0), COLOR_GRAY2BGR);
  cvtColor(image_DR_countours.at(1), image_DR_countours.at(1), COLOR_GRAY2BGR);
  cvtColor(image_DR_boxes.at(0), image_DR_boxes.at(0), COLOR_GRAY2BGR);
  cvtColor(image_DR_boxes.at(1), image_DR_boxes.at(1), COLOR_GRAY2BGR);
  cvtColor(image_charVec.at(0), image_charVec.at(0), COLOR_GRAY2BGR);
  cvtColor(image_charVec.at(1), image_charVec.at(1), COLOR_GRAY2BGR);
  cvtColor(image_charVec.at(2), image_charVec.at(2), COLOR_GRAY2BGR);
  cvtColor(image_charVec.at(3), image_charVec.at(3), COLOR_GRAY2BGR);

  for (int m = 0; m < 2; m++){
	  //detectRegions检测出的像素点集输出
	  if (show_flag){
		  for (int i = 0; i < all_contours.at(m).size(); i++){
			  for (int j = 0; j < all_contours.at(m)[i].size(); j++)
				  circle(image_DR_countours.at(m), all_contours.at(m)[i][j], 2, cv::Scalar(0, 0, 255));
		  }

		  imshow("detectRegions", image_DR_countours.at(m));
		  waitKey(0);
		  destroyWindow("detectRegions");
	  }

	  if (write_flag){
		  for (int i = 0; i < all_contours.at(m).size(); i++){
			  for (int j = 0; j < all_contours.at(m)[i].size(); j++)
				  circle(image_DR_countours.at(m), all_contours.at(m)[i][j], 2, cv::Scalar(0, 0, 255));
		  }

		  std::string path = "resources/image/interface/show_detect/" + std::to_string(write_num_flag) + ".png";
		  cv::imwrite(path, image_DR_countours.at(m));
		  write_num_flag++;
	  }

	  //detectRegions检测出的外接矩形输出
	  if (show_flag){
		  for (int i = 0; i < all_contours.at(m).size(); i++)
			  rectangle(image_DR_boxes.at(m), all_boxes.at(m)[i], cv::Scalar(0, 0, 255));

		  imshow("detectRegions", image_DR_boxes.at(m));
		  waitKey(0);
		  destroyWindow("detectRegions");
	  }

	  if (write_flag){
		  for (int i = 0; i < all_contours.at(m).size(); i++)
			  rectangle(image_DR_boxes.at(m), all_boxes.at(m)[i], cv::Scalar(0, 0, 255));

		  std::string path = "resources/image/interface/show_detect/" + std::to_string(write_num_flag) + ".png";
		  cv::imwrite(path, image_DR_boxes.at(m));
		  write_num_flag++;
	  }
  }


  // mser检测 
  // color_index = 0 : mser-, 检测白字（蓝牌）
  // color_index = 1 : mser+, 检测黑字（黄牌）

#pragma omp parallel for
  for (int color_index = 0; color_index < 2; color_index++) {
    Color the_color = flags.at(color_index);

    std::vector<CCharacter> charVec;
    charVec.reserve(128);

    match.at(color_index) = Mat::zeros(image.rows, image.cols, image.type());

    Mat result = image.clone();
	//B & G & R均为Y
    cvtColor(result, result, COLOR_GRAY2BGR);

    size_t size = all_contours.at(color_index).size();

    int char_index = 0;
    int char_size = 20;

    // 车牌最大字符数设置
    int char_max_count = 7;
	float delta_heightEnlargeRatio = 0.0f;

    //依据字符尺寸判断并输出字符
    for (size_t index = 0; index < size; index++) {
      Rect rect = all_boxes.at(color_index)[index];
      std::vector<Point>& contour = all_contours.at(color_index)[index];

      if (usePlateMser) {
        RotatedRect rrect = minAreaRect(Mat(contour));
        if (verifyRotatedPlateSizes(rrect)) {
          //rotatedRectangle(result, rrect, Scalar(255, 0, 0), 2);
          if (the_color == BLUE) out_plateRRect_blue.push_back(rrect);
          if (the_color == YELLOW) out_plateRRect_yellow.push_back(rrect);
        }
      }

      //搜索字符
      if (verifyCharSizes(rect)) {
        Mat mserMat = adaptive_image_from_points(contour, rect, Size(char_size, char_size));
        Mat charInput = preprocessChar(mserMat, char_size);
        Rect charRect = rect;

        Point center(charRect.tl().x + charRect.width / 2, charRect.tl().y + charRect.height / 2);
        Mat tmpMat;
		// 最佳阈值ostu_level返回
        double ostu_level = cv::threshold(image(charRect), tmpMat, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

        //cv::circle(result, center, 3, Scalar(0, 0, 255), 2);

        // use judegMDOratio2 function to
        // remove the small lines in character like "zh-cuan"
        if (judegMDOratio2(image, rect, contour, result)) {
          CCharacter charCandidate;
          charCandidate.setCharacterPos(charRect);
          charCandidate.setCharacterMat(charInput);
          charCandidate.setOstuLevel(ostu_level);
          charCandidate.setCenterPoint(center);
          charCandidate.setIsChinese(false);
          charVec.push_back(charCandidate);
        }
      }
    }

	//显示charVecs
	if (show_flag){
		cv::String win_name;
		for (int i = 0; i < charVec.size(); i++){
			rectangle(image_charVec.at(color_index), charVec.at(i).getCharacterPos(), Scalar(0,0,255));
		}
		if (color_index == 0){
			win_name = "image_charVec_BLUE";
		}
		else if (color_index == 1){
			win_name = "image_charVec_YELLOW";
		}
		imshow(win_name, image_charVec.at(color_index));
		waitKey(0);
		destroyWindow(win_name);
	}

	if (write_flag){
		cv::String win_name;
		if (color_index == 0){
			win_name = "image_charVec_BLUE";
		}
		else if (color_index == 1){
			win_name = "image_charVec_YELLOW";
		}
		for (int i = 0; i < charVec.size(); i++){
			rectangle(image_charVec.at(color_index), charVec.at(i).getCharacterPos(), Scalar(0, 0, 255));
		}
		std::string path = "resources/image/interface/show_detect/" + win_name + ".png";
		cv::imwrite(path, image_charVec.at(color_index));
	}
	
    // improtant, use matrix multiplication to acclerate the 
    // classification of many samples. use the character 
    // score, we can use non-maximum superssion (nms) to 
    // reduce the characters which are not likely to be true
    // charaters, and use the score to select the strong seed
    // of which the score is larger than 0.9
	//用矩阵乘法来加快许多样本的分类，用字符分数，可以采用非极大值抑制
	//方法来减少低可能性的字符，并且可以将分数大于0.9的强种子选出来
    CharsIdentify::instance()->classify(charVec);

	//采用非极大值抑制方法来移除不太可能是字符的charVec
    double overlapThresh = 0.6;
    //double overlapThresh = CParams::instance()->getParam1f();
	//NMS去掉重合部分较大的charVec
    NMStoCharacter(charVec, overlapThresh);
    charVec.shrink_to_fit();

	//显示去重合以后的charVec
	if (show_flag){
		for (int i = 0; i < charVec.size(); i++){
			rectangle(image_charVec.at(color_index + 2), charVec.at(i).getCharacterPos(), Scalar(0, 0, 255));
		}
		imshow("charVec_remove_overlap", image_charVec.at(color_index + 2));
		waitKey(0);
		destroyWindow("charVec_remove_overlap");
	}

	if (write_flag){
		cv::String win_name;
		if (color_index == 0){
			win_name = "charVec_remove_overlap_BLUE";
		}
		else if (color_index == 1){
			win_name = "charVec_remove_overlap_YELLOW";
		}
		for (int i = 0; i < charVec.size(); i++){
			rectangle(image_charVec.at(color_index + 2), charVec.at(i).getCharacterPos(), Scalar(0, 0, 255));
		}
		std::string path = "resources/image/interface/show_detect/" + win_name + ".png";
		cv::imwrite(path, image_charVec.at(color_index + 2));
	}

    std::vector<CCharacter> strongSeedVec;
    strongSeedVec.reserve(64);
    std::vector<CCharacter> weakSeedVec;
    weakSeedVec.reserve(64);
    std::vector<CCharacter> littleSeedVec;
    littleSeedVec.reserve(64);

    //按seed强弱归类
    for (auto charCandidate : charVec) {
      
      Rect rect = charCandidate.getCharacterPos();
      double score = charCandidate.getCharacterScore();
	  // >=0.9
      if (charCandidate.getIsStrong()) {
        strongSeedVec.push_back(charCandidate);
      }// >=0.5 && <0.9
      else if (charCandidate.getIsWeak()) {
        weakSeedVec.push_back(charCandidate);
        //cv::rectangle(result, rect, Scalar(255, 0, 255));
      }// <0.5
      else if (charCandidate.getIsLittle()) {
        littleSeedVec.push_back(charCandidate);
        //cv::rectangle(result, rect, Scalar(255, 0, 255));
      }
    }

    std::vector<CCharacter> searchCandidate = charVec;

    // nms to srong seed, only leave the strongest one
	// NMS去掉重合部分较大的强种子strongSeedVec
    overlapThresh = 0.3;
    NMStoCharacter(strongSeedVec, overlapThresh);

    // 合并强种子字符为组
    std::vector<std::vector<CCharacter>> charGroupVec;
    charGroupVec.reserve(64);
    mergeCharToGroup(strongSeedVec, charGroupVec);

	// 为强种子组生成一条贯穿中心的直线
	// 根据假设，强种子mser rects必定在车牌上，因此可以
	// 用这些characters来拟合一条贯穿它们中心的直线
	// 作为车牌的中轴线
    std::vector<CPlate> plateVec;
    plateVec.reserve(16);
    for (auto charGroup : charGroupVec) {
	  // charGroup[0]代表聚类后的每个分组只取第一个元素
      Rect plateResult = charGroup[0].getCharacterPos();
      std::vector<Point> points;
      points.reserve(32);

      Vec4f line;
      int maxarea = 0;
      Rect maxrect;
      double ostu_level_sum = 0;
	  // image = src
	  // 初始化左点为最右点，右点为最左点
      int leftx = image.cols;
      Point leftPoint(leftx, 0);
      int rightx = 0;
      Point rightPoint(rightx, 0);

      std::vector<CCharacter> mserCharVec;
      mserCharVec.reserve(32);

      // 从CharGroup中移除离群值（remove outlier）
      std::vector<CCharacter> roCharGroup;
      roCharGroup.reserve(32);
      removeRightOutliers(charGroup, roCharGroup, 0.2, 0.5, result);

      for (auto character : roCharGroup) {
        Rect charRect = character.getCharacterPos();
		// 强种子在图中用绿色标出
        cv::rectangle(result, charRect, Scalar(0, 255, 0), 1);
		
		// 取并集，plateResult代表多个车牌字符的并集Rect
        plateResult |= charRect;

        Point center(charRect.tl().x + charRect.width / 2, charRect.tl().y + charRect.height / 2);
        points.push_back(center);
        mserCharVec.push_back(character);
        //cv::circle(result, center, 3, Scalar(0, 255, 0), 2);

        ostu_level_sum += character.getOstuLevel();
		// 找到车牌字符最左&最右中心点，以及最大字符极其面积
        if (charRect.area() > maxarea) {
          maxrect = charRect;
          maxarea = charRect.area();
        }
        if (center.x < leftPoint.x) {
          leftPoint = center;
        }
        if (center.x > rightPoint.x) {
          rightPoint = center;
        }
      }

	  if (show_flag){
		  imshow("result_strong_seed", result);
		  waitKey(0);
		  destroyWindow("result_strong_seed");
	  }
	  if (write_flag){
		  cv::String win_name;
		  if (color_index == 0){
			  win_name = "result_strong_seed_BLUE";
		  }
		  else if (color_index == 1){
			  win_name = "result_strong_seed_YELLOW";
		  }
		  std::string path = "resources/image/interface/show_detect/" + win_name + ".png";
		  cv::imwrite(path, result);
	  }

	  int count_yellow = 0, count_green = 0;

	  // 采用MSER+方法时，需判断黄牌还是绿牌
	  // 对强种子并集区域用黄色和绿色模版匹配
	  // 统计黄色和绿色像素点数
	  if (color_index == 1){
		  cv::rectangle(result, plateResult, Scalar(255, 255, 0));
		  Mat plateResultColor = src_color(plateResult);
		  if (0) {
			  imshow("plateResult", result);
			  waitKey(0);
			  destroyWindow("plateResult");
		  }
		  if (0) {
			  imshow("plateResultColor", plateResultColor);
			  waitKey(0);
			  destroyWindow("plateResultColor");
		  }
		  Mat yellow_b_match, green_b_match;
		  colorMatch(plateResultColor, yellow_b_match, YELLOW, true);
		  colorMatch(plateResultColor, green_b_match, GREEN, true);
		  if (0){
			  imshow("yellow_b_match", yellow_b_match);
			  waitKey(0);
			  destroyWindow("yellow_b_match");
		  }
		  if (0){
			  imshow("green_b_match", green_b_match);
			  waitKey(0);
			  destroyWindow("green_b_match");
		  }

		  for (int i = 0; i < yellow_b_match.rows; i++){
			  for (int j = 0; j < yellow_b_match.cols; j++){
				  if (yellow_b_match.at<uchar>(i, j) != 0){
					  count_yellow++;
				  }
			  }
		  }
		  for (int i = 0; i < green_b_match.rows; i++){
			  for (int j = 0; j < green_b_match.cols; j++){
				  if (green_b_match.at<uchar>(i, j) != 0){
					  count_green++;
				  }
			  }
		  }
	  }

      double ostu_level_avg = ostu_level_sum / (double)roCharGroup.size();
      if (1 && showDebug) {
        std::cout << "ostu_level_avg:" << ostu_level_avg << std::endl;
      }
	  // 计算最大字符宽高比
      float ratio_maxrect = (float)maxrect.width / (float)maxrect.height;

	  // 中心点拟合直线并画线
      if (points.size() >= 2 && ratio_maxrect >= 0.3) {
        fitLine(Mat(points), line, CV_DIST_L2, 0, 0.01, 0.01);

        float k = line[1] / line[0];
        //float angle = atan(k) * 180 / (float)CV_PI;
        //std::cout << "k:" << k << std::endl;
        //std::cout << "angle:" << angle << std::endl;
        //std::cout << "cos:" << 0.3 * cos(k) << std::endl;
        //std::cout << "ratio_maxrect:" << ratio_maxrect << std::endl;

        std::sort(mserCharVec.begin(), mserCharVec.end(),
          [](const CCharacter& r1, const CCharacter& r2) {
          return r1.getCharacterPos().tl().x < r2.getCharacterPos().tl().x;
        });

		// 车牌强种子字符集中间字符
        CCharacter midChar = mserCharVec.at(int(mserCharVec.size() / 2.f));
        Rect midRect = midChar.getCharacterPos();
        Point midCenter(midRect.tl().x + midRect.width / 2, midRect.tl().y + midRect.height / 2);

        int mindist = 7 * maxrect.width;
        std::vector<Vec2i> distVecVec;
        distVecVec.reserve(32);

        Vec2i mindistVec;
        Vec2i avgdistVec;

		// 计算车牌中任意两个相邻的字符的距离向量distVec
		// 用于计算最大搜索范围
		// 在下一步进而可以确定滑动窗口的最好位置
        for (size_t mser_i = 0; mser_i + 1 < mserCharVec.size(); mser_i++) {
          Rect charRect = mserCharVec.at(mser_i).getCharacterPos();
          //Point center(charRect.tl().x + charRect.width / 2, charRect.tl().y + charRect.height / 2);

          Rect charRectCompare = mserCharVec.at(mser_i + 1).getCharacterPos();
          //Point centerCompare(charRectCompare.tl().x + charRectCompare.width / 2,
          //  charRectCompare.tl().y + charRectCompare.height / 2);

          //int dist = charRectCompare.x - charRect.x;
          Vec2i distVec(charRectCompare.x - charRect.x, charRectCompare.y - charRect.y);
          distVecVec.push_back(distVec);

          //if (dist < mindist) {
          //  mindist = dist;
          //  mindistVec = distVec;
          //}
        }

        std::sort(distVecVec.begin(), distVecVec.end(),
          [](const Vec2i& r1, const Vec2i& r2) {
          return r1[0] < r2[0];
        });

        avgdistVec = distVecVec.at(int((distVecVec.size() - 1) / 2.f));

		// 字符间隔平均值
        float step = (float)avgdistVec[0];

        // 画一条白线作为车牌中轴线
        cv::line(result, Point2f(midCenter.x - step, midCenter.y - k*step), Point2f(midCenter.x + step, k*step + midCenter.y), Scalar(255, 255, 255));
		if (show_flag){
			imshow("white_line", result);
			waitKey(0);
			destroyWindow("white_line");
		}
		if (write_flag){
			cv::String win_name;
			if (color_index == 0){
				win_name = "white_line_BLUE";
			}
			else if (color_index == 1){
				win_name = "white_line_YELLOW";
			}
			std::string path = "resources/image/interface/show_detect/" + win_name + ".png";
			cv::imwrite(path, result);
		}

        CPlate plate;
        plate.setPlateLeftPoint(leftPoint);
        plate.setPlateRightPoint(rightPoint);

        plate.setPlateLine(line);
        plate.setPlatDistVec(avgdistVec);
        plate.setOstuLevel(ostu_level_avg);

        plate.setPlateMergeCharRect(plateResult);
        plate.setPlateMaxCharRect(maxrect);
        plate.setMserCharacter(mserCharVec);
		// 新能源参数
		if (count_green > count_yellow){
			char_max_count += 2;
			delta_heightEnlargeRatio = 0.2;
			plate.setIsNewEnergy(true);
		}
        plateVec.push_back(plate);
      }
    }

	// 用强种子大致构建出初始的车牌形状
	// 然后需要再去弱种子中找到字符
	// 由于前面已经用强种子拟合了一条车牌中轴线
	// 故可以在中轴线附近寻找弱种子中的字符
    for (auto plate : plateVec) {
      Vec4f line = plate.getPlateLine();
      Point leftPoint = plate.getPlateLeftPoint();
      Point rightPoint = plate.getPlateRightPoint();

      Rect plateResult = plate.getPlateMergeCharRect();
      Rect maxrect = plate.getPlateMaxCharRect();
      Vec2i dist = plate.getPlateDistVec();
      double ostu_level = plate.getOstuLevel();

      std::vector<CCharacter> mserCharacter = plate.getCopyOfMserCharacters();
      mserCharacter.reserve(16);

      float k = line[1] / line[0];
      float x_1 = line[2];
      float y_1 = line[3];

      std::vector<CCharacter> searchWeakSeedVec;
      searchWeakSeedVec.reserve(16);

      std::vector<CCharacter> searchRightWeakSeed;
      searchRightWeakSeed.reserve(8);
      std::vector<CCharacter> searchLeftWeakSeed;
      searchLeftWeakSeed.reserve(8);

      std::vector<CCharacter> slideRightWindow;
      slideRightWindow.reserve(8);
      std::vector<CCharacter> slideLeftWindow;
      slideLeftWindow.reserve(8);

      // draw weak seed and little seed from line;
      // search for mser rect
      if (1 && showDebug) {
        std::cout << "search for mser rect:" << std::endl;
      }

      if (0 && showDebug) {
        std::stringstream ss(std::stringstream::in | std::stringstream::out);
        ss << "resources/image/tmp/" << img_index << "_1_" << "searcgMserRect.jpg";
        imwrite(ss.str(), result);
      }
      if (1 && showDebug) {
        std::cout << "mserCharacter:" << mserCharacter.size() << std::endl;
      }

	  // 若强种子数量已经大于车牌最大字符数，则不需进行以下步骤
	  // 判断情况包含强弱种子间距和rect相似性来提高种子生长算法的鲁棒性
      if (mserCharacter.size() < char_max_count) {
        double thresh1 = 0.15;
        double thresh2 = 2.0;
		// 强种子右侧找
        searchWeakSeed(searchCandidate, searchRightWeakSeed, thresh1, thresh2, line, rightPoint,
          maxrect, plateResult, result, CharSearchDirection::RIGHT);
        if (1 && showDebug) {
          std::cout << "searchRightWeakSeed:" << searchRightWeakSeed.size() << std::endl;
        }
		// 找到的弱种子字符标蓝色
        for (auto seed : searchRightWeakSeed) {
          cv::rectangle(result, seed.getCharacterPos(), Scalar(255, 0, 0), 1);
          mserCharacter.push_back(seed);
        }
		// 强种子左侧找
        searchWeakSeed(searchCandidate, searchLeftWeakSeed, thresh1, thresh2, line, leftPoint,
          maxrect, plateResult, result, CharSearchDirection::LEFT);
        if (1 && showDebug) {
          std::cout << "searchLeftWeakSeed:" << searchLeftWeakSeed.size() << std::endl;
        }
		// 找到的弱种子字符标蓝色
        for (auto seed : searchLeftWeakSeed) {
          cv::rectangle(result, seed.getCharacterPos(), Scalar(255, 0, 0), 1);
          mserCharacter.push_back(seed);
        }
      }

	  // 考虑弱种子在强种子中间 & 合并同属一个字符的种子
	  // 该步骤完成后，可根据最大字符数决定是否采用滑动窗口继续寻找
      float min_thresh = 0.3f;
      float max_thresh = 2.5f;
      reFoundAndCombineRect(mserCharacter, min_thresh, max_thresh, dist, maxrect, result);

	  // MSER方法可能无法找到带污渍的，低光照，“川”字等
	  // 此时字符数仍然小于字符数量，需采用滑动窗口法来继续寻找
      if (mserCharacter.size() < char_max_count) {
        if (1 && showDebug) {
          std::cout << "search chinese:" << std::endl;
          std::cout << "judege the left is chinese:" << std::endl;
        }

        // 若当前第一个字符已经是中文字符
		// 不需要再向左移动窗口检测
        bool leftIsChinese = false;
        if (1) {
          std::sort(mserCharacter.begin(), mserCharacter.end(),
            [](const CCharacter& r1, const CCharacter& r2) {
            return r1.getCharacterPos().tl().x < r2.getCharacterPos().tl().x;
          });

          CCharacter leftChar = mserCharacter[0];

          //Rect theRect = adaptive_charrect_from_rect(leftChar.getCharacterPos(), image.cols, image.rows);
          Rect theRect = leftChar.getCharacterPos();
          //cv::rectangle(result, theRect, Scalar(255, 0, 0), 1);

          Mat region = image(theRect);
          Mat binary_region;

          ostu_level = cv::threshold(region, binary_region, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
          if (1 && showDebug) {
            std::cout << "left : ostu_level:" << ostu_level << std::endl;
          }
          
		  //格式化字符rect，并倾斜矫正
          Mat charInput = preprocessChar(binary_region, char_size);
          if (0 /*&& showDebug*/) {
            imshow("charInput", charInput);
            waitKey(0);
            destroyWindow("charInput");
          }

          std::string label = "";
          float maxVal = -2.f;
          leftIsChinese = CharsIdentify::instance()->isCharacter(charInput, label, maxVal, true);
          //auto character = CharsIdentify::instance()->identifyChinese(charInput, maxVal, leftIsChinese);
          //label = character.second;
          if (0 /* && showDebug*/) {
            std::cout << "isChinese:" << leftIsChinese << std::endl;
            std::cout << "chinese:" << label;
            std::cout << "__score:" << maxVal << std::endl;
          }
        }

        // 若最左字符不是中文，需要向左滑动窗口来找MSER方法未找出的字符
        float ratioWindow  = 0.4f;
        float threshIsCharacter = 0.8f;
        
        if (!leftIsChinese) {
          slideWindowSearch(image, slideLeftWindow, line, leftPoint, dist, ostu_level, ratioWindow, threshIsCharacter,
            maxrect, plateResult, CharSearchDirection::LEFT, true, result);
          if (1 && showDebug) {
            std::cout << "slideLeftWindow:" << slideLeftWindow.size() << std::endl;
          }
          for (auto window : slideLeftWindow) {
			// 红色标出滑动窗口找到的字符
            cv::rectangle(result, window.getCharacterPos(), Scalar(0, 0, 255), 1);
            mserCharacter.push_back(window);
          }
        }
      }

      // 若找到的字符数还是不等于设定的字符数
	  // 则需要继续向右滑动寻找
      if (mserCharacter.size() < char_max_count) {
        // change ostu_level
        float ratioWindow  = 0.4f;
        float threshIsCharacter = 0.8f;
        
        slideWindowSearch(image, slideRightWindow, line, rightPoint, dist, plate.getOstuLevel(), ratioWindow, threshIsCharacter,
          maxrect, plateResult, CharSearchDirection::RIGHT, false, result);
        if (1 && showDebug) {
          std::cout << "slideRightWindow:" << slideRightWindow.size() << std::endl;
        }
        for (auto window : slideRightWindow) {
          cv::rectangle(result, window.getCharacterPos(), Scalar(0, 0, 255), 1);
          mserCharacter.push_back(window);
        }
      }

      // 计算车牌角度
      float angle = atan(k) * 180 / (float)CV_PI;
      if (1 && showDebug) {
        std::cout << "k:" << k << std::endl;
        std::cout << "angle:" << angle << std::endl;
      }

      // 需要扩大plateResult的Rect来使整个车牌包含在内
      float widthEnlargeRatio = 1.25f;//zc, 1.15 参数可调 //1.25
	  float heightEnlargeRatio = 1.3f + delta_heightEnlargeRatio;//zc, 1.25 参数可调 xny-1.5 //1.3
      RotatedRect platePos(Point2f((float)plateResult.x + plateResult.width / 2.f, (float)plateResult.y + plateResult.height / 2.f),
        Size2f(plateResult.width * widthEnlargeRatio, maxrect.height * heightEnlargeRatio), angle);

	  // 找到全部字符区域
	  if (show_flag) {
		  imshow("all_searched", result);
		  waitKey(0);
		  destroyWindow("all_searched");
	  }
	  if (write_flag){
		  cv::String win_name;
		  if (color_index == 0){
			  win_name = "all_searched_BLUE";
		  }
		  else if (color_index == 1){
			  win_name = "all_searched_YELLOW";
		  }
		  std::string path = "resources/image/interface/show_detect/" + win_name + ".png";
		  cv::imwrite(path, result);
	  }

      // 由候选区域形态判断是否可能为车牌
      if (verifyRotatedPlateSizes(platePos)) {
        rotatedRectangle(result, platePos, Scalar(0, 0, 255), 1);

        plate.setPlatePos(platePos);
        plate.setPlateColor(the_color);
        plate.setPlateLocateType(CMSER);

        if (the_color == BLUE) out_plateVec_blue.push_back(plate);
        if (the_color == YELLOW) out_plateVec_yellow.push_back(plate);
      }

	  // 需要二值化图像来使用deskew函数偏斜扭转
      if (1) {
        for (auto mserChar : mserCharacter) {
          Rect rect = mserChar.getCharacterPos();
          match.at(color_index)(rect) = 255;
        }
        cv::line(match.at(color_index), rightPoint, leftPoint, Scalar(255));
      }
    }

    if (0 /*&& showDebug*/) {
      imshow("result", result);
      waitKey(0);
      destroyWindow("result");
    }

	if (show_flag) {
      imshow("match", match.at(color_index));
      waitKey(0);
      destroyWindow("match");
    }
	if (write_flag){
		cv::String win_name;
		if (color_index == 0){
			win_name = "match_BLUE";
		}
		else if (color_index == 1){
			win_name = "match_YELLOW";
		}
		std::string path = "resources/image/interface/show_detect/" + win_name + ".png";
		cv::imwrite(path, match.at(color_index));
	}
	

	if (show_flag) {
      std::stringstream ss(std::stringstream::in | std::stringstream::out);
      ss << "resources/image/tmp/plateDetect/plate_" << img_index << "_" << the_color << ".jpg";
	  imwrite(ss.str(), result);
	  imshow("mser_result", result);
	  waitKey(0);
	  destroyWindow("mser_result");
    }
	if (write_flag){
		cv::String win_name;
		if (color_index == 0){
			win_name = "mser_result_BLUE";
		}
		else if (color_index == 1){
			win_name = "mser_result_YELLOW";
		}
		std::string path = "resources/image/interface/show_detect/" + win_name + ".png";
		cv::imwrite(path, result);
	}
  }


}

// 普通大津阈值方法二值化
void ostu(InputArray _src, Color type) {
	Mat src = _src.getMat();

	if (type == BLUE) {
		cv::threshold(src, src, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	}
	else if (type == YELLOW) {
		cv::threshold(src, src, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY_INV);
	}
	else if (type == GREEN) {
		cv::threshold(src, src, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY_INV);
	}
	else if (type == WHITE) {
		cv::threshold(src, src, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY_INV);
	}
	else {
		cv::threshold(src, src, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	}
}

// this spatial_ostu algorithm are robust to 
// the plate which has the same light shine, which is that
// the light in the left of the plate is strong than the right.
void spatial_ostu(InputArray _src, int grid_x, int grid_y, Color type) {
  Mat src = _src.getMat();

  int width = src.cols / grid_x;
  int height = src.rows / grid_y;

  // iterate through grid
  for (int i = 0; i < grid_y; i++) {
    for (int j = 0; j < grid_x; j++) {
      Mat src_cell = Mat(src, Range(i*height, (i + 1)*height), Range(j*width, (j + 1)*width));
      if (type == BLUE) {
        cv::threshold(src_cell, src_cell, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
      }
      else if (type == YELLOW) {
        cv::threshold(src_cell, src_cell, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY_INV);
      } 
	  else if (type == GREEN) {
		  cv::threshold(src_cell, src_cell, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY_INV);
	  }
      else if (type == WHITE) {
        cv::threshold(src_cell, src_cell, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY_INV);
      }
      else {
        cv::threshold(src_cell, src_cell, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
      }
    }
  }
}


bool mat_valid_position(const Mat& mat, int row, int col) {
  return row >= 0 && col >= 0 && row < mat.rows && col < mat.cols;
}


template<class T>
static void mat_set_invoke(Mat& mat, int row, int col, const Scalar& value) {
  if (1 == mat.channels()) {
    mat.at<T>(row, col) = (T)value.val[0];
  }
  else if (3 == mat.channels()) {
    T* ptr_src = mat.ptr<T>(row, col);
    *ptr_src++ = (T)value.val[0];
    *ptr_src++ = (T)value.val[1];
    *ptr_src = (T)value.val[2];
  }
  else if (4 == mat.channels()) {
    T* ptr_src = mat.ptr<T>(row, col);
    *ptr_src++ = (T)value.val[0];
    *ptr_src++ = (T)value.val[1];
    *ptr_src++ = (T)value.val[2];
    *ptr_src = (T)value.val[3];
  }
}

void setPoint(Mat& mat, int row, int col, const Scalar& value) {
  if (CV_8U == mat.depth()) {
    mat_set_invoke<uchar>(mat, row, col, value);
  }
  else if (CV_8S == mat.depth()) {
    mat_set_invoke<char>(mat, row, col, value);
  }
  else if (CV_16U == mat.depth()) {
    mat_set_invoke<ushort>(mat, row, col, value);
  }
  else if (CV_16S == mat.depth()) {
    mat_set_invoke<short>(mat, row, col, value);
  }
  else if (CV_32S == mat.depth()) {
    mat_set_invoke<int>(mat, row, col, value);
  }
  else if (CV_32F == mat.depth()) {
    mat_set_invoke<float>(mat, row, col, value);
  }
  else if (CV_64F == mat.depth()) {
    mat_set_invoke<double>(mat, row, col, value);
  }
}

Rect adaptive_charrect_from_rect(const Rect& rect, int maxwidth, int maxheight) {
  int expendWidth = 0;

  if (rect.height > 3 * rect.width) {
    expendWidth = (rect.height / 2 - rect.width) / 2;
  }

  //Rect resultRect(rect.tl().x - expendWidth, rect.tl().y, 
  //  rect.width + expendWidth * 2, rect.height);

  int tlx = rect.tl().x - expendWidth > 0 ? rect.tl().x - expendWidth : 0;
  int tly = rect.tl().y;

  int brx = rect.br().x + expendWidth < maxwidth ? rect.br().x + expendWidth : maxwidth;
  int bry = rect.br().y;

  Rect resultRect(tlx, tly, brx - tlx, bry - tly);
  return resultRect;
}


Mat adaptive_image_from_points(const std::vector<Point>& points,
  const Rect& rect, const Size& size, const Scalar& backgroundColor /* = ml_color_white */, 
  const Scalar& forgroundColor /* = ml_color_black */, bool gray /* = true */) {
  int expendHeight = 0;
  int expendWidth = 0;

  if (rect.width > rect.height) {
    expendHeight = (rect.width - rect.height) / 2;
  }
  else if (rect.height > rect.width) {
    expendWidth = (rect.height - rect.width) / 2;
  }

  Mat image(rect.height + expendHeight * 2, rect.width + expendWidth * 2, gray ? CV_8UC1 : CV_8UC3, backgroundColor);

  for (int i = 0; i < (int)points.size(); ++i) {
    Point point = points[i];
    Point currentPt(point.x - rect.tl().x + expendWidth, point.y - rect.tl().y + expendHeight);
    if (mat_valid_position(image, currentPt.y, currentPt.x)) {
      setPoint(image, currentPt.y, currentPt.x, forgroundColor);
    }
  }

  Mat result;
  cv::resize(image, result, size, 0, 0, INTER_NEAREST);

  return result;
}

// 移动图片
Mat translateImg(Mat img, int offsetx, int offsety){
  Mat dst;
  Mat trans_mat = (Mat_<double>(2, 3) << 1, 0, offsetx, 0, 1, offsety);
  warpAffine(img, dst, trans_mat, img.size());
  return dst;
}

// 旋转图片
Mat rotateImg(Mat source, float angle){
  Point2f src_center(source.cols / 2.0F, source.rows / 2.0F);
  Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
  Mat dst;
  warpAffine(source, dst, rot_mat, source.size());
  return dst;
}

// 计算安全的矩形坐标，若不存在，返回false
bool calcSafeRect(const RotatedRect &roi_rect, const Mat &src,
  Rect_<float> &safeBoundRect) {
  Rect_<float> boudRect = roi_rect.boundingRect();

  // 防止坐标越界
  float tl_x = boudRect.x > 0 ? boudRect.x : 0;
  float tl_y = boudRect.y > 0 ? boudRect.y : 0;

  float br_x = boudRect.x + boudRect.width < src.cols
    ? boudRect.x + boudRect.width - 1
    : src.cols - 1;
  float br_y = boudRect.y + boudRect.height < src.rows
    ? boudRect.y + boudRect.height - 1
    : src.rows - 1;

  float roi_width = br_x - tl_x;
  float roi_height = br_y - tl_y;

  if (roi_width <= 0 || roi_height <= 0) return false;

  // 返回安全的矩形坐标
  safeBoundRect = Rect_<float>(tl_x, tl_y, roi_width, roi_height);

  return true;
}

bool calcSafeRect(const RotatedRect &roi_rect, const int width, const int height,
  Rect_<float> &safeBoundRect) {
  Rect_<float> boudRect = roi_rect.boundingRect();

  float tl_x = boudRect.x > 0 ? boudRect.x : 0;
  float tl_y = boudRect.y > 0 ? boudRect.y : 0;

  float br_x = boudRect.x + boudRect.width < width
    ? boudRect.x + boudRect.width - 1
    : width - 1;
  float br_y = boudRect.y + boudRect.height < height
    ? boudRect.y + boudRect.height - 1
    : height - 1;

  float roi_width = br_x - tl_x;
  float roi_height = br_y - tl_y;

  if (roi_width <= 0 || roi_height <= 0) return false;

  //  a new rect not out the range of mat

  safeBoundRect = Rect_<float>(tl_x, tl_y, roi_width, roi_height);

  return true;
}

}