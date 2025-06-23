import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import FormatStrFormatter
import matplotlib.font_manager as fm

# 设置专业字体
plt.rcParams['font.family'] = 'DejaVu Sans'  # 使用更清晰的无衬线字体
plt.rcParams['axes.titlepad'] = 20  # 增加标题与图形的间距

# 从CSV文件读取数据
df = pd.read_csv('GNSS_RTK.csv', delim_whitespace=True, comment='#')

# 数据清洗
df = df.dropna(subset=['lat(deg)', 'lon(deg)'])
df = df[(df['lat(deg)'] > 30) & (df['lat(deg)'] < 31)]
df = df[(df['lon(deg)'] > 114) & (df['lon(deg)'] < 115)]

# 创建高质量图形 - 增加高度确保标题完整显示
fig = plt.figure(figsize=(14, 10), dpi=300)  # 保持宽度14英寸，高度10英寸
ax = fig.add_subplot(111)

# 计算轨迹方向变化点
df['dx'] = df['lon(deg)'].diff()
df['dy'] = df['lat(deg)'].diff()
df['direction'] = np.arctan2(df['dy'], df['dx'])

# 绘制轨迹线（带时间渐变）
sc = ax.scatter(df['lon(deg)'], df['lat(deg)'], 
                c=df['sec_week(s)'], 
                cmap='plasma', 
                s=2, 
                alpha=0.8,
                zorder=2)

# 添加轨迹线连接
ax.plot(df['lon(deg)'], df['lat(deg)'], 
        'k-', linewidth=0.3, alpha=0.4, zorder=1)

# 标记关键点
start_point = ax.scatter(df['lon(deg)'].iloc[0], df['lat(deg)'].iloc[0], 
                         s=200, c='lime', edgecolor='black', 
                         marker='*', zorder=5, label='Start Point')

end_point = ax.scatter(df['lon(deg)'].iloc[-1], df['lat(deg)'].iloc[-1], 
                       s=150, c='red', edgecolor='black', 
                       marker='s', zorder=5, label='End Point')

# 添加方向箭头（在关键转折点）
arrow_indices = []
for i in range(100, len(df), 300):
    if i < len(df) - 10:
        angle_diff = abs(df['direction'].iloc[i] - df['direction'].iloc[i-10])
        if angle_diff > 0.5:
            arrow_indices.append(i)

for idx in arrow_indices:
    if idx < len(df) - 1:
        ax.annotate('', 
                    xy=(df['lon(deg)'].iloc[idx+1], df['lat(deg)'].iloc[idx+1]),
                    xytext=(df['lon(deg)'].iloc[idx], df['lat(deg)'].iloc[idx]),
                    arrowprops=dict(arrowstyle="->", color='black', 
                                    lw=1.2, alpha=0.8, shrinkA=0, shrinkB=0))

# 添加比例尺
lat_center = df['lat(deg)'].mean()
lon_scale = 111320 * np.cos(np.radians(lat_center))
scale_bar_length = 0.001  # 代表约111米

scale_x_start = df['lon(deg)'].min() + 0.0001
scale_y = df['lat(deg)'].min() + 0.0001

ax.plot([scale_x_start, scale_x_start + scale_bar_length], 
        [scale_y, scale_y], 
        'k-', linewidth=3, zorder=10)
ax.text(scale_x_start + scale_bar_length/2, scale_y - 0.00002, 
        f'{scale_bar_length*lon_scale:.0f} m', 
        ha='center', va='top', fontsize=10, fontweight='bold',
        bbox=dict(boxstyle="round,pad=0.2", fc="white", ec="k", alpha=0.9))

# 设置图形属性 - 确保标题完整显示
ax.set_title('Precision Positioning Data', 
             fontsize=18, fontweight='bold', pad=20)  # 增加pad值
ax.set_xlabel('Longitude (degrees)', fontsize=13, labelpad=10)
ax.set_ylabel('Latitude (degrees)', fontsize=13, labelpad=10)
ax.xaxis.set_major_formatter(FormatStrFormatter('%.4f'))
ax.yaxis.set_major_formatter(FormatStrFormatter('%.4f'))
ax.grid(True, linestyle='--', alpha=0.5)

# 添加高程信息颜色条
cbar = fig.colorbar(sc, ax=ax, shrink=0.8)
cbar.set_label('GPS Time (seconds of week)', fontsize=11)
cbar.ax.tick_params(labelsize=9)

# 添加图例和信息框
legend = ax.legend(loc='upper left', fontsize=10, framealpha=0.9)
legend.get_frame().set_linewidth(0.5)

# 计算统计信息
duration = df['sec_week(s)'].iloc[-1] - df['sec_week(s)'].iloc[0]
distance_est = len(df) * 0.5  # 简化距离估计

info_text = (f"Data Points: {len(df):,}\n"
             f"Duration: {duration:.0f} seconds\n"
             f"Distance: ~{distance_est:.0f} meters\n"
             f"Avg. Speed: {distance_est/duration:.2f} m/s")

ax.annotate(info_text, xy=(0.98, 0.98), xycoords='axes fraction',
            ha='right', va='top', fontsize=10,
            bbox=dict(boxstyle="round,pad=0.4", fc="white", ec="gray", alpha=0.8))

# 设置等比例坐标轴
ax.set_aspect('equal', adjustable='datalim')

# 调整布局确保标题完整显示
plt.tight_layout(rect=[0, 0, 1, 0.96])  # 为标题保留顶部空间

# 保存和显示图像
plt.savefig('gnss_trajectory.png', dpi=300, bbox_inches='tight')
plt.show()
