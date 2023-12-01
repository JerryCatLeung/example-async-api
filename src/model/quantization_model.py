import onnx
from onnxruntime.quantization import quantize_dynamic, QuantType, quantize_static

onnx_model_fp32 = 'model.onnx'

# 检查模型
model = onnx.load(onnx_model_fp32)
onnx.checker.check_model(model)
print(onnx.helper.printable_graph(model.graph))

# 模型量化
onnx_model_uint8 = 'model_uint8.onnx'
quantize_dynamic(onnx_model_fp32, onnx_model_uint8, weight_type=QuantType.QUInt8)


# 检查量化模型
model = onnx.load(onnx_model_uint8)
onnx.checker.check_model(model)
print(onnx.helper.printable_graph(model.graph))
