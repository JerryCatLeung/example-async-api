
#include "MyController.hpp"

const char* MyController::model_path = "/home/llm/deeplearning-inference/example-async-api/src/model/model_uint8.onnx";
OnnxRuntimePredictor* MyController::m_predictor = new OnnxRuntimePredictor(MyController::model_path);
// TODO some code here