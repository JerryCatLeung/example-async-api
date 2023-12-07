#include "onnxruntime/core/session/onnxruntime_cxx_api.h"
#include <assert.h>
#include <iostream>

class OnnxRuntimePredictor {
private:
  Ort::Env env;
  Ort::SessionOptions session_options;
  Ort::Session session;
  std::vector<const char*> input_node_names = {"feat_ids", "feat_vals"};
  std::vector<const char*> output_node_names = {"prob"};
  std::vector<int64_t> input_node_dims = {15, 39};
  size_t input_tensor_size = 15 * 39;

public:
  OnnxRuntimePredictor(const char* modelPath)
    : env(ORT_LOGGING_LEVEL_WARNING, "Model"),
      session(env, modelPath, session_options) {
        // 初始化session_options
        session_options.SetIntraOpNumThreads(10);
        session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);
      }
  
  std::vector<float> predictor(std::vector<int64_t>& featIdsTensorValues,
                   std::vector<float>& featValuesTensorValues){
    
    auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Ort::Value feat_ids_input_tensor = Ort::Value::CreateTensor<int64_t>(memory_info, 
                                                                        featIdsTensorValues.data(), input_tensor_size,
                                                                        input_node_dims.data(), 2);
    Ort::Value feat_vals_input_tensor = Ort::Value::CreateTensor<float>(memory_info, 
                                                                        featValuesTensorValues.data(), input_tensor_size, 
                                                                        input_node_dims.data(), 2);
    assert(feat_ids_input_tensor.IsTensor());
    assert(feat_vals_input_tensor.IsTensor());

    std::vector<Ort::Value> ort_inputs;
    ort_inputs.push_back(std::move(feat_ids_input_tensor));
    ort_inputs.push_back(std::move(feat_vals_input_tensor));

    auto output_tensors = session.Run(Ort::RunOptions{nullptr}, input_node_names.data(), ort_inputs.data(), ort_inputs.size(), output_node_names.data(), 1);
    float* inferenceResult = output_tensors[0].GetTensorMutableData<float>();
    // Get the size of the output tensor.
    size_t output_size = output_tensors[0].GetTensorTypeAndShapeInfo().GetElementCount();
    // std::cout << output_size << std::endl;
    std::vector<float> result(inferenceResult, inferenceResult + output_size);
    return result;
  }
};