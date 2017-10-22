#ifndef EASYPR_CHARS_HPP
#define EASYPR_CHARS_HPP

namespace easypr {

namespace demo {

int test_chars_segment() {
  std::cout << "test_chars_segment" << std::endl;
  string file_name = "resources/image/chars_segment/";
  string input_file_name;
  std::cout << "please input ____.jpg: " << endl;
  std::cin >> input_file_name;
  file_name += input_file_name + ".jpg";
  cv::Mat src = cv::imread(file_name);
  

  std::vector<cv::Mat> resultVec;
  CCharsSegment plate;

  int result = plate.charsSegment(src, resultVec, 2);//zc
  if (result == 0) {
    size_t num = resultVec.size();
    for (size_t j = 0; j < num; j++) {
      cv::Mat resultMat = resultVec[j];
	  cv::Mat resultMat_reverse;
	  
	  //结果反色
	  cv::bitwise_not(resultMat, resultMat_reverse);

	  //调整结果分辨率
	  //cv::Mat dst;
	  //cv::resize(resultMat_reverse, dst, cv::Size(50, 60), (0, 0), (0, 0), cv::INTER_NEAREST);

	  //结果保存图片
	  string path = "resources/image/chars_segment/result/" + to_string(j) + ".jpg";
	  cv::imwrite(path, resultMat_reverse);

	  //显示结果
	  //cv::imshow("chars_segment", resultMat_reverse);
      //cv::waitKey(0);
    }
    //cv::destroyWindow("chars_segment");
	std::cout << "chars segment result saved successfully!" << std::endl;
  }

  return result;
}

int test_chars_identify() {
  std::cout << "test_chars_identify" << std::endl;

  cv::Mat plate = cv::imread("resources/image/chars_identify.jpg");

  std::vector<Mat> matChars;
  std::string license;

  CCharsSegment cs;

  int result = cs.charsSegment(plate, matChars, 2);//zc
  if (result == 0) {
    for (size_t block_i = 0; block_i < matChars.size(); block_i++) {
      auto block = matChars[block_i];
      if (block_i == 0) {       
        auto character = CharsIdentify::instance()->identify(block, true);
        license.append(character.second);
      }
      else {
        auto character = CharsIdentify::instance()->identify(block, false);
        license.append(character.second);
      }
    }
    //for (auto block : matChars) {
    //  auto character = CharsIdentify::instance()->identify(block);
    //  license.append(character.second);
    //}
  }

  std::string plateLicense = "苏E771H6";
#ifdef OS_WINDOWS
  plateLicense = utils::utf8_to_gbk(plateLicense.c_str());
#endif
  std::cout << "plateLicense: " << plateLicense << std::endl;
  std::cout << "plateIdentify: " << license << std::endl;

  if (plateLicense != license) {
    std::cout << "Identify Not Correct!" << std::endl;
    return -1;
  }
  std::cout << "Identify Correct!" << std::endl;

  return result;
}

int test_chars_recognise() {
  std::cout << "test_chars_recognise" << std::endl;

  cv::Mat src = cv::imread("resources/image/chars_recognise.jpg");
  CCharsRecognise cr;
  return 0;
  std::string plateLicense = "";
  int result = cr.charsRecognise(src, plateLicense);
  if (result == 0)
    std::cout << "charsRecognise: " << plateLicense << std::endl;
  return 0;
}
}
}

#endif  // EASYPR_CHARS_HPP
