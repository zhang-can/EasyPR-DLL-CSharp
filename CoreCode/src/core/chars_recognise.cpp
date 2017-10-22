#include "easypr/core/chars_recognise.h"
#include "easypr/core/character.hpp"
#include "easypr/util/util.h"

namespace easypr {

CCharsRecognise::CCharsRecognise() { m_charsSegment = new CCharsSegment(); }

CCharsRecognise::~CCharsRecognise() { SAFE_RELEASE(m_charsSegment); }

int CCharsRecognise::charsRecognise(Mat plate, std::string& plateLicense) {
  std::vector<Mat> matChars;

  int result = m_charsSegment->charsSegment(plate, matChars, 2);//zc

  //std::cout << "charsSegment:" << result << std::endl;

  if (result == 0) {
    //for (auto block : matChars) {
    //  auto character = CharsIdentify::instance()->identify(block);
    //  plateLicense.append(character.second);
    //}
    int num = matChars.size();
    for (int j = 0; j < num; j++)
    {
      Mat charMat = matChars.at(j);
      bool isChinses = false;
      float maxVal = 0;
      if (j == 0) {
        bool judge = true;
        isChinses = true;
        auto character = CharsIdentify::instance()->identifyChinese(charMat, maxVal, judge);
        plateLicense.append(character.second);
      } 
      else {
        isChinses = false;
        auto character = CharsIdentify::instance()->identify(charMat, isChinses);
        plateLicense.append(character.second);
      }
    }

  }
  if (plateLicense.size() < 7) {
    return -1;
  }

  return result;
}

int CCharsRecognise::charsRecognise(CPlate& plate, std::string& plateLicense, const int show_type) {

	int show_flag;
	int write_flag;
	int ann_write_flag;

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

	if (show_type == 5){
		ann_write_flag = 1;
	}
	else{
		ann_write_flag = 0;
	}

  std::vector<Mat> matChars;

  Mat plateMat = plate.getPlateMat();

  Color color;
  //zc
  bool is_newEnergy = plate.getIsNewEnergy();
  // 获取车牌颜色
  if (plate.getPlateLocateType() == CMSER) {
    color = plate.getPlateColor();
  }
  else {
    int w = plateMat.cols;
    int h = plateMat.rows;
    Mat tmpMat = plateMat(Rect_<double>(w * 0.1, h * 0.1, w * 0.8, h * 0.8));
    color = getPlateType(tmpMat, true);
  }
  // 车牌字符分割
  int result = m_charsSegment->charsSegment(plateMat, matChars, show_type, color, is_newEnergy);

  if (result == 0) {
    //for (auto block : matChars) {
    //  auto character = CharsIdentify::instance()->identify(block);
    //  plateLicense.append(character.second);
    //}
    int num = matChars.size();
    for (int j = 0; j < num; j++)
    {
      Mat charMat = matChars.at(j);
      bool isChinese = false;   
      //if (j == 0)
      //  isChinses = true;
      //auto character = CharsIdentify::instance()->identify(charMat, isChinses);
      //plateLicense.append(character.second);

	  //结果保存图片
	  cv::Mat resultMat = matChars[j];
	  cv::Mat resultMat_reverse;

	  //结果反色
	  cv::bitwise_not(resultMat, resultMat_reverse);

	  //调整结果分辨率
	  //cv::Mat dst;
	  //cv::resize(resultMat_reverse, dst, cv::Size(50, 60), (0, 0), (0, 0), cv::INTER_NEAREST);

	  if (write_flag){
		  std::string path = "resources/image/interface/chars_segment/" + std::to_string(j) + ".png";
		  cv::imwrite(path, resultMat_reverse);
	  }
	  
      std::pair<std::string, std::string> character;
      float maxVal;
      if (j == 0) {
        isChinese = true;
        bool judge = true;
		// 中文字符识别
        character = CharsIdentify::instance()->identifyChinese(charMat, maxVal, judge);
        plateLicense.append(character.second);
		if (ann_write_flag){
			std::string path = "resources/dataset/ann/" + character.first + "/" + std::to_string(utils::getTimestamp()) + ".jpg";
			imwrite(path, charMat);
		}
      }
      else {
        isChinese = false;
		// 非中文字符识别
        character = CharsIdentify::instance()->identify(charMat, isChinese);
        plateLicense.append(character.second);
		if (ann_write_flag){
			std::string path = "resources/dataset/ann/" + character.first + "/" + std::to_string(utils::getTimestamp()) + ".jpg";
			imwrite(path, charMat);
		}
      }

      CCharacter charResult;
      charResult.setCharacterMat(charMat);
      charResult.setCharacterStr(character.second);

      plate.addReutCharacter(charResult);
    }
    if (plateLicense.size() < 7) {
      return -1;
    }
  }
  
  return result;
}


}