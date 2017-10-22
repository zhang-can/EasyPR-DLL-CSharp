#include "easypr/train/svm_train.h"
#include "easypr/core/feature.h"
#include "easypr/util/util.h"
#include <fstream>

#ifdef OS_WINDOWS
#include <ctime>
#endif

using namespace cv::ml;

namespace easypr {

SvmTrain::SvmTrain(const char* plates_folder, const char* xml)
    : plates_folder_(plates_folder), svm_xml_(xml) {
  assert(plates_folder);
  assert(xml);
}

void SvmTrain::train() {
  svm_ = cv::ml::SVM::create();
  svm_->setType(cv::ml::SVM::C_SVC);
  svm_->setKernel(cv::ml::SVM::RBF);
  svm_->setDegree(0.1);
  svm_->setGamma(0.1); 
  svm_->setCoef0(0.1);
  svm_->setC(1);
  svm_->setNu(0.1);
  svm_->setP(0.1);
  svm_->setTermCriteria(cvTermCriteria(CV_TERMCRIT_ITER, 20000, 0.0001));

  // 训练数据构造
  auto train_data = tdata();

  fprintf(stdout, ">> Training SVM model, please wait...\n");
  long start = utils::getTimestamp();
  //svm_->trainAuto(train_data, 10, SVM::getDefaultGrid(SVM::C),
  //                SVM::getDefaultGrid(SVM::GAMMA), SVM::getDefaultGrid(SVM::P),
  //                SVM::getDefaultGrid(SVM::NU), SVM::getDefaultGrid(SVM::COEF),
  //                SVM::getDefaultGrid(SVM::DEGREE), true);
  // 模型训练
  svm_->train(train_data);

  long end = utils::getTimestamp();
  fprintf(stdout, ">> Training done. Time elapse: %ldms\n", end - start);
  fprintf(stdout, ">> Saving model file...\n");
  svm_->save(svm_xml_);

  fprintf(stdout, ">> Your SVM Model was saved to %s\n", svm_xml_);
  fprintf(stdout, ">> Testing...\n");

  // 测试模型
  this->test();
  
}

void SvmTrain::test() {
    svm_ = cv::ml::SVM::load<cv::ml::SVM>(svm_xml_);

  if (test_file_list_.empty()) {
    this->prepare();
  }
 
  double count_all = test_file_list_.size();
  double ptrue_rtrue = 0;
  double ptrue_rfalse = 0;
  double pfalse_rtrue = 0;
  double pfalse_rfalse = 0;

  for (auto item : test_file_list_) {
    auto image = cv::imread(item.file);
    if (!image.data) {
      
      std::cout << "no" << std::endl;
      continue;
    }
    cv::Mat feature;
    getLBPFeatures(image, feature);

    auto predict = int(svm_->predict(feature));
    //std::cout << "predict: " << predict << std::endl;

    auto real = item.label;
    if (predict == kForward && real == kForward) ptrue_rtrue++;
    if (predict == kForward && real == kInverse) ptrue_rfalse++;
    if (predict == kInverse && real == kForward) pfalse_rtrue++;
    if (predict == kInverse && real == kInverse) pfalse_rfalse++;
  }

  std::ofstream fout("svm_result.txt");
  fout << "count_all: " << count_all << std::endl;
  fout << "ptrue_rtrue: " << ptrue_rtrue << std::endl;
  fout << "ptrue_rfalse: " << ptrue_rfalse << std::endl;
  fout << "pfalse_rtrue: " << pfalse_rtrue << std::endl;
  fout << "pfalse_rfalse: " << pfalse_rfalse << std::endl;

  double precise = 0;
  if (ptrue_rtrue + ptrue_rfalse != 0) {
    precise = ptrue_rtrue / (ptrue_rtrue + ptrue_rfalse);
    fout << "precise: " << precise << std::endl;
  } else {
    fout << "precise: "
              << "NA" << std::endl;
  }

  double recall = 0;
  if (ptrue_rtrue + pfalse_rtrue != 0) {
    recall = ptrue_rtrue / (ptrue_rtrue + pfalse_rtrue);
    fout << "recall: " << recall << std::endl;
  } else {
    fout << "recall: "
              << "NA" << std::endl;
  }

  double Fsocre = 0;
  if (precise + recall != 0) {
    Fsocre = 2 * (precise * recall) / (precise + recall);
    fout << "Fsocre: " << Fsocre << std::endl;
  } else {
    fout << "Fsocre: "
              << "NA" << std::endl;
  }
  fout << std::flush;
  fout.close();
}

// 准备数据
void SvmTrain::prepare() {
  srand(unsigned(time(NULL)));

  char buffer[260] = {0};

  sprintf(buffer, "%s/has/train", plates_folder_);
  auto has_file_train_list = utils::getFiles(buffer);
  // 元素序列随机重排序
  std::random_shuffle(has_file_train_list.begin(), has_file_train_list.end());

  sprintf(buffer, "%s/has/test", plates_folder_);
  auto has_file_test_list = utils::getFiles(buffer);
  std::random_shuffle(has_file_test_list.begin(), has_file_test_list.end());

  sprintf(buffer, "%s/no/train", plates_folder_);
  auto no_file_train_list = utils::getFiles(buffer);
  std::random_shuffle(no_file_train_list.begin(), no_file_train_list.end());

  sprintf(buffer, "%s/no/test", plates_folder_);
  auto no_file_test_list = utils::getFiles(buffer);
  std::random_shuffle(no_file_test_list.begin(), no_file_test_list.end());

  // 构建数据
  fprintf(stdout, ">> Collecting train data...\n");

  for (auto file : has_file_train_list)
    train_file_list_.push_back({ file, kForward });

  for (auto file : no_file_train_list)
    train_file_list_.push_back({ file, kInverse });

  fprintf(stdout, ">> Collecting test data...\n");

  for (auto file : has_file_test_list)
    test_file_list_.push_back({ file, kForward });

  for (auto file : no_file_test_list)
    test_file_list_.push_back({ file, kInverse });
}

cv::Ptr<cv::ml::TrainData> SvmTrain::tdata() {
  this->prepare();

  cv::Mat samples;
  std::vector<int> responses;

  for (auto f : train_file_list_) {
    auto image = cv::imread(f.file);
    if (!image.data) {
      fprintf(stdout, ">> Invalid image: %s  ignore.\n", f.file.c_str());
      continue;
    }
    cv::Mat feature;
	// LBP特征提取
    getLBPFeatures(image, feature);
    feature = feature.reshape(1, 1);

    samples.push_back(feature);
    responses.push_back(int(f.label));//label = kForward or kInverse
  }

  cv::Mat samples_, responses_;
  samples.convertTo(samples_, CV_32FC1);
  cv::Mat(responses).copyTo(responses_);

  return cv::ml::TrainData::create(samples_, cv::ml::SampleTypes::ROW_SAMPLE,
                                   responses_);
}

}  // namespace easypr
