import torch
from model import NovaSRTiny

def export():
    model = NovaSRTiny(upscale_factor=2)
    model.eval()
    
    # Dummy input (e.g. 720p)
    dummy_input = torch.randn(1, 3, 720, 1280)
    
    torch.onnx.export(model, dummy_input, "novascale_ultra.onnx", 
                      opset_version=11, 
                      input_names=['input'], 
                      output_names=['output'],
                      dynamic_axes={'input': {2: 'height', 3: 'width'}})
    print("Model exported to novascale_ultra.onnx")

if __name__ == "__main__":
    export()
