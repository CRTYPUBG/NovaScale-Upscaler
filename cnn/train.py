import torch
import torch.nn as nn
import torch.optim as optim
from model import NovaSRTiny

def train():
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model = NovaSRTiny(upscale_factor=2).to(device)
    criterion = nn.MSELoss()
    optimizer = optim.Adam(model.parameters(), lr=1e-4)

    print(f"Starting training on {device}...")
    # Training loop would go here (Dataset loading, etc.)
    
    # Save dummy weights
    torch.save(model.state_dict(), "novascale_weights.pth")
    print("Training complete. Weights saved.")

if __name__ == "__main__":
    train()
