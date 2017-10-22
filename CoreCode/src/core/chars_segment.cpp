#include "easypr/core/chars_segment.h"
#include "easypr/core/chars_identify.h"
#include "easypr/core/core_func.h"
#include "easypr/core/params.h"
#include "easypr/config.h"

using namespace std;

namespace easypr {

const float DEFAULT_BLUEPERCEMT = 0.3f;
const float DEFAULT_WHITEPERCEMT = 0.1f;

CCharsSegment::CCharsSegment() {
  m_LiuDingSize = DEFAULT_LIUDING_SIZE;
  m_theMatWidth = DEFAULT_MAT_WIDTH;

  m_ColorThreshold = DEFAULT_COLORTHRESHOLD;
  m_BluePercent = DEFAULT_BLUEPERCEMT;
  m_WhitePercent = DEFAULT_WHITEPERCEMT;

  m_debug = DEFAULT_DEBUG;
}

// 判断字符尺寸
bool CCharsSegment::verifyCharSizes(Mat r) {
  // 字符尺寸
  float aspect = 45.0f / 90.0f;
  float charAspect = (float)r.cols / (float)r.rows;
  float error = 0.7f;
  float minHeight = 10.f;
  float maxHeight = 35.f;
  // We have a different aspect ratio for number 1, and it can be ~0.2
  float minAspect = 0.05f;
  float maxAspect = aspect + aspect * error;
  // 计算非零像素数
  int area = cv::countNonZero(r);
  // 图片总面积
  int bbArea = r.cols * r.rows;
  // 非零像素百分比
  int percPixels = area / bbArea;

  if (percPixels <= 1 && charAspect > minAspect && charAspect < maxAspect &&
      r.rows >= minHeight && r.rows < maxHeight)
    return true;
  else
    return false;
}

// 预处理，仿射变换
Mat CCharsSegment::preprocessChar(Mat in) {
  // Remap image
  int h = in.rows;
  int w = in.cols;

  int charSize = CHAR_SIZE;

  Mat transformMat = Mat::eye(2, 3, CV_32F);
  int m = max(w, h);
  transformMat.at<float>(0, 2) = float(m / 2 - w / 2);
  transformMat.at<float>(1, 2) = float(m / 2 - h / 2);

  Mat warpImage(m, m, in.type());
  warpAffine(in, warpImage, transformMat, warpImage.size(), INTER_LINEAR,
             BORDER_CONSTANT, Scalar(0));

  Mat out;
  resize(warpImage, out, Size(charSize, charSize));

  return out;
}

// 选择对于中文判定效果最佳的二值化方法（OSTU或Adap），并返回用其进行二值化后输入ANN模型预测后的分数
void CCharsSegment::judgeChinese(Mat in, Mat& out, Color plateType) {
  
  Mat auxRoi = in;
  float valOstu = -1.f, valAdap = -1.f;
  Mat roiOstu, roiAdap;
  bool isChinese = true;
  if (1) {
    if (BLUE == plateType) {
      threshold(auxRoi, roiOstu, 0, 255, CV_THRESH_BINARY + CV_THRESH_OTSU);
    }
    else if (YELLOW == plateType) {
      threshold(auxRoi, roiOstu, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);
    }
	else if (GREEN == plateType) {
		threshold(auxRoi, roiOstu, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);
	}
    else if (WHITE == plateType) {
      threshold(auxRoi, roiOstu, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);
    }
    else {
      threshold(auxRoi, roiOstu, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
    }
    roiOstu = preprocessChar(roiOstu);
    if (0) {
      imshow("roiOstu", roiOstu);
      waitKey(0);
      destroyWindow("roiOstu");
    }
    auto character = CharsIdentify::instance()->identifyChinese(roiOstu, valOstu, isChinese);
  }
  if (1) {
    if (BLUE == plateType) {
      adaptiveThreshold(auxRoi, roiAdap, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 0);
    }
    else if (YELLOW == plateType) {
      adaptiveThreshold(auxRoi, roiAdap, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 3, 0);
    }
	else if (GREEN == plateType) {
		adaptiveThreshold(auxRoi, roiAdap, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 3, 0);
	}
    else if (WHITE == plateType) {
      adaptiveThreshold(auxRoi, roiAdap, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 3, 0);
    }
    else {
      adaptiveThreshold(auxRoi, roiAdap, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 0);
    }
    roiAdap = preprocessChar(roiAdap);
    auto character = CharsIdentify::instance()->identifyChinese(roiAdap, valAdap, isChinese);
  }

  std::cout << "valOstu: " << valOstu << std::endl;
  std::cout << "valAdap: " << valAdap << std::endl;

  if (valOstu >= valAdap) {
    out = roiOstu;
  }
  else {
    out = roiAdap;
  }

}

// 采用滑动窗口法左右微调确定chineseRect的正确位置
bool slideChineseWindow(Mat& image, Rect mr, Mat& newRoi, Color plateType, float slideLengthRatio, bool useAdapThreshold) {
  std::vector<CCharacter> charCandidateVec;
  
  Rect maxrect = mr;
  Point tlPoint = mr.tl();

  bool isChinese = true;
  int slideLength = int(slideLengthRatio * maxrect.width);
  int slideStep = 1;
  int fromX = 0;
  fromX = tlPoint.x;
  
  for (int slideX = -slideLength; slideX < slideLength; slideX += slideStep) {
    float x_slide = 0;

    x_slide = float(fromX + slideX);

    float y_slide = (float)tlPoint.y;
    Point2f p_slide(x_slide, y_slide);

    int chineseWidth = int(maxrect.width);
    int chineseHeight = int(maxrect.height);

    Rect rect(Point2f(x_slide, y_slide), Size(chineseWidth, chineseHeight));

    if (rect.tl().x < 0 || rect.tl().y < 0 || rect.br().x >= image.cols || rect.br().y >= image.rows)
      continue;

    Mat auxRoi = image(rect);

    Mat roiOstu, roiAdap;
    if (1) {
      if (BLUE == plateType) {
        threshold(auxRoi, roiOstu, 0, 255, CV_THRESH_BINARY + CV_THRESH_OTSU);
      }
      else if (YELLOW == plateType) {
        threshold(auxRoi, roiOstu, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);
      }
	  else if (GREEN == plateType) {
		  threshold(auxRoi, roiOstu, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);
	  }
      else if (WHITE == plateType) {
        threshold(auxRoi, roiOstu, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);
      }
      else {
        threshold(auxRoi, roiOstu, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
      }
      roiOstu = preprocessChar(roiOstu, kChineseSize_pic);

      CCharacter charCandidateOstu;
      charCandidateOstu.setCharacterPos(rect);
      charCandidateOstu.setCharacterMat(roiOstu);
      charCandidateOstu.setIsChinese(isChinese);
      charCandidateVec.push_back(charCandidateOstu);
    }
	// 使用自适应阈值方法二值化
    if (useAdapThreshold) {
      if (BLUE == plateType) {
        adaptiveThreshold(auxRoi, roiAdap, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 0);
      }
      else if (YELLOW == plateType) {
        adaptiveThreshold(auxRoi, roiAdap, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 3, 0);
      }
	  else if (GREEN == plateType) {
		  adaptiveThreshold(auxRoi, roiAdap, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 3, 0);
	  }
      else if (WHITE == plateType) {
        adaptiveThreshold(auxRoi, roiAdap, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 3, 0);
      }
      else {
        adaptiveThreshold(auxRoi, roiAdap, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 0);
      }
      roiAdap = preprocessChar(roiAdap, kChineseSize_pic);

      CCharacter charCandidateAdap;
      charCandidateAdap.setCharacterPos(rect);
      charCandidateAdap.setCharacterMat(roiAdap);
      charCandidateAdap.setIsChinese(isChinese);
      charCandidateVec.push_back(charCandidateAdap);
    }

  }

  // ANN模型预测其为中文字符的概率
  CharsIdentify::instance()->classifyChinese(charCandidateVec);

  // NMS去重
  double overlapThresh = 0.1;
  NMStoCharacter(charCandidateVec, overlapThresh);

  if (charCandidateVec.size() >= 1) {
    std::sort(charCandidateVec.begin(), charCandidateVec.end(),
      [](const CCharacter& r1, const CCharacter& r2) {
      return r1.getCharacterScore() > r2.getCharacterScore();
    });

	// 只取一个ROI框
    newRoi = charCandidateVec.at(0).getCharacterMat();
    return true;
  }

  return false;

}

//分割重要函数
int CCharsSegment::charsSegment(Mat input, vector<Mat>& resultVec, const int show_type, Color color, bool is_newEnergy) {
  if (!input.data) return 0x01;

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

  Color plateType = color;

  Mat input_grey;
  cvtColor(input, input_grey, CV_BGR2GRAY);

  // 车牌保存
  /*
  if (1){
		  std::string path = "resources/image/chepai/" + std::to_string(rand()) + ".jpg";
		  cv::imwrite(path, input);
  }*/

  if (show_flag) {
    imshow("0灰度化", input_grey);
    waitKey(0);
    destroyWindow("0灰度化");
  }

  if (write_flag){
	  std::string path = "resources/image/interface/show_segment/" + std::to_string(write_num_flag) + ".png";
	  cv::imwrite(path, input_grey);
	  write_num_flag++;
  }

  Mat img_threshold;
  Mat img_ostu;

  img_threshold = input_grey.clone();
  img_ostu = input_grey.clone();

  // 空间大津阈值二值化，可以和大津阈值进行对比
  spatial_ostu(img_threshold, 8, 2, plateType);//参数可调
  // 普通大津阈值方法二值化
  ostu(img_ostu, plateType);

  if (show_flag) {
	imshow("1空间大津阈值", img_threshold);
    waitKey(0);
	destroyWindow("1空间大津阈值");
  }
  if (write_flag){
	  std::string path = "resources/image/interface/show_segment/" + std::to_string(write_num_flag) + ".png";
	  cv::imwrite(path, img_threshold);
	  write_num_flag++;
  }

  if (show_flag) {
	  imshow("2普通大津阈值", img_ostu);
	  waitKey(0);
	  destroyWindow("2普通大津阈值");
  }
  if (write_flag){
	  std::string path = "resources/image/interface/show_segment/" + std::to_string(write_num_flag) + ".png";
	  cv::imwrite(path, img_ostu);
	  write_num_flag++;
  }

  // 去除铆钉和水平线
  if (!clearLiuDing(img_threshold)) return 0x02;

  if (show_flag){
	  imshow("3去铆钉", img_threshold);
	  waitKey(0);
	  destroyWindow("3去铆钉");
  }
  if (write_flag){
	  std::string path = "resources/image/interface/show_segment/" + std::to_string(write_num_flag) + ".png";
	  cv::imwrite(path, img_threshold);
	  write_num_flag++;
  }

  Mat img_contours;
  Mat img_before_verify;
  Mat img_after_verify;
  img_threshold.copyTo(img_contours);

  img_threshold.copyTo(img_before_verify);
  cvtColor(img_before_verify, img_before_verify, CV_GRAY2BGR);

  img_threshold.copyTo(img_after_verify);
  cvtColor(img_after_verify, img_after_verify, CV_GRAY2BGR);

  vector<vector<Point> > contours;
  findContours(img_contours,
               contours,
               CV_RETR_EXTERNAL,       // 检索外部轮廓，找到的轮廓里面没有小轮廓（洞）
               CV_CHAIN_APPROX_NONE);  // 获取轮廓上所有像素点

  for (int i = 0; i < contours.size(); i++){
	  Rect temp_mr = boundingRect(Mat(contours.at(i)));
	  rectangle(img_before_verify, temp_mr, Scalar(0, 0, 255));
  }

  if (show_flag) {
	  imshow("4剔除非字符前", img_before_verify);
	  waitKey(0);
	  destroyWindow("4剔除非字符前");
  }
  if (write_flag){
	  std::string path = "resources/image/interface/show_segment/" + std::to_string(write_num_flag) + ".png";
	  cv::imwrite(path, img_before_verify);
	  write_num_flag++;
  }

  vector<vector<Point> >::iterator itc = contours.begin();//迭代器
  vector<Rect> vecRect;

  while (itc != contours.end()) {
    Rect mr = boundingRect(Mat(*itc));
	//构造函数：Mat::Mat(const Mat& m, const Rect& roi)
    Mat auxRoi(img_threshold, mr);
	//截取区域是否字符判断
    if (verifyCharSizes(auxRoi)) vecRect.push_back(mr);
    ++itc;
  }

  for (int i = 0; i < vecRect.size(); i++){
	  rectangle(img_after_verify, vecRect.at(i), Scalar(0, 0, 255));
  }
  if (show_flag) {
	  imshow("5剔除非字符后", img_after_verify);
	  waitKey(0);
	  destroyWindow("5剔除非字符后");
  }
  if (write_flag){
	  std::string path = "resources/image/interface/show_segment/" + std::to_string(write_num_flag) + ".png";
	  cv::imwrite(path, img_after_verify);
	  write_num_flag++;
  }

  if (vecRect.size() == 0) return 0x03;

  // 按从左至右排序
  vector<Rect> sortedRect(vecRect);
  std::sort(sortedRect.begin(), sortedRect.end(),
            [](const Rect& r1, const Rect& r2) { return r1.x < r2.x; });

  size_t specIndex = 0;

  // 根据先验知识得到特殊字符的索引
  specIndex = GetSpecificRect(sortedRect);

  // 由SpecificRect得到ChineseRect
  Rect chineseRect;
  if (specIndex < sortedRect.size())
    chineseRect = GetChineseRect(sortedRect[specIndex]);
  else
    return 0x04;

  // 显示ChineseRect
  Mat img_chineseRect;
  img_chineseRect = input;
  if (show_flag) {
	rectangle(img_chineseRect, chineseRect, Scalar(0, 0, 255));
	imshow("6中文字符", img_chineseRect);
    waitKey(0);
    destroyWindow("6中文字符");
  }
  if (write_flag){
	  rectangle(img_chineseRect, chineseRect, Scalar(0, 0, 255));
	  std::string path = "resources/image/interface/show_segment/" + std::to_string(write_num_flag) + ".png";
	  cv::imwrite(path, img_chineseRect);
	  write_num_flag++;
  }

  vector<Rect> newSortedRect;
  newSortedRect.push_back(chineseRect);
  RebuildRect(sortedRect, newSortedRect, specIndex, is_newEnergy);

  if (newSortedRect.size() == 0) return 0x05;
  
  //文件写入
  if (show_flag) {
	  for (int a = 0; a < newSortedRect.size(); a++)
		  rectangle(input, newSortedRect[a], Scalar(0, 0, 255));
	  imshow("7最终字符分割结果", input);
	  waitKey(0);
	  destroyWindow("7最终字符分割结果");
  }
  if (write_flag){
	  for (int a = 0; a < newSortedRect.size(); a++)
		  rectangle(input, newSortedRect[a], Scalar(0, 0, 255));
	  std::string path1 = "resources/image/interface/show_segment/" + std::to_string(write_num_flag) + ".png";
	  std::string path2 = "resources/image/interface/chars_segment/whole.png";
	  cv::imwrite(path1, input);
	  cv::imwrite(path2, input);
	  write_num_flag++;
  }

  bool useSlideWindow = true;
  bool useAdapThreshold = true;

  for (size_t i = 0; i < newSortedRect.size(); i++) {
    Rect mr = newSortedRect[i];
    Mat auxRoi(input_grey, mr);
    Mat newRoi;

    if (i == 0) {
      if (useSlideWindow) {
        float slideLengthRatio = 0.1f;
		// 采用滑动窗口法左右微调确定chineseRect的正确位置
        if (!slideChineseWindow(input_grey, mr, newRoi, plateType, slideLengthRatio, useAdapThreshold))
	      // 选择对于中文判定效果最佳的二值化方法（OSTU或Adap），并返回用其进行二值化后输入ANN模型预测后的分数
          judgeChinese(auxRoi, newRoi, plateType);
      }
      else
        judgeChinese(auxRoi, newRoi, plateType);
    }
    else {
      if (BLUE == plateType) {  
        threshold(auxRoi, newRoi, 0, 255, CV_THRESH_BINARY + CV_THRESH_OTSU);
      }
      else if (YELLOW == plateType) {
        threshold(auxRoi, newRoi, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);
      }
	  else if (GREEN == plateType) {
		  threshold(auxRoi, newRoi, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);
	  }
      else if (WHITE == plateType) {
        threshold(auxRoi, newRoi, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY_INV);
      }
      else {
        threshold(auxRoi, newRoi, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
      }

      newRoi = preprocessChar(newRoi);
    }
     
    if (0) {
      if (i == 0) {
        imshow("input_grey", input_grey);
        waitKey(0);
        destroyWindow("input_grey");
      }
      if (i == 0) {
        imshow("newRoi", newRoi);
        waitKey(0);
        destroyWindow("newRoi");
      }
    }

    resultVec.push_back(newRoi);
  }

  return 0;
}

// 由SpecificRect得到ChineseRect
Rect CCharsSegment::GetChineseRect(const Rect rectSpe) {
  // 宽度为特殊字符的1.15倍
  int height = rectSpe.height;
  float newwidth = rectSpe.width * 1.15f;
  int x = rectSpe.x;
  int y = rectSpe.y;

  // x坐标左移中文字符宽度的1.15倍，并防止坐标为负值
  int newx = x - int(newwidth * 1.15);
  newx = newx > 0 ? newx : 0;

  Rect a(newx, y, int(newwidth), height);

  return a;
}

// 根据先验知识得到特殊字符的索引
int CCharsSegment::GetSpecificRect(const vector<Rect>& vecRect) {
  vector<int> xpositions;
  int maxHeight = 0;
  int maxWidth = 0;

  for (size_t i = 0; i < vecRect.size(); i++) {
    xpositions.push_back(vecRect[i].x);

    if (vecRect[i].height > maxHeight) {
      maxHeight = vecRect[i].height;
    }
    if (vecRect[i].width > maxWidth) {
      maxWidth = vecRect[i].width;
    }
  }

  int specIndex = 0;
  for (size_t i = 0; i < vecRect.size(); i++) {
    Rect mr = vecRect[i];
    int midx = mr.x + mr.width / 2;

    // 其外接轮廓的中心点位置在车牌宽度的 1/7 - 2/7 之间
    if ((mr.width > maxWidth * 0.8 || mr.height > maxHeight * 0.8) &&
        (midx < int(m_theMatWidth / 7) * 2 &&
         midx > int(m_theMatWidth / 7) * 1)) {
      specIndex = i;
    }
  }

  return specIndex;
}

// 整理字符顺序，后数count个字符放入outRect
int CCharsSegment::RebuildRect(const vector<Rect>& vecRect,
                               vector<Rect>& outRect, int specIndex, bool is_newEnergy) {
	int count;
	if (is_newEnergy){
		count = 7;
	}
	else{
		count = 6;
	}
		
  for (size_t i = specIndex; i < vecRect.size() && count; ++i, --count) {
    outRect.push_back(vecRect[i]);
  }

  return 0;
}

}
