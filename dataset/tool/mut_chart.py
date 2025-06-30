import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import tkinter as tk
from tkinter import filedialog
import os

# 创建Tkinter根窗口（隐藏）
root = tk.Tk()
root.withdraw()

# 弹出文件选择对话框
file_paths = filedialog.askopenfilenames(
    title="选择CSV文件",
    filetypes=[("CSV文件", "*.csv"), ("所有文件", "*.*")]
)

# 检查用户是否选择了文件
if not file_paths:
    print("未选择文件，程序退出")
    exit()

# 存储数据和文件名
trajectories = []
colors = ['blue', 'red', 'green', 'purple', 'orange']  # 不同轨迹的颜色
labels = ['GNSS', 'INS', 'Trajectory 3', 'Trajectory 4', 'Trajectory 5']  # 图例标签

# 读取所有选择的文件
for i, file_path in enumerate(file_paths):
    try:
        # 读取CSV文件
        data = pd.read_csv(file_path, delim_whitespace=True)
        
        # 检查必要的列是否存在
        required_cols = ['lon(deg)', 'lat(deg)']
        if not all(col in data.columns for col in required_cols):
            print(f"文件 {os.path.basename(file_path)} 缺少必要的列: {required_cols}")
            continue
            
        # 添加到轨迹列表
        trajectories.append({
            'data': data,
            'label': f"{labels[i] if i < len(labels) else 'Trajectory'} ({os.path.basename(file_path)})",
            'color': colors[i] if i < len(colors) else 'black',
            'file_name': os.path.basename(file_path)
        })
        
        print(f"成功加载: {os.path.basename(file_path)} ({len(data)}行)")
        
    except Exception as e:
        print(f"读取文件 {os.path.basename(file_path)} 失败: {e}")

# 检查是否有有效数据
if not trajectories:
    print("没有有效数据可绘制")
    exit()

# 创建图形
plt.figure(figsize=(12, 10), dpi=100)

# 准备计算整体范围
all_lons = pd.Series(dtype=float)
all_lats = pd.Series(dtype=float)

# 绘制所有轨迹
for traj in trajectories:
    data = traj['data']
    # 绘制轨迹
    if 'vel_n(m/s)' in data.columns:  # 可能是INS数据，绘制连续线
        plt.plot(data['lon(deg)'], data['lat(deg)'], 
                 color=traj['color'], linewidth=2.0, alpha=0.8, label=traj['label'])
    else:  # 可能是GNSS数据，绘制带标记的点
        plt.plot(data['lon(deg)'], data['lat(deg)'], 
                 'o-', color=traj['color'], markersize=6, linewidth=1.5, alpha=0.8, label=traj['label'])
    
    # 收集数据范围
    all_lons = pd.concat([all_lons, data['lon(deg)']])
    all_lats = pd.concat([all_lats, data['lat(deg)']])

# 设置视图范围（如果有足够的数据点）
if len(all_lons) > 0 and len(all_lats) > 0:
    # 计算中心点
    center_lon = all_lons.mean()
    center_lat = all_lats.mean()
    
    # 计算范围并添加20%的边界
    lon_range = (all_lons.max() - all_lons.min()) * 1.2
    lat_range = (all_lats.max() - all_lats.min()) * 1.2
    
    # 应用缩放（确保范围不为零）
    if lon_range > 0 and lat_range > 0:
        plt.xlim(center_lon - lon_range/2, center_lon + lon_range/2)
        plt.ylim(center_lat - lat_range/2, center_lat + lat_range/2)
    else:
        # 如果范围太小，使用默认视图
        print("数据范围太小，使用自动缩放")
else:
    print("没有有效的位置数据")

# 添加图例和标签
plt.legend(loc='best', fontsize=10)
plt.xlabel('Longitude (deg)', fontsize=12)
plt.ylabel('Latitude (deg)', fontsize=12)

# 设置标题（如果有多个文件显示文件数量）
if len(trajectories) == 1:
    plt.title(f'Trajectory: {trajectories[0]["file_name"]}', fontsize=15)
else:
    plt.title(f'Trajectory Comparison ({len(trajectories)} files)', fontsize=15)

# 添加网格
plt.grid(True, linestyle='--', alpha=0.6)

# 优化布局
plt.tight_layout()

# 显示图像
plt.show()
