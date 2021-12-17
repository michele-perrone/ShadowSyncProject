def default_handler(address, *args):
    print(f"DEFAULT {address}: {args}")

def pose_handler(address, *args):
    # global_model.parameter...
    print(f"{address}: {args}")